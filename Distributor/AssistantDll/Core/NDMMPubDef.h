#pragma once

#ifdef WIN32
    #include <windows.h>
    typedef UINT64 uint64_t;
#else
    #include <inttypes.h>
    #include <string.h>
#endif

#define UNFIND_DEVICE_ERROR 1				//δ�ҵ��豸
#define PRODUCT_VERSION_NOT_AGREE_ERROR 2   //�̼��汾������Ҫ��
#define UNINSTALL_APPSYNC_ERROR 3			//δ��װAPPSYNC
#define UNFIND_APP_ERROR 4					//δ�ҵ�Ӧ��
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

    /*INT32*/signed int nMusic; // ����
    /*INT32*/signed int nVideo; // ��Ƶ
    /*INT32*/signed int nPhoto; // ͼƬ
    /*INT32*/signed int nBook;  // ������

}SDCARD_FILE_INFO, *PSDCARD_FILE_INFO;

//��λ�����1,����Ҫ���������ö�
#define PHONE_NONE				0x00000000		//Ԥ��
#define PHONE_CONNECTED			0x00000001		//һ�������������ֻ������
#define PHONE_CONNECTING		0x00000002		//һ������ʧ�ܵ��ֻ������
#define	PHONE_CONNECT_FIAL		0x00000003		//һ������ʧ�ܵ��ֻ����������򵼵Ĳ������ǲ������ӳɹ���������Ҫ��ʾ������ʧ�ܡ�
#define PHONE_REMOVE			0x00000004		//һ���ֻ����Ƴ�
#define PHONE_CONNECT_NODEAMON	0x00000005		//�ֻ����������ӵ�ûװdeamon

//�豸����
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

	/*DWORD*/unsigned long				nDeviceType;					  // �豸������  DEVICE_IOS/DEVICE_ANDROID
	/*DWORD*/unsigned long				nMobileState;                     // �ֻ�״̬ PHONE_CONNECTED/PHONE_CONNECTING/PHONE_CONNECT_FIAL/PHONE_REMOVE 
	SDCARD_FILE_INFO	sdCardInfo;                       // sd�����ļ���Ϣ
	/*INT32*/signed int				uSystemApp;						  // ϵͳӦ������
	/*INT32*/signed int				uDataApp;						  // ������Ӧ������
	/*INT32*/signed int				uGameApp;						  // ��ϷӦ������
	/*INT32*/signed int				uContact;						  // ��ϵ������
	/*INT32*/signed int				uSms;							  // ��������
	/*INT32*/signed int				uMms;							  // ��������
	/*INT32*/signed int				uVersionDaemon;					  // Daemon�汾
	/*INT32*/signed int				uSdk;							  // �ֻ�SDK
	DISK_INFO			dskMemory;						  // �ֻ��ڴ�
	DISK_INFO			dskDisk;						  // �ֻ�Ӳ��
	DISK_INFO			dskSdcard;						  // sd ��

	/*UINT*/unsigned int			uConnType;						  // ���ӷ�ʽ 0 USB������ 1 WIFI
	/*UINT*/unsigned int			uBattery;						  // ��ص���
	/*WCHAR*/wchar_t				szCPU[MOBILE_BASE_INFO_LEN];      // CPU ��Ϣ
	/*WCHAR*/wchar_t				szManu[MOBILE_BASE_INFO_LEN];     // ����
	/*WCHAR*/wchar_t				szModel[MOBILE_BASE_INFO_LEN];    // �ֻ��ͺ�
	/*WCHAR*/wchar_t				szSerial[MOBILE_BASE_INFO_LEN];   // �ֻ����к�
	/*WCHAR*/wchar_t				szImei[MOBILE_BASE_INFO_LEN];     // �ֻ�IMEI��
	/*WCHAR*/wchar_t				szOSBuild[MOBILE_BASE_INFO_LEN];  // ϵͳ�汾��
	/*WCHAR*/wchar_t		        szUid[MOBILE_BASE_INFO_LEN];	  // �ֻ�vid+pid
	/*WCHAR*/wchar_t				wszImageUrl[ URL_MAX_LEN ];       // �ֻ�ͼƬ
	/*WCHAR*/wchar_t				wszVendorImageUrl[ URL_MAX_LEN ]; // ����ͼƬ	
	/*INT32*/signed int				nReverse[10];					  // ������
	/*WCHAR*/wchar_t				wszReverse[ URL_MAX_LEN ];		  // ����

	/*BOOL*/bool                    bJailBreaked;                     //�Ƿ�Խ��
	/*WCHAR*/wchar_t                szDevName[MOBILE_BASE_INFO_LEN];  //�豸��
	/*INT32*/signed int             uDeviceType;                      // �豸����: 1 ios 2 android


}MOBILE_BASE_INFO, *PMOBILE_BASE_INFO;

