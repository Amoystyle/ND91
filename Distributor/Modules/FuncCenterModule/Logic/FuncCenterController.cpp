#include "stdafx.h"
#include "FuncCenterController.h"
#include "UI/MainView.h"

FuncCenterController::FuncCenterController(wstring deviceID, void* phone)
{
	this->_deviceID = deviceID;
	this->_nPhone = phone;

	_pMainView = new MainView(deviceID, phone);
}

FuncCenterController::~FuncCenterController()
{
	CControlUI* pContainer = _pMainView->GetParent();
	if(pContainer)
		pContainer->Remove(_pMainView);

	_pMainView = NULL;
}

bool FuncCenterController::Init( CContainerUI* pContainer )
{
	_pMainView->Init(pContainer);

	return true;
}

void FuncCenterController::Show()
{
	this->_pMainView->Show();
}

void FuncCenterController::Hide()
{
	this->_pMainView->Hide();
}

void FuncCenterController::Release()
{
}