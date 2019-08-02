package daemon.model.calendar;

import java.util.List;


import daemon.logger.LogCenter;
import daemon.model.EntityCollection;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;

public class CalendarEvents implements IBaseEntity {
	private Integer id;
	private String title;
	private String eventLocation;
	private String description;
	private Long dtstart;
	private Long dtend;
	private Integer allDay;
	private Integer hasAlarm;
	private String rrule;
	private Integer calendarId;
	private Integer eventStatus = 0;
	private String timeZone;
	private String duration;
	private String originalEvent;
	private Long originalInstanceTime;
	private Long lastDate;
	private String sync_id;
	private Integer lephone_Category;   //j���¼���������
	private String phone_Category;
	private String exdate;
	private EntityCollection<Reminders> childReminders = new EntityCollection<Reminders>(
			"Reminders");
	private Long instaceBegin; // instance begin ����ڷ��ػ�4��ʱ����д����Ҫ���޸Ĳ����õ�
	private Long instaceEnd;
	private EntityCollection<CalendarEvents> cancelEventList = new EntityCollection<CalendarEvents>(
			"CalendarEvents"); // �����event�� �б��Ϊȡ��ģ�eventStatus =

	// 2����һ����ɾ������������4
	
	public String getExdate() {
		return exdate;
	}

	public void setExdate(String exdate) {
		this.exdate = exdate;
	}	
	
	public String getPhone_Category() {
		return phone_Category;
	}

	public void setPhone_Category(String phone_Category) {
		this.phone_Category = phone_Category;
	}	
	
	public Integer getlephone_Category() {
		return lephone_Category;
	}

	public void setLephone_Category(Integer lephone_Category) {
		this.lephone_Category = lephone_Category;
	}
	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public void setSync_id(String sync_id) {
		this.sync_id = sync_id;
	}

	public String getSync_id() {
		return sync_id;
	}
	
	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getEventLocation() {
		return eventLocation;
	}

	public void setEventLocation(String eventLocation) {
		this.eventLocation = eventLocation;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public Long getDtstart() {
		return dtstart;
	}

	public void setDtstart(Long dtstart) {
		this.dtstart = dtstart;
	}

	public Long getDtend() {
		return dtend;
	}

	public void setDtend(Long dtend) {
		this.dtend = dtend;
	}

	public Integer getAllDay() {
		return allDay;
	}

	public void setAllDay(Integer allDay) {
		this.allDay = allDay;
	}

	public Integer getHasAlarm() {
		return hasAlarm;
	}

	public void setHasAlarm(Integer hasAlarm) {
		this.hasAlarm = hasAlarm;
	}

	public String getRrule() {
		return rrule;
	}

	public void setRrule(String rrule) {
		this.rrule = rrule;
	}

	public void setTimeZone(String timeZone) {
		this.timeZone = timeZone;
	}

	public String getTimeZone() {
		return timeZone;
	}

	public void setChildReminders(EntityCollection<Reminders> childReminders) {
		this.childReminders = childReminders;
	}

	public EntityCollection<Reminders> getChildReminders() {
		return childReminders;
	}

	public void setInstaceBegin(Long instaceBegin) {
		this.instaceBegin = instaceBegin;
	}

	public Long getInstaceBegin() {
		return instaceBegin;
	}

	public void setInstaceEnd(Long instaceEnd) {
		this.instaceEnd = instaceEnd;
	}

	public Long getInstaceEnd() {
		return instaceEnd;
	}

	public void setCancelEventList(
			EntityCollection<CalendarEvents> cancelEventList) {
		this.cancelEventList = cancelEventList;
	}

	public List<CalendarEvents> getCancelEventList() {
		return cancelEventList;
	}

	public void setCalendarId(int calendarId) {
		this.calendarId = calendarId;
	}

	public int getCalendarId() {
		return calendarId;
	}

	public void setEventStatus(int eventStatus) {
		this.eventStatus = eventStatus;
	}

	public int getEventStatus() {
		return eventStatus;
	}

	public void setDuration(String duration) {
		this.duration = duration;
	}

	public String getDuration() {
		return duration;
	}

	public void setOriginalEvent(String originalEvent) {
		this.originalEvent = originalEvent;
	}

	public String getOriginalEvent() {
		return originalEvent;
	}

	public void setOriginalInstanceTime(Long originalInstanceTime) {
		this.originalInstanceTime = originalInstanceTime;
	}

	public Long getOriginalInstanceTime() {
		return originalInstanceTime;
	}

	public void setLastDate(Long lastDate) {
		this.lastDate = lastDate;
	}

	public Long getLastDate() {
		return lastDate;
	}

	@Override
	public Integer GetAction() {
		return 0;
	}

	private Boolean IsNull(ByteReader reader) {
		Boolean result = false;
		char temp = reader.ReadChar();
		if (temp == 0) {
			result = true;
		}
		return result;
	}

	@Override
	public void Read(ByteReader reader) {
		calendarId = reader.ReadInt32();
		id = reader.ReadInt32();
		title = reader.ReadString();
		eventLocation = reader.ReadString();
		description = reader.ReadString();
		dtstart = reader.ReadInt64();
		dtend = reader.ReadInt64();
		allDay = reader.ReadInt32();
		hasAlarm = reader.ReadInt32();
		rrule = reader.ReadString();
		if (!IsNull(reader)) {
			eventStatus = reader.ReadInt32();
		}
		timeZone = reader.ReadString();
		duration = reader.ReadString();
		originalEvent = reader.ReadString();
		originalInstanceTime = reader.ReadInt64();
		lastDate = reader.ReadInt64();
		instaceBegin = reader.ReadInt64();
		instaceEnd = reader.ReadInt64();
		lephone_Category =reader.ReadInt32();    //j��lephone�¼�����
		phone_Category =reader.ReadString();
		exdate=reader.ReadString();
		if (!IsNull(reader)) {
			int count = reader.ReadInt32();
			for (int i = 0; i < count; i++) {
				try {
					Reminders entity = new Reminders();
					entity.Read(reader);
					childReminders.add(entity);
				} catch (Exception e) {
					LogCenter.error(e.getMessage(), "");
				}
			}

		}

	}

	@Override
	public void Write(ByteWriter writer) {
		if (calendarId == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(calendarId);
		}
		if (id == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(id);
		}

		if (title == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(title);
		}

		if (eventLocation == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(eventLocation);
		}

		if (description == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(description);
		}

		if (dtstart == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(dtstart);
		}

		if (dtend == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(dtend);
		}

		if (allDay == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(allDay);
		}

		if (hasAlarm == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(hasAlarm);
		}

		if (rrule == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(rrule);
		}

		if (eventStatus == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(eventStatus);
		}

		if (timeZone == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(timeZone);
		}

		if (duration == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(duration);
		}

		if (originalEvent == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(originalEvent);
		}

		if (originalInstanceTime == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(originalInstanceTime);
		}

		if (lastDate == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(lastDate);
		}
		if (instaceBegin == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(instaceBegin);
		}

		if (instaceEnd == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(instaceEnd);
		}

		if (sync_id == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(sync_id);
		}

		//j��lephone
		if (lephone_Category == null) {               
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(lephone_Category);
		}
				
		//j��lephone
		if (phone_Category == null) {               
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(phone_Category);
		}
				
		//j��lephone
		if (exdate == null) {               
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(exdate);
		}
		
		if (childReminders == null || childReminders.size() < 1) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			childReminders.Write(writer);
		}

		if (cancelEventList == null || cancelEventList.size() < 1) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			cancelEventList.Write(writer);
		}

	}

}
