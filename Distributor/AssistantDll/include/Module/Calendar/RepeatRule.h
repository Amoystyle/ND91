#pragma once
#include "AssistantDll.h"
#include <xstring>

using namespace std;
namespace ND91Assistant
{

	enum RepeatDayType
	{
		ERepeatNoneday		= 0,	 // ���ظ�
		ERepeatSunday		= 1,	 // ������
		ERepeatMonday		= 1 << 1,// ����һ
		ERepeatTuesday		= 1 << 2,// ���ڶ�
		ERepeatWednesday	= 1	<< 3,// ������
		ERepeatThursday		= 1 << 4,// ������
		ERepeatFriday		= 1 << 5,// ������
		ERepeatSaturday		= 1 << 6,// ������
		ERepeatMonthlyByDay	= 1 << 7 // һ���µĵڼ������ڼ�
	};
	// ���ڽṹ
	class CLASSINDLL_CLASS_DECL Days
	{
		//---------weekly-------------
	public:

		Days();
		~Days();

		bool IsNullOfDays();

		// ����bydays
		wstring ByDays();

		void clear();

		// Summary  : �����ظ�����
		// Returns	: void
		// Parameter: RepeatDayType type 
		// Parameter: bool enable �Ƿ���
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

	// �ظ�����
	class CLASSINDLL_CLASS_DECL RepeatRule
	{
	public:
		RepeatRule(void);
		virtual ~RepeatRule(void);

		wstring	_frem;		// �ظ�����
		wstring	_weekstart;	// ÿ�ܿ�ʼ��ʼ�ڣ�һ��������
		wstring	_until;		// �ظ�����ʱ��  �����ʽ����yyyy-MM-dd HH:mm:ss
		__int64	_repeatEnd; // �ظ���������
		int		_interval;  // ���
		Days	_byDays;	// su,mo �����������õ�
		int		_byMonthday;// һ���µļ���
		wstring	_strByDay;	// �ַ����������������ݿ��
		int		_count;		// �ظ�����
	};

}

