#pragma once
#include "AblDeviceInfo.h"

// ��ȡ�豸��Ϣ����
struct RequestDeviceTypeMsg : public BaseMessage<RequestDeviceTypeMsg>
{
	wstring deviceID;
	
	OUT EnumPhoneOS deviceType;
};

/// <summary>
///	�����豸״̬
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
	OUT int conncetType;//CONNECT_TYPE������͵�ö��
	OUT bool bIsPad;
};

struct RequestActiveDeviceMsg : public BaseMessage<RequestActiveDeviceMsg>
{
	wstring deviceID;
};

/// <summary>
///	�����豸��Ϣ
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
///	�����豸��Ϣ
/// </summary>
struct GetAblDeviceInfoMsg : public BaseMessage<GetAblDeviceInfoMsg>
{
	wstring deviceID;
	AblDeviceInfo info;
};

/// <summary>
///	�����豸��Ϣ
/// </summary>
struct ResponseDeviceBaseInfoMsg : public BaseMessage<ResponseDeviceBaseInfoMsg>
{
	wstring deviceID;
	EnumPhoneOS nOsType;

	wstring serialNumber;           //���к�
	wstring deviceIMEI;		        //IMEI    
	wstring uuid;                   //UUID
	wstring manufacturer;           //����
	wstring deviceHWModel;          //Ӳ���ͺ�
	wstring deviceProductVersion;   //�̼��汾
	wstring deviceProductType;      //�豸�ͺ�iPhone3,1 iPod4,1 Android
	wstring deviceProductTypeAlias; //�豸�ͺ��Ѻñ���
	wstring language;               //����
	wstring deviceCoreVersion;		//�ں˰汾    
	wstring deviceBaseVersion;      //�����汾
	wstring deviceInsideVersion;    //�ڲ��汾��
	wstring iccid;                  //iccid
	wstring mobileNumber;           //�ֻ�����
	wstring strRomBuildTeam;        //�����Ŷ�
	wstring strSellArea;            //���۵���
	wstring macAddress;

	wstring deviceColor;            //�豸��ɫ
	bool    bIsActived;             //�Ƿ񼤻�
	bool    bIsJailBreak;           //iOs�Ƿ�Խ��
	bool    bIsPad;					//�Ƿ���ipad
	bool	bIsRoot;				//�Ƿ�root
};

/// <summary>
///	�����豸����
/// </summary>
struct ResponseDeviceNameMsg : public BaseMessage<ResponseDeviceNameMsg>
{
	wstring deviceID;
	EnumPhoneOS nOsType;

	wstring deviceName;
};

// ��ȡ�豸��Ϣ����
struct RequestDeviceImageMsg : public BaseMessage<RequestDeviceImageMsg>
{
	wstring deviceID;
	bool bRefresh;
};

/// <summary>
///	�����豸����
/// </summary>
struct ResponseDeviceImageMsg : public BaseMessage<ResponseDeviceImageMsg>
{
	wstring deviceID;
	wstring imagePath;
};

/// <summary>
///	�豸��Ϣ�������
/// </summary>
struct NotifyLoadDeviceInfoFinishMsg : public BaseMessage<NotifyLoadDeviceInfoFinishMsg>
{
	wstring deviceID;
};

/// <summary>
///	�����豸״̬
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
///	�����豸��Ϣ
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

// ������ռ���Ϣ
struct RequestDeviceSpaceMsg : public BaseMessage<RequestDeviceSpaceMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// ���ظ��ռ���Ϣ
/// </summary>
struct ResponseDeviceSpaceMsg : public BaseMessage<ResponseDeviceSpaceMsg>
{
	wstring deviceID;           // �豸ID
	EnumPhoneOS nOsType;

	AblSpaceInfoItem info;
};

/// <summary>
/// �����豸�����Ϣ
/// </summary>
struct RequestDevicePowerInfoMsg : public BaseMessage<RequestDevicePowerInfoMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// �����豸�����Ϣ
/// </summary>
struct ResponseDevicePowerInfoMsg : public BaseMessage<ResponseDevicePowerInfoMsg>
{
	wstring deviceID;           // �豸ID
	EnumPhoneOS nOsType;

	int nPower;                 // ������100Ϊ��
};

/// <summary>
/// ���������豸
/// </summary>
struct RequestRebootDeviceMsg : public BaseMessage<RequestRebootDeviceMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// �����豸����Recover
/// </summary>
struct RequestRecoveryDeviceMsg : public BaseMessage<RequestRecoveryDeviceMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// �����豸�Ƴ�Recover
/// </summary>
struct RequestExitRecoveryDeviceMsg : public BaseMessage<RequestExitRecoveryDeviceMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// ���������豸�ռ�
/// </summary>
struct RequestClearDeviceMemoryMsg : public BaseMessage<RequestClearDeviceMemoryMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// ���������豸�ռ���
/// </summary>
struct ResponseClearDeviceMemoryMsg : public BaseMessage<ResponseClearDeviceMemoryMsg>
{
	wstring deviceID;           // �豸ID
	EnumPhoneOS nOsType;

	int64_t nSize;
};

/// <summary>
/// ���������޸�
/// </summary>
struct RequestRepairDeviceMsg : public BaseMessage<RequestRepairDeviceMsg>
{
	wstring deviceID;           // �豸ID
};

/// <summary>
/// ���������޸����
/// </summary>
struct ResponseRepairDeviceMsg : public BaseMessage<ResponseRepairDeviceMsg>
{
	wstring deviceID;           // �豸ID

	bool bResult;
};

/// <summary>
/// �����Ƿ���Ҫ��װAppSync
/// </summary>
struct RequestNeedInstallAppSyncMsg : public BaseMessage<RequestNeedInstallAppSyncMsg>
{
	RequestNeedInstallAppSyncMsg()
	{
		deviceID = L"";
		bSync = false;
		bNeed = false;
	}

	wstring deviceID;           // �豸ID
	bool bSync;

	OUT bool bNeed;
};

/// <summary>
/// �����Ƿ���Ҫ��װAppSync
/// </summary>
struct ResponseNeedInstallAppSyncMsg : public BaseMessage<ResponseNeedInstallAppSyncMsg>
{
	wstring deviceID;           // �豸ID
	OUT bool bNeedInstall;
};

/// <summary>
/// ����װAppSync
/// </summary>
struct RequestInstallAppSyncMsg : public BaseMessage<RequestInstallAppSyncMsg>
{
	wstring deviceID;           // �豸ID
};



//��װappsync�������
struct RequestInstallAppsyncMsg : public BaseMessage<RequestInstallAppsyncMsg>
{
	wstring deviceID;
	bool installReturn;		//true�ɹ� falseʧ��
};


