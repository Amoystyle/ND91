package daemon.provider;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;


import daemon.logger.LogCenter;
import daemon.receiver.CallReceiver;
import daemon.receiver.PackageReceiver;
import daemon.receiver.SdcardStatuChangeReceiver;
import daemon.receiver.SmartHomeReceiver;
import daemon.receiver.ThemeManageReceiver;

public class ProviderBroadcastReceiver {

	private Context ctx;
	private static final String BROADCAST_RECEIVER_PANDAHOMENAME = "com.nd.android.pandahome.manage_theme2_result";
	private String TAG = ProviderBroadcastReceiver.class.getName();
	// theme
	private ThemeManageReceiver theme_controller;
	private IntentFilter filter_theme;
	private PackageReceiver package_Control;
	private IntentFilter filter_package;
	private SmartHomeReceiver smartHomeReceiver;
	private IntentFilter filterSmartHome;
	private SdcardStatuChangeReceiver sdcardStatuChangeReceiver;
	private IntentFilter sdcardIntentFilter;
	private CallReceiver callReceiver;
	private IntentFilter filterCallReceiver;
	
	public ProviderBroadcastReceiver(Context context) {
		this.ctx = context;

		theme_controller = new ThemeManageReceiver();
		filter_theme = new IntentFilter(BROADCAST_RECEIVER_PANDAHOMENAME);
		
		smartHomeReceiver = new SmartHomeReceiver();
		filterSmartHome = new IntentFilter("com.nd.android.smarthome.manage_theme_result");
		filterSmartHome.addAction("com.nd.android.moborobo.home.manage_theme_result");
		
		package_Control =new PackageReceiver();		
		filter_package = new IntentFilter(
				Intent.ACTION_PACKAGE_INSTALL);
		filter_package.addAction(Intent.ACTION_PACKAGE_REMOVED);
		filter_package.addAction(Intent.ACTION_PACKAGE_ADDED);
		filter_package.addAction(Intent.ACTION_PACKAGE_CHANGED);
		filter_package.addAction(Intent.ACTION_PACKAGE_RESTARTED);
		filter_package.addDataScheme("package");
		
		sdcardStatuChangeReceiver = new SdcardStatuChangeReceiver();
		sdcardIntentFilter = new IntentFilter(Intent.ACTION_MEDIA_MOUNTED);// sd卡被插入，且已经挂载
		sdcardIntentFilter.setPriority(1000);// 设置最高优先级
		sdcardIntentFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);// sd卡存在，但还没有挂载
		sdcardIntentFilter.addAction(Intent.ACTION_MEDIA_REMOVED);// sd卡被移除
		sdcardIntentFilter.addAction(Intent.ACTION_MEDIA_SHARED);// sd卡作为USB大容量存储被共享，挂载被解除
		sdcardIntentFilter.addAction(Intent.ACTION_MEDIA_BAD_REMOVAL);// sd卡已经从sd卡插槽拔出，但是挂载点还没解除
		sdcardIntentFilter.addDataScheme("file");
		
		callReceiver = new CallReceiver();
		filterCallReceiver = new IntentFilter();
		filterCallReceiver.addAction(CallReceiver.ACTION_PHONE_ANSWER_CALL);
		filterCallReceiver.addAction(CallReceiver.ACTION_PHONE_HANGUP_CALL);
	}

	public void register() {
		LogCenter.error(TAG, "register assistance BroadcastReceiver !");
		ctx.registerReceiver(theme_controller, filter_theme);
		ctx.registerReceiver(package_Control, filter_package);
		ctx.registerReceiver(smartHomeReceiver, filterSmartHome);
		ctx.registerReceiver(sdcardStatuChangeReceiver, sdcardIntentFilter);
		ctx.registerReceiver(callReceiver, filterCallReceiver);
	}

	public void unregister() {
		ctx.unregisterReceiver(theme_controller);
		ctx.unregisterReceiver(package_Control);
		ctx.unregisterReceiver(smartHomeReceiver);
		ctx.unregisterReceiver(sdcardStatuChangeReceiver);
	}

	/*
	 * @Override public void onReceive(Context context, Intent intent) { String
	 * action = intent.getAction(); LogCenter.error(TAG, "action is : " +action);
	 * ProviderManager.getInstance(this.ctx).DealBroadcast(intent); }
	 */
}
