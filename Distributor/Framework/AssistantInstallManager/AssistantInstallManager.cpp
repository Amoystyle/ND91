#include "stdafx.h"
#include "AssistantInstallManager.h"
#include "Logic/InstallAppsyncController.h"
#include "Logic/DeamonDownlaodManager.h"
#include "logic/DeamonInstallManager.h"
#include "Common/UserCenter/UserCenterMsg.h"

AssistantInstallManager* AssistantInstallManager::_pInstance = NULL;

AssistantInstallManager::AssistantInstallManager()
{
	_pController = new InstallAppsyncController();
}

AssistantInstallManager::~AssistantInstallManager()
{

}

void AssistantInstallManager::Init()
{
	if(_pInstance == NULL)
	{
		_pInstance = new AssistantInstallManager();
		_pInstance->OnInit();
	}
}

void AssistantInstallManager::OnInit()
{
	_pDownLoad = DeamonDownloadManager::GetInstance();
	_pInstall  = DeamonInstallManager::GetInstance();

	BeginDownLoadDeamonMsg msg;
	msg.Send();
}
