#include "stdafx.h"
#include "LoadDeviceSpaceThread.h"
#include "Module/System/SystemManager.h"

LoadDeviceSpaceThread::LoadDeviceSpaceThread( string threadID, int nRequestID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;    
	_nRequestID = nRequestID;
	_pInfo		= NULL;
}

LoadDeviceSpaceThread::~LoadDeviceSpaceThread()
{
}

void LoadDeviceSpaceThread::SetDeviceInfo( AblDeviceInfo* pInfo )
{
	_pInfo = pInfo;
}

unsigned long LoadDeviceSpaceThread::Main()
{
    this->LoadSpace();

	return 0;
}

bool LoadDeviceSpaceThread::LoadSpace()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (!pDevice || (!pDevice->IsActivate() && pDevice->GetDevInfo()->_deviceType == Android))
		return false;

	RETURN_VALUE_IF(CheckExit(), 0);

	if (pDevice->GetDevInfo()->_deviceType == Android)
	{
		ResponseDeviceSpaceMsg msg;
		msg.nOsType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;
		msg.deviceID = _deviceID;

		RETURN_VALUE_IF(CheckExit(), 0);

		ND91Assistant::SpaceInfo info;
		pDevice->GetDeviceSpaceInfo(info, true);
		
		RETURN_VALUE_IF(CheckExit(), 0);
		
		int nRetry = 0;
		while(nRetry<3 && info._totalsize <= 0)
		{
			nRetry++;
			pDevice->GetDeviceSpaceInfo(info, true);
			Sleep(500);
		}

		
		RETURN_VALUE_IF(CheckExit(), 0);

		msg.info.nLocation = eSpaceType_Location_SDCard;
		msg.info.nAll = info._totalsize;
		msg.info.nUsed = info._usedsize;
		msg.info.nLeft = info._totalsize - info._usedsize;

		msg.info.nSysAll = info._systemSize;
		msg.info.nSysUsed = info._systemUsed;
		msg.info.nSysLeft = info._systemAvailable;

		msg.info.nUserAll = info._systemSize - info._systemSize;
		msg.info.nUserUsed = info._systemUsed - info._systemUsed;
		msg.info.nUserLeft = info._systemAvailable - info._systemAvailable;

		RETURN_VALUE_IF(CheckExit(), 0);

		msg.PostToUI();
	}
	
	ResponseDeviceSpaceMsg msg;
	msg.nOsType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;
	msg.deviceID = _deviceID;

	RETURN_VALUE_IF(CheckExit(), 0);

	ND91Assistant::SpaceInfo info;
	pDevice->GetDeviceSpaceInfo(info, false);
	if(info._totalsize <= 0)
	{
		pDevice->GetDeviceSpaceInfo(info, false);
	}

	RETURN_VALUE_IF(CheckExit(), 0);

	msg.info.nLocation = eSpaceType_Location_Phone;
	msg.info.nAll = info._totalsize;
	msg.info.nUsed = info._usedsize;
	msg.info.nLeft = info._totalsize - info._usedsize;

	msg.info.nSysAll = info._systemSize;
	msg.info.nSysUsed = info._systemUsed;
	msg.info.nSysLeft = info._systemAvailable;

	msg.info.nUserAll = info._totalsize - info._systemSize;
	msg.info.nUserUsed = info._usedsize - info._systemUsed;
	msg.info.nUserLeft = msg.info.nUserAll - msg.info.nUserUsed;

	RETURN_VALUE_IF(CheckExit(), 0);

	msg.PostToUI();

	RETURN_VALUE_IF(CheckExit(), 0);

	ResponseSDCardStateMsg sdcardmsg;
	sdcardmsg.deviceID = this->_deviceID;
	sdcardmsg.bHaveSDCard = pDevice->GetSystemManager()->GetSDCardState() == MOUNTED;
	sdcardmsg.PostToUI();

	RETURN_VALUE_IF(CheckExit(), 0);

	ResponseChangeInstallPositionMsg posmsg;
	posmsg.deviceID = this->_deviceID;
	posmsg.bInstallOnPhone = _pInfo?_pInfo->bInstallOnPhone:true;
	posmsg.PostToUI();

	return true;
}