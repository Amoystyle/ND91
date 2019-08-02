package daemon.model.calendar;

public interface IEvents {
	String _ID = "_id";
	String TITLE = "title"; // 1 do not remove; used in DeleteEventHelper
	String RRULE = "rrule"; // 2 do not remove; used in DeleteEventHelper
	String ALL_DAY = "allDay"; // 3 do not remove; used in DeleteEventHelper
	String CALENDAR_ID = "calendar_id"; // 4 do not remove; used in
	// DeleteEventHelper
	String DTSTART = "dtstart"; // 5 do not remove; used in DeleteEventHelper
	String _SYNC_ID = "_sync_id"; // 6 do not remove; used in DeleteEventHelper
	String EVENT_TIMEZONE = "eventTimezone"; // 7 do not remove; used in
	// DeleteEventHelper
	String DESCRIPTION = "description"; // 8
	String EVENT_LOCATION = "eventLocation"; // 9
	String HAS_ALARM = "hasAlarm"; // 10
	String ORIGINAL_EVENT = "originalEvent";
	String DTEND = "dtend";
	String STATUS = "eventStatus";
	String ORIGINAL_INSTANCE_TIME = "originalInstanceTime";
	String LASTDATE = "lastDate";
	String TRANSPARENCY = "transparency";
	String VISIBILITY = "visibility";
	String DURATION = "duration";
	String EVENTSTATUS = "eventStatus";
	String ORIGINALEVENT = "originalEvent";
	String ORIGINALINSTANCETIME = "originalInstanceTime";
	String LEPHONE_CATEGORY="category";                     // Lephone event type
	String EXDATE="exdate";
}
