package daemon.provider.business;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.annotation.SuppressLint;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.IPackageDataObserver;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;
import android.content.pm.Signature;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.RemoteException;
import android.os.storage.StorageManager;
import android.text.ClipboardManager;
import android.util.Log;
import daemon.common.Common;
import daemon.common.Device;
import daemon.common.FileSpaceInfo;
import daemon.common.MD5Util;
import daemon.common.MountPartitionParam;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.BspatchUtil;
import daemon.util.IconUtil;
import daemon.util.KeyguardHelper;
import daemon.util.ManifestBean;
import daemon.util.ManifestUtil;
import daemon.util.ZipUtil;

public class PackManager implements IProvider {

	private Context mContext;
	private final String CACHE_ALL_APP_INFO_PATH = "cache_all_app_info";
	private final String CACHE_USER_APP_INFO_PATH = "cache_user_app_info";
	private final String CACHE_SYSTEM_APP_INFO_PATH = "cache_system_app_info";


	// 清除程序缓存的函数名
	private final String FREE_STORAGE_AND_NOTIFY = "freeStorageAndNotify";

	private final int INSTALL_SUCCESS = 1;
	private final int INSTALL_WAIT_CONFIRM = 2;

	private char[] clockClearCache = new char[0];

	private String TAG = getClass().getName();

	private HashMap<String, List<String>> apkInfoMap = new HashMap<String, List<String>>();
	
	private Object lock =new Object();
	private long leastMemery =-1;

	public PackManager(Context context) {
		this.mContext = context;
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		int action = reader.ReadInt32();
		switch (action) {
			case 1:
				queryAllAppInfos(writer,true);
				break;
			case 2:
				queryUserAppInfos(writer);
				break;
			case 3:
				querySystemAppInfos(writer);
				break;
			case 4:
				queryAllAppCache(writer);
				break;
			case 5:
				queryUserAppCache(writer);
				break;
			case 6:
				querySystemAppCache(writer);
				break;
			case 7:
				queryAppInfo(reader, writer);
				break;
			case 8:
				queryAppIcon(reader, writer);
				break;
			case 9:
				queryAppCount(writer);
				break;
			case 10:
				queryAppPermissions(reader, writer);
				break;
			case 21:
				installApp(reader, writer);
				break;
			case 22:
				uninstallApp(reader, writer);
				break;
			case 23:
				uninstallSystemApp(reader, writer);
				break;
			case 24:
				judgeHasUpdateAddApp(reader, writer, context.getAccess().getConnectionId());
				break;
			case 25:
				updateAddApp(reader, writer, context.getAccess().getConnectionId());
				break;
			case 26:
				changeAppDataOwnAndGrp(reader, writer);
			case 30:
				queryCache(reader, writer);
				break;
			case 31:
				clearCache(reader, writer);
				break;
			case 32:
			    startApp(reader, writer);
			    break;
			case 33:
				getAppSignInfo(reader, writer);
			    break;
			case 34:
			    getAllAppSignInfo(reader, writer);
			    break;
			case 35:
				updateAddAppByNew(reader, writer);
				break;
			case 36:
			    getBootCompletedReceivers(reader, writer);
			    break;   
			case 37:
			    setBootCompletedReceiversEnable(reader, writer);
			    break;  
			case 40:
				getLeastStorate(writer);
				break;
			case 41:
				queryAllAppInfos(writer,false);
				break;
			case 42:
				queryAppsLabel(reader,writer);
				break;
			case 43:
				queryAppIconByLocalPath(reader,writer);
				break;
			default:
				break;
		}
	}
	


	private void queryAppIconByLocalPath(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		  
		PackageManager manager = mContext.getPackageManager();
		try {
			BitmapDrawable  drawable =(BitmapDrawable)IconUtil.getAppIcon(mContext, path);
			if (drawable != null) {
				Bitmap image = drawable.getBitmap();
				ByteArrayOutputStream stream = new ByteArrayOutputStream();
				image.compress(CompressFormat.PNG, 0, stream);
				writer.Write(stream.size());
				writer.Write(stream.toByteArray());
				// image.recycle();
				stream.close();
			}
		}
		catch (Exception e) {
			LogCenter.error(e.getMessage(), "");
			writer.Write(0);
		}
		
	}

