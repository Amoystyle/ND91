#include "stdafx.h"
#include "RecentInstallCaseController.h"

RecentInstallCaseController::RecentInstallCaseController(wstring deviceID)
{
	_pDevicePanelCaselistUI = new DevicePanelCaselistUI();
	_pDevicePanelCaselistUI->SetCurrentDeviceID(deviceID);
}

RecentInstallCaseController::~RecentInstallCaseController()
{
LOG->WriteDebug(L"Begin--~RecentInstallCaseController");
	CControlUI* pContainer = _pDevicePanelCaselistUI->GetParent();
	if(pContainer)
		pContainer->Remove(_pDevicePanelCaselistUI);
LOG->WriteDebug(L"End--~RecentInstallCaseController");
}

bool RecentInstallCaseController::Init( CContainerUI* pContainer )
{
	_pDevicePanelCaselistUI->Init(pContainer);

	return true;
}

void RecentInstallCaseController::Show()
{
	this->_pDevicePanelCaselistUI->Show();
}

void RecentInstallCaseController::Hide()
{
	this->_pDevicePanelCaselistUI->Hide();
}

