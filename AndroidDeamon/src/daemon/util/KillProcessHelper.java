package daemon.util;

import android.app.ActivityManager;

public class KillProcessHelper {

	public static void killProcess(ActivityManager activityManager,String processName){
		activityManager.killBackgroundProcesses(processName);
	}
}
