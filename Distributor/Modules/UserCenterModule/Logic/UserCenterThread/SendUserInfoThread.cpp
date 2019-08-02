#include "stdafx.h"
#include "SendUserInfoThread.h"
#include "../UserCenterInfoMsg.h"
#include "Common/UserCenter/UserCenterMsg.h"


unsigned long SendUserInfoThread::Main()
{
	UbdgLibHandle handle = OpenHandle(1, _pData);
	RETURN_FALSE_IF (!handle);
	int re = SendCustomerData(handle);
	Close(handle);
	SAFE_DELETE(_pData);

	if (!re)
	{
		time_t now;
		time(&now);
		CDealTime ctime(now);
		WCHAR day[16] = {0};
		wsprintf(day, L"%dƒÍ%d‘¬%d»’", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() );
		wstring strDay = day;

		SendCustomerInfoMsg msg;
		msg.strSaveTime = strDay;
		msg.SendToUI();

		ResponseUserInfoMsg reMsg;
		reMsg.deviceID = L"";
		reMsg.lastTime = strDay;
		reMsg.SendToUI();

		return true;
	}
	else
	{
		SendCustomerInfoMsg msg;
		msg.SendToUI();
		return false;
	}
	
	return 0;
}

SendUserInfoThread::SendUserInfoThread( const string id )
:BaseThread(id)
{
	_pData			= NULL;
}

SendUserInfoThread::~SendUserInfoThread()
{
	_pData			= NULL;
}
