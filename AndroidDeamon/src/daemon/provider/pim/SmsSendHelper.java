package daemon.provider.pim;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;

import daemon.common.Device;
import daemon.model.pim20.SmsItem;

import android.app.PendingIntent;
import android.content.Context;
import android.os.Build;
import android.telephony.SmsManager;
import android.telephony.TelephonyManager;

public class SmsSendHelper {
	
	
	public static boolean sendBySecondCard(Context c, SmsItem item, ArrayList<String> messageArray, ArrayList<PendingIntent> sentIntents){
		String model = android.os.Build.MODEL.toLowerCase();
		if("sch-n719".equals(model)){
			return sendSchN719(c,item,  messageArray,  sentIntents);
		}
		return false;
	}
	
	public static int getSimCardState(Context c){		
		TelephonyManager manager = (TelephonyManager)c.getSystemService(Context.TELEPHONY_SERVICE);
		int simState = manager.getSimState();
		if(simState!=TelephonyManager.SIM_STATE_READY){
			String model = android.os.Build.MODEL.toLowerCase();
			if("sch-n719".equals(model)){
				return getSchN719SecondCardState(c);
			}
		}
		return simState;
	}


	
	private static int getSchN719SecondCardState(Context c) {
		Class cls = TelephonyManager.class;
		TelephonyManager manager = (TelephonyManager)c.getSystemService(Context.TELEPHONY_SERVICE);
		Method m1;
		try {
			m1 = cls.getDeclaredMethod("getSecondary");
			TelephonyManager manager2 = (TelephonyManager) m1.invoke(manager);
			int sim2 = manager2.getSimState();
			return sim2;
		} catch (Exception e) {
			e.printStackTrace();
		} 

		return TelephonyManager.SIM_STATE_UNKNOWN;
	}
	
	private static boolean sendSchN719(Context c, SmsItem item, ArrayList<String> messageArray, ArrayList<PendingIntent> sentIntents) {
		TelephonyManager manager = (TelephonyManager)c.getSystemService(Context.TELEPHONY_SERVICE);
		int sim1 = manager.getSimState();
		if(sim1==TelephonyManager.SIM_STATE_READY){
			return false;
		}
		Class cls = TelephonyManager.class;
		Method m1;
		try {
            int sim2 = getSchN719SecondCardState(c);
			if(sim2==TelephonyManager.SIM_STATE_READY){
				SmsManager sms =SmsManager.getDefault();
				cls = sms.getClass();
				m1 = cls.getDeclaredMethod("sendMultipartTextMessage",String.class,String.class,messageArray.getClass(),sentIntents.getClass(),sentIntents.getClass()
				,boolean.class,int.class,int.class,int.class,int.class,boolean.class);   
				m1.invoke(sms,item.getNumber(),	null, messageArray, sentIntents, null
						,false, 255, 0, 2, 2, true);
				return true;
			}
		} catch (Exception e) {
			e.printStackTrace();
		} 
		return false;
	}
	
	
	
	
	public static boolean send9100(Context c, SmsItem item,
			ArrayList<String> messageArray, ArrayList<PendingIntent> sentIntents) {
		try {
		    SmsManager smsManager = SmsManager.getDefault();
		    Class[] classArray = {String.class, String.class,ArrayList.class, ArrayList.class,ArrayList.class,
		            Boolean.TYPE,Integer.TYPE,Integer.TYPE, Integer.TYPE};
		    Method method;
		    method = smsManager.getClass().getMethod("sendMultipartTextMessage", classArray);
		    Object[] valueArray = {item.getNumber(), null, messageArray, sentIntents, null,
		    		Boolean.valueOf(false), Integer.valueOf(0),Integer.valueOf(0),Integer.valueOf(0)};
		    method.invoke(smsManager, valueArray);
		    return true;
		} catch (Exception e) {
			e.printStackTrace();
		} 
		return false;
	  }

	public static boolean sendMmsSpecial(Context context, SmsItem item,
			ArrayList<String> messageArray, ArrayList<PendingIntent> sentIntents) {
        if (("GT-I9100".equals(Build.MODEL)) && (Device.getIntSdkVersion() >= 14)){
        	 return send9100(context,item, messageArray,sentIntents);
        }
		return false;
	}

}
