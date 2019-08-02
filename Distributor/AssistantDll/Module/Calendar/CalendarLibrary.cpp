#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "CalendarLibrary.h"
#include "CalendarUtility.h"

#define  ONE_HOUR_SECENDS  3600
#define  ONE_DAY_SECENDS   24*3600

CalendarLibrary::CalendarLibrary(void)
{
}

CalendarLibrary::~CalendarLibrary(void)
{
	map<int, CalendarData*>::iterator it = _calendarList.begin();
	while(it != _calendarList.end())
	{
		SAFE_DELETE(it->second);
		it++;
	}
	_calendarList.clear();

	map<int,CalendarEventData*>::iterator iter = _allCalendarEvents.begin();
	while(iter != _allCalendarEvents.end())
	{
		SAFE_DELETE(iter->second);
		iter++;
	}
	_allCalendarEvents.clear();
}

void ND91Assistant::CalendarLibrary::InsertCalendar( CalendarData* pCalendar )
{
	map<int, CalendarData*>::iterator it = _calendarList.find(pCalendar->_rowId);
	if (it == _calendarList.end())
		_calendarList[pCalendar->_rowId] = pCalendar;
}

void ND91Assistant::CalendarLibrary::InsertCalendarEvent( CalendarEventData* pEvent )
{

	if(pEvent->_eventStatus != 2 || pEvent->_originalEvent.empty())
	{
		map<int, CalendarData*>::iterator it = _calendarList.find(pEvent->_calendarId);
		if (it != _calendarList.end())
			it->second->InsertCalendarEvent(pEvent);
		if(_allCalendarEvents.find(pEvent->_rowid) == _allCalendarEvents.end())
			_allCalendarEvents[pEvent->_rowid] = pEvent;
	}
}

void ND91Assistant::CalendarLibrary::InitAllEvents()
{
	map<int, CalendarData*>::iterator caliter = _calendarList.begin();
	while(caliter != _calendarList.end())
	{
		deque<CalendarEventData*> updates;

		deque<CalendarEventData*> callist;
		caliter->second->GetEventList(callist);
		for (deque<CalendarEventData*>::const_iterator eventIter = callist.begin(); eventIter != callist.end(); eventIter++)
		{
			if((*eventIter)->_origEventId != 0 )
			{
				updates.push_back((*eventIter));
			}
			if(_allCalendarEvents.find((*eventIter)->_rowid) == _allCalendarEvents.end())
			{
				_allCalendarEvents[(*eventIter)->_rowid] = (*eventIter);
			}
		}
		
		for (size_t i = 0; i < updates.size(); i++)
		{
			int eventId =updates.at(i)->_origEventId;
			if(_allCalendarEvents.find(eventId) != _allCalendarEvents.end())
			{
				_allCalendarEvents[eventId]->_updateEventList[updates.at(i)->_rowid]=updates.at(i);
			}

		}
		caliter++;
	}
}

void ND91Assistant::CalendarLibrary::GetEventList( ViewType type, int calendarId, deque<__int64> timelist, deque<CalendarEventData*>& eventList)
{
	/*map<int, CalendarData*>::iterator it = _calendarList.find(calendarId);
	if(it == _calendarList.end())
		return;*/

	map<int, CalendarEventData*>::iterator evtIter = _allCalendarEvents.begin();
	while( evtIter != _allCalendarEvents.end())
	{
		CalendarEventData* pEvent = evtIter->second;
		if(calendarId == -1 || pEvent->_calendarId == calendarId)
		{
			GetEffectEventByDateList(type, pEvent, timelist, eventList);
		}
		evtIter++;
	}
}

bool ND91Assistant::CalendarLibrary::IsCancelEvent( CalendarEventData* pEvent )
{
	bool contain = false;
	if(pEvent->_phoneCategory != L"lenovo")
	{
		map<int, CalendarEventData*>::iterator it = pEvent->_cancelEventList.begin();
		while(it != pEvent->_cancelEventList.end())
		{
			if(it->second->_allDay)
			{
				if(CDealTime(it->second->_start).DaysToTime(CDealTime(pEvent->_start))== 0 && CDealTime(it->second->_end).DaysToTime( CDealTime(pEvent->_end)) == 0)
				{
					contain = true;
					break;
				}
				else if(it->second->_start - pEvent->_start == 0 && it->second->_end - pEvent->_end == 0)
				{
					contain = true;
					break;
				}
			}
			it++;
		}

		it = pEvent->_updateEventList.begin();
		while(it != pEvent->_updateEventList.end())
		{
			if(it->second->_originalInstanceTime - pEvent->_start == 0)
			{
				contain = true;
				break;
			}
			it++;
		}
	}
	else
	{
		wstring exdate = pEvent->_exdate;
		if(!pEvent->_exdate.empty())
		{
			vector<wstring> deleteEvents = CStrOperation::parseStringsW(pEvent->_exdate, L",");
			for(size_t i = 0; i < deleteEvents.size(); i++)
			{
				wstring s = deleteEvents[i];
				__int64 deltime = CalendarUtility::ConvertStringToTime(CCodeOperation::UnicodeToUTF_8(s));
				deltime += 8*ONE_HOUR_SECENDS;
				if(CDealTime(deltime).DaysToTime(CDealTime(pEvent->_start))==0)
				{
					contain = true;
					break;
				}
			}
		}
	}
	return contain;
}

