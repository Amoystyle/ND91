package daemon.provider.system;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import com.baidu.android.bba.common.util.CommonParam;



import daemon.common.Common;
import daemon.common.Device;
import daemon.common.GpuInfoHelper;
import daemon.common.GpuInfoHelper.GpuInfo;
import daemon.common.MountPartitionParam;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.provider.pim.SmsSendHelper;
import daemon.receiver.BatteryReceiver;
import daemon.receiver.NewSmsReceiver;
import daemon.shell.Su;
import daemon.util.ConfigManager;
import daemon.util.ConstEvtKey;
import daemon.util.DeviceTouchInfo;
import daemon.util.KeyguardHelper;
import daemon.util.KillProcessHelper;
import daemon.util.FileHelper;
import daemon.util.SendeventInfo;
import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Process;
import android.os.StatFs;
import android.os.Debug.MemoryInfo;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.text.ClipboardManager;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;


public class SysInfoProvider implements IProvider {
	private String TAG = getClass().getName();
	
	private Context mContext;
	private BatteryReceiver batteryReceiver;
	private NewSmsReceiver mSmsReceiver;
	private DeviceTouchInfo mDeviceTouchInfo;
	private String clipInfo;
	private char[] lockGetCopyInfo = new char[0];

	public SysInfoProvider(Context context) {
		mContext = context;
		batteryReceiver = new BatteryReceiver();
		mSmsReceiver = new NewSmsReceiver();
		context.registerReceiver(batteryReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		if(Device.getIntSdkVersion()>=19){
			context.registerReceiver(mSmsReceiver, new IntentFilter("android.provider.Telephony.SMS_RECEIVED"));
		}
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		int action = reader.ReadInt32();
		// LogCenter.debug("SystemInfo", "action=" + action);

		switch (action) {
			case 1:// IMEI
				writer.Write(action);
				GetIMEI(reader, writer);
				break;
			case 2:// IMSI
				writer.Write(action);
				GetIMSI(reader, writer);
				break;
			case 3:
				GetSDCardState(reader, writer);
				break;
			case 4:
				getExternalSDCardState(writer);
				break;
			case 5:
				getExternalSDCardPath(writer);
				break;
			case 6:
				GetICCID(reader, writer);
				break;
			case 11: // Query
				QueryPhoneNameByKey(reader, writer);
				break;
			case 12: // Insert(first delete then Insert )
				InsertPhoneName(reader, writer);
				break;
			case 13: // Update(first delete then Insert )
				InsertPhoneName(reader, writer);
				break;
			case 14:
				getBattery(context);
				break;
			case 15:
				writer.Write(action);
				GetDisplayMetrics(reader, writer);
				break;
			case 20:
				File dataDirectory = Environment.getDataDirectory();
				getSpaceInfo(dataDirectory, writer);
				break;
			case 21:
				File storageDirectory = Environment.getExternalStorageDirectory();
				getSpaceInfo(storageDirectory, writer);
				getBattery(context);
				break;
			case 22:
				getRootDirectSpaceInfo(writer);
				break;
			case 23:
				String strPath = reader.ReadString();
				strPath = getRecentlyExistingDir(strPath);
				File file = null;
				if (strPath != null) {
					file = new File(strPath);
				}
				getSpaceInfo(file, writer);
				break;
			case 24:
				getBasebandVersion(writer);
				break;
			case 25:
				getKernelVersion(writer);
				break;
			case 26:
				getMacAddress(writer);
				break;
			case 27:
				getWifiState(writer);
				break;
			case 28:
				getReleaseVersion(writer);
				break;
			case 29:
				writer.Write(getIsRoot());
				break;
			case 30:
				writer.Write(isSystemAppCanWrite());
				break;
			case 31:
				writer.Write(remountSystem());
				break;
			case 32:
				writer.Write(deleteSystemApp(reader));
				break;
			case 33:
				writer.Write(Device.getModel());
				break;
			case 34:
				writer.Write(Device.getStringSdkVersion());
				break;
			case 35:
				writer.Write(getBrand());
				break;
			case 36:
				copyToClipboard(reader);
				break;
			case 37:
				startPandaSpace(writer);
				break;
			case 38:
				KeyguardHelper.getInstance().lightScreen(mContext);
				break;
			case 39:
				getSimState(writer);
				break;
			case 40:
				getIsAirPlaneMode(writer);
				break;
			case 41:
				touch(reader, writer);
				break;
			case 42:
				keyboard(reader, writer);
				break;
			case 43:
				sendKey(reader, writer);
				break;
			case 44:
				getInfoFromClipboard(writer);
				break;
			case 45:
				getRomBuildVersion(writer);
				break;
			case 46:
				killProcess(reader, writer);
				break;
			case 47:
				getGpuInfo(writer);
				break;
			case 48:
				getCurrTime(writer);
				break;
			case 49:
				writer.Write(CommonParam.getCUID(mContext));
				break;
			case 50:
				getProcessInfo(reader, writer);
				break;
			case 51:
				killProcessByNew(reader, writer);
				break;
			case 52:
				getSystemMemoryInfo(reader, writer);
				break;
			case 53:
				writer.Write(Device.getIntSdkVersion());
				break;
		}
	}
	
	private void getSystemMemoryInfo(ByteReader reader, ByteWriter writer) {
		writer.Write(1);
		writer.Write(Device.getAvailableMemory(mContext));
		writer.Write(Device.getTotalMemory(mContext)*1024);
	}

	private void killProcessByNew(ByteReader reader, ByteWriter writer) {
		try {
			int count = reader.ReadInt32();
			ActivityManager activityManager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
			long old = getMemoryavailMem(activityManager);
			ArrayList<Integer> list =new ArrayList<Integer>();
			for (int i = 0; i < count; i++) {
				int pid = reader.ReadInt32();
				String processName = reader.ReadString();
				if (Device.getIntSdkVersion() < 8) {
					activityManager.restartPackage(processName);
				}
				else {
					KillProcessHelper.killProcess(activityManager, processName);
				}
				Process.killProcess(pid);
				list.add(pid);

			}
			int num = list.size();
			long avial = getMemoryavailMem(activityManager) - old;
			if (avial < 0) {
				avial = 0;
			}
			writer.Write(1);
			writer.Write(avial);
			List<RunningAppProcessInfo> rurlist = activityManager.getRunningAppProcesses();
			
			int runProcessCount = rurlist.size();
			for (int i = 0; i < runProcessCount; i++) {
				if(list.contains(rurlist.get(i).pid)){
					num--;
				}
			}
			writer.Write(num);
			
		} catch (Exception e) {
			writer.Write(0);
		}


		
	}

	
	
	
	@SuppressLint("NewApi")
	private void getProcessInfo(ByteReader reader, ByteWriter writer) {
		ActivityManager activityManager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> list = activityManager.getRunningAppProcesses();
		int runProcessCount = list.size();
		ArrayList<String> whiteList = new ArrayList<String>();
		int count = reader.ReadInt32();
		if (count > 0) {
			whiteList.add(reader.ReadString());
		}
		ArrayList<RunningAppProcessInfo> runList = new ArrayList<RunningAppProcessInfo>();
		
		for (int i = 0; i < runProcessCount; i++) {
			RunningAppProcessInfo info = list.get(i);
			if (whiteList.contains(info.processName)) {
				continue;
			}
			if (info.pid == Process.myPid()) {
				continue;
			}
			if (info.processName.contains(mContext.getPackageName())) {
				continue;
			}
			runList.add(info);

		}
		runProcessCount = runList.size();
		writer.Write(runProcessCount);
		for (int i = 0; i < runProcessCount; i++) {
			RunningAppProcessInfo info = runList.get(i);
			int [] ii ={info.pid};
			MemoryInfo[] memoryinfo = activityManager.getProcessMemoryInfo(ii);
			writer.Write(info.pid);
			writer.Write(info.processName);
			String packname="";
			String[] pkgList =info.pkgList;
			if(pkgList.length>0){
				packname = pkgList[0];
			}
			writer.Write(packname);
			writer.Write(memoryinfo[0].getTotalPss());
		}

		
	}

	private void getCurrTime(ByteWriter writer) {
		writer.Write(System.currentTimeMillis());
	}

	private void getGpuInfo(ByteWriter writer) {
		GpuInfo info = GpuInfoHelper.getInstance().getGpuInfo(mContext);		
		if(info != null){
			writer.Write(1);
			writer.Write(info.getVendor());
			writer.Write(info.getRenderer());
		}else{
			writer.Write(0);
		}
	}


	
	private void getRomBuildVersion(ByteWriter writer) {
		String ver = Device.getInternalVersion();
		writer.Write(ver);
	}

	private void killProcess(ByteReader reader, ByteWriter writer) {
		try {
			int count = reader.ReadInt32();
			ArrayList<String> whiteList = new ArrayList<String>();
			if (count > 0) {
				whiteList.add(reader.ReadString());
			}
			ActivityManager activityManager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
			List<RunningAppProcessInfo> list = activityManager.getRunningAppProcesses();
			int runProcessCount = list.size();
			long old = getMemoryavailMem(activityManager);
			for (int i = 0; i < runProcessCount; i++) {
				RunningAppProcessInfo info = list.get(i);
				if (whiteList.contains(info.processName)) {
					continue;
				}
				if (info.pid == Process.myPid()) {
					continue;
				}
				if (info.processName.contains(mContext.getPackageName())) {
					continue;
				}
				if (Device.getIntSdkVersion() < 8) {
					activityManager.restartPackage(info.processName);
				}
				else {
					KillProcessHelper.killProcess(activityManager, info.processName);
				}
				Process.killProcess(info.pid);

			}
			long avial = getMemoryavailMem(activityManager) - old;
			if (avial < 0) {
				avial = 0;
			}
			writer.Write(1);
			writer.Write(avial);
			int newRunProcessCount = runProcessCount - activityManager.getRunningAppProcesses().size();
			if (newRunProcessCount<0) {
				newRunProcessCount = 0;
			}
			writer.Write(newRunProcessCount);
		}catch (Exception e) {
			writer.Write(0);
		}
	}

	private long getMemoryavailMem(ActivityManager activityManager) {
		ActivityManager.MemoryInfo outInfo = new ActivityManager.MemoryInfo();
		activityManager.getMemoryInfo(outInfo);
		return outInfo.availMem;
	}

	private synchronized void getInfoFromClipboard(ByteWriter writer) {
		clipInfo = null;
		try {
			Handler handler = new Handler(mContext.getApplicationContext().getMainLooper());
			handler.post(new Runnable() {
				@Override
				public void run() {
					try{
						ClipboardManager cmb = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);
						if(cmb!=null&&cmb.getText()!=null){
							String info = cmb.getText().toString();
							if (info == null)
								info = "";
							clipInfo = info;
						}else{
							clipInfo = "";
						}			
					}catch (Exception e) {
						clipInfo = "";
					}
					
					
					synchronized (lockGetCopyInfo) {
						lockGetCopyInfo.notifyAll();
					}
				}
			});
		}
		catch (Exception e) {

		}
		synchronized (lockGetCopyInfo) {
			try {
				lockGetCopyInfo.wait(2000);
			} catch (InterruptedException e) {
				clipInfo ="";
			}
		}
		writer.Write(clipInfo);
	}

