#include "stdafx.h"
#include "DeviceInfoController.h"
#include "UI/MainView.h"

DeviceInfoController::DeviceInfoController(wstring deviceID, void* phone)
{
	_deviceID = deviceID;
	_pPhone = phone;
	_bImgRrefreshing = false;
	_bInfoRefreshing = false;
	_bSpaceRefreshing = false;
	_bActiveRefreshing = false;
	_pMainView = new MainView(deviceID, phone);
}

DeviceInfoController::~DeviceInfoController()
{
	LOG->WriteDebug(L"DeviceInfoController Delete Begin: pPhone->"+ CStrOperation::IntToWString((UINT)_pPhone));
	CControlUI* pContainer = _pMainView->GetParent();
	if(pContainer)
		pContainer->Remove(_pMainView);

	_pMainView = NULL;
	LOG->WriteDebug(L"DeviceInfoController Delete End: pPhone->"+ CStrOperation::IntToWString((UINT)_pPhone));
}

bool DeviceInfoController::Init( CContainerUI* pContainer )
{
	_pMainView->Init(pContainer);

	return true;
}

void DeviceInfoController::Show()
{
	this->_pMainView->Show();
}

void DeviceInfoController::Hide()
{
	this->_pMainView->Hide();
}

bool DeviceInfoController::Handle( const NotifyRefreshModuleMsg* pMsg )
{
	if(_bImgRrefreshing || _bInfoRefreshing || _bActiveRefreshing)
		return true;

	if(!_bImgRrefreshing)
	{
		_bImgRrefreshing = true;

		RequestDeviceImageMsg imgMsg;
		imgMsg.deviceID = this->_deviceID;
		imgMsg.Send();
	}

	if(!_bInfoRefreshing)
	{
		_bInfoRefreshing = true;

		RequestDeviceInfoMsg infoMsg;
		infoMsg.deviceID = this->_deviceID;
		infoMsg.bRefresh = true;
		infoMsg.nType = RequestDeviceInfoType_All;
		infoMsg.Send();
	}

	if(!_bSpaceRefreshing)
	{
		RequestDeviceSpaceMsg spaceMsg;
		spaceMsg.deviceID = this->_deviceID;
		spaceMsg.Send();
	}

	RequestDeviceStateMsg stateMsg;
	stateMsg.deviceID = this->_deviceID;
	stateMsg.SendAndGetReturn();

	if(!stateMsg.bIsActive && _bActiveRefreshing)
	{
		RequestActiveDeviceMsg activeMsg;
		activeMsg.deviceID = this->_deviceID;
		activeMsg.Send();
	}

	return true;
}

bool DeviceInfoController::Handle( const ResponseDeviceImageMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	_bImgRrefreshing = false;
	return true;
}

bool DeviceInfoController::Handle( const NotifyLoadDeviceInfoFinishMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	_bInfoRefreshing = false;
	return true;
}

bool DeviceInfoController::Handle( const ResponseDeviceSpaceMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	_bSpaceRefreshing = false;
	return true;
}

bool DeviceInfoController::Handle( const DeviceActivatedMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	_bActiveRefreshing = false;
	return true;
}
