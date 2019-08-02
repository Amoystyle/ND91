package daemon.ndk;

import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import daemon.common.Device;
import daemon.common.MD5Util;
import daemon.common.MountPartitionParam;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.shell.Shell;
import daemon.shell.Su;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.os.SystemClock;
import android.util.Log;

class NdkPipe {
	private static String mNdshFileName = "ndsh";
	private static String mMonitorFileName = "ndmonitor";
	
	private static int mNdshResourcewId = 0;
	private static int mMonitorResourcewId = 0;
	
	private String TAG = getClass().getName();
	private Context mContext = null;

	private final String mFifoC2JAVA;
	private final String mFifoJAVA2C;

	private String mNdshResoucePath;
	private String mNdshExecutablePath;

	private String mMonitorResoucePath; 
	private String mMonitorExecutablePath;
	
	private boolean mIsFisOpened = false;
	private FileInputStream fis = null;
	private FileOutputStream fos = null;

	private boolean mIsInputThreadStared = false;

	private boolean mIsNdshRunning = false;

	private static NdkPipe mInstance = null;
	private String mNdshRawMD5 = null; 
	private String mMonitorRawMD5 = null; 
	private Process mNdshProc = null;
	private Object mWriteLock = new Object();
	private Object mStartNdshLock = new Object(); 
	private long mNdshId = 0;
	// ndmonitor服务端PORT
	private final static String mMonitorPortFile = "ndmonitor_port"; 
	// 默认端口
	private final static int mMonitorDefaultPort = 8891;
	//返回数据存储列表
	ArrayList<NdshDataInfo> mExecData = new ArrayList<NdshDataInfo>();
	class NdshDataInfo {
		private byte[] resultInfo =null;
		private byte[] writeInfo=null;
		private boolean hasWrite =false;
		private boolean hasRead =false;
		private int key;
		private int timeRemain;
		
	}
	//是否启动
	private boolean readResultIng =false;
	
    //执行命令个数
    private int mExecCount =0;
	//最大的执行命令个数
	private final int MAX_EXECCOUNT = 5;
	
	private NdkPipe(Context context) {
		LogCenter.debug(TAG, "new NdkPipe(Context context)");
		mContext = context;
		String packageName = mContext.getPackageName();
		if (mNdshFileName == null ||  "".equals(mNdshFileName))
			mNdshFileName = packageName;

		String pipeDir = "/data/data/" + packageName + "/files";
		mFifoC2JAVA = pipeDir + "/fifo_c2java";
		mFifoJAVA2C = pipeDir + "/fifo_java2c";

		mNdshExecutablePath = "/system/bin/" + mNdshFileName;
		mNdshResoucePath = "/data/data/" + packageName + "/" + mNdshFileName;
		
		mMonitorExecutablePath = "/system/bin/" + mMonitorFileName;
		mMonitorResoucePath = "/data/data/" + packageName + "/" + mMonitorFileName; 
		File f = new File(pipeDir);
		if (!f.exists())
			f.mkdir();                                                                                   
		new NdshThread(this).start();
	}

	public synchronized static NdkPipe getInstance(Context context) {
		if (mInstance == null) {
			mInstance = new NdkPipe(context);
		}

		return mInstance;
	}

	public static void setNdInfo(int resNdshId, String ndShFileName, int resNdMonitorId, String ndMonitorFileName) {
		mNdshResourcewId = resNdshId;
		mNdshFileName = ndShFileName;
		mMonitorResourcewId = resNdMonitorId;
		mMonitorFileName = ndMonitorFileName;
	}

	public static String GetNdshFileName() {
		return mNdshFileName;
	}
	
	public static String GetNdMonitorFileName(){
		return mMonitorFileName;
	}

	private boolean startNdshInThread() {
		boolean result = false;
		if (initNdshFile()) {
			result = runNdsh(mNdshExecutablePath);
		}
		
		LogCenter.debug(TAG, "startNdshInThread() return: " + result);
		
		return result;
	}

