#pragma once

#include "Module/BaseData.h"

namespace ND91Assistant
{
	// 闹钟
	enum AlarmWeekDay
	{
		AlarmDay_Monday		= 1 ,// 星期一
		AlarmDay_Tuesday	= 1 << 1,// 星期二
		AlarmDay_Wednesday	= 1 << 2,// 星期三
		AlarmDay_Thursday	= 1 << 3,// 星期四
		AlarmDay_Friday		= 1 << 4,// 星期五
		AlarmDay_Saturday	= 1 << 5,// 星期六
		AlarmDay_Sunday		= 1 << 6,// 星期天
	};

	struct AlarmRing // 
	{
		wstring _ringUri;	// 铃声uri,对于ios对应GUID
		wstring _ringTittle;// 铃声名称
		wstring _ringPath;	// 铃声文件路径
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

		// Summary  : 设置星期几闹铃
		// Returns	: void
		// Parameter: AlarmWeekDay day 星期几
		// Parameter: bool weekup 是否闹
		void SetWeekdays(AlarmWeekDay day, bool weekup=true);

		// Summary  : 星期几是否闹铃
		// Returns	: bool
		// Parameter: AlarmWeekDay day 星期几
		bool IsDayWeekup(AlarmWeekDay day);

		// Summary  : 闹钟id
		// Returns	: int
		int  Id(){return _id;}

		wstring IosGuid(){return _ios_guid;}

		int		_hour;		// 时
		int		_minutes;	// 分
		__int64	_alarmtime;
		bool	_enabled;	// 是否开启
		bool	_vibrate;	// 是否震动
		wstring _message;	// 闹钟名称或提醒内容
		wstring _ringTittle;// 闹钟铃声名称
		bool    _ios_allowSnooze;//
		bool    _ios_repeat;//
        wstring	_ringuri;	// 闹钟铃声

        // 备份还原用
        int GetDaysOfWeek() {return _daysofweek; };
        void SetDaysOfWeek(int n) {_daysofweek = n; };
	private:
		int			_id;        // 闹钟id

		wstring     _ios_guid;

		int			_daysofweek;// 一周闹铃日

		bool		_isEdit;	//


	private:

		bool ParseAndroidData(BinaryBufferReader& reader);
		void AndroidDataToStream(BinaryBufferWriter& writer);
	};
}
