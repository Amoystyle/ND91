#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/RepeatRule.h"

RepeatRule::RepeatRule(void)
{
	_frem		= L"";	// �ظ�����
	_weekstart	= L"";	// ÿ�ܿ�ʼ��ʼ�ڣ�һ��������
	_until		= L"";	// �ظ�����ʱ��  �����ʽ����yyyy-MM-dd HH:mm:ss
	_interval	= 0;	// ���
	_byMonthday = 0;	// һ���µļ���
	_strByDay	= L"";	// �ַ����������������ݿ��
	_count		= 0;	// �ظ�����
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