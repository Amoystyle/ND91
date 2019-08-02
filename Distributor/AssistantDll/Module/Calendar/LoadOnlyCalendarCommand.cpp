#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/LoadOnlyCalendarCommand.h"
#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"
#include "Module/Calendar/CalendarStreamHelper.h"
#include "Module/Calendar/CalendarLibrary.h"
#include "Module/AndroidDefine.h"

LoadOnlyCalendarCommand::LoadOnlyCalendarCommand(const DeviceData* pDevice, CalendarLibrary* pLibrary)
:TableQueryCommand(pDevice)
{
	_pLibrary = pLibrary;
	_nAndroidChannelID = CALENDAR;
}

LoadOnlyCalendarCommand::~LoadOnlyCalendarCommand(void)
{
}

void ND91Assistant::LoadOnlyCalendarCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	switch(_type)
	{
		case ELoadCalendars:
			ParseCalendars(reader);
			break;
		case ELoadCalendarEvents:
			ParseCalendarEvents(reader);
			break;
		default:
			break;
	}
}

void ND91Assistant::LoadOnlyCalendarCommand::LoadCalendars()
{
	SetParam("QureyOnlyCalenar", "", "", "");
	_type = ELoadCalendars;
	Execute();
}

void ND91Assistant::LoadOnlyCalendarCommand::LoadCalendarEventsByCalendarId( int calendarId )
{
	SetParam("QureyEventsByCalenarId", "", CStrOperation::Int2string(calendarId), "");
	_type = ELoadCalendarEvents;
	Execute();
}

void ND91Assistant::LoadOnlyCalendarCommand::ParseCalendars(BinaryBufferReader& reader)
{
	int count = 0;
	reader.ReadInt32(count);
	for (int i = 0; i < count; i++)
	{
		CalendarData* cl = new CalendarData();
		CalendarStreamHelper::ParseCalendar(reader, cl);
		_pLibrary->InsertCalendar(cl);
	}
}

void ND91Assistant::LoadOnlyCalendarCommand::ParseCalendarEvents(BinaryBufferReader& reader)
{
	int count = 0;
	reader.ReadInt32(count);
	for (int i = 0; i < count; i++)
	{
		CalendarEventData* pEvent = new CalendarEventData();
		CalendarStreamHelper::ParseCalendarEvent(reader, pEvent);
		_pLibrary->InsertCalendarEvent(pEvent);
	}
}


#endif