#pragma once
#include "AblDeviceInfo.h"

// 获取设备信息缓存
struct RequestDeviceTypeMsg : public BaseMessage<RequestDeviceTypeMsg>
{
	wstring deviceID;
	
	OUT EnumPhoneOS deviceType;
};

/// <summary>
///	返回设备状态
/// </summary>
struct RequestDeviceStateMsg : public BaseMessage<RequestDeviceStateMsg>
{
	wstring deviceID;

	OUT bool bResult;
	OUT EnumPhoneOS nOsType;
	OUT bool bIsActive;
	OUT bool bIsRoot;
	OUT bool bIsJailBreak;
	OUT string strTrueImei;
	OUT wstring strDeviceIMSI;
	OUT wstring deviceProductType;
	OUT wstring deviceProductVersion;
	OUT int conncetType;//CONNECT_TYPE这个类型的枚举
	OUT bool bIsPad;
};

struct RequestActiveDeviceMsg : public BaseMessage<RequestActiveDeviceMsg>
{
	wstring deviceID;
};

/// <summary>
///	请求设备信息
/// </summary>
struct RequestDeviceInfoMsg : public BaseMessage<RequestDeviceInfoMsg>
{
	RequestDeviceInfoMsg()
	{
		deviceID = L"";
		bRefresh = false;
	}
	wstring deviceID;
	bool bRefresh;
	RequestDeviceInfoType nType;
};

/// <summary>
///	请求设备信息
/// </summary>
struct GetAblDeviceInfoMsg : public BaseMessage<GetAblDeviceInfoMsg>
{
	wstring deviceID;
	AblDeviceInfo info;
};

/// <summary>
///	返回设备信息
/// </summary>
struct ResponseDeviceBaseInfoMsg : public BaseMessage<ResponseDeviceBaseInfoMsg>
{
	wstring deviceID;
	EnumPhoneOS nOsType;

	wstring serialNumber;           //序列号
	wstring deviceIMEI;		        //IMEI    
	wstring uuid;                   //UUID
	wstring manufacturer;           //厂家
	wstring deviceHWModel;          //硬件型号
	wstring deviceProductVersion;   //固件版本
	wstring deviceProductType;      //设备型号iPhone3,1 iPod4,1 Android
	wstring deviceProductTypeAlias; //设备型号友好别名
	wstring language;               //语言
	wstring deviceCoreVersion;		//内核版本    
	wstring deviceBaseVersion;      //基带版本
	wstring deviceInsideVersion;    //内部版本号
	wstring iccid;                  //iccid
	wstring mobileNumber;           //手机号码
	wstring strRomBuildTeam;        //制作团队
	wstring strSellArea;            //销售地区
	wstring macAddress;

	wstring deviceColor;            //设备颜色
	bool    bIsActived;             //是否激活
	bool    bIsJailBreak;           //iOs是否越狱
	bool    bIsPad;					//是否是ipad
	bool	bIsRoot;				//是否root
};

/// <summary>
///	返回设备名称
/// </summary>
struct ResponseDeviceNameMsg : public BaseMessage<ResponseDeviceNameMsg>
{
	wstring deviceID;
	EnumPhoneOS nOsType;

	wstring deviceName;
};

// 获取设备信息缓存
struct RequestDeviceImageMsg : public BaseMessage<RequestDeviceImageMsg>
{
	wstring deviceID;
	bool bRefresh;
};

/// <summary>
///	返回设备名称
/// </summary>
struct ResponseDeviceImageMsg : public BaseMessage<ResponseDeviceImageMsg>
{
	wstring deviceID;
	wstring imagePath;
};

/// <summary>
///	设备信息加载完成
/// </summary>
struct NotifyLoadDeviceInfoFinishMsg : public BaseMessage<NotifyLoadDeviceInfoFinishMsg>
{
	wstring deviceID;
};

/// <summary>
///	返回设备状态
/// </summary>
struct ResponseDeviceStateMsg : public BaseMessage<ResponseDeviceStateMsg>
{
	wstring deviceID;
	EnumPhoneOS nOsType;

	bool bIsActive;
	bool bIsRoot;
	bool bIsJailBreak;
};

