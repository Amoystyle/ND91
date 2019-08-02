#pragma once

// �豸����ϵͳ���� - �Ժ��ϲ���������滻��ABI��DDM���������
enum EnumPhoneOS
{
	ePhoneOS_None			= 0x0000,
	ePhoneOS_Ios			= 0x0100,
	ePhoneOS_Android		= 0x0200,
};

// �豸ƽ̨����
enum EnumPhonePlatForm
{   
	ePhonePlatForm_None      = 0x0000, 
	ePhonePlatForm_iPhone    = 0x0101,
	ePhonePlatForm_iPad      = 0x0102,
	ePhonePlatForm_iPod      = 0x0103,
	ePhonePlatForm_iTouch    = 0x0104,
	ePhonePlatForm_iPadmini  = 0x0105,
	ePhonePlatForm_Android   = 0x0200,
};

enum AblSDCardState
{
    eSDCardState_REMOVED,
    eSDCardState_UNMOUNTED,
    eSDCardState_CHECKING,
    eSDCardState_NOFS,
    eSDCardState_MOUNTED,
    eSDCardState_MOUNTED_READ_ONLY,
    eSDCardState_SHARED,
    eSDCardState_BAD_REMOVAL,
    eSDCardState_UNMOUNTABLE,
    eSDCardState_UNKNOW
};
// �豸ƽ̨����
// �������Ϣ����
enum RequestDeviceInfoType
{
    RequestDeviceInfoType_All,      //ȫ����Ϣ
    RequestDeviceInfoType_BaseInfo,
    RequestDeviceInfoType_PlusInfo,    
    RequestDeviceInfoType_SellInfo,    
    RequestDeviceInfoType_DeviceName,
    RequestDeviceInfoType_IsRoot,
    RequestDeviceInfoType_IMEI,
    RequestDeviceInfoType_IMSI,
    RequestDeviceInfoType_WifiInfo,
    RequestDeviceInfoType_ScreenInf,
    RequestDeviceInfoType_WallPaperInfo,
    RequestDeviceInfoType_MacInfo, 
    RequestDeviceInfoType_PowerInfo,
    RequestDeviceInfoType_SignalInfo,
    RequestDeviceInfoType_InfoForNetInterface,
    RequestDeviceInfoType_SDCardState,
};

//�豸�ͺŵ�����
enum AblDeviceModelType
{
    AblDeviceModelType_iPhone,
    AblDeviceModelType_iPad,
    AblDeviceModelType_iPod,
    AblDeviceModelType_iTouch,
    AblDeviceModelType_Android,
};

enum Ablconnect_Type
{
	USB_TYPE,
	WIFI_TYPE,
};

// �����豸�ռ���Ϣ��ģ���б�
enum eSpaceInfo_Module
{
    eSpaceInfo_Module_ALL,        // ȫ��    
    eSpaceInfo_Module_File,       // �ļ�ϵͳ���������ȡSDCard/�ֻ��ռ���Ϣ
    eSpaceInfo_Module_APP,        // Ӧ��
    eSpaceInfo_Module_Music,      // ����
    eSpaceInfo_Module_Video,      // ��Ƶ
    eSpaceInfo_Module_Picture,    // ͼƬ
    eSpaceInfo_Module_Contact,    // ��ϵ��
    eSpaceInfo_Module_SMS,        // ����
    eSpaceInfo_Module_ElseFile,   // ϵͳ�ļ�
};
// �����豸�ռ���Ϣ��λ���б�
enum eSpaceInfo_Location
{
    eSpaceInfo_Location_ALL,
    eSpaceType_Location_SDCard,
    eSpaceType_Location_Phone,
};

// �ؼ���Ϣ�ṹ
struct AblSpaceInfoItem
{
    eSpaceInfo_Location nLocation;      // �豸�ռ����� sdcard/�ֻ�

    __int64 nAll;  // �ܿռ��С
    __int64 nUsed; // ���ÿռ��С
    __int64 nLeft; // ʣ��ռ��С

    __int64 nUserAll;  // �ܿռ��С
    __int64 nUserUsed; // ���ÿռ��С
    __int64 nUserLeft; // ʣ��ռ��С

    __int64 nSysAll;  // �ܿռ��С
    __int64 nSysUsed; // ���ÿռ��С
    __int64 nSysLeft; // ʣ��ռ��С
};

// �豸������Ϣ
struct AblBaseDeviceInfo
{    
    EnumPhoneOS nOsType;            //�豸����ϵͳAndroid/iOs
    EnumPhonePlatForm nPlatForm;    //�豸ƽ̨
    wstring serialNumber;           //���к�
    wstring deviceIMEI;		        //IMEI    
    wstring deviceIMSI;		        //IMSI    
    wstring manufacturer;           //����
    wstring deviceHWModel;          //Ӳ���ͺ�
    wstring deviceProductVersion;   //�̼��汾
    wstring deviceProductType;      //�豸�ͺ�iPhone3,1 iPod4,1 Android
	wstring deviceModelName;		//�豸���Ͷ����� (��"iPhone3,1"�Ķ�������"iPhone4") 
    wstring deviceProductTypeAlias; //�豸�ͺ��Ѻñ���
    wstring language;               //����
    wstring deviceCoreVersion;		//�ں˰汾    
    wstring deviceBaseVersion;      //�����汾
    wstring deviceInsideVersion;    //�ڲ��汾��
    wstring mobileNumber;           //�ֻ�����
    wstring iccid;                  //iccid
    wstring uuid;                   //UUID
    wstring deviceColor;            //�豸��ɫ
    wstring strRomBuildTeam;        //�����Ŷ�
    wstring strSellArea;            //���۵���
};