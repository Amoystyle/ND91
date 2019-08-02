package daemon.net.base;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import daemon.common.MountPartitionParam;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.provider.file.FileOperater;
import daemon.util.FileHelper;
import daemon.util.FileInfo;
import daemon.util.IConstants;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.StatFs;
import android.text.TextUtils;

@SuppressLint("DefaultLocale")
public class FileTransferHandler_New extends PackageHandler {
	private String TAG = getClass().getName();

	public static final int PUSH_FILE_ID = 203;
	public static final int PULL_FILE_ID = 204;

	private final byte[] ID_DATA = {68, 65, 84, 65};
	private final byte[] ID_DONE = {68, 79, 78, 69};
	private final byte[] ID_OKAY = {79, 75, 65, 89};
	private final byte[] ID_FAIL = {70, 65, 73, 76};
	private final byte[] ID_LOFS = {76, 79, 70, 83}; // lake of space


	private final int FOR_FAILED = -1; //未知错误
    private final int FOR_FILE_NO_FOUND = 10002; //文件不存在
    private final int FOR_PERMISSION_DENIED = 10013; //没权限
    private final int FOR_LACK_OF_SPACE = 10028; //空间不足
    private final int FOR_READ_ONLY = 10030; //只读文件系统
    private final int FOR_NAME_TOOLONG = 10036; //文件名字过长
    private final int FOR_FILE_COVER_FAIL = 11001;//覆盖（删除）失败 
    private final int FOR_CHECK_FAIL = 11002; //文件校验失败
    
  
	
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

