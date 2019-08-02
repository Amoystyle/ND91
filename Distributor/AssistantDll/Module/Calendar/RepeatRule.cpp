#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/RepeatRule.h"

RepeatRule::RepeatRule(void)
{
	_frem		= L"";	// 重复类型
	_weekstart	= L"";	// 每周开始起始于，一般是周日
	_until		= L"";	// 重复结束时间  这个格式就是yyyy-MM-dd HH:mm:ss
	_interval	= 0;	// 间隔
	_byMonthday = 0;	// 一个月的几号
	_strByDay	= L"";	// 字符串，用来插入数据库的
	_count		= 0;	// 重复次数
}

RepeatRule::~RepeatRule(void)
{
}

ND91Assistant::Days::Days()
{
	clear();
}

ND91Assistant::Days::~Days()
{
}

bool ND91Assistant::Days::IsNullOfDays()
{
	return (_repeatDaytype == ERepeatNoneday);
}

std::wstring ND91Assistant::Days::ByDays()
{
	wstring s = L"";

	s += _repeatDaytype&ERepeatSunday		? L"SU," : L"";
	s += _repeatDaytype&ERepeatMonday		? L"MO," : L"";
	s += _repeatDaytype&ERepeatTuesday		? L"TU," : L"";
	s += _repeatDaytype&ERepeatWednesday	? L"WE," : L"";
	s += _repeatDaytype&ERepeatThursday		? L"TH," : L"";
	s += _repeatDaytype&ERepeatFriday		? L"FR," : L"";
	s += _repeatDaytype&ERepeatSaturday		? L"SA," : L"";

	//
	if (!s.empty())
	{
		s = s.substr(0, s.length() - 1);
	}
	return s;
}

void ND91Assistant::Days::clear()
{
	_repeatDaytype = ERepeatNoneday;
}

void ND91Assistant::Days::SetDays( RepeatDayType type, bool enable /*= true*/ )
{
	if (enable)
		_repeatDaytype |= type;
	else
	{
		if(_repeatDaytype&type)
			_repeatDaytype -= type;
	}
}

bool ND91Assistant::Days::IsRepeat( RepeatDayType type )
{
	return (_repeatDaytype&type) == type;
}

int ND91Assistant::Days::GetRepeatDayType()
{
    return _repeatDaytype;
}

void ND91Assistant::Days::SetRepeatDayType(int nRepeatType)
{
    _repeatDaytype = nRepeatType;
}


#endif