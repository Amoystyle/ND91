#pragma once

#ifdef WIN32
    #include <windows.h>
    typedef UINT64 uint64_t;
#else
    #include <inttypes.h>
    #include <string.h>
#endif

#define UNFIND_DEVICE_ERROR 1				//未找到设备
#define PRODUCT_VERSION_NOT_AGREE_ERROR 2   //固件版本不符合要求
#define UNINSTALL_APPSYNC_ERROR 3			//未安装APPSYNC
#define UNFIND_APP_ERROR 4					//未找到应用
#define UNKNOW_ERROR 5

#define MOBILE_BASE_INFO_LEN 64
#define URL_MAX_LEN 2048

enum NDASSISTANT_DEVICE_STATE
{
	NDS_NORMAL,
	NDS_RECOVERY,
	NDS_UNCONNECT,
};

typedef struct _DISK_INFO
{
    _DISK_INFO() : i64Total(0),
        i64Free(0)
    {
    };

    _DISK_INFO& operator=( const _DISK_INFO& rhs)
    {
        this->i64Total = rhs.i64Total;	
        this->i64Free = rhs.i64Free;	

        return *this;
    };

    /*UINT64*/uint64_t i64Total;
    /*UINT64*/uint64_t i64Free;

}DISK_INFO, *PDISK_INFO;

typedef struct _SDCARD_FILE_INFO
{
    _SDCARD_FILE_INFO()		
    {		
        memset(this,0,sizeof(_SDCARD_FILE_INFO));
    };

    /*INT32*/signed int nMusic; // 音乐
    /*INT32*/signed int nVideo; // 视频
    /*INT32*/signed int nPhoto; // 图片
    /*INT32*/signed int nBook;  // 电子书

}SDCARD_FILE_INFO, *PSDCARD_FILE_INFO;

//低位如果置1,则需要让连接向导置顶
#define PHONE_NONE				0x00000000		//预留
#define PHONE_CONNECTED			0x00000001		//一款连接正常的手机被添加
#define PHONE_CONNECTING		0x00000002		//一款连接失败的手机被添加
#define	PHONE_CONNECT_FIAL		0x00000003		//一款连接失败的手机经过连接向导的操作还是不能连接成功，界面需要提示“连接失败”
#define PHONE_REMOVE			0x00000004		//一款手机被移除
#define PHONE_CONNECT_NODEAMON	0x00000005		//手机可正常连接但没装deamon

//设备类型
#define DEVICE_NONE				0
#define DEVICE_IOS				1
#define DEVICE_ANDROID			2

