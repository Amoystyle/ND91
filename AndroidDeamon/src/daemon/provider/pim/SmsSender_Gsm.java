package daemon.provider.pim;

import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT;

import java.util.ArrayList;

import android.app.PendingIntent;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.telephony.gsm.SmsManager;
import android.telephony.gsm.SmsMessage;


import daemon.model.pim20.SmsItem;
import daemon.model.sms.SmsInfo;
import daemon.receiver.SendSmsStatusReceiver;

public class SmsSender_Gsm {
	private static final Uri SMS_URI = Uri.parse("content://sms");
	private static final String ID = "_id";
	private static final String DATE = "date";
	private static final String ADDRESS = "address";
	private static final String THREAD_ID = "thread_id";
	private static final String BODY = "body";
	
	public void SendSms(Context context, SmsItem item, boolean isNew,
			int connectionId) {

		ArrayList<String> messageArray = SmsManager.getDefault().divideMessage(
				item.getBody());

		try {		
			Uri uri = ContentUris.withAppendedId(SMS_URI, item.getId());
			ArrayList<PendingIntent> sentIntents = new ArrayList<PendingIntent>(
					messageArray.size());
			for (int j = 0; j < messageArray.size(); j++) {
				Intent intent = new Intent(ACTION_SEND_SMS_RESULT, uri,
						context, SendSmsStatusReceiver.class);
				intent.putExtra("sms_id", item.getId());
				intent.putExtra("connectionId", connectionId);
				PendingIntent sentPI = PendingIntent.getBroadcast(context, 0,
						intent, 0);
				sentIntents.add(sentPI);
			}

			SmsManager.getDefault().sendMultipartTextMessage(item.getNumber(),
					null, messageArray, sentIntents, null);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
