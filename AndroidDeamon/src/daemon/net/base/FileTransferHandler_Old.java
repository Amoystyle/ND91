package daemon.net.base;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.util.IConstants;

import android.content.Context;

//负责文件的上传和下载
public class FileTransferHandler_Old extends PackageHandler {
	private String TAG = getClass().getName();

	private static final byte[] SYNC_TOKEN = new byte[] {(byte) 'S', (byte) 'Y', (byte) 'N', (byte) 'C'};

	private static final byte[] SYNC_END = new byte[] {(byte) 'S', (byte) 'E', (byte) 'N', (byte) 'D'};

	public static int PUSH_FILE_ID = 22;
	public static int PULL_FILE_ID = 23;

	private final int TIME_OUT = 10 * 1000;

	private FileChannel mFileChannel = null;
	private FileOutputStream mOutputStream = null;
	private String mFilePath = "";
	private String mTmpFilePath = "";
	private long mFileSize = 0;
	private boolean mIsCancel = false;

	private FileInputStream mInputStream = null;

	public FileTransferHandler_Old(Context context, PackageHead head, Peer peer) {
		super(context, head, peer);
	}

	public static boolean isFileTransfer_Old(PackageHead head) {
		if (head.getBusinessId() == PUSH_FILE_ID || head.getBusinessId() == PULL_FILE_ID)
			return true;

		return false;
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

	// 异步接收PC发送的文件
	public boolean doPushFile() {
		boolean result = false;

		ByteReader reader = getReader();
		if (reader != null) {
			mFilePath = reader.ReadString();
			if (!initFilePush())
				return false;

			ByteBuffer headBuf = ByteBuffer.allocate(IConstants.HEADER_LENGTH);
			ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE);

			int packCnt = 0;
			int packIndex = 0;
			while (true) {
				headBuf.clear();
				if (!mPeer.read(headBuf, TIME_OUT))
					break;

				DataPackage pack = readHeader(headBuf.array());
				if (pack == null || pack.isCancel()) {
					mIsCancel = true;
					break;
				}

				boolean isFirst = true;
				dataBuf.clear();
				dataBuf.limit(pack.getDataSize());

				if (mPeer.read(dataBuf, TIME_OUT)) {
					byte[] buf = dataBuf.array();
					int index = 0;
					if (isFirst) {
						packCnt = ByteReader.ReadInt32(buf, 0);
						packIndex = ByteReader.ReadInt32(buf, 4);
						index = 8;
						isFirst = false;
					}

					dataBuf.position(index);
					boolean writeSuccess = writeToFile(dataBuf);
					if (!writeSuccess) {
						result = false;
						break;
					}

					if (packIndex + 1 == packCnt) {
						result = true;
						break;
					}
				}else{
					break;
				}
			}
		}

		doEndPushFile(result);

		return result;
	}

	private DataPackage readHeader(byte[] buf) {
		if (buf[0] == IConstants.TOKEN[0] && buf[1] == IConstants.TOKEN[1] && buf[2] == IConstants.TOKEN[2] && buf[3] == IConstants.TOKEN[3]) {
			int flag = ByteReader.ReadInt32(buf, IConstants.TOKEN.length);
			int dataSize = ByteReader.ReadInt32(buf, IConstants.TOKEN.length + 4) - 4;
			int businessId = ByteReader.ReadInt32(buf, IConstants.TOKEN.length + 8);

			boolean isCancel = (flag & IConstants.PACKAGEFLAG_CancelPushFile) == IConstants.PACKAGEFLAG_CancelPushFile;
			DataPackage pack = new DataPackage(businessId, isCancel, dataSize);
			return pack;
		}
		return null;
	}

	// 将接收的文件数据保存到文件
	private boolean writeToFile(ByteBuffer buf) {
		try {
			while (buf.hasRemaining()) {
				mFileChannel.write(buf);
			}

			return true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	// 结束文件接收并返回结果给PC端
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
			isSuccess = fileTemp.getAbsoluteFile().renameTo(file.getAbsoluteFile());
		}

		if (!isSuccess) {
			fileTemp.delete();
		}

		if (!mIsCancel) {
			ByteWriter writer = new ByteWriter();
			writer.Write(IConstants.TOKEN);
			writer.Write(IConstants.PACKAGEFLAG_Server);
			writer.Write(5);
			writer.Write(PUSH_FILE_ID);
			writer.Write(isSuccess);

			writer.getBaseStream().setOffset(0);
			ByteBuffer buf = writer.getBaseStream().GetBuffer();
			mPeer.write(buf, TIME_OUT);
		}
	}

