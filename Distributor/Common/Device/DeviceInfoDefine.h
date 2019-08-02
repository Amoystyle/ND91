#pragma once

// 设备操作系统类型 - 以后上层逐渐用这个替换掉ABI和DDM定义的类型
enum EnumPhoneOS
{
	ePhoneOS_None			= 0x0000,
	ePhoneOS_Ios			= 0x0100,
	ePhoneOS_Android		= 0x0200,
};

// 设备平台类型
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
// 设备平台类型
// 请求的信息定义
enum RequestDeviceInfoType
{
    RequestDeviceInfoType_All,      //全部信息
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

//设备型号的类型
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

// 请求设备空间信息的模块列表
enum eSpaceInfo_Module
{
    eSpaceInfo_Module_ALL,        // 全部    
    eSpaceInfo_Module_File,       // 文件系统，用这个来取SDCard/手机空间信息
    eSpaceInfo_Module_APP,        // 应用
    eSpaceInfo_Module_Music,      // 音乐
    eSpaceInfo_Module_Video,      // 视频
    eSpaceInfo_Module_Picture,    // 图片
    eSpaceInfo_Module_Contact,    // 联系人
    eSpaceInfo_Module_SMS,        // 短信
    eSpaceInfo_Module_ElseFile,   // 系统文件
};
// 请求设备空间信息的位置列表
enum eSpaceInfo_Location
{
    eSpaceInfo_Location_ALL,
    eSpaceType_Location_SDCard,
    eSpaceType_Location_Phone,
};

// 控件信息结构
struct AblSpaceInfoItem
{
    eSpaceInfo_Location nLocation;      // 设备空间类型 sdcard/手机

    __int64 nAll;  // 总空间大小
    __int64 nUsed; // 已用空间大小
    __int64 nLeft; // 剩余空间大小

    __int64 nUserAll;  // 总空间大小
    __int64 nUserUsed; // 已用空间大小
    __int64 nUserLeft; // 剩余空间大小

    __int64 nSysAll;  // 总空间大小
    __int64 nSysUsed; // 已用空间大小
    __int64 nSysLeft; // 剩余空间大小
};

// 设备基本信息
struct AblBaseDeviceInfo
{    
    EnumPhoneOS nOsType;            //设备操作系统Android/iOs
    EnumPhonePlatForm nPlatForm;    //设备平台
    wstring serialNumber;           //序列号
    wstring deviceIMEI;		        //IMEI    
    wstring deviceIMSI;		        //IMSI    
    wstring manufacturer;           //厂家
    wstring deviceHWModel;          //硬件型号
    wstring deviceProductVersion;   //固件版本
    wstring deviceProductType;      //设备型号iPhone3,1 iPod4,1 Android
	wstring deviceModelName;		//设备类型短名称 (如"iPhone3,1"的短名称是"iPhone4") 
    wstring deviceProductTypeAlias; //设备型号友好别名
    wstring language;               //语言
    wstring deviceCoreVersion;		//内核版本    
    wstring deviceBaseVersion;      //基带版本
    wstring deviceInsideVersion;    //内部版本号
    wstring mobileNumber;           //手机号码
    wstring iccid;                  //iccid
    wstring uuid;                   //UUID
    wstring deviceColor;            //设备颜色
    wstring strRomBuildTeam;        //制作团队
    wstring strSellArea;            //销售地区
};