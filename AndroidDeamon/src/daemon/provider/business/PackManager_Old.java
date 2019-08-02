package daemon.provider.business;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;

import daemon.common.Common;
import daemon.common.MountPartitionParam;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.shell.Su;

public class PackManager_Old implements IProvider {
	private static String[] LAUNCHERS = new String[] {"com.nd.android.pandahome2", // 熊猫桌面
			"com.nd.android.smarthome", // 安卓桌面
			"com.gau.go.launcher", // go桌面
			"com.dianxinos.dxhome", // 点心桌面
			"com.tencent.qqlauncher", // QQ Pro桌面
			"net.qihoo.launcher", // 360桌面
			"com.android.aimoxiu", // 魔秀桌面
			"org.adwfreak.launcher", // ADW
			"com.fede.launcher", // Launcher Pro
			"org.adw.launcher.miui", // MIUI
	};
	// 普通的Android手机，默认
	private static int PLATFORM_ANDROID_DEFAULT = 4;
	// oms系统
	private static int PLATFORM_OMS = 5;

	private Context mContext;

	private final String SYSTEMAPP_FILE_PATH = "cahcesystemapp";
	private final String CUSTAPP_FILE_PATH = "cahcecustapp";

	private final String CACHE_USER_APP_INFO_PATH = "cache_user_app_info";
	private final String CACHE_SYSTEM_APP_INFO_PATH = "cache_system_app_info";

	public PackManager_Old(Context context) {
		this.mContext = context;
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		byte action = reader.ReadByte();
		switch (action) {
			case 0:
				QueryAppInfo(reader, writer);
				break;
			case 1:
				GetAppIcon(reader, writer);
				break;
			case 2:
				GetAppInfosCount(reader, writer);
				break;
			case 3:
				installApp(reader, writer);
				break;
			case 4:
				uninstallApp(reader, writer);
				break;
			case 5:
				QueryAllAPPInfos_3_X(reader, writer);
				break;
			case 6:
				QueryAppPermissions(reader, writer);
				break;
			case 7:
				// 获取已安装的桌面软件
				getInstalledLaunchers_3_X(reader, writer);
				break;
			case 8:
				// 加载缓存信息
				loadCacheInfo_3_X(reader, writer);
				break;
			case 21:
				uninstallSystemApp(reader, writer);
				break;
			case 30:
				// 查询所有（系统和用户）软件信息
				queryAllAppInfos(reader, writer);
				break;
			case 31:
				// 查询用户软件信息
				queryUserAppInfos(reader, writer);
			case 32:
				// 查询系统软件信息
				querySystemAppInfos(reader, writer);
				break;
			case 33:
				// 加载缓存信息
				loadCacheInfo(reader, writer);
				break;
			case 34:
				// 查询指定软件信息
				queryAppInfo(reader, writer);
				break;
			case 35:
				// 获取已安装的桌面软件
				getInstalledLaunchers(reader, writer);
				break;
			case 99:
				QueryAPPInfos(reader, writer);
				break;
			default:
				break;
		}
	}

