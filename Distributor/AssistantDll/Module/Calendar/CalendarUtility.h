#pragma once

#include "Module/Calendar/RepeatRule.h"
#include "Module/Calendar/CalendarEventData.h"

namespace ND91Assistant
{

enum EventDateWhere // 事件时间段在所选时间的位置
{
	ENothing = 0,  
	ELeft	 = 1,		// 左侧
	EInner   = 2,		// 中间
	ERight   = 3		// 右侧
};
// 日程表的一些辅助方法
class CalendarUtility
{
public:
	CalendarUtility(void);
	virtual ~CalendarUtility(void);

	// Summary  : 解析得到重复规则
	// Returns	: void
	// Parameter: RepeatRlue & rrlueObj 重复规则
	// Parameter: string rrlue 要解析的规则串
	static void SplitRepeatrule(RepeatRule& rrlueObj, string rrlue);

	// Summary  : 获得按周重复的星期几mask，主要是用来判断周重复(当天还是工作日(星期一至星期五)62)
	// Returns	: int 星期几mask
	// Parameter: RepeatRlue repeatObj 重复规则
	static int SelectRepeatWeek(RepeatRule repeatObj);

	// Summary  : 规则转化为字符串
	// Returns	: string
	// Parameter: RepeatRlue repeatObj
	static string RepeatruleToString(RepeatRule repeatObj);

	// Summary  : 重构规则
	// Returns	: string 规则字串
	// Parameter: __int64 instanceTime
	// Parameter: RepeatRlue repeatObj
	static string  ModifyRepeatRule(__int64 instanceTime,  RepeatRule repeatObj);


	// Summary  : 取出仅包含“UNTIL”的串
	// Returns	: string
	// Parameter: string oldrrule
	static string GetRepeatRuleWithOut(string oldrrule);

	// Summary  : 根据重复规则获得重复类型
	// Returns	: ND91Assistant::RepeatTypes
	// Parameter: RepeatRlue repeatObj
	static RepeatTypes SelectRepeat(RepeatRule repeatObj);

	// Summary  : 工作日截止日期
	// Returns	: __int64 
	// Parameter: __int64 starttime 开始时间
	// Parameter: int count 重复次数
	static __int64 SelectWorkDatesEndDateTime(__int64 starttime, int count);

	// Summary  :获取重复的每月日期列表
	// Returns	: deque<__int64> 时间列表
	// Parameter: __int64 dtStart 起始时间
	// Parameter: int count 重复次数
	static deque<__int64> GetMonthRepeatTimeList(__int64 dtstart, int count);

	// Summary  : 取得事件截止时间
	// Returns	: __int64 截止时间
	// Parameter: CalendarEventData * pEvent 事件
	static __int64 GetUntilTime(CalendarEventData* pEvent);

	// Summary  :
	// Returns	: ND91Assistant::EventDateWhere
	// Parameter: __int64 dt
	// Parameter: __int64 selectBeginDate
	// Parameter: __int64 selectEndDate
	static EventDateWhere ExistInSelectDates(__int64 dt, __int64 selectBeginDate, __int64 selectEndDate);

	// Summary  : 解析得到Days
	// Returns	: ND91Assistant::Days 
	// Parameter: string s 要解析的字符串
	static void ByDays(Days& d, string s);

	// Summary  : 获得星期几字符串
	// Returns	: std::wstring 星期几
	// Parameter: __int64 time 时间
	static wstring GetWeekDay( __int64 time);

	// Summary  :获取重复的每年日期列表
	// Returns	: deque<__int64> 时间列表
	// Parameter: __int64 dtStart 起始时间
	// Parameter: int count 重复次数
	static deque<__int64> LephoneYearRepeatView( __int64 dtStart, int count );

	// Summary  : 得到下个月的第几个星期几的日期
	// Returns	: __int64 
	// Parameter: __int64 begin 本月日期
	// Parameter: int monthdays
	static __int64 GetByWeekDate(__int64 begin, int monthdays);

	// Summary  : 一个月的第几个星期索引，内部用
	// Returns	: int
	// Parameter: __int64 dtSel 时间
	// Parameter: bool sundayStart 是否从星期天开始
	static int WeekOfMonth(__int64 dtSel, bool sundayStart);

	// Summary  : 解析为时间格式yyyy-MM-dd HH:mm:ss
	// Returns	: string
	// Parameter: string s要解析的字符串
	static string ConvertStrToLongDateTime(string s);

	// Summary  : 时间转化为yyyyMMddTHHmmssZ格式字符串
	// Returns	: string
	// Parameter: __int64 time
	static string ConvertTimeRule(__int64 time);

	// Summary  : yyyyMMddTHHmmssZ格式字符串转化为时间
	// Returns	: __int64
	// Parameter: string timestr
	static __int64 ConvertStringToTime(string timestr);

	// Summary  :联想手机，该事件以后事件的截止日期
	// Returns	: __int64 
	// Parameter: CalendarEventData * pEvent
	static __int64 FollowingInstanceEditUntilDate(CalendarEventData* pEvent);
};

}