	private void sendKey(ByteReader reader, ByteWriter writer) {
		int key = reader.ReadInt32();
		// 3 HOME
		// 4 BACK
		// 5 CALL
		// 6 END_CALL
		// 24 KEYCODE_VOLUME_UP
		// 25 KEYCODE_VOLUME_DOWN
		// 26 POWER
		// 82 MENU
		NdkManager.execShell(mContext, "input keyevent " + key);
	}

	private void keyboard(ByteReader reader, ByteWriter writer) {
		char keyValue = reader.ReadChar();
		int key = getKeyCode(keyValue);
		NdkManager.execShell(mContext, "input keyevent " + key);
	}

	private int getKeyCode(char c) {
		int code = 0;
		if (Character.isLetter(c))
			code = ConstEvtKey.KEYCODE_A + (Character.toLowerCase(c) - 'a');
		if (Character.isDigit(c))
			code = ConstEvtKey.KEYCODE_0 + (c - '0');

		if (c == '\n')
			code = ConstEvtKey.KEYCODE_ENTER;

		if (c == ' ')
			code = ConstEvtKey.KEYCODE_SPACE;

		if (c == '\b')
			code = ConstEvtKey.KEYCODE_DEL;

		if (c == '\t')
			code = ConstEvtKey.KEYCODE_TAB;

		if (c == '/')
			code = ConstEvtKey.KEYCODE_SLASH;

		if (c == '\\')
			code = ConstEvtKey.KEYCODE_BACKSLASH;

		if (c == ',')
			code = ConstEvtKey.KEYCODE_COMMA;

		if (c == ';')
			code = ConstEvtKey.KEYCODE_SEMICOLON;

		if (c == '.')
			code = ConstEvtKey.KEYCODE_PERIOD;

		if (c == '*')
			code = ConstEvtKey.KEYCODE_STAR;

		if (c == '+')
			code = ConstEvtKey.KEYCODE_PLUS;

		if (c == '-')
			code = ConstEvtKey.KEYCODE_MINUS;

		if (c == '=')
			code = ConstEvtKey.KEYCODE_EQUALS;
		return code;
	}

