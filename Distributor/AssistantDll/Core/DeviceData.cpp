#include "stdafx.h"

#include "Core/DeviceData.h"
#include "Common/Path.h"

#include "Device/IConnection.h"
#include "Device/ConnectionPool.h"

#include "Monitor/IosMonitorThread.h"
#include "Monitor/AndroidMonitorThread.h"

#include "Device/IosFileIO.h"
#include "Device/AndroidTcpIO.h"
#include "Device/IosUsbConnection.h"
#include "Device/IosRecoveryConnection.h"
#include "Device/AndroidUsbConnection.h"
#include "Device/AndroidWifiConnection.h"

#include "Module/File/DirData.h"
#include "Module/File/RootData.h" 
#include "Module/File/FileManager.h" 
#include "Module/File/DirectoryOperationCommand.h"
#include "Module/Application/AppManager.h"
#include "Module/Application/AppManagerEx.h"
#include "Module/System/SystemManager.h"
#include "Module/ScreenShot/ScreenShotManager.h"
#include "Device/AdbForwardTcpCommand.h"
#include "Device/AndroidAdbHelper.h"

#ifndef ASSISTANT_LITE
#include "Module/Music/AndroidMusicManager.h"
#include "Module/Picture/IosPictureManager.h"
#include "Module/Picture/AndroidPictureManager.h"
#include "Module/Record/RecordingManager.h"
#include "Module/Contact/ContactManager.h"
#include "Module/Message/MessageManager.h"
#include "Module/Call/CallManager.h"
#include "Module/Calendar/CalendarManager.h"
#include "Module/Theme/IosThemeManage.h"
#include "Module/Theme/AndroidThemeManager.h"
#include "Module/Video/IosVideoManager.h"
#include "Module/Video/AndroidVideoManager.h"
#include "Module/Note/NoteManager.h"
#include "Module/Note/IosNoteManager.h"
#include "Module/Bookmark/IosBookmarkManager.h"
#include "Module/Bookmark/AndroidBookmarkManager.h"
#include "Module/Alarm/AlarmManager.h"
#include "Module/Ring/RingManager.h"
#include "Module/BackupAndRestore/BackupAndRestoreManager.h"
#include "Module/Server/ServerManager.h"
#endif

DeviceData::DeviceData(const std::wstring id, const DeviceInfo* pInfo)
: _id(id)
, _DeviceInfo(*pInfo)
{
	_bActivated             = false;

	// 等连接完成之后再根据情况创建正确的监听线程实例
	_pMonitorThread         = NULL;
    _pConnectionPool        = NULL;
	_pMusicManager          = NULL;   
	_pPictureManager        = NULL;
	_pScreenManager         = NULL;
	_pRootData              = NULL;
	_pRecordingManager      = NULL;
    _pAppManager            = NULL;

    _pContactManager        = NULL;
    _pMessageManager	    = NULL; 
    _pCallManager           = NULL;

    _pThemeManager          = NULL;
    _pVideoManager          = NULL;
    _pRingManager           = NULL;
	_pNoteManager			= NULL;
	_pBookmarkManager		= NULL;

    _pSystemManager         = NULL;
	_pServerManager			= NULL; 	
    _pFileManager           = NULL;
    _pBackupAndRestoreManager = NULL;
	_pAppManagerEx			= NULL;
	_bRetry					= false;

	_nAndroidDeamonMode     = 1;
	_nIosDeamonMode         = 0;
	_supportColor           = false;
	OPTION->Get(GlobalOption::INT_ANDROID_DEAMON_MODE, _nAndroidDeamonMode);
	OPTION->Get(GlobalOption::INT_IOS_DEAMON_MODE, _nIosDeamonMode);

	LOG->WriteDebug(CCodeOperation::StringToWstring("生成DeviceData:") + _id);

    _pPath = new Path(&_DeviceInfo);

    InitializeCriticalSection(&_csAndroidCommandLock);
	InitializeCriticalSection(&_csLock);
}

std::wstring ND91Assistant::DeviceData::GetId()
{
    return _id;
}

