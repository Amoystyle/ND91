#pragma once

#ifdef DEVICECENTER_EXPORTS
#define DEVICECENTERM_API __declspec(dllexport)
#else
#define DEVICECENTERM_API __declspec(dllimport)
#endif

DEVICECENTERM_API SourceModule* CreateDeviceCenterModule();

class DeviceController;

class DeviceCenterModule 
	: public SourceModule
{
public:
	DeviceCenterModule();
	~DeviceCenterModule();

	static DeviceCenterModule* GetInstance();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pLeft,  CContainerUI* pContainer );

private:
	DeviceController* _pController;

	static DeviceCenterModule* _pInstance;
};
