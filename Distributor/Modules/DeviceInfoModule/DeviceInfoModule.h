#ifdef DEVICEINFO_EXPORTS
#define DEVICEINFO_API __declspec(dllexport)
#else
#define DEVICEINFO_API __declspec(dllimport)
#endif

DEVICEINFO_API DeviceModule* CreateDeviceInfoModule(wstring deviceID, void* phone);

class DeviceInfoController;
class DeviceInfoModule
	: public DeviceModule
{
public:
	DeviceInfoModule(wstring deviceID, void* phone);
	~DeviceInfoModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pMain);

	virtual void Release();

private:
	DeviceInfoController* _pController;

};

