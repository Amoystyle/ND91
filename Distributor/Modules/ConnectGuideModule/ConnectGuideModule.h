#ifdef CONNECTGUIDE_EXPORTS
#define CONNECTGUIDE_API __declspec(dllexport)
#else
#define CONNECTGUIDE_API __declspec(dllimport)
#endif

CONNECTGUIDE_API DeviceModule* CreateConnectGuideModule(wstring deviceID, void* phone);

class ConnectGuideController;

class ConnectGuideModule 
	: public DeviceModule
{
public:
	ConnectGuideModule(wstring deviceID, void* phone);

	~ConnectGuideModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pMain);

	virtual void Release();

private:
	ConnectGuideController* pController;

};

