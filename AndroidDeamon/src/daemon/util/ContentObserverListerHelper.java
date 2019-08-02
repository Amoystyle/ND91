package daemon.util;

public class ContentObserverListerHelper {

	private  static long sSmsLastTime =0;
	private  static long sCallLastTime =0;
	private  static long sConactLastTime =0;
	
	
	public static long getSmsLastTime() {
		return sSmsLastTime;
	}
	public static void setSmsLastTime(long sSmsLastTime) {
		ContentObserverListerHelper.sSmsLastTime = sSmsLastTime;
	}
	public static long getCallLastTime() {
		return sCallLastTime;
	}
	public static void setCallLastTime(long sCallLastTime) {
		ContentObserverListerHelper.sCallLastTime = sCallLastTime;
	}
	public static long getConactLastTime() {
		return sConactLastTime;
	}
	public static void setConactLastTime(long sConactLastTime) {
		ContentObserverListerHelper.sConactLastTime = sConactLastTime;
	}
	
	
	
}
