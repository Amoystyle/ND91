#pragma once
#include "ConnectionErrorCode.h"
// Ӳ��������Ϣ
struct DeviceLinkInMsg: public BaseMessage<DeviceLinkInMsg>
{
	wstring id;         // Ӳ�� id
	void* pPhone;       // �豸 ����
	EnumPhoneOS nPhoneOs; 
};

// Ӳ���γ���Ϣ
struct DeviceLinkOutMsg: public BaseMessage<DeviceLinkOutMsg>
{
	wstring id;         // Ӳ�� id
	void* pPhone;       // �豸 ����
	EnumPhoneOS nPhoneOs; 
	int nDeviceCount;
};

// �豸�Ͽ���Ϣ
struct DeviceDisconnectMsg: public BaseMessage<DeviceDisconnectMsg>
{
	wstring  id;         // device id
	bool    isUsb;      // true: usb false: wifi
	bool    isRecovery; // true: recovery ģʽ
};


// �豸������Ϣ(Androidƽ̨���ܻ�δ����)
struct DeviceConnectMsg: public BaseMessage<DeviceConnectMsg>
{
	void* pPhone;       // ��Ӧ��DDM �豸 ����

	wstring  deviceID;  // device id
	wstring  deviceName;// �ֻ����ƣ�������ʾ
	wstring  phoneID;   // DDM�ڲ�������
	bool    bIsUsb;      // true: usb false: wifi
	bool    bIsRecovery; // true: recovery ģʽ.  
	bool	bIsJailBreak;
	EnumPhoneOS nPhoneOs;    // �豸����ϵͳ  
};

//�豸������Ϣ
struct DeviceActivatedMsg: public BaseMessage<DeviceActivatedMsg>
{
	wstring deviceID;
	void* pPhone;
	wstring deviceName;
	bool bActived;
	EnumPhoneOS nPhoneOs;    // �豸����ϵͳ  
};

//�豸������Ϣ
struct DeviceJailbreakMsg: public BaseMessage<DeviceJailbreakMsg>
{
	wstring deviceID;
	bool bJailbreak;
};

struct DeviceConnectInitMsg: public BaseMessage<DeviceConnectInitMsg>
{
	void* pPhone;       // �豸 ����
};

struct DeviceConnectWaitWindowsMsg: public BaseMessage<DeviceConnectWaitWindowsMsg>
{
	void* pPhone;       // �豸 ����
};

enum DriverInstallState
{
	DriverInstallState_Init,
	DriverInstallState_Downloading,
	DriverInstallState_Installing,
	DriverInstallState_End,
};

struct DeviceDriverInstallInitMsg : public BaseMessage<DeviceDriverInstallInitMsg>
{
	void* pPhone;       // �豸 ����
};

struct DeviceDriverInstallProgressMsg : public BaseMessage<DeviceDriverInstallProgressMsg>
{
	DeviceDriverInstallProgressMsg()
	{
		pPhone = NULL;
		nState = DriverInstallState_Init;
		nStep = 0;
		nProgress = 0;
	}
	void* pPhone;       // �豸 ����

	DriverInstallState nState;

	int nStep;

	int nProgress;
};

struct DeviceDriverInstallEndMsg : public BaseMessage<DeviceDriverInstallEndMsg>
{
	void* pPhone;       // �豸 ����
};

struct DeviceConnectErrorMsg : public BaseMessage<DeviceConnectErrorMsg>
{
	void* pPhone;       // �豸 ����
	DeviceConnectError nError;
};

struct RequestConnectMsg : public BaseMessage<RequestConnectMsg>
{
	RequestConnectMsg()
	{
		pPhone = NULL;
		bRetry = false;
	}

	void* pPhone;
	bool bRetry;
};

struct NotifyDeviceKeyChangeMsg : public BaseMessage<NotifyDeviceKeyChangeMsg>
{
	void* pPhone;
	wstring newKey;
};

struct RequestIsRecoveryMode : public BaseMessage<RequestIsRecoveryMode>
{
	wstring deviceID;

	bool bRecoverMode;
};

//�豸�䶯��Ϣ
struct DeviceChangeMsg : BaseMessage<DeviceChangeMsg>
{
	DeviceChangeMsg()
	{
		_bLinkOut = false;
		phoneType = 0;
	}
	LPARAM  _lParam;
	wstring _st_sn;
	bool    _bLinkOut;
	int phoneType;//0��ʾEnumPhoneType_Android��1��ʾEnumPhoneType_IPhone
};