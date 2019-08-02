#pragma once
#include "ConnectionErrorCode.h"
// 硬件插入消息
struct DeviceLinkInMsg: public BaseMessage<DeviceLinkInMsg>
{
	wstring id;         // 硬件 id
	void* pPhone;       // 设备 索引
	EnumPhoneOS nPhoneOs; 
};

// 硬件拔出消息
struct DeviceLinkOutMsg: public BaseMessage<DeviceLinkOutMsg>
{
	wstring id;         // 硬件 id
	void* pPhone;       // 设备 索引
	EnumPhoneOS nPhoneOs; 
	int nDeviceCount;
};

// 设备断开消息
struct DeviceDisconnectMsg: public BaseMessage<DeviceDisconnectMsg>
{
	wstring  id;         // device id
	bool    isUsb;      // true: usb false: wifi
	bool    isRecovery; // true: recovery 模式
};


// 设备连接消息(Android平台可能还未激活)
struct DeviceConnectMsg: public BaseMessage<DeviceConnectMsg>
{
	void* pPhone;       // 对应的DDM 设备 索引

	wstring  deviceID;  // device id
	wstring  deviceName;// 手机名称，用于显示
	wstring  phoneID;   // DDM内部索引。
	bool    bIsUsb;      // true: usb false: wifi
	bool    bIsRecovery; // true: recovery 模式.  
	bool	bIsJailBreak;
	EnumPhoneOS nPhoneOs;    // 设备操作系统  
};

//设备激活消息
struct DeviceActivatedMsg: public BaseMessage<DeviceActivatedMsg>
{
	wstring deviceID;
	void* pPhone;
	wstring deviceName;
	bool bActived;
	EnumPhoneOS nPhoneOs;    // 设备操作系统  
};

//设备激活消息
struct DeviceJailbreakMsg: public BaseMessage<DeviceJailbreakMsg>
{
	wstring deviceID;
	bool bJailbreak;
};

struct DeviceConnectInitMsg: public BaseMessage<DeviceConnectInitMsg>
{
	void* pPhone;       // 设备 索引
};

struct DeviceConnectWaitWindowsMsg: public BaseMessage<DeviceConnectWaitWindowsMsg>
{
	void* pPhone;       // 设备 索引
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
	void* pPhone;       // 设备 索引
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
	void* pPhone;       // 设备 索引

	DriverInstallState nState;

	int nStep;

	int nProgress;
};

struct DeviceDriverInstallEndMsg : public BaseMessage<DeviceDriverInstallEndMsg>
{
	void* pPhone;       // 设备 索引
};

struct DeviceConnectErrorMsg : public BaseMessage<DeviceConnectErrorMsg>
{
	void* pPhone;       // 设备 索引
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

//设备变动消息
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
	int phoneType;//0表示EnumPhoneType_Android，1表示EnumPhoneType_IPhone
};