	private void loadCacheInfo(ByteReader reader, ByteWriter writer) {
		boolean isSystem = reader.ReadBoolean();
		String path = null;

		if (isSystem) {
			path = CACHE_SYSTEM_APP_INFO_PATH;
		}
		else {
			path = CACHE_USER_APP_INFO_PATH;
		}

		try {
			File file = new File(path);
			if (file.exists()) {
				InputStream in = mContext.openFileInput(path);
				// 存在缓存，直接返回
				writer.Write(true);
				ByteArrayOutputStream outStream = new ByteArrayOutputStream();
				byte[] buffer = new byte[1024];
				int len = 0;
				while ((len = in.read(buffer)) != -1) {
					outStream.write(buffer, 0, len);
				}
				outStream.close();
				byte[] data = outStream.toByteArray();
				writer.Write(data);
			}
			else {
				writer.Write(false);
				if (isSystem) {
					querySystemAppInfos(reader, writer);
				}
				else {
					queryUserAppInfos(reader, writer);
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		// 开启线程，做缓存
		if (isSystem) {
			(new SystemCacheThread()).start();
		}
		else {
			(new CustCacheThread()).start();
		}
	}

	/**
	 * 加载缓存信息
	 * 
	 * @return
	 */
	private void loadCacheInfo_3_X(ByteReader reader, ByteWriter writer) {
		boolean isSystem = reader.ReadBoolean();
		String path = null;

		if (isSystem) {
			path = SYSTEMAPP_FILE_PATH;
		}
		else {
			path = CUSTAPP_FILE_PATH;
		}

		try {
			InputStream in = mContext.openFileInput(path);
			if (in != null) {
				// 存在缓存，直接返回
				writer.Write(true);
				ByteArrayOutputStream outStream = new ByteArrayOutputStream();
				byte[] buffer = new byte[1024];
				int len = 0;
				while ((len = in.read(buffer)) != -1) {
					outStream.write(buffer, 0, len);
				}
				outStream.close();
				byte[] data = outStream.toByteArray();
				writer.Write(data);
			}
		}
		catch (FileNotFoundException e) {
			writer.Write(false);
			QueryAPPInfos(isSystem, reader, writer);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		// 开启线程，做缓存
		if (isSystem) {
			(new SystemCacheThread()).start();
		}
		else {
			(new CustCacheThread()).start();
		}
	}

	/**
	 * 执行保存缓存信息
	 * 
	 */
	private void saveCache(boolean isSystem) {
		ByteWriter cache = new ByteWriter();
		FileOutputStream cacheStream;
		try {
			QueryAPPInfos(isSystem, null, cache);
			if (isSystem) {
				cacheStream = mContext.openFileOutput(SYSTEMAPP_FILE_PATH, Context.MODE_WORLD_WRITEABLE);
			}
			else {
				cacheStream = mContext.openFileOutput(CUSTAPP_FILE_PATH, Context.MODE_WORLD_WRITEABLE);
			}
			cacheStream.write(cache.getBaseStream().GetArray());
			cacheStream.close();
		}
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

	class CustCacheThread extends Thread {
		public void run() {
			saveCache(false);
		}
	}

	class SystemCacheThread extends Thread {
		public void run() {
			saveCache(true);
		}
	}

	private void QueryAPPInfos(boolean getSystem, ByteReader reader, ByteWriter writer) {
		List<PackageInfo> infos = GetAppInfos(getSystem, writer);
		if (infos != null) {
			writer.Write(infos.size());
			writer.Write(infos.size());
			for (int i = 0; i < infos.size(); i++) {
				PackageInfo info = infos.get(i);
				WritePackageInfo_3_X(info, writer);
			}
		}
		else {
			writer.Write(0);
		}
	}

	private void QueryAPPInfos(ByteReader reader, ByteWriter writer) {
		boolean getSystem = reader.ReadBoolean();
		QueryAPPInfos(getSystem, reader, writer);
	}

	private void QueryAppInfo(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(appName, 0);
			writer.WriteString("OKAY");
			WritePackageInfo_3_X(info, writer);
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.WriteString("FAIL");
		}
	}

	private void QueryAppPermissions(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		LogCenter.error("QueryAppPermissions Info", "packagename " + appName);
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(appName, PackageManager.GET_PERMISSIONS);
			if (info != null) {
				String[] packagePermissions = info.requestedPermissions;
				if (packagePermissions != null && packagePermissions.length > 0) {
					writer.WriteString("OKAY");
					writer.Write(packagePermissions.length);
					for (int j = 0; j < packagePermissions.length; j++) {
						writer.Write(packagePermissions[j]);
					}
				}
				else {
					writer.Write(0);
				}
			}
			else {
				writer.WriteString("FAIL");
			}
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.WriteString("FAIL");
		}
	}

	private List<PackageInfo> GetAppInfos(boolean getSystem, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);

		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			boolean isSystem = false;
			if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
				isSystem = true;

			if (getSystem != isSystem)
				infos.remove(i);
		}

		return infos;
	}

	private void queryAllAppInfos(ByteReader reader, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		
		String daemonPackageName = mContext.getPackageName();
		for (int i = infos.size() - 1; i >= 0; i--) {
		    if(infos.get(i).packageName.equals(daemonPackageName)){
				infos.remove(i);
			}
		}
		
		writer.Write(infos.size());
		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			PackageInfo info = infos.get(i);
			writePackageInfo(info, writer);
		}
	}

