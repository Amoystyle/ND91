#pragma once

#include "Module/Calendar/ReminderData.h"
#include "Module/Calendar/RepeatRule.h"
#include "Module/Calendar/CalendarData.h"
#include "Module/Calendar/CalendarEventData.h"

namespace ND91Assistant
{
	enum ViewType	//查看类型
	{
		EViewNone,
		EViewByDay, // 查看今天
		EViewByWeek,// 查看本周
		EViewByMonth,//查看本月
		EViewByList // 查看清单
	};

	enum AffectState // 事件操作类型
	{
		ENone,
		EOnlyInstance,		// 仅当前事件
		EAllEvents,			// 该事件的所有事件
		EFollowingInstance	// 该事件以后的事件
	};
	class CalendarLibrary;
	class DeviceData;
	class CalendarEventData;
	class CalendarData;

	// 日程表管理类
	class CLASSINDLL_CLASS_DECL CalendarManager
	{
	public:
		CalendarManager(const DeviceData* pDevice);
		virtual ~CalendarManager(void);

		// Summary  : 加载日历数据
		// Returns	: void
		void Load();

		// Summary  : 获得所有日历账户
		// Returns	: const deque<CalendarData*>
		const deque<CalendarData*> GetCalendars();

		// Summary  : 获取指定账户下指定时间的事件实例
		// Returns	: const deque<CalendarEventData*> 事件列表,由外部释放
		// Parameter: ViewType type  查看类型
		// Parameter: int calendarId 日历账户Id
		// Parameter: deque<__int64> timelist 指定时间列表
		deque<CalendarEventData*> GetEvents(ViewType type, int calendarId, deque<__int64> timelist);

		// Summary  : 删除日程事件
		// Returns	: bool 成功失败
		// Parameter: CalendarEventData * pEvent要删除的事件
		// Parameter: AffectState state 事件操作类型
		bool DeleteEvent(CalendarEventData* pEvent, AffectState state);

		// Summary  : 添加事件
		// Returns	: bool 成功失败
		// Parameter: CalendarEventData * pEvent 要添加的事件,由外部释放
		bool AddEvent(CalendarEventData* pEvent);

		// Summary  : 修改事件
		// Returns	: bool成功失败
		// Parameter: CalendarEventData * pEvent 通过界面修改后的事件
		// Parameter: CalendarEventData * pOldEvent 修改前的事件
		// Parameter: AffectState state 事件操作类型
		// Parameter: bool IsLenove  是否联想手机
		bool EditEvent(CalendarEventData* pEvent, CalendarEventData* pOldEvent, AffectState state, bool IsLenove = false);


	private:
		// Summary  : 删除全部事件实例
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteAll(CalendarEventData* pEvent);

		// Summary  : 仅删除当前事件实例
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteOnlyInstance(CalendarEventData* pEvent);

		// Summary  : 删除当前事件以后事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent
		bool DeleteFollowInstance(CalendarEventData* pEvent);

		// Summary  : 编辑事件全部实例
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent 通过界面修改后的事件
		// Parameter: CalendarEventData * pOldEvent 修改前的事件
		// Parameter: bool IsLenove 是否联想手机
		bool EditAllInstances(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

		// Summary  : 仅编辑当前事件实例
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent 通过界面修改后的事件
		// Parameter: CalendarEventData * pOldEvent 修改前的事件
		// Parameter: bool IsLenove 是否联想手机
		bool EditOnlyInstance(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

		// Summary  : 编辑当前事件以后事件
		// Returns	: bool
		// Parameter: CalendarEventData * pEvent 通过界面修改后的事件
		// Parameter: CalendarEventData * pOldEvent 修改前的事件
		// Parameter: bool IsLenove 是否联想手机
		bool EditFollowInstances(CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove);

	private:

		const DeviceData*  _pDevice;

		CalendarLibrary*   _pLibrary; // 日程表数据管理类

	};

}
