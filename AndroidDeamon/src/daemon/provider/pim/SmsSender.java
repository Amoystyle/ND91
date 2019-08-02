package daemon.provider.pim;

import android.content.Context;
import daemon.common.Device;
import daemon.model.pim20.SmsItem;

public class SmsSender {
    public static void SendSms(Context context, SmsItem item, boolean isNew, int connectionId) {
	    if(Device.getIntSdkVersion() >= 5){
	    	SmsSender_20 sender20 = new  SmsSender_20();
	    	sender20.SendSms(context, item, isNew, connectionId);
	    }else{
	    	SmsSender_Gsm senderGsm = new  SmsSender_Gsm();
	    	senderGsm.SendSms(context, item, isNew, connectionId);
	    }
	}   
}