typedef struct _MOBILE_BASE_INFO
{
	_MOBILE_BASE_INFO()
	{
		memset(this,0,sizeof(_MOBILE_BASE_INFO));
		cbSize = sizeof(_MOBILE_BASE_INFO);
		nMobileState = PHONE_NONE;
	};

	/*DWORD*/unsigned long				cbSize;

	/*DWORD*/unsigned long				nDeviceType;					  // 设备的类型  DEVICE_IOS/DEVICE_ANDROID
	/*DWORD*/unsigned long				nMobileState;                     // 手机状态 PHONE_CONNECTED/PHONE_CONNECTING/PHONE_CONNECT_FIAL/PHONE_REMOVE 
	SDCARD_FILE_INFO	sdCardInfo;                       // sd卡上文件信息
	/*INT32*/signed int				uSystemApp;						  // 系统应用数量
	/*INT32*/signed int				uDataApp;						  // 第三方应用数量
	/*INT32*/signed int				uGameApp;						  // 游戏应用数量
	/*INT32*/signed int				uContact;						  // 联系人数量
	/*INT32*/signed int				uSms;							  // 短信数量
	/*INT32*/signed int				uMms;							  // 彩信数量
	/*INT32*/signed int				uVersionDaemon;					  // Daemon版本
	/*INT32*/signed int				uSdk;							  // 手机SDK
	DISK_INFO			dskMemory;						  // 手机内存
	DISK_INFO			dskDisk;						  // 手机硬盘
	DISK_INFO			dskSdcard;						  // sd 卡

	/*UINT*/unsigned int			uConnType;						  // 连接方式 0 USB数据线 1 WIFI
	/*UINT*/unsigned int			uBattery;						  // 电池电量
	/*WCHAR*/wchar_t				szCPU[MOBILE_BASE_INFO_LEN];      // CPU 信息
	/*WCHAR*/wchar_t				szManu[MOBILE_BASE_INFO_LEN];     // 厂商
	/*WCHAR*/wchar_t				szModel[MOBILE_BASE_INFO_LEN];    // 手机型号
	/*WCHAR*/wchar_t				szSerial[MOBILE_BASE_INFO_LEN];   // 手机序列号
	/*WCHAR*/wchar_t				szImei[MOBILE_BASE_INFO_LEN];     // 手机IMEI号
	/*WCHAR*/wchar_t				szOSBuild[MOBILE_BASE_INFO_LEN];  // 系统版本号
	/*WCHAR*/wchar_t		        szUid[MOBILE_BASE_INFO_LEN];	  // 手机vid+pid
	/*WCHAR*/wchar_t				wszImageUrl[ URL_MAX_LEN ];       // 手机图片
	/*WCHAR*/wchar_t				wszVendorImageUrl[ URL_MAX_LEN ]; // 厂商图片	
	/*INT32*/signed int				nReverse[10];					  // 保留，
	/*WCHAR*/wchar_t				wszReverse[ URL_MAX_LEN ];		  // 保留

	/*BOOL*/bool                    bJailBreaked;                     //是否越狱
	/*WCHAR*/wchar_t                szDevName[MOBILE_BASE_INFO_LEN];  //设备名
	/*INT32*/signed int             uDeviceType;                      // 设备类型: 1 ios 2 android


}MOBILE_BASE_INFO, *PMOBILE_BASE_INFO;

//typedef struct _MOBILE_BASE_INFO
//{
//    DWORD				cbSize;
//
//    DWORD				nMobileState;                     // 手机状态 PHONE_CONNECTED/PHONE_CONNECTING/PHONE_CONNECT_FIAL/PHONE_REMOVE 
//    SDCARD_FILE_INFO	sdCardInfo;                       // sd卡上文件信息
//    INT32				uSystemApp;						  // 系统应用数量
//    INT32				uDataApp;						  // 第三方应用数量
//    INT32				uGameApp;						  // 游戏应用数量
//    INT32				uContact;						  // 联系人数量
//    INT32				uSms;							  // 短信数量
//    INT32				uMms;							  // 彩信数量
//    INT32				uVersionDaemon;					  // Daemon版本
//    INT32				uSdk;							  // 手机SDK
//    DISK_INFO			dskMemory;						  // 手机内存
//    DISK_INFO			dskDisk;						  // 手机硬盘
//    DISK_INFO			dskSdcard;						  // sd 卡
//
//    UINT				uConnType;						  // 连接方式 0 USB数据线 1 WIFI
//    UINT				uBattery;						  // 电池电量
//    WCHAR				szCPU[MOBILE_BASE_INFO_LEN];      // CPU 信息
//    WCHAR				szManu[MOBILE_BASE_INFO_LEN];     // 厂商
//    WCHAR				szModel[MOBILE_BASE_INFO_LEN];    // 手机型号
//    WCHAR				szSerial[MOBILE_BASE_INFO_LEN];   // 手机序列号
//    WCHAR				szImei[MOBILE_BASE_INFO_LEN];     // 手机IMEI号
//    WCHAR				szOSBuild[MOBILE_BASE_INFO_LEN];  // 系统版本号
//    WCHAR		        szUid[MOBILE_BASE_INFO_LEN];	  // 手机vid+pid
//    WCHAR				wszImageUrl[ URL_MAX_LEN ];       // 手机图片
//    WCHAR				wszVendorImageUrl[ URL_MAX_LEN ]; // 厂商图片	
//    INT32				nReverse[10];					  // 保留，
//    WCHAR				wszReverse[ URL_MAX_LEN ];		  // 保留
//
//    BOOL                bJailBreaked;                       //是否越狱
//    WCHAR               szDevName[MOBILE_BASE_INFO_LEN];    //设备名
//    INT32               uDeviceType;                        // 设备类型: 1 ios 2 android
//
//    _MOBILE_BASE_INFO()
//    {
//        memset(this,0,sizeof(_MOBILE_BASE_INFO));
//    }
//
//}MOBILE_BASE_INFO, *PMOBILE_BASE_INFO;