const deque<CalendarData*> ND91Assistant::CalendarLibrary::GetCalendars()
{
	deque<CalendarData*> calendarlist;
	map<int, CalendarData*>::iterator it = _calendarList.begin();
	while(it != _calendarList.end())
	{
		calendarlist.push_back(it->second);
		it++;
	}
	return calendarlist;
}

CalendarEventData* ND91Assistant::CalendarLibrary::ContainsEvent( int eventId )
{
	map<int, CalendarEventData*>::iterator it = _allCalendarEvents.find(eventId);
	if (it != _allCalendarEvents.end())
	{
		return it->second;
	}
	return NULL;
}

void ND91Assistant::CalendarLibrary::RemoveEvent( int eventId )
{
	map<int, CalendarEventData*>::iterator it = _allCalendarEvents.find(eventId);
	if(it != _allCalendarEvents.end())
	{
		map<int, CalendarData*>::iterator itCal = _calendarList.find(it->second->_calendarId);
		if(itCal != _calendarList.end())
		{
			itCal->second->RemoveCalendarEvent(it->second->_rowid);
		}
		SAFE_DELETE(it->second);
		_allCalendarEvents.erase(it);
	}
}

void CalendarLibrary::GetEffectEventByDateList(ViewType type, CalendarEventData* pEvent, deque<__int64> timeList, deque<CalendarEventData*>& eventList)
{
	for(deque<__int64>::iterator it = timeList.begin(); it != timeList.end(); it++)
	{
		__int64 selectEndDay = (*it)+ONE_DAY_SECENDS;
		__int64 dtstart = pEvent->_start;
		__int64 dtend = pEvent->_end;
		if(dtstart <= 0)
			continue;
		int eventDayLength = 0;

		__int64 dtOver = 0;

		if (dtend <=0)
			dtend = dtstart+ONE_HOUR_SECENDS;

		CDealTime deltime(dtend);
		if (deltime.GetHour()==0 && deltime.GetMinute()==0 && deltime.GetSecond()==0)
			dtOver = dtend-1;

		eventDayLength = CDealTime(dtstart).DaysToTime(deltime);

		int  interval = pEvent->_repeatRlue._interval;
		if (interval == 0)
			interval = 1;

		__int64 until = CalendarUtility::GetUntilTime(pEvent);

		int startTotodays = CDealTime(dtstart).DaysToTime(CDealTime((*it)));

		__int64 today = dtstart+startTotodays*ONE_DAY_SECENDS;

		int untilTo = 1;
		if(until > 0 && startTotodays == 0)
		{
			untilTo = CDealTime(dtstart).DaysToTime(CDealTime(until));
		}

		bool isFirstEvent = ((*it)-dtstart==0);
		
		if(!isFirstEvent && until != 0 && until-today < 0 && untilTo != 0)
			continue;

		CalendarEventData* newEvent = pEvent->Clone();
		bool contains = false;
		EventDateWhere datewhere = CalendarUtility::ExistInSelectDates(dtstart, (*it), selectEndDay);
		switch(datewhere)
		{
			case ELeft:
				{
					switch(pEvent->_repeatType)
					{
						case None:
							{
								if(eventDayLength >= 1 && (type != EViewByWeek || timeList.size() != 7))
								{
									if (datewhere == ELeft && dtend > (*it))
									{
										datewhere = EInner;
										contains = true;
									}
								}
							}
							break;
						case EveryDay:
							{
								if(startTotodays%interval == 0)
								{
									newEvent->_start = dtstart+startTotodays*ONE_DAY_SECENDS;
									newEvent->_end = dtend+startTotodays*ONE_DAY_SECENDS;
									contains = true;
								}
							}
							break;
						case EveryWeek:
						case Every2Weeks:
							{
								wstring bydayofweek = pEvent->_repeatRlue._byDays.ByDays();
								__int64 beginRang = CDealTime(dtstart).WeekRange().GetTime();
								__int64 selectRang = CDealTime((*it)).WeekRange().GetTime();
								int fromdays = CDealTime(beginRang).DaysToTime(CDealTime(selectRang));
								if(fromdays >= 7 && (fromdays % (interval*7)) != 0)
									break;
								wstring s = CalendarUtility::GetWeekDay((*it));
								if( (int)bydayofweek.find(s) >= 0 )
								{
									newEvent->_start = dtstart+startTotodays*ONE_DAY_SECENDS;
									newEvent->_end = dtend + startTotodays*ONE_DAY_SECENDS;
									contains = true;
								}
							}
							break;
						case EveryMonth:
							{
								contains = InitLeftEveryMonthEffectEvent(pEvent, newEvent, (*it));
							}
							break;
						case EveryYear:
							{
								int fromyear = CDealTime((*it)).GetYear() - CDealTime(dtstart).GetYear();
								if (fromyear%interval != 0)
									break;
								CDealTime dedt(dtstart);
								dedt.addMonth(12*fromyear);
								if(dedt.GetMonth() == CDealTime((*it)).GetMonth() && dedt.GetDay() == CDealTime((*it)).GetDay())
								{
									newEvent->_start = CDealTime(dtstart).addMonth(12*(fromyear-1));
									newEvent->_end = CDealTime(dtend).addMonth(12*fromyear);
									contains = true;
								}
							}
							break;
						default:
							break;
					}
				}
				break;
			case EInner:
				{
					contains = true;
				}
				break;
			case ERight:
				break;
			default:
				break;
		}
		if(contains)
		{
			newEvent->_instaceBegin = newEvent->_start;
			newEvent->_instaceEnd = newEvent->_end;	
			if (!IsCancelEvent(newEvent))
				eventList.push_back(newEvent);
		}
		else
			SAFE_DELETE(newEvent);
	}
}

