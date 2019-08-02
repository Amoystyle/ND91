package daemon.provider.business;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.text.format.DateFormat;

import daemon.model.AlarmInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

public class AlarmProvider implements IProvider {

	private String[] UriStrings = new String[] {"content://com.android.alarmclock/alarm", "content://com.android.deskclock/alarm", "content://com.htc.android.deskclock/alarm", "content://com.samsung.sec.android.clockpackage/alarm",
			"content://com.htc.android.worldclock/alarm"};

	private AlarmAdapter mAdapter = new AlarmAdapter();
	private Context mContext;
	private Uri alarmUri = Uri.parse("content://com.android.alarmclock/alarm");

	public AlarmProvider(Context context) {
		mContext = context;
		BuildURI();
	}

	private void BuildURI() {
		try {
			Uri uri = null;
			for (String uriStr : UriStrings) {
				uri = Uri.parse(uriStr);
				Cursor alarmCur = mContext.getContentResolver().query(uri, null, null, null, null);

				if (alarmCur != null) {
					alarmUri = uri;
					BuildAlarmAdapter(alarmCur);
					break;
				}
			}
		}
		catch (Exception e) {

		}
	}

	private void BuildAlarmAdapter(Cursor alarmCur) {
		if (alarmCur != null) {
			String[] columns = alarmCur.getColumnNames();

			// LogCenter.error("alarm colums", "===");
			// for(String column : columns){
			// LogCenter.error("alarm columns", column);
			// }

			String[] tryColumns1 = new String[] {"_id", "hour", "minutes", "daysofweek", "alarmtime", "enabled", "vibrate", "message", "alert"};
			if (IsArrayContainsItems(columns, tryColumns1)) {
				mAdapter.ID = "_id";
				mAdapter.Hour = "hour";
				mAdapter.MINUTES = "minutes";
				mAdapter.DAYSOFWEEK = "daysofweek";
				mAdapter.ALARMTIME = "alarmtime";
				mAdapter.ENABLED = "enabled";
				mAdapter.VIBRATE = "vibrate";
				mAdapter.MESSAGE = "message";
				mAdapter.ALERT = "alert";
			}
		}
	}

	private boolean IsArrayContainsItem(String[] array, String item) {
		if (array != null && array.length > 0) {
			for (String str : array) {
				if (str.equals(item))
					return true;
			}
		}

		return false;
	}

	private boolean IsArrayContainsItems(String[] array, String[] items) {
		for (String item : items) {
			if (!IsArrayContainsItem(array, item)) {
				return false;
			}
		}

		return true;
	}

