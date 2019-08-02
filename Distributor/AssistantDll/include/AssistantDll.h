// AssistantDll.h
#pragma once
// 在project的编译属性中 ，定义了_ASSISTANT_DLL宏。使用此DLL的代码，无此宏就自动为dllimport
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


// 设置信息的名称
#define ANDROID_RINGTONE			 "ringtone"              // 当前来电铃声
#define ANDROID_NOTIFICATION_SOUND   "notification_sound"     // 当前短信铃声
#define ANDROID_ALARM_SOUND			 "alarm_sound"            // 当前闹钟铃声
#define ANDROID_CURRENT_PROFILE		 "current_profile"        // 当前情景模式

#define BACKUP_DATAFORMAT_VERSION       1                       // 备份数据的格式版本
namespace ND91Assistant
{
	/// 表示传输结果。
	enum TransferResult
	{
		Transfer_Success,			/// 成功完成。
		Transfer_Fail,				/// 失败。
		Transfer_LackOfSpace,		/// 空间不足。
		Transfer_Cancel,            /// 取消。
		Transfer_Transfering,		/// 传输中。
		Transfer_Unknown			/// 手机没有正常响应，无法确定结果。
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

	//通过iTunes接口获取应用程序类型Ipa类型(未越狱)
	//2.0中只用到User和System
	typedef enum eApplicationType
	{
		eApplicationType_User,
		eApplicationType_System,
		/// '内部的,还有其他的'?
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
		APP_TYPE_IPA,	//未越狱只有此类型，通过ITUNES获取的也都是此类型
		APP_TYPE_DEB,	//有分系统和用户的
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
        PHONE_ANDROIDTemp,//疑是Android
	};

	enum ThemeAppIconType
	{
		android_AppIcon_contacts_Dialtacts=0, //打电话                                        
		android_AppIcon_contacts_DialtactsContactsEntry, //联系人                                        
		android_AppIcon_browser_Browser,//浏览器                                       
		android_AppIcon_google_maps_Maps, //地图                                        
		android_AppIcon_clock_dial, //闹钟主图                                           
		android_AppIcon_clock_hour, // 时钟
		android_AppIcon_clock_minute// 分钟
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
        COMMAND_EXECUTE_FAIL = 0,       // 结果失败
        COMMAND_EXECUTE_SUCCESS = 1,    // 结果成功
        COMMAND_EXECUTE_CANCEL,         // 取消
        COMMAND_EXECUTE_BREAK,          // 执行失败
        COMMAND_EXECUTE_UNSUPPORT,      // 不支持
        COMMAND_EXECUTE_NOFOUND,        // 找不到可操作的数据
        COMMAND_EXECUTE_ARGUMENTERROR,  // 参数错误
		COMMAND_EXECUTE_PARSEPKGERROR,	// 解析包出错
        COMMAND_THREAD_OVER_LIMIT,      // 并发超过限制
		COMMAND_EXECUTE_SUCCESS_AuthorizorFaile,//授权失败
        COMMAND_EXECUTE_UNKNOWN,        // 未知结果
		COMMADN_DEAMON_NO_RUN,			//守护没有运行
    };

    enum InstallState    
    {
        STATE_INSTALL   = 0,     // 可以安装
        STATE_UNINSTALL = 1,     // 可以卸载
        STATE_UNNEED    = 2,     // 不需要拼音排序
    };

    /// 还原结果。
    enum RestoreResult
    {
        Restore_Success,			// 成功
        Restore_Fail,				// 失败
        Restore_Cancel,             // 取消
        Restore_DeviceDisconnect,   // 设备断开
        Restore_NoSDcard,           // SDCARD不存在
        Restore_UploadFileError,    // 上传文件错误
        Restore_NoHome,             // 没安装桌面
        Restore_NoPandarReader,     // 没安装熊猫看书
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