	private void queryAppsLabel(ByteReader reader, ByteWriter writer) {
		int count = reader.ReadInt32();
		PackageManager manager= mContext.getPackageManager();
		ArrayList<String> list =new ArrayList<String>();
		for(int i=0;i<count;i++){
			String appName = reader.ReadString();
			try {
				PackageInfo info = manager.getPackageInfo(appName, 0);
				list.add(manager.getApplicationLabel(info.applicationInfo).toString());
			}catch(Exception e){
				
			}
		}
		writer.Write(1);
		count = list.size();
		writer.Write(count);
		for (int i = 0; i < count; i++) {
			writer.Write(list.get(i));
		}
		
	}

	private void setBootCompletedReceiversEnable(ByteReader reader,
			ByteWriter writer) {
			boolean result =false;
			try {
				if(NdkManager.isNdshActualRunning(mContext)){
					int count = reader.ReadInt32();
					for (int i = 0; i < count; i++) {
					    String packname = reader.ReadString();
					    boolean enable = reader.ReadBoolean();
					    String cmd ="LD_LIBRARY_PATH=/vendor/lib:/system/lib pm enable "+packname+"/";
					    if(!enable){
					    	cmd ="LD_LIBRARY_PATH=/vendor/lib:/system/lib pm disable "+packname+"/";
					    }
					    int num = reader.ReadInt32();
					    for (int j = 0; j < num; j++) {
					    	NdkManager.execShell(mContext, cmd+reader.ReadString());
						}
					}
				    result =true;
				}
			} catch (Exception e) {
				
			}
			writer.Write(result);
		    
     	}
		
		class BootCompletedReceiverInfo{
			private String packname;
			private ArrayList<String> receiversList = new ArrayList<String>();
			boolean enable=false;
		}
		

		private void getBootCompletedReceivers(ByteReader reader, ByteWriter writer) {
			Intent intent = new Intent(Intent.ACTION_BOOT_COMPLETED);  
	        PackageManager mPackageManager= mContext.getPackageManager();
	
			List<ResolveInfo> resolveInfoList = mPackageManager.queryBroadcastReceivers(intent, PackageManager.GET_DISABLED_COMPONENTS);  
            ArrayList<BootCompletedReceiverInfo> list =new ArrayList<PackManager.BootCompletedReceiverInfo>();
			
	        for (int i = 0; i < resolveInfoList.size(); i++) {
	        	ResolveInfo resolveInfo =resolveInfoList.get(i);
	        	
	    		ComponentName mComponentName = new ComponentName(resolveInfo.activityInfo.packageName, resolveInfo.activityInfo.name);  
	    		int state = mPackageManager.getComponentEnabledSetting(mComponentName);
	    		boolean enable =true;
	    		if(state==PackageManager.COMPONENT_ENABLED_STATE_DISABLED||state==PackageManager.COMPONENT_ENABLED_STATE_DISABLED_USER){
	    			enable =false;
	    		}
	    		boolean add =true;
	    		for (int j = 0; j < list.size(); j++) {
	    			BootCompletedReceiverInfo info = list.get(j);
	    			if(resolveInfo.activityInfo.packageName.equals(info.packname)){
	    				if(enable){
	    					info.enable = true;
	    				}
	    				info.receiversList.add(resolveInfo.activityInfo.name);
	    				add =false;
	    				break;
	    			}
				}
	    		if(add){
	    			BootCompletedReceiverInfo info =new BootCompletedReceiverInfo();
	    			info.enable = enable;
	    			info.packname = resolveInfo.activityInfo.packageName;
	    			info.receiversList.add(resolveInfo.activityInfo.name);
	    			list.add(info);
	    		} 
	   		}
	        int count = list.size();
	        writer.Write(count);
	        for (int i = 0; i < count; i++) {
	        	BootCompletedReceiverInfo info = list.get(i);
	        	writer.Write(info.packname);
	        	writer.Write(info.enable);
	        	int size = info.receiversList.size();
	        	writer.Write(size);
	        	for (int j = 0; j < size; j++) {
	        		writer.Write(info.receiversList.get(j));
				}
			}
		
	    }

