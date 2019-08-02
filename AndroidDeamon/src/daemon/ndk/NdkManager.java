package daemon.ndk;

import daemon.common.Device;
import daemon.logger.LogCenter;
import daemon.model.ProcessInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.shell.Shell;
import android.content.Context;
import android.os.Build;
import daemon.util.DeviceTouchInfo;
import daemon.util.RootHelper;
import daemon.util.ScreenSizeInfo;
import daemon.util.SendeventInfo;

public class NdkManager {
	public static void init(Context context, int resNdshId, String ndShFileName, int resNdMonitorId, String ndMonitorFileName) {
		NdkPipe.setNdInfo(resNdshId, ndShFileName, resNdMonitorId, ndMonitorFileName);
		NdkPipe.getInstance(context);
	}

	private static final int MULTIPLE  = 5;
	public static final int  HEART_KEY =0;
	public static final int  UPGRADE_KEY =1;
	private static int sKey =1;
	public static synchronized int getKey(){  
		sKey++;
		while(sKey==HEART_KEY||sKey==UPGRADE_KEY||sKey==-1){
			sKey++;
		}
		return sKey;
	}

	public static byte[] getDirInfo(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.GET_DIR, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 3000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}
		
		return result;
	}
	
	//新增接口，获取目录信息的时候，获取目录子目录是否可写属性
	public static byte[] getDirInfoSubWritable(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.GET_DIR_SUB_WRITABLE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 3000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}

		return result;
	}

	public static byte[] getFileAttr(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.GET_FILE_ATTR, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 3000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}
		return result;
	}
	
	//新增接口，获取文件属性中增加文件是否可写属性
	public static byte[] getFileAttrWritable(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.GET_FILE_ATTR_WRITABLE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 3000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}
		return result;
	}

	public static byte[] makeDir(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.MKDIR, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 1000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}

		return result;
	}

	public static byte[] remove(Context context, String path) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			DataPackage dataPackage = new DataPackage(BusinessType.REMOVE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 1000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}
		return result;
	}

	public static byte[] move(Context context, String oldPath, String newPath) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(oldPath);
			writer.Write(newPath);
			DataPackage dataPackage = new DataPackage(BusinessType.MOVE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataPackage, 1000*MULTIPLE,key);
			if (dataReceive != null)
				result = dataReceive.getData();
		}
		return result;
	}

	public static byte[] copy(Context context, String oldPath, String newPath) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(oldPath);
			writer.Write(newPath);
			DataPackage dataSend = new DataPackage(BusinessType.COPY, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 3000*MULTIPLE,key);
			if (dataReceive != null) {
				result = dataReceive.getData();
			}
		}
		return result;
	}

	public static byte[] srceenshot(Context context, boolean toBitmap565) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(toBitmap565);
			DataPackage dataSend = new DataPackage(BusinessType.SCREENSHOT, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 5000*MULTIPLE,key);
			if (dataReceive != null) {
				result = dataReceive.getData();
			}
		}

		return result;
	}

	public static byte[] readFile(Context context, String path) {
		return readFile(context, path, -1);
	}

	public static byte[] readFile(Context context, String path, int length) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			writer.Write(length);
			DataPackage dataSend = new DataPackage(BusinessType.READ_FILE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 10000*MULTIPLE,key);
			if (dataReceive != null) {
				result = dataReceive.getData();
			}
		}

		return result;
	}

	public static boolean install(Context context, String packagePath, boolean isLockApp, boolean isRecover, int installLocation) {
		boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(packagePath);
			writer.Write(isLockApp);
			writer.Write(isRecover);
			writer.Write(installLocation);
			DataPackage dataSend = new DataPackage(BusinessType.INSTALL, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = NdkPipe.getInstance(context).execForResult(dataSend, 60000,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if (reader.ReadInt32() == 1) {
					result = true;
				}
			}
		}

		return result;
	}

	public static boolean uninstall(Context context, String packageName, boolean isKeepData) {
		boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(packageName);
			writer.Write(isKeepData);
			DataPackage dataSend = new DataPackage(BusinessType.UNINSTALL, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 60000,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if (reader.ReadInt32() == 1) {
					result = true;
				}
			}
		}

		return result;
	}
	
	/**
	 * 卸载系统程序，实际操作是重挂载/system为读写后，删除apk文件
	 * @param context
	 * @param apkPath 系统程序apk文件的路径
	 * @param type /system的文件系统格式
	 * @param source /system对应的设备名
	 * @return 卸载系统程序成功返回true,失败返回false
	 */
	public static boolean uninstallSystemApp(Context context, String packageName, String apkPath, String type, String source, String target) {
		boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(packageName);
			writer.Write(apkPath);
			writer.Write(type);
			writer.Write(source);
			writer.Write(target);
			DataPackage dataSend = new DataPackage(BusinessType.UNINSTALL_SYSTEM_APP, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 60* 1000,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				result = reader.ReadInt32() == 1 ? true : false;
			}
		} 
		
		return result;
	}

	public static ProcessInfo getProcessInfo(Context context) {
		ProcessInfo result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			DataPackage dataSend = new DataPackage(BusinessType.GET_PROCESS_INFO, writer.getBaseStream().ToArray(), false);

			DataPackage dataReceive = pipe.execForResult(dataSend, 3000*MULTIPLE,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				result = new ProcessInfo(reader);
			}
		}

		return result;
	}

	public static boolean isRunning(Context context) {
		//以前版本中如果NDSH没有运行成功，就走adb，现在的版本如果没有运行成功，走jni，对于PC端来说
		//都是一样的调用（走守护），所以该状态一直返回true。注意，这只是为了和旧版兼容, 
		//以前守护内部对ndsh的判断改用isNdshActualRunning方法 
		
		return true;
	}
	
	/**
	 * 该方法只给守护内部判断使用，如果该方法返回true，那么ndsh一定已root
	 * @param context
	 * @return
	 */
	public static boolean isNdshActualRunning(Context context) {
		boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			result = pipe.isNdshRunning();
		}
		
		return result;
	}

	public static boolean isRunInRoot(Context context) {

		if(RootHelper.isMiuiNoRoot()){
			return false;
		}
		/*boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			result = pipe.isRunInRoot();
		}
		
		return result;*/
		
		//旧版中判断是否root要等NDSH运行起来后做判断，新版判断改为只要/system/bin/ndsh存在，就认为root
		boolean result = false;
		Shell sh = Shell.getInstance();
		String cmd = "ls -l /system/bin/" + NdkPipe.GetNdshFileName();
		String cmdResult = sh.execForString(cmd);
		if (cmdResult.contains("No such file or directory")) {
			result = false;
		}
		else {
			result = true;
		}
		
		// 对应Android4.4，同时要判断monitor是否跑起来
		if(Device.getIntSdkVersion() >= 19 && result){
			// NdMonitor运行，就说明授权成功
			result = NdkPipe.IsProcExist(NdkPipe.GetNdMonitorFileName());
		}
		
		// 对应5.0，如果SEAndorid没关闭，对应的权限也是不够的
		if(Device.getIntSdkVersion() >= 21 && result){
			result = NdkPipe.IsCloseSEAndroid();
		}		
		
		return result;
	}
	

	public static boolean suAuthorize(Context context) {
		boolean result = false;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			result = pipe.initNdshSuFileByShell();
		}
		
		return result;
	}

	public static byte[] chmod(Context context, String path, int mode){		
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			writer.Write(mode);
			DataPackage dataSend = new DataPackage(BusinessType.CHMOD, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 3000*MULTIPLE,key);
			if (dataReceive != null) 
			    return dataReceive.getData();			
		}

		return null;
	}
	
	public static byte[] chownAndGrp(Context context, String path, int uid, int gid){
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			writer.Write(uid);
			writer.Write(gid);
			DataPackage dataSend = new DataPackage(BusinessType.CHOWN_AND_GRP, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 3000*MULTIPLE,key);
			if (dataReceive != null) {
				return dataReceive.getData();
			}			
		}

		return null;
	}
	
	public static void quit(Context context) {
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			pipe.quit();
			pipe.clearInstace();
		}
	}	

	public static byte[] srceenshotJpeg(Context context,int quaity) {
		byte[] result = null;
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(quaity);
			DataPackage dataSend = new DataPackage(BusinessType.SCREENSHOTJPEG, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 5000*MULTIPLE,key);
			if (dataReceive != null) {
				result = dataReceive.getData();
			}
		}
		return result;
	}
	public static void sendevent(Context context,DeviceTouchInfo mDeviceTouchInfo, SendeventInfo sendeventInfo) {
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			/*
			 * event
			 * Tracking
			 * isSynaptics
			 * has3A 是否有003a代替0030
			 * hasKeydownEvent 是否有1 14a按键事件
			 * 
			 * action
			 * 1 num,.......(x,y).....
			 * 2 (x,y)
			 */
			writer.Write(mDeviceTouchInfo.getEvent());
			writer.Write(mDeviceTouchInfo.isHasTracking());
			writer.Write(mDeviceTouchInfo.isSynaptics());
			writer.Write(mDeviceTouchInfo.has3A());
			writer.Write(mDeviceTouchInfo.hasKeydownEvent());
			int action=sendeventInfo.getAction();
			writer.Write(action);
			writer.Write(sendeventInfo.getX());
			writer.Write(sendeventInfo.getY());
			DataPackage dataSend = new DataPackage(BusinessType.SENDEVENT, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 5000*MULTIPLE,key);
			if (dataReceive != null) {
				
			}
		}
	}
	public static void execShell(Context context,String cmd) {
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(cmd);
			DataPackage dataSend = new DataPackage(BusinessType.EXECUTESHELL, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 1* 1000*MULTIPLE,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					
				}
			}
		} 
	}
	
	public static String execShellforStr(Context context,String cmd) {
		String result="";
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(cmd);
			DataPackage dataSend = new DataPackage(BusinessType.EXECUTESHELLFORSTR, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 60* 1000,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					result=reader.ReadString();
				}
			}
		} 
        return result;
	}
	
	public static ScreenSizeInfo getScreenSizeInfo(Context context) {
		ScreenSizeInfo info=new ScreenSizeInfo();
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			DataPackage dataSend = new DataPackage(BusinessType.SCREENSIZE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 60* 1000,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					info.setWidth(reader.ReadInt32());
					info.setHeight(reader.ReadInt32());
				}
			}
		} 
		return info;
	}

	public static boolean makeFile(Context mContext, String mTmpFilePath) {
		NdkPipe pipe = NdkPipe.getInstance(mContext);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(mTmpFilePath);
			DataPackage dataSend = new DataPackage(BusinessType.MAKE_FILE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 1* 1000*MULTIPLE,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					return true;
				}
			}
		} 
		return false;
	}

	public static boolean renameFile(Context mContext, String oldfile,
			String newfile) {
		NdkPipe pipe = NdkPipe.getInstance(mContext);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(oldfile);
			writer.Write(newfile);
			DataPackage dataSend = new DataPackage(BusinessType.RENAME_FILE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 1* 1000*MULTIPLE,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					return true;
				}
			}
		} 
		return false;
		
	}

	public static boolean writeFile(Context mContext, String path,
			byte[] array, int count) {
		NdkPipe pipe = NdkPipe.getInstance(mContext);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			writer.Write(count);
			writer.Write(array);
			DataPackage dataSend = new DataPackage(BusinessType.WRITE_FILE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 1* 1000*MULTIPLE,key);
			if (dataReceive != null) {
				ByteReader reader = new ByteReader(new MemoryStream(dataReceive.getData()));
				if(reader.ReadInt32()==1){
					return true;
				}
			}
		} 
		return false;
	}

	public static byte[] readFileData(Context context, String path, int num,
			int readsize) {
		NdkPipe pipe = NdkPipe.getInstance(context);
		if (pipe != null) {
			ByteWriter writer = new ByteWriter();
			int key = getKey();
			writer.Write(key);
			writer.Write(path);
			writer.Write(num);
			writer.Write(readsize);
			DataPackage dataSend = new DataPackage(BusinessType.READDATA_FILE, writer.getBaseStream().ToArray(), false);
			DataPackage dataReceive = pipe.execForResult(dataSend, 1* 1000*MULTIPLE,key);
			if (dataReceive != null) {
				return dataReceive.getData();
			}
		} 
		return null;
	}
}