	public void destroy() {
		try {
			if (fis != null) {
				fis.close();
			}
			if (fos != null) {
				fos.close();
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			fis = null;
			fos = null;
			// pis = null;
			// pos = null;
		}
	}
 
	private boolean runNdsh(String runPath) {
		LogCenter.error(TAG, "runNdsh(\"" + runPath + "\")");
		clear();
		try {
			mNdshId = 0;
			mIsFisOpened = false;

			File fileNdsh = new File(runPath);
			int totalSleep = 3000;
			while (!fileNdsh.exists() && totalSleep > 0) {
				Thread.sleep(10);
				totalSleep -= 10;
			}
			String cmd = runPath + " " + mFifoC2JAVA + " " + mFifoJAVA2C;
			try {
				if (mNdshProc != null) {
					mNdshProc.destroy();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
			// mNdshProc = Runtime.getRuntime().exec(cmd);
			
			// 这里只改动android 4.4
			if(Device.getIntSdkVersion() >= 19)
			{  
				boolean bMonitorIsRunning = true;
				// 看monitor是否存在
				if(isFileExist(mMonitorExecutablePath))
				{  
					// 是否运行
					// 如果还没起来，su启动
					if(IsProcExist(mMonitorFileName) == false)
					{
						LogCenter.error(TAG, "ndmonitor proc not find...");
						
						// 是否授权
						if(Su.getInstance().hasSuAuthorize())
						{
							// android 5.0需要关闭SEAndroid
							if(Device.getIntSdkVersion() >= 21){
								if(IsCloseSEAndroid() == false){
									boolean res = Su.getInstance().exec("setenforce 0");
									LogCenter.error(TAG, "Android 5.0: Open SEAndroid = " + res);
									// 设置失败，对于5.0如果没开起enforce，是读写不了文件,socket
									if(res == false){
										return false;
									}
								}
							}
							
							cmd = mMonitorExecutablePath;
							Su.getInstance().exec(cmd);
							totalSleep = 15000;
							while (totalSleep > 0) { 
								totalSleep -= 1000;
								Thread.sleep(1000);
								if(IsProcExist(mMonitorFileName)){
									LogCenter.debug(TAG, "ndmonitor running...");
									break;
								}
							}
						}
						else
						{
							// 没有授权
							bMonitorIsRunning = false;
							LogCenter.error(TAG, "No su, getSuAuthorize first...");
						}
					}
					
					if(bMonitorIsRunning)
					{
						// 发送重启命令
						SystemClock.sleep(1000); 
						cmd = "restart";
						cmd+= "|";
						cmd+= mNdshExecutablePath;
						cmd+= "|";
						cmd+= mFifoC2JAVA;
						cmd+= "|";
						cmd+= mFifoJAVA2C;
						
						sendCmd2NdMonitor(cmd);
					}
					else
					{
						// ndmonitor没起来，就需要授权了
						return false;
					}
				}
				else
				{
					// 理论上，这个分支是不会进来的
					LogCenter.error(TAG, "ndmonitor is not exist");
					if(Su.getInstance().hasSuAuthorize())
					{
						// 运行
						cmd = mMonitorExecutablePath;
						Su.getInstance().exec(cmd); 
						
						totalSleep = 15000;
						while (totalSleep > 0) { 
							totalSleep -= 1000;
							Thread.sleep(1000);
							if(IsProcExist(mMonitorFileName)){
								LogCenter.debug(TAG, "ndmonitor running...");
								break;
							}
						}
							
						// 发送重启命令
						SystemClock.sleep(1000); 
						cmd = "restart";
						cmd+= "|";
						cmd+= mNdshExecutablePath;
						cmd+= "|";
						cmd+= mFifoC2JAVA;
						cmd+= "|";
						cmd+= mFifoJAVA2C;
						
						sendCmd2NdMonitor(cmd);
					}
					else
					{
						LogCenter.error(TAG, "No su, getSuAuthorize first...");
					}
				}
			}
			else
			{
				mNdshProc = Runtime.getRuntime().exec(cmd);
			}
			
			// pis = mNdshProc.getInputStream();
			// pos = mNdshProc.getOutputStream();
			new Thread() {
				public void run() {
					initFifoC2Java();
				}
			}.start();

			if (!checkThreadStart(3000)) {
				LogCenter.error(TAG, "checkThreadStart false");
				// return false;
			}		
			if (!checkFileExist(mFifoJAVA2C, 3000)) {
				LogCenter.error(TAG, "check java2C false");
				return false;
			}

			fos = new FileOutputStream(mFifoJAVA2C);
			DataPackage initPack = new DataPackage(BusinessType.INIT_C2JAVA, null, false);
			synchronized (mWriteLock) {
				fos.write(initPack.getProtocalPackage());
				fos.flush();
			}
			if (!checkFisOpend(3000)) {
				LogCenter.error(TAG, "check inputStream false");
				return false;
			}
			mNdshId = System.currentTimeMillis();		
		}
		catch (Exception e) {
			e.printStackTrace();
			String msg = e.getMessage();
			LogCenter.error(TAG, msg);
			return false;
		}

		setNdshRunning(true);
		mExecCount =0;
		DoHeartBeat();

		return true;
	}

	private boolean checkFisOpend(long timeout) {
		boolean result = false;
		try {
			long totalSleep = 0;
			long onceSleep = 20;
			while (!mIsFisOpened && totalSleep < timeout) {
				Thread.sleep(onceSleep);
				totalSleep += onceSleep;
				LogCenter.error(TAG, "checkFisOpend, timeremain:" + (timeout - totalSleep));
			}
			result = mIsFisOpened;
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		return result;
	}

	private boolean checkThreadStart(long timeout) {
		boolean result = false;
		try {
			long totalSleep = 0;
			long onceSleep = 20;
			while (!mIsInputThreadStared && totalSleep < timeout) {
				Thread.sleep(onceSleep);
				totalSleep += onceSleep;
			}
			result = mIsInputThreadStared;
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}
		return result;
	}

	private boolean checkFileExist(String path, long timeout) {
		boolean result = false;
		File file = new File(path);
		try {
			long totalSleep = 0;
			long onceSleep = 20;
			while (!file.exists() && totalSleep < timeout) {
				Thread.sleep(onceSleep);
				totalSleep += onceSleep;
			}
			result = file.exists();
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		return result;
	}

	private boolean releaseNdsh() {
		boolean result = false;
		InputStream is = null;
		FileOutputStream os = null;
		try {
			is = (InputStream) mContext.getResources().openRawResource(mNdshResourcewId);
			os = new FileOutputStream(mNdshResoucePath);
			int length = is.available();
			byte[] data = new byte[length];
			is.read(data);
			os.write(data);

			result = true;
			LogCenter.error(TAG, "release ndsh from raw success");
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}
		finally {
			try {
				if (is != null)
					is.close();

				if (os != null)
					os.close();
			}
			catch (Exception e) {

			}
		}

		return result;
	}
	
	private boolean releaseNdMonitor() {
		boolean result = false;
		InputStream is = null;
		FileOutputStream os = null;
		try {
			is = (InputStream) mContext.getResources().openRawResource(mMonitorResourcewId);
			os = new FileOutputStream(mMonitorResoucePath);
			int length = is.available();
			byte[] data = new byte[length];
			is.read(data);
			os.write(data);

			result = true;
			LogCenter.error(TAG, "release NdMonitor from raw success");
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}
		finally {
			try {
				if (is != null)
					is.close();

				if (os != null)
					os.close();
			}
			catch (Exception e) {

			}
		}

		return result;
	}

	/*
	 * private boolean initNdshPrivateFile() { boolean result = false; mFifoExeutablePrivate =
	 * ConfigManager.getFifoPrivate(mContext); if (mFifoExeutablePrivate.equals("")) mFifoExeutablePrivate =
	 * FIFO_EXECUTABLE_PRIVATE;
	 * 
	 * File file = new File(mFifoExeutablePrivate); String privateMD5 = ""; if (file.exists()) { privateMD5 =
	 * MD5Util.getFileMD5String(file); }
	 * 
	 * if (!privateMD5.equals(mRawMD5)) { String traversePath = FIFO_EXECUTABLE_PRIVATE; file = new File(traversePath);
	 * int index = 1; // 删除所有ndsh文件 while (file.exists()) { file.delete(); if
	 * (traversePath.equals(mFifoExeutablePrivate)) { break; } index++; traversePath = FIFO_EXECUTABLE_PRIVATE + index;
	 * file = new File(traversePath); } mFifoExeutablePrivate = FIFO_EXECUTABLE_PRIVATE; file = new
	 * File(mFifoExeutablePrivate); index = 1; while (file.exists()) { index++; mFifoExeutablePrivate =
	 * FIFO_EXECUTABLE_PRIVATE + index; file = new File(mFifoExeutablePrivate); } mFifoExecutablePath =
	 * mFifoExeutablePrivate; ConfigManager.setFifoPrivate(mContext, mFifoExeutablePrivate);
	 * 
	 * InputStream is = null; FileOutputStream os = null; try { is = (InputStream)
	 * mContext.getResources().openRawResource(mResourceId); os = new FileOutputStream(mFifoExecutablePath); int length
	 * = is.available(); byte[] data = new byte[length]; is.read(data); os.write(data);
	 * 
	 * String command = "chmod 0755 " + mFifoExecutablePath; Shell.getInstance().execForString(command);
	 * 
	 * result = true; LogCenter.error(TAG, "init private ndsh success"); } catch (Exception e) { e.printStackTrace();
	 * result = false; } finally { try { if (is != null) is.close();
	 * 
	 * if (os != null) os.close(); } catch (Exception e) {
	 * 
	 * } } } else { result = true; }
	 * 
	 * return result; }
	 */

	private boolean initNdshSuFileByOld() {
		LogCenter.debug(TAG, "initNdshSuFileByOld()");
		File f = new File(mNdshExecutablePath);
		if (f.exists()&&f.length()==0) {
			//如果文件存在，且大小为0，则删除后，重新加载ndsh
			f.delete();
			return false;
		}
		boolean result = false;
		String suMD5 = "";
		File file = new File(mNdshExecutablePath);
		if (file.exists()) {
			suMD5 = MD5Util.getFileMD5String(file);
		}
		
		LogCenter.debug(TAG, "\"" + mNdshExecutablePath + "\" is " + (file.exists() ? "exist" : "not exist"));
		LogCenter.error(TAG, "NdshExe suMD5: " + suMD5);
		if (!suMD5.equals(mNdshRawMD5)) {
			if (file.exists()) {
				if (runNdsh(mNdshExecutablePath)) {
					LogCenter.error(TAG, "runNdsh_su success");
					releaseNdsh();
					if (upgrade()) {
						LogCenter.error(TAG, "upgrade by old ndsh success");
						result = true;
					}

					quit();
				}
			}
		}
		else {
			result = true;
		}
		return result;
	}

	public synchronized boolean initNdshSuFileByShell() {
		LogCenter.debug(TAG, "initNdshSuFileByShell()");
		Su su = Su.getInstance();
		if (su.hasSU()) {
			String suNdshMD5 = "";
			String suMonitorMD5 = "";
			File fileNdsh = new File(mNdshExecutablePath);
			File fileMonitor = new File(mMonitorExecutablePath);
			// 是否要授权
			boolean bAuthorize = false;
			
			if (fileNdsh.exists()) {
				suNdshMD5 = MD5Util.getFileMD5String(fileNdsh);
			}
			if (fileMonitor.exists()) {
				suMonitorMD5 = MD5Util.getFileMD5String(fileMonitor);
			}
			  
			LogCenter.error(TAG, "mNdshRawMD5:" + mNdshRawMD5);
			LogCenter.error(TAG, "mSuNdshMD5:" + suNdshMD5); 
			
			if(suNdshMD5.equals("") || !suNdshMD5.equals(mNdshRawMD5)){
				bAuthorize = true;
			}
			
			if(Device.getIntSdkVersion() >= 19){
				if(suMonitorMD5.equals("") || !suMonitorMD5.equals(mMonitorRawMD5)){
					bAuthorize = true;
				}
				// 如果ndmonitor没跑起来也要授权
				if(IsProcExist(mMonitorFileName) == false){
					bAuthorize = true;
				}
			}
			
			// android 5.0需要关闭SEAndroid
			if(Device.getIntSdkVersion() >= 21){
				// 判断是否开启了SEAndroid
				if(IsCloseSEAndroid() == false){
					bAuthorize = true;
					LogCenter.error(TAG, "Android 5.0: SEAndroid is close...");
				}				
			}
			
			if(bAuthorize) {
				if (!su.getSuAuthorize(mContext)){
					LogCenter.error(TAG, "su.getSuAuthorize fail");
					return false;
				}
				
				if(Device.getIntSdkVersion() >= 21){
					if(IsCloseSEAndroid() == false){
						boolean res = su.exec("setenforce 0");
						LogCenter.error(TAG, "Android 5.0: Open SEAndroid = " + res);
						// 设置失败，对于5.0如果没开起enforce，是读写不了文件,socket
						if(res == false){
							return false;
						}
					}
				}
					
				MountPartitionParam systemMount = MountPartitionParam.createPartitionParam(MountPartitionParam.SYSTEM);
				String cmd = String.format("mount -o remount -w -t %1$s %2$s /system", systemMount.getFilesystemType(), systemMount.getSource());
				su.exec(cmd);

				cmd = "chmod 777 /system/bin";
				su.exec(cmd);
 
				if (!releaseNdsh()) {
					return false;
				}
				
				if(!releaseNdMonitor()){
					return false;
				}
				
				if(!copyFile2SystemBin(su, mNdshRawMD5, mNdshResoucePath,mNdshExecutablePath)){
					return false;
				}
				// android 4.4 需要copy
				if((Device.getIntSdkVersion() >= 19)){
					if(!copyFile2SystemBin(su, mMonitorRawMD5, mMonitorResoucePath,mMonitorExecutablePath)){
						return false;
					}
				}

				cmd = "chmod 755 /system/bin";
				su.exec(cmd);

				cmd = String.format("mount -o remount -r -t %1$s %2$s /system", systemMount.getFilesystemType(), systemMount.getSource());
				su.exec(cmd);

				return runNdsh(mNdshExecutablePath);
			}
			else{
				return runNdsh(mNdshExecutablePath);
			}
		}

		return false;
	}

	private boolean copyFile2SystemBin(Su su, String strRawMD5, String strSrcPath, String strDestPath){
		File file;
		String cmd;
		String strMD5;
		int timeout;
		
		file = new File(strDestPath);
		
		if (file.exists()) {
			cmd = "rm " + strDestPath;
			su.exec(cmd);
		}
		
		cmd = "dd if=" + strSrcPath + " of=" + strDestPath;
		su.exec(cmd);

		int cnt = 30;
		while (!file.exists() && cnt > 0) {
			try {
				Thread.sleep(100);
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}

			cnt--;
		}

		if (file.exists()) {
			cmd = "chown root:root " + strDestPath;
			su.exec(cmd);

			cmd = "chmod 4755 " + strDestPath;
			su.exec(cmd);
		}
		else
		{
			return false;
		}
		
		strMD5 = MD5Util.getFileMD5String(new File(strDestPath));
		timeout = 8000;
		while (!strRawMD5.equals(strMD5) && timeout > 0) {
			try {
				Thread.sleep(100);
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			} 

			timeout -= 100;
			strMD5 = MD5Util.getFileMD5String(new File(strDestPath));
		}

		LogCenter.error(TAG, "check " + strDestPath + " timeout: " + timeout);
		
		if(timeout > 0)
		{
			return true;
		}
		else
		{
			if ((new File(strDestPath)).exists()) {
				cmd = "rm " + strDestPath;
				su.exec(cmd);
			}
			return false;
		}
	}
	
	private boolean initNdshFile() {
		String str1MonitorMD5;
		String str2MonitorMD5;
		String str3MonitorMD5;
		
		if (mNdshRawMD5 == null) {
			mNdshRawMD5 = MD5Util.getFileMD5String(mContext, mNdshResourcewId);
		}
		if(mMonitorRawMD5 == null){
			mMonitorRawMD5 = MD5Util.getFileMD5String(mContext, mMonitorResourcewId);
		}
		
		LogCenter.debug(TAG, "mRawMD5: " + mNdshRawMD5);
		
		str1MonitorMD5 = MD5Util.getFileMD5String(mContext, mMonitorResourcewId);
		str2MonitorMD5 = MD5Util.getFileMD5String(new File(mMonitorResoucePath)); 
		
		if(str1MonitorMD5.equals(str2MonitorMD5) == false)
		{
			// 加载
			releaseNdMonitor();
			LogCenter.debug(TAG, "mNdMonitorResourcewId MD5: " + str1MonitorMD5);
			LogCenter.debug(TAG, "mNdMonitorResoucePath MD5: " + str2MonitorMD5);
		}
		
		str2MonitorMD5 = MD5Util.getFileMD5String(new File(mMonitorResoucePath));
		str3MonitorMD5 = MD5Util.getFileMD5String(new File(mMonitorExecutablePath));
		
		// android4.4
		if((Device.getIntSdkVersion() >= 19) 
		&& (str3MonitorMD5.equals(str2MonitorMD5) == false))
		{
			String cmd = "";
			cmd = "upgrade";
			cmd+= "|";
			cmd+= mMonitorResoucePath;
			
			LogCenter.debug(TAG, "UpGrade NdMonitor...");
			// 发起升级命令
			sendCmd2NdMonitor(cmd);
		}
		
		return initNdshSuFileByOld();
	}

	private void clear() {
		setNdshRunning(false);
		mIsInputThreadStared = false;

		LogCenter.error(TAG, "clear");
		destroy();
		
		File pipeJava2C = new File(mFifoJAVA2C);
		if (pipeJava2C.exists()) {
			if (pipeJava2C.delete())
				LogCenter.error(TAG, "delete file:" + mFifoJAVA2C);
		}

		File pipeC2Java = new File(mFifoC2JAVA);
		if (pipeC2Java.exists()) {
			if (pipeC2Java.delete())
				LogCenter.error(TAG, "delete file:" + mFifoC2JAVA);
		}
	}

	private void initFifoC2Java() {
		try {
			if (fis == null) {
				File file = new File(mFifoC2JAVA);
				int totalSleep = 3000;
				while (!file.exists() && totalSleep > 0) {
					Thread.sleep(10);
					totalSleep -= 10;
				}
				if (totalSleep <= 0) {
					LogCenter.error(TAG, "initFifoC2Java time out");
					return;
				}
				mIsInputThreadStared = true;
				fis = new FileInputStream(mFifoC2JAVA);

				if (fis != null) {
					mIsFisOpened = true;
					LogCenter.error(TAG, "fis opened success");
				}else{
					LogCenter.error(TAG, "fis opened fail");
				}
			}

		}
		catch (Exception e) {
			e.printStackTrace();
			LogCenter.error(TAG, e.getMessage());
		}
	}

	private boolean upgrade() {
		boolean result = false;
		ByteWriter writer = new ByteWriter();
		MountPartitionParam systemMount = MountPartitionParam.createPartitionParam(MountPartitionParam.SYSTEM);
		writer.Write(systemMount.getFilesystemType());
		writer.Write(systemMount.getSource());
		writer.Write(mNdshResoucePath);

		DataPackage dataSend = new DataPackage(BusinessType.UPGRADE, writer.getBaseStream().ToArray(), false);
		DataPackage dataReceive = execForResult(dataSend, 5000,NdkManager.UPGRADE_KEY);
		if (dataReceive != null) {
			ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
			int status = reader.ReadInt32();
			LogCenter.error(TAG, "ndsh upgrade result:" + status);
			result = status == 1;
		}
		File f = new File(mNdshExecutablePath);
		String newMd5 = MD5Util.getFileMD5String(f);
		if(newMd5.equals("") || !newMd5.equals(mNdshRawMD5)){
			f.delete();
			return false;
		}

		return result;
	}

	private void DoHeartBeat() { 
		final DataPackage pack = new DataPackage(BusinessType.HEART_BEAT, null,
				false);
		new Thread(new Runnable() {
			@Override
			public void run() {
				LogCenter.error(TAG, "start DoHeartBeat");
				while (true) {
					if (!isNdshRunning()) {
						break;
					}
					if(mExecCount<1){
						execForResult(pack, 10000, NdkManager.HEART_KEY);
					}					
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start(); 
	}

	public synchronized void quit() {
		LogCenter.error(TAG, "quit");
		try {
			ByteWriter writer = new ByteWriter();
			writer.Write(NdkManager.getKey());
			DataPackage dataSend = new DataPackage(BusinessType.QUIT, writer.getBaseStream().ToArray(), false);
			exec(dataSend);
			Thread.sleep(1000);

			if (mNdshProc != null) {
				mNdshProc.destroy();
				mNdshProc = null;
			}

			// 等待ndsh进程退出
			Thread.sleep(100);
			clear();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void clearInstace(){
		mInstance =null;
	}

	public void exec(DataPackage pack) {
		try {
			synchronized (mWriteLock) {
				if (fos != null) {
					byte[] data = pack.getProtocalPackage();
					fos.write(data);
					fos.flush();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
 
	Thread mReadResultThread  = new  Thread(){
		public void run(){
			readResultData();
		}
	};
	
	
	private  void  startReadThread(){
		if(!readResultIng){
			readResultIng = true;
			synchronized (mExecData) {
				mExecData.clear();
			}
			new  Thread(){
				public void run(){
					readResultData();
				}
			}.start();
		}
	}
	
	
	private int byte2int(byte head1,byte head2,byte head3,byte head4){
		int dataLen = 0;
		dataLen += head1 & 0x000000FF;
		dataLen += (head2 << 8) & 0x0000FF00;
		dataLen += (head3 << 16) & 0x00FF0000;
		dataLen += (head4 << 24) & 0xFF000000;
		return dataLen;
	}
	
	
	private NdshDataInfo getNdshDataInfo(int key){
		int count = mExecData.size();
		for (int i = 0; i < count; i++) {
			NdshDataInfo val = mExecData.get(i);
			if(val!=null){
				if(val.key == key){
					return val;
				}
			}
		}
		return null;
	}

	
	private void readResultData(){
		int timeout = 5 * 1000;
        int errkey =-1;
		try {
			while (true) {
				Thread.sleep(10);
				if(!mIsNdshRunning){
					break;
				}	
				ArrayList<NdshDataInfo> timeOutList =new ArrayList<NdkPipe.NdshDataInfo>();
				int hasWriteNotReadCount =0;
				boolean needwrite =false;
				//先写
				synchronized (mExecData) {
					int count = mExecData.size();
					for (int i = 0; i < count; i++) {
						NdshDataInfo val = mExecData.get(i);
						if(val!=null){
							errkey = val.key;
							if(!val.hasWrite){
								fos.write(val.writeInfo);
								fos.flush();
								val.writeInfo =null;
								val.writeInfo = new byte[1]; 
								val.hasWrite = true;
								needwrite = true;
								hasWriteNotReadCount ++;
							}else if(val.hasWrite&&(!val.hasRead)){
								hasWriteNotReadCount ++;
							}
							if(val.hasWrite&&val.timeRemain<3000 ){
								timeOutList.add(val);
							}
						}
					}

				}
				if(needwrite){
					
				}
				
                if(hasWriteNotReadCount==0){
                	continue;
                }
                Thread.sleep(30);
                int timeOutCount = timeOutList.size();
                do{
    				if(!mIsNdshRunning){
    					break;
    				}	
    				byte[] head = Read(9, timeout);
    				if (head != null && head[0] == 'N' && head[1] == 'D'
    						&& head[2] == 'P' && head[3] == 'C') {
    					int dataLen = byte2int(head[5],head[6],head[7],head[8]);
    					if(head[4]==BusinessType.HEART_BEAT.getValue()){
    						synchronized (mExecData) {
    							NdshDataInfo val = getNdshDataInfo(NdkManager.HEART_KEY);
    							if(val!=null){
    								val.hasRead = true;
    								hasWriteNotReadCount --;  								
    								byte[] data = {0};
    								val.resultInfo = data;
    								if(timeOutCount>0){
    									timeOutList.remove(val);
    									timeOutCount--;
    								}
    							}
    						}
    					}else if(head[4]==BusinessType.UPGRADE.getValue()){
    						byte[] data = Read(dataLen, timeout);
    						synchronized (mExecData) {
    							NdshDataInfo val = getNdshDataInfo(NdkManager.UPGRADE_KEY);
    							if(val!=null){
    								val.hasRead = true;
    								hasWriteNotReadCount --;  
    								val.resultInfo = data;
    								if(timeOutCount>0){
    									timeOutList.remove(val);
    									hasWriteNotReadCount--;
    								}
    							}
    						}
    					}else{
    						if (dataLen > 0) {
    							byte[] key = Read(4, timeout);
    							int keyVaulle =byte2int(key[0],key[1],key[2],key[3]);
    							int size = 0;
    							byte[] sizebyte =Read(4, timeout);
    							size = byte2int(sizebyte[0],sizebyte[1],sizebyte[2],sizebyte[3]);
    							byte[] data = Read(size, timeout);
    							synchronized (mExecData) {
    								NdshDataInfo val = getNdshDataInfo(keyVaulle);
    								if(val!=null){
    									val.hasRead = true;
    									val.resultInfo = data;
    									hasWriteNotReadCount --;  
        								if(timeOutCount>0){
        									timeOutList.remove(val);
        									timeOutCount--;
        								}
    								}
    							}	
    						}
    					}
    				}else if(head != null){
    					Log.e("readresult data", "head != null but error "+head[0]+","+head[1]+","+head[2]+","+head[3]);
    				}
//    				if(!needReadOver){
//    					break;
//    				}
                }while(timeOutCount>0); 
			}
		} catch (Exception e) {
			e.printStackTrace();
			LogCenter.error("readResultData "+errkey, e.toString());
		} catch (OutOfMemoryError e) {
			LogCenter.error("readResultData", e.toString());
		}
		readResultIng = false;
	}
	
	long lasttime =0;
	
	public DataPackage execForResult(DataPackage pack, int timeout,int key) {
		DataPackage result = null;
		long ndshId = mNdshId;
		
		try {
			NdshDataInfo info =new NdshDataInfo();
			synchronized (mWriteLock) {
				while(mExecCount>=MAX_EXECCOUNT){
					Thread.sleep(100);
					if(ndshId!=mNdshId){
						return null;
					}
				}
				
				info.writeInfo = pack.getProtocalPackage();
				info.key = key;
				startReadThread();
				info.timeRemain = timeout;

				mExecData.add(info);
				// 封装协议包并写入到管道
				//fos.write();
				//fos.flush();
				mExecCount ++;
			}
			 
			int sleepTime = 0;
			boolean isFirst =  true;
			while (true) {
				Thread.sleep(25);
				synchronized (mExecData) {
					if(info!=null){
						if(!info.hasWrite){
							continue;
						}else{
							info.timeRemain -= 25;
						}
					}
				}	
				if(ndshId!=mNdshId){
					return null;
				}
				sleepTime += 25;
				if (sleepTime >= timeout && !isFirst) {
					LogCenter.error(TAG, "Read data timeout !!!  "+key);
					result = null;
					break;
				}

				
				isFirst = false;
				byte[] data =null;
				synchronized (mExecData) {
					if(info!=null){
						if(!info.hasRead){
							continue;
						}
						data = info.resultInfo;
						mExecData.remove(info);
						if(ndshId==mNdshId){
							mExecCount--;
						}
					}						 
				}
				if (data != null) {
					result = new DataPackage(pack.getBusiness(), data,false);
					info.resultInfo = null;
					break;
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			result = null;
			mExecCount = 0;
		}
		synchronized (mStartNdshLock) {
			if (result == null&&ndshId==mNdshId) {
				setNdshRunning(false);
				// 与ndsh通信失败，自动重启ndsh
				new NdshThread(this).start();
			}
		}	
		return result;
	}

	
	// 从管道读取数据，超时返回null
	private byte[] Read(int len, int timeout) {
		byte[] data = new byte[len];
		try {
			int readCnt = 0;
			int sleepTime = 0;
			boolean isFirst = true;
			while (readCnt < len) {
				if (sleepTime >= timeout && !isFirst) {
					LogCenter.error(TAG, "Read data timeout !!!");
					data = null;
					break;
				}
				isFirst = false;
				int onceReadCnt = fis.read(data, readCnt, len - readCnt);
				if (onceReadCnt > 0) {
					readCnt += onceReadCnt;
				}
				else if (onceReadCnt == 0) {
					Thread.sleep(10);
					sleepTime += 10;
					continue;
				}
				else if (onceReadCnt < 0) {
					data = null;
					break;
				}

			}
		}
		catch (Exception e) {
			e.printStackTrace();
			data = null;
		}
		return data;
	}
	

	public synchronized boolean isNdshRunning() {
		return mIsNdshRunning;
	}
	

	public synchronized void setNdshRunning(boolean bRunning) {
		mIsNdshRunning = bRunning;
		LogCenter.error(TAG, "setNdshRunning:"+bRunning);
	}
	

	class NdshThread extends Thread {
		private int mTryCount = 5;
		private NdkPipe mPipe;

		public NdshThread(NdkPipe pipe) {
			mPipe = pipe;
			mNdshId = 0;
		}

		@Override
		public void run() {
			while (mTryCount > 0) {
				LogCenter.debug(TAG, "try count: " + mTryCount);
				if (mPipe.startNdshInThread()) {
					break;
				} 
				try{
					Thread.sleep(1000);
				}catch(Exception e){
					e.printStackTrace();
				}
				--mTryCount;
			}
		} 
	}
	
	// 判断文件是否存在
	public boolean isFileExist(String fileName)
	{
		File file = new File(fileName);
		return file.exists();
	}
	
	// 判断进程是否存在
	public static boolean IsProcExist(String procName)
	{
		for(int i = 0; i < 2; i++)
		{
			String result = Shell.getInstance().execForString("ps "+ procName);
			if(result.indexOf(procName) > 0)
			{
				return true;
			}
			SystemClock.sleep(1000);
		}
		return false;
	}
	// 判断是否关闭SEAndroid(enforce)
	public static boolean IsCloseSEAndroid(){
		String result = Shell.getInstance().execForString("getenforce");
		System.out.println(result);
		// 如果关闭则显示Permissive
		if(result.indexOf("Permissive") < 0){
			return false;
		} 
		return true;
	}
	// 获取服务端port
	private int getNdMonitorServerPort()
	{
		int port = mMonitorDefaultPort;
		try {
			// 获得输入流
			FileInputStream fis = mContext.openFileInput(mMonitorPortFile); 
			// 用来获得内存缓冲区的数据，转换成字节数组
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			byte[] buffer = new byte[16];
			
			int length = fis.read(buffer);
			if(length > 0)
			{
				stream.write(buffer,0,length);
			}
			port = Integer.parseInt(stream.toString());
			
			// 端口默认值以上
			if(port < mMonitorDefaultPort)
			{
				port = mMonitorDefaultPort;
			}
			
			stream.close(); //关闭
			fis.close(); 
		}
		catch (IOException e){
			e.printStackTrace();
		}
		return port;
	}
	
	// 发送重启命令
	private boolean sendCmd2NdMonitor(String cmd)
	{
		Socket socket = null;
		boolean result = false;
		int port;
		
		// 获取端口
		port = getNdMonitorServerPort();
		
		try {
			InetAddress serverAddr = InetAddress.getByName("127.0.0.1");

			// 应用Server的IP和端口建立Socket对象
            socket = new Socket(serverAddr, port);

            Log.d(TAG, "NdMonitor Port:" + port);  
            
            // 将信息通过这个对象来发送给Server  
            PrintWriter out = new PrintWriter(new BufferedWriter(  
                    new OutputStreamWriter(socket.getOutputStream())),  
                    true);  
              
            // 把用户输入的内容发送给server
            String toServer = cmd;
            out.printf(toServer);
            out.flush(); 
            out.close();
            result = true;
		} catch(UnknownHostException e) {
			Log.e(TAG, "127.0.0.1 is unkown server!");
		} catch(Exception e) {
			e.printStackTrace();
		} 
		
		finally {
			try { 
				socket.close();
			} catch(Exception e) {
				e.printStackTrace();
			}
		}
		return result;
	}
}