	// 初始化接收过程
	private boolean initFilePush() {
		boolean initSuccess = true;

		File file = new File(mFilePath);
		if (file.exists()) {
			initSuccess = file.delete();
		}

		if (initSuccess) {
			try {
				mTmpFilePath = getValidTmpPath(mFilePath + ".tmp");
				File f = new File(mTmpFilePath);
				f.getParentFile().mkdirs();

				mOutputStream = new FileOutputStream(f);
				mFileChannel = mOutputStream.getChannel();
			}
			catch (Exception e) {
				e.printStackTrace();
				initSuccess = false;
				doEndPushFile(false);
			}
		}

		ByteWriter writer = new ByteWriter();
		writer.Write(IConstants.TOKEN);
		writer.Write(IConstants.PACKAGEFLAG_Server);
		writer.Write(5);
		writer.Write(PUSH_FILE_ID);
		writer.Write(initSuccess);
		ByteBuffer buf = writer.getBaseStream().GetBuffer();

		if (!mPeer.write(buf, TIME_OUT)) {
			initSuccess = false;
		}

		return initSuccess;
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

	// 将本地文件异步发送到pc端
	public boolean doPullFile() {
		ByteReader reader = getReader();
		int action = reader.ReadInt32();
		if (action != 2)
			return false;

		mFilePath = reader.ReadString();
		if (!initPullFile())
			return false;

		long readSize = 0;
		ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE);

		while (readSize < mFileSize) {
			int bufSize = (int) Math.min(IConstants.SOCKET_BUFFER_SIZE, mFileSize - readSize);
			dataBuf.clear();
			dataBuf.limit(bufSize);
			if (readFromFile(dataBuf)) {
				dataBuf.flip();

				ByteWriter asyncWriter = new ByteWriter();
				asyncWriter.Write(SYNC_TOKEN);
				asyncWriter.Write(bufSize + 8);

				if (!mPeer.write(asyncWriter.getBaseStream().GetBuffer(), TIME_OUT))
					break;

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

		boolean isSuccess = readSize == mFileSize;
		isSuccess = doEndPullFile(isSuccess);
		return isSuccess;
	}

	private boolean initPullFile() {
		boolean initSuccess = false;

		File f = new File(mFilePath);
		if (f.exists()) {
			try {
				mInputStream = new FileInputStream(f);
				mFileChannel = mInputStream.getChannel();
				mFileSize = f.length();
				initSuccess = true;
			}
			catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}

		ByteWriter writer = new ByteWriter();
		writer.Write(IConstants.TOKEN);
		writer.Write(IConstants.PACKAGEFLAG_Server);
		writer.Write(4);
		writer.Write(PULL_FILE_ID);

		if (initSuccess) {
			writer.Write(SYNC_TOKEN);
			writer.Write(21); // head + int + int + bool + long
			writer.Write(2); // action
			writer.Write(true);
			writer.Write(mFileSize);
			initSuccess = mPeer.write(writer, TIME_OUT);
		}
		else {
			writer.Write(SYNC_TOKEN);
			writer.Write(5); // bool + int
			writer.Write(2); // action
			writer.Write(false);
			writer.Write(SYNC_END);
			mPeer.write(writer, TIME_OUT);
		}

		return initSuccess;
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

	private boolean doEndPullFile(boolean isSuccess) {
		try {
			mFileChannel.close();
			mInputStream.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		if (isSuccess) {
			ByteWriter writer = new ByteWriter();
			writer.Write(SYNC_END);
			return mPeer.write(writer.getBaseStream().GetBuffer(), TIME_OUT);
		}

		return true;
	}

	class DataPackage {

		private int mBusinessId = 0;
		private boolean mIsCancel = false;
		private int mDataSize = 0;

		public DataPackage(int businessId, boolean isCancel, int dataSize) {
			mBusinessId = businessId;
			mIsCancel = isCancel;
			mDataSize = dataSize;
		}

		public int getBusinessId() {
			return mBusinessId;
		}

		public boolean isCancel() {
			return mIsCancel;
		}

		public int getDataSize() {
			return mDataSize;
		}

	}

}