DeviceData::~DeviceData()
{
	LOG->WriteDebug(CCodeOperation::StringToWstring("销毁DeviceData:") + _id);

	SAFE_DELETE(_pRootData);
//	 CFileOperation::DeleteDirectory(_pPath->GetWorkFolder());
    SAFE_DELETE(_pPath);
    ReleaseAll();   
    SAFE_DELETE(_pConnectionPool); //关闭连接池	
	LOG->WriteDebug(CCodeOperation::StringToWstring("完成销毁DeviceData:") + _id);
	
	DeleteCriticalSection(&_csAndroidCommandLock);

	SAFE_DELETE(_pMonitorThread);
	SAFE_DELETE(_pConnectionPool);
	SAFE_DELETE(_pMusicManager);
	SAFE_DELETE(_pPictureManager);
	SAFE_DELETE(_pScreenManager);
	SAFE_DELETE(_pRootData);
	SAFE_DELETE(_pRecordingManager);
	SAFE_DELETE(_pAppManager);

	SAFE_DELETE(_pContactManager);
	SAFE_DELETE(_pMessageManager);
	SAFE_DELETE(_pCallManager);

	SAFE_DELETE(_pThemeManager);
	SAFE_DELETE(_pVideoManager);
	SAFE_DELETE(_pRingManager);
	SAFE_DELETE(_pNoteManager);
	SAFE_DELETE(_pBookmarkManager);

	SAFE_DELETE(_pSystemManager);
	SAFE_DELETE(_pServerManager);
	SAFE_DELETE(_pFileManager);
	SAFE_DELETE(_pBackupAndRestoreManager);
	SAFE_DELETE(_pAppManagerEx);
}

bool DeviceData::IsProhibitAppInstall()
{
    IConnection *pConn = CreateConnection();
    bool ret = pConn->IsProhibitAppInstall();
    delete pConn;
    return ret;
}

// 连接设备
bool DeviceData::Connect()
{
	SetCSLock(_csLock);
    IConnection *pConn = CreateConnection();
    if (!pConn) return false;
    bool ret = pConn->Connect();
    delete pConn;
    return ret;
}   
//读取初始化信息供初始话后外部使用
bool DeviceData::AndroidReadDeviceInfo()
{
	try
	{
		IConnection *pConn = CreateConnection();
		pConn->InitDeviceData();
		delete pConn;

		// TODO: 取空间之类基础数据
        if (_DeviceInfo._deviceType == Android)
            ReadSdcardPath(); 
	}
	catch(...)
	{

		LOG->WriteError(L"AndroidReadDeviceInfo func error!");
		return false;
	}
	return true;
}

bool DeviceData::AndroidReadDeviceOtherInfo()
{
	if ( Android == _DeviceInfo._deviceType )
	{
		if ( USB == _DeviceInfo._connectType)
		{
			AndroidUsbConnection* pConn = new AndroidUsbConnection(this);
			try
			{
				pConn->InitDeviceOtherData();
				return true;
			}
			catch(...)
			{
				return false;
			}
		}
	}
	return false;
};

bool DeviceData::Activate()
{
	return IntActivate()==1;
}
// 激活设备
int DeviceData::IntActivate()
{
    SetCSLock(_csLock);

    //if (_bActivated)
    //    return 1;

	int retryCount =0;
	int retCode=1;

	//激活失败可能adb有问题或者守护启动的不正常,这时候重试3次
FailRetry:
    IConnection *pConn = CreateConnection();
    if(!pConn)
        return false;
    if(!pConn->PreActivate())
    {
        //启动守护失败
        _bActivated = false;
		retCode = 2;
		LOG->WriteDebug(L"Activate Dev Error code=2");
		goto end;
    }

    try
    {
        //心跳线程要先启动，以保证后续的操作可以成功
        if ((_DeviceInfo._deviceType == Ios && _nIosDeamonMode) ||
            (_DeviceInfo._deviceType == Android && _nAndroidDeamonMode) )
            StartMonitor();
		 _bActivated = pConn->Activate();
	}	
	catch (AssistantException ex)
	{
		if(retryCount<3)
		{
			//断开连接
			pConn->Disconnect();
			//停止adb
			AndroidAdbHelper aah;
			aah.stopServer();
			//删除pConn重新创建
			delete pConn;
			retryCount++;
			goto FailRetry;
		}
		else
		{
			_bActivated = false;
			retCode = ex.GetErrorCode();
			LOG->WriteDebug(L"Activate Dev Error code=" + CStrOperation::IntToWString(retCode));
			goto end;
		}
	}


end:
    delete pConn;

    return retCode;

}