/// <summary>
///	请求设备信息
/// </summary>
struct ResponseSDCardStateMsg : public BaseMessage<ResponseSDCardStateMsg>
{
	wstring deviceID;
	bool bHaveSDCard;
};

struct RequestInstallPositionMsg : public BaseMessage<RequestInstallPositionMsg>
{
	const wstring deviceID;
	RequestInstallPositionMsg(const wstring deviceIDVar)
		:deviceID(deviceIDVar)
	{
	}
	OUT
	bool bInstallOnPhone;
};

struct RequestChangeInstallPositionMsg : public BaseMessage<RequestChangeInstallPositionMsg>
{
	wstring deviceID;
	bool bInstallOnPhone;
};

struct ResponseChangeInstallPositionMsg : public BaseMessage<ResponseChangeInstallPositionMsg>
{
	wstring deviceID;
	bool bInstallOnPhone;
};

// 请求各空间信息
struct RequestDeviceSpaceMsg : public BaseMessage<RequestDeviceSpaceMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 返回各空间信息
/// </summary>
struct ResponseDeviceSpaceMsg : public BaseMessage<ResponseDeviceSpaceMsg>
{
	wstring deviceID;           // 设备ID
	EnumPhoneOS nOsType;

	AblSpaceInfoItem info;
};

/// <summary>
/// 请求设备电池信息
/// </summary>
struct RequestDevicePowerInfoMsg : public BaseMessage<RequestDevicePowerInfoMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 返回设备电池信息
/// </summary>
struct ResponseDevicePowerInfoMsg : public BaseMessage<ResponseDevicePowerInfoMsg>
{
	wstring deviceID;           // 设备ID
	EnumPhoneOS nOsType;

	int nPower;                 // 电量，100为满
};

/// <summary>
/// 请求重启设备
/// </summary>
struct RequestRebootDeviceMsg : public BaseMessage<RequestRebootDeviceMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 请求设备进入Recover
/// </summary>
struct RequestRecoveryDeviceMsg : public BaseMessage<RequestRecoveryDeviceMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 请求设备推出Recover
/// </summary>
struct RequestExitRecoveryDeviceMsg : public BaseMessage<RequestExitRecoveryDeviceMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 请求清理设备空间
/// </summary>
struct RequestClearDeviceMemoryMsg : public BaseMessage<RequestClearDeviceMemoryMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 返回清理设备空间结果
/// </summary>
struct ResponseClearDeviceMemoryMsg : public BaseMessage<ResponseClearDeviceMemoryMsg>
{
	wstring deviceID;           // 设备ID
	EnumPhoneOS nOsType;

	int64_t nSize;
};

/// <summary>
/// 请求闪退修复
/// </summary>
struct RequestRepairDeviceMsg : public BaseMessage<RequestRepairDeviceMsg>
{
	wstring deviceID;           // 设备ID
};

/// <summary>
/// 返回闪退修复结果
/// </summary>
struct ResponseRepairDeviceMsg : public BaseMessage<ResponseRepairDeviceMsg>
{
	wstring deviceID;           // 设备ID

	bool bResult;
};

/// <summary>
/// 请求是否需要安装AppSync
/// </summary>
struct RequestNeedInstallAppSyncMsg : public BaseMessage<RequestNeedInstallAppSyncMsg>
{
	RequestNeedInstallAppSyncMsg()
	{
		deviceID = L"";
		bSync = false;
		bNeed = false;
	}

	wstring deviceID;           // 设备ID
	bool bSync;

	OUT bool bNeed;
};

/// <summary>
/// 返回是否需要安装AppSync
/// </summary>
struct ResponseNeedInstallAppSyncMsg : public BaseMessage<ResponseNeedInstallAppSyncMsg>
{
	wstring deviceID;           // 设备ID
	OUT bool bNeedInstall;
};

/// <summary>
/// 请求安装AppSync
/// </summary>
struct RequestInstallAppSyncMsg : public BaseMessage<RequestInstallAppSyncMsg>
{
	wstring deviceID;           // 设备ID
};



//安装appsync结果反馈
struct RequestInstallAppsyncMsg : public BaseMessage<RequestInstallAppsyncMsg>
{
	wstring deviceID;
	bool installReturn;		//true成功 false失败
};