	private void queryUserAppInfos(ByteReader reader, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		String daemonPackageName = mContext.getPackageName();
	
		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
			{
				infos.remove(i);
			}else if(info.packageName.equals(daemonPackageName)){
				infos.remove(i);
			}
		}

		writer.Write(infos.size());
		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			PackageInfo info = infos.get(i);
			writePackageInfo(info, writer);
		}
	}

	private void querySystemAppInfos(ByteReader reader, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		String daemonPackageName = mContext.getPackageName();
		
		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) == 0 && (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0){
				infos.remove(i);
			}else if(info.packageName.equals(daemonPackageName)){
				infos.remove(i);
			}
		}

		writer.Write(infos.size());
		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			PackageInfo info = infos.get(i);
			writePackageInfo(info, writer);
		}
	}

	private void queryAppInfo(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(appName, 0);
			writer.WriteString("OKAY");
			writePackageInfo(info, writer);
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.WriteString("FAIL");
		}
	}

	private void QueryAllAPPInfos_3_X(ByteReader reader, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		if (infos != null) {
			writer.Write(infos.size());
			writer.Write(infos.size());
			for (int i = 0; i < infos.size(); i++) {
				PackageInfo info = infos.get(i);
				WritePackageInfo_3_X(info, writer);
			}
		}
		else {
			writer.Write(0);
		}
	}

	private void GetAppInfosCount(ByteReader reader, ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		// all
		if (infos != null) {
			String daemonPackageName = mContext.getPackageName();
			for (int i = infos.size() - 1; i >= 0; i--) {
			    if(infos.get(i).packageName.equals(daemonPackageName)){
					infos.remove(i);
				}
			}
			
			writer.Write(infos.size());
		}
		else {
			writer.Write(0);
		}
		
		String daemonPackageName = mContext.getPackageName();	
		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
				infos.remove(i);
		}
		// customer
		if (infos != null) {
			writer.Write(infos.size());
		}
	}

	private void writePackageInfo(PackageInfo info, ByteWriter writer) {
		boolean isSystem = false;
		if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
			isSystem = true;

		PackageManager manager = mContext.getPackageManager();
		writer.Write(info.packageName);
		writer.Write(info.versionCode);
		writer.Write(info.versionName);
		String path = info.applicationInfo.sourceDir;
		writer.Write(path);
		writer.Write(manager.getApplicationLabel(info.applicationInfo).toString());
		writer.Write(isSystem);
		File file = new File(path);
		long length = 0;
		if (file.exists()) {
			length = file.length();
		}
		writer.Write(length);
	}

	// 3.X版本使用，待删除，统一使用WritePackageInfo(PackageInfo info, ByteWriter writer)
	private void WritePackageInfo_3_X(PackageInfo info, ByteWriter writer) {
		boolean isSystem = false;
		if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
			isSystem = true;

		PackageManager manager = mContext.getPackageManager();
		writer.Write(info.packageName);
		writer.Write(info.versionCode);
		writer.Write(info.versionName);
		writer.Write(info.applicationInfo.sourceDir);
		writer.Write(manager.getApplicationLabel(info.applicationInfo).toString());
		writer.Write(isSystem);
	}

	private void GetAppIcon(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		PackageManager manager = mContext.getPackageManager();
		try {
			BitmapDrawable drawable = (BitmapDrawable) manager.getApplicationIcon(appName);
			if (drawable != null) {
				Bitmap image = drawable.getBitmap();
				ByteArrayOutputStream stream = new ByteArrayOutputStream();
				image.compress(CompressFormat.PNG, 0, stream);
				writer.WriteString("OKAY");
				writer.Write(stream.size());
				writer.Write(stream.toByteArray());
				// image.recycle();
				stream.close();
			}
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.WriteString("FAIL");
		}
		catch (IOException e) {
			LogCenter.error(e.getMessage(), "");
			writer.WriteString("FAIL");
		}

	}

	private void installApp(ByteReader reader, ByteWriter writer) {
		String appPath = reader.ReadString();
		boolean isLockApp = reader.ReadBoolean();
		boolean isRecover = reader.ReadBoolean();
		int isOnSdcard = reader.ReadInt32();
		int mode = reader.ReadInt32();
		boolean isSuccess = false;
		if (mode == 1)
			isSuccess = installByNdsh(appPath, isLockApp, isRecover, isOnSdcard);
		if (isSuccess) {
			writer.WriteString("OKAY");
			writer.Write("success");
		}
		else {
			Uri apkUri = Uri.fromFile(new File(appPath));
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			mContext.startActivity(intent);
			writer.WriteString("OKAY");
			writer.Write("WaitForConfirm");
		}
	}

	private boolean installByNdsh(String packagePath, boolean isLockApp, boolean isRecover, int isOnSDCard) {
		boolean result = false;
		if (NdkManager.isNdshActualRunning(mContext) && NdkManager.isRunInRoot(mContext)) {
			result = NdkManager.install(mContext, packagePath, isLockApp, isRecover, isOnSDCard);
		}
		return result;
	}

	// 有ndsh前的方法，需要授权，有ndsh后不调这个方法改走installByNdsh()方法。 这个方法暂保留
	@SuppressWarnings("unused")
	private boolean installInSuMode(String packagePath, boolean isLockApp, boolean isRecover, int isOnSDCard) {
		boolean result = false;
		Su su = Su.getInstance();
		if (su.hasSuAuthorize()) {
			String dest = "/data/local/tmp/" + String.valueOf(System.currentTimeMillis()) + ".apk";
			String command = "cat \"" + packagePath + "\" > \"" + dest + "\"";

			int copyTimes = 0;

			FileInputStream srcStream = null;
			FileInputStream desStream = null;
			File sourceFile = new File(packagePath);
			if (sourceFile.exists()) {
				try {
					su.exec(command);
					srcStream = new FileInputStream(sourceFile);
					long fileSize = srcStream.getChannel().size();
					File destFile = new File(dest);
					boolean blnCopy = false;
					while (true) {
						if (destFile.exists()) {
							if (desStream == null)
								desStream = new FileInputStream(destFile);
							if (desStream.getChannel().size() == fileSize) {
								blnCopy = true;
								break;
							}
						}
						Thread.sleep(20);
						copyTimes++;

						if (copyTimes > 50) {
							blnCopy = false;
							break;
						}
					}
					if (blnCopy) {
						StringBuilder sbResult = new StringBuilder();
						sbResult.append("pm install ");
						if (isLockApp)
							sbResult.append("-l ");
						if (isRecover)
							sbResult.append("-r ");
						switch (isOnSDCard) {
							case 1:
								sbResult.append("-s ");
								break;
							case 2:
								sbResult.append("-f ");
								break;
							case 3:
								break;
						}
						sbResult.append(dest);
						command = sbResult.toString();
						String cmdResult = su.exec_with_str_result(command, 60000);
						if (cmdResult != null) {
							if (cmdResult.toLowerCase().contains("success")) {
								result = true;
								command = "rm " + dest;
								su.exec(command);
							}
						}
					}
				}
				catch (Exception e) {
					e.printStackTrace();
					result = false;
				}
				finally {
					try {
						if (srcStream != null) {
							srcStream.close();
							srcStream = null;
						}
						if (desStream != null) {
							desStream.close();
							desStream = null;
						}
					}
					catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
		return result;
	}

	// 有ndsh前的方法，需要授权，有ndsh后不调这个方法改走uninstallByNdsh()方法。 这个方法暂保留
	private void uninstallApp(ByteReader reader, ByteWriter writer) {
		String packageName = reader.ReadString();
		boolean keepData = reader.ReadBoolean();
		int mode = reader.ReadInt32();

		boolean blnInstall = false;
		if (mode == 1)
			blnInstall = uninstallByNdsh(packageName, keepData);
		if (blnInstall) {
			writer.WriteString("OKAY");
			writer.Write(true);
		}
		else {
			String uriString = "package:" + packageName;
			Uri packageURI = Uri.parse(uriString);

			Intent uninstallIntent = new Intent(Intent.ACTION_DELETE, packageURI);
			uninstallIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			mContext.startActivity(uninstallIntent);
			writer.WriteString("OKAY");
		}
	}

	private boolean uninstallByNdsh(String packageName, boolean isKeepData) {
		boolean result = false;
		if (NdkManager.isNdshActualRunning(mContext) && NdkManager.isRunInRoot(mContext)) {
			result = NdkManager.uninstall(mContext, packageName, isKeepData);
		}
		return result;
	}

	@SuppressWarnings("unused")
	private boolean uninstallInSuMode(String packageName, boolean keepData) {
		boolean result = false;
		Su su = Su.getInstance();
		if (su.hasSuAuthorize()) {
			StringBuilder installCommand = new StringBuilder("pm uninstall ");
			if (keepData)
				installCommand.append("-k ");
			installCommand.append(packageName);

			String cmdResult = su.exec_with_str_result(installCommand.toString(), 20000);

			if (cmdResult != null && cmdResult.toLowerCase().contains("success")) {
				result = true;
			}
		}
		return result;
	}

	private void uninstallSystemApp(ByteReader reader, ByteWriter writer) {
		boolean blnUninstall = false;

		String packageName = reader.ReadString();
		PackageManager pm = mContext.getPackageManager();
		PackageInfo info = null;

		try {
			info = pm.getPackageInfo(packageName, 0);
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		if (info != null) {
			String apkPath = info.applicationInfo.sourceDir;
			if (!apkPath.startsWith(MountPartitionParam.SYSTEM)) {
				NdkManager.uninstall(mContext, packageName, false);

				try {
					info = pm.getPackageInfo(packageName, 0);
				}
				catch (Exception e) {
					e.printStackTrace();
					info = null;
				}
			}

			if (info != null) {
				apkPath = info.applicationInfo.sourceDir;
				// 有些sourceDir指向的路径是符号链接，要转为实际路径
				apkPath = Common.getCanonicalPath(apkPath);
				if (apkPath != null) {
					MountPartitionParam param = MountPartitionParam.createPartitionParam(apkPath);
					blnUninstall = NdkManager.uninstallSystemApp(mContext, packageName, apkPath, param.getFilesystemType(), param.getSource(), param.getTarget());
				}
			}
			else {// 第二次获取软件信息，如果不存在，则认为已被成功卸载，返回true。
				blnUninstall = true;
			}
		}
		else {// 第一次获取软件信息，如果不存在，则返回false
			blnUninstall = false;
		}

		writer.Write(blnUninstall);
	}

	private void getInstalledLaunchers(ByteReader reader, ByteWriter writer) {
		PackageManager packageManager = mContext.getPackageManager();
		List<PackageInfo> listPackageInfos = packageManager.getInstalledPackages(0);
		if (listPackageInfos != null) {
			for (int i = listPackageInfos.size() - 1; i >= 0; i--) {
				PackageInfo info = listPackageInfos.get(i);
				if (!isLauncher(info.packageName)) {
					listPackageInfos.remove(i);
				}
			}
			// 终端平台Platform
			// int platfrom = getPlatform();
			// writer.Write(platfrom);
			String strCurrentUseLaunchers = getCurrentUseLaunchers(listPackageInfos);
			// 正在使用的桌面
			writer.Write(strCurrentUseLaunchers);
			// 已安装的桌面个数
			writer.Write(listPackageInfos.size());
			for (PackageInfo info : listPackageInfos) {
				writePackageInfo(info, writer);
			}

		}
		else {

		}
	}

	private void getInstalledLaunchers_3_X(ByteReader reader, ByteWriter writer) {
		PackageManager packageManager = mContext.getPackageManager();
		List<PackageInfo> listPackageInfos = packageManager.getInstalledPackages(0);
		if (listPackageInfos != null) {
			for (int i = listPackageInfos.size() - 1; i >= 0; i--) {
				PackageInfo info = listPackageInfos.get(i);
				if (!isLauncher(info.packageName)) {
					listPackageInfos.remove(i);
				}
			}
			// 终端平台Platform
			// int platfrom = getPlatform();
			// writer.Write(platfrom);
			String strCurrentUseLaunchers = getCurrentUseLaunchers(listPackageInfos);
			// 正在使用的桌面
			writer.Write(strCurrentUseLaunchers);
			// 已安装的桌面个数
			writer.Write(listPackageInfos.size());
			for (PackageInfo info : listPackageInfos) {
				WritePackageInfo_3_X(info, writer);
			}

		}
		else {

		}
	}

	@SuppressWarnings("unused")
	private int getPlatform() {
		int ret = PLATFORM_ANDROID_DEFAULT;
		if (isOMS()) {
			ret = PLATFORM_OMS;
		}
		return ret;
	}

	@SuppressWarnings("unused")
	private boolean isOMS() {
		boolean result = false;

		try {
			Class<?> classType = Class.forName("oms.content.Action");
			result = true;
		}
		catch (Exception e) {
			result = false;
		}

		LogCenter.error("isoms", result + "");
		return result;
	}

	/**
	 * 获取当前正在使用的桌面类软件的包名， 多个包名以‘;’分隔， 如果参数listPackageInfos == null或listPackageInfos.size() == 0，则返回""
	 * 
	 * @param listPackageInfos
	 *            手机上已安装的桌面类软件的包名列表
	 * @return 返回当前正在使用的桌面类软件的包名，多个包名以‘;’分隔
	 */
	private String getCurrentUseLaunchers(List<PackageInfo> listPackageInfos) {
		if (listPackageInfos == null || listPackageInfos.size() == 0) {
			return "";
		}
		StringBuffer sb = new StringBuffer();
		ActivityManager activityManager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> listRunningAppProcessInfos = activityManager.getRunningAppProcesses();

		if (listPackageInfos == null || listPackageInfos.size() == 0) {
			return "";
		}
		for (PackageInfo infoLauncher : listPackageInfos) {
			for (RunningAppProcessInfo runningInfo : listRunningAppProcessInfos) {
				String[] stringsPkgList = runningInfo.pkgList;
				for (String strPkg : stringsPkgList) {
					if (infoLauncher.packageName.equals(strPkg)) {
						if (sb.length() != 0) {
							sb.append(";");
						}
						sb.append(infoLauncher.packageName);
						break;
					}
				}
			}
		}
		return sb.toString();
	}

	/**
	 * 判断安装程序是否是桌面类软件， 桌面类软件只检查静态变量LAUNCHERS中包含的程序，其他暂时不管
	 * 
	 * @param strPackageName
	 *            已安装的应用程序包名
	 * @return 该安装程序是桌面类软件则返回true,否则返回false
	 */
	private boolean isLauncher(String strPackageName) {
		boolean blnReturn = false;
		for (String strLauncher : LAUNCHERS) {
			if (strLauncher.equalsIgnoreCase(strPackageName)) {
				blnReturn = true;
				break;
			}
		}
		return blnReturn;
	}

	@Override
	public int getBusiness() {
		return 12;
	}

}