// 断开连接
void DeviceData::Disconnect()
{
    IConnection *pConn = CreateConnection();
    pConn->Disconnect();
    delete pConn;

	_bActivated = false;

//    ReleaseAll();
}

void DeviceData::StartMonitor()
{
	if (_DeviceInfo._deviceType == Android && _nAndroidDeamonMode == 0) 		
		return ;
	if(_DeviceInfo._deviceType == Ios && _nIosDeamonMode == 0)
	{

		return;
	}

	if (! _pMonitorThread)
	{
        if ( _DeviceInfo._deviceType == Android)
        {
            if (_DeviceInfo._connectType == USB || _DeviceInfo._connectType == WIFI)
                _pMonitorThread = new AndroidMonitorThread(&_DeviceInfo, _id);
            else
                THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
        }
        else if ( _DeviceInfo._deviceType == Ios)
        {
            if (_DeviceInfo._connectType == USB || _DeviceInfo._connectType == WIFI)
                _pMonitorThread = new IosMonitorThread(&_DeviceInfo, _id);
            else
                THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
        }
        else
            THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
	}

	// 初始化连接，并启动监听线程
	int nLongConnectPort = 0;

    // 需要等待一段时候让端口映射生效；取决于手机的速度
    Sleep(50);
    int retryCount = 0;
    int nTickBegin = ::GetTickCount();
    while ( retryCount++ < 40 )
    {
        try
        {
			if ( _pMonitorThread )
				 nLongConnectPort = _pMonitorThread->StartThread();
            break;
        }
        catch (AssistantException& )
        {
            Sleep(200);
        }
        if (GetTickCount() - nTickBegin > 10000)
        {
            // 最高15秒后重试退出
            Sleep(300); //减少重试次数
            retryCount = 40;
            break;
        }
		//设置端口映射
		if (_DeviceInfo._nPort == 0)
			_DeviceInfo._nPort = CBaseSocket::getActiveSocketPort();
		AdbForwardTcpCommand forwardCommand(_DeviceInfo._strSerialNumber,_DeviceInfo._ios_deviceIdentifier, _DeviceInfo._nPort, 5900);
		forwardCommand.Execute();
    }

    if ( retryCount >= 40 ) // 原本在AndroidMonitorThread::StartThread里是sleep五秒
    {
        LOG->Write(Log::LOG_DEBUG, L"AndroidMonitorThread::StartThread(): retry count = %d", retryCount);
        THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
    }

	// 设置长连接值
	_DeviceInfo._nLongConnectSign = (short)nLongConnectPort;
}


void DeviceData::Attach( DeamonMessageObserver* pObserver )
{
    if ( _pMonitorThread )
        _pMonitorThread->Attach(dynamic_cast<IObserver*>(pObserver));
}

void DeviceData::Detach( DeamonMessageObserver* pObserver )
{
    if ( _pMonitorThread )
        _pMonitorThread->Detach(dynamic_cast<IObserver*>(pObserver));
}

BaseIO* DeviceData::CreateIO() const
{
    BaseIO* pIO = NULL;

    if ( _DeviceInfo._deviceType == Ios )
    {
		if( _DeviceInfo._connectType == USB)
            pIO = new IosFileIO(&_DeviceInfo);
		else if (_DeviceInfo._connectType == WIFI )
            pIO = new IosFileIO(_DeviceInfo._ios_wifi_handle);
		else
            THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
	}
    else if ( _DeviceInfo._deviceType == Android )
	{
		if ( _DeviceInfo._connectType == USB ||  _DeviceInfo._connectType == WIFI)
		{
			// 连接池必须先初始化
			if (!_pConnectionPool)
				THROW_ASSISTANT_EXCEPTION(DEVICE_NOT_ACTIVE);

			// 注意pBaseSocket这个指针，将在 ~BaseIO() 中退回给连接池
			int nConnectionID = _pConnectionPool->Get();
			pIO = new AndroidTcpIO(nConnectionID, _pConnectionPool);
		}
		else
			THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
	}
	else
        THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);

    return pIO;
}

