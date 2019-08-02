#pragma once
#include "Common/DeviceBaseThread.h"

/*! @class
********************************************************************************
<PRE>
类名称   : LoadDevicePowerInfoThread
功能     : 获取设备电源信息
异常类   :  
--------------------------------------------------------------------------------
备注     : 
典型用法 : 
</PRE>
*******************************************************************************/
class LoadDevicePowerInfoThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    LoadDevicePowerInfoThread( string threadID, int nRequestID, wstring deviceID );
	~LoadDevicePowerInfoThread();

	void SetDeviceInfo(AblDeviceInfo* pInfo);

private:
	virtual unsigned long Main();

    bool GetPowerInfo();

private:
	AblDeviceInfo* _pInfo;
	int _nRequestID;
};