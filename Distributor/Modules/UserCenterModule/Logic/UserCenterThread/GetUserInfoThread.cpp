#include "stdafx.h"
#include "GetUserInfoThread.h"
#include "../UserCenterInfoMsg.h"
#include "Common/UserCenter/UserCenterMsg.h"


unsigned long GetUserInfoThread::Main()
{
	Web web(1, _pData);
	UserData* pdata = web.GetCustomerInfoC(); 
	
	if ( !pdata )
	{
		if ( _bShowUserInfo )
		{
			GetCustomerInfoMsg msg;
			msg.userName	= L"";
			msg.phoneNum	= L"";
			msg.comment		= L"";
			msg.strSaveTime = L"";
			msg.SendToUI();
		}
		else
		{
			ResponseUserInfoMsg msg;
			msg.deviceID = _strDeviceID;
			msg.lastTime = L"";
			msg.SendToUI();
		}
		return 0;
	}

	wstring userName = CCodeOperation::UTF_8ToUnicode(pdata->_userName);
	wstring phoneNum = CCodeOperation::UTF_8ToUnicode(pdata->_phoneNumber);
	wstring comment = CCodeOperation::UTF_8ToUnicode(pdata->_comment);
	time_t tiem = pdata->_time;

	CDealTime ctime(tiem);
	WCHAR day[16] = {0};
	wsprintf(day, L"%dƒÍ%d‘¬%d»’", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() );
	wstring strSaveTime = day;

	SAFE_DELETE(_pData);

	if ( _bShowUserInfo )
	{
		GetCustomerInfoMsg msg;
		msg.userName	= userName;
		msg.phoneNum	= phoneNum;
		msg.comment		= comment;
		msg.strSaveTime = strSaveTime;
		msg.SendToUI();

	}
	else
	{
		ResponseUserInfoMsg msg;
		msg.deviceID = _strDeviceID;
		msg.lastTime = strSaveTime;
		msg.SendToUI();
		//dynamic_cast<DeviceInfoUI*>(_pInterface)->ChangeUserStatus(true, strSaveTime);
	}

	return 0;
}

GetUserInfoThread::GetUserInfoThread( const string id )
:BaseThread(id)
{
	_bShowUserInfo	= false;
	_strDeviceID	= L"";
	_pData			= NULL;
}

GetUserInfoThread::~GetUserInfoThread()
{
	_bShowUserInfo	= false;
	_strDeviceID	= L"";
	_pData			= NULL;
}
