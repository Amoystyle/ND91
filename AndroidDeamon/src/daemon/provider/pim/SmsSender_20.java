package daemon.provider.pim;

import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT;


import java.util.ArrayList;

import android.annotation.TargetApi;
import android.app.PendingIntent;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.telephony.SmsManager;
import daemon.model.pim20.SmsItem;
import daemon.receiver.SendSmsStatusReceiver;

@TargetApi(Build.VERSION_CODES.DONUT)
public class SmsSender_20 {
	private String TAG = getClass().getSimpleName();
	private static final Uri SMS_URI = Uri.parse("content://sms");
	
	private static int sVirtualId = -10000;

	public void SendSms(Context context, SmsItem item, boolean isNew,
			int connectionId) {

		ArrayList<String> messageArray = SmsManager.getDefault().divideMessage(
				item.getBody());


		try {
			Uri uri = ContentUris.withAppendedId(SMS_URI, item.getId());
			ArrayList<PendingIntent> sentIntents = new ArrayList<PendingIntent>(
					messageArray.size());
			for (int j = 0; j < messageArray.size(); j++) {
				Intent intent = new Intent(ACTION_SEND_SMS_RESULT, uri, context, SendSmsStatusReceiver.class);
				intent.putExtra("sms_id", item.getId());
				intent.putExtra("connectionId", connectionId);
				PendingIntent sentPI = PendingIntent.getBroadcast(context, 0,
						intent, 0);
				sentIntents.add(sentPI);
			}
			boolean sendBySendCard = SmsSendHelper.sendBySecondCard(context,item,messageArray,sentIntents);
            if(sendBySendCard){
                return;
            }
            if(SmsSendHelper.sendMmsSpecial(context,item,messageArray,sentIntents)){
            	return;
            }

           	if(item.getId()==0){
           		sVirtualId--;
            	item.setId(sVirtualId);
           	}
           	SmsItem_v4  info = new SmsItem_v4();
           	info.setConnectId(connectionId);
           	info.setTime(System.currentTimeMillis());
           	info.setItem(item);
            NewMessageHelper.getInstance(context).addSendSmsInfo(info);

			SmsManager.getDefault().sendMultipartTextMessage
			(item.getNumber(),null, messageArray, sentIntents, null);	


		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	
}
