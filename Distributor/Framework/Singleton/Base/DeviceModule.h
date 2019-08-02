#pragma once

// Ä£¿é»ùÀà
class DeviceModule
	: public BaseModule
{
public:
	DeviceModule(wstring deviceID, void* phone)
	{
		this->_deviceID = deviceID;
		this->_nPhone = phone;
	}
	~DeviceModule(){}

	wstring GetDeviceID()
	{
		return this->_deviceID;
	}

	void* GetPhone()
	{
		return this->_nPhone;
	}

	virtual bool Init(CContainerUI* pMain) = 0;

	virtual void Release() = 0;

private:
	wstring _deviceID;
	void* _nPhone;
};