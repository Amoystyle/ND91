#pragma once

namespace ND91Assistant
{
	class AlarmData;

	class IosAlarmPlistHelper
	{
	public:
		IosAlarmPlistHelper(void);
		~IosAlarmPlistHelper(void);

		// Summary  : �������ӵ�plist�ļ��õ������б�
		// Returns	: void
		// Parameter: wstring plistfilePath ����plist�ļ�
		// Parameter: deque<AlarmData * > & alarms ���������б�
		static void ParseAlarms(wstring plistfilePath, deque<AlarmData*>& alarms,bool ios4);

		// Summary  : �����б�д��plist�ļ�
		// Returns	: void
		// Parameter: wstring plistfilePath
		// Parameter: const deque<AlarmData * > alarms
		// Parameter: bool ios4
		static void WriteAlarmPlist(wstring plistfilePath, const deque<AlarmData*> alarms, bool ios4);
	};

}
