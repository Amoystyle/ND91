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

	// �ճ̱����ݹ�����
	class CalendarLibrary
	{
	public:
		CalendarLibrary(void);
		virtual ~CalendarLibrary(void);

		// Summary  : ��ȡ���������˻�
		// Returns	: const deque<CalendarData*>
		const deque<CalendarData*> GetCalendars();

		// Summary  : ��ȡָ���˻�ָ��ʱ�����¼�
		// Returns	: void
		// Parameter: ViewType type
		// Parameter: int calendarId
		// Parameter: deque<__int64> timelist
		// Parameter: deque<CalendarEventData * > & eventList
		void GetEventList(ViewType type, int calendarId, deque<__int64> timelist, deque<CalendarEventData*>& eventList);

		// Summary  : ����һ�������˻�
		// Returns	: void
		// Parameter: CalendarData * pCalendar
		void InsertCalendar(CalendarData* pCalendar);

		// Summary  : ����һ�������¼�
		// Returns	: void
		// Parameter: CalendarEventData * pEvent
		void InsertCalendarEvent(CalendarEventData* pEvent);

		// Summary  : 
		// Returns	: void
		void InitAllEvents();

		// Summary  : �Ƿ����ĳ�¼�
		// Returns	: CalendarEventData*
		// Parameter: int eventId
		CalendarEventData* ContainsEvent(int eventId);

		// Summary  : �Ƴ�һ���¼�
		// Returns	: void
		// Parameter: int eventId
		void RemoveEvent(int eventId);

	private:

		// Summary  : ȡ��ĳ�¼�ָ��ʱ���ڵ���Ч�¼�
		// Returns	: void
		// Parameter: ViewType type
		// Parameter: CalendarEventData * pEvent
		// Parameter: deque<__int64> timeList
		// Parameter: deque<CalendarEventData * > & eventList
		void GetEffectEventByDateList(ViewType type, CalendarEventData* pEvent, deque<__int64> timeList, deque<CalendarEventData*>& eventList);

		// Summary  : �Ƿ���ȡ���¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool IsCancelEvent(CalendarEventData* pEvent);

		// Summary  : �����ظ�����Ϊÿ�µ��¼���ָ��ʱ������ֹʱ��
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent  ԭʼ�¼�
		// Parameter: CalendarEventData * newEvent ���ɵ�ĳ�ظ��¼�
		// Parameter: int monthWay
		// Parameter: __int64 selTime
		bool InitLeftEveryMonthEffectEvent(CalendarEventData* pEvent, CalendarEventData* newEvent, __int64 selTime);

	private:
		map<int, CalendarData*>			_calendarList; // �����˻��б�

		map<int, CalendarEventData*>	_allCalendarEvents;// �����¼��б�
	};

}