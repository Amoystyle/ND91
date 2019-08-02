package daemon.receiver;

import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteWriter;

import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;

public class DaemonPhoneStateListener extends PhoneStateListener {
	private static DaemonPhoneStateListener mInstance = null;
	private int mState = TelephonyManager.CALL_STATE_IDLE;
	
	private String TAG = getClass().getSimpleName();
	
	private DaemonPhoneStateListener() {
		
	}
	
	public static DaemonPhoneStateListener getInstance() {
		if (mInstance == null) {
			mInstance = new DaemonPhoneStateListener();
		}
		return mInstance;
	}
	
	@Override
	public void onCallStateChanged(int state, String incomingNumber) {
		super.onCallStateChanged(state, incomingNumber);
		mState = state;
		
		LogCenter.error(TAG, "PhoneState : " + state + ", incomingNumber: " + incomingNumber);
		ConnectionManager connManager = ConnectionManager.getInstance();
		ByteWriter writer = new ByteWriter();
		writer.Write(getMessageType(state)); 
		writer.Write(incomingNumber);
		connManager.sendMessage(SendMessageType.PIM_BUSINESS_ID, writer);
	}
	
	public int getMessageType(int state) {
		int result = 20;
		
		switch (state) {
			case TelephonyManager.CALL_STATE_IDLE:
				result = SendMessageType.PIM_CALL_STATE_IDLE;
				break;
			case TelephonyManager.CALL_STATE_RINGING:
				result = SendMessageType.PIM_CALL_STATE_RINGING;
				break;
			case TelephonyManager.CALL_STATE_OFFHOOK:
				result = SendMessageType.PIM_CALL_STATE_OFFHOOK;
				break;
			default:
				break;
		}
		
		return result;
	}
	
	public int getState() {
		return mState;
	}
}