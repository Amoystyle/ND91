#pragma once

#include "RepeatRule.h"

namespace ND91Assistant
{

	enum RepeatTypes
	{
		None = 0, 			// ���ظ�(0)

		EveryDay = 11,		// ÿ���ظ�(11)

		EveryWeek = 21,		// ÿ���ظ�(21)

		Every2Weeks = 22,	// ÿ�����ظ�(22)

		EveryMonth = 31,	// ÿ���ظ�(31)

		EveryYear = 41		// ÿ���ظ�(41)
	};

    //class BinaryBufferReader;
    //class BinaryBufferWriter;
    class ReminderData;

    // �ճ̱��¼�
    class CLASSINDLL_CLASS_DECL CalendarEventData : public BaseData
    {
    public:
	    CalendarEventData(void);
	    virtual ~CalendarEventData(void);

	    CalendarEventData* Clone();

	    void ConvertFrequency();

	    void UpdateReminders(int alertBeforeMinutes);

	    int			_rowid;				// �¼�Id
	    int			_calendarId;		// ����ID,calendar���е�_id
	    int			_instanceId;		// instance���е�_id
	    wstring		_summary;			// �¼�����
	    wstring		_location;			// �ص�
	    int			_endSecond;
	    __int64		_start;				// ��ʼʱ��instance begin
	    int			_startSecond;
	    bool		_hasChange;			// �Ƿ�ı�
	    wstring		_specifier;
	    __int64		_alarmTime;			// ��������ʱ��
	    int			_weekStart;			// (week_start)
	    bool		_alert ;			// �Ƿ�����
	    int			_alertBefore;		// ����ʱ��ǰ����
        int         _reAlertBefore;     // �ٴ�����ʱ��ǰ������(-1:������)
	    wstring		_description;		// ����
	    __int64		_eventdtend;		// event �϶�Ӧ�Ľ���ʱ��
	    __int64		_eventdtstart;		// event �ϵĿ�ʼʱ��
	    __int64		_originalInstanceTime;// ԭʼ�¼�ʵ��ʱ��
	    __int64		_lastDate;			// ��ֹ����
	    RepeatTypes _repeatType;		// �ظ���ʽ �͵ķ�ʽ
	    bool		_allDay;			// �Ƿ�Ϊȫ���¼�
	    int			_repeatCount;		// �ظ�����
	    __int64		_endRepeatDate;		// ֹͣ�ظ�����
	    int			_origEventId;		// �����¼�ID
	    wstring		_originalEvent;		// ԭʼ�¼�
	    RepeatRule	_repeatRlue;		// �ظ�������
	    __int64		_origStartDate;		// �����¼���ʼʱ��
	    int			_by_Month_Months;
	    wstring		_timeZone;			// ʱ��
	    bool		_repeatForever;		// �Ƿ������ظ�
	    wstring		_syncId;			// ��Google������ͬ����id
	    int			_eventStatus;		// �¼�״̬
	    wstring		_duration;			// ���ʱ�䣨��ʼ��������p3600s p2d
	    __int64		_instaceBegin;		// ʵ����ʼ�¼�
	    __int64		_instaceEnd;		// ʵ������ʱ��

	    __int64		_end;				// ����ʱ�� instance end

        __int64     _occurrenceDay;     // �¼�����ʱ��
        wstring     _exceptionDate;     // �쳣�¼�ԭ������ʱ�䣬ͳһ����0�㣬�ڵ����ʱ��iPhoneƽ̨�����㵽����ԭ�¼�����ʱ��,�ö��ŷָ�yyyy-MM-dd
        long        _interval;          // ���ڼ�� 1����1�ܣ�2����2��

	    deque<ReminderData*>  _reminders;// �¼�����

	    int			_lephoneCategory;	// �Ƿ���phone
	    wstring		_phoneCategory;		// �ֻ�����
	    wstring		_exdate;			// ����ר�ã����»�ȡ���¼����ڴ�

	    map<int, CalendarEventData*>  _cancelEventList;// ȡ���¼�
	    map<int, CalendarEventData*>  _updateEventList;// �����¼�

	    //void ParseAndroidData(BinaryBufferReader& reader, bool IsNew = false);

	    //void AndroidDataToStream(BinaryBufferWriter& writer);

    };

}