IConnection* DeviceData::CreateConnection()
{
    IConnection* pConn = NULL;
    if (Ios == _DeviceInfo._deviceType)
    {
        if ( USB == _DeviceInfo._connectType)
            pConn = new IosUsbConnection(&_DeviceInfo);
        else if ( WIFI == _DeviceInfo._connectType )
            pConn = new IosWifiConnection(_DeviceInfo._ios_deviceIP, _DeviceInfo._ios_devicePSW, _DeviceInfo._ios_wifi_handle);
        else if ( RECOVERY == _DeviceInfo._connectType )
            pConn = new IosRecoveryConnection(&_DeviceInfo);
        else
            THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
    }
    else if ( Android == _DeviceInfo._deviceType )
    {
        if ( USB == _DeviceInfo._connectType)
            pConn = new AndroidUsbConnection(this);
        else if ( WIFI == _DeviceInfo._connectType )
            pConn = new AndroidWifiConnection(this);
        else
            THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
    }
    else
        THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);

    return pConn;
}


#pragma region 一堆Get方法

const DeviceInfo* DeviceData::GetDevInfo() const
{
    return &_DeviceInfo;
}

AndroidMusicManager* ND91Assistant::DeviceData::GetMusicManager()
{	
#ifdef WIN32	
#ifdef MODULE_MUSIC
	if(!_pMusicManager)
		_pMusicManager = new AndroidMusicManager(this);
#endif
#endif
	return _pMusicManager;
}

#ifdef MODULE_PICTURE
PictureManager* ND91Assistant::DeviceData::GetPictureManager()
{
	if(!_pPictureManager)
	{
		switch (this->GetDevInfo()->_deviceType)
		{
		case Ios:
			_pPictureManager = new IosPictureManager(this);
			break;
		case Android:
			_pPictureManager = new AndroidPictureManager(this);
			break;
		default:
			break;
		}
	}
	return  _pPictureManager;
}
#endif

ScreenShotManager* ND91Assistant::DeviceData::GetScreenShotManager()
{
#ifdef MODULE_SCREENSHOT
	if(!_pScreenManager)
		_pScreenManager = new ScreenShotManager(this);
#endif
	return _pScreenManager;

}
#ifdef MODULE_THEME
ThemeManager* ND91Assistant::DeviceData::GetThemeManager()
{
	if(!_pThemeManager)
	{
        switch (this->GetDevInfo()->_deviceType)
        {
        case Ios:
            _pThemeManager = new IosThemeManage(this);
            break;
        case Android:
            _pThemeManager = new AndroidThemeManager(this);
            break;
        default:
            break;
        }
	}
	return _pThemeManager;
}
#endif
VideoManager* ND91Assistant::DeviceData::GetVideoManager()
{
#ifdef MODULE_VIDEO
	if(!_pVideoManager)
    {
        switch (this->GetDevInfo()->_deviceType)
        {
        case Ios:
            _pVideoManager = new IosVideoManager(this);
            break;
        case Android:
            _pVideoManager = new AndroidVideoManager(this);
            break;
        default:
            break;
        }
    }
#endif
	return _pVideoManager;
}
RecordingManager* ND91Assistant::DeviceData::GetRecordingManager()
{
#ifdef MODULE_RECORD
	if(!_pRecordingManager)
		_pRecordingManager = new RecordingManager(this);
#endif
	return _pRecordingManager;
}

RootData* DeviceData::GetRootData()
{
	if (!_pRootData)
		_pRootData = new RootData(this);

//     if (_shareAppIdentifier.empty())
//         _pRootData->GetDir()->_name = L"/";
//     else
//         _pRootData->GetDir()->_name = L"/Documents/";

	return _pRootData;
}

void ND91Assistant::DeviceData::OnDeviceDisconnect()
{
	_bActivated = false;
	_DeviceInfo._nPort = 0;
	if (_pMonitorThread)
		_pMonitorThread->StopThread(); //退出监听线程
// 	SAFE_DELETE(_pConnectionPool); //关闭连接池	
}

bool ND91Assistant::DeviceData::SupportColor() const
{
	return _supportColor;
}

