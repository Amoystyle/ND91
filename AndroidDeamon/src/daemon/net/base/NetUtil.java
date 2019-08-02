package daemon.net.base;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;

public class NetUtil {
	
	public final static int GENERAL_PEER = 1;
	public final static int SHORT_CONNECTION_PEER = 1 << 1;
	public final static int LONG_CONNECTION_PEER = 1 << 2;
	
	//获取手机的IMEI
	public static String getIMEI(Context mContext) {
		TelephonyManager tel = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		String imei = "";
		if(tel != null){
			imei = tel.getDeviceId();
		}
		return imei;
	}

	//获取手机Mac地址
	public static String getMac(Context mContext) {
		String mac = "";
		WifiManager wifi = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
		if(wifi != null){
			WifiInfo info = wifi.getConnectionInfo();
			if(info != null &&info.getMacAddress() != null)
				mac = info.getMacAddress().toUpperCase();
		}
		return mac;
	}	
	
	//判断当前wifi是否可用
	public static boolean isWiFiActive(Context mContext) {
		Context context = mContext.getApplicationContext();
		ConnectivityManager connectivity = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getTypeName().equals("WIFI")
							&& info[i].isConnected()) {
						return true;
					}
				}
			}
		}

		return false;
	}
}
