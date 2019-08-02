#include "stdafx.h"
#include "NeedInstallThread.h"
#include "../CheckCanInstallAppsync.h"

NeedInstallThread::NeedInstallThread( string threadID, wstring deviceID )
						: BaseThread(threadID)
{

	_deviceID = deviceID;
	this->_deviceExit = false;
    _autoDelete = true;
}

NeedInstallThread::~NeedInstallThread()
{
}

unsigned long NeedInstallThread::Main()
{
	RequestDeviceTypeMsg typeMsg;
	typeMsg.deviceID = this->_deviceID;
	typeMsg.SendAndGetReturn();

	if (typeMsg.deviceType != ePhoneOS_Ios)
		return 0;


	RequestDeviceStateMsg stateMsg;
	stateMsg.deviceID = this->_deviceID;
	stateMsg.SendAndGetReturn();

	RETURN_VALUE_IF(CheckExit(), 0);

	if(stateMsg.bIsJailBreak && stateMsg.deviceProductVersion >= L"6.0")
	{
		ResponseNeedInstallAppSyncMsg msg;
		msg.deviceID = this->_deviceID;
		msg.bNeedInstall = CheckCanInstallAppsync::CanInstall(_deviceID);		//获取是否安装appasync
		msg.PostToUI();
	}
	return 0;
}

bool NeedInstallThread::CheckExit()
{
	return __super::CheckExit() || this->_deviceExit;
}
