#pragma once

#include "Module/Alarm/AlarmData.h"
#include <deque>

namespace ND91Assistant
{
	// 闹钟管理类
	class CLASSINDLL_CLASS_DECL AlarmManager
	{
	public:
		AlarmManager(const DeviceData* pDevice);
		virtual ~AlarmManager(void);

		// Summary  : 获取闹钟
		// Returns	: void
		CommandExecuteResult LoadAlarms();

		// Summary  : 添加闹钟
		// Returns	: bool
		// Parameter: AlarmData * pNewAlarm，由外部释放
		AlarmData* AddAlarm(AlarmData* pNewAlarm);

		// Summary  : 删除闹钟
		// Returns	: bool
		// Parameter: AlarmData * pAlarm
		bool DeleteAlarm(AlarmData* pAlarm);

		// Summary  : 修改闹钟
		// Returns	: bool
		// Parameter: AlarmData * pAlarm
		bool UpdateAlarm(AlarmData* pAlarm);

		// Summary  : 闹钟列表
		// Returns	: const deque<AlarmData*>
		const deque<AlarmData*> GetAllAlarms(){return _alarmList;}

		// Summary  : 获取闹铃声列表
		// Returns	: const deque<wstring> 铃声名称
		const deque<wstring> GetAlarmRings();

		// Summary  : 获取闹钟当前铃声,在铃声列表中的索引，因为tittle不唯一，所以提供此接口定位当前铃声
		// Returns	: ND91Assistant::AlarmRing
		// Parameter: AlarmData * pAlarm
		int GetAlarmRing(AlarmData* pAlarm);

		// Summary  : 给闹钟设置铃声，此处无交互
		// Returns	: void 
		// Parameter: AlarmData * pAlarm 闹钟
		// Parameter: int ringIndex 铃声在闹钟铃声列表中的索引
		void SetAlarmRing(AlarmData* pAlarm, int ringIndex);

		// Summary  : 同步到手机,ios用
		// Returns	: void
		bool SyncToPhone();


		// Summary  : 是否支持闹钟开启
		// Returns	: bool
		bool IsSupportedAlarmOpen();

	private:
		// Summary  : 加载闹钟铃声
		// Returns	: void
		void LoadAlarmRings();

		// Summary  : 加载ios闹钟
		// Returns	: void
		bool LoadIosAlarms();

	private:

#pragma warning (disable:4251)

        deque<AlarmData*>     _alarmList;		// 闹钟列表
		deque<AlarmRing>      _alarmRinglist;	// 闹钟铃声

#pragma warning (default:4251)

		const DeviceData*     _pDevice;   

		int					  _version;
	};

}