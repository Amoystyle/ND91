package daemon.provider.pim;

import java.io.UnsupportedEncodingException;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.telephony.TelephonyManager;


import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class SimContactsProvider {
	private static Uri CONTENT_URI = Uri.parse("content://icc/adn");
	private static String _ID = "_id";
	private static String NAME = "name";
	private static String TAG = "tag";
	private static String NUMBER = "number";
	//private static String EMAILS = "emails";
	private static String NEW_TAG = "newTag";
	private static String NEW_NUMBER = "newNumber";
	private static String SUCCESS = "success";
	private static String FAIL = "fail";

	private static String UNKNOWN = "unknown";
	private static String SIM = "sim";
	private static String UIM = "uim";
	private static String USIM = "usim";

	private Context mContext;
	private ContentResolver mResolver;
	private String simType = UNKNOWN;

	public SimContactsProvider(Context context) {
		this.mContext = context;
		mResolver = mContext.getContentResolver();
		simType = getSimType();
	}

	public void insertContacts(ByteReader reader, ByteWriter writer) {
		String strName = reader.ReadString();
		String strNumber = reader.ReadString();
		ContentValues values = new ContentValues();
		values.put(SimContactsProvider.TAG, getUtfString(strName));
		values.put(SimContactsProvider.NUMBER, strNumber);
		Uri uriNew = null;
		try {
			uriNew = mResolver.insert(SimContactsProvider.CONTENT_URI, values);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		if (uriNew != null) {
			// 增加Sim卡联系人成功
			writer.Write(SimContactsProvider.SUCCESS);
		}
		else {
			// 增加Sim卡联系人失败
			writer.Write(SimContactsProvider.FAIL);
		}
	}

	public void deleteContacts(ByteReader reader, ByteWriter writer) {
		String strName = reader.ReadString();
		String strNumber = reader.ReadString();
		String strWhere = String.format("tag = '%1$s' AND number = '%2$s'", strName, strNumber);
		int count = 0;
		try {
			count = mResolver.delete(SimContactsProvider.CONTENT_URI, strWhere, null);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		if (count > 0) {
			// 删除Sim卡联系人成功
			writer.Write(SimContactsProvider.SUCCESS);
		}
		else {
			// 删除Sim卡联系人成功
			writer.Write(SimContactsProvider.FAIL);
		}
	}

	public void updateContact(ByteReader reader, ByteWriter writer) {
		ContentValues values = new ContentValues();
		String strNameOld = reader.ReadString();
		String strNumberOld = reader.ReadString();
		String strNameNew = reader.ReadString();
		String strNumberNew = reader.ReadString();
		values.put(SimContactsProvider.TAG, strNameOld);
		values.put(SimContactsProvider.NUMBER, strNumberOld);
		values.put(SimContactsProvider.NEW_TAG, strNameNew);
		values.put(SimContactsProvider.NEW_NUMBER, strNumberNew);
		int count = 0;
		try {
			count = mResolver.update(SimContactsProvider.CONTENT_URI, values, null, null);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		if (count > 0) {
			// 编辑Sim卡联系人成功
			writer.Write(SimContactsProvider.SUCCESS);
		}
		else {
			// 编辑Sim卡联系人失败
			writer.Write(SimContactsProvider.FAIL);
		}
	}

	public void queryContacts(ByteWriter writer) {
		Cursor cursor = null;
		try {
			cursor = mContext.getContentResolver().query(CONTENT_URI, null, null, null, null);
			if (cursor != null && cursor.moveToFirst()) {				
				int cnt = cursor.getCount();
				writer.Write(cnt);
				while (!cursor.isAfterLast()) {
					int index = cursor.getColumnIndex(SimContactsProvider._ID);
					String strID = "";
					if (index != -1)
						strID = cursor.getString(index);
					index = cursor.getColumnIndex(SimContactsProvider.NAME);
					if (index == -1) {
						index = cursor.getColumnIndex(SimContactsProvider.TAG);
					}
					String strName = cursor.getString(index);
					index = cursor.getColumnIndex(SimContactsProvider.NUMBER);
					String strNumber = "";
					if (index != -1)
						strNumber = cursor.getString(index);
					// String strEmails = cursor.getString(cursor.getColumnIndex(SimContactsProvider.EMAILS));
					writer.Write(strID);
					writer.Write(strName);
					writer.Write(strNumber);
					// email暂时不用，PC端也不显示，所以写空
					writer.Write("");
					cursor.moveToNext();
				}
			}
			else {
				writer.Write(0);
			}
		}
		catch (Exception e) {
			writer.Write(0);
			e.printStackTrace();
		}
		finally {
			if (cursor != null) {
				cursor.close();
			}
		}
	}

	/**
	 * Sim卡存储的字符编码为ucs2 "80"开头
	 * 
	 * @param strSrc
	 * @return
	 */
	private String getUtfString(String strSrc) {
		String strReturn = null;
		try {
			strReturn = new String(strSrc.getBytes("utf-16be"), "utf-16be");
		}
		catch (UnsupportedEncodingException e) {
			strReturn = strSrc;
		}
		return strReturn;
	}

	private String getSimType() {
		String strReturn = UNKNOWN;
		TelephonyManager tm = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		int type = tm.getNetworkType();
		switch (type) {
			case TelephonyManager.NETWORK_TYPE_GPRS:
			case TelephonyManager.NETWORK_TYPE_EDGE:
				strReturn = SIM;
				break;
			case TelephonyManager.NETWORK_TYPE_CDMA:
				strReturn = UIM;
				break;
			case TelephonyManager.NETWORK_TYPE_UMTS:
				strReturn = USIM;
				break;

			default:
				strReturn = UNKNOWN;
				break;
		}
		return strReturn;
	}

}
