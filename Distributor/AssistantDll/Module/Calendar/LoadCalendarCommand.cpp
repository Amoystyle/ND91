#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/LoadCalendarCommand.h"
#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"
#include "Module/Calendar/CalendarStreamHelper.h"
#include "Module/Calendar/CalendarLibrary.h"
#include "Module/AndroidDefine.h"

LoadCalendarCommand::LoadCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary)
:BaseCommand(pDevice)
{
	_pLibrary = pLibrary;
	_nAndroidChannelID = CALENDAR;
	_nAndroidCommandID = 7;
}

LoadCalendarCommand::~LoadCalendarCommand(void)
{
}

void ND91Assistant::LoadCalendarCommand::Execute()
{
	DefaultExecute();
}

std::string ND91Assistant::LoadCalendarCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(_nAndroidCommandID);
	return bufwriter.GetBuffer();
}

void ND91Assistant::LoadCalendarCommand::SetAndroidReturnData( std::string data )
{
	map<int, CalendarData*>  calenserList;
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	//  Ω‚ŒˆCalendarData
	int calCount = 0;
	reader.ReadInt32(calCount);
	for (int i = 0; i < calCount; i++)
	{
		CalendarData* cl = new CalendarData();
		CalendarStreamHelper::ParseCalendar(reader, cl, true);
		_pLibrary->InsertCalendar(cl);
	}
	// Ω‚Œˆ CalendarEventData
	map<int, CalendarEventData*>  eventlist;
	map<wstring, CalendarEventData*> syncEvents;
	int eventcount = 0;
	reader.ReadInt32(eventcount);
	for(int j = 0; j < eventcount; j++)
	{
		CalendarEventData* pevent = new CalendarEventData();
		CalendarStreamHelper::ParseCalendarEvent(reader, pevent, true);
		eventlist[pevent->_rowid] = pevent;
		if(!pevent->_syncId.empty())
			syncEvents[pevent->_syncId] = pevent;

		if(!pevent->_originalEvent.empty())
		{
			if(pevent->_eventStatus == 2)
			{
				map<wstring, CalendarEventData*>::iterator it = syncEvents.find(pevent->_originalEvent);
				if (it != syncEvents.end())
					it->second->_cancelEventList[pevent->_rowid]=pevent;
			}
			else
			{
				_pLibrary->InsertCalendarEvent(pevent);
				map<wstring, CalendarEventData*>::iterator it = syncEvents.find(pevent->_originalEvent);
				if (it != syncEvents.end())
					it->second->_updateEventList[pevent->_rowid] = pevent;
			}
		}
		else
			_pLibrary->InsertCalendarEvent(pevent);
	}
	//  Ω‚ŒˆReminderData
	int remindcnt = 0;
	reader.ReadInt32(remindcnt);
	for(int k = 0; k < remindcnt; k++)
	{
		ReminderData* remind = new ReminderData();
		CalendarStreamHelper::ParseReminder(reader, remind);
		map<int, CalendarEventData*>::iterator it = eventlist.find(remind->_eventId);
		if(it != eventlist.end())
			it->second->_reminders.push_back(remind);
	}

}

void ND91Assistant::LoadCalendarCommand::SetIosReturnFile( std::wstring filename )
{
	return;
}


#endif