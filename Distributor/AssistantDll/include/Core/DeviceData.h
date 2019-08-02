#pragma once

#include "AssistantDll.h"
#include "DeviceInfo.h"

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    class Path;
    class IosRingPath;
    class AndroidPath;
    class IosPicturePath;
    class ThemePath;
    class IosRecordingPath;

	class RootData;
	class RingDirData;
	class DirData;
	class AndroidMusicManager;
	class PictureManager;
	class ScreenShotManager;
	class ThemeManager;
	class ThemeManager;
	class VideoManager;    
	class RecordingManager;
	class BookMarkFolder;

    class ContactManager;
	class MessageManager;
    class CallManager;
    class CalendarManager;
    class NoteManager;
    class AlarmManager;
    class BookmarkManager;

    class RingManager;
    class SystemManager;
    class FileManager;
	class ServerManager;

	class BaseIO;
	class MonitorThread;
	class IConnection;
	class ConnectionPool;
	class AndroidUsbConnection;
	class DeamonMessageObserver;
	class BackupAndRestoreManager;

    class AppManager;
	class AppManagerEx;

	struct SpaceInfo;

    // 一台设备的所有数据
    class CLASSINDLL_CLASS_DECL DeviceData
    {
    friend class DeviceManager;
    friend class AndroidUsbConnection;
    friend class AndroidWifiConnection;
    public:

        // 取ID
        std::wstring     GetId();

        // 硬件信息
		const DeviceInfo*     GetDevInfo() const;

		//设备连接后读取设备的基础信息
		bool AndroidReadDeviceInfo();

		//分辨率 mac地址 imei 是否root
		bool AndroidReadDeviceOtherInfo();

        // 取硬件信息：可写
        DeviceInfo*     GetMutableDevInfo()
        {
            return &_DeviceInfo;
        }

        MonitorThread* GetMonitorThread() const
        {
            return _pMonitorThread;
        }

		AndroidMusicManager*			    GetMusicManager();    // 音乐

		PictureManager*				GetPictureManager();	// 图片

		ScreenShotManager*          GetScreenShotManager();	// 屏幕截图管理

		ThemeManager*               GetThemeManager();		// 主题管理

		VideoManager*               GetVideoManager();		// 视频管理

		RecordingManager*           GetRecordingManager();	// 录音管理


        ContactManager*             GetContactManager();
        MessageManager*             GetMessageManager();	// 短信管理
        CallManager*                GetCallManager();   
        CalendarManager*            GetCalendarManager();
        NoteManager*                GetNoteManager();
        AlarmManager*               GetAlarmManager();
        BookmarkManager*            GetBookmarkManager();
        RingManager*                GetRingManager();
        SystemManager*              GetSystemManager();
        FileManager*                GetFileManager();
		BackupAndRestoreManager*	GetBSManager();
		ServerManager*				GetServerManager();

        // 连接，取硬件信息
		bool Connect();

        // 激活（此时已连接上）：包括安装守护等工作
        bool Activate();
		int IntActivate();

        // 断开指定设备（断开连接）
        void Disconnect();

        // 创建 IO 实例的工厂方法
        BaseIO* CreateIO() const;

        // 增加守护消息接收者
        void Attach(DeamonMessageObserver* pObserver);

        // 移除守护消息接收者
        void Detach(DeamonMessageObserver* pObserver);

		// 设备是否已激活
		bool IsActivate() const;

        // 手工设置设备激活状态   by QiuChang 2012.08.05
        void SetActive(bool val) { _bActivated = val; }

		// 获取Android的守护模式
		int GetAndroidDeamonMode() const;

		// 获取Ios的守护模式
		int GetIosDeamonMode() const;

		//需要外面删除返回指针
		RootData* GetRootData();

		bool SupportColor() const;

        // 当前设备是否禁止程序安装,也就是在设备的设置里面开启了限制程序安装.wifi连接的是否可以判断不?
        bool IsProhibitAppInstall();

        // 各种路径
        const Path*             GetPath             () const ;
        const IosRingPath*      GetIosRingPath      () const ;
        const AndroidPath*      GetAndroidPath      () const ;
        const IosPicturePath*   GetIosPicturePath   () const ;
        const ThemePath*        GetThemePath        () const ;
		const IosRecordingPath* GetIosRecordingPath () const ;

        void SetIosDeamonInstalled(bool b);

        CRITICAL_SECTION _csAndroidCommandLock;
		CRITICAL_SECTION _csLock;

		void ResetConnect();
    private:
        DeviceData(const std::wstring id, const DeviceInfo* pInfo);
        ~DeviceData();

		// 启动监听进程
		void StartMonitor();

        //创建处理连接的实例
        IConnection* CreateConnection();

        //获取Android Wifi 模式下的硬件信息，可能
        void GetAndroidProperty();

        // 释放所有内存
        void ReleaseAll();

		// 该设备已经断开的事件，主要用于处理底层返回的断开事件
		void OnDeviceDisconnect();

		void ReadSdcardPath();

    private:
		RootData*		_pRootData;			// 设备根目录

		int				_nAndroidDeamonMode;// 守护模式
		int				_nIosDeamonMode;	// 守护模式

		bool			_bActivated;		// 是否已经激活

        std::wstring     _id;

        DeviceInfo      _DeviceInfo;        // 设备硬件相关信息

        ConnectionPool* _pConnectionPool;    // 连接池

        MonitorThread*  _pMonitorThread;    // 心跳检测线程

		AndroidMusicManager*              _pMusicManager;  // 音乐

		PictureManager*            _pPictureManager; // 图片

		ScreenShotManager*		   _pScreenManager; // 屏幕截图
		RecordingManager*          _pRecordingManager;  // 视频管理

       
        Path*                        _pPath;    // 临时目录与设备相关路径

		bool                        _supportColor;


        ContactManager*             _pContactManager;
        MessageManager*			    _pMessageManager;  // 短消息
        CallManager*                _pCallManager;
        CalendarManager*            _pCalendarManager;
        NoteManager*                _pNoteManager;
        AlarmManager*               _pAlarmManager;
        BookmarkManager*            _pBookmarkManager;

        ThemeManager*               _pThemeManager;  // 主题管理
        VideoManager*               _pVideoManager;  // 视频管理
        RingManager*                _pRingManager;

        SystemManager*              _pSystemManager;
		ServerManager*				_pServerManager;
        FileManager*                _pFileManager;
		BackupAndRestoreManager*	_pBackupAndRestoreManager;
    private:
        AppManager* _pAppManager;
		AppManagerEx* _pAppManagerEx;
        wstring     _shareAppIdentifier;// 当前设备开启的程序共享目录
		bool		_bRetry;

    public:
        AppManager* GetAppManager();
		AppManagerEx* GetAppManagerEx();
        wstring  GetShareAppIdentifier() { return _shareAppIdentifier; }
        void SetShareAppIdentifier(wstring shareAppIdentifier) { _shareAppIdentifier = shareAppIdentifier; }
		void SetConnectRetry( bool bRetry );
		bool IsConnectRetry(){ return _bRetry; }

		int GetBatteryLever();
		
		void GetDeviceSpaceInfo(SpaceInfo &spinfo,bool bSdCard);
    };
}