	private void touch(ByteReader reader, ByteWriter writer) {
		synchronized (this) {
			if (mDeviceTouchInfo == null) {
				mDeviceTouchInfo = new DeviceTouchInfo();
				if (!mDeviceTouchInfo.init(mContext)) {
					mDeviceTouchInfo = null;
					writer.Write(0);
					return;
				}
			}
		}
		SendeventInfo sendeventInfo = new SendeventInfo();
		int action = reader.ReadInt32();
		sendeventInfo.setAction(action);
		int x = mDeviceTouchInfo.getRealX(reader.ReadInt32());
		int y = mDeviceTouchInfo.getRealY(reader.ReadInt32());
		sendeventInfo.setX(x);
		sendeventInfo.setY(y);
		Log.e("action", action + "");
		NdkManager.sendevent(mContext, mDeviceTouchInfo, sendeventInfo);
		writer.Write(1);
	}

	private void getIsAirPlaneMode(ByteWriter writer) {
		int isAirplaneMode = Settings.System.getInt(mContext.getContentResolver(), Settings.System.AIRPLANE_MODE_ON, 0);
		writer.Write(isAirplaneMode);

	}

	private void getSimState(ByteWriter writer) {
		// TelephonyManager tm = (TelephonyManager)mContext.getSystemService(Context.TELEPHONY_SERVICE);//ȡ�����ϵͳ����
		// 0 δ֪״?1?7?1?7
		// 1 �޿�
		// 2 ��1?7ҪPIN����
		// 3 ��1?7ҪPUK����
		// 4 ��1?7ҪNetworkPIN����
		// 5 |��
		int state = SmsSendHelper.getSimCardState(mContext);
		writer.Write(state);
	}

