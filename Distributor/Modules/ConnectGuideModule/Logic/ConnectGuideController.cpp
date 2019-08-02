#include "stdafx.h"
#include "ConnectGuideController.h"
#include "UI/MainView.h"

ConnectGuideController::ConnectGuideController(wstring deviceID, void* phone)
{
	_deviceID = deviceID;
	_pPhone = phone;
	_pMainView = new MainView(phone);
}

ConnectGuideController::~ConnectGuideController()
{
	LOG->WriteDebug(L"ConnectGuideController Delete Begin: pPhone->"+ CStrOperation::IntToWString((UINT)_pPhone));
	CControlUI* pContainer = _pMainView->GetParent();
	if(pContainer)
		pContainer->Remove(_pMainView);
	
	_pMainView = NULL;
	LOG->WriteDebug(L"ConnectGuideController Delete End: pPhone->"+ CStrOperation::IntToWString((UINT)_pPhone));
}

bool ConnectGuideController::Init( CContainerUI* pContainer )
{
	_pMainView->Init(pContainer);
	
	return true;
}

void ConnectGuideController::Show()
{
	this->_pMainView->Show();
}

void ConnectGuideController::Hide()
{
	this->_pMainView->Hide();
}

bool ConnectGuideController::Handle( const DeviceConnectInitMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

	this->_pMainView->SetStep(ConnectGuideStep_ConnectInit);
	return true;
}

bool ConnectGuideController::Handle( const DeviceConnectWaitWindowsMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

	this->_pMainView->SetStep(ConnectGuideStep_ConnectWaitWindow);

	return true;
}

bool ConnectGuideController::Handle( const DeviceDriverInstallInitMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

 	this->_pMainView->SetStep(ConnectGuideStep_DriverInstall);

 	this->_pMainView->OnDriverInstallInit();
	return true;
}

bool ConnectGuideController::Handle( const DeviceDriverInstallProgressMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

	this->_pMainView->SetStep(ConnectGuideStep_DriverInstall);
	this->_pMainView->OnDriverProgress(pMsg->nState, pMsg->nStep, pMsg->nProgress);

	return true;
}

bool ConnectGuideController::Handle( const DeviceConnectMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

	this->_pMainView->SetStep(ConnectGuideStep_Connecting);
	return true;
}

bool ConnectGuideController::Handle( const DeviceConnectErrorMsg* pMsg )
{
	if(pMsg->pPhone != _pPhone)
		return true;

	this->_pMainView->SetStep(ConnectGuideStep_Error);
	this->_pMainView->OnError(pMsg->nError);
	return true;
}