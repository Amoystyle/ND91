#include "stdafx.h"
#include "ApplicationController.h"

ApplicationController::ApplicationController(wstring deviceID, void* phone)
{
	this->_deviceID = deviceID;
	this->_nPhone = phone;

	_pAppInfoUI = new ApplicationInfoUI(Singleton::GetMainWnd(), deviceID);
}

ApplicationController::~ApplicationController()
{
LOG->WriteDebug(L"Begin--~ApplicationController");
 	CControlUI* pContainer = _pAppInfoUI->GetParent();
 	if(pContainer)
 		pContainer->Remove(_pAppInfoUI);
LOG->WriteDebug(L"End--~ApplicationController");
	//_pAppInfoUI->Release();
}

bool ApplicationController::Init( CContainerUI* pContainer )
{
	_pAppInfoUI->Init(pContainer);

	return true;
}

void ApplicationController::Show()
{
	_pAppInfoUI->Show();
}

void ApplicationController::Hide()
{
	_pAppInfoUI->Hide();
}

void ApplicationController::Release()
{
	//zjf待制作
}

std::wstring ApplicationController::GetDeviceID()
{
	return this->_deviceID;
}

bool ApplicationController::InstallApp(bool isFile, wstring wsTitle)
{
	return _pAppInfoUI->InstallApp(isFile, wsTitle);
}

bool ApplicationController::InstallApp( vector<wstring> files )
{
	return _pAppInfoUI->InstallApp(files);
}


bool ApplicationController::Handle(const NotifyInstallAppMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	if (_deviceID != pMsg->deviceID)
	{
		return true;
	}
	if (NotifyInstallAppType_SelectFiles == pMsg->nType)//选择文件安装操作
	{
		this->InstallApp(pMsg->bFile, pMsg->wsText);
	}
	else if(NotifyInstallAppType_InstallFiles == pMsg->nType)//拖拽操作
	{
		_pAppInfoUI->DropFilesHandle(pMsg->vetFiles);
	}
	return true;
}

bool ApplicationController::Handle(const AppThreadMsg* pMsg)
{

	_pAppInfoUI->HandleAppMsg(pMsg->message, pMsg->wParam, pMsg->lParam);

	return true;
}

bool ApplicationController::Handle(const NotifyCenterInfoMsg* pMsg)
{

	_pAppInfoUI->HandleCenterMsg(pMsg->nParam);

	return true;
}

bool ApplicationController::Handle(const NotifyTaskItemMsg* pMsg)
{
	_pAppInfoUI->HandleTaskItemMsg(pMsg->pTask, pMsg->message, pMsg->curID, pMsg->taskid);

	return true;
}

bool ApplicationController::Handle(const NotifyRefreshModuleMsg* pMsg)
{
	_pAppInfoUI->HandleRefreshModuleMsg(pMsg->deviceID);

	return true;
}