	/**
	 * ��ٱ��u1?7
	 */
	private void startPandaSpace(ByteWriter writer) {
		String packageName = "com.dragon.android.pandaspace";
		PackageManager pm = mContext.getPackageManager();

		try {
			Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
			List<ResolveInfo> apps = pm.queryIntentActivities(mainIntent, 0);
			for (ResolveInfo info : apps) {
				if (packageName.equals(info.activityInfo.applicationInfo.packageName)) {
					ComponentName componentName = new ComponentName(info.activityInfo.applicationInfo.packageName, info.activityInfo.name);
					mainIntent.setComponent(componentName);
					mainIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
					mContext.startActivity(mainIntent);

					writer.Write(1);
					return;
				}

			}
		}
		catch (Exception e) {
		}

		writer.Write(0);
	}

	private void copyToClipboard(ByteReader reader) {
		final String info = reader.ReadString();
		try {
			Handler handler = new Handler(mContext.getApplicationContext().getMainLooper());
			handler.post(new Runnable() {
				@Override
				public void run() {
					ClipboardManager cmb = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);
					cmb.setText(info);
				}
			});
		}
		catch (Exception e) {
		}

	}

	private String getBrand() {
		String brand = android.os.Build.BRAND;
		brand = brand == null ? "" : brand;
		return brand;
	}

	private void getRootDirectSpaceInfo(ByteWriter writer) {
		File fileRoot = new File("/");
		File[] files = fileRoot.listFiles();
		for (File file : files) {
			if (file.isDirectory()) {
				long all = getAllSpace(file);
				long free = getFreeSpace(file);
				long used = all - free;
				writer.Write(all);
				writer.Write(free);
				writer.Write(used);
				writer.Write(file.getPath());
			}
		}

	}

	private void getExternalSDCardState(ByteWriter writer) {
		
		boolean exist = false;
		String strSd = Common.getExternalSDCardPath(mContext); //getExternalSDCardPath();
		if (!TextUtils.isEmpty(strSd)) {
			exist = true;
		}
		writer.Write(exist);
	}

	//默认的sd卡是否是外置的sd卡
	private boolean isDSdcard(){
		try {
			File dataDirectory = Environment.getDataDirectory();
			long Allsize = getAllSpace(dataDirectory);
			long freesize = getFreeSpace(dataDirectory);
			
			File storageDirectory = Environment.getExternalStorageDirectory();
			long storeageSize = getAllSpace(storageDirectory);
			long storeageFreeSize = getFreeSpace(storageDirectory);
			
			if (storeageSize == 0 || Allsize - freesize == storeageSize - storeageFreeSize) {
				//没有sd卡 空间大小一致
				return false;
			}
			else
			{
				return true;
			}
		} catch (Exception e) {
			// TODO: handle exception 出现意外交由shell命令判断
			LogCenter.error("SysInfoProvider", "获取sd卡路径失败" + e.getMessage());
			return true;
		}
	}
	
	
	private void getExternalSDCardPath(ByteWriter writer) {
		//先判断sd卡是否有挂在 
		String sdState = android.os.Environment.getExternalStorageState();
		if (!sdState.equals("mounted") && !sdState.equals("shared") ) {
			writer.Write(""); //当sd卡不可用的时候写入空
		}
		else if(isDSdcard()) { //当获取的sd卡路径等于默认sd卡时，直接写入默认sd卡路径
			writer.Write(Environment.getExternalStorageDirectory().getAbsolutePath());
		}
		else{
			String strResult = Common.getExternalSDCardPath(mContext); //getExternalSDCardPath();
			writer.Write(strResult);
		}
		
	}

	
	/*
	 * private void getRootDirectSpaceInfo(ByteWriter writer) { Map<String, Map<String, Long>> map =
	 * SystemInfo.getRootDirectSpaceInfo(); Iterator<Map.Entry<String, Map<String, Long>>> it =
	 * map.entrySet().iterator(); while (it.hasNext()) { Map.Entry<String, Map<String, Long>> next = it.next();
	 * Map<String, Long> mapFile = next.getValue(); writer.Write(mapFile.get("all")); writer.Write(mapFile.get("free"));
	 * writer.Write(mapFile.get("used")); writer.Write(next.getKey()); } }
	 */

	private void getSpaceInfo(File file, ByteWriter write) {
		//要是文件不存在， 则调用StatFs构造函数会造成异常，所以这里判断要是不存在就创建
		long all = getAllSpace(file);
		long free = getFreeSpace(file);
		long used = all - free;
		write.Write(all);
		write.Write(free);
		write.Write(used);
		write.Write(file.getPath());
	}
	
	private String getRecentlyExistingDir(String strPath) {
		if (strPath.equals("")) {
			return null;
		}
		
		String strDir = Common.getCanonicalPath(strPath);
		if (strDir != null) {
			File fileDir = new File(strDir);
			while (true) {
				if (fileDir == null) {
					strDir = null;
					break;
				}
				else if (fileDir.exists()) {
					strDir = fileDir.getAbsolutePath();
					break;
				}
				else {
					fileDir = fileDir.getParentFile();
				}
			}
		}
		else {
			strDir = strPath;
		}
		
		return strDir;
	}

	private long getAllSpace(File file) {
		String path = file.getPath();
		StatFs sf = new StatFs(path);
		long blockCount = sf.getBlockCount();
		long blockSize = sf.getBlockSize();
		long result = blockCount * blockSize;
		return result > 0 ? result : 0;
	}

	private long getFreeSpace(File file) {
		String path = file.getPath();
		StatFs sf = new StatFs(path);
		long blockCount = sf.getAvailableBlocks();
		long blockSize = sf.getBlockSize();
		long result = blockCount * blockSize;
		return result > 0 ? result : 0;
	}

	private void GetDisplayMetrics(ByteReader reader, ByteWriter writer) {
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager) mContext.getSystemService(Service.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);
		int width = metrics.widthPixels;
		int height = metrics.heightPixels;
		writer.Write(width);
		writer.Write(height);
	}

	private void GetIMSI(ByteReader reader, ByteWriter writer) {
		TelephonyManager tManager = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		if (tManager.getSimState() == android.telephony.TelephonyManager.SIM_STATE_READY) {
			String imei = tManager.getSubscriberId();
			writer.Write(imei);
		}
		else
			writer.Write("");
	}

	private void GetIMEI(ByteReader reader, ByteWriter writer) {
		TelephonyManager tManager = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		String imsi = tManager.getDeviceId();
		if (imsi != null)
			writer.Write(imsi);
		else
			writer.Write("");
	}

	private void GetICCID(ByteReader reader, ByteWriter writer) {
		TelephonyManager tManager = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		String ICCID = "";
		if (tManager.getSimState() == android.telephony.TelephonyManager.SIM_STATE_READY) {
			ICCID = tManager.getSimSerialNumber();
		}
		if (ICCID != null) {
			writer.Write(ICCID);
		}
		else {
			writer.Write("");
		}
	}

	private void GetSDCardState(ByteReader reader, ByteWriter writer) {
		try {
			writer.Write(android.os.Environment.getExternalStorageState());
		}
		catch (Exception e) {
			e.printStackTrace();
		}

	}

	private void QueryPhoneNameByKey(ByteReader reader, ByteWriter writer) {
		String name = ConfigManager.getPhoneName(mContext);
		if (null == name || name.equals("")) {
			writer.Write(0);
		}
		else {
			writer.Write(1);
			writer.Write(name);
		}
	}

	private void InsertPhoneName(ByteReader reader, ByteWriter writer) {
		// 旧版接口有key，新街�1?7��没有
		String key = reader.ReadString();
		String name = key;
		if (reader.available() > 0)
			name = reader.ReadString();

		ConfigManager.setPhoneName(mContext, name);
		writer.Write(1);
	}

	private void getBattery(ProviderExecuteContext cxt) {
		cxt.getByteWriter().Write(batteryReceiver.getPower());
		LogCenter.error("SysInfoProvider", "power = " + batteryReceiver.getPower());
	}

	private void getBasebandVersion(ByteWriter writer) {
		writer.Write(getBaseBand());
	}

	private void getKernelVersion(ByteWriter writer) {
		writer.Write(Device.getKernelVersion());
	}

	private void getMacAddress(ByteWriter writer) {
		String mac = "UnKnown";
		WifiManager wifi = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
		if (wifi != null) {
			WifiInfo info = wifi.getConnectionInfo();
			if (info != null) {
				mac = info.getMacAddress();
				if (mac == null)
					mac = "UnKnown";
			}
		}

		writer.Write(mac);
	}

	private void getWifiState(ByteWriter writer) {
		WifiManager wifi = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
		if (wifi != null)
			writer.Write(wifi.isWifiEnabled() ? 1 : 0);

		writer.Write(0);
	}

	private void getReleaseVersion(ByteWriter writer) {
		LogCenter.error("sys release:", android.os.Build.VERSION.RELEASE);
		writer.Write(android.os.Build.VERSION.RELEASE);
	}

	private String getBaseBand() {
		return Device.getSystemProperties("gsm.version.baseband");
	}

	private boolean getIsRoot() {
		return Su.getInstance().hasSU();
	}


    //使用守护java方式可写或者ndsh方式可写，则均认为是可写
	private boolean isSystemAppCanWrite() {
//		String path = "/system/app/isSystemAppCanWritetestfile";
//		File sysFile = new File(path);
//		if(sysFile.exists()){
//			return sysFile.delete();
//		}else{
//			boolean result = sysFile.mkdirs();
//			sysFile.delete();
//			return result;
//		}
		//return MountHelper.isFileWrite(mContext,"/system/");
		return FileHelper.isFileCanWriteByJava("/system/app" )||FileHelper.isFileCanWriteByNdsh(mContext,"/system/app" );
	}

	private boolean remountSystem() {
		if (NdkManager.isNdshActualRunning(mContext)) {
			MountPartitionParam param = MountPartitionParam.createPartitionParam("/system/");
			if(param==null)
				return isSystemAppCanWrite();;
			String partition = param.getSource();
			String type = param.getFilesystemType();
			String format = "mount -o remount -w -t %1$s %2$s /system";
			if (!TextUtils.isEmpty(partition) && !TextUtils.isEmpty(type)) {
				String cmd = String.format(format, type, partition);
				NdkManager.execShell(mContext,cmd);
				NdkManager.execShell(mContext,"chmod 0777 /system/app");
			}
		}
		return isSystemAppCanWrite();
	}

	private boolean deleteSystemApp(ByteReader reader) {

		if (!isSystemAppCanWrite())
			return false;
		String appPath = reader.ReadString();
		File appFile = new File(appPath);
		appFile.delete();

		boolean deleteSuccess = !appFile.exists();
		return deleteSuccess;
	}

	@Override
	public int getBusiness() {
		return 14;
	}

	/*
	 * protected class BatteryReceiver extends BroadcastReceiver {
	 * 
	 * private int power;
	 * 
	 * public int getPower() { return power; }
	 * 
	 * @Override public void onReceive(Context context, Intent intent) { if
	 * (intent.getAction().equals(Intent.ACTION_BATTERY_CHANGED)) { int level = intent.getIntExtra("level", 0); int
	 * scale = intent.getIntExtra("scale", 100);
	 * 
	 * if (level > scale) level /= 10;
	 * 
	 * power = level * 100 / scale; LogCenter.error("BatteryBroadcastReceiver", "Level: " + String.valueOf(level) +
	 * " Scale: " + String.valueOf(scale) + " Power: " + String.valueOf(power)); } } }
	 */
}
