package daemon.provider.pim;

import java.util.ArrayList;

import daemon.model.pim20.CallItem;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.receiver.CallReceiver;
import daemon.receiver.DaemonPhoneStateListener;
import daemon.util.PhoneUtil;

import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.media.AudioManager;
import android.net.Uri;
import android.provider.CallLog.Calls;
import android.telephony.TelephonyManager;

public class CallProvider_V2 {
	private Context mContext;
	//0成功，1 失败， 2异常
	private int mErrorCode = 0;
	public CallProvider_V2(Context context) {
		mContext = context;
	}

	public void GetCalls(ByteWriter writer) {
		Cursor cur = null;
		String[] columns = new String[] {Calls._ID, Calls.NUMBER, Calls.DATE, Calls.TYPE, Calls.DURATION};
		//MiUI新增“新建联系人”类型，此处过滤
		String where = Calls.TYPE + " in(" + Calls.INCOMING_TYPE + "," + Calls.OUTGOING_TYPE + "," + Calls.MISSED_TYPE + ")";
		try {
			cur = mContext.getContentResolver().query(Calls.CONTENT_URI, columns, where, null, null);
			if (cur != null && cur.moveToFirst()) {
				writer.Write(cur.getCount());
				while (!cur.isAfterLast()) {
					writer.Write(cur.getInt(0)); // id
					writer.WriteBlob(cur.getBlob(1)); // number
					writer.Write(cur.getLong(2)); // date
					writer.Write(cur.getInt(3));// type
					writer.Write(cur.getLong(4)); // duration
					cur.moveToNext();
				}
			}
			else {
				writer.Write(0);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null) {
				cur.close();
			}
		}
	}

	public void AddCalls(ByteReader reader, ByteWriter writer, Boolean bplugin) {
		int cnt = reader.ReadInt32();
		ArrayList<CallItem> lstCall = new ArrayList<CallItem>();

		for (int i = 0; i < cnt; i++) {
			CallItem item = new CallItem();
			item.Read(reader);
			lstCall.add(item);
		}

		if (AddCalls(lstCall)) {
			writer.Write(true);
			for (CallItem item : lstCall) {
				writer.Write(item.getId());
			}
		}
		else {
			writer.Write(false);
			if (bplugin) {
				writer.Write(mErrorCode);
			}
		}
	}

	private boolean AddCalls(ArrayList<CallItem> lstCall) {
		boolean result = false;
		Cursor cur = null;

		int cnt = lstCall.size();
		ContentValues[] lstValues = new ContentValues[cnt];

		for (int i = 0; i < cnt; i++) {
			ContentValues values = new ContentValues();
			CallItem item = lstCall.get(i);
			values.put(Calls.NUMBER, item.getNumber());
			values.put(Calls.DATE, item.getDate());
			values.put(Calls.TYPE, item.getType());
			values.put(Calls.DURATION, item.getDuration());
			lstValues[i] = values;
		}

		if (cnt == 1) {
			try {
				Uri resultUri = mContext.getContentResolver().insert(Calls.CONTENT_URI, lstValues[0]);
				if (resultUri != null) {
					int id = Integer.parseInt(resultUri.getLastPathSegment());
					lstCall.get(0).setId(id);
					result = true;
				}
				else{
					mErrorCode = 1; //错误
				}
			}
			catch (Exception e) {
				mErrorCode = 2; //异常
				e.printStackTrace();
			}
		}
		else {
			try {
				if (cnt == mContext.getContentResolver().bulkInsert(Calls.CONTENT_URI, lstValues)) {
					String[] columns = new String[] {Calls._ID};
					String order = Calls._ID + " DESC";
					cur = mContext.getContentResolver().query(Calls.CONTENT_URI, columns, null, null, order);
					if (cur != null && cur.moveToFirst()) {

						while (!cur.isAfterLast() && cnt > 0) {
							cnt--;
							lstCall.get(cnt).setId(cur.getInt(0));
							cur.moveToNext();
						}
					}

					result = true;
				}else{
					mErrorCode = 1;
				}
			}
			catch (Exception e) {
				result = false;
				mErrorCode = 2;
				e.printStackTrace();
			}
			finally {
				if (cur != null)
					cur.close();
			}
		}
		
		return result;
	}

	public void DeleteCalls(ByteReader reader, ByteWriter writer) {
		boolean result = false;

		try {
			int cnt = reader.ReadInt32();

			String where = Calls._ID + " in(";

			for (int i = 0; i < cnt; i++) {
				where += reader.ReadInt32() + ",";
			}

			where = where.substring(0, where.length() - 1) + ")";
			mContext.getContentResolver().delete(Calls.CONTENT_URI, where, null);
			result = true;
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		writer.Write(result);
	}

	public void call(ByteReader reader, ByteWriter writer) {
		String number = reader.ReadString();
		DaemonPhoneStateListener listener = DaemonPhoneStateListener.getInstance();
		int state = listener.getState();
		int result = -1;
		switch (state) {
			case TelephonyManager.CALL_STATE_IDLE:
				// if (Common.isPhoneNumberValid(number)) {
				Uri uri = Uri.parse("tel:" + number);
				Intent intent = new Intent(Intent.ACTION_CALL, uri);
				intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				mContext.startActivity(intent);
				result = 0;
				/*
				 * } else { result = 1; }
				 */
				break;
			case TelephonyManager.CALL_STATE_OFFHOOK:
				result = 2;
				break;
			case TelephonyManager.CALL_STATE_RINGING:
				result = 3;
				break;
			default:
				break;
		}
		writer.Write(result);
	}
	
	/**
	 * 接听来电
	 * 
	 * @param reader
	 * @param writer
	 */
	public void answerCall(ByteReader reader, ByteWriter writer) {
		CallReceiver.setAnswerReponse(false);
		Intent intent = new Intent(CallReceiver.ACTION_PHONE_ANSWER_CALL);
		mContext.sendBroadcast(intent);
		CallReceiver.waitAnswerResponse();
		int success = CallReceiver.isAnswerSuccess() ? 1 : 0; 
		writer.Write(success);
	}

	/**
	 * 挂断来电
	 * 
	 * @param reader
	 * @param writer
	 */
	public void hangupCall(ByteReader reader, ByteWriter writer) {
		CallReceiver.setHangupReponse(false);
		Intent intent = new Intent(CallReceiver.ACTION_PHONE_HANGUP_CALL);
		mContext.sendBroadcast(intent);
		CallReceiver.waitHangupResponse();
		int success = CallReceiver.isHangupSuccess() ? 1 : 0;
		writer.Write(success);
	}
	
	public void openSpeaker(ByteReader reader, ByteWriter writer)
	{
		AudioManager manager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
		if (!manager.isSpeakerphoneOn())
		{
			PhoneUtil.openSpeaker(mContext);
		}
		
		int success = manager.isSpeakerphoneOn() ? 1 : 0;
		writer.Write(success);
	}
	
	public void closeSpeaker(ByteReader reader, ByteWriter writer)
	{
		AudioManager manager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
		if (manager.isSpeakerphoneOn())
		{
			PhoneUtil.closeSpeaker(mContext);
		}
		
		int success = manager.isSpeakerphoneOn() ? 0 : 1;
		writer.Write(success);
	}
}
