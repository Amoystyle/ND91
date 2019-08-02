#pragma once

#include "Module/BaseCommand.h"
#include "Module/Alarm/AlarmData.h"

namespace ND91Assistant
{
    // ���Ӳ���������    
    class AlarmOperationCommand :public BaseCommand
    {
    public:
	    AlarmOperationCommand(const DeviceData* pDevice);
	    virtual ~AlarmOperationCommand(void);

	    // Summary  : ��ȡ����
	    // Returns	: void
	    // Parameter: deque<AlarmData * > * pAlarms �洢��ȡ������
	    void  LoadAlarms(deque<AlarmData*>* pAlarms);

	    // Summary  : �������
	    // Returns	: bool
	    // Parameter: AlarmData * alarm Ҫ��ӵ�����
	    bool  AddAlarm(AlarmData* alarm);

	    // Summary  : ɾ������
	    // Returns	: bool
	    // Parameter: AlarmData * alarm Ҫɾ��������
	    bool  DeleteAlarm(AlarmData* alarm);

	    // Summary  : ��������
	    // Returns	: bool
	    // Parameter: AlarmData * alarm Ҫ���µ�����
	    bool  UpdateAlarm(AlarmData* alarm);

    private:

	    // from BaseCommand
	    void Execute();

	    // from BaseCommand
	    virtual std::string GetAndroidBodyData();

	    // from BaseCommand�����������ݰ�
	    virtual void SetAndroidReturnData(std::string data);

	    // from BaseCommand
	    virtual void SetIosReturnFile(std::wstring filename){}


    private:

	    enum OperationType
	    {
		    Load_Alarms = 1,  // ��ȡ����
		    Add_Alarm	= 2,  // �������
		    Delete_Alarm= 3,  // ɾ������ 
		    Update_Alarm= 4	  // ��������
	    };

	    OperationType  _type;			// ��������

	    AlarmData*     _srcAlarm;		// Ҫ����������

	    deque<AlarmData*>*   _pAlarms;	// �洢���ص�����

    };

}

