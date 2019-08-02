#pragma once

#include "Module/BaseCommand.h"
#include "Module/Alarm/AlarmData.h"

namespace ND91Assistant
{
    // 闹钟操作命令类    
    class AlarmOperationCommand :public BaseCommand
    {
    public:
	    AlarmOperationCommand(const DeviceData* pDevice);
	    virtual ~AlarmOperationCommand(void);

	    // Summary  : 获取闹钟
	    // Returns	: void
	    // Parameter: deque<AlarmData * > * pAlarms 存储获取的闹钟
	    void  LoadAlarms(deque<AlarmData*>* pAlarms);

	    // Summary  : 添加闹钟
	    // Returns	: bool
	    // Parameter: AlarmData * alarm 要添加的闹钟
	    bool  AddAlarm(AlarmData* alarm);

	    // Summary  : 删除闹钟
	    // Returns	: bool
	    // Parameter: AlarmData * alarm 要删除的闹钟
	    bool  DeleteAlarm(AlarmData* alarm);

	    // Summary  : 更新闹钟
	    // Returns	: bool
	    // Parameter: AlarmData * alarm 要更新的闹钟
	    bool  UpdateAlarm(AlarmData* alarm);

    private:

	    // from BaseCommand
	    void Execute();

	    // from BaseCommand
	    virtual std::string GetAndroidBodyData();

	    // from BaseCommand解析返回数据包
	    virtual void SetAndroidReturnData(std::string data);

	    // from BaseCommand
	    virtual void SetIosReturnFile(std::wstring filename){}


    private:

	    enum OperationType
	    {
		    Load_Alarms = 1,  // 获取闹钟
		    Add_Alarm	= 2,  // 添加闹钟
		    Delete_Alarm= 3,  // 删除闹钟 
		    Update_Alarm= 4	  // 更新闹钟
	    };

	    OperationType  _type;			// 操作类型

	    AlarmData*     _srcAlarm;		// 要操作的闹钟

	    deque<AlarmData*>*   _pAlarms;	// 存储加载的闹钟

    };

}

