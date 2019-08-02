package daemon.common;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


import daemon.logger.LogCenter;
import daemon.shell.Shell;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.text.TextUtils;
import android.util.Log;

public class Common {
	public static String getFormatedFileSize(long size) {
		String formatedSize = "";
		DecimalFormat df = new DecimalFormat("#.00");

		long gbUnit = 1024 * 1024 * 1024;
		double gbSize = (double) size / gbUnit;
		if (gbSize > 1) {
			formatedSize = df.format(gbSize) + "GB";
		} else {
			long mbUnit = 1024 * 1024;
			double mbSize = (double) size / mbUnit;

			if (mbSize > 1) {
				formatedSize = df.format(mbSize) + "MB";
			} else {
				long kbUnit = 1024;
				double kbSize = (double) size / kbUnit;
				if (kbSize > 1) {
					formatedSize = df.format(kbSize) + "KB";
				} else {
					formatedSize = size + "Bytes";
				}
			}
		}

		return formatedSize;
	}
	
	public static boolean isProcessRunning(String processName, Context context) {
		if (TextUtils.isEmpty(processName)) {
			return false;
		}
		boolean result = false;
		try {
			ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
			List<RunningAppProcessInfo> listProcess = am.getRunningAppProcesses();
			Iterator<RunningAppProcessInfo> it = listProcess.iterator();
			while (it.hasNext()) {
				RunningAppProcessInfo processInfo = it.next();
				if (processName.equals(processInfo.processName)) {
					result = true;
					break;
				}
			}
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}
		return result;
	}
	
	public static void startComponentActivity(ComponentName component, Context context) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setComponent(component);
		context.startActivity(intent);
	}
	
	/**
	 * 获取path路径的真实路径。path或者path的父路径是符号链接，该方法将转为真实路径
	 * @param path
	 * @return
	 */
	public static String getCanonicalPath(String path) {
		String result = null;
		try {
			if (path.endsWith("/") && !path.equals("/")) {
				path = path.substring(0, path.length() - 1);
			}
			int index = path.length();
			if (index <= 1) {
				return path;
			}
			
			File file = new File(path);
			File folder = file.getParentFile();
			if (folder != null) {
				String folderPath = folder.getCanonicalPath();
				if (folderPath != null) {
					String name = file.getName();
					String canonicalPath = getCanonicalPath(folderPath);
					if (canonicalPath != null) {
						if (!canonicalPath.equals("/")) {
							result = canonicalPath + "/" + name;
						}
						else {
							result = canonicalPath + name;
						}
					}
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			
			result = null;
		}
		
		return result;
	}
	

	public static String getAppVersionName(Context context, String path) {
		String versionName = "";
		try {
			// ---get the package info---
			PackageManager pm = context.getPackageManager();

			PackageInfo pi = null;
			if (path != null && !path.equals(""))
				pi = pm.getPackageArchiveInfo(path, 0);
			else
				pi = pm.getPackageInfo(context.getPackageName(), 0);

			versionName = pi.versionName;
			if (versionName == null || versionName.length() == 0) {
				return "";
			}
		} catch (Exception e) {
			Log.e("VersionInfo", "Exception", e);
		}
		return versionName;
	}
	
	//获取路径所在挂载设备的路径
	public static String getPathMount(String path) {
		if (path.equals(""))
			return null;

		String df = Shell.getInstance().getDfInfo();
		if (df == null || df.equals(""))
			return null;

		int start = 1;
		String root = null;

		for (int i = 0; i < 10; i++) {
			int nextSplitIndex = path.indexOf('/', start);
			if (nextSplitIndex == -1)
				break;

			String tmp = path.substring(0, nextSplitIndex);
			if (df.contains(tmp)) {
				root = tmp;

				if (nextSplitIndex < path.length() - 1) {
					start = nextSplitIndex + 1;
				}
				else {
					break;
				}
			}

		}

		return root;
	}
	
	private static String sExternalSDCardPath =null;
	
	public static String getExternalSDCardPath(Context c) {
		if(sExternalSDCardPath!=null){
			return sExternalSDCardPath;
		}
		
		String firstPath = Environment.getExternalStorageDirectory().getPath();
		String result = "";
        
        try {
        	
        	String[] lines = Shell.getInstance().execForString("mount").split("\n");
			for (String line : lines) {
				if (line.contains("secure")||line.contains("asec")
						|| line.contains("media")
						|| line.contains("system") || line.contains("cache")
						|| line.contains("sys") || line.contains("data")
						|| line.contains("tmpfs") || line.contains("shell")
						|| line.contains("root") || line.contains("acct")
						|| line.contains("proc") || line.contains("misc")
						|| line.contains("obb")) {
					continue;
				}

				if (line.contains("fat") || line.contains("fuse") || (line
						.contains("ntfs"))) {
					String columns[] = line.split(" ");
					if (columns != null && columns.length > 1) {
						String path = columns[1];
						if (path!=null&!path.equals(firstPath)&&isPartition(path)&&path.contains("sd"))
							result = path;
							break;
					}
				}
			}
        }
        catch (Exception e) {
        	LogCenter.debug("com.nd.assistance.ServerService", "SDCard Exception: " + e.toString());
            //e.printStackTrace();
        }
        
        //LogCenter.debug("com.nd.assistance.ServerService", "ext sd mount point: " + result);
        if(!"".equals(result)){
        	sExternalSDCardPath = result;
        }else{
			StorageManager sm = (StorageManager) c
					.getSystemService(Context.STORAGE_SERVICE);
			// 获取sdcard的路径：外置和内置
			try {
				String[] paths = (String[]) sm.getClass()
						.getMethod("getVolumePaths", null).invoke(sm, null);
				int count = paths.length;
				if (count > 1) {
					for (int i = 0; i < count; i++) {
						if ((!firstPath.equals(paths[i]))&&(!"".equals(paths[i]))) {
							
							File f= new File(paths[i]);
							if(f.canRead()&&f.canWrite()){
								result = paths[i];
								sExternalSDCardPath = result;
								break;
							}
							
						}
					}
				}
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }

        //LogCenter.debug("com.nd.assistance.ServerService", "ext sd mount point: " + result);
        
        return result;
    }
	
    private static boolean isPartition(String mountPoint) {
        boolean result = false;
        if (TextUtils.isEmpty(mountPoint)) {
            return false;
        }

        String df = Shell.getInstance().execForString("df");
        String[] lines = df.split("\n");
        for (String strLine : lines) {
            if (strLine.indexOf(mountPoint) != -1) {
                result = true;
                break;
            }
        }

        return result;
    }

	
	//判断路径是否在sdcard分区
	public static boolean isSdcardPath(String path){
		String sdcardPath = Environment.getExternalStorageDirectory().getPath();
		
		if(path.startsWith("/sdcard"))
		{
			path = sdcardPath + path.substring(7);
		}
		
		if(getPathMount(path).equals(sdcardPath))
			return true;
		
		return false;
	}
	
	public static void setIntentPackage(Intent intentTemp, String packageName){
		int sdkVersion = Device.getIntSdkVersion();
		if(sdkVersion >= 4){
			try {
				Method methodSetPackage = Intent.class.getMethod("setPackage", new Class[] {String.class});
				if (methodSetPackage != null) {
					methodSetPackage.invoke(intentTemp, new Object[] {packageName});
				}
			}
			catch (Exception e) {

			}
		}
	}
}
