#pragma once

#include "Module/Alarm/AlarmData.h"
#include <deque>

namespace ND91Assistant
{
	// ���ӹ�����
	class CLASSINDLL_CLASS_DECL AlarmManager
	{
	public:
		AlarmManager(const DeviceData* pDevice);
		virtual ~AlarmManager(void);

		// Summary  : ��ȡ����
		// Returns	: void
		CommandExecuteResult LoadAlarms();

		// Summary  : �������
		// Returns	: bool
		// Parameter: AlarmData * pNewAlarm�����ⲿ�ͷ�
		AlarmData* AddAlarm(AlarmData* pNewAlarm);

		// Summary  : ɾ������
		// Returns	: bool
		// Parameter: AlarmData * pAlarm
		bool DeleteAlarm(AlarmData* pAlarm);

		// Summary  : �޸�����
		// Returns	: bool
		// Parameter: AlarmData * pAlarm
		bool UpdateAlarm(AlarmData* pAlarm);

		// Summary  : �����б�
		// Returns	: const deque<AlarmData*>
		const deque<AlarmData*> GetAllAlarms(){return _alarmList;}

		// Summary  : ��ȡ�������б�
		// Returns	: const deque<wstring> ��������
		const deque<wstring> GetAlarmRings();

		// Summary  : ��ȡ���ӵ�ǰ����,�������б��е���������Ϊtittle��Ψһ�������ṩ�˽ӿڶ�λ��ǰ����
		// Returns	: ND91Assistant::AlarmRing
		// Parameter: AlarmData * pAlarm
		int GetAlarmRing(AlarmData* pAlarm);

		// Summary  : �����������������˴��޽���
		// Returns	: void 
		// Parameter: AlarmData * pAlarm ����
		// Parameter: int ringIndex ���������������б��е�����
		void SetAlarmRing(AlarmData* pAlarm, int ringIndex);

		// Summary  : ͬ�����ֻ�,ios��
		// Returns	: void
		bool SyncToPhone();


		// Summary  : �Ƿ�֧�����ӿ���
		// Returns	: bool
		bool IsSupportedAlarmOpen();

	private:
		// Summary  : ������������
		// Returns	: void
		void LoadAlarmRings();

		// Summary  : ����ios����
		// Returns	: void
		bool LoadIosAlarms();

	private:

#pragma warning (disable:4251)

        deque<AlarmData*>     _alarmList;		// �����б�
		deque<AlarmRing>      _alarmRinglist;	// ��������

#pragma warning (default:4251)

		const DeviceData*     _pDevice;   

		int					  _version;
	};

}