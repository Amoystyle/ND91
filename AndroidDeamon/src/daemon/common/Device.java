package daemon.common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import daemon.logger.LogCenter;
import daemon.shell.Su;
import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.PixelFormat;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.view.WindowManager;
import android.widget.Toast;

@SuppressLint("NewApi")
public class Device {
	
	public static long getAvailableMemory(Context context) {
		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
		MemoryInfo mi = new MemoryInfo();
		am.getMemoryInfo(mi);
		return mi.availMem;
	}

	public static long getTotalMemory(Context context) {
		Map<String, Long> map = getMemInfo();
		long totalMemory = map.get("MemTotal") == null ? 0 : map.get("MemTotal");
		return totalMemory;
	}

	private static Map<String, Long> getMemInfo() {
		Map<String, Long> mapResult = new HashMap<String, Long>();
		String filePath = "/proc/meminfo";
		String strPattern = "(\\S+):\\s+(\\d+)\\s+\\S+";
		FileReader reader = null;
		try {
			Pattern pattern = Pattern.compile(strPattern);
			reader = new FileReader(filePath);
			BufferedReader localBufferedReader = new BufferedReader(reader, 8192);
			String line = null;
			while ((line = localBufferedReader.readLine()) != null) {
				Matcher matcher = pattern.matcher(line);
				if (matcher.find()) {
					mapResult.put(matcher.group(1), Long.valueOf(matcher.group(2)));
				}
			}
		}
		catch (Exception e) {
		}
		finally {
			if (reader != null) {
				try {
					reader.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return mapResult;
	}

	public static FileSpaceInfo getDataSpaceInfo() {
		File fileData = Environment.getDataDirectory();
		return new FileSpaceInfo(fileData);
	}

	public static FileSpaceInfo getSystemSpaceInfo() {
		File fileSystem = Environment.getRootDirectory();
		return new FileSpaceInfo(fileSystem);
	}

	public static FileSpaceInfo getExternalStorageInfo() {
		File fileExternalStorage = Environment.getExternalStorageDirectory();
		return new FileSpaceInfo(fileExternalStorage);
	}

	public static Map<String, FileSpaceInfo> getRootDirectSpaceInfo() {
		Map<String, FileSpaceInfo> mapResult = new HashMap<String, FileSpaceInfo>();
		File fileRoot = new File("/");
		File[] files = fileRoot.listFiles();
		for (File file : files) {
			if (file.isDirectory()) {
				FileSpaceInfo info = new FileSpaceInfo(file);
				mapResult.put(file.getPath(), info);
			}
		}
		return mapResult;
	}

	public static long getFileSize(String strPath) {
		long size = 0;
		try {
			File file = new File(strPath);
			if (file.exists()) {
				if (file.isDirectory()) {
					String[] strFileSub = file.list();
					for (String strFileItem : strFileSub) {
						size += getFileSize(strFileItem);
					}
				}
				else {
					size += file.length();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return size;
	}

	public static Map<String, Integer> getDisplayMetrics(Context context) {
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);
		int width = metrics.widthPixels;
		int height = metrics.heightPixels;
		Map<String, Integer> mapResult = new HashMap<String, Integer>();
		mapResult.put("width", width);
		mapResult.put("height", height);
		return mapResult;
	}

	public static int getCPUMaxFreq() {
		Su su = Su.getInstance();

		String cmd = "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq";
		int freq = 0;
		try {
			String strFreq = null;
			for (int i = 0; i < 3 && strFreq == null; i++) {
				strFreq = su.exec_with_str_result(cmd, 1000);
			}
			if (strFreq != null) {
				LogCenter.error("Device", "CpuMaxFreq:" + strFreq);
				int index = strFreq.indexOf("\n");
				if (index != -1) {
					strFreq = strFreq.substring(0, index);
				}

				freq = Integer.parseInt(strFreq);
			}
		}
		catch (NumberFormatException e) {
			e.printStackTrace();
			LogCenter.error("get cup max freq", e.getMessage());
		}
		return freq;
	}

	public static String getIMSI(Context context) {
		String imsiResult = "";
		TelephonyManager manager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		if (manager.getSimState() == android.telephony.TelephonyManager.SIM_STATE_READY) {
			imsiResult = manager.getSubscriberId();
		}
		return imsiResult;
	}

	public static String getIMEI(Context context) {
		TelephonyManager manager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		String imei = manager.getDeviceId();
		return imei;
	}

	public static String getExternalStorageState() {
		return android.os.Environment.getExternalStorageState();
	}

	public static String getBasebandVersion() {
		return getSystemProperties("gsm.version.baseband");
	}

	public static String getKernelVersion() {
		return getFormattedKernelVersion();
	}

	public static String getInternalVersion() {
		return getSystemProperties("ro.build.description");
	}

	public static String getMacAddress(Context context) {
		String mac = "UnKnown";
		WifiManager wifi = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		if (wifi != null) {
			WifiInfo info = wifi.getConnectionInfo();
			if (info != null) {
				mac = info.getMacAddress();
				if (mac == null)
					mac = "UnKnown";
			}
		}
		return mac;
	}

	public static boolean getWifiState(Context context) {
		WifiManager manager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		return manager.isWifiEnabled();
	}

	public static String getWifiSSID(Context context) {
		WifiManager manager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = manager.getConnectionInfo();
		return info.getSSID();
	}

	private static String getFormattedKernelVersion() {
		String procVersionStr;

		try {
			BufferedReader reader = new BufferedReader(new FileReader("/proc/version"), 256);
			try {
				procVersionStr = reader.readLine();
			}
			finally {
				reader.close();
			}

			final String PROC_VERSION_REGEX = "\\w+\\s+" + /* ignore: Linux */
			"\\w+\\s+" + /* ignore: version */
			"([^\\s]+)\\s+" + /* group 1: 2.6.22-omap1 */
			"\\(([^\\s@]+(?:@[^\\s.]+)?)[^)]*\\)\\s+" + /*
														 * group 2: (xxxxxx@xxxxx .constant)
														 */
			"\\(.*?(?:\\(.*?\\)).*?\\)\\s+" + /* ignore: (gcc ..) */
			"([^\\s]+)\\s+" + /* group 3: #26 */
			"(?:PREEMPT\\s+)?" + /* ignore: PREEMPT (optional) */
			"(.+)"; /* group 4: date */

			Pattern p = Pattern.compile(PROC_VERSION_REGEX);
			Matcher m = p.matcher(procVersionStr);

			if (!m.matches()) {
				LogCenter.error("sysInfo", "Regex did not match on /proc/version: " + procVersionStr);
				return "Unavailable";
			}
			else if (m.groupCount() < 4) {
				LogCenter.error("sysInfo", "Regex match on /proc/version only returned " + m.groupCount() + " groups");
				return "Unavailable";
			}
			else {
				return (new StringBuilder(m.group(1)).append("\n").append(m.group(2)).append(" ").append(m.group(3)).append("\n").append(m.group(4))).toString();
			}
		}
		catch (IOException e) {
			LogCenter.error("sysInfo", "IO Exception when getting kernel version for Device Info screen", e);

			return "Unavailable";
		}
	}

	public static String getSystemProperties(String key) {
		String value = "";

		try {
			Class<?> classType = Class.forName("android.os.SystemProperties");
			Constructor<?> con = classType.getDeclaredConstructor();
			con.setAccessible(true);
			Object classThread = con.newInstance();

			Method m = classType.getMethod("get", new Class[] {String.class, String.class});

			Object obj = m.invoke(classThread, new Object[] {key, "UnKnown"});
			value = obj.toString();

		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return value;
	}

	public static boolean canCleanMemory(Context context) {
		final File fileTmp = new File("/data/local/tmp");
		final File fileData = new File("/data/data");
		if (fileTmp.canWrite() && fileData.canWrite()) {
			return true;
		}
		else {
			Su su = Su.getInstance();

			if (su.getSuAuthorize(context)) {
				su.exec("mount -o remount -w /data");
				su.exec("chmod 0777 /data/local/tmp");
				su.exec("chmod 0777 /data/data");
				return true;
			}
		}

		return false;
	}

	public static long cleanMemory(Context context) {

		final File fileTmp = new File("/data/local/tmp");
		final File fileData = new File("/data/data");
		long clearSize = 0;

		if (fileTmp.canWrite()) {
			clearSize += deleteFile(fileTmp);
		}
		if (fileData.canWrite()) {
			File[] lstData = fileData.listFiles();
			List<String> lstApp = getApps(context);
			for (File file : lstData) {
				if (!lstApp.contains(file.getName()))
					clearSize += deleteFile(fileData);
			}
		}

		return clearSize;
	}

	public static long deleteFile(File file) {
		if (!file.exists()) {
			return 0;
		}

		long size = 0;
		try {
			if (file.isDirectory()) {
				File[] subFiles = file.listFiles();
				if (subFiles != null) {
					for (File subFile : subFiles) {
						size += deleteFile(subFile);
					}
				}
				file.delete();
			}
			else {
				long tmpSize = file.length();
				if (file.delete())
					size += tmpSize;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		return size;
	}

	public static List<String> getApps(Context context) {
		List<String> apps = new ArrayList<String>();
		PackageManager manager = context.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		for (PackageInfo info : infos) {
			apps.add(info.packageName);
		}
		return apps;
	}

	public static int getIntSdkVersion() {
		int sdk = 8;
		try {
			sdk = Integer.parseInt(android.os.Build.VERSION.SDK);
		}
		catch (Exception e) {
			e.printStackTrace();
			sdk = android.os.Build.VERSION.SDK_INT;
		}

		return sdk;
	}

	public static String getStringSdkVersion() {
		int sdk = getIntSdkVersion();

		String version = "";

		if (sdk <= 17) {
			switch (sdk) {
				case 1:
					version = "1.0";
					break;
				case 2:
					version = "1.1";
					break;
				case 3:
					version = "1.5";
					break;
				case 4:
					version = "1.6";
					break;
				case 5:
					version = "2.0";
					break;
				case 6:
					version = "2.0.1";
					break;
				case 7:
					version = "2.1";
					break;
				case 8:
					version = "2.2";
					break;
				case 9:
					version = "2.3";
					break;
				case 10:
					version = "2.3.3";
					break;
				case 11:
					version = "3.0";
					break;
				case 12:
					version = "3.1";
					break;
				case 13:
					version = "3.2";
					break;
				case 14:
					version = "4.0";
					break;
				case 15:
					version = "4.0.3";
					break;
				case 16:
					version = "4.1.2";
					break;
				case 17:
					version = "4.2";
					break;
			}
		}
		else {
			version = "4.2";
		}

		return version;
	}

	// 获取设备型号
	public static String getModel() {
		return android.os.Build.MODEL;
	}

	

}
