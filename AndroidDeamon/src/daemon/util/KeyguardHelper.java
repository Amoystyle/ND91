package daemon.util;

import android.content.Context;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;

public class KeyguardHelper {

	static KeyguardHelper mKeyguardHelper = null;
	private final int TIME_OUT = 15 * 1000;
	private Object mLock = new Object();

	public static KeyguardHelper getInstance() {
		if (mKeyguardHelper == null) {
			mKeyguardHelper = new KeyguardHelper();
		}
		return mKeyguardHelper;
	}

	public void lightScreen(Context context) {
		synchronized (mLock) {
			try {
				PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);// 获取电源管理器对象
				WakeLock lock = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP | PowerManager.SCREEN_DIM_WAKE_LOCK, "bright");
				lock.acquire(TIME_OUT);
			}
			catch (Exception e) {
				Log.e("keyguard", e.toString());
			}
		}
	}
}
