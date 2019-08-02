package daemon.receiver;

import daemon.common.Device;
import daemon.model.pim20.SmsItem;
import daemon.provider.pim.PimProvider;
import daemon.provider.pim.Sms44;
import daemon.provider.pim.SmsProvider_V2;
import android.annotation.SuppressLint;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telephony.SmsMessage;

public class NewSmsReceiver extends BroadcastReceiver {
    private static final String ACTION = "android.provider.Telephony.SMS_RECEIVED";
    @SuppressLint("NewApi")
	@Override
    public void onReceive(Context context, Intent intent)
    {
        if(intent!=null && intent.getAction()!=null &&
                  ACTION.compareToIgnoreCase(intent.getAction())==0){
        	
        	if(Device.getIntSdkVersion()>=19){
        		if(!context.getPackageName().equals(Sms44.getDefualPakName(context))){
        			return;
        		}
                Object[]pduArray= (Object[]) intent.getExtras().get("pdus");
                SmsMessage[] messages = new SmsMessage[pduArray.length];
                for (int i = 0; i<pduArray.length; i++) {
                        messages[i] = SmsMessage.createFromPdu ((byte[])pduArray [i]);
                }
                SmsProvider_V2 smsProvider_V2 =new SmsProvider_V2(context);
                for (SmsMessage cur:messages)
                {
                	SmsItem item =new SmsItem();
                	item.setBody(cur.getDisplayMessageBody());
                	item.setDate(System.currentTimeMillis());
                	item.setNumber(cur.getDisplayOriginatingAddress());
                	item.setRead(false);
                	item.setType(1);
                	smsProvider_V2.AddSms(item);
                	
                }
        	}

        }
    }
}
