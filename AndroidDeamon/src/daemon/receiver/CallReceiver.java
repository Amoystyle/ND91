package daemon.receiver;

import com.android.internal.telephony.ITelephony;

import daemon.common.Device;
import daemon.util.PhoneUtil;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telephony.TelephonyManager;
import android.view.KeyEvent;

public class CallReceiver extends BroadcastReceiver {
	public final static String ACTION_PHONE_ANSWER_CALL = "ACTION_PHONE_ANSWER_CALL";
	public final static String ACTION_PHONE_HANGUP_CALL = "ACTION_PHONE_HANGUP_CALL";
	private static boolean sblnAnswer = false;
	private static boolean sblnHangup = false;
	private static boolean sblnAnswerReponse = false;
	private static boolean sblnHangupReponse = false;

	private String TAG = getClass().getName();

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			String strAction = intent.getAction();
			/*if (TelephonyManager.ACTION_PHONE_STATE_CHANGED.equals(strAction)) {
				//answerCall(context, intent);
			}*/
			if (ACTION_PHONE_ANSWER_CALL.equals(strAction)) {
				answerCall(context, intent);
			}
			else if (ACTION_PHONE_HANGUP_CALL.equals(strAction)) {
				hangupCall(context, intent);
			}
		} catch (Exception e) {
			// TODO: handle exception
		}

	}

	private void answerCall(Context context, Intent intent) {
		sblnAnswer = false;
		int sdk = Device.getIntSdkVersion();
		if (sdk <= 8) {
			answerCallByITelephony(context);
		}
		else {
			answerCallByMediaButton(context);
		}
		
		sblnAnswerReponse = true;
	}

	private void answerCallByITelephony(Context context) {
		try {
			TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			ITelephony itelephony = PhoneUtil.getITelephony(tm);
			// itelephony.silenceRinger();
			itelephony.answerRingingCall();
			//PhoneUtil.openSpeaker(context);
			sblnAnswer = true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void answerCallByMediaButton(Context context) {
		try {
			/**
			 * 插耳机广播 state —— 0代表拔出，
			 * 1代表插入 name——字符串，代表headset的类型 
			 * microphone —— 1代表插入的headset有麦克风，0表示没有麦克风
			 */
			Intent localIntent1 = new Intent(Intent.ACTION_HEADSET_PLUG);
			localIntent1.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
			localIntent1.putExtra("state", 1);
			localIntent1.putExtra("microphone", 1);
			localIntent1.putExtra("name", "Headset");
			context.sendOrderedBroadcast(localIntent1, "android.permission.CALL_PRIVILEGED");

			Intent localIntent2 = new Intent(Intent.ACTION_MEDIA_BUTTON);
			KeyEvent localKeyEvent1 = new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_HEADSETHOOK);
			localIntent2.putExtra("android.intent.extra.KEY_EVENT", localKeyEvent1);
			context.sendOrderedBroadcast(localIntent2, "android.permission.CALL_PRIVILEGED");

			Intent localIntent3 = new Intent(Intent.ACTION_MEDIA_BUTTON);
			KeyEvent localKeyEvent2 = new KeyEvent(KeyEvent.ACTION_UP, KeyEvent.KEYCODE_HEADSETHOOK);
			localIntent3.putExtra("android.intent.extra.KEY_EVENT", localKeyEvent2);
			context.sendOrderedBroadcast(localIntent3, "android.permission.CALL_PRIVILEGED");

			Intent localIntent4 = new Intent(Intent.ACTION_HEADSET_PLUG);
			localIntent4.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
			localIntent4.putExtra("state", 0);
			localIntent4.putExtra("microphone", 1);
			localIntent4.putExtra("name", "Headset");
			context.sendOrderedBroadcast(localIntent4, "android.permission.CALL_PRIVILEGED");

			//PhoneUtil.openSpeaker(context);
			sblnAnswer = true;
		}
		catch (Exception e2) {
			e2.printStackTrace();
			
			Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON);
			KeyEvent keyEvent = new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_HEADSETHOOK);
			intent.putExtra(Intent.EXTRA_KEY_EVENT, keyEvent);
			context.sendOrderedBroadcast(intent, "android.permission.CALL_PRIVILEGED");

			intent = new Intent(Intent.ACTION_MEDIA_BUTTON);
			keyEvent = new KeyEvent(KeyEvent.ACTION_UP, KeyEvent.KEYCODE_HEADSETHOOK);
			intent.putExtra(Intent.EXTRA_KEY_EVENT, keyEvent);
			context.sendOrderedBroadcast(intent, "android.permission.CALL_PRIVILEGED");

			//PhoneUtil.openSpeaker(context);
			sblnAnswer = true;
		}

		/*Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON);
		KeyEvent keyEvent = new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_HEADSETHOOK);
		intent.putExtra(Intent.EXTRA_KEY_EVENT, keyEvent);
		context.sendOrderedBroadcast(intent, "android.permission.CALL_PRIVILEGED");

		intent = new Intent(Intent.ACTION_MEDIA_BUTTON);
		keyEvent = new KeyEvent(KeyEvent.ACTION_UP, KeyEvent.KEYCODE_HEADSETHOOK);
		intent.putExtra(Intent.EXTRA_KEY_EVENT, keyEvent);
		context.sendOrderedBroadcast(intent, "android.permission.CALL_PRIVILEGED");

		PhoneUtil.openSpeaker(context);*/
	}

	private void hangupCall(Context context, Intent intent) {
		sblnHangup = false;
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		if (tm != null) {
			try {
				ITelephony it = PhoneUtil.getITelephony(tm);
				it.endCall();
				// int volume = reader.ReadInt32();
				//PhoneUtil.closeSpeaker(context, -1);
				sblnHangup = true;
			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		sblnHangupReponse = true;
	}

	public static void waitAnswerResponse() {
		int totalSleep = 5000;
		int oneSleep = 100;
		int sleepSum = 0;
		while (sleepSum < totalSleep && !sblnAnswerReponse)
		{
			sleepSum += oneSleep;
			try {
				Thread.sleep(oneSleep);
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void waitHangupResponse() {
		int totalSleep = 5000;
		int oneSleep = 100;
		int sleepSum = 0;
		while (sleepSum < totalSleep && !sblnHangupReponse)
		{
			sleepSum += oneSleep;
			try {
				Thread.sleep(oneSleep);
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public static boolean isAnswerSuccess() {
		return sblnAnswer;
	}
	
	public static boolean isHangupSuccess() {
		return sblnHangup;
	}

	public static void setAnswerReponse(boolean reponse) {
		sblnAnswerReponse = reponse;
	}
	
	public static void setHangupReponse(boolean reponse) {
		sblnHangupReponse = reponse;
	}
}
