#pragma once

#include "Module/Calendar/ReminderData.h"
#include "Module/Calendar/RepeatRule.h"
#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"

namespace ND91Assistant
{
	enum ViewType	//�鿴����
	{
		EViewNone,
		EViewByDay, // �鿴����
		EViewByWeek,// �鿴����
		EViewByMonth,//�鿴����
		EViewByList // �鿴�嵥
	};

	enum AffectState // �¼���������
	{
		ENone,
		EOnlyInstance,		// ����ǰ�¼�
		EAllEvents,			// ���¼��������¼�
		EFollowingInstance	// ���¼��Ժ���¼�
	};
	class CalendarLibrary;
	class DeviceData;
	class CalendarEventData;
	class CalendarData;

	// �ճ̱������
	class CLASSINDLL_CLASS_DECL CalendarManager
	{
	public:
		CalendarManager(const DeviceData* pDevice);
		virtual ~CalendarManager(void);

		// Summary  : ������������
		// Returns	: void
		void Load();

		// Summary  : ������������˻�
		// Returns	: const deque<CalendarData*>
		const deque<CalendarData*> GetCalendars();

		// Summary  : ��ȡָ���˻���ָ��ʱ����¼�ʵ��
		// Returns	: const deque<CalendarEventData*> �¼��б�,���ⲿ�ͷ�
		// Parameter: ViewType type  �鿴����
		// Parameter: int calendarId �����˻�Id
		// Parameter: deque<__int64> timelist ָ��ʱ���б�
		deque<CalendarEventData*> GetEvents(ViewType type, int calendarId, deque<__int64> timelist);

		// Summary  : ɾ���ճ��¼�
		// Returns	: bool �ɹ�ʧ��
		// Parameter: CalendarEventData * pEventҪɾ�����¼�
		// Parameter: AffectState state �¼���������
		bool DeleteEvent(CalendarEventData* pEvent, AffectState state);

		// Summary  : ����¼�
		// Returns	: bool �ɹ�ʧ��
		// Parameter: CalendarEventData * pEvent Ҫ��ӵ��¼�,���ⲿ�ͷ�
		bool AddEvent(CalendarEventData* pEvent);

		// Summary  : �޸��¼�
		// Returns	: bool�ɹ�ʧ��
		// Parameter: CalendarEventData * pEvent ͨ�������޸ĺ���¼�
		// Parameter: CalendarEventData * pOldEvent �޸�ǰ���¼�
		// Parameter: AffectState state �¼���������
		// Parameter: bool IsLenove  �Ƿ������ֻ�
		bool EditEvent(CalendarEventData* pEvent, CalendarEventData* pOldEvent, AffectState state, bool IsLenove = false);


	private:
		// Summary  : ɾ��ȫ���¼�ʵ��
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteAll(CalendarEventData* pEvent);

		// Summary  : ��ɾ����ǰ�¼�ʵ��
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteOnlyInstance(CalendarEventData* pEvent);

		// Summary  : ɾ����ǰ�¼��Ժ��¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteFollowInstance(CalendarEventData* pEvent);

		// Summary  : �༭�¼�ȫ��ʵ��
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent ͨ�������޸ĺ���¼�
		// Parameter: CalendarEventData * pOldEvent �޸�ǰ���¼�
		// Parameter: bool IsLenove �Ƿ������ֻ�
		bool EditAllInstances(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

		// Summary  : ���༭��ǰ�¼�ʵ��
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent ͨ�������޸ĺ���¼�
		// Parameter: CalendarEventData * pOldEvent �޸�ǰ���¼�
		// Parameter: bool IsLenove �Ƿ������ֻ�
		bool EditOnlyInstance(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

		// Summary  : �༭��ǰ�¼��Ժ��¼�
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent ͨ�������޸ĺ���¼�
		// Parameter: CalendarEventData * pOldEvent �޸�ǰ���¼�
		// Parameter: bool IsLenove �Ƿ������ֻ�
		bool EditFollowInstances(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

	private:

		const DeviceData*  _pDevice;

		CalendarLibrary*   _pLibrary; // �ճ̱����ݹ�����

	};

}
