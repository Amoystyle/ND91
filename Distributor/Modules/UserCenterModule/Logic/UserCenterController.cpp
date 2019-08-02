#include "stdafx.h"
#include "UserCenterController.h"
#include "UI/MainView.h"

UserCenterController::UserCenterController()
{
	_pMainView = new MainView();
}

UserCenterController::~UserCenterController()
{

}

bool UserCenterController::Init( CContainerUI* pContainer )
{
	_pMainView->Init(pContainer);
	_pMainView->StatusChange(false, L"");
	return true;
}

void UserCenterController::Show()
{
	this->_pMainView->Show();
}

void UserCenterController::Hide()
{
	this->_pMainView->Hide();
}

