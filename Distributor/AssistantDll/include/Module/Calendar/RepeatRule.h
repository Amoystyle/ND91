#pragma once
#include "AssistantDll.h"
#include <xstring>

using namespace std;
namespace ND91Assistant
{

	enum RepeatDayType
	{
		ERepeatNoneday		= 0,	 // 无重复
		ERepeatSunday		= 1,	 // 星期天
		ERepeatMonday		= 1 << 1,// 星期一
		ERepeatTuesday		= 1 << 2,// 星期二
		ERepeatWednesday	= 1	<< 3,// 星期三
		ERepeatThursday		= 1 << 4,// 星期四
		ERepeatFriday		= 1 << 5,// 星期五
		ERepeatSaturday		= 1 << 6,// 星期六
		ERepeatMonthlyByDay	= 1 << 7 // 一个月的第几个星期几
	};
	// 星期结构
	class CLASSINDLL_CLASS_DECL Days
	{
		//---------weekly-------------
	public:

		Days();
		~Days();

		bool IsNullOfDays();

		// 返回bydays
		wstring ByDays();

		void clear();

		// Summary  : 设置重复日期
		// Returns	: void
		// Parameter: RepeatDayType type 
		// Parameter: bool enable 是否开启
		void SetDays(RepeatDayType type, bool enable = true);

		// Summary  :
		// Returns	: bool
		// Parameter: RepeatDayType type
		bool IsRepeat(RepeatDayType type);

        int GetRepeatDayType();
        void SetRepeatDayType(int nRepeatType);
	private:
		int     _repeatDaytype;
	};

	// 重复规则
	class CLASSINDLL_CLASS_DECL RepeatRule
	{
	public:
		RepeatRule(void);
		virtual ~RepeatRule(void);

		wstring	_frem;		// 重复类型
		wstring	_weekstart;	// 每周开始起始于，一般是周日
		wstring	_until;		// 重复结束时间  这个格式就是yyyy-MM-dd HH:mm:ss
		__int64	_repeatEnd; // 重复截至日期
		int		_interval;  // 间隔
		Days	_byDays;	// su,mo 这个用来拆解用的
		int		_byMonthday;// 一个月的几号
		wstring	_strByDay;	// 字符串，用来插入数据库的
		int		_count;		// 重复次数
	};

}