#ifdef MODULE_CONTACT
ContactManager* ND91Assistant::DeviceData::GetContactManager()
{
    if(!_pContactManager)
        _pContactManager = new ContactManager(this);
    return _pContactManager;
}
#endif

#ifdef MODULE_MESSAGE
MessageManager* ND91Assistant::DeviceData::GetMessageManager()
{
	if(!_pMessageManager)
		_pMessageManager = new MessageManager(this);
	return _pMessageManager;
}
#endif

#ifdef MODULE_CALL
CallManager* ND91Assistant::DeviceData::GetCallManager()
{
    if(!_pCallManager)
        _pCallManager = new CallManager(this);
    return _pCallManager;
}
#endif
#ifdef MODULE_CALENDAR
CalendarManager* ND91Assistant::DeviceData::GetCalendarManager()
{
    if (!_pCalendarManager)
        _pCalendarManager = new CalendarManager(this);
    return _pCalendarManager;
}
#endif
#ifdef MODULE_NOTE
NoteManager* ND91Assistant::DeviceData::GetNoteManager()
{

	if(!_pNoteManager)
	{
		switch (this->GetDevInfo()->_deviceType)
		{
		case Ios:
			_pNoteManager = new IosNoteManager(this);
			break;
		case Android:
			_pNoteManager = new NoteManager(this);
			break;
		default:
			break;
		}
	}

    return _pNoteManager;
}
#endif
#ifdef MODULE_ALARM
AlarmManager* ND91Assistant::DeviceData::GetAlarmManager()
{
    if (!_pAlarmManager)
        _pAlarmManager = new AlarmManager(this);
    return _pAlarmManager;
}
#endif
#ifdef MODULE_BOOKMARK
BookmarkManager* ND91Assistant::DeviceData::GetBookmarkManager()
{
    if (!_pBookmarkManager)
	{
		switch (this->GetDevInfo()->_deviceType)
		{
		case Ios:
			_pBookmarkManager = new IosBookmarkManager(this);
			break;
		case Android:
			_pBookmarkManager = new AndroidBookmarkManager(this);
			break;
		default:
			break;
		}
	}
    return _pBookmarkManager;
}
#endif
void DeviceData::ReadSdcardPath()
{
	if(!_DeviceInfo._sdcardpath.empty())
		return;
	_supportColor = DirectoryOperationCommand(this).IsSupportColor();

	_DeviceInfo._sdcardpath = L"/sdcard/";

	DirData* pdir = new DirData(this);
	pdir->_name = L"/";
	if((!pdir->Refresh() || pdir->GetChildrens().empty()) && _supportColor)
	{
		_supportColor = false;
		pdir->Refresh();
	}
	NodeData* pnode = pdir->findChildrenbyName(L"sdcard");
	if(pnode)
	{
		_DeviceInfo._sdcardpath = pnode->GetFullPath();
	}
	SAFE_DELETE(pdir);
	return;
}

#pragma endregion

void DeviceData::ReleaseAll()
{
    SAFE_DELETE ( _pMonitorThread );
    SAFE_DELETE ( _pConnectionPool);
#ifdef MODULE_MESSAGE
    SAFE_DELETE ( _pMessageManager );
#endif
#ifdef MODULE_MUSIC
	SAFE_DELETE ( _pMusicManager );
#endif
#ifdef MODULE_PICTURE
	SAFE_DELETE ( _pPictureManager );
#endif
#ifdef MODULE_SCREENSHOT
	SAFE_DELETE ( _pScreenManager);
#endif
#ifdef MODULE_THEME
	SAFE_DELETE ( _pThemeManager );
#endif
	SAFE_DELETE ( _pRootData	);
#ifdef MODULE_VIDEO
	SAFE_DELETE ( _pVideoManager);
#endif
#ifdef MODULE_RECORD
	SAFE_DELETE ( _pRecordingManager);
#endif
#ifdef MODULE_SYSTEM
    SAFE_DELETE ( _pSystemManager);
#endif
#ifdef MODULE_RING
    SAFE_DELETE ( _pRingManager );
#endif
#ifdef MODULE_NOTE
	SAFE_DELETE ( _pNoteManager );
#endif
#ifdef MODULE_SERVER
	SAFE_DELETE ( _pServerManager);
#endif
//     for( vector<ContactData*>::iterator it = _contactDataList.begin(); it != _contactDataList.end(); it++)
//     {
//         delete *it;
//     }
//     _contactDataList.clear();
//     for( vector<ContactAccount*>::iterator it = _contactAccountList.begin(); it != _contactAccountList.end(); it++)
//     {
//         delete *it;
//     }
//     _contactAccountList.clear();
//     for( vector<ContactGroupData*>::iterator it = _contactGroupList.begin(); it != _contactGroupList.end(); it++)
//     {
//         delete *it;
//     }
//     _contactGroupList.clear();
}

