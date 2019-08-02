#pragma once

#ifdef FUNCCENTER_EXPORTS
#define FUNCCENTER_API __declspec(dllexport)
#else
#define FUNCCENTER_API __declspec(dllimport)
#endif

FUNCCENTER_API DeviceModule* CreateFuncCenterModule(wstring deviceID, void* phone);

class FuncCenterController;

class FuncCenterModule 
	: public DeviceModule
{
public:
	FuncCenterModule(wstring deviceID, void* phone);
	~FuncCenterModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init( CContainerUI* pMain );

	virtual void Release();

private:
	FuncCenterController* _pController;
};