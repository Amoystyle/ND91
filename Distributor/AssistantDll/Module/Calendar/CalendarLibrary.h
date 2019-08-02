#pragma once
#include "AssistantDll.h"
#include <map>
#include <vector>
#include "Module/Calendar/CalendarManager.h"

using namespace std;

namespace ND91Assistant
{
	class CalendarData;
	class CalendarEventData;

	// 日程表数据管理类
	class CalendarLibrary
	{
	public:
		CalendarLibrary(void);
		virtual ~CalendarLibrary(void);

		// Summary  : 获取所有日历账户
		// Returns	: const deque<CalendarData*>
		const deque<CalendarData*> GetCalendars();

		// Summary  : 获取指定账户指定时间内事件
		// Returns	: void
		// Parameter: ViewType type
		// Parameter: int calendarId
		// Parameter: deque<__int64> timelist
		// Parameter: deque<CalendarEventData * > & eventList
		void GetEventList(ViewType type, int calendarId, deque<__int64> timelist, deque<CalendarEventData*>& eventList);

		// Summary  : 加入一个日历账户
		// Returns	: void
		// Parameter: CalendarData * pCalendar
		void InsertCalendar(CalendarData* pCalendar);

		// Summary  : 加入一个日历事件
		// Returns	: void
		// Parameter: CalendarEventData * pEvent
		void InsertCalendarEvent(CalendarEventData* pEvent);

		// Summary  : 
		// Returns	: void
		void InitAllEvents();

		// Summary  : 是否包含某事件
		// Returns	: CalendarEventData*
		// Parameter: int eventId
		CalendarEventData* ContainsEvent(int eventId);

		// Summary  : 移除一个事件
		// Returns	: void
		// Parameter: int eventId
		void RemoveEvent(int eventId);

	private:

		// Summary  : 取得某事件指定时间内的有效事件
		// Returns	: void
		// Parameter: ViewType type
		// Parameter: CalendarEventData * pEvent
		// Parameter: deque<__int64> timeList
		// Parameter: deque<CalendarEventData * > & eventList
		void GetEffectEventByDateList(ViewType type, CalendarEventData* pEvent, deque<__int64> timeList, deque<CalendarEventData*>& eventList);

		// Summary  : 是否是取消事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool IsCancelEvent(CalendarEventData* pEvent);

		// Summary  : 设置重复类型为每月的事件的指定时间下起止时间
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent  原始事件
		// Parameter: CalendarEventData * newEvent 生成的某重复事件
		// Parameter: int monthWay
		// Parameter: __int64 selTime
		bool InitLeftEveryMonthEffectEvent(CalendarEventData* pEvent, CalendarEventData* newEvent, __int64 selTime);

	private:
		map<int, CalendarData*>			_calendarList; // 日历账户列表

		map<int, CalendarEventData*>	_allCalendarEvents;// 日历事件列表
	};

}