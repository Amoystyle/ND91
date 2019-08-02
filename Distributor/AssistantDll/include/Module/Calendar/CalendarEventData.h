#pragma once

#include "RepeatRule.h"

namespace ND91Assistant
{

	enum RepeatTypes
	{
		None = 0, 			// 不重复(0)

		EveryDay = 11,		// 每日重复(11)

		EveryWeek = 21,		// 每周重复(21)

		Every2Weeks = 22,	// 每两周重复(22)

		EveryMonth = 31,	// 每月重复(31)

		EveryYear = 41		// 每年重复(41)
	};

    //class BinaryBufferReader;
    //class BinaryBufferWriter;
    class ReminderData;

    // 日程表事件
    class CLASSINDLL_CLASS_DECL CalendarEventData : public BaseData
    {
    public:
	    CalendarEventData(void);
	    virtual ~CalendarEventData(void);

	    CalendarEventData* Clone();

	    void ConvertFrequency();

	    void UpdateReminders(int alertBeforeMinutes);

	    int			_rowid;				// 事件Id
	    int			_calendarId;		// 日历ID,calendar表中的_id
	    int			_instanceId;		// instance表中的_id
	    wstring		_summary;			// 事件名称
	    wstring		_location;			// 地点
	    int			_endSecond;
	    __int64		_start;				// 开始时间instance begin
	    int			_startSecond;
	    bool		_hasChange;			// 是否改变
	    wstring		_specifier;
	    __int64		_alarmTime;			// 闹钟提醒时间
	    int			_weekStart;			// (week_start)
	    bool		_alert ;			// 是否提醒
	    int			_alertBefore;		// 提醒时提前的秒
        int         _reAlertBefore;     // 再次提醒时提前的秒数(-1:不提醒)
	    wstring		_description;		// 描述
	    __int64		_eventdtend;		// event 上对应的结束时间
	    __int64		_eventdtstart;		// event 上的开始时间
	    __int64		_originalInstanceTime;// 原始事件实例时间
	    __int64		_lastDate;			// 截止日期
	    RepeatTypes _repeatType;		// 重复方式 就的方式
	    bool		_allDay;			// 是否为全天事件
	    int			_repeatCount;		// 重复次数
	    __int64		_endRepeatDate;		// 停止重复日期
	    int			_origEventId;		// 关联事件ID
	    wstring		_originalEvent;		// 原始事件
	    RepeatRule	_repeatRlue;		// 重复规则类
	    __int64		_origStartDate;		// 关联事件开始时间
	    int			_by_Month_Months;
	    wstring		_timeZone;			// 时区
	    bool		_repeatForever;		// 是否无限重复
	    wstring		_syncId;			// 与Google行事历同步的id
	    int			_eventStatus;		// 事件状态
	    wstring		_duration;			// 间隔时间（开始到结束）p3600s p2d
	    __int64		_instaceBegin;		// 实例起始事件
	    __int64		_instaceEnd;		// 实例结束时间

	    __int64		_end;				// 结束时间 instance end

        __int64     _occurrenceDay;     // 事件发生时间
        wstring     _exceptionDate;     // 异常事件原本发生时间，统一处理到0点，在导入的时候iPhone平台另外算到该天原事件发生时间,用逗号分割yyyy-MM-dd
        long        _interval;          // 周期间隔 1，隔1周，2，隔2周

	    deque<ReminderData*>  _reminders;// 事件提醒

	    int			_lephoneCategory;	// 是否乐phone
	    wstring		_phoneCategory;		// 手机类型
	    wstring		_exdate;			// 联想专用，更新或取消事件存于此

	    map<int, CalendarEventData*>  _cancelEventList;// 取消事件
	    map<int, CalendarEventData*>  _updateEventList;// 更新事件

	    //void ParseAndroidData(BinaryBufferReader& reader, bool IsNew = false);

	    //void AndroidDataToStream(BinaryBufferWriter& writer);

    };

}
