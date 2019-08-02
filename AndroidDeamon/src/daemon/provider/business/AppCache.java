package daemon.provider.business;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import daemon.common.Device;
import daemon.logger.LogCenter;
import android.content.Context;
import android.content.pm.IPackageStatsObserver;
import android.content.pm.PackageManager;
import android.content.pm.PackageStats;
import android.os.RemoteException;

public class AppCache {
	private String TAG = getClass().getName();
	private long internalCacheSize = 0;
	private long externalCacheSize = 0;
	private boolean isCompleted = false;
	
	private final int TIME_OUT = 10000;

	public long getInternalCacheSize() {
		return internalCacheSize;
	}

	public long getExternalCacheSize() {
		return externalCacheSize;
	}

	public boolean queryCache(Context context, String packageName) {
		internalCacheSize = 0;
		externalCacheSize = 0;	
		isCompleted = false;		

		try {
			PackageManager manager = context.getPackageManager();
			
			// 通过反射机制获得该隐藏函数
			Method getPackageSizeInfo = manager.getClass().getDeclaredMethod("getPackageSizeInfo", String.class, IPackageStatsObserver.class);
			if(getPackageSizeInfo == null)
				return false;

			// 调用该函数，并且给其分配参数 ，待调用流程完成后会回调PkgSizeObserver类的函数
			getPackageSizeInfo.invoke(manager, packageName, new IPackageStatsObserver.Stub() {
				@Override
				public void onGetStatsCompleted(PackageStats pStats, boolean succeeded) throws RemoteException {
					internalCacheSize = pStats.cacheSize;				

					if (Device.getIntSdkVersion() >= 11) {
						try {
							Field field = pStats.getClass().getField("externalCacheSize");
							externalCacheSize = field.getLong(pStats);						
						}
						catch (Exception e) {
							e.printStackTrace();
						}
					}	
					
					isCompleted = true;
				}
			});	
			
			int waitTime = 0;
			while(!isCompleted && waitTime < TIME_OUT){
				Thread.sleep(10);
				waitTime += 10;
			}
			
			return isCompleted;
		}
		catch (Exception ex) {
			LogCenter.error(TAG, "PackManager::queryCache() exception msg: " + ex.getMessage());			
		}
		
		return false;
	}
}
