package daemon.receiver;

import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT;
import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT_LONGCONNECTION;
import daemon.provider.pim.NewMessageHelper;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class SendSmsStatusReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			String action = intent.getAction();

			if (ACTION_SEND_SMS_RESULT.equals(action)) {
				int id = intent.getIntExtra("sms_id", 0);
				int connectionId = (int)intent.getIntExtra("connectionId", -1);
				
				if (id > 0) {
					boolean sendSuccess = false;

					int resultCode = getResultCode();
					if (resultCode == Activity.RESULT_OK)
						sendSuccess = true;
					if(!sendSuccess){
						if(NewMessageHelper.getInstance(context).hasSendFail(id))
							return;
					}

					Intent longConnectionIntent = new Intent(ACTION_SEND_SMS_RESULT_LONGCONNECTION);
					longConnectionIntent.putExtra("sms_id", id);
					longConnectionIntent.putExtra("success", sendSuccess);
					longConnectionIntent.putExtra("connectionId", connectionId);
					context.sendBroadcast(longConnectionIntent);
				}
			}
		} catch (Exception e) {
			// TODO: handle exception
		}

	}
	
}
