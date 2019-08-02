package daemon.net.base;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.util.IConstants;

import android.content.Context;

public class FileTransferHandler extends PackageHandler {
	private String TAG = getClass().getName();

	public static final int PUSH_FILE_ID = 201;
	public static final int PULL_FILE_ID = 202;

	private final byte[] ID_DATA = {68, 65, 84, 65};
	private final byte[] ID_DONE = {68, 79, 78, 69};
	private final byte[] ID_OKAY = {79, 75, 65, 89};
	private final byte[] ID_FAIL = {70, 65, 73, 76};
	private final byte[] ID_LOFS = {76, 79, 70, 83}; // lake of space

	private final int TIME_OUT = 60 * 1000;

	private FileChannel mFileChannel = null;
	private FileInputStream mInputStream = null;
	private FileOutputStream mOutputStream = null;
	private String mFilePath = "";
	private String mTmpFilePath = "";
	private long mFileSize = 0;

	public static boolean isFileTransfer(PackageHead head) {
		if (head.getBusinessId() == PUSH_FILE_ID || head.getBusinessId() == PULL_FILE_ID)
			return true;

		return false;
	}

	public FileTransferHandler(Context context, PackageHead head, Peer peer) {
		super(context, head, peer);
	}

	@Override
	public boolean handle() {
		if (mHead.getBusinessId() == PUSH_FILE_ID) {
			return doPushFile();
		}
		else if (mHead.getBusinessId() == PULL_FILE_ID) {
			return doPullFile();
		}

		return false;
	}