	@Override
	public void execute(ProviderExecuteContext ctx) {
		ByteReader reader = ctx.getByteReader();
		ByteWriter writer = ctx.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);
		switch (action) {
			case 1:// ��ѯ
				Query(writer);
				break;
			case 2:// ����
				AddAlarm(reader, writer);
				break;
			case 3:// ɾ��
				DeleteAlarm(reader, writer);
				break;
			case 4:// �޸�
				UpdateAlarm(reader, writer);
				break;
		}
	}

	@Override
	public int getBusiness() {
		// TODO Auto-generated method stub
		return 13;
	}

	private void Query(ByteWriter writer) {
		List<AlarmInfo> alarmLst = getAlarms();

		//为null说明获取异常，返回-1
		if (alarmLst == null) {
			writer.Write(-1);
		}
		else {
			int alarmCnt = alarmLst.size();
			writer.Write(alarmCnt); // д�������Ŀ

			for (AlarmInfo alarm : alarmLst) {
				alarm.Write(writer);
			}
		}
	}

	// ����
	private void AddAlarm(ByteReader reader, ByteWriter writer) {
		AlarmInfo alarm = new AlarmInfo();
		alarm.Read(reader);

		boolean addSuccess = AddAlarm(alarm);
		writer.Write(addSuccess);
		if (addSuccess) {
			writer.Write(alarm.getId());
		}
	}

	// ɾ��
	private void DeleteAlarm(ByteReader reader, ByteWriter writer) {
		Integer id = reader.ReadInt32();

		boolean delSuccess = DeleteAlarm(id);
		writer.Write(delSuccess);
	}

	// ����
	private void UpdateAlarm(ByteReader reader, ByteWriter writer) {
		AlarmInfo alarm = new AlarmInfo();
		alarm.Read(reader);

		boolean updateSuccess = UpdateAlarm(alarm);
		writer.Write(updateSuccess);
	}

	public ArrayList<AlarmInfo> getAlarms() {
		ArrayList<AlarmInfo> alarms = new ArrayList<AlarmInfo>();

		Cursor alarmCur = mContext.getContentResolver().query(alarmUri, null, null, null, null);
		if(alarmCur == null)
			return null;

		try {
			if (alarmCur != null && alarmCur.moveToFirst()) {
				while (!alarmCur.isAfterLast()) {
					AlarmInfo alarm = new AlarmInfo();
					alarm.setId(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("_id")));
					alarm.setHour(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("hour")));
					alarm.setMinutes(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("minutes")));
					alarm.setDaysOfWeek(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("daysofweek")));
					alarm.setAlarmTime(alarmCur.getLong(alarmCur.getColumnIndexOrThrow("alarmtime")));
					alarm.setEnabled(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("enabled")));
					alarm.setVibrate(alarmCur.getInt(alarmCur.getColumnIndexOrThrow("vibrate")));
					alarm.setMessage(alarmCur.getString(alarmCur.getColumnIndexOrThrow("message")));
					alarm.setAlert(alarmCur.getString(alarmCur.getColumnIndexOrThrow("alert")));

					alarms.add(alarm);
					alarmCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		finally {
			if (null != alarmCur) {
				alarmCur.close();
			}
		}

		return alarms;
	}

	// ������ￄ1�7
	public boolean AddAlarm(AlarmInfo alarm) {
		boolean result = false;
		try {
			ContentValues values = new ContentValues();
			values.put("hour", alarm.getHour());
			values.put("minutes", alarm.getMinutes());
			values.put("daysofweek", alarm.getDaysOfWeek());
			values.put("enabled", alarm.getEnabled());
			values.put("vibrate", alarm.getVibrate());
			values.put("message", alarm.getMessage());
			values.put("alert", alarm.getAlert());

			long alarmTime = calculateAlarmTime(alarm.getHour(), alarm.getMinutes());
			values.put("alarmtime", alarmTime);

			Uri inserted = mContext.getContentResolver().insert(alarmUri, values);
			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			alarm.setId(id);

			setNextAlert(mContext);

			result = true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	// ɾ������
	public boolean DeleteAlarm(int id) {
		boolean result = false;
		try {
			Uri uri = ContentUris.withAppendedId(alarmUri, id);
			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				setNextAlert(mContext);
				result = true;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	// ��������
	public boolean UpdateAlarm(AlarmInfo alarm) {
		boolean result = false;
		try {
			ContentValues values = new ContentValues();
			values.put("hour", alarm.getHour());
			values.put("minutes", alarm.getMinutes());
			values.put("daysofweek", alarm.getDaysOfWeek());
			values.put("enabled", alarm.getEnabled());
			values.put("vibrate", alarm.getVibrate());
			values.put("message", alarm.getMessage());
			values.put("alert", alarm.getAlert());
			long alarmTime = calculateAlarmTime(alarm.getHour(), alarm.getMinutes());
			values.put("alarmtime", alarmTime);

			Uri uri = ContentUris.withAppendedId(alarmUri, alarm.getId());
			if (this.mContext.getContentResolver().update(uri, values, null, null) != 0) {

				setNextAlert(mContext);

				result = true;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	// ������һ������ʱ��
	private long calculateAlarmTime(int hour, int minute) {
		// start with now
		Calendar c = Calendar.getInstance();
		c.setTimeInMillis(System.currentTimeMillis());

		int nowHour = c.get(Calendar.HOUR_OF_DAY);
		int nowMinute = c.get(Calendar.MINUTE);

		// if alarm is behind current time, advance one day
		if (hour < nowHour || hour == nowHour && minute <= nowMinute) {
			c.add(Calendar.DAY_OF_YEAR, 1);
		}

		c.set(Calendar.HOUR_OF_DAY, hour);
		c.set(Calendar.MINUTE, minute);
		c.set(Calendar.SECOND, 0);
		c.set(Calendar.MILLISECOND, 0);

		return c.getTimeInMillis();
	}

	// ��������
	private void enableAlert(Context context, int id, long atTimeInMillis) {
		AlarmManager am = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);

		Intent intent = BuildAlarmIntent();
		intent.putExtra("alarm_id", id);
		intent.putExtra("alarm_time", atTimeInMillis);
		PendingIntent sender = PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);

		am.set(AlarmManager.RTC_WAKEUP, atTimeInMillis, sender);

		Calendar c = Calendar.getInstance();
		c.setTime(new java.util.Date(atTimeInMillis));
		String timeString = formatDayAndTime(context, c);
		saveNextAlarm(context, timeString);
		SetStatusBarIcon(true);

	}

	private Intent BuildAlarmIntent() {
		String model = Build.MODEL;
		if (model.equals("qsd8250_surf"))
			return new Intent("com.android.clock.alarmclock.ALARM_ALERT");
		// else if(model.equals("CHERR-i58"))
		// return new Intent("com.clock.alarm.ALARM_ALERT");

		return new Intent("com.android.alarmclock.ALARM_ALERT");
	}

	private void SetStatusBarIcon(boolean enabled) {
		Intent alarmChanged = new Intent("android.intent.action.ALARM_CHANGED");
		alarmChanged.putExtra("alarmSet", enabled);
		this.mContext.sendBroadcast(alarmChanged);
	}

	// ȡ����ￄ1�7
	private void disableAlert(Context context, int id) {
		AlarmManager am = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
		Intent intent = BuildAlarmIntent();
		intent.putExtra("alarm_id", id);
		PendingIntent sender = PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
		am.cancel(sender);
		SetStatusBarIcon(false);
		saveNextAlarm(context, "");
	}

	private int getSnoozeAlarmId() {
		SharedPreferences prefs = mContext.getSharedPreferences("AlarmClock", 0);
		return prefs.getInt("snooze_id", -1);
	}

	// ������һ������
	private void setNextAlert(Context context) {
		int snoozeId = getSnoozeAlarmId();
		if (snoozeId == -1) {

			AlarmCalculator ac = new AlarmCalculator();
			getNextAlarm(ac);
			Integer id = ac.getIndex();
			long atTime = ac.getAlert();

			if (atTime < Long.MAX_VALUE) {
				enableAlert(context, id, atTime);
			}
			else {
				disableAlert(context, id);
			}
		}
		else {
			enableSnoozeAlert(context);
		}
	}

	private void saveNextAlarm(final Context context, String timeString) {
		Settings.System.putString(context.getContentResolver(), Settings.System.NEXT_ALARM_FORMATTED, timeString);
	}

	private String formatDayAndTime(final Context context, Calendar c) {
		String format = get24HourMode(context) ? "E k:mm" : "E h:mm aa";
		return (c == null) ? "" : (String) DateFormat.format(format, c);
	}

	private boolean get24HourMode(final Context context) {
		return android.text.format.DateFormat.is24HourFormat(context);
	}

	// �������ӣ���һ������δ���õ�����µ��ãￄ1�7
	private boolean enableSnoozeAlert(final Context context) {
		SharedPreferences prefs = context.getSharedPreferences("AlarmClock", 0);

		int id = prefs.getInt("snooze_id", -1);

		if (id == -1)
			return false;

		long atTimeInMillis = prefs.getLong("snooze_time", -1);

		enableAlert(context, id, atTimeInMillis);
		return true;
	}

	// ѭ���ж�ȷ����һ���ô���������
	private void getNextAlarm(AlarmCalculator ac) {
		Cursor alarmCur = mContext.getContentResolver().query(alarmUri, null, null, null, null);

		try {
			if (alarmCur != null && alarmCur.moveToFirst()) {
				while (!alarmCur.isAfterLast()) {
					Integer id = alarmCur.getInt(alarmCur.getColumnIndexOrThrow("_id"));
					long alarmTime = alarmCur.getLong(alarmCur.getColumnIndexOrThrow("alarmtime"));
					boolean enabled = alarmCur.getInt(alarmCur.getColumnIndexOrThrow("enabled")) == 1 ? true : false;

					ac.reportAlarm(id, enabled, alarmTime);

					alarmCur.moveToNext();
				}
			}
		}
		finally {
			if (null != alarmCur) {
				alarmCur.close();
			}
		}
	}

	class AlarmCalculator {
		public long mMinAlert = Long.MAX_VALUE;
		public int mMinIdx = -1;

		/**
		 * returns next scheduled alert, MAX_VALUE if none
		 */
		public long getAlert() {
			return mMinAlert;
		}

		public int getIndex() {
			return mMinIdx;
		}

		public void reportAlarm(int idx, boolean enabled, long atTime) {
			if (enabled) {
				if (atTime < mMinAlert) {
					mMinIdx = idx;
					mMinAlert = atTime;
				}
			}
		}
	}

	public class AlarmAdapter {
		public String ID = "_id";
		public String Hour = "hour";
		public String MINUTES = "minutes";
		public String DAYSOFWEEK = "daysofweek";
		public String ALARMTIME = "alarmtime";
		public String ENABLED = "enabled";
		public String VIBRATE = "vibrate";
		public String MESSAGE = "message";
		public String ALERT = "alert";
	}
}
