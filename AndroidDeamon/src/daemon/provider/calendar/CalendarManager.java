package daemon.provider.calendar;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.TimeZone;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.text.TextUtils;
import android.text.format.Time;


import daemon.logger.LogCenter;
import daemon.model.EntityCollection;
import daemon.model.calendar.CalendarEvents;
import daemon.model.calendar.CalendarInfo;
import daemon.model.calendar.IEvents;
import daemon.model.calendar.IReminders;
import daemon.model.calendar.Reminders;
import daemon.protocol.ByteWriter;

public class CalendarManager extends CalendarManagerBase {

	long SECOND_IN_MILLIS = 1000;
	long MINUTE_IN_MILLIS = SECOND_IN_MILLIS * 60;
	long HOUR_IN_MILLIS = MINUTE_IN_MILLIS * 60;
	long DAY_IN_MILLIS = HOUR_IN_MILLIS * 24;
	long WEEK_IN_MILLIS = DAY_IN_MILLIS * 7;
	long YEAR_IN_MILLIS = WEEK_IN_MILLIS * 52;
	private static final int DAY_IN_SECONDS = 24 * 60 * 60;

	long mill = HOUR_IN_MILLIS * 24;
	// private static final String TAG = "CalendarProvider";
	private String CALENDAR_URI = "content://calendar/calendars";
	private String EVENTS_URI = "content://calendar/events";
	public String REMINDERS_URI = "content://calendar/reminders";
	public static final String ADDRESS = "address";
	// Calendars Field
	public static final String CALENDAR_ID = "_id";
	public static final String SYNC_COUNT = "_sync_account";
	public static final String NAME = "name";
	public static final String DISPLAYNAME = "displayName";
	public static final String HIDDEN = "hidden";
	public static final String COLOR = "color";
	public static final String SELECTED = "selected";
	public static final String TIMEZONE = "timezone";
	public static final String ACCESS_LEVEL = "access_level";

	// Events Field
	public static final String ID = "_id";
	public static final String SYNC_ID = "_sync_id";
	public static final String TITLE = "title";
	public static final String EVENTLOCATION = "eventLocation";
	public static final String DESCRIPTION = "description";
	public static final String DTSTART = "dtstart";
	public static final String DTEND = "dtend";
	public static final String ALLDAY = "allDay";
	public static final String HASALARM = "hasAlarm";
	public static final String RRULE = "rrule";
	public static final String EVENT_STATUS = "eventStatus";
	public static final String EVENT_CALENDAR_ID = "calendar_id";
	public static final String ORIGINAL_EVENT = "originalEvent";

	static final int DELETE_SELECTED = 0;
	static final int DELETE_ALL_FOLLOWING = 1;
	static final int DELETE_ALL = 2;

	private static final int MODIFY_SELECTED = 1;
	private static final int MODIFY_ALL = 2;
	private static final int MODIFY_ALL_FOLLOWING = 3;

	private static final int REMINDERS_INDEX_MINUTES = 1;
	private static final String REMINDERS_WHERE = IReminders.EVENT_ID
			+ "=%d AND (" + IReminders.METHOD + "=" + IReminders.METHOD_ALERT
			+ " OR " + IReminders.METHOD + "=" + IReminders.METHOD_DEFAULT
			+ ")";
	//
	// private static final String CANCELEVENT_WHERE = IEvents.CALENDAR_ID
	// + "=%d AND (" + IEvents.STATUS + "=" + 2 + ") " + "AND ("
	// + IEvents.ORIGINAL_EVENT + "=%d " + ")"; // ȡ���¼�
	//
	// private static final String NORMAL_WHERE = IEvents.CALENDAR_ID
	// + "=%d AND (" + IEvents.STATUS + " isnull" + ")"; // ��ȷ�¼�

	// ��ȡ����¼�(��ѯ�����)
	private static final String CALCEL_WHERE = " AND(" + EVENT_STATUS + "=" + 2
			+ ")" + "AND(";

	// �����ڵ��¼�(��ѯ�����)
	private static final String NORMAL_WHERE = " AND (" + EVENT_STATUS
			+ " isnull)";

	private Context mContext;

	public CalendarManager(Context mContext) {
		this.mContext = mContext;
		try {
			Class<?> calendarClass = Class
					.forName("android.provider.Calendar$Calendars");
			CALENDAR_URI = ((Uri) calendarClass.getField("CONTENT_URI").get(
					null)).toString();

			Class<?> EVENTSClass = Class
					.forName("android.provider.Calendar$Events");
			EVENTS_URI = ((Uri) EVENTSClass.getField("CONTENT_URI").get(null))
					.toString();

			Class<?> REMINDERSClass = Class
					.forName("android.provider.Calendar$Reminders");
			REMINDERS_URI = ((Uri) REMINDERSClass.getField("CONTENT_URI").get(
					null)).toString();

		} catch (Exception e) {
			e.printStackTrace();
		}

		/*
		 * CALENDAR_URI =
		 * android.provider.Calendar.Calendars.CONTENT_URI.toString();
		 * EVENTS_URI = android.provider.Calendar.Events.CONTENT_URI.toString();
		 * REMINDERS_URI =
		 * android.provider.Calendar.Reminders.CONTENT_URI.toString();
		 */
	}

	private boolean mHasDeletedColumnChecked = false;
	private boolean mHasDeletedColumn = false;

	public boolean HasDeletedColumn() {

		if (!mHasDeletedColumnChecked) {
			mHasDeletedColumnChecked = true;
			String columnDeleted = "deleted";
			String[] columns = new String[] { columnDeleted };
			Cursor cur = null;

			try {
				cur = mContext.getContentResolver().query(
						Uri.parse(EVENTS_URI), columns, null, null, null);
				if (cur != null)
					mHasDeletedColumn = true;
				else
					mHasDeletedColumn = false;
			} catch (Exception ex) {
				mHasDeletedColumn = false;
			} finally {
				if (cur != null)
					cur.close();
			}
		}

		return mHasDeletedColumn;
	}

	/**
	 * 
	 * @return Query calendar
	 */
	public EntityCollection<CalendarInfo> QueryCalendar() {
		EntityCollection<CalendarInfo> result = null;
		Cursor cal_cursor = mContext.getContentResolver().query(
				Uri.parse(CALENDAR_URI), null, null, null, null);
		// ���α��Ѿ�����ر�
		if (null != cal_cursor) {
			if (cal_cursor.moveToFirst()) {
				try {
					result = new EntityCollection<CalendarInfo>("CalendarInfo");
					CalendarInfo calendarinfo = null;

					while (!cal_cursor.isAfterLast()) {
						calendarinfo = new CalendarInfo();

						int calendarId = cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(CALENDAR_ID));
						String normalwhere = EVENT_CALENDAR_ID + "="
								+ calendarId;
						String NormalWhere = normalwhere + NORMAL_WHERE;
						Cursor event_cursor = mContext.getContentResolver()
								.query(
										// ���������¼����۳�cancel �¼�
										Uri.parse(EVENTS_URI), null,
										NormalWhere, null, null);

						// ����Events,��ȡevents ��cancellist
						EntityCollection<CalendarEvents> calendarEventList = GetCalendarEventFromCursor(
								event_cursor, calendarId);
						if (null != event_cursor) {
							event_cursor.close();
						}
						calendarinfo.setId(calendarId);
						calendarinfo.setSync_count(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(SYNC_COUNT)));

						calendarinfo.setName(cal_cursor.getString(cal_cursor
								.getColumnIndexOrThrow(NAME)));

						calendarinfo.setDisplayName(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(DISPLAYNAME)));

						calendarinfo.setHidden(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(HIDDEN)));

						calendarinfo.setColor(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(COLOR)));

