#include "stdafx.h"
#include "FileManagerController.h"
#include "UI/MainView.h"

FileManagerController::FileManagerController(wstring deviceId)
{
	_pMainView = new MainView(deviceId);
}

FileManagerController::~FileManagerController()
{
//#ifdef _DEBUG
	LOG->WriteDebug(_T("文件管理remove  begin"));
	CControlUI* pContainer = _pMainView->GetParent();
	if(pContainer)
		pContainer->Remove(_pMainView);
//#endif
	LOG->WriteDebug(_T("文件管理remove  end"));
	_pMainView = NULL;
}

bool FileManagerController::Init( CContainerUI* pContainer )
{
	_pMainView->Init(pContainer);

	return true;
}

void FileManagerController::Show()
{
	this->_pMainView->Show();
}

void FileManagerController::Hide()
{
	this->_pMainView->Hide();
}


bool FileManagerController::Handle( const InitRootMsg* pMsg )
{
	_pMainView->InitRootMsg_Handle(pMsg);
	return true;
}

bool FileManagerController::Handle( const RefreshNodeMsg* pMsg )
{
	_pMainView->RefreshNodeMsg_Handle(pMsg);
	return true;
}

bool FileManagerController::Handle( const RefreshAppMsg* pMsg )
{
	_pMainView->RefreshAppMsg_Handle(pMsg);
	return true;
}

bool FileManagerController::Handle( const DragdropFileMsg* pMsg )
{
	_pMainView->DragdropFileMsg_Handle(pMsg);
	return true;
}