//typedef struct _MOBILE_BASE_INFO
//{
//    DWORD				cbSize;
//
//    DWORD				nMobileState;                     // �ֻ�״̬ PHONE_CONNECTED/PHONE_CONNECTING/PHONE_CONNECT_FIAL/PHONE_REMOVE 
//    SDCARD_FILE_INFO	sdCardInfo;                       // sd�����ļ���Ϣ
//    INT32				uSystemApp;						  // ϵͳӦ������
//    INT32				uDataApp;						  // ������Ӧ������
//    INT32				uGameApp;						  // ��ϷӦ������
//    INT32				uContact;						  // ��ϵ������
//    INT32				uSms;							  // ��������
//    INT32				uMms;							  // ��������
//    INT32				uVersionDaemon;					  // Daemon�汾
//    INT32				uSdk;							  // �ֻ�SDK
//    DISK_INFO			dskMemory;						  // �ֻ��ڴ�
//    DISK_INFO			dskDisk;						  // �ֻ�Ӳ��
//    DISK_INFO			dskSdcard;						  // sd ��
//
//    UINT				uConnType;						  // ���ӷ�ʽ 0 USB������ 1 WIFI
//    UINT				uBattery;						  // ��ص���
//    WCHAR				szCPU[MOBILE_BASE_INFO_LEN];      // CPU ��Ϣ
//    WCHAR				szManu[MOBILE_BASE_INFO_LEN];     // ����
//    WCHAR				szModel[MOBILE_BASE_INFO_LEN];    // �ֻ��ͺ�
//    WCHAR				szSerial[MOBILE_BASE_INFO_LEN];   // �ֻ����к�
//    WCHAR				szImei[MOBILE_BASE_INFO_LEN];     // �ֻ�IMEI��
//    WCHAR				szOSBuild[MOBILE_BASE_INFO_LEN];  // ϵͳ�汾��
//    WCHAR		        szUid[MOBILE_BASE_INFO_LEN];	  // �ֻ�vid+pid
//    WCHAR				wszImageUrl[ URL_MAX_LEN ];       // �ֻ�ͼƬ
//    WCHAR				wszVendorImageUrl[ URL_MAX_LEN ]; // ����ͼƬ	
//    INT32				nReverse[10];					  // ������
//    WCHAR				wszReverse[ URL_MAX_LEN ];		  // ����
//
//    BOOL                bJailBreaked;                       //�Ƿ�Խ��
//    WCHAR               szDevName[MOBILE_BASE_INFO_LEN];    //�豸��
//    INT32               uDeviceType;                        // �豸����: 1 ios 2 android
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

    /*BOOL*/bool	bInRom;		//	0��װ��SD���У�1Ϊ��װ���ֻ��ڴ�
    /*UINT32*/unsigned int	nFlags;		//	Ӧ�ñ�ʶ; flags=0������Ӧ�ã� flags=1ϵͳӦ��
    /*UINT32*/unsigned int	nVersion;	//	Ӧ�ð汾
    /*UINT64*/uint64_t	tInstall;	//	ts��װʱ��
    /*UINT64*/uint64_t  uAppSize;	//  app ��С
    /*WCHAR*/wchar_t	wszPackageName[ MOBILE_PACKAGE_NAME_LEN ]; 	// �����Ψһ��ʶ  ����
    /*WCHAR*/wchar_t	wszAppName[ MOBILE_PACKAGE_NAME_LEN ]; 		// Ӧ�ó�������
    /*WCHAR*/wchar_t	wszVersionName[ MOBILE_PACKAGE_NAME_LEN ]; 	// �汾��
    /*WCHAR*/wchar_t	wszIconPath[ /*MAX_PATH*/260]; 					// app icon �ı���·��
    /*WCHAR*/wchar_t   wszPermission[/*MAX_PATH*/260*10];					// Ȩ��

    /*WCHAR*/wchar_t        szDescription[URL_MAX_LEN];     // ����
    /*WCHAR*/wchar_t        szUrl[URL_MAX_LEN];             // ���ӵ�ַ
    /*WCHAR*/wchar_t        szAppPhonePath[ /*MAX_PATH*/260];    // ����·�� (�ֻ���)
    /*WCHAR*/wchar_t        szIconPhonePath[ /*MAX_PATH*/260];   // ͼ��·�� (�ֻ���)
    /*WCHAR*/wchar_t        szPurchaser[ MOBILE_PACKAGE_NAME_LEN ];       // ������
    /*UINT32*/unsigned int      eAppType;           // ������� 1 pxl 2 ipa 3 installer 4 panda 5 deb
    /*BOOL*/bool        bShare;             // �����Ƿ��й���Ŀ¼
    /*WCHAR*/wchar_t       szShareFolder[ /*MAX_PATH*/260]; // ������
	/*WCHAR*/wchar_t		szIosVersion[MOBILE_PACKAGE_NAME_LEN]; //IOS�İ汾��

} MOBILE_APP_INFO, *PMOBILE_APP_INFO;

// �豸Ӳ����Ϣ
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