	private boolean doPushFile() {
		boolean result = false;
		ByteReader reader = getReader();
		if (reader != null) {
			mFilePath = reader.ReadString();
			mFileSize = reader.ReadInt64();

			boolean initSuccess = initFilePush();
			if (!initSuccess)
				return false;

			ByteBuffer idBuf = ByteBuffer.allocate(4);
			ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE);
			while (true) {
				idBuf.clear();
				if (mPeer.read(idBuf, TIME_OUT)) {
					if (checkDataId(idBuf, ID_DATA)) {
						idBuf.clear();
						if (mPeer.read(idBuf, TIME_OUT)) {
							int dataSize = readInt32(idBuf);
							if (dataSize > dataBuf.capacity()) // 如果数据大小比默认缓冲区大，重新分配缓冲区
								dataBuf = ByteBuffer.allocate(dataSize);

							dataBuf.rewind();
							dataBuf.limit(dataSize);
							if (mPeer.read(dataBuf, TIME_OUT)) {
								dataBuf.flip();
								if (!writeToFile(dataBuf))
									break;
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}
					else if (checkDataId(idBuf, ID_DONE)) {
						if (checkFile(mFileSize)) {
							result = true;
						}

						break;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
		}

		doEndPushFile(result);
		return result;
	}

	private boolean checkDataId(ByteBuffer buf, byte[] dataId) {
		byte[] array = buf.array();
		if (array.length != dataId.length)
			return false;

		for (int i = 0; i < dataId.length; i++) {
			if (array[i] != dataId[i])
				return false;
		}

		return true;
	}

	private boolean initFilePush() {
		File file = new File(mFilePath);
		if (file.exists()) {
			if (!file.delete()) {
				sendFail();
				return false;
			}
		}

		try {
			mTmpFilePath = getValidTmpPath(mFilePath + ".tmp");
			File f = new File(mTmpFilePath);
			f.getParentFile().mkdirs();

			mOutputStream = new FileOutputStream(f);
			mFileChannel = mOutputStream.getChannel();

			if (sendOKAY()) {// 返回初始化成功标志
				return true;
			}
			else {
				doEndPushFile(false);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	private boolean checkFile(long fileSize) {
		try {
			mOutputStream.flush();
		}
		catch (IOException e) {
			e.printStackTrace();
		}

		File f = new File(mTmpFilePath);
		if (f.exists() && f.length() == fileSize)
			return true;

		sendFail();
		return false;
	}

	private String getValidTmpPath(String originalPath) {
		File f = new File(originalPath);
		if (!f.exists()) {
			return originalPath;
		}
		else {
			int index = 1;
			String newPath = originalPath + index;
			f = new File(newPath);
			while (f.exists()) {
				index++;
				newPath = originalPath + index;
				f = new File(newPath);
			}

			return newPath;
		}
	}

	private void doEndPushFile(boolean isSuccess) {
		try {
			mOutputStream.flush();
			mFileChannel.close();
			mOutputStream.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		File fileTemp = new File(mTmpFilePath);
		if (isSuccess) {
			File file = new File(mFilePath);
			fileTemp.getAbsoluteFile().renameTo(file.getAbsoluteFile());
			sendOKAY();

		}
		else {
			fileTemp.delete();
		}
	}

	private boolean writeToFile(ByteBuffer buf) {
		try {
			while (buf.hasRemaining()) {
				mFileChannel.write(buf);
			}

			return true;
		}
		catch (Exception e) {
			e.printStackTrace();

			String msg = e.getMessage();
			if (msg != null && msg.toLowerCase().contains("no space left on device"))
				sendLOFS();
		}

		return false;
	}

	private int readInt32(ByteBuffer buf) {
		byte[] array = buf.array();
		int value = 0;
		value |= ((int) array[0]) & 0x000000FF;
		value |= (((int) array[1]) & 0x000000FF) << 8;
		value |= (((int) array[2]) & 0x000000FF) << 16;
		value |= (((int) array[3]) & 0x000000FF) << 24;

		return value;
	}

	private boolean sendFail() {
		ByteBuffer buf = ByteBuffer.allocate(ID_OKAY.length);
		buf.put(ID_FAIL);
		buf.flip();
		return mPeer.write(buf, TIME_OUT);
	}

	private boolean sendOKAY() {
		ByteBuffer buf = ByteBuffer.allocate(ID_OKAY.length);
		buf.put(ID_OKAY);
		buf.flip();
		return mPeer.write(buf, TIME_OUT);
	}

	private boolean sendDONE() {
		ByteBuffer buf = ByteBuffer.allocate(ID_DONE.length);
		buf.put(ID_DONE);
		buf.flip();
		return mPeer.write(buf, TIME_OUT);
	}

	private boolean sendLOFS() {
		ByteBuffer buf = ByteBuffer.allocate(ID_LOFS.length);
		buf.put(ID_LOFS);
		buf.flip();
		boolean result = mPeer.write(buf, TIME_OUT);

		//发回空间不足信息后，pc端得等写入超时才读取错误信息，因此休眠20秒后再关闭socket，否则pc端接收不到
		try {
			Thread.sleep(12000);
		}
		catch (InterruptedException e) {		
			e.printStackTrace();
		}
		
		mPeer.close();
		
		return result;
	}

	private boolean doPullFile() {
		boolean result = false;

		ByteReader reader = getReader();
		if (reader != null) {
			mFilePath = reader.ReadString();

			if (!initPullFile())
				return false;

			long readSize = 0;
			ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE + 8);
			dataBuf.put(ID_DATA);

			while (readSize < mFileSize) {
				dataBuf.position(4);
				int bufSize = (int) Math.min(IConstants.SOCKET_BUFFER_SIZE, mFileSize - readSize);
				ByteBuffer sizeBuf = ByteBuffer.allocate(4);
				writeInt32(sizeBuf, bufSize);
				sizeBuf.flip();
				dataBuf.put(sizeBuf);
				dataBuf.limit(bufSize + 8);

				if (readFromFile(dataBuf)) {
					dataBuf.flip();
					if (mPeer.write(dataBuf, TIME_OUT)) {
						readSize += bufSize;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}

			if (readSize == mFileSize) {
				result = sendDONE();
			}
		}

		doEndPullFile();
		return result;
	}

	private boolean initPullFile() {
		LogCenter.error(TAG, "pull file:" + mFilePath);
		File f = new File(mFilePath);
		if (f.exists()) {
			try {
				mInputStream = new FileInputStream(f);
				mFileChannel = mInputStream.getChannel();
				mFileSize = f.length();

				ByteWriter writer = new ByteWriter(12);
				writer.Write(ID_OKAY);
				writer.Write(mFileSize);
				return mPeer.write(writer.getBaseStream().GetBuffer(), TIME_OUT);
			}
			catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}
		else {
			LogCenter.error(TAG, "pull file error: file not found:" + mFilePath);
			sendFail();
		}

		return false;
	}

	private boolean readFromFile(ByteBuffer buf) {
		try {
			while (buf.hasRemaining()) {
				mFileChannel.read(buf);
			}

			return true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	private void doEndPullFile() {
		try {
			mFileChannel.close();
			mInputStream.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void writeInt64(ByteBuffer bb, long value) {
		byte[] buf = new byte[8];
		if (value != 0) {
			for (int index = 0; index < 8; index++) {
				buf[index] = (byte) (value >>> (8 * index) & 0xFF);
			}
		}

		bb.put(buf);
	}

	private void writeInt32(ByteBuffer bb, int value) {
		byte[] buf = new byte[4];
		if (value != 0) {
			for (int index = 0; index < 4; index++) {
				buf[index] = (byte) (value >>> (8 * index) & 0xFF);
			}
		}

		bb.put(buf);
	}
}
