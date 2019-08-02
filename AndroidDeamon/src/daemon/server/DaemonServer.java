package daemon.server;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;

import daemon.logger.LogCenter;

public class DaemonServer extends Thread {
	private final static String WIFI_LOCK = "91_assistance";
	private String TAG = getClass().getName();
	private Context mContext;
	private String mIsIpv6Enabled = null;
	private DaemonServerImp mDeamonImp;
	private WakeLock mWakeLock;
	private WifiLock mWifiLock = null;

	public DaemonServer(Context context, int port) {
		mContext = context;
		mDeamonImp = new DaemonServerImp(mContext, port);
	}

	// 设置手机是否启用ipv6
	private void SetIpv6(String value) {
		String ipv6Enabled = System.getProperty("java.net.preferIPv6Addresses");
		if (ipv6Enabled != null) {
			if (!ipv6Enabled.equals(value)) {
				System.setProperty("java.net.preferIPv6Addresses", value);
			}
		}
	}

	// 初始化守护程序
	public boolean InitServer() {
		// SetIpv6 false
		try {
			mIsIpv6Enabled = System.getProperty("java.net.preferIPv6Addresses");
			SetIpv6("false");
		}
		catch (Exception e) {
			LogCenter.debug(TAG, "SetIpv6 false.");
			e.printStackTrace();
		}

		return true;
	}

	// 初始化守护线程
	public void run() {
		mDeamonImp.start();
	}

	// 启动守护服务
	public void StartServer() {
		acquirePowerLock();
		acquireWifiLock();

		start();
	}

	// 停止守护服务
	public void StopServer() {
		LogCenter.debug(TAG, "*Enter Stop DaemonServer.");

		// setipv6 mIsIpv6Enabled
		try {
			LogCenter.debug(TAG, "--SetIpv6:" + mIsIpv6Enabled);
			SetIpv6(mIsIpv6Enabled);
		}
		catch (Exception e) {
			LogCenter.debug(TAG, "--SetIpv6 Exception.");
			e.printStackTrace();
		}		

		// stop DeamonImp
		try {
			LogCenter.debug(TAG, "--stop DeamonImp.");
			mDeamonImp.stop();
		}
		catch (Exception e) {
			LogCenter.debug(TAG, "--stop DeamonImp Exception.");
			e.printStackTrace();
		}

		releasePowerLock();
		releasekWifiLock();
		
		LogCenter.debug(TAG, "*End Stop DaemonServer.");
	}

	// 申请设备电源锁
	private void acquirePowerLock() {
		try {
			if (null == mWakeLock) {
				PowerManager pm = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
				mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK | PowerManager.ON_AFTER_RELEASE, TAG);
				if (null != mWakeLock) {
					mWakeLock.acquire();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg);
		}
	}

	// 释放设备电源锁
	private void releasePowerLock() {
		try {
			if (null != mWakeLock) {
				if (mWakeLock.isHeld())
					mWakeLock.release();

				mWakeLock = null;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg);
		}
	}

	// 申请wifi锁
	private void acquireWifiLock() {
		Log.e(TAG, "acquireWifiLock()");
		try {
			if (mWifiLock == null) {
				WifiManager wm = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
				if (wm != null)
					mWifiLock = wm.createWifiLock(WIFI_LOCK);
			}

			if (mWifiLock != null) {
				if (!mWifiLock.isHeld())
					mWifiLock.acquire();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg);
		}
	}

	// 释放wifi锁
	private void releasekWifiLock() {
		Log.e(TAG, "releasekWifiLock()");
		try {
			if (mWifiLock != null) {
				if (mWifiLock.isHeld()) {
					mWifiLock.release();
				}

				mWifiLock = null;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg);
		}
	}
}
