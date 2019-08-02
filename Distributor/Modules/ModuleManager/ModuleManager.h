#ifdef MODULEMANAGER_EXPORTS
#define MODULEMANAGER_API __declspec(dllexport)
#else
#define MODULEMANAGER_API __declspec(dllimport)
#endif

class ModuleController;

class MODULEMANAGER_API ModuleManager
{
public:
	ModuleManager();
	~ModuleManager();

	static ModuleManager* GetInstance();

	static void Release();

	bool Init(CContainerUI* pTitleToolBar, CContainerUI* pMain);

	void Show();

	void Hide();

	void LoadResource();

private:
	static ModuleManager* _pInstance;
	ModuleController* _pController;

};

