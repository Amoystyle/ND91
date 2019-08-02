#pragma once

#ifdef ASSISTANTINSTALLMANAGER_EXPORTS
#define ASSISTANTINSTALLMANAGER_API __declspec(dllexport)
#else
#define ASSISTANTINSTALLMANAGER_API __declspec(dllimport)
#endif

class InstallAppsyncController;
class DeamonDownloadManager;
class DeamonInstallManager;
class ASSISTANTINSTALLMANAGER_API AssistantInstallManager 
{
public:
	AssistantInstallManager(void);
	~AssistantInstallManager();

	static void Init();

private:
	void OnInit();
private:
	InstallAppsyncController* _pController;

	static AssistantInstallManager* _pInstance;

	DeamonDownloadManager*	_pDownLoad;
	DeamonInstallManager*	_pInstall ;

};