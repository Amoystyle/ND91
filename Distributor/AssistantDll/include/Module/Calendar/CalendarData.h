#pragma once
#include "Module/BaseData.h"
#include <deque>
#include <map>
using namespace std;
#pragma warning(disable : 4251)

namespace ND91Assistant
{
	class CalendarEventData;
	// 日历账户
	class CLASSINDLL_CLASS_DECL CalendarData :public BaseData
	{
		friend class LoadCalendarCommand;
		friend class CalendarStreamHelper;
	public:
		CalendarData(void);
		virtual ~CalendarData(void);

		// Summary  : 插入一条事件
		// Returns	: void
		// Parameter: CalendarEventData * pEvent
		void InsertCalendarEvent(CalendarEventData* pEvent);

		// Summary  :	获得事件列表
		// Returns	: const deque<CalendarEventData*>
		void GetEventList(deque<CalendarEventData*>& calendarList);

		// Summary  : 移除一条事件
		// Returns	: void
		// Parameter: int eventId
		void RemoveCalendarEvent(int eventId);

		// Summary  : 是否具有写入权限
		// Returns	: bool
		bool CanWrite();

		int			_rowId;			// 日历Id

		wstring		_syncAccount;	// 账户

		wstring		_name;			// 名称

		wstring		_displayName;	// 显示的名称

		int			_selected;		// 是否选择

		bool		_hidden;		// 是否隐藏

		int			_color;			// 颜色

		wstring		_timeZone;		// 时区

	private:
		// 访问级别
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

		CalendarAccessLevel	 _accessLevel;	// 访问级别

		map<int, CalendarEventData*>  _eventList; // 事件列表
	};

}