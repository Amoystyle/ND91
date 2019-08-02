package daemon.util;

import java.io.FileOutputStream;
import java.util.List;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class AppCache {
	private static Object mLock = new Object();
	private Context mContext;

	private final String SYSTEMAPP_FILE_PATH = "cahcesystemapp";
	private final String CUSTAPP_FILE_PATH = "cahcecustapp";

	public AppCache(Context context) {
		mContext = context;
	}

	public void saveUserAppCache() {
		new UserCacheThread().start();
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
			synchronized (mLock) {
				if (isSystem) {
					cacheStream = mContext.openFileOutput(SYSTEMAPP_FILE_PATH, Context.MODE_WORLD_WRITEABLE);
				}
				else {
					cacheStream = mContext.openFileOutput(CUSTAPP_FILE_PATH, Context.MODE_WORLD_WRITEABLE);
				}

				cacheStream.write(cache.getBaseStream().GetArray());
				cacheStream.close();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void QueryAPPInfos(boolean getSystem, ByteReader reader, ByteWriter writer) {
		List<PackageInfo> infos = GetAppInfos(getSystem, writer);
		if (infos != null) {
			writer.Write(infos.size());
			writer.Write(infos.size());
			for (int i = 0; i < infos.size(); i++) {
				PackageInfo info = infos.get(i);
				WritePackageInfo(info, writer);
			}
		}
		else {
			writer.Write(0);
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

	private void WritePackageInfo(PackageInfo info, ByteWriter writer) {
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

	class UserCacheThread extends Thread {
		public void run() {
			saveCache(false);
		}
	}

	class SystemCacheThread extends Thread {
		public void run() {
			saveCache(true);
		}
	}

}
