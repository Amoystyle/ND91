#ifdef RECENTINSTALLCASEMODULE_EXPORTS
#define RECENTINSTALLCASEMODULE_API __declspec(dllexport)
#else
#define RECENTINSTALLCASEMODULE_API __declspec(dllimport)
#endif

RECENTINSTALLCASEMODULE_API DeviceModule* CreateRecentInstallCaseModule(wstring deviceID, void* phone);

class RecentInstallCaseController;
class RecentInstallCaseModule
	: public DeviceModule
{
public:
	RecentInstallCaseModule(wstring deviceID, void* phone);
	~RecentInstallCaseModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pMain);

	void Release();

private:
	RecentInstallCaseController* _pController;

};