bool CalendarLibrary::InitLeftEveryMonthEffectEvent(CalendarEventData* pEvent, CalendarEventData* newEvent, __int64 selTime)
{
	bool contains = false;
	__int64 dtstart = pEvent->_start;
	__int64 dtend = pEvent->_end;

	if (dtend <=0)
		dtend = dtstart+ONE_HOUR_SECENDS;

	int  interval = pEvent->_repeatRlue._interval;
	if (interval == 0)
		interval = 1;

	int monthWay = 0;
	if(!pEvent->_repeatRlue._strByDay.empty() && !pEvent->_repeatRlue._byDays.IsRepeat(ERepeatMonthlyByDay))
		monthWay = 1;

	int years = CDealTime(dtstart).GetYear()-CDealTime(selTime).GetYear();
	int selectMonth = years*12+CDealTime(dtstart).GetMonth()-CDealTime(selTime).GetMonth();
	if(monthWay == 1)
	{
		int repeatDay = CDealTime(dtstart).GetDay();
		if (repeatDay <= 28 || pEvent->_phoneCategory != L"lenovo")
		{
			if(selectMonth == 0 || selectMonth %interval == 0)
			{
				CDealTime dealdt(dtstart);
				dealdt.addMonth(selectMonth);
				if(dealdt.GetDay() == CDealTime(selTime).GetDay())
					contains = true;
			}
		}
		else
		{
			if (selectMonth == 0 || (selectMonth % interval == 0))
			{
				if(CDealTime(selTime).GetDay() == CDealTime(dtstart).GetDay())
				{
					deque<__int64> rpdates = CalendarUtility::GetMonthRepeatTimeList(dtstart, pEvent->_repeatRlue._count);
					for (size_t i = 0; i < rpdates.size(); i++)
					{
						if(CDealTime(selTime).DaysToTime(CDealTime(rpdates[i])) == 0)
						{
							contains = true;
							break;
						}
					}
				}
			}
		}
		if(contains)
		{
			newEvent->_start = CDealTime(dtstart).addMonth(selectMonth);
			newEvent->_end = CDealTime(dtend).addMonth(selectMonth);
		}
	}
	else
	{
		if(selectMonth == 0 || (selectMonth % interval) == 0)
		{
			__int64 weekTime = CalendarUtility::GetByWeekDate(dtstart, selectMonth);
			if(CDealTime(weekTime).GetDay() == CDealTime(selTime).GetDay())
			{
				contains = true;
				int alldays = CDealTime(dtstart).DaysToTime(CDealTime(weekTime));
				newEvent->_start = dtstart+alldays*ONE_DAY_SECENDS;
				newEvent->_end = dtstart+alldays*ONE_DAY_SECENDS;
			}
		}
	}
	return contains;
}


#endif