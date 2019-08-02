#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarStreamHelper.h"
#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"
#include "Module/Calendar/CalendarUtility.h"
#include "Module/Calendar/ReminderData.h"

CalendarStreamHelper::CalendarStreamHelper(void)
{
}

CalendarStreamHelper::~CalendarStreamHelper(void)
{
}

void ND91Assistant::CalendarStreamHelper::ParseCalendar( BinaryBufferReader& reader, CalendarData* pcal, bool IsNew /*= false*/ )
{
	RETURN_IF(!pcal);
	if (IsNew || !IsNull(reader))//  如果非new，需要读取一个字节的标志位，标志位为1则读取内容
		reader.ReadInt32(pcal->_rowId);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pcal->_syncAccount);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pcal->_name);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pcal->_displayName);
	int hide = 0;
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(hide);
	pcal->_hidden = hide == 1?true:false;
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(pcal->_color);
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(pcal->_selected);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pcal->_timeZone);
	if (IsNew)
	{
		int level = 0;
		reader.ReadInt32(level);
		switch(level)
		{
			case CalendarData::FREEBUSY_ACCESS:
			case CalendarData::READ_ACCESS:
			case CalendarData::RESPOND_ACCESS:
			case CalendarData::OVERRIDE_ACCESS:
			case CalendarData::CONTRIBUTOR_ACCESS:
			case CalendarData::EDITOR_ACCESS:
			case CalendarData::OWNER_ACCESS:
			case CalendarData::ROOT_ACCESS:
				pcal->_accessLevel = (CalendarData::CalendarAccessLevel)level;
				break;
			default:
				pcal->_accessLevel = CalendarData::NO_ACCESS;
				break;	
		}
		return;
	}

	if(!IsNull(reader))
	{
		int count = 0;
		reader.ReadInt32(count);
		for (int i = 0; i < count; i++)
		{
			CalendarEventData* pEvent = new CalendarEventData();
			ParseCalendarEvent(reader, pEvent);
			pcal->InsertCalendarEvent(pEvent);
		}
	}
}

void ND91Assistant::CalendarStreamHelper::ParseCalendarEvent( BinaryBufferReader& reader, CalendarEventData* pEvent, bool IsNew /*= false*/ )
{
	RETURN_IF(!pEvent);
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(pEvent->_calendarId);
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(pEvent->_rowid);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_summary);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_location);
	if (IsNew || !IsNull(reader))
	{
		reader.ReadStringFromUTF8(pEvent->_description);
		pEvent->_description = CStrOperation::ReplaceW(CStrOperation::ReplaceW(pEvent->_description, L"\r\n", L"\n"),L"\n", L"\r\n");
	}
	if (IsNew || !IsNull(reader))
	{
		reader.ReadInt64(pEvent->_start);
		pEvent->_start = pEvent->_start/1000;
	}
	if (IsNew || !IsNull(reader))
	{
		reader.ReadInt64(pEvent->_end);
		pEvent->_end = pEvent->_end/1000;
	}
	int allday = 0;
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(allday);
	pEvent->_allDay = (allday == 1)?true:false;
	int hasAlert =0;
	if (IsNew || !IsNull(reader))
		reader.ReadInt32(hasAlert);
	pEvent->_alert = (hasAlert==1)?true:false;
	string rruleStr;
	if (IsNew || !IsNull(reader))
		reader.ReadString(rruleStr);

	if(!rruleStr.empty())
	{
		CalendarUtility::SplitRepeatrule(pEvent->_repeatRlue, rruleStr);
		if(pEvent->_repeatRlue._until.empty())
		{
			pEvent->_repeatForever = true;
		}
		else
		{
			pEvent->_repeatForever = false;
			pEvent->_endRepeatDate = CDealTime::StringToTime(pEvent->_repeatRlue._until);
		}
		pEvent->_repeatType = CalendarUtility::SelectRepeat(pEvent->_repeatRlue);
	}

	if (IsNew || !IsNull(reader))
		reader.ReadInt32(pEvent->_eventStatus);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_timeZone);
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_duration);
	int distance = 0;
	if (!pEvent->_duration.empty())
		distance = _wtoi(pEvent->_duration.substr(1, pEvent->_duration.length()-2).c_str());
	if(pEvent->_allDay)
	{
		CDealTime time(pEvent->_start);
		CDealTime newtime(time.GetYear(),time.GetMonth(),time.GetDay());
		pEvent->_start = (__int64)(newtime.GetTime());

		if(distance > 0)
			pEvent->_end = pEvent->_start +distance*24*3600;
		else
		{
			CDealTime endTime(pEvent->_end);
			CDealTime newEndTime(endTime.GetYear(),endTime.GetMonth(),endTime.GetDay());
			pEvent->_end = newEndTime.GetTime();
		}
	}
	else if(distance > 0)
	{
		pEvent->_end = pEvent->_start +distance;
	}
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_originalEvent);
	if (IsNew || !IsNull(reader))
	{
		reader.ReadInt64(pEvent->_originalInstanceTime);
		pEvent->_originalInstanceTime = pEvent->_originalInstanceTime/1000;
	}
	if (IsNew || !IsNull(reader))
	{
		reader.ReadInt64(pEvent->_lastDate);
		pEvent->_lastDate = pEvent->_lastDate/1000;
	}
	if(!IsNew)
	{
		if (!IsNull(reader))
		{
			reader.ReadInt64(pEvent->_instaceBegin);
			pEvent->_instaceBegin = pEvent->_instaceBegin/1000;
		}
		if (!IsNull(reader))
		{
			reader.ReadInt64(pEvent->_instaceEnd);
			pEvent->_instaceEnd = pEvent->_instaceBegin/1000;
		}
	}
	if (IsNew || !IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_syncId);
	if (IsNew)
		return;
	if (!IsNull(reader))
		reader.ReadInt32(pEvent->_lephoneCategory);
	if (!IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_phoneCategory);
	if (!IsNull(reader))
		reader.ReadStringFromUTF8(pEvent->_exdate);
	if (!IsNull(reader))
	{
		int count = 0;
		reader.ReadInt32(count);
		for (int i = 0; i < count; i++)
		{
			//读reminder
			ReminderData* pReminder = new ReminderData();
			ParseReminder(reader,pReminder);
			pEvent->_reminders.push_back(pReminder);
		}
	}
	if (!IsNull(reader))
	{
		int cancelCount = 0;
		for(int j = 0; j < cancelCount; j++)
		{
			//
			CalendarEventData* pCancelEvent = new CalendarEventData();
			ParseCalendarEvent(reader, pCancelEvent);
			pEvent->_cancelEventList[pCancelEvent->_rowid] = pCancelEvent;
		}
	}
}

