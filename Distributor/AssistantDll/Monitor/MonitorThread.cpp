#include "stdafx.h"
#include "MonitorThread.h"
#include "Core/DeviceManager.h"

MonitorThread::MonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id)
:_pDeviceInfo(pDeviceInfo)
,_id(id)
{
    _bThreadQuitSign = true;
    _hThread = NULL;
    _nSilenceTime = 0;

}

MonitorThread::~MonitorThread()
{

}

void MonitorThread::onDisConnectDevice()
{
    DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(_id);
    RETURN_IF( ! pDeviceData);

    // 假如连接成功，重新激活设备
    if ( pDeviceData->Connect() )
    {
        try
        {
			//connect可能导致_id被破坏，原因不明
            LOG->WriteDebug(L"重新连接并激活设备"); 
            pDeviceData->SetActive(false);
			pDeviceData->ResetConnect();
            pDeviceData->Activate();
        }
        catch(...)
        {
    //        DEVICE_MANAGER->OnDeviceDisconnect(_id);
        }
    }
    else
    {
        DEVICE_MANAGER->OnDeviceDisconnect(_id);
    }
}

void MonitorThread::onDaemonDisable()
{
	DEVICE_MANAGER->DaemonDisable(_id);
}