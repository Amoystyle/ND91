package com.nd.assistance.receiver;


import java.text.SimpleDateFormat;

import com.nd.assistance.R;
import com.nd.assistance.ui.AssistanceActivity;

import daemon.common.Device;
import daemon.util.ConfigManager;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Parcelable;
import android.util.Log;


public class WifiStateChangeReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(final Context context, Intent i) {
		try{
			boolean showNotify = ConfigManager.getIsWifiShowNotify(context);
			if(!showNotify){
				return;
			}
			if(!isWiFiActive(context)){
				return;
			}
			String action = i.getAction();
			if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(action)) {  
				Parcelable parcelableExtra = i.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);  
	            if (null != parcelableExtra) {  
	                NetworkInfo networkInfo = (NetworkInfo) parcelableExtra;  
	                boolean isConnected = networkInfo.isAvailable();  
	                if(isConnected){
	                	//一个wifi一天仅提示一次
	                	String wifiname = ConfigManager.getWifiName(context);
	                	long timeMillis = System.currentTimeMillis();
	                	if("".equals(wifiname))
	                	{
	                		ConfigManager.setWifiName(context,timeMillis+";"+Device.getWifiSSID(context));
	                	}else{
	                		String info[] = wifiname.split(";");
	                		//先对比是否同一天
	                		SimpleDateFormat sf = new SimpleDateFormat("yyyy-MM-dd"); 
	                		Log.e("记录的时间", ""+info[0]);
	                		Log.e("当前的时间", ""+timeMillis);
	                        String sp_time = sf.format(Long.parseLong(info[0]));  
	                        String current_time = sf.format(timeMillis);  
	                		if(sp_time.equals(current_time)){
	                		    //同一天，则比较wifi是否有记录过	
	                			String ssid = Device.getWifiSSID(context);
	                			int num = info.length;
	                			for (int j = 1; j < num; j++) {
									if (info[j]!=null&&!info[j].equals("")&&info[j].equals(ssid)) {
										//有记录过
										return;
									}
								}
	                			wifiname+=";"+ssid;
	                			ConfigManager.setWifiName(context,wifiname);
	                		}else{
	                			//不是同一天，则清空，重新记录
	                			ConfigManager.setWifiName(context,timeMillis+";"+Device.getWifiSSID(context));
	                		}
	                	}
	    				NotificationManager nm = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
	    				nm.cancelAll();
	    				Intent intent = new Intent(context, AssistanceActivity.class);
	    				intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
	    				PendingIntent pi = PendingIntent.getActivity(context, 1, intent, 0);
	    				Notification notification = new Notification(R.drawable.icon_notification_wifi, context.getString(R.string.wifi_connect_prompt), 0);
	    				String title = context.getString(R.string.wifi_connected);
	    				String text = context.getString(R.string.wifi_open);
	    				notification.setLatestEventInfo(context, title, text, pi);
	    				nm.notify(1, notification);
	                }
	            }
	        }
		}catch (Exception e) {

		}

	}
	
	
	private boolean isWiFiActive(Context inContext) {
		Context context = inContext.getApplicationContext();
		ConnectivityManager connectivity = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getTypeName().equals("WIFI") && info[i].isConnected()) {
						return true;
					}
				}
			}
		}

		return false;
	}
	
}
