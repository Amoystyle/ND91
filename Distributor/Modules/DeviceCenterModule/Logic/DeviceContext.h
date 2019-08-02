#pragma once

enum DeviceModuleType;

class DeviceContext
{
public:
	DeviceContext(wstring deviceID, void* phone, CContainerUI* pContainer);
	~DeviceContext();

	void InitModule( DeviceModuleType type );

	void UpdateDeviceID( wstring newKey );

	void ShowModule(DeviceModuleType type);

	void HideModule();

	void* GetPhone();

	wstring GetDeviceID();

	void RemoveDeviceModule();

	DeviceModuleType GetCurrentType(){ return this->_currentType;}

private:
	void Init();

	CContainerUI* _pContainer;

	map<DeviceModuleType, DeviceModule*> _mapModules;

	DeviceModuleType _currentType;
	wstring _deviceID;
	void* _nPhone;
};