bool DeviceData::IsActivate() const
{
	return _bActivated;
}

int DeviceData::GetAndroidDeamonMode() const
{
	return _nAndroidDeamonMode;
}
int DeviceData::GetIosDeamonMode() const
{
	return _nIosDeamonMode;
}

#ifdef MODULE_APP
AppManager* ND91Assistant::DeviceData::GetAppManager()
{
    if (!_pAppManager)
        _pAppManager = new AppManager(this);
    return _pAppManager;
}
AppManagerEx* ND91Assistant::DeviceData::GetAppManagerEx()
{
	if (!_pAppManagerEx)
		_pAppManagerEx = new AppManagerEx(this);
	return _pAppManagerEx;
}
#endif
#ifdef MODULE_RING
RingManager* ND91Assistant::DeviceData::GetRingManager()
{
    if (!_pRingManager)
        _pRingManager = new RingManager(this);
    return _pRingManager;
}
#endif
#ifdef MODULE_SYSTEM
SystemManager* ND91Assistant::DeviceData::GetSystemManager()
{
    if (!_pSystemManager)
        _pSystemManager = new SystemManager(this);
    return _pSystemManager;
}
#endif
FileManager* ND91Assistant::DeviceData::GetFileManager()
{
    if (!_pFileManager)
        _pFileManager = new FileManager(this);
    return _pFileManager;
}
#ifdef MODULE_BACKUPANDRESTORE
BackupAndRestoreManager* ND91Assistant::DeviceData::GetBSManager()
{
	if (!_pBackupAndRestoreManager)
		_pBackupAndRestoreManager = new BackupAndRestoreManager(this);
	return _pBackupAndRestoreManager;
}
#endif
void ND91Assistant::DeviceData::SetIosDeamonInstalled( bool b )
{
    _DeviceInfo._ios_daemonInstalled = b;
}
#ifdef MODULE_SERVER
ServerManager* ND91Assistant::DeviceData::GetServerManager()
{
	if (!_pServerManager)
		_pServerManager = new ServerManager(this);
	return _pServerManager;
}

void ND91Assistant::DeviceData::ResetConnect()
{
	if (_pConnectionPool)
		_pConnectionPool->ReleaseAll();
}

void ND91Assistant::DeviceData::SetConnectRetry( bool bRetry )
{
	_bRetry = bRetry;
}

#endif

const Path*             DeviceData::GetPath             () const { return _pPath; }
const IosRingPath*      DeviceData::GetIosRingPath      () const { return _pPath->GetPIosRingPath(); }
const AndroidPath*      DeviceData::GetAndroidPath      () const { return _pPath->GetAndroidPath(); }
const IosPicturePath*   DeviceData::GetIosPicturePath   () const { return _pPath->GetPIosPicturePath(); }
const ThemePath*        DeviceData::GetThemePath        () const { return _pPath->GetPThemePath(); }
const IosRecordingPath* DeviceData::GetIosRecordingPath () const { return _pPath->GetPIosRecordingPath(); }

int ND91Assistant::DeviceData::GetBatteryLever()
{
	return this->GetSystemManager()->GetBatteryLever();
}

void ND91Assistant::DeviceData::GetDeviceSpaceInfo(SpaceInfo &spinfo,bool bSdCard)
{
	RootData* rootdata = GetRootData();
	if(!rootdata)
		return;

	if(GetDevInfo()->_deviceType ==Android)
	{
		if(bSdCard)
			rootdata->GetSpaceInfo(RootData::SDCARD,&spinfo);
		else
			rootdata->GetSpaceInfo(RootData::PHONEDATA,&spinfo);
	}
	else
	{
		rootdata->GetSpaceInfo(RootData::PHONEDATA,&spinfo);
	}
}