						calendarinfo.setSelected(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(SELECTED)));

						calendarinfo.setTimezone(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(TIMEZONE)));
						if (calendarEventList != null
								&& calendarEventList.size() > 0) {
							calendarinfo.setEvents(calendarEventList);
						}
						// ���
						result.add(calendarinfo);
						cal_cursor.moveToNext();
					}
				} finally {
					if (null != cal_cursor) {
						cal_cursor.close();
					}
				}
			} else {
				if (null != cal_cursor) {
					cal_cursor.close();
				}
			}
		}
		return result;
	}

	public void QueryCalendarAndEvents(ByteWriter writer) {
		String[] columns = new String[] { CALENDAR_ID, SYNC_COUNT, NAME,
				DISPLAYNAME, HIDDEN, COLOR, SELECTED, TIMEZONE, ACCESS_LEVEL };

		String where = ACCESS_LEVEL + " >= 500";

		Cursor cal_cursor = null;
		int calCnt = 0;
		ArrayList<Integer> calIdList = new ArrayList<Integer>();

		try {
			cal_cursor = mContext.getContentResolver().query(
					Uri.parse(CALENDAR_URI), columns, where, null, null);
			if (null != cal_cursor) {
				calCnt = cal_cursor.getCount();
				writer.Write(calCnt); // count

				if (cal_cursor.moveToFirst()) {
					while (!cal_cursor.isAfterLast()) {
						int id = cal_cursor.getInt(0);
						calIdList.add(id);

						writer.Write(id); // id
						writer.WriteBlob(cal_cursor.getBlob(1)); // sync_account
						writer.WriteBlob(cal_cursor.getBlob(2)); // name
						writer.WriteBlob(cal_cursor.getBlob(3)); // displayname
						writer.Write(cal_cursor.getInt(4)); // hidden
						writer.Write(cal_cursor.getInt(5)); // color
						writer.Write(cal_cursor.getInt(6)); // selected
						writer.WriteBlob(cal_cursor.getBlob(7)); // timezone
						writer.Write(cal_cursor.getInt(8)); // access_level
						cal_cursor.moveToNext();
					}
				}
			} else {
				writer.Write(0);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != cal_cursor)
				cal_cursor.close();
		}

		if (calCnt > 0) {
			QueryEvents(calIdList, writer);
			QueryReminds(writer);
		} else {
			writer.Write(0);
		}
	}

	private void QueryEvents(ArrayList<Integer> calIdList, ByteWriter writer) {
		Cursor event_cursor = null;
		String[] columns = new String[] { IEvents.CALENDAR_ID, IEvents._ID,
				IEvents.TITLE, IEvents.EVENT_LOCATION, IEvents.DESCRIPTION,
				IEvents.DTSTART, IEvents.DTEND, IEvents.ALL_DAY,
				IEvents.HAS_ALARM, IEvents.RRULE, IEvents.STATUS,
				IEvents.EVENT_TIMEZONE, IEvents.DURATION,
				IEvents.ORIGINAL_EVENT, IEvents.ORIGINAL_INSTANCE_TIME,
				IEvents.LASTDATE, IEvents._SYNC_ID };
		String where = EVENT_CALENDAR_ID + " in(";
		for (int calId : calIdList) {
			where += calId + ",";
		}

		where = where.substring(0, where.length() - 1);
		where += ")";
		
		if(HasDeletedColumn())
			where += " AND deleted=0";

		try {
			Uri uri = Uri.parse(EVENTS_URI);
			event_cursor = mContext.getContentResolver().query(uri, columns,
					where, null, null);
			// �α��Ѿ��ر�
			if (null != event_cursor) {

				writer.Write(event_cursor.getCount()); // count

				if (event_cursor.moveToFirst()) {
					while (!event_cursor.isAfterLast()) {

						writer.Write(event_cursor.getInt(0)); // calendar_id
						writer.Write(event_cursor.getInt(1)); // _id
						writer.WriteBlob(event_cursor.getBlob(2)); // title
						writer.WriteBlob(event_cursor.getBlob(3)); // location
						writer.WriteBlob(event_cursor.getBlob(4)); // description
						writer.Write(event_cursor.getLong(5)); // startTime
						writer.Write(event_cursor.getLong(6)); // endTime
						writer.Write(event_cursor.getInt(7)); // allDay
						writer.Write(event_cursor.getInt(8)); // hasAlarm
						writer.WriteBlob(event_cursor.getBlob(9)); // rule
						writer.Write(event_cursor.getInt(10)); // status
						writer.WriteBlob(event_cursor.getBlob(11)); // timeZone
						writer.WriteBlob(event_cursor.getBlob(12)); // duration
						writer.WriteBlob(event_cursor.getBlob(13)); // originalEvent
						writer.Write(event_cursor.getLong(14)); // originalInstanceTime
						writer.Write(event_cursor.getLong(15)); // lastDate
						writer.WriteBlob(event_cursor.getBlob(16)); // sync_id
						event_cursor.moveToNext();
					}

				}
			} else {
				writer.Write(0);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != event_cursor) {
				event_cursor.close();
			}
		}
	}

	private void QueryReminds(ByteWriter writer) {
		Uri remindersUri = Uri.parse(REMINDERS_URI);
		Cursor reminderCursor = null;
		String[] columns = new String[] { IReminders._ID, IReminders.EVENT_ID,
				IReminders.MINUTES, IReminders.METHOD };

		String where = IReminders.METHOD + "=" + IReminders.METHOD_ALERT
				+ " OR " + IReminders.METHOD + "=" + IReminders.METHOD_DEFAULT;

		try {
			reminderCursor = mContext.getContentResolver().query(remindersUri,
					columns, where, null, null);

			if (reminderCursor != null) {
				int count = reminderCursor.getCount();
				writer.Write(count);
				if (reminderCursor.moveToFirst()) {
					while (!reminderCursor.isAfterLast()) {
						writer.Write(reminderCursor.getInt(0));
						writer.Write(reminderCursor.getInt(1));
						writer.Write(reminderCursor.getInt(2));
						writer.Write(reminderCursor.getInt(3));
						reminderCursor.moveToNext();
					}
				}
			} else {
				writer.Write(0);
			}
		} catch (Exception e) {

		} finally {
			if (reminderCursor != null)
				reminderCursor.close();
		}

	}

	public EntityCollection<CalendarInfo> QuerySimpleCalendar() {
		EntityCollection<CalendarInfo> result = null;
		Cursor cal_cursor = mContext.getContentResolver().query(
				Uri.parse(CALENDAR_URI), null, null, null, null);
		// �α��Ѿ��ر�
		if (null != cal_cursor) {
			if (cal_cursor.moveToFirst()) {
				try {
					result = new EntityCollection<CalendarInfo>("CalendarInfo");
					CalendarInfo calendarinfo = null;

					while (!cal_cursor.isAfterLast()) {
						calendarinfo = new CalendarInfo();

						int calendarId = cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(CALENDAR_ID));

						String NormalWhere = EVENT_CALENDAR_ID + "="
								+ calendarId;

						Cursor event_cursor = mContext.getContentResolver()
								.query(
										// ���������¼����۳�cancel �¼�
										Uri.parse(EVENTS_URI), null,
										NormalWhere, null, null);

						// // ����Events,��ȡevents û�д�cancellist
						EntityCollection<CalendarEvents> calendarEventList = GetSimpleCalendarEventFromCursor(
								event_cursor, calendarId);

						if (null != event_cursor) {
							event_cursor.close();
						}
						calendarinfo.setId(calendarId);

						calendarinfo.setSync_count(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(SYNC_COUNT)));

						calendarinfo.setName(cal_cursor.getString(cal_cursor
								.getColumnIndexOrThrow(NAME)));

						calendarinfo.setDisplayName(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(DISPLAYNAME)));

						calendarinfo.setHidden(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(HIDDEN)));

						calendarinfo.setColor(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(COLOR)));

						calendarinfo.setSelected(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(SELECTED)));

						calendarinfo.setTimezone(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(TIMEZONE)));
						if (calendarEventList != null
								&& calendarEventList.size() > 0) {
							calendarinfo.setEvents(calendarEventList);
						}
						// ���
						result.add(calendarinfo);
						cal_cursor.moveToNext();
					}
				} finally {
					if (null != cal_cursor) {
						cal_cursor.close();
					}
				}
			} else {
				if (null != cal_cursor) {
					cal_cursor.close();
				}
			}
		}

		return result;
	}

	// ��ȡcalendar��2010
	public EntityCollection<CalendarInfo> QueryCalendarTable() {
		EntityCollection<CalendarInfo> result = null;
		Cursor cal_cursor = mContext.getContentResolver().query(
				Uri.parse(CALENDAR_URI), null, null, null, null);
		// �α��Ѿ��ر�
		if (null != cal_cursor) {
			if (cal_cursor.moveToFirst()) {
				try {
					result = new EntityCollection<CalendarInfo>("CalendarInfo");
					CalendarInfo calendarinfo = null;

					while (!cal_cursor.isAfterLast()) {
						calendarinfo = new CalendarInfo();

						int calendarId = cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(CALENDAR_ID));
						calendarinfo.setId(calendarId);

						calendarinfo.setSync_count(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(SYNC_COUNT)));

						calendarinfo.setName(cal_cursor.getString(cal_cursor
								.getColumnIndexOrThrow(NAME)));

						calendarinfo.setDisplayName(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(DISPLAYNAME)));

						calendarinfo.setHidden(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(HIDDEN)));

						calendarinfo.setColor(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(COLOR)));

						calendarinfo.setSelected(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(SELECTED)));

						calendarinfo.setTimezone(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(TIMEZONE)));
						// ���
						result.add(calendarinfo);
						cal_cursor.moveToNext();
					}
				} finally {
					if (null != cal_cursor) {
						cal_cursor.close();
					}
				}
			} else {
				if (null != cal_cursor) {
					cal_cursor.close();
				}
			}
		}
		return result;
	}

	// 2010
	public EntityCollection<CalendarEvents> QueryEventsByCalendarId(
			int calendarId) {
		String where = EVENT_CALENDAR_ID + "=" + calendarId;
		
		if(HasDeletedColumn())
			where += " AND deleted=0";


		Cursor event_cursor = mContext.getContentResolver().query(
		// ���������¼����۳�cancel �¼�
				Uri.parse(EVENTS_URI), null, where, null, null);
		// ����Events,��ȡevents û�д�cancellist
		EntityCollection<CalendarEvents> calendarEventList = GetSimpleCalendarEventFromCursor(
				event_cursor, calendarId);
		return calendarEventList;
	}

	/**
	 * �ж��Ƿ�����������û����Ƿ�ע����google calenar
	 * 
	 * @return
	 */
	public int QueryCalendarCount() {
		int calendarCount = 0;
		Cursor cal_cursor = mContext.getContentResolver().query(
				Uri.parse(CALENDAR_URI), null, null, null, null);

		if (null != cal_cursor && cal_cursor.moveToFirst()) {
			try {
				calendarCount = cal_cursor.getCount();
				// while (!cal_cursor.isAfterLast()) {
				// calendarCount++;
				// cal_cursor.moveToNext();
				// }
			} finally {
				cal_cursor.close();
			}
		}
		return calendarCount;
	}

	/**
	 * 
	 * @param addEvent
	 * @return
	 */
	public int AddEvent(CalendarEvents addEvent) {
		int select = 1; // Ĭ��
		int resultId = save(null, addEvent, select);
		return resultId;
	}

	// private static final int MODIFY_SELECTED = 1;
	// private static final int MODIFY_ALL = 2;
	// private static final int MODIFY_ALL_FOLLOWING = 3;

	/**
	 * 
	 * @param event
	 * @param modifySelect
	 * @return
	 */
	public int EditEvent(CalendarEvents event, int modifySelect) {
		int resultId = -1;
		Uri eventUri = ContentUris.withAppendedId(Uri.parse(EVENTS_URI), event
				.getId());
		resultId = save(eventUri, event, modifySelect);
		return resultId;
	}

	/**
	 * 
	 * @param start
	 *            Instance start
	 * @param end
	 *            Instance end
	 * @param eventId
	 *            EventId
	 * @param deleteSelect
	 *            DELETE_SELECTED 0 DELETE_ALL_FOLLOWING 1 DELETE_ALL 2
	 * @param repeat
	 *            it is repeat event ?
	 * @return
	 */
	public int DeleteEvent(long start, long end, int eventId, int deleteSelect,
			Boolean repeat, String phoneType, String exdate) {
		int success = -1;
		Cursor mCursor = GetDeleteEventCursor(eventId);
		if ((mCursor == null) || (mCursor.getCount() == 0)) {
			return -1;
		}
		if (repeat) {
			if (!phoneType.equals("lenovo")) {
				success = DeleteRepeatingEvent(start, end, mCursor,
						deleteSelect);
			} else {

				// lenovo
				success = DeleteRepeatingLenovoEvent(start, end, mCursor,
						deleteSelect, phoneType, exdate);
			}
		} else {
			success = DeleteNormalEvent(mCursor);
		}
		return success;
	}

	// Delete Event
	public boolean deleteEventsById(CalendarEvents event) {
		boolean result = false;
		if (null != event) {
			Integer id = event.getId();
			Uri eventUri = ContentUris
					.withAppendedId(Uri.parse(EVENTS_URI), id);
			int count = mContext.getContentResolver().delete(eventUri, null,
					null);
			if (count > 0) {
				result = true;
			}
		}
		return result;
	}

	/**
	 * 
	 * @param eventId
	 *            �¼�ID
	 * @return �¼������б�
	 */
	private EntityCollection<Reminders> GetReminderList(int eventId) {
		EntityCollection<Reminders> reminderList = null;
		Uri remindersUri = Uri.parse(REMINDERS_URI);
		// String where = String.format(REMINDERS_WHERE, eventId);
		String where1 = IReminders.EVENT_ID + "=" + eventId;
		String where2 = " AND (" + IReminders.METHOD + "="
				+ IReminders.METHOD_ALERT + " OR " + IReminders.METHOD + "="
				+ IReminders.METHOD + ")";
		String where = where1 + where2;
		Cursor reminderCursor = mContext.getContentResolver().query(
				remindersUri, null, where, null, null);
		if (reminderCursor != null && reminderCursor.getCount() > 0) {
			reminderList = new EntityCollection<Reminders>("Reminders");
			try {
				while (reminderCursor.moveToNext()) {
					Reminders tempReminder = new Reminders();

					int minutes = reminderCursor.getInt(reminderCursor
							.getColumnIndexOrThrow(IReminders.MINUTES));
					int id = reminderCursor.getInt(reminderCursor
							.getColumnIndexOrThrow(IReminders._ID));
					int method = reminderCursor.getInt(reminderCursor
							.getColumnIndexOrThrow(IReminders.METHOD));

					tempReminder.setId(id);
					tempReminder.setEventId(eventId);
					tempReminder.setMinutes(minutes);
					tempReminder.setMethod(method);
					reminderList.add(tempReminder);
				}

			} finally {
				reminderCursor.close();
			}
		}

		return reminderList;
	}

	/**
	 * 
	 * @param event_cursor
	 * 
	 * @param calendarId
	 * 
	 * @return �¼��б?��ȡ���¼��б�
	 */
	private EntityCollection<CalendarEvents> GetCalendarEventFromCursor(
			Cursor event_cursor, int calendarId) {
		EntityCollection<CalendarEvents> eventList = new EntityCollection<CalendarEvents>(
				"CalendarEvents");
		// �α��Ѿ��ر�
		if (null != event_cursor) {
			if (event_cursor.moveToFirst()) {
				try {
					while (!event_cursor.isAfterLast()) {
						CalendarEvents event = new CalendarEvents();
						int eventId = event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents._ID));
						event.setId(eventId);

						LogCenter.error("info", "Query Query event is"
								+ String.valueOf(event.getId()));

						event.setTitle(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.TITLE)));

						event
								.setEventLocation(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_LOCATION)));

						event
								.setDescription(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.DESCRIPTION)));

						event.setDtstart(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTSTART)));

						event.setDtend(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTEND)));

						event.setAllDay(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.ALL_DAY)));

						event.setHasAlarm(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.HAS_ALARM)));

						event.setRrule(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.RRULE)));

						event.setCalendarId(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.CALENDAR_ID)));

						int status = event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.EVENTSTATUS));
						event.setEventStatus(status);

						event
								.setTimeZone(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_TIMEZONE)));

						event.setDuration(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.DURATION)));

						event
								.setOriginalEvent(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_EVENT)));

						event
								.setOriginalInstanceTime(event_cursor
										.getLong(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_INSTANCE_TIME)));

						event.setLastDate(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.LASTDATE)));
						// event.set
						Cursor cancelCursor = null;
						String calcelevent1 = EVENT_CALENDAR_ID + "="
								+ calendarId;
						String calcelevent2 = ORIGINAL_EVENT + "=" + eventId
								+ ")";
						String cancelEvent = calcelevent1 + CALCEL_WHERE
								+ calcelevent2;
						try {
							cancelCursor = mContext.getContentResolver().query(
									Uri.parse(EVENTS_URI), null, cancelEvent,
									null, null); // ��ȡָ��calendarId
						} catch (Exception de) {
							cancelCursor = null;
						}
						EntityCollection<CalendarEvents> cancelEventList = null;

						// get reminders
						EntityCollection<Reminders> reminderList = GetReminderList(event
								.getId());

						if (reminderList != null && reminderList.size() > 0) {
							LogCenter.error("info", " event is"
									+ String.valueOf(event.getId())
									+ " add reming  rrrrrrr");
							event.setChildReminders(reminderList);
						}

						if (null != cancelCursor && cancelCursor.getCount() > 0) {
							LogCenter.error("info", "QUERY Event is"
									+ String.valueOf(event.getId())
									+ "has child"); // ĳ���¼�������ȡ���¼�
							cancelEventList = GetCalendarEventFromCursor(
									cancelCursor, calendarId);
						}
						if (cancelEventList != null
								&& cancelEventList.size() > 0) {

							event.setCancelEventList(cancelEventList); // ȡ���¼�

							LogCenter.error("info", "qqqqqqqqqq event is"
									+ String.valueOf(event.getId())
									+ " add child");
						}

						eventList.add(event);
						LogCenter.error("info", "Calendar add event is"
								+ String.valueOf(event.getId()));

						event_cursor.moveToNext();
					}
				} finally {
					if (null != event_cursor) {
						event_cursor.close();
					}
				}
			} else {
				if (null != event_cursor) {
					event_cursor.close();
				}
			}
		}

		if (null != event_cursor && event_cursor.moveToFirst()) {

		}
		return eventList;
	}

	/**
	 * �¼�һ�������¼���״̬Ϊȡ����¼��������Ӽ��������ӣ�
	 * 
	 * @param event_cursor
	 * @param calendarId
	 * @return
	 */
	private EntityCollection<CalendarEvents> GetSimpleCalendarEventFromCursor(
			Cursor event_cursor, int calendarId) {
		EntityCollection<CalendarEvents> eventList = new EntityCollection<CalendarEvents>(
				"CalendarEvents");

		// ================Lephone===============================
		boolean exist_Lephone_Category = false;
		String[] columnNames = event_cursor.getColumnNames();
		if (columnNames.length > 0) {
			for (String columnName : columnNames) {
				if (columnName.toString().equals(IEvents.LEPHONE_CATEGORY)) {
					exist_Lephone_Category = true;
					break;
				}
			}
		}
		// ================Lephone===============================

		// �α��Ѿ��ر�
		if (null != event_cursor) {
			if (event_cursor.moveToFirst()) {
				try {
					while (!event_cursor.isAfterLast()) {
						CalendarEvents event = new CalendarEvents();
						int eventId = event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents._ID));
						event.setId(eventId);

						if (exist_Lephone_Category) {
							event
									.setLephone_Category(event_cursor
											.getInt(event_cursor
													.getColumnIndexOrThrow(IEvents.LEPHONE_CATEGORY)));
							event.setPhone_Category("lenovo");
							LogCenter.error("info",
									"Phone----->>> is is is---->>> Lephone!");
						} else {
							LogCenter.error("info",
									"Phone----->>> is not---->>> Lephone!");
						}

						event.setExdate(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.EXDATE)));

						event.setTitle(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.TITLE)));

						event
								.setEventLocation(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_LOCATION)));

						event
								.setDescription(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.DESCRIPTION)));

						event.setDtstart(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTSTART)));

						event.setDtend(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTEND)));

						event.setAllDay(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.ALL_DAY)));

						event.setHasAlarm(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.HAS_ALARM)));

						event.setRrule(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.RRULE)));

						event.setCalendarId(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.CALENDAR_ID)));

						int status = event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.EVENTSTATUS));
						event.setEventStatus(status);

						event
								.setTimeZone(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_TIMEZONE)));

						event.setDuration(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.DURATION)));

						event
								.setOriginalEvent(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_EVENT)));

						event
								.setOriginalInstanceTime(event_cursor
										.getLong(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_INSTANCE_TIME)));

						event.setLastDate(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.LASTDATE)));

						event.setSync_id(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents._SYNC_ID)));
						// get reminders
						EntityCollection<Reminders> reminderList = GetReminderList(event
								.getId());

						if (reminderList != null && reminderList.size() > 0) {
							LogCenter.error("info", " event is"
									+ String.valueOf(event.getId())
									+ " add reming  rrrrrrr");
							event.setChildReminders(reminderList);
						}

						eventList.add(event);
						LogCenter.error("info", "Calendar add event is"
								+ String.valueOf(event.getId()));

						event_cursor.moveToNext();
					}
				} catch (Exception e) {
					LogCenter.error("calendar", e.getMessage());
				} finally {
					if (null != event_cursor) {
						event_cursor.close();
					}
				}
			} else {
				if (null != event_cursor) {
					event_cursor.close();
				}
			}
		}
		return eventList;
	}

	private Cursor GetDeleteEventCursor(int eventId) {
		Cursor mCursor = null;
		Uri eventsURL = Uri.parse(EVENTS_URI);
		Uri eventUri = ContentUris.withAppendedId(eventsURL, eventId); // event
		ContentResolver cr = mContext.getContentResolver();
		mCursor = cr.query(eventUri, null, null, null, null); //�õ����α������ֻ��
		// һ�����
		if ((mCursor == null) || (mCursor.getCount() == 0)) {
			return mCursor;
		}
		mCursor.moveToFirst();
		return mCursor;
	}

	private int DeleteNormalEvent(Cursor mCursor) {
		int result = 0;
		try {
			Uri eventsURL = Uri.parse(EVENTS_URI);
			int mEventIndexId = mCursor.getColumnIndexOrThrow(IEvents._ID);
			long id = mCursor.getInt(mEventIndexId);
			Uri uri = ContentUris.withAppendedId(eventsURL, id);
			ContentResolver mContentResolver = mContext.getContentResolver();
			result = mContentResolver
					.delete(uri, null /* where */, null /* selectionArgs */); // ɾ��
		} catch (Exception ex) {
			LogCenter.error("info", "Delte Fail,Delte Fail,Delte NormalEvent Fail :"
					+ ex.toString());
			result = -1;
		}
		return result;
	}

	private int DeleteRepeatingEvent(long start, long end, Cursor mCursor,
			int deleteSelect) {
		int result = -1;

		Uri eventsURL = Uri.parse(EVENTS_URI);
		String mSyncId;
		long mStartMillis = start; // Instance���е�ʱ��
		long mEndMillis = end; // Instance���е�ʱ��
		ContentResolver cr = mContext.getContentResolver();

		int mEventIndexId = mCursor.getColumnIndexOrThrow(IEvents._ID);
		int indexDtstart = mCursor.getColumnIndexOrThrow(IEvents.DTSTART);
		int indexAllDay = mCursor.getColumnIndexOrThrow(IEvents.ALL_DAY);
		int indexTitle = mCursor.getColumnIndexOrThrow(IEvents.TITLE);
		int indexTimezone = mCursor
				.getColumnIndexOrThrow(IEvents.EVENT_TIMEZONE);
		int indexCalendarId = mCursor
				.getColumnIndexOrThrow(IEvents.CALENDAR_ID);
		int mEventIndexRrule = mCursor.getColumnIndexOrThrow(IEvents.RRULE);

		int eventIndexSyncId = mCursor.getColumnIndexOrThrow(IEvents._SYNC_ID);
		mSyncId = mCursor.getString(eventIndexSyncId);

		String rRule = mCursor.getString(mEventIndexRrule);
		boolean allDay = mCursor.getInt(indexAllDay) != 0;
		long dtstart = mCursor.getLong(indexDtstart); // event �Ͽ�ʼʱ��
		long id = mCursor.getInt(mEventIndexId);
		switch (deleteSelect) {
		case DELETE_SELECTED:

			// If we are deleting the first event in the series, then
			// instead of creating a recurrence exception, just change
			// the start time of the recurrence.
			if (dtstart == mStartMillis) {
			}

			// Create a recurrence exception by creating a new event
			// with the status "cancelled".
			ContentValues values = new ContentValues();

			// The title might not be necessary, but it makes it easier
			// to find this entry in the database when there is a problem.
			String title = mCursor.getString(indexTitle);
			values.put(IEvents.TITLE, title);

			String timezone = mCursor.getString(indexTimezone);
			int calendarId = mCursor.getInt(indexCalendarId);
			values.put(IEvents.EVENT_TIMEZONE, timezone);
			values.put(IEvents.ALL_DAY, allDay ? 1 : 0);
			values.put(IEvents.CALENDAR_ID, calendarId);
			values.put(IEvents.DTSTART, mStartMillis);
			values.put(IEvents.DTEND, mEndMillis);
			values.put(IEvents.ORIGINAL_EVENT, mSyncId);
			values.put(IEvents.ORIGINAL_INSTANCE_TIME, mStartMillis);
			values.put(IEvents.LASTDATE, mEndMillis);
			values.put(IEvents.STATUS, 2);
			try {
				Uri uri = cr.insert(eventsURL, values);
				result = (int) ContentUris.parseId(uri);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_SELECTED :"
								+ ex.toString());
				result = -1;
			}

			break;
		case DELETE_ALL:
			Uri uri = ContentUris.withAppendedId(eventsURL, id); // Calendar.
			// Events.
			// CONTENT_URI
			try {
				result = cr
						.delete(uri, null /* where */, null /* selectionArgs */);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_ALL :"
								+ ex.toString());
				result = -1;
			}

			break;
		case DELETE_ALL_FOLLOWING:
			// If we are deleting the first event in the series and all
			// following events, then delete them all.
			if (dtstart == mStartMillis) {
				Uri uri2 = ContentUris.withAppendedId(eventsURL, id);
				result = cr
						.delete(uri2, null /* where */, null /* selectionArgs */);
				break;
			}

			long lastdate = mStartMillis - 1000;

			String rl = ConvertRrul(lastdate);
			String transRrule = GetrruleWithOut(rRule);
			StringBuilder s = new StringBuilder();
			s.append(transRrule);
			s.append(";UNTIL=");
			s.append(rl);

			ContentValues values3 = new ContentValues();
			values3.put(IEvents.DTSTART, dtstart);
			values3.put(IEvents.RRULE, s.toString());
			Uri uri3 = ContentUris.withAppendedId(eventsURL, id);
			try {
				result = cr.update(uri3, values3, null, null);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_ALL_FOLLOWING :"
								+ ex.toString());
				result = -1;
			}

			break;
		default:
			break;
		}
		return result;
	}

	/*
	 * ɾ��j���¼�20100128 hkk
	 */
	private int DeleteRepeatingLenovoEvent(long start, long end,
			Cursor mCursor, int deleteSelect, String phoneType, String addExdate) {
		int result = -1;

		Uri eventsURL = Uri.parse(EVENTS_URI);
		String mSyncId;
		long mStartMillis = start; // Instance���е�ʱ��
		long mEndMillis = end; // Instance���е�ʱ��
		ContentResolver cr = mContext.getContentResolver();

		int mEventIndexId = mCursor.getColumnIndexOrThrow(IEvents._ID);
		int indexDtstart = mCursor.getColumnIndexOrThrow(IEvents.DTSTART);
		int indexAllDay = mCursor.getColumnIndexOrThrow(IEvents.ALL_DAY);
		int indexTitle = mCursor.getColumnIndexOrThrow(IEvents.TITLE);
		int indexTimezone = mCursor
				.getColumnIndexOrThrow(IEvents.EVENT_TIMEZONE);
		int indexCalendarId = mCursor
				.getColumnIndexOrThrow(IEvents.CALENDAR_ID);
		int mEventIndexRrule = mCursor.getColumnIndexOrThrow(IEvents.RRULE);

		int eventIndexSyncId = mCursor.getColumnIndexOrThrow(IEvents._SYNC_ID);
		mSyncId = mCursor.getString(eventIndexSyncId);

		String rRule = mCursor.getString(mEventIndexRrule);

		long dtstart = mCursor.getLong(indexDtstart); // event �Ͽ�ʼʱ��
		long id = mCursor.getInt(mEventIndexId);
		switch (deleteSelect) {
		case DELETE_SELECTED:

			int indexExdate = mCursor.getColumnIndexOrThrow(IEvents.EXDATE);
			String oldExdate = mCursor.getString(indexExdate);
			String newExdate = "";

			if (null != oldExdate && !oldExdate.equals("")) {
				newExdate = oldExdate + "," + addExdate; // lenovo 2010.01.28
				// hkk
			} else {
				newExdate = addExdate; // lenovo 2010.01.28
			}

			ContentValues valuesDelete = new ContentValues();
			valuesDelete.put(IEvents.EXDATE, newExdate);

			Uri uriDelete = ContentUris.withAppendedId(eventsURL, id);
			try {
				result = cr.update(uriDelete, valuesDelete, null, null);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_ALL_FOLLOWING :"
								+ ex.toString());
				result = -1;
			}

			break;
		case DELETE_ALL:
			Uri uri = ContentUris.withAppendedId(eventsURL, id); // Calendar.
			// Events.
			// CONTENT_URI
			try {
				result = cr
						.delete(uri, null /* where */, null /* selectionArgs */);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_ALL :"
								+ ex.toString());
				result = -1;
			}

			break;
		case DELETE_ALL_FOLLOWING:
			// If we are deleting the first event in the series and all
			// following events, then delete them all.
			if (dtstart == mStartMillis) {
				Uri uri2 = ContentUris.withAppendedId(eventsURL, id);
				result = cr
						.delete(uri2, null /* where */, null /* selectionArgs */);
				break;
			}

			long lastdate = mStartMillis - 1000;

			String rl = ConvertRrul(lastdate);
			String transRrule = GetrruleWithOut(rRule);
			StringBuilder s = new StringBuilder();
			s.append(transRrule);
			s.append(";UNTIL=");
			s.append(rl);

			ContentValues values3 = new ContentValues();
			values3.put(IEvents.DTSTART, dtstart);
			values3.put(IEvents.RRULE, s.toString());
			Uri uri3 = ContentUris.withAppendedId(eventsURL, id);
			try {
				result = cr.update(uri3, values3, null, null);
			} catch (Exception ex) {
				LogCenter.error("info",
						"Delte Fail,Delte Fail,Delte RepeatingEvent Fail ,when DELETE_ALL_FOLLOWING :"
								+ ex.toString());
				result = -1;
			}

			break;
		default:
			break;
		}
		return result;
	}

	public String ConvertRrul(long timeStamp) {
		// DateTime now = System.DateTime.Now.AddHours(-8);
		String rrul = "";
		// rrul = s + "T105959Z";
		// rrul = s + "T" + now.ToString("HHmmss") + "Z";
		// long timeStamp =1252537200000l;
		Calendar c = Calendar.getInstance();
		c.setTime(new Date(timeStamp));
		// c.add(Calendar.HOUR_OF_DAY, 8);
		rrul = writeDateTime(c);
		return rrul;
	}

	private String writeDateTime(Calendar cal) {
		TimeZone tz = TimeZone.getTimeZone("GMT");
		GregorianCalendar c = new GregorianCalendar(tz);
		c.setTimeInMillis(cal.getTimeInMillis());
		return writeDateTime(c, true);
	}

	private String writeDateTime(Calendar cal, boolean zulu) {
		StringBuilder sb = new StringBuilder();
		sb.ensureCapacity(16);
		if (zulu) {
			sb.setLength(16);
			sb.setCharAt(15, 'Z');
		} else {
			sb.setLength(15);
		}
		return writeDateTime(cal, sb);
	}

	private String writeDateTime(Calendar cal, StringBuilder sb) {
		int n;

		n = cal.get(Calendar.YEAR);
		sb.setCharAt(3, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(2, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(1, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(0, (char) ('0' + n % 10));

		n = cal.get(Calendar.MONTH) + 1;
		sb.setCharAt(5, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(4, (char) ('0' + n % 10));

		n = cal.get(Calendar.DAY_OF_MONTH);
		sb.setCharAt(7, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(6, (char) ('0' + n % 10));

		sb.setCharAt(8, 'T');

		n = cal.get(Calendar.HOUR_OF_DAY);
		sb.setCharAt(10, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(9, (char) ('0' + n % 10));

		n = cal.get(Calendar.MINUTE);
		sb.setCharAt(12, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(11, (char) ('0' + n % 10));

		n = cal.get(Calendar.SECOND);
		sb.setCharAt(14, (char) ('0' + n % 10));
		n /= 10;
		sb.setCharAt(13, (char) ('0' + n % 10));

		return sb.toString();
	}

	private String GetrruleWithOut(String oldrrule) {
		StringBuilder s = new StringBuilder();
		String[] parts = null;
		parts = oldrrule.split(";");
		for (String part : parts) {
			if (!part.contains("UNTIL")) {
				s.append(part + ";");
			}
		}
		String rrul = s.toString();
		return rrul.substring(0, rrul.length() - 1);
	}

	// get Form Value
	private ContentValues getContentValuesFromUi(CalendarEvents editEvent) {
		String title = editEvent.getTitle();
		boolean isAllDay = editEvent.getAllDay() == 0 ? false : true;
		String location = editEvent.getEventLocation();
		String description = editEvent.getDescription();

		ContentValues values = new ContentValues();
		String timezone = null;
		long startMillis = editEvent.getDtstart();
		// if (editEvent.getDtend() != null) {
		// long endMillis = editEvent.getDtend();
		// values.put(IEvents.DTEND, endMillis);
		// }

		int calendarId = editEvent.getCalendarId();

		timezone = editEvent.getTimeZone();
		if (TextUtils.isEmpty(timezone)) {
			timezone = TimeZone.getDefault().getID();
		}

		values.put(IEvents.CALENDAR_ID, calendarId);
		values.put(IEvents.EVENT_TIMEZONE, timezone);
		values.put(IEvents.TITLE, title);
		values.put(IEvents.ALL_DAY, isAllDay ? 1 : 0);
		values.put(IEvents.DTSTART, startMillis);

		values.put(IEvents.DESCRIPTION, description);
		values.put(IEvents.EVENT_LOCATION, location);
		values.put(IEvents.TRANSPARENCY, 0); // default value

		// =======================================
		// lenovo
		// =======================================
		if (editEvent.getPhone_Category().equals("lenovo")) {
			values.put(IEvents.LEPHONE_CATEGORY, editEvent
					.getlephone_Category());
		}

		int visibility = 0; // default value
		// if (visibility > 0) {
		// // For now we the array contains the values 0, 2, and 3. We add one
		// // to match.
		// visibility++;
		// }
		values.put(IEvents.VISIBILITY, visibility);
		return values;
	}

	private void addRecurrenceRule(ContentValues values, String mRrule,
			long start, long end, boolean isAllDay) {

		if (mRrule == null || mRrule == "") {
			return;
		}
		values.put(IEvents.RRULE, mRrule);
		String duration;

		if (isAllDay) {
			long days = (end - start + DAY_IN_MILLIS - 1) / DAY_IN_MILLIS;
			duration = "P" + days + "D";
		} else {
			long seconds = (end - start) / SECOND_IN_MILLIS;
			duration = "P" + seconds + "S";
		}
		values.put(IEvents.DURATION, duration);
	}

	private int save(Uri mUri, CalendarEvents editEvent, int mModification) {

		int result = -1; // -1 is false

		Cursor mEventCursor = null;
		ContentValues mInitialValues = new ContentValues(); // �޸�ǰ���

		ContentResolver cr = mContext.getContentResolver();
		Uri uri = mUri; // mUri Ϊ�� ˵������������� //���ǿ����޸��¼�
		ContentValues values = getContentValuesFromUi(editEvent);// ����û�����
		// Events
		// .DTEND
		// �ӽ����ϻ�ȡֵ
		String mNewRrule = editEvent.getRrule();
		String mRrule = "";
		long start = editEvent.getDtstart();

		long end = 0;
		if (editEvent.getDtend() != null)
			end = editEvent.getDtend();
		boolean isAllDay = editEvent.getAllDay() == 0 ? false : true;

		boolean exist_Lephone_Category = false;

		if (uri != null) {
			mEventCursor = cr.query(mUri, null, null, null, null);
			if (mEventCursor == null || mEventCursor.getCount() == 0) {
				// The cursor is empty. This can happen if the event was
				// deleted.
				if (null != mEventCursor) {
					mEventCursor.close();
				}
				return result;
			}
			mEventCursor.moveToFirst();
			mRrule = mEventCursor.getString(mEventCursor
					.getColumnIndex(IEvents.RRULE)); // �޸�ǰ���ظ�ֵ

			long oldBegin = editEvent.getInstaceBegin(); // ������instance
			// ����begin
			long oldEnd = editEvent.getInstaceEnd(); // ������instance ����end
			int oldAllDayNum = mEventCursor.getInt(mEventCursor
					.getColumnIndex(IEvents.ALL_DAY));
			Boolean oldAll = oldAllDayNum == 0 ? false : true;

			String oldTimezone = mEventCursor.getString(mEventCursor
					.getColumnIndex(IEvents.EVENT_TIMEZONE));
			int oldCalendarId = mEventCursor.getInt(mEventCursor
					.getColumnIndex(IEvents.CALENDAR_ID));

			// ================================================
			// lenovo
			// ================================================
			String[] columnNames = mEventCursor.getColumnNames();
			if (uri != null) {
				if (columnNames.length > 0) {
					for (String columnName : columnNames) {
						if (columnName.toString().equals(
								IEvents.LEPHONE_CATEGORY)) {
							exist_Lephone_Category = true;
							break;
						}
					}
				}
			}

			mInitialValues.put(IEvents.DTSTART, oldBegin);
			mInitialValues.put(IEvents.DTEND, oldEnd);
			mInitialValues.put(IEvents.ALL_DAY, oldAll);
			mInitialValues.put(IEvents.RRULE, mRrule);
			mInitialValues.put(IEvents.EVENT_TIMEZONE, oldTimezone);
			mInitialValues.put(IEvents.CALENDAR_ID, oldCalendarId);

			if (exist_Lephone_Category) {
				int lephone_category = mEventCursor.getInt(mEventCursor
						.getColumnIndexOrThrow(IEvents.LEPHONE_CATEGORY));
				mInitialValues.put(IEvents.LEPHONE_CATEGORY, lephone_category);
			}

			if (editEvent.getDtend() != null) { // ���������Ļ�
				long endMillis = editEvent.getDtend();
				values.put(IEvents.DTEND, endMillis);
			}
		}

		if (uri == null) { // ���
			if (mNewRrule != null && !mNewRrule.equals("")) {
				values.put(IEvents.RRULE, mNewRrule); // repeat
				String duration;
				if (isAllDay) {
					long days = (end - start + DAY_IN_MILLIS - 1)
							/ DAY_IN_MILLIS;
					duration = "P" + days + "D";
				} else {
					long seconds = (end - start) / SECOND_IN_MILLIS;
					duration = "P" + seconds + "S";
				}
				values.put(IEvents.DURATION, duration);
			} else {
				if (editEvent.getDtend() != null) { // ���������Ļ�
					long endMillis = editEvent.getDtend();
					values.put(IEvents.DTEND, endMillis);
				}
			}
			Uri insertEventsURL = Uri.parse(EVENTS_URI);
			uri = cr.insert(insertEventsURL, values);
		} else if ((mRrule == null || mRrule.equals("")) // 1.noRepeat --> noRepeat
				&& (mNewRrule == null || mNewRrule.equals(""))) {
			addRecurrenceRule(values, mNewRrule, start, end, isAllDay);
			checkTimeDependentFields(values, mInitialValues, mModification,
					mEventCursor);
			cr.update(uri, values, null, null);
		} else if (mInitialValues.getAsString(IEvents.RRULE) == null //2.noRepeat
				// -->
				// Repeat
				&& mNewRrule != null) {
			addRecurrenceRule(values, mNewRrule, start, end, isAllDay);
			values.remove(IEvents.DTEND);
			cr.update(uri, values, null, null);
		} else if (mModification == MODIFY_SELECTED) {

			if (!exist_Lephone_Category) {
				long begin = mInitialValues.getAsLong(IEvents.DTSTART);
				values.put(IEvents.ORIGINAL_EVENT, mEventCursor
						.getString(mEventCursor
								.getColumnIndex(IEvents._SYNC_ID)));
				values.put(IEvents.ORIGINAL_INSTANCE_TIME, begin);
				boolean allDay = mInitialValues.getAsBoolean(IEvents.ALL_DAY);
				values.put(IEvents.ALL_DAY, allDay ? 1 : 0);
				uri = cr.insert(Uri.parse(EVENTS_URI), values);
			} else {
				// 1.�޸�exdate

				long lastdate = mInitialValues.getAsLong(IEvents.DTSTART);
				String addExdate = ConvertRrul(lastdate);
				int indexExdate = mEventCursor
						.getColumnIndexOrThrow(IEvents.EXDATE);
				String oldExdate = mEventCursor.getString(indexExdate);
				String newExdate = "";
				if (null != oldExdate && !oldExdate.equals("")) {
					newExdate = oldExdate + "," + addExdate; // lenovo
				} else {
					newExdate = addExdate; // lenovo 2010.01.28
				}

				ContentValues exdateValues = new ContentValues();
				exdateValues.put(IEvents.EXDATE, newExdate);
				cr.update(uri, exdateValues, null, null);

				// 2.����
				values.put(IEvents.ALL_DAY, 0);
				uri = cr.insert(Uri.parse(EVENTS_URI), values);
			}

		} else if (mModification == MODIFY_ALL_FOLLOWING) {
			addRecurrenceRule(values, mNewRrule, start, end, isAllDay);
			if (mNewRrule == null||mNewRrule.equals("")) {
				if (isFirstEventInSeries(mEventCursor, start)) {
					cr.delete(uri, null, null);
				} else {
					updatePastEvents(cr, uri, mEventCursor, start);
				}
				uri = cr.insert(Uri.parse(EVENTS_URI), values);
			} else {
				if (isFirstEventInSeries(mEventCursor, start)) {
					checkTimeDependentFields(values, mInitialValues,
							mModification, mEventCursor);
					values.remove(IEvents.DTEND);
					cr.update(uri, values, null, null);
				} else {
					updatePastEvents(cr, uri, mEventCursor, start);
					values.remove(IEvents.DTEND);
					uri = cr.insert(Uri.parse(EVENTS_URI), values);
				}
			}
		} else if (mModification == MODIFY_ALL) {

			// Modify all instances of repeating event
			// addRecurrenceRule(values);
			addRecurrenceRule(values, mNewRrule, start, end, isAllDay);

			if (mNewRrule == null||mNewRrule.equals("")) {
				// We've changed a recurring event to a non-recurring event.
				// Delete the whole series and replace it with a new
				// non-recurring event.
				cr.delete(uri, null, null);
				uri = cr.insert(Uri.parse(EVENTS_URI), values);
			} else {
				checkTimeDependentFields(values, mInitialValues, mModification,
						mEventCursor);
				values.remove(IEvents.DTEND);
				cr.update(uri, values, null, null);
			}
		}
		if (null != mEventCursor) {
			mEventCursor.close();
		}

		// �����Ǵ���һ��ʵ��,�����������ѵ�
		if (uri != null) {
			long eventId = ContentUris.parseId(uri);
			result = (int) eventId;
			Uri remindersUri = Uri.parse(REMINDERS_URI);
			ArrayList<Integer> mOriginalMinutes = new ArrayList<Integer>();
			String where = String.format(REMINDERS_WHERE, eventId);
			Cursor reminderCursor = cr.query(remindersUri, null, where, null,
					null);
			if (null != reminderCursor) {
				try {
					while (reminderCursor.moveToNext()) {
						int minutes = reminderCursor
								.getInt(REMINDERS_INDEX_MINUTES);
						mOriginalMinutes.add(minutes);
					}
				} finally {
					reminderCursor.close();
				}
			}
			ArrayList<Integer> reminderMinutes = new ArrayList<Integer>();
			List<Reminders> remindersList = editEvent.getChildReminders();
			for (Reminders x : remindersList) {
				reminderMinutes.add(x.getMinutes());
			}
			saveReminders(cr, eventId, reminderMinutes, mOriginalMinutes);
		}
		return result;
	}

	private void updatePastEvents(ContentResolver cr, Uri uri,
			Cursor mEventCursor, long newBegin) {
		long oldStartMillis = mEventCursor.getLong(mEventCursor
				.getColumnIndexOrThrow(IEvents.DTSTART));
		String oldDuration = mEventCursor.getString(mEventCursor
				.getColumnIndexOrThrow(IEvents.DURATION));
		boolean allDay = mEventCursor.getInt(mEventCursor
				.getColumnIndexOrThrow(IEvents.ALL_DAY)) != 0;
		String oldRrule = mEventCursor.getString(mEventCursor
				.getColumnIndexOrThrow(IEvents.RRULE));

		Time untilTime = new Time();
		ContentValues oldValues = new ContentValues();

		untilTime.timezone = Time.TIMEZONE_UTC;

		long lastdate = newBegin - 1000;

		String oldRruleWithUntil = GetrruleWithOut(oldRrule);
		String r1 = ConvertRrul(lastdate);

		StringBuilder s = new StringBuilder();
		s.append(oldRruleWithUntil);
		s.append(";UNTIL=");
		s.append(r1);
		if (allDay) {
			int len = oldDuration.length();
			if (oldDuration.charAt(0) == 'P'
					&& oldDuration.charAt(len - 1) == 'S') {
				int seconds = Integer.parseInt(oldDuration
						.substring(1, len - 1));
				int days = (seconds + DAY_IN_SECONDS - 1) / DAY_IN_SECONDS;
				oldDuration = "P" + days + "D";
			}
		}
		oldValues.put(IEvents.DTSTART, oldStartMillis);
		oldValues.put(IEvents.DURATION, oldDuration);
		oldValues.put(IEvents.RRULE, s.toString());
		cr.update(uri, oldValues, null, null);
	}

	private boolean isFirstEventInSeries(Cursor mEventCursor, long mStartTime) { // mStartTime
		// /
		// /
		// ��������ϵ�ʱ��
		int dtStart = mEventCursor.getColumnIndexOrThrow(IEvents.DTSTART);
		long start = mEventCursor.getLong(dtStart);
		return start == mStartTime;
	}

	private void checkTimeDependentFields(ContentValues values,
			ContentValues mInitialValues, int mModification, Cursor mEventCursor) {

		long oldBegin = mInitialValues.getAsLong(IEvents.DTSTART); //instance���
		// �����
		long oldEnd = mInitialValues.getAsLong(IEvents.DTEND);
		boolean oldAllDay = mInitialValues.getAsBoolean(IEvents.ALL_DAY);

		String oldRrule = mInitialValues.getAsString(IEvents.RRULE);
		String oldTimezone = mInitialValues.getAsString(IEvents.EVENT_TIMEZONE);

		long newBegin = values.getAsLong(IEvents.DTSTART);
		long newEnd = 0;
		if (values.getAsLong(IEvents.DTEND) != null) {
			newEnd = values.getAsLong(IEvents.DTEND);
		}

		boolean newAllDay = values.getAsInteger(IEvents.ALL_DAY) == 1;
		String newRrule = values.getAsString(IEvents.RRULE);
		String newTimezone = values.getAsString(IEvents.EVENT_TIMEZONE);

		// If none of the time-dependent fields changed, then remove them.
		if (oldBegin == newBegin && oldEnd == newEnd && oldAllDay == newAllDay
				&& TextUtils.equals(oldRrule, newRrule)
				&& TextUtils.equals(oldTimezone, newTimezone)) {
			values.remove(IEvents.DTSTART);
			values.remove(IEvents.DTEND);
			values.remove(IEvents.DURATION);
			values.remove(IEvents.ALL_DAY);
			values.remove(IEvents.RRULE);
			values.remove(IEvents.EVENT_TIMEZONE);
			return;
		}

		if (oldRrule == null || newRrule == null) {
			return;
		}

		// If we are modifying all events then we need to set DTSTART to the
		// start time of the first event in the series, not the current
		// date and time. If the start time of the event was changed
		// (from, say, 3pm to 4pm), then we want to add the time difference
		// to the start time of the first event in the series (the DTSTART
		// value). If we are modifying one instance or all following instances,
		// then we leave the DTSTART field alone.
		if (mModification == MODIFY_ALL) {
			long oldStartMillis = mEventCursor.getLong(mEventCursor
					.getColumnIndex(IEvents.DTSTART));
			if (oldBegin != newBegin) {
				// The user changed the start time of this event
				long offset = newBegin - oldBegin;
				oldStartMillis += offset;
			}
			values.put(IEvents.DTSTART, oldStartMillis);
		}
	}

	private void saveReminders(ContentResolver cr, long eventId,
			ArrayList<Integer> reminderMinutes,
			ArrayList<Integer> originalMinutes) {
		// If the reminders have not changed, then don't update the database
		if (reminderMinutes.equals(originalMinutes)) {
			return;
		}
		// Delete all the existing reminders for this event
		String where = IReminders.EVENT_ID + "=?";
		String[] args = new String[] { Long.toString(eventId) };
		cr.delete(Uri.parse(REMINDERS_URI), where, args); // part one : Delete
		// All Reminders

		// Update the "hasAlarm" field for the event
		ContentValues values = new ContentValues();
		int len = reminderMinutes.size();
		values.put(IEvents.HAS_ALARM, (len > 0) ? 1 : 0);
		Uri uri = ContentUris.withAppendedId(Uri.parse(EVENTS_URI), eventId);
		cr.update(uri, values, null /* where */, null /* selection args */); // part

		// Insert the new reminders, if any
		for (int i = 0; i < len; i++) {
			int minutes = reminderMinutes.get(i);
			values.clear();
			values.put(IReminders.MINUTES, minutes);
			values.put(IReminders.METHOD, IReminders.METHOD_ALERT);
			values.put(IReminders.EVENT_ID, eventId);
			cr.insert(Uri.parse(REMINDERS_URI), values);
		}
	}

	// ========================�������=========================
	public int ImportEvent(CalendarEvents event) {
		int result = -1;
		ContentResolver cr = mContext.getContentResolver();
		ContentValues values = GetContentValuesFromImportEvent(event);// �ֶΰ�
		Uri uri = null;
		Uri insertEventsURL = Uri.parse(EVENTS_URI);
		uri = cr.insert(insertEventsURL, values); // ����event���ű�
		long eventId = ContentUris.parseId(uri);
		result = (int) eventId;
		if (uri != null) {
			Uri remindersUri = Uri.parse(REMINDERS_URI);
			ArrayList<Integer> mOriginalMinutes = new ArrayList<Integer>();
			String where = String.format(REMINDERS_WHERE, eventId);
			Cursor reminderCursor = cr.query(remindersUri, null, where, null,
					null);
			if (null != reminderCursor) {
				try {
					while (reminderCursor.moveToNext()) {
						int minutes = reminderCursor
								.getInt(REMINDERS_INDEX_MINUTES);
						mOriginalMinutes.add(minutes);
					}
				} finally {
					reminderCursor.close();
				}
			}
			ArrayList<Integer> reminderMinutes = new ArrayList<Integer>();
			List<Reminders> remindersList = event.getChildReminders();
			for (Reminders x : remindersList) {
				reminderMinutes.add(x.getMinutes());
			}
			saveReminders(cr, eventId, reminderMinutes, mOriginalMinutes);
		}
		return result;
	}

	// ����ר���ǵ�������õģ���GetContentValuesFromUI������ֶ���ݶ���
	private ContentValues GetContentValuesFromImportEvent(
			CalendarEvents editEvent) {
		String title = editEvent.getTitle();
		boolean isAllDay = editEvent.getAllDay() == 0 ? false : true;
		String location = editEvent.getEventLocation();
		String description = editEvent.getDescription();
		ContentValues values = new ContentValues();
		String timezone = null;
		long startMillis = editEvent.getDtstart();
		long end = 0;
		if (editEvent.getDtend() != null) {
			end = editEvent.getDtend();
			values.put(IEvents.DTEND, end); // dtend
		}
		int calendarId = editEvent.getCalendarId();
		timezone = editEvent.getTimeZone();
		if (TextUtils.isEmpty(timezone)) {
			timezone = TimeZone.getDefault().getID();
		}
		String originalEvent = editEvent.getOriginalEvent();
		long originalInstanceTime = editEvent.getOriginalInstanceTime();
		long lastDate = editEvent.getLastDate();

		int eventStatus = editEvent.getEventStatus();
		String mNewRrule = editEvent.getRrule();
		values.put(IEvents.CALENDAR_ID, calendarId); // calendar_id
		values.put(IEvents.EVENT_TIMEZONE, timezone); // eventTimezone
		values.put(IEvents.TITLE, title); // title
		values.put(IEvents.ALL_DAY, isAllDay ? 1 : 0); // allDay
		values.put(IEvents.DTSTART, startMillis); // dtstart
		values.put(IEvents.DESCRIPTION, description); // description
		values.put(IEvents.EVENT_LOCATION, location); // eventLocation
		values.put(IEvents.TRANSPARENCY, 0); // default value
		int visibility = 0; // default value
		values.put(IEvents.VISIBILITY, visibility);
		if (eventStatus == 2)
			values.put(IEvents.EVENTSTATUS, 2); // �������4����Ƿ�ɾ���event
		if (mNewRrule != null && mNewRrule != "") {
			values.put(IEvents.RRULE, mNewRrule); // repeat
			String duration;
			if (isAllDay) {
				long days = (end - startMillis + DAY_IN_MILLIS - 1)
						/ DAY_IN_MILLIS;
				duration = "P" + days + "D";
			} else {
				long seconds = (end - startMillis) / SECOND_IN_MILLIS;
				duration = "P" + seconds + "S";
			}
			values.put(IEvents.DURATION, duration);
		}
		if (originalEvent != null && originalEvent != "")
			values.put(IEvents.ORIGINAL_EVENT, originalEvent);
		if (originalInstanceTime > 0)
			values.put(IEvents.ORIGINAL_INSTANCE_TIME, originalInstanceTime);
		if (lastDate > 0)
			values.put(IEvents.LASTDATE, lastDate);
		return values;
	}

	// =======================�������===========================
	public EntityCollection<CalendarInfo> QueryAllCalendar() {
		EntityCollection<CalendarInfo> result = null;
		Cursor cal_cursor = mContext.getContentResolver().query(
				Uri.parse(CALENDAR_URI), null, null, null, null);
		// �α��Ѿ��ر�
		if (null != cal_cursor) {
			if (cal_cursor.moveToFirst()) {
				try {
					result = new EntityCollection<CalendarInfo>("CalendarInfo");
					CalendarInfo calendarinfo = null;

					while (!cal_cursor.isAfterLast()) {
						calendarinfo = new CalendarInfo();
						int calendarId = cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(CALENDAR_ID));
						String normalwhere = EVENT_CALENDAR_ID + "="
								+ calendarId;
						// All Events Cusor
						Cursor event_cursor = mContext.getContentResolver()
								.query(Uri.parse(EVENTS_URI), null,
										normalwhere, null, null);
						// All Events
						EntityCollection<CalendarEvents> calendarEventList = GetALLEventsFromCursor(
								event_cursor, calendarId);
						if (null != event_cursor) {
							event_cursor.close();
						}
						calendarinfo.setId(calendarId);

						calendarinfo.setName(cal_cursor.getString(cal_cursor
								.getColumnIndexOrThrow(NAME)));

						calendarinfo.setDisplayName(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(DISPLAYNAME)));

						calendarinfo.setHidden(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(HIDDEN)));

						calendarinfo.setColor(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(COLOR)));

						calendarinfo.setSelected(cal_cursor.getInt(cal_cursor
								.getColumnIndexOrThrow(SELECTED)));

						calendarinfo.setTimezone(cal_cursor
								.getString(cal_cursor
										.getColumnIndexOrThrow(TIMEZONE)));
						if (calendarEventList != null
								&& calendarEventList.size() > 0) {
							calendarinfo.setEvents(calendarEventList);
						}
						// ���
						result.add(calendarinfo);
						cal_cursor.moveToNext();
					} // end while(calendar)
				} finally {
					if (null != cal_cursor) {
						cal_cursor.close();
					}
				}
			} else {
				if (null != cal_cursor) {
					cal_cursor.close();
				}
			}
		}
		return result;
	}

	private EntityCollection<CalendarEvents> GetALLEventsFromCursor(
			Cursor event_cursor, int calendarId) {
		EntityCollection<CalendarEvents> eventList = new EntityCollection<CalendarEvents>(
				"CalendarEvents");
		// �α��Ѿ��ر�
		if (null != event_cursor) {
			if (event_cursor.moveToFirst()) {
				try {
					while (!event_cursor.isAfterLast()) {
						CalendarEvents event = new CalendarEvents();

						event.setId(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents._ID)));
						LogCenter.error("info", "_id" + event.getId());

						event.setTitle(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.TITLE)));

						LogCenter.error("info", "title" + event.getTitle());

						event
								.setEventLocation(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_LOCATION)));

						LogCenter.error("info", "eventLocation"
								+ event.getEventLocation());

						event
								.setDescription(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.DESCRIPTION)));

						LogCenter.error("info", "description" + event.getDescription());

						event.setDtstart(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTSTART)));

						LogCenter.error("info", "dtstart" + event.getDtstart());

						event.setDtend(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.DTEND)));

						LogCenter.error("info", "dtend" + event.getDtend());

						event.setAllDay(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.ALL_DAY)));

						LogCenter.error("info", "allDay" + event.getAllDay());

						event.setHasAlarm(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.HAS_ALARM)));

						LogCenter.error("info", "hasAralm" + event.getHasAlarm());

						event.setRrule(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.RRULE)));

						LogCenter.error("info", "rrule" + event.getRrule());

						event.setCalendarId(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.CALENDAR_ID)));

						LogCenter.error("info", "calendar id" + event.getCalendarId());

						event.setEventStatus(event_cursor.getInt(event_cursor
								.getColumnIndexOrThrow(IEvents.EVENTSTATUS)));

						LogCenter.error("info", "eventStatus" + event.getEventStatus());

						event
								.setTimeZone(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.EVENT_TIMEZONE)));

						LogCenter.error("info", "timezone" + event.getTimeZone());

						event.setDuration(event_cursor.getString(event_cursor
								.getColumnIndexOrThrow(IEvents.DURATION)));

						LogCenter.error("info", "duration" + event.getDuration());

						event
								.setOriginalEvent(event_cursor
										.getString(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_EVENT)));

						LogCenter.error("info", "OriginalEvent"
								+ event.getOriginalEvent());

						event
								.setOriginalInstanceTime(event_cursor
										.getLong(event_cursor
												.getColumnIndexOrThrow(IEvents.ORIGINAL_INSTANCE_TIME)));

						LogCenter.error("info", "OriginalInstanceTime"
								+ event.getOriginalInstanceTime());

						event.setLastDate(event_cursor.getLong(event_cursor
								.getColumnIndexOrThrow(IEvents.LASTDATE)));

						LogCenter.error("info", "lastdate" + event.getLastDate());

						// get reminders
						EntityCollection<Reminders> reminderList = GetReminderList(event
								.getId());

						if (reminderList != null && reminderList.size() > 0) {
							LogCenter.error("info", " event is"
									+ String.valueOf(event.getId())
									+ " add reming  rrrrrrr");
							event.setChildReminders(reminderList);
						}

						eventList.add(event);
						LogCenter.error("info", "Calendar add event is"
								+ String.valueOf(event.getId()));

						event_cursor.moveToNext();
					}
				} finally {
					if (null != event_cursor) {
						event_cursor.close();
					}
				}
			} else {
				if (null != event_cursor) {
					event_cursor.close();
				}
			}
		}
		return eventList;
	}
}
