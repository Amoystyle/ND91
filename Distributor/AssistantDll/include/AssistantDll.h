// AssistantDll.h
#pragma once
// ��project�ı��������� ��������_ASSISTANT_DLL�ꡣʹ�ô�DLL�Ĵ��룬�޴˺���Զ�Ϊdllimport
#ifdef WIN32
    #ifdef _ASSISTANT_DLL 
    #define CLASSINDLL_CLASS_DECL     __declspec(dllexport) 
    #else 
    #define CLASSINDLL_CLASS_DECL     __declspec(dllimport) 
    #endif 
#else
    #ifdef _ASSISTANT_DLL 
    #define CLASSINDLL_CLASS_DECL
    #else 
    #define CLASSINDLL_CLASS_DECL
    #endif
#endif 


#ifndef WIN32
    typedef signed char         INT8, *PINT8;
    typedef signed short        INT16, *PINT16;
    typedef signed int          INT32, *PINT32;
    typedef int64_t             INT64, *PINT64;
    typedef unsigned char       UINT8, *PUINT8;
    typedef unsigned short      UINT16, *PUINT16;
    typedef unsigned int        UINT32, *PUINT32;
    typedef uint64_t            UINT64, *PUINT64;

    #define CITunesMobileDevice MobileDevice
    #define CCFConverter        CFWrapper
    #include <stdint.h>
#endif

#define SAFE_DELETE(p)          if ( p ) { delete p; p=NULL; }
#define SAFE_DELETE_ARRAY(p)    if ( p ) { delete[] p; p=NULL; }

#define RETURN_IF(x)            if ( x ) { return; }
#define RETURN_VALUE_IF(x, y)   if ( x ) { return y; }
#define RETURN_FALSE_IF(x)      if ( x ) { return false; }
#define RETURN_TRUE_IF(x)       if ( x ) { return true; }
#define RETURN_NULL_IF(x)       if ( x ) { return NULL; }

#define DUMP_PNG(pImage, name) {\
	GUID png = {0x557CF406, 0x1A04, 0x11D3, 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E};\
	pImage->Save(name, &png, NULL); \
}

#define DUMP_IMAGE(pImage, name) {\
    GUID jpg = {0x557CF401, 0x1A04, 0x11D3, 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E};\
    pImage->Save(name, &jpg, NULL); \
}

#define RELEASE_VECTOR(v)  {for (size_t i =0; i < v.size(); i++) { SAFE_DELETE(v[i]);} v.clear(); }


// ������Ϣ������
#define ANDROID_RINGTONE			 "ringtone"              // ��ǰ��������
#define ANDROID_NOTIFICATION_SOUND   "notification_sound"     // ��ǰ��������
#define ANDROID_ALARM_SOUND			 "alarm_sound"            // ��ǰ��������
#define ANDROID_CURRENT_PROFILE		 "current_profile"        // ��ǰ�龰ģʽ

#define BACKUP_DATAFORMAT_VERSION       1                       // �������ݵĸ�ʽ�汾
namespace ND91Assistant
{
	/// ��ʾ��������
	enum TransferResult
	{
		Transfer_Success,			/// �ɹ���ɡ�
		Transfer_Fail,				/// ʧ�ܡ�
		Transfer_LackOfSpace,		/// �ռ䲻�㡣
		Transfer_Cancel,            /// ȡ����
		Transfer_Transfering,		/// �����С�
		Transfer_Unknown			/// �ֻ�û��������Ӧ���޷�ȷ�������
	};

	enum SDCardState
	{
		REMOVED,
		UNMOUNTED,
		CHECKING,
		NOFS,
		MOUNTED,
		MOUNTED_READ_ONLY,
		SHARED,
		BAD_REMOVAL,
		UNMOUNTABLE,
		UNKNOW
	};

	//ͨ��iTunes�ӿڻ�ȡӦ�ó�������Ipa����(δԽ��)
	//2.0��ֻ�õ�User��System
	typedef enum eApplicationType
	{
		eApplicationType_User,
		eApplicationType_System,
		/// '�ڲ���,����������'?
		eApplicationType_Internal,
		/// System+User
		eApplicationType_Any,
		eApplicationType_Unknow
	};

