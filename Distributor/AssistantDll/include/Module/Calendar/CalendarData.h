#pragma once
#include "Module/BaseData.h"
#include <deque>
#include <map>
using namespace std;
#pragma warning(disable : 4251)

namespace ND91Assistant
{
	class CalendarEventData;
	// �����˻�
	class CLASSINDLL_CLASS_DECL CalendarData :public BaseData
	{
		friend class LoadCalendarCommand;
		friend class CalendarStreamHelper;
	public:
		CalendarData(void);
		virtual ~CalendarData(void);

		// Summary  : ����һ���¼�
		// Returns	: void
		// Parameter: CalendarEventData * pEvent
		void InsertCalendarEvent(CalendarEventData* pEvent);

		// Summary  :	����¼��б�
		// Returns	: const deque<CalendarEventData*>
		void GetEventList(deque<CalendarEventData*>& calendarList);

		// Summary  : �Ƴ�һ���¼�
		// Returns	: void
		// Parameter: int eventId
		void RemoveCalendarEvent(int eventId);

		// Summary  : �Ƿ����д��Ȩ��
		// Returns	: bool
		bool CanWrite();

		int			_rowId;			// ����Id

		wstring		_syncAccount;	// �˻�

		wstring		_name;			// ����

		wstring		_displayName;	// ��ʾ������

		int			_selected;		// �Ƿ�ѡ��

		bool		_hidden;		// �Ƿ�����

		int			_color;			// ��ɫ

		wstring		_timeZone;		// ʱ��

	private:
		// ���ʼ���
		enum CalendarAccessLevel
		{
			NO_ACCESS = 0,
			/** Can only see free/busy information about the calendar */
			FREEBUSY_ACCESS = 100,
			/** Can read all event details */
			READ_ACCESS = 200,

			RESPOND_ACCESS = 300,

			OVERRIDE_ACCESS = 400,
			/** Full access to modify the calendar, but not the access control settings */
			CONTRIBUTOR_ACCESS = 500,

			EDITOR_ACCESS = 600,
			/** Full access to the calendar */
			OWNER_ACCESS = 700,
			/** Domain admin */
			ROOT_ACCESS = 800
		};

		CalendarAccessLevel	 _accessLevel;	// ���ʼ���

		map<int, CalendarEventData*>  _eventList; // �¼��б�
	};

}