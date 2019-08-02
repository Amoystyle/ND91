#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"

CalendarData::CalendarData(void)
{
	_rowId = 1;			// 日历Id


	_selected = 0;		// 是否选择

	_hidden = false;		// 是否隐藏

	_color = 0;			// 颜色

	_accessLevel = NO_ACCESS;	// 访问级别
}

CalendarData::~CalendarData(void)
{
	/*map<int, CalendarEventData*>::iterator it = _eventList.begin();
	while(it != _eventList.end())
	{
		SAFE_DELETE(it->second);
		it++;
	}*/
	_eventList.clear();
}

/*void ND91Assistant::CalendarData::ParseAndroidData( BinaryBufferReader& reader, bool IsNew)
{
	reader.ReadInt(_rowId);
	reader.ReadStringFromUTF8(_syncAccount);
	reader.ReadStringFromUTF8(_name);
	reader.ReadStringFromUTF8(_displayName);
	int hide = 0;
	reader.ReadInt(hide);
	_hidden = hide == 1?true:false;
	reader.ReadInt(_color);
	reader.ReadInt(_selected);
	reader.ReadStringFromUTF8(_timeZone);
	if (IsNew)
		reader.ReadInt(_accessLevel);
	else
	{
		int count = 0;
		reader.ReadInt(count);
		for (int i = 0; i < count; i++)
		{
			CalendarEventData* pEvent = new CalendarEventData();
			pEvent->ParseAndroidData(reader);
			_eventList[pEvent->_calendarId]= pEvent;
		}
	}
	}*/

void ND91Assistant::CalendarData::InsertCalendarEvent( CalendarEventData* pEvent )
{
	RETURN_IF(!pEvent);
	map<int, CalendarEventData*>::iterator evtit = _eventList.find(pEvent->_rowid);
	if (evtit == _eventList.end())
		_eventList[pEvent->_rowid] = pEvent;
}

void ND91Assistant::CalendarData::GetEventList(deque<CalendarEventData*>& calendarList)
{
	map<int, CalendarEventData*>::const_iterator it = _eventList.begin();
	while( it != _eventList.end() )
	{
		calendarList.push_back(it->second);
		it ++;
	}
}

void ND91Assistant::CalendarData::RemoveCalendarEvent( int eventId )
{
	map<int, CalendarEventData*>::iterator it = _eventList.find(eventId);
	if(it != _eventList.end())
	{
		_eventList.erase(it);
	}
}

bool ND91Assistant::CalendarData::CanWrite()
{
	return _accessLevel > OVERRIDE_ACCESS;
}


#endif