void ND91Assistant::CalendarStreamHelper::CalendarEventToStream( BinaryBufferWriter& writer, CalendarEventData* pEvent )
{
	RETURN_IF(!pEvent);
	writer.WriteInt(pEvent->_calendarId);
	writer.WriteInt(pEvent->_rowid);
	writer.WriteStringAsUTF8(pEvent->_summary);
	writer.WriteStringAsUTF8(pEvent->_location);
	writer.WriteStringAsUTF8(CStrOperation::ReplaceW(pEvent->_description,L"\r\n", L"\n"));
	__int64 starttime = (pEvent->_allDay?pEvent->_start+8*3600:pEvent->_start);
	__int64 endtime =  pEvent->_allDay?pEvent->_end+8*3600:pEvent->_end;

	writer.WriteInt64(starttime*1000);
	writer.WriteInt64(endtime*1000);
	writer.WriteInt(pEvent->_allDay?1:0);
	writer.WriteInt(pEvent->_alert?1:0);
	writer.WriteString(CalendarUtility::RepeatruleToString(pEvent->_repeatRlue));  //rule;
	if (pEvent->_eventStatus != 2)
		writer.WriteChar(0);
	else
	{
		writer.WriteChar(1);
		writer.WriteInt(pEvent->_eventStatus);
	}
	writer.WriteStringAsUTF8(pEvent->_timeZone);
	writer.WriteStringAsUTF8(pEvent->_duration);
	writer.WriteStringAsUTF8(pEvent->_originalEvent);
	writer.WriteInt64(pEvent->_originalInstanceTime*1000);
	writer.WriteInt64(pEvent->_lastDate*1000);

	writer.WriteInt64(pEvent->_instaceBegin*1000);
	writer.WriteInt64(pEvent->_instaceEnd*1000);
	writer.WriteInt(pEvent->_lephoneCategory);
	writer.WriteStringAsUTF8(pEvent->_phoneCategory);
	writer.WriteStringAsUTF8(pEvent->_exdate);

	if(pEvent->_reminders.size() > 0)
	{
		writer.WriteChar(1);
		writer.WriteInt(pEvent->_reminders.size());
		for (int i = pEvent->_reminders.size() -1; i >= 0; i--)
		{
			ReminderToStream(writer, pEvent->_reminders.at(i));
		}
	}
	else
	{
		writer.WriteChar(0);
	}

}

void ND91Assistant::CalendarStreamHelper::ParseReminder( BinaryBufferReader& reader, ReminderData* pReminder )
{
	RETURN_IF(!pReminder);
	reader.ReadInt32(pReminder->_id);
	reader.ReadInt32(pReminder->_eventId);
	reader.ReadInt32(pReminder->_minutes);
	reader.ReadInt32(pReminder->_method);
}

void ND91Assistant::CalendarStreamHelper::ReminderToStream( BinaryBufferWriter& writer, ReminderData* pReminder )
{
	RETURN_IF(!pReminder);
	writer.WriteInt(pReminder->_id);
	writer.WriteInt(pReminder->_eventId);
	writer.WriteInt(pReminder->_minutes);
	writer.WriteInt(pReminder->_method);
}

bool ND91Assistant::CalendarStreamHelper::IsNull( BinaryBufferReader& reader )
{
	char a=0;
	reader.ReadChar(a);
	return a==0;
}

#endif