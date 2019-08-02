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

    // һ̨�豸����������
    class CLASSINDLL_CLASS_DECL DeviceData
    {
    friend class DeviceManager;
    friend class AndroidUsbConnection;
    friend class AndroidWifiConnection;
    public:

        // ȡID
        std::wstring     GetId();

        // Ӳ����Ϣ
		const DeviceInfo*     GetDevInfo() const;

		//�豸���Ӻ��ȡ�豸�Ļ�����Ϣ
		bool AndroidReadDeviceInfo();

		//�ֱ��� mac��ַ imei �Ƿ�root
		bool AndroidReadDeviceOtherInfo();

        // ȡӲ����Ϣ����д
        DeviceInfo*     GetMutableDevInfo()
        {
            return &_DeviceInfo;
        }

        MonitorThread* GetMonitorThread() const
        {
            return _pMonitorThread;
        }

		AndroidMusicManager*			    GetMusicManager();    // ����

		PictureManager*				GetPictureManager();	// ͼƬ

		ScreenShotManager*          GetScreenShotManager();	// ��Ļ��ͼ����

		ThemeManager*               GetThemeManager();		// �������

		VideoManager*               GetVideoManager();		// ��Ƶ����

		RecordingManager*           GetRecordingManager();	// ¼������


        ContactManager*             GetContactManager();
        MessageManager*             GetMessageManager();	// ���Ź���
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

        // ���ӣ�ȡӲ����Ϣ
		bool Connect();

        // �����ʱ�������ϣ���������װ�ػ��ȹ���
        bool Activate();
		int IntActivate();

        // �Ͽ�ָ���豸���Ͽ����ӣ�
        void Disconnect();

        // ���� IO ʵ���Ĺ�������
        BaseIO* CreateIO() const;

        // �����ػ���Ϣ������
        void Attach(DeamonMessageObserver* pObserver);

        // �Ƴ��ػ���Ϣ������
        void Detach(DeamonMessageObserver* pObserver);

		// �豸�Ƿ��Ѽ���
		bool IsActivate() const;

        // �ֹ������豸����״̬   by QiuChang 2012.08.05
        void SetActive(bool val) { _bActivated = val; }

		// ��ȡAndroid���ػ�ģʽ
		int GetAndroidDeamonMode() const;

		// ��ȡIos���ػ�ģʽ
		int GetIosDeamonMode() const;

		//��Ҫ����ɾ������ָ��
		RootData* GetRootData();

		bool SupportColor() const;

        // ��ǰ�豸�Ƿ��ֹ����װ,Ҳ�������豸���������濪�������Ƴ���װ.wifi���ӵ��Ƿ�����жϲ�?
        bool IsProhibitAppInstall();

        // ����·��
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

		// ������������
		void StartMonitor();

        //�����������ӵ�ʵ��
        IConnection* CreateConnection();

        //��ȡAndroid Wifi ģʽ�µ�Ӳ����Ϣ������
        void GetAndroidProperty();

        // �ͷ������ڴ�
        void ReleaseAll();

		// ���豸�Ѿ��Ͽ����¼�����Ҫ���ڴ���ײ㷵�صĶϿ��¼�
		void OnDeviceDisconnect();

		void ReadSdcardPath();

    private:
		RootData*		_pRootData;			// �豸��Ŀ¼

		int				_nAndroidDeamonMode;// �ػ�ģʽ
		int				_nIosDeamonMode;	// �ػ�ģʽ

		bool			_bActivated;		// �Ƿ��Ѿ�����

        std::wstring     _id;

        DeviceInfo      _DeviceInfo;        // �豸Ӳ�������Ϣ

        ConnectionPool* _pConnectionPool;    // ���ӳ�

        MonitorThread*  _pMonitorThread;    // ��������߳�

		AndroidMusicManager*              _pMusicManager;  // ����

		PictureManager*            _pPictureManager; // ͼƬ

		ScreenShotManager*		   _pScreenManager; // ��Ļ��ͼ
		RecordingManager*          _pRecordingManager;  // ��Ƶ����

       
        Path*                        _pPath;    // ��ʱĿ¼���豸���·��

		bool                        _supportColor;


        ContactManager*             _pContactManager;
        MessageManager*			    _pMessageManager;  // ����Ϣ
        CallManager*                _pCallManager;
        CalendarManager*            _pCalendarManager;
        NoteManager*                _pNoteManager;
        AlarmManager*               _pAlarmManager;
        BookmarkManager*            _pBookmarkManager;

        ThemeManager*               _pThemeManager;  // �������
        VideoManager*               _pVideoManager;  // ��Ƶ����
        RingManager*                _pRingManager;

        SystemManager*              _pSystemManager;
		ServerManager*				_pServerManager;
        FileManager*                _pFileManager;
		BackupAndRestoreManager*	_pBackupAndRestoreManager;
    private:
        AppManager* _pAppManager;
		AppManagerEx* _pAppManagerEx;
        wstring     _shareAppIdentifier;// ��ǰ�豸�����ĳ�����Ŀ¼
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
