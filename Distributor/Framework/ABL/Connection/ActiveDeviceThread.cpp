#include "stdafx.h"
#include "ActiveDeviceThread.h"

ActiveDeviceThread::ActiveDeviceThread( string threadID, int requestID, wstring deviceID, void* pPhone )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
	_nRequestID = requestID;
	_pPhone = pPhone;
}

ActiveDeviceThread::~ActiveDeviceThread()
{
}

unsigned long ActiveDeviceThread::Main()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(this->_deviceID);
	if (pDevice == NULL || pDevice->GetDevInfo() == NULL) 
		return 0;

	RETURN_VALUE_IF(CheckExit(), 0);

	DeviceActivatedMsg msg;
	msg.deviceID = _deviceID;
	msg.pPhone = _pPhone;
	msg.nPhoneOs = pDevice->GetDevInfo()->_deviceType == Android? ePhoneOS_Android:ePhoneOS_Ios;

	GetAblDeviceInfoMsg ablMsg;
	ablMsg.deviceID = _deviceID;
	ablMsg.SendAndGetReturn();
	AblDeviceInfo* ablData = new AblDeviceInfo(&ablMsg.info);
	ablData->deviceID = _deviceID;
	RETURN_FALSE_IF ( !ablData );

	CStaticsticsDevice msgStatistics;
	msgStatistics.pAblDeviceInfo = (void*)ablData;
	msgStatistics.Send();	

	bool bActive = pDevice->IsActivate();
	try
	{
		if(!bActive)
		{
			bActive = pDevice->Activate();

			if(msg.nPhoneOs == ePhoneOS_Android)
			{
				pDevice->AndroidReadDeviceInfo();
				pDevice->AndroidReadDeviceOtherInfo();
			}
		}
	}
	catch (...)
	{
	}

	RETURN_VALUE_IF(CheckExit(), 0);

	wstring deviceName = pDevice->GetDevInfo()->GetDeviceName();
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->_deviceHWModel;
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->_strSerialNumberForShow;
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->_strSerialNumber;

	msg.deviceName = deviceName;
	msg.bActived = bActive;

	RETURN_VALUE_IF(CheckExit(), 0);

	msg.SendToUI();

	if(!msg.bActived)
	{
		Singleton::GetThreadManager()->ExitThreads(this->_deviceID);
	}

	return 0;
}