#ifdef INSTALLCASE_EXPORTS
#define INSTALLCASE_API __declspec(dllexport)
#else
#define INSTALLCASE_API __declspec(dllimport)
#endif

INSTALLCASE_API SourceModule* CreateInstallCaseModule();

class InstallCaseController;

class InstallCaseModule
	: public SourceModule
{
public:
	InstallCaseModule(void);
	~InstallCaseModule();

	static InstallCaseModule* GetInstance();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pLeft, CContainerUI* pContainer);

private:
	static InstallCaseModule* _pInstance;
	InstallCaseController* _pController;

};