	public FileTransferHandler_New(Context context, PackageHead head, Peer peer) {
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
		boolean writeByNdsh =false;
		ByteReader reader = getReader();
		long time = System.currentTimeMillis();
		if (reader != null) {
			mFilePath = reader.ReadString();
			mFileSize = reader.ReadInt64();
			
			File file =new File(mFilePath);
			
			if(FileHelper.isFileCanWriteByJava(file.getParent())){
				writeByNdsh =false;
			}else if(FileHelper.isFileCanWriteByNdsh(mContext, file.getParent())){
				writeByNdsh =true;
			}else if (NdkManager.isNdshActualRunning(mContext)) {
			    FileHelper.mount(mContext,file);
				writeByNdsh = FileHelper.isFileCanWriteByNdsh(mContext, file.getParent());
			}
			
			boolean initSuccess =false;
			if(writeByNdsh){
				initSuccess = initFilePushByNdsh();
			}else{
				initSuccess = initFilePush();
			}
			if (!initSuccess)
				return false;
			
			int index = mFilePath.lastIndexOf("/");
			if(index!=-1){
				String dirpath = mFilePath.substring(0, index);
				File dir = new File(dirpath);
				if(!dir.exists()){
					if(!dir.mkdirs()){
						sendFail(FOR_FILE_NO_FOUND);
						return false;
					}
				}
				if(!checkFileSize(dirpath,mFileSize)){
					sendFail(FOR_LACK_OF_SPACE);
					return false;
				}
			}else{
				sendFail(FOR_FILE_NO_FOUND);
				return false;
			}
			ByteBuffer idBuf = ByteBuffer.allocate(4);
			ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE);
			long lasttime =0;
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
							int count = mPeer.readEx(dataBuf, TIME_OUT);
							if (count>0) {
								dataBuf.flip();
								if(writeByNdsh){
									if (!writeToFileByndsh(dataBuf,count))
										break;
								}else{
									if (!writeToFile(dataBuf))
										break;
								}

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
						if (checkFile(mFileSize,writeByNdsh)) {
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
        if(writeByNdsh){
        	doEndPushFileByNdsh(result);
        }else{
        	doEndPushFile(result);
        }
		if(NdkManager.isNdshActualRunning(mContext)){
			if(!FileHelper.isSdcardFile(mFilePath))
			   NdkManager.execShell(mContext,"chmod 0777 "+mFilePath);
		}
		
		return result;
	}

	private boolean checkFileSize(String path, long size) {
		long free = getFreeSpace(path);
		return size <= free;
	}
	private long getFreeSpace(String  path) {
		try {
			StatFs sf = new StatFs(path);
			long blockCount = sf.getAvailableBlocks();
			long blockSize = sf.getBlockSize();
			long result = blockCount * blockSize;
			return result > 0 ? result : 0;
		} catch (Exception e) {
			return 0;
		}

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
	
	
	
	private boolean initFilePushByNdsh() {
		
		if (FileHelper.isFileExitByNdsh(mContext, mFilePath)) {
			if (!FileHelper.removeFileByNdsh(mContext, mFilePath)) {
				sendFail(FOR_FILE_COVER_FAIL);
				return false;
			}
		}
	
		try {
			mTmpFilePath = getValidTmpPath(mFilePath + ".tmp");
			boolean result = NdkManager.makeFile(mContext, mTmpFilePath);
			if(!result){
				sendFail(FOR_FAILED);
				return false;
			}

			if (sendOKAY()) {// 返回初始化成功标志
				return true;
			}
			else {
				doEndPushFileByNdsh(false);
			}
		}
		catch (Exception e) {
			checkException(e);	
		}

		return false;
	}
	
	
	

	private boolean initFilePush() {
		File file = new File(mFilePath);
		if (file.exists()) {
			if (!file.delete()) {
				sendFail(FOR_FILE_COVER_FAIL);
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
			checkException(e);	
		}

		return false;
	}
	

	private boolean checkFile(long fileSize,boolean byNdsh) {
		if(byNdsh){
	        FileInfo info = FileHelper.getFileInfo(mContext, mTmpFilePath);
	        if(info.isExist() && info.getSize()== fileSize)
				return true;
		}else{
			try {
				mOutputStream.flush();
			}
			catch (IOException e) {
				e.printStackTrace();
			}

			File f = new File(mTmpFilePath);
			if (f.exists() && f.length() == fileSize)
				return true;
		}
		sendFail(FOR_CHECK_FAIL);
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
	
	
	private void doEndPushFileByNdsh(boolean isSuccess) {	
		if (isSuccess) {
			NdkManager.renameFile(mContext, mTmpFilePath, mFilePath);
			sendOKAY();
		}
		else {
			FileHelper.removeFileByNdsh(mContext, mTmpFilePath);
		}
	}
	

	
	
	
	private boolean writeToFileByndsh(ByteBuffer buf,int count) {
		try {
			boolean result = NdkManager.writeFile(mContext, mTmpFilePath,buf.array(),count);
			return result;
		}
		catch (Exception e) {
			checkException(e);				
		}
		return false;
	}
	
	private boolean writeToFile(ByteBuffer buf) {
		try {
			while (buf.hasRemaining()) {
				mFileChannel.write(buf);
			}
			return true;
		}
		catch (Exception e) {
			checkException(e);				
		}
		return false;
	}
	
	
	private void checkException(Exception e){
		e.printStackTrace();
		String msg = e.getMessage();
		if (msg != null ){
			String errmsg = msg.toLowerCase();
			if(errmsg.contains("no space left on device")){
				sendFail(FOR_LACK_OF_SPACE);
			}else if(errmsg.contains("read-only file system")){
				sendFail(FOR_READ_ONLY);
			}else if(errmsg.contains("permission denied")){
				sendFail(FOR_PERMISSION_DENIED);
			}else if(errmsg.contains("no such file or directory")){
				sendFail(FOR_FILE_NO_FOUND);
			}else if(errmsg.contains("file name too long")){
				sendFail(FOR_NAME_TOOLONG);
			}else{
				sendFail(FOR_FAILED,errmsg);
			}
		}
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

	private boolean sendFail(int errorCode,String errmsg) {	
		ByteWriter writer = new ByteWriter();
		writer.Write(ID_FAIL);
		writer.Write(errorCode);
		if(errmsg!=null){
			writer.Write(errmsg);
		}
		writer.getBaseStream().GetBuffer();
		
		boolean result = mPeer.write(writer, TIME_OUT);
		try {
			Thread.sleep(20000);
		}
		catch (InterruptedException e) {		
			e.printStackTrace();
		}
		
		mPeer.close();
		return result;
	}
	private boolean sendFail(int errorCode) {
		return sendFail(errorCode,null);
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
			Thread.sleep(20000);
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
            int initResult = initPullFile();
			if (initResult==0)
				return false;

			long readSize = 0;
			int num=0;
			ByteBuffer dataBuf = ByteBuffer.allocate(IConstants.SOCKET_BUFFER_SIZE + 8);
			dataBuf.put(ID_DATA);

			while (readSize < mFileSize) {
				byte[] readdata;
				boolean readReult = false;
				dataBuf.position(4);
				int bufSize = (int) Math.min(IConstants.SOCKET_BUFFER_SIZE, mFileSize - readSize);
				ByteBuffer sizeBuf = ByteBuffer.allocate(4);
				writeInt32(sizeBuf, bufSize);
				sizeBuf.flip();
				dataBuf.put(sizeBuf);
				dataBuf.limit(bufSize + 8);
				if(initResult==1){
					readReult = readFromFile(dataBuf);
				}else{
					readdata =FileHelper.readFromFileByNdsh(mContext,mFilePath,num,bufSize);
					if(readdata!=null){
						dataBuf.put(readdata);
						readReult = true;
					}else{
						break;
					}
					   
				}
				
				
				if (readReult) {
					dataBuf.flip();
					if (mPeer.write(dataBuf, TIME_OUT)) {
						readSize += bufSize;
						num++;
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
			}else{
				sendFail(FOR_FAILED);
			}
		}

		doEndPullFile();
		return result;
	}


	//0失败 1java层可读成功 2ndsh可读成功
	private int initPullFile() {
		LogCenter.error(TAG, "pull file:" + mFilePath);
		File f = new File(mFilePath);
		ByteWriter writer = new ByteWriter(12);
		writer.Write(ID_OKAY);
		
		if (f.exists()) {
			try {
				mInputStream = new FileInputStream(f);
				mFileChannel = mInputStream.getChannel();
				mFileSize = f.length();
				writer.Write(mFileSize);
				boolean result = mPeer.write(writer.getBaseStream().GetBuffer(), TIME_OUT);
				if(result){
					return 1;
				}else{
					return 0;
				}
				
			}
			catch (FileNotFoundException e) {
				e.printStackTrace();
				if(NdkManager.isNdshActualRunning(mContext)){
					if(FileHelper.isSdcardFile(mFilePath)){
						checkException(e);
						return 0;
					}
						
				}
			}
		}
        try {
    		if (NdkManager.isNdshActualRunning(mContext)) {
    			FileInfo info = FileHelper.getFileInfo(mContext, mFilePath);
    			if (info != null && info.isExist()) {
    				mFileSize = info.getSize();
    				writer.Write(mFileSize);
    				boolean result = mPeer.write(
    						writer.getBaseStream().GetBuffer(), TIME_OUT);
    				if (result) {
    					return 2;
    				} else {
    					return 0;
    				}
    			}
    		}
    		byte[] data = FileOperater.getFileAttr(mContext, mFilePath);
    		ByteReader read = new ByteReader(new MemoryStream(data));
    		int result = read.ReadInt32();
    		LogCenter.error(TAG, "pull file error: " + result + " :" + mFilePath);
    		sendFail(result);
		} catch (Exception e) {
	
		}


		return 0;
	}

	private boolean readFromFile(ByteBuffer buf) {
		try {
			while (buf.hasRemaining()) {
				mFileChannel.read(buf);
			}

			return true;
		}
		catch (Exception e) {
			checkException(e);
		}

		return false;
	}
	


	private void doEndPullFile() {
		try {
			if(mFileChannel!=null)
			  mFileChannel.close();
			if(mInputStream!=null)
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
