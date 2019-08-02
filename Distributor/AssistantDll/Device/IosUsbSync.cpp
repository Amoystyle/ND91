#include "stdafx.h"

#include "IosUsbSync.h"

#ifdef WIN32
    #include "Device/ITunesMobileDevice.h"
#else
    #include "Device/MobileDevice.h"
#endif

#define SYNCWILLSTART	"com.apple.itunes-mobdev.syncWillStart"
#define SYNCLOCKREQUEST "com.apple.itunes-mobdev.syncLockRequest"
#define SYNCDIDFINISH	"com.apple.itunes-mobdev.syncDidFinish"
#define SYNCDIDSTART	"com.apple.itunes-mobdev.syncDidStart"
#define NOTICE_PROXY	"com.apple.mobile.notification_proxy"
#define SYNCLOCKPATH	L"/com.apple.itunes.lock_sync"


IosUsbSync::IosUsbSync( const DeviceInfo* pDevInfo )
{	
	_pDevInfo		= pDevInfo;
	_socketHandle	= NULL;
	_isRuning		= false;
	_ref			= 0;
}

IosUsbSync::~IosUsbSync(void)
{

}

bool ND91Assistant::IosUsbSync::StartSync()
{
	if ( SendNotification( SYNCWILLSTART ) )
	{
		try
		{
			afc_error_t ret = CITunesMobileDevice::iTunes_AFCFileRefOpen(_pDevInfo->_ios_afcHandle, SYNCLOCKPATH, 3, &_ref);
			if (ret)
			{
				if ( !SendNotification( SYNCLOCKREQUEST ) )
				{
					CITunesMobileDevice::iTunes_AFCFileRefClose(_pDevInfo->_ios_afcHandle, _ref);
					SendNotification( SYNCDIDFINISH );
					CloseNotification();
					return false;
				}
				SendNotification( SYNCDIDSTART );
				CloseNotification();
			}
		}
		catch (...)
		{
			LOG->WriteError(L"StartSync  Faild");
			SendNotification( SYNCDIDFINISH );
			return false;
		}
	}

	return true;
}

void ND91Assistant::IosUsbSync::StopSync()
{

	CITunesMobileDevice::iTunes_AFCFileRefClose(_pDevInfo->_ios_afcHandle, _ref);
	SendNotification( SYNCDIDFINISH );
	CloseNotification();

}

bool ND91Assistant::IosUsbSync::SendNotification( std::string notification )
{
	std::map<string, string> msgDict;
	msgDict["Command"]	= "PostNotification";
	msgDict["Name"]		= notification;

	RETURN_FALSE_IF ( !StartNotification() );

	IosUsbConnection iosConet((DeviceInfo*)_pDevInfo);

	RETURN_FALSE_IF ( !iosConet.SendMessageToService(_socketHandle, msgDict) );

	return true;

}

bool ND91Assistant::IosUsbSync::StartNotification()
{
	bool result = false;

	RETURN_VALUE_IF ( _isRuning == true, true );

	int ret = 0;

	IosUsbConnection iosConet( (DeviceInfo*)_pDevInfo );
	ret = iosConet.StartIOSService( (char*)NOTICE_PROXY , &_socketHandle);

	if ( ret != 0 )
		result = false;
	else
		result = true;
	
	_isRuning = result;

	return result;
}

bool ND91Assistant::IosUsbSync::CloseNotification()
{
	bool result = false;

	std::map<string, string> msgDict;
	msgDict["Command"]	= "Shutdown";

	RETURN_FALSE_IF ( !StartNotification() );

	IosUsbConnection iosConet( (DeviceInfo*)_pDevInfo );

	if ( !iosConet.SendMessageToService( _socketHandle, msgDict) )
	{
		LOG->WriteError(L"Can't send Shudown comman 2 np service");
	}
	else
	{
		std::map<string, string>  replyInfo;
		iosConet.ReadMssageFromService( _socketHandle, replyInfo );

		std::string cmdValue;

		std::map<string, string>::iterator it = replyInfo.find("Command");
		
		if ( it != replyInfo.end() )
		{
			cmdValue = it->second;
			if (cmdValue == "ProxyDeath")
			{
				result		= true;
				_isRuning	= false;
			}
			else
			{
				LOG->WriteError( CCodeOperation::GB2312ToUnicode(cmdValue) );
			}
		}
	}

	return result;
}