	enum APP_TYPE
	{
		APP_TYPE_APK,
		APP_TYPE_NPK,
		APP_TYPE_PXL,
		APP_TYPE_IPA,	//δԽ��ֻ�д����ͣ�ͨ��ITUNES��ȡ��Ҳ���Ǵ�����
		APP_TYPE_DEB,	//�з�ϵͳ���û���
		APP_TYPE_INSTALLER,
		APP_TYPE_PANDA,
		APP_TYPE_ALL,
		APP_TYPE_UNKNOW,
		APP_TYPE_EXE,
        APP_TYPE_APPSYNC,
	};
	enum APP_OWNER
	{
		APP_OWNER_USER,
		APP_OWNER_SYS,
		APP_OWNER_INTERNAL,
		APP_OWNER_ANY,
		APP_OWNER_UNKNOW,
	};

	enum HARDWARE_TYPE
	{
		PHONE_UNKNOWN,
		PHONE_IPHONE,
		PHONE_ANDROID,
        PHONE_ANDROIDTemp,//����Android
	};

	enum ThemeAppIconType
	{
		android_AppIcon_contacts_Dialtacts=0, //��绰                                        
		android_AppIcon_contacts_DialtactsContactsEntry, //��ϵ��                                        
		android_AppIcon_browser_Browser,//�����                                       
		android_AppIcon_google_maps_Maps, //��ͼ                                        
		android_AppIcon_clock_dial, //������ͼ                                           
		android_AppIcon_clock_hour, // ʱ��
		android_AppIcon_clock_minute// ����
	};

	enum MediaFileType
	{
		MEDIA_AUDIO_UNKNOWN,
		MEDIA_AUDIO_AAC,
		MEDIA_AUDIO_OGG,
		MEDIA_AUDIO_WAV,
		MEDIA_AUDIO_MP3,
		MEDIA_AUDIO_M4A,
		MEDIA_AUDIO_M4R,
		MEDIA_VIDEO_WMV,
		MEDIA_AUDIO_WMA,
		MEDIA_VIDEO_GP3,
		MEDIA_VIDEO_MP4,
		MEDIA_AUDIO_M4V,
		MEDIA_VIDEO_AVI,
		MEDIA_VIDEO_REAL
	};

    enum CommandExecuteResult
    {
        COMMAND_EXECUTE_FAIL = 0,       // ���ʧ��
        COMMAND_EXECUTE_SUCCESS = 1,    // ����ɹ�
        COMMAND_EXECUTE_CANCEL,         // ȡ��
        COMMAND_EXECUTE_BREAK,          // ִ��ʧ��
        COMMAND_EXECUTE_UNSUPPORT,      // ��֧��
        COMMAND_EXECUTE_NOFOUND,        // �Ҳ����ɲ���������
        COMMAND_EXECUTE_ARGUMENTERROR,  // ��������
		COMMAND_EXECUTE_PARSEPKGERROR,	// ����������
        COMMAND_THREAD_OVER_LIMIT,      // ������������
		COMMAND_EXECUTE_SUCCESS_AuthorizorFaile,//��Ȩʧ��
        COMMAND_EXECUTE_UNKNOWN,        // δ֪���
		COMMADN_DEAMON_NO_RUN,			//�ػ�û������
    };

    enum InstallState    
    {
        STATE_INSTALL   = 0,     // ���԰�װ
        STATE_UNINSTALL = 1,     // ����ж��
        STATE_UNNEED    = 2,     // ����Ҫƴ������
    };

    /// ��ԭ�����
    enum RestoreResult
    {
        Restore_Success,			// �ɹ�
        Restore_Fail,				// ʧ��
        Restore_Cancel,             // ȡ��
        Restore_DeviceDisconnect,   // �豸�Ͽ�
        Restore_NoSDcard,           // SDCARD������
        Restore_UploadFileError,    // �ϴ��ļ�����
        Restore_NoHome,             // û��װ����
        Restore_NoPandarReader,     // û��װ��è����
    };

#ifndef _CSHARPDLL
 	class NDMobileSrvCommu;
 	class NDMobileMgrCommu;

    #ifdef WIN32
 	    extern "C" __declspec(dllexport) NDMobileSrvCommu* CreateInstanceSrvCommu();
 	    extern "C" __declspec(dllexport) NDMobileMgrCommu* CreateInstanceMgrCommu();
    #else
        extern "C"  NDMobileSrvCommu* CreateInstanceSrvCommu();
        extern "C"  NDMobileMgrCommu* CreateInstanceMgrCommu();
    #endif
#endif
}