	private void getLeastStorate(ByteWriter writer) {
		leastMemery = -1;

		Handler handler = new Handler(mContext.getApplicationContext()
				.getMainLooper());
		handler.post(new Runnable() {
			@Override
			public void run() {
				try {
					switch (Device.getIntSdkVersion()) {
					case 18:
						leastMemery = getLeastStorate();
						break;
					case 17:// 4.2.*
					case 16:// 4.1.*
					case 15:// 4.0.3 && 4.0.4
					case 14:// 4.0.1 && 4.0.2

						leastMemery = getLeastMem42();
						break;
					case 13:// 3.2
					case 12:// 3.1.*
					case 11:// 3.0.*
					case 10:// 2.3.4 && 2.3.3
					case 9:// 2.3.1 && 2.3.2 && 2.3
					case 8:// 2.2.*
					case 7:// 2.1.*
						leastMemery = getLeastMem23();
						break;
					default:
						leastMemery = getLeastMem42();
						break;
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
				synchronized (lock) {
					lock.notifyAll();
				}
			}
		});
		synchronized (lock) {
			try {
				lock.wait(2000);
			} catch (InterruptedException e) {

			}
		}
		writer.Write(leastMemery);
	}

	
	// 4.3
	@SuppressLint("NewApi")
	private long getLeastStorate() {
		long result = -1;
		try {
			StorageManager sm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
			
			 
			Class<StorageManager> clsSm = StorageManager.class;
			Method mthGetStorageLowBytes = clsSm.getMethod("getStorageLowBytes", new Class[] {File.class});
			if (mthGetStorageLowBytes == null) {
				return result;
			}
			result = (Long) mthGetStorageLowBytes.invoke(sm, new File("/data"));
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}
	

	//4.2-2.3以上
	private long getLeastMem42()
	{
		long nRes = -1;
		
		try{
		
		   Method getService = Class.forName("android.os.ServiceManager").getMethod("getService", String.class);		 
		   Object obj = getService.invoke(Class.forName("android.os.ServiceManager"), new Object[]{new String("devicestoragemonitor")});
	
		   Class<?> callClass=Class.forName("com.android.server.DeviceStorageMonitorService");
		 
		    Constructor tor = callClass.getConstructor(new Class[]{Context.class});
		    Method callMe = callClass.getMethod("getMemoryLowThreshold");
			Object instance = tor.newInstance(new Object[]{mContext});
			nRes = (Long)callMe.invoke(instance);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		 

		
		return nRes;
	}
	
	private long getLeastMem23()
	{
		long lRes = -1;
		try{
			 Class<?> callClass=Class.forName("com.android.server.DeviceStorageMonitorService");
			 
			 Constructor tor = callClass.getConstructor(new Class[]{Context.class});
			 //Method callMe = callClass.getMethod("getMemThreshold");
			 Method callMe = callClass.getDeclaredMethod("getMemThreshold");
			 callMe.setAccessible(true);
			 
			 
			 try {
				Object instance = tor.newInstance(new Object[]{mContext});
				lRes = (Long)callMe.invoke(instance);
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InstantiationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			 
			 
			}catch(ClassNotFoundException ex){
	        //ignored
	       }catch(NoSuchMethodException ex){
	        //ignored
	       }catch(IllegalAccessException ex){
	        //ignored
	       }catch(InvocationTargetException ex){
	        //ignored
	       }
		
		return lRes;
	}
	
	
	private void updateAddAppByNew(ByteReader reader, ByteWriter write) {
		String oldFile = reader.ReadString();
		String newFile = reader.ReadString();
		String patchFile = reader.ReadString();
		boolean reslt = BspatchUtil.patch(mContext, oldFile, newFile, patchFile);
		write.Write(reslt);
		
	}



		private void getAllAppSignInfo(ByteReader reader, ByteWriter writer) {
		boolean isSystem = reader.ReadBoolean();
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		String daemonPackageName = mContext.getPackageName();
		for (int i = infos.size() - 1; i >= 0; i--) {
			if (infos.get(i).packageName.equals(daemonPackageName)) {
				infos.remove(i);
			}
			PackageInfo info = infos.get(i);
			boolean isSystemApp = isSystemApp(info);
			if (isSystemApp != isSystem) {
				infos.remove(i);
			}
			else if (info.packageName.equals(daemonPackageName)) {
				infos.remove(i);
			}
		}

		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			String packname = infos.get(i).packageName;
			writer.Write(packname);
			writer.Write(getSignInfo(packname));
		}
	}

	private void getAppSignInfo(ByteReader reader, ByteWriter writer) {
		int count = reader.ReadInt32();
		for (int i = 0; i < count; i++) {
			String packanme = reader.ReadString();
			writer.Write(getSignInfo(packanme));
		}
	}
	
	private String getSignInfo(String packname) {
		String sign ="";
		try {
			PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(
					packname, PackageManager.GET_SIGNATURES);
			Signature[] signs = packageInfo.signatures;
			StringBuilder builder =new StringBuilder();
			builder.append(signs.length);
			for (int i = 0; i < signs.length; i++) {
				builder.append(signs[i].toCharsString());
			}
			sign = MD5Util.stringToMD5(builder.toString());	    
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sign;
	}

	private void judgeHasUpdateAddApp(ByteReader reader, ByteWriter writer, int connectionId) {
		try {
			String installedApkFile = reader.ReadString();
			String newMainfestFile = reader.ReadString();
			File installFile = new File(installedApkFile);
			File mainFile = new File(newMainfestFile);
			if (!installFile.exists() || !mainFile.exists()) {
				return;
			}
			Map<String, String> manifestMap = ManifestUtil.getManifestListFromApk(installedApkFile);
			List<ManifestBean> manifestList = ManifestUtil.getManifestListFromFile(newMainfestFile);
			List<String> deleteList = ManifestUtil.compareManifest(manifestMap, manifestList);
			if (deleteList == null) {
				writer.Write(0);
			}
			else {
				apkInfoMap.put(connectionId + installedApkFile, deleteList);
				writer.Write(1);
			}
		}
		catch (Exception e) {
			writer.Write(0);
		}

	}

	private void updateAddApp(ByteReader reader, ByteWriter writer, int connectionId) {
		try {
			String installedApkFile = reader.ReadString();
			String addApkPath = reader.ReadString();
			File installFile = new File(installedApkFile);
			File addApkFile = new File(addApkPath);
			if (!installFile.exists() || !addApkFile.exists()) {
				writer.Write(0);
				return;
			}
			List<String> delete = ManifestUtil.getDeleteManifest(addApkPath);
			List<String> deleteList = apkInfoMap.get(connectionId + installedApkFile);
			if (deleteList == null) {
				writer.Write(0);
				return;
			}
			delete.addAll(deleteList);
			ZipUtil.mergerZip(installedApkFile, addApkPath, delete);
			writer.Write(1);
			apkInfoMap.remove(connectionId + installedApkFile);
		}
		catch (Exception e) {
			writer.Write(0);
		}
	}

	private void queryAllAppCache(ByteWriter writer) {
		if (!loadCacheInfo(writer, CACHE_ALL_APP_INFO_PATH)) {
			writer.Write(false);
			queryAllAppInfos(writer,true);
			saveCache(writer, CACHE_ALL_APP_INFO_PATH);
		}
	}

	private void queryUserAppCache(ByteWriter writer) {
		if (!loadCacheInfo(writer, CACHE_USER_APP_INFO_PATH)) {
			queryUserAppInfos(writer);
			saveCache(writer, CACHE_USER_APP_INFO_PATH);
		}
	}

	private void querySystemAppCache(ByteWriter writer) {
		if (!loadCacheInfo(writer, CACHE_SYSTEM_APP_INFO_PATH)) {
			querySystemAppInfos(writer);
			saveCache(writer, CACHE_SYSTEM_APP_INFO_PATH);
		}
	}

	private boolean loadCacheInfo(ByteWriter writer, String cachePath) {
		try {
			File file = new File(cachePath);
			if (file.exists()) {
				InputStream in = mContext.openFileInput(cachePath);
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

				return true;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	private void saveCache(ByteWriter writer, String cachePath) {
		try {
			FileOutputStream cacheStream = mContext.openFileOutput(cachePath, Context.MODE_WORLD_WRITEABLE);;
			cacheStream.write(writer.getBaseStream().GetArray());
			cacheStream.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void queryAppPermissions(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		LogCenter.error("QueryAppPermissions Info", "packagename " + appName);
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(appName, PackageManager.GET_PERMISSIONS);
			if (info != null) {
				String[] packagePermissions = info.requestedPermissions;
				if (packagePermissions != null && packagePermissions.length > 0) {
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
				writer.Write(0);
			}
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.Write(0);
		}
	}

	private void queryAllAppInfos(ByteWriter writer,boolean needLabel) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);

		String daemonPackageName = mContext.getPackageName();
		for (int i = infos.size() - 1; i >= 0; i--) {
			if (infos.get(i).packageName.equals(daemonPackageName)) {
				infos.remove(i);
			}
		}

		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			PackageInfo info = infos.get(i);
			writePackageInfo(info, writer,needLabel);
		}
	}

	private void queryUserAppInfos(ByteWriter writer) {
		queryAppInfos(writer, false);
	}

	private void querySystemAppInfos(ByteWriter writer) {
		queryAppInfos(writer, true);
	}

	private void queryAppInfos(ByteWriter writer, boolean isSystem) {
		String daemonPackageName = mContext.getPackageName(); // 排除守护本身

		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);

		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			boolean isSystemApp = isSystemApp(info);
			if (isSystemApp != isSystem) {
				infos.remove(i);
			}
			else if (info.packageName.equals(daemonPackageName)) {
				infos.remove(i);
			}
		}

		writer.Write(infos.size());
		for (int i = 0; i < infos.size(); i++) {
			PackageInfo info = infos.get(i);
			writePackageInfo(info, writer,true);
		}
	}

	private boolean isSystemApp(PackageInfo appInfo) {
		int flags = appInfo.applicationInfo.flags;
		if ((flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (flags & ApplicationInfo.FLAG_SYSTEM) != 0)
			return true;

		return false;
	}

	private void queryAppInfo(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(appName, 0);
			writer.Write(1);
			writePackageInfo(info, writer,true);
		}
		catch (NameNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
			writer.Write(0);
		}
	}

	private void queryAppCount(ByteWriter writer) {
		PackageManager manager = mContext.getPackageManager();
		List<PackageInfo> infos = manager.getInstalledPackages(0);
		// all
		if (infos != null) {
			String daemonPackageName = mContext.getPackageName();
			for (int i = infos.size() - 1; i >= 0; i--) {
				if (infos.get(i).packageName.equals(daemonPackageName)) {
					infos.remove(i);
				}
			}

			writer.Write(infos.size());
		}
		else {
			writer.Write(0);
		}

		for (int i = infos.size() - 1; i >= 0; i--) {
			PackageInfo info = infos.get(i);
			if (isSystemApp(info)) {
				infos.remove(i);
			}
		}
		// customer
		if (infos != null) {
			writer.Write(infos.size());
		}
	}

	private void writePackageInfo(PackageInfo info, ByteWriter writer, boolean needLabel) {
		boolean isSystem = false;
		if ((info.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0 || (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0)
			isSystem = true;

		PackageManager manager = mContext.getPackageManager();
		writer.Write(info.packageName);
		writer.Write(info.versionCode);
		writer.Write(info.versionName);
		String path = info.applicationInfo.sourceDir;
		writer.Write(path);
		if(needLabel){
			writer.Write(manager.getApplicationLabel(info.applicationInfo).toString());
		}
		writer.Write(isSystem);
		File file = new File(path);
		long length = 0;
		if (file.exists()) {
			length = file.length();
		}
		writer.Write(length);
	}

	private void queryAppIcon(ByteReader reader, ByteWriter writer) {
		String appName = reader.ReadString();
		PackageManager manager = mContext.getPackageManager();
		try {
			BitmapDrawable drawable = (BitmapDrawable) manager.getApplicationIcon(appName);
			if (drawable != null) {
				Bitmap image = drawable.getBitmap();
				ByteArrayOutputStream stream = new ByteArrayOutputStream();
				image.compress(CompressFormat.PNG, 0, stream);
				writer.Write(stream.size());
				writer.Write(stream.toByteArray());
				// image.recycle();
				stream.close();
			}
		}
		catch (Exception e) {
			LogCenter.error(e.getMessage(), "");
			writer.Write(0);
		}
	}

	private void installApp(ByteReader reader, ByteWriter writer) {
		KeyguardHelper.getInstance().lightScreen(mContext);
		String appPath = reader.ReadString();
		boolean isLockApp = reader.ReadBoolean();
		boolean isRecover = reader.ReadBoolean();
		int isOnSdcard = reader.ReadInt32();
		int mode = reader.ReadInt32();
		boolean isSuccess = false;
		if (mode == 1 || mode == 3)
			isSuccess = installByNdsh(appPath, isLockApp, isRecover, isOnSdcard);
		if (isSuccess) {
			writer.Write(1);
			writer.Write(INSTALL_SUCCESS);
		}else if(mode == 3){
		    writer.Write(0);
		}
		else {
			Uri apkUri = Uri.fromFile(new File(appPath));
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			mContext.startActivity(intent);
			writer.Write(1);
			writer.Write(INSTALL_WAIT_CONFIRM);
		}
	}

	private boolean installByNdsh(String packagePath, boolean isLockApp, boolean isRecover, int isOnSDCard) {
		boolean result = false;
		if (NdkManager.isNdshActualRunning(mContext) && NdkManager.isRunInRoot(mContext)) {
			result = NdkManager.install(mContext, packagePath, isLockApp, isRecover, isOnSDCard);
		}
		return result;
	}

	// 有ndsh前的方法，需要授权，有ndsh后不调这个方法改走uninstallByNdsh()方法。 这个方法暂保留
	private void uninstallApp(ByteReader reader, ByteWriter writer) {
		KeyguardHelper.getInstance().lightScreen(mContext);
		String packageName = reader.ReadString();
		boolean keepData = reader.ReadBoolean();
		int mode = reader.ReadInt32();

		boolean blnInstall = false;
		if (mode == 1)
			blnInstall = uninstallByNdsh(packageName, keepData);
		if (blnInstall) {
			writer.Write(1);
			writer.Write(INSTALL_SUCCESS);
		}
		else {
			String uriString = "package:" + packageName;
			Uri packageURI = Uri.parse(uriString);

			Intent uninstallIntent = new Intent(Intent.ACTION_DELETE, packageURI);
			uninstallIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			mContext.startActivity(uninstallIntent);
			writer.Write(1);
			writer.Write(INSTALL_WAIT_CONFIRM);
		}
	}

	private boolean uninstallByNdsh(String packageName, boolean isKeepData) {
		boolean result = false;
		if (NdkManager.isNdshActualRunning(mContext) && NdkManager.isRunInRoot(mContext)) {
			result = NdkManager.uninstall(mContext, packageName, isKeepData);
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

	@Override
	public int getBusiness() {
		return 213;
	}

	private void changeAppDataOwnAndGrp(ByteReader reader, ByteWriter writer) {
		String strPackage = reader.ReadString();

		PackageManager manager = mContext.getPackageManager();
		ApplicationInfo info;
		int uid = 0;
		try {
			info = manager.getApplicationInfo(strPackage, 0);
			uid = info.uid;
		}
		catch (Exception e) {
			LogCenter.error(TAG, e.getMessage());
		}

		if (uid == 0) {
			writer.Write(-1); // 获取uid失败
			return;
		}

		strPackage = "/data/data/" + strPackage;
		if (NdkManager.isNdshActualRunning(mContext)) {
			byte[] rtnData = NdkManager.chownAndGrp(mContext, strPackage, uid, uid);
			if (rtnData != null) {
				writer.Write(rtnData);
			}
			else {
				// ndsh执行失败
				writer.Write(-2);
			}
		}
		else {
			// ndsh没启动
			writer.Write(-3);
		}
	}

	/**
	 * 获取应用程序的缓存数据，返回格式为<package, cacheSize>
	 * 
	 * @param reader
	 * @param writer
	 */
	private void queryCache(ByteReader reader, ByteWriter writer) {		
		long totalSize = 0;
		final Map<String, Long> map = new HashMap<String, Long>();		
	
		try {		
			PackageManager manager = mContext.getPackageManager();
			List<ApplicationInfo> listApps = manager.getInstalledApplications(PackageManager.GET_UNINSTALLED_PACKAGES | PackageManager.GET_ACTIVITIES);
			
			for (ApplicationInfo info : listApps) {
				AppCache cache = new AppCache();
				if(cache.queryCache(mContext, info.packageName)){
					long cacheSize = cache.getInternalCacheSize() + cache.getExternalCacheSize(); 
					if( cacheSize > 0){
						map.put(info.packageName, cacheSize);
						totalSize += cacheSize;
					}
				}			
			}			
			
			writer.Write(totalSize);
			writer.Write(map.size());
			for (Entry<String, Long> entry : map.entrySet()) {
				writer.Write(entry.getKey());
				writer.Write(entry.getValue().longValue());
			}
		}
		catch (Exception ex) {
			LogCenter.debug(TAG, "PackManager::queryCache() exception msg: " + ex.getMessage());
			ex.printStackTrace();
			writer.Write(-1L);
		}
	}

	/**
	 * 清除应用程序的缓存
	 * 
	 * @param reader
	 * @param writer
	 */
	private void clearCache(ByteReader reader, ByteWriter writer) {
		try {
			PackageManager manager = mContext.getPackageManager();
			Class<?> paramLong = Long.TYPE;
			Class<?> paramObserver = IPackageDataObserver.class;
			Method freeStorageAndNotify = manager.getClass().getMethod(FREE_STORAGE_AND_NOTIFY, paramLong, paramObserver);

			FileSpaceInfo info = Device.getDataSpaceInfo();
			long dataSize = info.getTotal();
			long sizeBeforeFree = info.getFree();
			freeStorageAndNotify.invoke(manager, dataSize, new IPackageDataObserver.Stub() {
				@Override
				public void onRemoveCompleted(String packageName, boolean succeeded) throws RemoteException {
					String strName = packageName == null ? "null" : packageName;
					LogCenter.error(TAG, "packageName: " + strName);
					synchronized (clockClearCache) {
						clockClearCache.notify();
					}
				}
			});

			synchronized (clockClearCache) {
				clockClearCache.wait(5000);
			}

			long sizeAfterFree = Device.getDataSpaceInfo().getFree();
			long sizeFree = sizeAfterFree - sizeBeforeFree;
			sizeFree += clearPkgExternalCache();
			writer.Write(sizeFree);
			LogCenter.debug(TAG, "free size: " + sizeFree);
		}
		catch (Exception e) {
			e.printStackTrace();
			writer.Write(-1L);
		}
	}

    private void startApp(ByteReader reader, ByteWriter writer){
	    try {
	        String packageName = reader.ReadString();
	        int count = reader.ReadInt32();
	        Map<String,String> values = new HashMap<String, String>();
	        for(int i = 0; i < count; i++)
	        {
	            String key = reader.ReadString();
	            String value = reader.ReadString();
	            values.put(key, value);
	        }
	        PackageManager manager = mContext.getPackageManager();
	        PackageInfo packInfo = manager.getPackageInfo(packageName, 0);  
	        Intent resolveIntent = new Intent(Intent.ACTION_MAIN, null);   
	        resolveIntent.addCategory(Intent.CATEGORY_LAUNCHER);  	      
	        Common.setIntentPackage(resolveIntent, packInfo.packageName);	
	        
	        List<ResolveInfo> apps = manager.queryIntentActivities(resolveIntent, 0);

	        ResolveInfo ri = apps.iterator().next();
	        if (ri != null ) {
                packageName = ri.activityInfo.packageName;
                String className = ri.activityInfo.name;

                Intent intent = new Intent(Intent.ACTION_MAIN);
                intent.addCategory(Intent.CATEGORY_LAUNCHER);
                
                for (Entry<String, String> entry  : values.entrySet()) {
                    String key = entry.getKey();
                    String value = entry.getValue();
                    intent.putExtra(key, value);
                }

                ComponentName cn = new ComponentName(packageName, className);
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                intent.setComponent(cn);
                mContext.startActivity(intent);
                writer.Write(1);
            }
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
            writer.Write(-1);
        }
	}
	
	private long clearPkgExternalCache() {
		long size = 0;
		String strFolder = Environment.getExternalStorageDirectory() + "/Android/data/";
		PackageManager manager = mContext.getPackageManager();
		List<ApplicationInfo> listApps = manager.getInstalledApplications(PackageManager.GET_UNINSTALLED_PACKAGES | PackageManager.GET_ACTIVITIES);
		for (ApplicationInfo info : listApps) {
			String strCachePath = strFolder + info.packageName + "/cache/";
			size += Device.deleteFile(new File(strCachePath));
		}

		return size;
	}
}
