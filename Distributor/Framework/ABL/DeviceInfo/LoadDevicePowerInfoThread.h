#pragma once
#include "Common/DeviceBaseThread.h"

/*! @class
********************************************************************************
<PRE>
������   : LoadDevicePowerInfoThread
����     : ��ȡ�豸��Դ��Ϣ
�쳣��   :  
--------------------------------------------------------------------------------
��ע     : 
�����÷� : 
</PRE>
*******************************************************************************/
class LoadDevicePowerInfoThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
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