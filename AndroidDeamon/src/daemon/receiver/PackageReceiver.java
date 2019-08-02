package daemon.receiver;


import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteWriter;
import daemon.util.AppCache;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class PackageReceiver extends BroadcastReceiver {
	private Context mContext;
	private final int PACK_BUSINESS_ID = SendMessageType.PACK_BUSINESS_ID;
	private final int PACK_BUSINESS_ID_OLD = SendMessageType.PACK_OLD_BUSINESS_ID;
	
	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			mContext = context;
			String action = intent.getAction();
			Uri uri = intent.getData();
			String pkgName = uri.getSchemeSpecificPart();
			LogCenter.error("PackageReceiver Info", "packagename " + pkgName + " action "
					+ action);
			SendResult(action, pkgName);
		} catch (Exception e) {
			// TODO: handle exception
		}

	}

	protected void SendResult(String action, String packageName) {
		if (action.equals(Intent.ACTION_PACKAGE_INSTALL)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_INSTALL, packageName);
		} else if (action.equals(Intent.ACTION_PACKAGE_ADDED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_ADDED, packageName);
			//系统软件无法监听到
			new AppCache(mContext).saveUserAppCache();
		} else if (action.equals(Intent.ACTION_PACKAGE_REPLACED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_REPLACED, packageName);
			//系统软件无法监听到
			new AppCache(mContext).saveUserAppCache();
		} else if (action.equals(Intent.ACTION_PACKAGE_REMOVED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_REMOVED, packageName);
			//系统软件无法监听到
			new AppCache(mContext).saveUserAppCache();
		} else if (action.equals(Intent.ACTION_PACKAGE_CHANGED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_CHANGED, packageName);
			//系统软件无法监听到
			new AppCache(mContext).saveUserAppCache();
		} else if (action.equals(Intent.ACTION_PACKAGE_RESTARTED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_RESTARTED, packageName);
		} else if (action.equals(Intent.ACTION_PACKAGE_DATA_CLEARED)) {
			SendNotify(SendMessageType.PACK_ACTION_PACKAGE_DATA_CLEARED, packageName);
		}
	}
	
	private void SendNotify(int flag, String packageName){
		ByteWriter writer = new ByteWriter();
		writer.Write(flag);
		writer.Write(packageName);
		ConnectionManager.getInstance().sendMessage(PACK_BUSINESS_ID,
				writer);
		
		ConnectionManager.getInstance().sendMessage(PACK_BUSINESS_ID_OLD,
				writer);	
	}
}
