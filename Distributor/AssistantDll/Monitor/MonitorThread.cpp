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

    // �������ӳɹ������¼����豸
    if ( pDeviceData->Connect() )
    {
        try
        {
			//connect���ܵ���_id���ƻ���ԭ����
            LOG->WriteDebug(L"�������Ӳ������豸"); 
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