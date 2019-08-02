#include "stdafx.h"
#include "LoadResourceThread.h"

LoadResourceThread::LoadResourceThread(string threadId) : BaseThread(threadId)
{
}

LoadResourceThread::~LoadResourceThread()
{

}

unsigned long LoadResourceThread::Main()
{
	this->LoadResource();

	ModuleManagerInnerMsg msg;
	msg.nAction = ModuleManager_Action_LoadResourceCompleted;
	msg.SendToUI();


	return 0;
}


void LoadResourceThread::LoadResource()
{
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	TCHAR *pLastBias = NULL;
	if (pLastBias = _tcsrchr(szPath, '\\'))
		*(pLastBias + 1) = '\0';

	tstring commonUIPath = szPath;
	commonUIPath += _T("Resource\\CommonUI\\");
	GetUIRes()->InitResDir(commonUIPath.c_str(),false);//CommonUI

	tstring moduleManagerPath = szPath;
	moduleManagerPath += _T("Resource\\ModuleManager\\");
	GetUIRes()->InitResDir(moduleManagerPath.c_str());

	tstring deviceModulePath = szPath;
	deviceModulePath += _T("Resource\\DeviceCenterModule\\");
	GetUIRes()->InitResDir(deviceModulePath.c_str());

	tstring connectGuideModulePath = szPath;
	connectGuideModulePath += _T("Resource\\ConnectGuideModule\\");
	GetUIRes()->InitResDir(connectGuideModulePath.c_str());

	tstring deviceInfoModulePath = szPath;
	deviceInfoModulePath += _T("Resource\\DeviceInfoModule\\");
	GetUIRes()->InitResDir(deviceInfoModulePath.c_str());

	tstring applicationModulePath = szPath;
	applicationModulePath += _T("Resource\\ApplicationModule\\");
	GetUIRes()->InitResDir(applicationModulePath.c_str());

	tstring fileManagerModulePath = szPath;
	fileManagerModulePath += _T("Resource\\FileManagerModule\\");
	GetUIRes()->InitResDir(fileManagerModulePath.c_str());

	tstring backupAndRestoreModulePath = szPath;
	backupAndRestoreModulePath += _T("Resource\\BackupAndRestoreModule\\");
	GetUIRes()->InitResDir(backupAndRestoreModulePath.c_str());

	tstring recentInstallCaseModulePath = szPath;
	recentInstallCaseModulePath += _T("Resource\\RecentInstallCaseModule\\");
	GetUIRes()->InitResDir(recentInstallCaseModulePath.c_str());

	tstring installCaseModulePath = szPath;
	installCaseModulePath += _T("Resource\\InstallCaseModule\\");
	GetUIRes()->InitResDir(installCaseModulePath.c_str());

	tstring webResourcePath = szPath;
	webResourcePath += _T("Resource\\WebResourceModule\\");
	GetUIRes()->InitResDir(webResourcePath.c_str());

	tstring taskCenterModulePath = szPath;
	taskCenterModulePath += _T("Resource\\TaskCenterModule\\");
	GetUIRes()->InitResDir(taskCenterModulePath.c_str());

	tstring userCenterModulePath = szPath;
	userCenterModulePath += _T("Resource\\UserCenterModule\\");
	GetUIRes()->InitResDir(userCenterModulePath.c_str());

	tstring funcCenterModulePath = szPath;
	funcCenterModulePath += _T("Resource\\FuncCenterModule\\");
	GetUIRes()->InitResDir(funcCenterModulePath.c_str());

	tstring dependenceFuncCenterPath = szPath;
	dependenceFuncCenterPath += _T("Resource\\DependenceFuncCenter\\");
	GetUIRes()->InitResDir(dependenceFuncCenterPath.c_str());
}

