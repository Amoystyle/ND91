#pragma once

#include "Module/Calendar/RepeatRule.h"
#include "Module/Calendar/CalendarEventData.h"

namespace ND91Assistant
{

enum EventDateWhere // �¼�ʱ�������ѡʱ���λ��
{
	ENothing = 0,  
	ELeft	 = 1,		// ���
	EInner   = 2,		// �м�
	ERight   = 3		// �Ҳ�
};
// �ճ̱��һЩ��������
class CalendarUtility
{
public:
	CalendarUtility(void);
	virtual ~CalendarUtility(void);

	// Summary  : �����õ��ظ�����
	// Returns	: void
	// Parameter: RepeatRlue & rrlueObj �ظ�����
	// Parameter: string rrlue Ҫ�����Ĺ���
	static void SplitRepeatrule(RepeatRule& rrlueObj, string rrlue);

	// Summary  : ��ð����ظ������ڼ�mask����Ҫ�������ж����ظ�(���컹�ǹ�����(����һ��������)62)
	// Returns	: int ���ڼ�mask
	// Parameter: RepeatRlue repeatObj �ظ�����
	static int SelectRepeatWeek(RepeatRule repeatObj);

	// Summary  : ����ת��Ϊ�ַ���
	// Returns	: string
	// Parameter: RepeatRlue repeatObj
	static string RepeatruleToString(RepeatRule repeatObj);

	// Summary  : �ع�����
	// Returns	: string �����ִ�
	// Parameter: __int64 instanceTime
	// Parameter: RepeatRlue repeatObj
	static string  ModifyRepeatRule(__int64 instanceTime,  RepeatRule repeatObj);


	// Summary  : ȡ����������UNTIL���Ĵ�
	// Returns	: string
	// Parameter: string oldrrule
	static string GetRepeatRuleWithOut(string oldrrule);

	// Summary  : �����ظ��������ظ�����
	// Returns	: ND91Assistant::RepeatTypes
	// Parameter: RepeatRlue repeatObj
	static RepeatTypes SelectRepeat(RepeatRule repeatObj);

	// Summary  : �����ս�ֹ����
	// Returns	: __int64 
	// Parameter: __int64 starttime ��ʼʱ��
	// Parameter: int count �ظ�����
	static __int64 SelectWorkDatesEndDateTime(__int64 starttime, int count);

	// Summary  :��ȡ�ظ���ÿ�������б�
	// Returns	: deque<__int64> ʱ���б�
	// Parameter: __int64 dtStart ��ʼʱ��
	// Parameter: int count �ظ�����
	static deque<__int64> GetMonthRepeatTimeList(__int64 dtstart, int count);

	// Summary  : ȡ���¼���ֹʱ��
	// Returns	: __int64 ��ֹʱ��
	// Parameter: CalendarEventData * pEvent �¼�
	static __int64 GetUntilTime(CalendarEventData* pEvent);

	// Summary  :
	// Returns	: ND91Assistant::EventDateWhere
	// Parameter: __int64 dt
	// Parameter: __int64 selectBeginDate
	// Parameter: __int64 selectEndDate
	static EventDateWhere ExistInSelectDates(__int64 dt, __int64 selectBeginDate, __int64 selectEndDate);

	// Summary  : �����õ�Days
	// Returns	: ND91Assistant::Days 
	// Parameter: string s Ҫ�������ַ���
	static void ByDays(Days& d, string s);

	// Summary  : ������ڼ��ַ���
	// Returns	: std::wstring ���ڼ�
	// Parameter: __int64 time ʱ��
	static wstring GetWeekDay( __int64 time);

	// Summary  :��ȡ�ظ���ÿ�������б�
	// Returns	: deque<__int64> ʱ���б�
	// Parameter: __int64 dtStart ��ʼʱ��
	// Parameter: int count �ظ�����
	static deque<__int64> LephoneYearRepeatView( __int64 dtStart, int count );

	// Summary  : �õ��¸��µĵڼ������ڼ�������
	// Returns	: __int64 
	// Parameter: __int64 begin ��������
	// Parameter: int monthdays
	static __int64 GetByWeekDate(__int64 begin, int monthdays);

	// Summary  : һ���µĵڼ��������������ڲ���
	// Returns	: int
	// Parameter: __int64 dtSel ʱ��
	// Parameter: bool sundayStart �Ƿ�������쿪ʼ
	static int WeekOfMonth(__int64 dtSel, bool sundayStart);

	// Summary  : ����Ϊʱ���ʽyyyy-MM-dd HH:mm:ss
	// Returns	: string
	// Parameter: string sҪ�������ַ���
	static string ConvertStrToLongDateTime(string s);

	// Summary  : ʱ��ת��ΪyyyyMMddTHHmmssZ��ʽ�ַ���
	// Returns	: string
	// Parameter: __int64 time
	static string ConvertTimeRule(__int64 time);

	// Summary  : yyyyMMddTHHmmssZ��ʽ�ַ���ת��Ϊʱ��
	// Returns	: __int64
	// Parameter: string timestr
	static __int64 ConvertStringToTime(string timestr);

	// Summary  :�����ֻ������¼��Ժ��¼��Ľ�ֹ����
	// Returns	: __int64 
	// Parameter: CalendarEventData * pEvent
	static __int64 FollowingInstanceEditUntilDate(CalendarEventData* pEvent);
};

}
