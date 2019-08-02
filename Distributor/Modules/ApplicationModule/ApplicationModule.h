#ifdef APPLICATION_EXPORTS
#define APPLICATION_API __declspec(dllexport)
#else
#define APPLICATION_API __declspec(dllimport)
#endif

APPLICATION_API DeviceModule* CreateApplicationModule(wstring deviceID, void* phone);

class ApplicationController;
class ApplicationModule
	: public DeviceModule
{
public:
	ApplicationModule(wstring deviceID, void* phone);
	~ApplicationModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init( CContainerUI* pMain );

	virtual void Release();


private:
	ApplicationController* _pController;
};

