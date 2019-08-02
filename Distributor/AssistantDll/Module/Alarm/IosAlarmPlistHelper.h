#pragma once

namespace ND91Assistant
{
	class AlarmData;

	class IosAlarmPlistHelper
	{
	public:
		IosAlarmPlistHelper(void);
		~IosAlarmPlistHelper(void);

		// Summary  : 解析闹钟的plist文件得到闹钟列表
		// Returns	: void
		// Parameter: wstring plistfilePath 闹钟plist文件
		// Parameter: deque<AlarmData * > & alarms 返回闹钟列表
		static void ParseAlarms(wstring plistfilePath, deque<AlarmData*>& alarms,bool ios4);

		// Summary  : 闹钟列表写回plist文件
		// Returns	: void
		// Parameter: wstring plistfilePath
		// Parameter: const deque<AlarmData * > alarms
		// Parameter: bool ios4
		static void WriteAlarmPlist(wstring plistfilePath, const deque<AlarmData*> alarms, bool ios4);
	};

}
