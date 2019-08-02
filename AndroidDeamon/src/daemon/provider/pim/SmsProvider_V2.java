package daemon.provider.pim;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import daemon.model.pim20.SmsItem;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import android.annotation.SuppressLint;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

public class SmsProvider_V2 {

	private Context mContext;

	//0成功 1 插入数据库失败 2出现异常
	private int mErrorCode = 0;
	public SmsProvider_V2(Context context) {
		mContext = context;
	}

	private static final String SMS_URI = "content://sms";
	public static final String ADDRESS = "address";
	public static final String THREAD_ID = "thread_id";
	public static final String PERSON = "person";
	public static final String DATE = "date";
	public static final String READ = "read";
	public static final String STATUS = "status";
	public static final String TYPE = "type";
	public static final String BODY = "body";
	public static final String ID = "_id";

	public static void SetSmsType(Context context, int id, int type) {
		try {
			Uri uri = ContentUris.withAppendedId(Uri.parse(SMS_URI), id);
			ContentValues values = new ContentValues();
			values.put(TYPE, type);
			context.getContentResolver().update(uri, values, null, null);
		}
		catch (Exception e) {

		}
	}

	private Object classThread = null;
	private Method methodGetThreadId = null;

	private long getOrCreateThreadId(String[] numbers) {
		HashSet<String> recipients = new HashSet<String>();
		recipients.addAll(Arrays.asList(numbers));

		try {
			if (methodGetThreadId == null) {

				Class<?> classType = Class.forName("android.provider.Telephony$Threads");
				Constructor con = classType.getDeclaredConstructor(null);
				con.setAccessible(true);
				classThread = con.newInstance();

				methodGetThreadId = classType.getMethod("getOrCreateThreadId", new Class[] {Context.class, Set.class});
			}

			Object objID = methodGetThreadId.invoke(classThread, new Object[] {mContext, recipients});
			long threadId = Long.parseLong(objID.toString());
			return threadId;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return -1;
	}

	public void LoadSms(ByteWriter writer, boolean isAsc) {
		Cursor cur = null;
		String[] columns = new String[] {ID, THREAD_ID, ADDRESS, DATE, TYPE, BODY, READ};
		String where = TYPE + "<>3";
		String order = isAsc ? ID + " ASC" : null;

		try {
			Uri uri = Uri.parse(SMS_URI);
			cur = mContext.getContentResolver().query(uri, columns, where, null, order);
			if (cur != null) {
				writer.Write(cur.getCount());

				if (cur.moveToFirst()) {
					while (!cur.isAfterLast()) {

						writer.Write(cur.getInt(0)); // id
						writer.Write(cur.getInt(1)); // thread_id
						writer.WriteBlob(cur.getBlob(2)); // number
						writer.Write(cur.getLong(3));
						writer.Write(cur.getInt(4)); // type
						writer.WriteBlob(cur.getBlob(5)); // body
						writer.Write(cur.getInt(6) == 1); // read

						cur.moveToNext();
					}
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null)
				cur.close();
		}
	}

	public void GetUnreadSms(ByteReader reader, ByteWriter writer) {
		Cursor cur = null;
		String[] columns = new String[] {ID, THREAD_ID, ADDRESS, DATE, TYPE, BODY};
		String where = TYPE + " <>3 AND " + READ + " = 0";
		try {
			Uri uri = Uri.parse(SMS_URI);
			cur = mContext.getContentResolver().query(uri, columns, where, null, null);
			if (cur != null) {
				writer.Write(cur.getCount());

				if (cur.moveToFirst()) {
					while (!cur.isAfterLast()) {

						writer.Write(cur.getInt(0)); // id
						writer.Write(cur.getInt(1)); // thread_id
						writer.WriteBlob(cur.getBlob(2)); // number
						writer.Write(cur.getLong(3));
						writer.Write(cur.getInt(4)); // type
						writer.WriteBlob(cur.getBlob(5)); // body
						writer.Write(false); // read

						cur.moveToNext();
					}
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null)
				cur.close();
		}
	}

	public SmsItem GetSmsById(int smsId) {
		SmsItem item = null;
		Cursor cur = null;

		try {
			Uri uri = ContentUris.withAppendedId(Uri.parse(SMS_URI), smsId);
			String[] projection = {ADDRESS, BODY, DATE};
			cur = mContext.getContentResolver().query(uri, projection, null, null, null);
			if (cur != null && cur.moveToFirst()) {
				item = new SmsItem();
				item.setId(smsId);
				item.setNumber(cur.getString(0));
				item.setBody(cur.getString(1));
				item.setDate(cur.getLong(2));
			}
		}
		catch (Exception e) {
			item = null;
		}
		finally {
			if (cur != null) {
				cur.close();
			}
		}

		return item;
	}

	public void AddSms(ByteReader reader, ByteWriter writer, Boolean bplugin) {
		int cnt = reader.ReadInt32();
		ArrayList<SmsItem> lstSms = new ArrayList<SmsItem>();

		for (int i = 0; i < cnt; i++) {
			SmsItem item = new SmsItem();
			item.Read(reader);
			lstSms.add(item);
		}

		if (AddSms(lstSms)) {
			writer.Write(true);
			writer.Write(cnt);
			for (SmsItem item : lstSms) {
				writer.Write(item.getId());
			}
		}
		else {
			writer.Write(false);
			if (bplugin) { //如果需要加入错误信息
				writer.Write(mErrorCode);
			}
		}
	}

	private boolean AddSms(ArrayList<SmsItem> lstSms) {
		boolean result = false;

		int count = lstSms.size();
		ContentValues[] lstValues = new ContentValues[count];
		for (int i = 0; i < count; i++) {
			SmsItem item = lstSms.get(i);
			ContentValues values = new ContentValues();
			long threadId = getOrCreateThreadId(new String[] {item.getNumber()});

			values.put(THREAD_ID, threadId);
			values.put(ADDRESS, item.getNumber());
			values.put(DATE, item.getDate());
			values.put(TYPE, item.getType());
			values.put(BODY, item.getBody());
			values.put(READ, item.getRead() ? 1 : 0);
			if(!item.getRead()){
				NewMessageHelper.getInstance(mContext).addNotReadSmsInfo(item);
			}
			lstValues[i] = values;
		}

		Cursor cur = null;

		if (count == 1) {
			try {
				Uri uri = Uri.parse(SMS_URI);
				Uri resultUri = mContext.getContentResolver().insert(uri, lstValues[0]);
				if(resultUri != null){
					int id =  Integer.parseInt(resultUri.getLastPathSegment());
					lstSms.get(0).setId(id);
					result = true;
				}
				else{
					mErrorCode = 1;
				}
			}
			catch (Exception e) {
				e.printStackTrace();
				mErrorCode = 2; //插入短信出现异常
			}
		}
		else {
			try {
				Uri uri = Uri.parse(SMS_URI);
				if (count == mContext.getContentResolver().bulkInsert(uri, lstValues)) {

					String[] columns = new String[] {ID};
					String order = ID + " DESC";

					cur = mContext.getContentResolver().query(uri, columns, null, null, order);
					if (cur != null && cur.moveToFirst()) {
						while (!cur.isAfterLast() && count > 0) {
							int id = cur.getInt(0);

							count--;
							lstSms.get(count).setId(id);

							cur.moveToNext();
						}
					}

					result = true;
				}
				else{
					mErrorCode = 1;
				}
			}
			catch (Exception e) {
				result = false;
				e.printStackTrace();
				mErrorCode = 2;
			}
			finally {
				if (cur != null)
					cur.close();
			}
		}

		return result;
	}

	public boolean AddSms(SmsItem item) {
		boolean result = false;

		ContentValues values = new ContentValues();

		long threadId = getOrCreateThreadId(new String[] {item.getNumber()});

		values.put(THREAD_ID, threadId);
		values.put(ADDRESS, item.getNumber());
		values.put(DATE, item.getDate());
		values.put(TYPE, item.getType());
		values.put(BODY, item.getBody());
		values.put(READ, item.getRead() ? 1 : 0);

		Cursor cur = null;

		try {
			Uri uri = Uri.parse(SMS_URI);
			Uri inserted = mContext.getContentResolver().insert(uri, values);

			if (inserted != null) {
				int id = Integer.parseInt(inserted.getLastPathSegment());
				item.setId(id);
				return true;
			}
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}
		finally {
			if (cur != null)
				cur.close();
		}

		return result;
	}

	public void UpdateSms(ByteReader reader, ByteWriter writer) {
		SmsItem item = new SmsItem();
		item.Read(reader);
		boolean success = UpdateSms(item);
		writer.Write(success);
	}

	private boolean UpdateSms(SmsItem item) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();
			values.put(BODY, item.getBody());
			values.put(DATE, item.getDate());
			Uri uri = Uri.parse(SMS_URI);
			uri = ContentUris.withAppendedId(uri, item.getId());
			mContext.getContentResolver().update(uri, values, null, null);
			result = true;
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		return result;
	}

	public void DeleteSms(ByteReader reader, ByteWriter writer) {
		boolean result = false;

		try {
			int cnt = reader.ReadInt32();
			String where = ID + " in(";
			for (int i = 0; i < cnt; i++) {
				where += reader.ReadInt32() + ",";
			}
			where = where.substring(0, where.length() - 1) + ")";

			Uri uri = Uri.parse(SMS_URI);
		 	int rowsNum = mContext.getContentResolver().delete(uri, where, null);
		 	if (rowsNum == cnt) {
                result = true;
            }
			result = true;
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		writer.Write(result);
	}

	public void DeleteSmsByThread(ByteReader reader, ByteWriter writer) {
		boolean result = false;

		try {
			int cnt = reader.ReadInt32();
			String where = THREAD_ID + " in(";
			for (int i = 0; i < cnt; i++) {
				where += reader.ReadInt32() + ",";
			}
			where = where.substring(0, where.length() - 1) + ")";

			Uri uri = Uri.parse(SMS_URI);
			mContext.getContentResolver().delete(uri, where, null);
			result = true;
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		writer.Write(result);
	}

	public void SendSms(ByteReader reader, ByteWriter writer, int connectionId) {
		SmsItem item = new SmsItem();
		item.Read(reader);
		item.setDate(System.currentTimeMillis());
		SendSms(item, true, connectionId);
		writer.Write(item.getId());
		writer.Write(item.getDate());
	}

	public void ReSendSms(ByteReader reader, ByteWriter writer, int connectionId) {
		int smsId = reader.ReadInt32();
		SmsItem item = GetSmsById(smsId);
		long time = System.currentTimeMillis();
		if (item != null) {
			item.setDate(time);
			UpdateSms(item);
			SendSms(item, false, connectionId);
			writer.Write(true);
		}
		else {
			writer.Write(false);
		}
		writer.Write(time);
	}

	private void SendSms(SmsItem item, boolean isNew, int connectionId) {
		if (isNew) {
			AddSms(item);
		}
		else {
			SetSmsType(mContext, item.getId(), 4);
		}

		SmsSender.SendSms(mContext, item, isNew, connectionId);
	}

	public void SetSmsRead(ByteReader reader, ByteWriter writer) {
		int id = reader.ReadInt32();
		boolean isRead = reader.ReadBoolean();

		boolean result = false;
		Uri uri = ContentUris.withAppendedId(Uri.parse(SMS_URI), id);
		ContentValues values = new ContentValues();
		values.put(READ, isRead == true ? 1 : 0);
		if (this.mContext.getContentResolver().update(uri, values, null, null) != 0)
			result = true;
		else
			result = false;

		writer.Write(result);
	}

	public void GetSMSCount(ByteWriter writer) {
		String[] projection = {ID};

		// Count of all
		String where = TYPE + "<>3";
		Cursor cursor = mContext.getContentResolver().query(Uri.parse(SMS_URI), projection, where, null, null);

		if (cursor != null) {
			int cnt = cursor.getCount();
			cursor.close();
			writer.Write(cnt);
		}

		// Count of unRead
		where = TYPE + " = 1 and " + READ + " = 0 ";
		cursor = mContext.getContentResolver().query(Uri.parse(SMS_URI), projection, where, null, null);

		if (cursor != null) {
			int cnt = cursor.getCount();
			cursor.close();
			writer.Write(cnt);
		}
	}

	@SuppressLint("NewApi")
	public void getDefaultPackName(ByteWriter writer) {
		if(android.os.Build.VERSION.SDK_INT>=19){
			writer.Write(1);
			writer.Write(Sms44.getDefualPakName(mContext));
		}else{
			writer.Write(0);
		}
		
		
	}

	@SuppressLint("NewApi")
	public void setDefaultPack(ByteReader reader, ByteWriter writer) {
		if(android.os.Build.VERSION.SDK_INT>=19){
			String packname = reader.ReadString();
			Sms44.sendChangeDefault(mContext,packname);
			writer.Write(1);
		}else{
			writer.Write(0);
		}

		
	}
}
