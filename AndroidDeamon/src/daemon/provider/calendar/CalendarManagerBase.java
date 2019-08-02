package daemon.provider.calendar;


import daemon.model.EntityCollection;
import daemon.model.calendar.CalendarEvents;
import daemon.model.calendar.CalendarInfo;
import daemon.protocol.ByteWriter;

public abstract class CalendarManagerBase {



	public abstract boolean HasDeletedColumn();


	public abstract EntityCollection<CalendarInfo> QueryCalendar();
	public abstract void QueryCalendarAndEvents(ByteWriter writer);
		
	public abstract EntityCollection<CalendarInfo> QuerySimpleCalendar();


	public abstract EntityCollection<CalendarInfo> QueryCalendarTable() ;

	// 2010
	public abstract EntityCollection<CalendarEvents> QueryEventsByCalendarId(
			int calendarId);

	public abstract int QueryCalendarCount() ;

	public abstract int AddEvent(CalendarEvents addEvent) ;

	public abstract int EditEvent(CalendarEvents event, int modifySelect);


	public abstract int DeleteEvent(long start, long end, int eventId, int deleteSelect,
			Boolean repeat, String phoneType, String exdate);

	// Delete Event
	public abstract boolean deleteEventsById(CalendarEvents event) ;

	public abstract String ConvertRrul(long timeStamp);

	// ========================�������=========================
	public abstract int ImportEvent(CalendarEvents event) ;

	

	// =======================�������===========================
	public abstract EntityCollection<CalendarInfo> QueryAllCalendar() ;
	
	public CalendarManagerBase(){
		
	}

	
}
