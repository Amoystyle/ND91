#include "stdafx.h"
#include "SaveUserInfoModel.h"
#include "LoginModel.h"
#include "DeviceDataHelp.h"
#include "UbdgLib.h"
#include "UserCenterThread/GetUserInfoThread.h"
#include "UserCenterThread/SendUserInfoThread.h"
#include "UserCenterInfoMsg.h"
#include "Common/Device/DeviceMessage.h"
#include "Common/Device/AblDeviceInfo.h"


SaveUserInfoModel::SaveUserInfoModel()
{
	_bIsPad		= false;
	_strDeviceID= L"";
}	

SaveUserInfoModel::~SaveUserInfoModel()
{
	_bIsPad		= false;
	_strDeviceID= L"";	

	ReleaseData();

}

bool SaveUserInfoModel::SendUserInfo(wstring name, wstring userTel, wstring comment)
{
	AblDeviceInfo* ablData = _mapDeviceData[_strDeviceID];

	RETURN_FALSE_IF( !ablData );
	DeviceDataHelp help(ablData);

	CUSTOMER_DATA *pData = new CUSTOMER_DATA;
	pData->userid = _wtol(LoginModel::GetInstance()->GetUserID().c_str());
	pData->type   = help.GetDeviceType();
	std::string imei = help.GetTrueImei();
	memset( pData->imei, '\0', 100 );
	memcpy(pData->imei, imei.c_str(), imei.length() );
	memset( pData->imsi, '\0', 100 );
	memcpy(pData->imsi, CCodeOperation::UnicodeToUTF_8(ablData->baseInfo.deviceIMSI).c_str(), ablData->baseInfo.deviceIMSI.length() );
	memset( pData->userName, '\0', 50 );
	memcpy(pData->userName, CCodeOperation::UnicodeToUTF_8(name).c_str(), 50);
	memset( pData->phoneNumber, '\0', 50 );
	memcpy(pData->phoneNumber, CCodeOperation::UnicodeToUTF_8(userTel).c_str(), userTel.length() );
	memset( pData->comment, '\0', 100 );
	memcpy(pData->comment, CCodeOperation::UnicodeToUTF_8(comment).c_str(), 100);
	memset( pData->mobileVersion, '\0', 50 );
	memcpy(pData->mobileVersion, CCodeOperation::UnicodeToUTF_8(ablData->baseInfo.deviceProductVersion).c_str(), ablData->baseInfo.deviceProductVersion.length() );
	memset( pData->mobileModel, '\0', 50 );
	if ( ablData->baseInfo.nOsType == ePhoneOS_Ios )
		memcpy(pData->mobileModel, CCodeOperation::UnicodeToUTF_8(ablData->baseInfo.deviceProductType).c_str(), ablData->baseInfo.deviceProductType.length() );
	else
		memcpy(pData->mobileModel, CCodeOperation::UnicodeToUTF_8(ablData->baseInfo.deviceHWModel).c_str(), ablData->baseInfo.deviceHWModel.length() );
	pData->jailbroken = help.GetJailbroken();

	SendUserInfoThread *pThread = new SendUserInfoThread("");
	pThread->_pData = pData;
	pThread->Run();

	return true;
}

bool SaveUserInfoModel::Handle( const RequestUserInfoMsg* pMsg )
{
	GetAblDeviceInfoMsg ablMsg;
	ablMsg.deviceID = pMsg->deviceID;
	ablMsg.SendAndGetReturn();
	AblDeviceInfo* ablData = new AblDeviceInfo(&ablMsg.info);
	ablData->deviceID = pMsg->deviceID;
	_mapDeviceData[pMsg->deviceID] = ablData;

	if ( !LoginModel::GetInstance()->GetBeLogin() )
	{
		ResponseUserInfoMsg msg;
		msg.SendToUI();
	}
	else
	{
		RETURN_FALSE_IF ( !ablData );
		DeviceDataHelp help(ablData);

		CUSTOMER_DATA *pData = new CUSTOMER_DATA;
		pData->userid = _wtol(LoginModel::GetInstance()->GetUserID().c_str());
		pData->type   = help.GetDeviceType();
		std::string imei = help.GetTrueImei();
		memset( pData->imei, '\0', 100 );
		memcpy(pData->imei, imei.c_str(), imei.length() );

		GetUserInfoThread *pThread = new GetUserInfoThread("");
		pThread->_bShowUserInfo = false;
		pThread->_strDeviceID	= pMsg->deviceID;
		pThread->_pData			= pData;
		pThread->Run();
	}
	
	return true;
		
}

bool SaveUserInfoModel::Handle( const RequestSaveUserInfoMsg* pMsg )
{
	if ( !LoginModel::GetInstance()->GetBeLogin() )
	{
		GetCustomerInfoMsg msg;
		msg.SendToUI();
	}
	else
	{
		AblDeviceInfo* ablData = _mapDeviceData[pMsg->deviceID];

		RETURN_FALSE_IF ( !ablData );
		DeviceDataHelp help(ablData);

		CUSTOMER_DATA *pData = new CUSTOMER_DATA;
		pData->userid = _wtol(LoginModel::GetInstance()->GetUserID().c_str());
		pData->type   = help.GetDeviceType();
		std::string imei = help.GetTrueImei();
		memset( pData->imei, '\0', 100 );
		memcpy(pData->imei, imei.c_str(), imei.length() );

		_strDeviceID = pMsg->deviceID;
		_bIsPad		 = ablData->baseInfo.nPlatForm == ePhonePlatForm_iPad;
		
		RequestDeviceImageMsg msg;
		msg.deviceID = pMsg->deviceID;
		msg.bRefresh = true;
		msg.Send();

		GetUserInfoThread *pThread = new GetUserInfoThread("");
		pThread->_bShowUserInfo = true;
		pThread->_strDeviceID	= pMsg->deviceID;
		pThread->_pData			= pData;
		pThread->Run();
	}
	return true;
}

bool SaveUserInfoModel::Handle( const ResponseDeviceImageMsg* pMsg )
{
	_mapImgPath[pMsg->deviceID] = pMsg->imagePath;
	return true;
}

bool SaveUserInfoModel::Handle( GetUserLoginInfoMsg* pMsg )
{
	pMsg->userId	= LoginModel::GetInstance()->GetUserID();
	pMsg->bLongin	= LoginModel::GetInstance()->GetBeLogin();
	pMsg->bCpaUser	= LoginModel::GetInstance()->GetBeCpaUser() == 1;
	pMsg->channelID = LoginModel::GetInstance()->GetChannelID();
	return true;
}

bool SaveUserInfoModel::GetImgPath( wstring& strPath, bool& bIspad )
{
	strPath = _mapImgPath[_strDeviceID];
	bIspad	= _bIsPad;

	return true;
}

void SaveUserInfoModel::ReleaseData()
{
	map<wstring, AblDeviceInfo*>::iterator it;
	for ( it=_mapDeviceData.begin();it!=_mapDeviceData.end();it++ )
	{
		AblDeviceInfo* pInfo= it->second;
		SAFE_DELETE(pInfo);
	}
}