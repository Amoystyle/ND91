#pragma once

#include "Module/BaseData.h"

namespace ND91Assistant
{
	// ����
	enum AlarmWeekDay
	{
		AlarmDay_Monday		= 1 ,// ����һ
		AlarmDay_Tuesday	= 1 << 1,// ���ڶ�
		AlarmDay_Wednesday	= 1 << 2,// ������
		AlarmDay_Thursday	= 1 << 3,// ������
		AlarmDay_Friday		= 1 << 4,// ������
		AlarmDay_Saturday	= 1 << 5,// ������
		AlarmDay_Sunday		= 1 << 6,// ������
	};

	struct AlarmRing // 
	{
		wstring _ringUri;	// ����uri,����ios��ӦGUID
		wstring _ringTittle;// ��������
		wstring _ringPath;	// �����ļ�·��
		bool    _IsSystem;
	};

	class BinaryBufferReader;
	class BinaryBufferWriter;
	class CLASSINDLL_CLASS_DECL AlarmData : public BaseData
	{
 		friend class AlarmOperationCommand;
 		friend class AlarmManager;
 		friend class IosAlarmPlistHelper;
	public:
		AlarmData(void);
		virtual ~AlarmData(void);

		AlarmData* Clone();

		// Summary  : �������ڼ�����
		// Returns	: void
		// Parameter: AlarmWeekDay day ���ڼ�
		// Parameter: bool weekup �Ƿ���
		void SetWeekdays(AlarmWeekDay day, bool weekup=true);

		// Summary  : ���ڼ��Ƿ�����
		// Returns	: bool
		// Parameter: AlarmWeekDay day ���ڼ�
		bool IsDayWeekup(AlarmWeekDay day);

		// Summary  : ����id
		// Returns	: int
		int  Id(){return _id;}

		wstring IosGuid(){return _ios_guid;}

		int		_hour;		// ʱ
		int		_minutes;	// ��
		__int64	_alarmtime;
		bool	_enabled;	// �Ƿ���
		bool	_vibrate;	// �Ƿ���
		wstring _message;	// �������ƻ���������
		wstring _ringTittle;// ������������
		bool    _ios_allowSnooze;//
		bool    _ios_repeat;//
        wstring	_ringuri;	// ��������

        // ���ݻ�ԭ��
        int GetDaysOfWeek() {return _daysofweek; };
        void SetDaysOfWeek(int n) {_daysofweek = n; };
	private:
		int			_id;        // ����id

		wstring     _ios_guid;

		int			_daysofweek;// һ��������

		bool		_isEdit;	//


	private:

		bool ParseAndroidData(BinaryBufferReader& reader);
		void AndroidDataToStream(BinaryBufferWriter& writer);
	};
}
