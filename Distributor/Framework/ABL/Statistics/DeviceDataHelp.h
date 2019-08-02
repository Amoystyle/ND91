#pragma once

class AblDeviceInfo;
class DeviceDataHelp
{
public:
    DeviceDataHelp(AblDeviceInfo* pdata);
    ~DeviceDataHelp();

	// 获取与服务端通信的真正设备唯一标识
	std::string GetTrueImei();

	// 获取设备平台类型
	int GetDeviceType();

	// 获取手机是否越狱或者root
	int GetJailbroken();

private:

	// 获取设备的hardID代替
	//std::string GetDeviceHardID();

	AblDeviceInfo* _pData;
	//DeviceData* _pData;

};