#pragma once

class AblDeviceInfo;
class DeviceDataHelp
{
public:
    DeviceDataHelp(AblDeviceInfo* pdata);
    ~DeviceDataHelp();

	// ��ȡ������ͨ�ŵ������豸Ψһ��ʶ
	std::string GetTrueImei();

	// ��ȡ�豸ƽ̨����
	int GetDeviceType();

	// ��ȡ�ֻ��Ƿ�Խ������root
	int GetJailbroken();

private:

	// ��ȡ�豸��hardID����
	//std::string GetDeviceHardID();

	AblDeviceInfo* _pData;
	//DeviceData* _pData;

};