#define MOBILE_PACKAGE_NAME_LEN  64

typedef struct _MOBILE_APP_INFO
{
    _MOBILE_APP_INFO()
    {
        memset(this,0,sizeof(_MOBILE_APP_INFO));
        nVersion = 0xFFFFFFF;
    };

    /*BOOL*/bool	bInRom;		//	0安装在SD卡中；1为安装在手机内存
    /*UINT32*/unsigned int	nFlags;		//	应用标识; flags=0第三方应用； flags=1系统应用
    /*UINT32*/unsigned int	nVersion;	//	应用版本
    /*UINT64*/uint64_t	tInstall;	//	ts安装时间
    /*UINT64*/uint64_t  uAppSize;	//  app 大小
    /*WCHAR*/wchar_t	wszPackageName[ MOBILE_PACKAGE_NAME_LEN ]; 	// 程序的唯一标识  包名
    /*WCHAR*/wchar_t	wszAppName[ MOBILE_PACKAGE_NAME_LEN ]; 		// 应用程序名字
    /*WCHAR*/wchar_t	wszVersionName[ MOBILE_PACKAGE_NAME_LEN ]; 	// 版本名
    /*WCHAR*/wchar_t	wszIconPath[ /*MAX_PATH*/260]; 					// app icon 的本地路径
    /*WCHAR*/wchar_t   wszPermission[/*MAX_PATH*/260*10];					// 权限

    /*WCHAR*/wchar_t        szDescription[URL_MAX_LEN];     // 描述
    /*WCHAR*/wchar_t        szUrl[URL_MAX_LEN];             // 链接地址
    /*WCHAR*/wchar_t        szAppPhonePath[ /*MAX_PATH*/260];    // 程序路径 (手机中)
    /*WCHAR*/wchar_t        szIconPhonePath[ /*MAX_PATH*/260];   // 图标路径 (手机中)
    /*WCHAR*/wchar_t        szPurchaser[ MOBILE_PACKAGE_NAME_LEN ];       // 购买者
    /*UINT32*/unsigned int      eAppType;           // 软件分类 1 pxl 2 ipa 3 installer 4 panda 5 deb
    /*BOOL*/bool        bShare;             // 程序是否有共享目录
    /*WCHAR*/wchar_t       szShareFolder[ /*MAX_PATH*/260]; // 购买者
	/*WCHAR*/wchar_t		szIosVersion[MOBILE_PACKAGE_NAME_LEN]; //IOS的版本号

} MOBILE_APP_INFO, *PMOBILE_APP_INFO;

// 设备硬件信息
typedef struct _MOBILE_DEVICE_INFO
{
	_MOBILE_DEVICE_INFO()
	{
		memset(this,0,sizeof(_MOBILE_DEVICE_INFO));
		_nState = 0;
	};

	/*WCHAR*/wchar_t szHardID[MOBILE_BASE_INFO_LEN];
	/*WCHAR*/wchar_t szProviderName[MOBILE_BASE_INFO_LEN];
	/*WCHAR*/wchar_t szDriverDesc[MOBILE_BASE_INFO_LEN];

	/*WCHAR*/wchar_t szDriverUrl[/*MAX_PATH*/260];

	unsigned long _nState;
}MOBILE_DEVICE_INFO, *PMOBILE_DEVICE_INFO;