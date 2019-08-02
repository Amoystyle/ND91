package daemon.provider.business;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.text.style.BulletSpan;


import daemon.common.Device;
import daemon.model.EntityCollection;
import daemon.model.calendar.CalendarEvents;
import daemon.model.calendar.CalendarInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.provider.calendar.CalendarManager;
import daemon.provider.calendar.CalendarManager40;
import daemon.provider.calendar.CalendarManagerBase;


public class CalendarProvider implements IProvider {

	private Context context;
	private CalendarManagerBase provider = null;

	public CalendarProvider(Context context) {
		this.context = context;
		if(Device.getIntSdkVersion()>=14){
			provider = new CalendarManager40(context);
		}else{
			provider = new CalendarManager(context);
		}
		
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		byte action = reader.ReadByte();

		if (action == 99) { // 99 query
			String tableName = reader.ReadString();
			if (tableName.equals("Query")) {
				QueryCalendar(reader, writer);
			} else if (tableName.equals("Export")) {
				Export(reader, writer);
			} else if (tableName.equals("SimpleQuery")) {
				SimpleQueryCalendar(reader, writer);
			} else if (tableName.equals("QureyOnlyCalenar")) {
				QueryOnlyCalenars(reader, writer);
			} else if (tableName.equals("QureyEventsByCalenarId")) {
				QureyEventsByCalenarId(reader, writer);
			}
		} else if (action == 1) // 1 add
		{
			Add(reader, writer);
		} else if (action == 2) { // 2 delte
			deleteEvent(reader, writer);
		} else if (action == 3) { // 3 edit
			Edit(reader, writer);
		} else if (action == 4) { // 4 import
			Import(reader, writer);
		} else if (action == 5) { // 5 get calendar count		
			QueryCalendarCount(reader, writer);
		} else if (action == 6) {
			queryDbInfo(writer);
		}else if(action == 7){
			provider.QueryCalendarAndEvents(writer);
		}
	}
	
	private void queryDbInfo(ByteWriter writer) {
		Cursor cur = null;
		Uri uri = Uri.parse("content://com.android.calendar/events");
		cur = context.getContentResolver().query(uri, null, null, null, null);
		if (cur != null) {
			try {
                int columnCnt = cur.getColumnCount();
                writer.Write(columnCnt);
                for(int i=0;i<columnCnt;i++){
                	String columnName = cur.getColumnName(i);
                	writer.Write(columnName);
                }
                
                if(cur.moveToFirst()){
                	int rowCnt = cur.getCount();
                	writer.Write(rowCnt);                	
                	
                	while(!cur.isAfterLast()){
                		for(int i=0;i<columnCnt;i++){
                			String value = "";
                			try{
                				value = cur.getString(i);
                			}catch(Exception e){
                				value = "err";
                			}
                			
                			writer.Write(value);                			
                		}
                		
                		cur.moveToNext();
                	}
                }
			} catch (Exception e) {

			} finally {
				cur.close();
			}
		}else{
			writer.Write(0);
		}
		
	}

	private void QueryCalendar(ByteReader reader, ByteWriter writer) {

		EntityCollection<CalendarInfo> infos = new EntityCollection<CalendarInfo>(
				"CalendarInfo");
		infos = provider.QueryCalendar();
		infos.Write(writer);
	}

	private void Add(ByteReader reader, ByteWriter writer) {
		CalendarEvents addEvent = new CalendarEvents();
		addEvent.Read(reader);
		int addResultId = provider.AddEvent(addEvent);
		writer.Write(addResultId);
	}

	private void deleteEvent(ByteReader reader, ByteWriter writer) {
		long start = reader.ReadInt64();
		long end = reader.ReadInt64();
		int eventId = reader.ReadInt32();
		int deleteSelect = reader.ReadInt32();
		Boolean repeat = reader.ReadBoolean();
		String phoneType = reader.ReadString();
		String exdate = reader.ReadString();
		int delteResult = provider.DeleteEvent(start, end, eventId,
				deleteSelect, repeat, phoneType, exdate);
		writer.Write(delteResult);
	}

	private void Edit(ByteReader reader, ByteWriter writer) {
		CalendarEvents editEvent = new CalendarEvents();
		editEvent.Read(reader);
		int modifySelect = reader.ReadInt32();
		int editResultId = provider.EditEvent(editEvent, modifySelect);
		writer.Write(editResultId);
	}

	private void Export(ByteReader reader, ByteWriter writer) {
		EntityCollection<CalendarInfo> infos = new EntityCollection<CalendarInfo>(
				"CalendarInfo");
		infos = provider.QueryAllCalendar();
		infos.Write(writer);
	}

	private void Import(ByteReader reader, ByteWriter writer) {
		CalendarEvents addEvent = new CalendarEvents();
		addEvent.Read(reader);
		int addResultId = provider.ImportEvent(addEvent);
		writer.Write(addResultId);
	}

	private void SimpleQueryCalendar(ByteReader reader, ByteWriter writer) {
		EntityCollection<CalendarInfo> infos = new EntityCollection<CalendarInfo>(
				"CalendarInfo");
		infos = provider.QuerySimpleCalendar();
		if (infos == null) {
			writer.WriteString("FAIL");
		} else {
			infos.Write(writer);
		}
	}

	private void QueryCalendarCount(ByteReader reader, ByteWriter writer) {
		int calendarCount = provider.QueryCalendarCount();
		writer.Write(calendarCount);
	}

	// 2010.1.16
	private void QueryOnlyCalenars(ByteReader reader, ByteWriter writer) {
		EntityCollection<CalendarInfo> infos = new EntityCollection<CalendarInfo>(
				"CalendarInfo");
		infos = provider.QueryCalendarTable();
		if (infos == null) {
			writer.WriteString("FAIL");
		} else {
			infos.Write(writer);
		}
	}

	private void QureyEventsByCalenarId(ByteReader reader, ByteWriter writer) {
		int columnCount = reader.ReadInt32();
		String[] columns = null;
		if (columnCount > 0) {
			columns = new String[columnCount];
			for (int i = 0; i < columnCount; i++) {
				columns[i] = reader.ReadString();
			}
		}
		String where_calendarId = reader.ReadString();
		String order = reader.ReadString();
		EntityCollection<CalendarEvents> eventList = new EntityCollection<CalendarEvents>(
				"CalendarEvents");
		int calendarId = Integer.valueOf(where_calendarId).intValue();
		eventList = provider.QueryEventsByCalendarId(calendarId);
		if (eventList == null) {
			writer.WriteString("FAIL");
		} else {
			eventList.Write(writer);
		}
	}

	@Override
	public int getBusiness() {
		return 6;
	}

}
