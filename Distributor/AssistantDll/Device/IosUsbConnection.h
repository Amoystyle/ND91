///////////////////////////////////////////////////////////
//  IosUsbConnection.h
//  Implementation of the Class IosUsbConnection
//  Created on:      01-六月-2011 9:42:58
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_)
#define EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_

#include "IConnection.h"
#include "Core/DeviceInfo.h"
#include "AssistantDll.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

namespace ND91Assistant
{
#define PASSWORD_PROTECTED_CODE -402653158

	typedef struct IosDiskMemoInfo
	{
		/// 总的磁盘空间
		INT64 nTotalDiskCapacity;

		/// 总系统空间
		INT64 nTotalSystemCapacity;

		/// 总的系统可用空间
		INT64 nTotalSystemAvailable;

		/// 总数据空间
		INT64 nTotalDataCapacity;

		/// 总可用数据空间 = 保留的数据空间 + 可用数据空间
		INT64 nTotalDataAvailable;

		/// 保留的数据空间
		INT64 nAmountDataReserved;

		/// 可用的数据空间
		INT64 nAmountDataAvailable;

		/// 应用程序占用
		INT64 nMobileApplicationUsage;

		IosDiskMemoInfo()
		{
			nTotalDiskCapacity = 0;
			nTotalSystemCapacity = 0;
			nTotalSystemAvailable = 0;
			nTotalDataCapacity = 0;
			nTotalDataAvailable = 0;
			nAmountDataReserved = 0;
			nAmountDataAvailable = 0;
			nMobileApplicationUsage = 0;
		}
	}IosDiskMemoInfo;

    enum RelayType
    {
        AppleSupport,
        Network,
        VPN,
        WiFi,
        UserDatabases,
        CrashReporter,
        tmp,
        SystemConfiguration
    };

    // 与守护程序通信（ios设备 usb连接）
    class IosUsbConnection : public IConnection
    {
    public:
        IosUsbConnection(DeviceInfo* devInfo);//(void *dev);
        virtual ~IosUsbConnection();

		// 激活准备
		virtual bool PreActivate(){return true;};

		// 激活
		virtual bool Activate();

		virtual bool Connect();
		virtual void Disconnect();
        virtual const DeviceInfo* GetDeviceInfo();

        // 当前设备是否禁止程序安装,也就是在设备的设置里面开启了限制程序安装.wifi连接的是否可以判断不?
        virtual bool IsProhibitAppInstall();

		// 连接到某个应用程序的共享文档目录
		// strIdentifier		: 该应用程序标识符
		// 返回					: 成功为TURE并将服务通信句柄和服务句柄存入DeviceInfo内，失败为FALSE
		// 注					: 暂定为每台手机仅对应一个服务，切换应用程序时候会将上一个服务关闭
		// 固件3.x开始才有这个功能
		bool Connect2AppSharingDirectory(wstring strIdentifier);

        // 安装服务启动(多线程操作需配合锁使用)
        bool StartInstallationProxyService();
        // 安装服务停止
        void StopInstallationProxyService();

		//由于设置图标状态后需关闭此服务，后面使用再重新开启，故添加此函数
		//构造函数中需传入设备句柄_ios_devptr及_deviceProductVersion，返回的ServiceSocket由调用处自行释放
		bool StartSpringBoardService(void** ServiceSocket);

		// Summary  : 获取电池电量
		// Returns  : int 电池电量值（如80，则电量为80%）
		int GetBetteryState();

		// Summary  : 获取设备磁盘信息
		// Returns  : bool 获取成功返回TRUE
		// Parameter: IosDiskMemoInfo & diskInfo ：存储磁盘信息
		bool GetDiskMemoInfo(IosDiskMemoInfo& diskInfo);

		// Summary  : 修改设备名称
		// Returns  : bool 成功返回true
		// Parameter: wstring strName ：修改后的名称
		bool SetDeviceName(wstring strName);

		// Summary  : 获取设备是否锁定
		// Returns  : bool true锁定
		bool GetIsDeviceLock(){return _bDeviceLock;}

        // Summary  : 获取文件
        // Returns  : bool
        // Parameter: RelayType eType 
        // Parameter: wstring strSavePath
        bool FileRelay(RelayType eType, wstring strSavePath);

		// Summary  : 启动IOS服务
		// Returns  : int 启动返回码
		// Parameter: char* service 服务名称
		// Parameter: void** ServiceSocket 套接字指针
		int StartIOSService(char* service, void **ServiceSocket);

		//进入恢复模式
		int EnterRecoveryMode();

        // Summary  : 停止IOS服务
        // Returns  : void 
        // Parameter: void** pSocket 要停止的套接字指针
        void StopIOSService(void** pSocket);

		//使用诊断模式来重启服务
		bool RebootWithDiagnosticsRelay();

		//使用诊断模式来关闭服务
		bool ShutDownWithDiagnosticsRelay();

		// Summary  : 发送数据到iDevice设备
		// Returns  : bool 是否获取到数据
		// Parameter: void* pSocket 套接字指针
		// Parameter: map<string,string> mapMsg 存放数据的容器
		bool SendMessageToService(void* pSocket, map<string, void*> mapMsg);
		bool SendMessageToService(void* pSocket, map<string, string> mapMsg);
        bool SendMessageToService(void* pSocket, vector<string> vecMsg);
        bool SendMessageToService(void* pSocket, vector<void*> vecMsg);

        // Summary  : 从iDevice设备接收数据
        // Returns  : bool 是否成功发送数据
        // Parameter: void* pSocket 套接字指针
        // Parameter: map<string, string> & mapMsg 用来接收数据的容器
        bool ReadMssageFromService(void* pSocket, map<string, string> &mapMsg);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg);
		bool ReadMssageFromService(void* pSocket, vector<void*> &vecMsg);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map< string, map<string,void*> > &mapEntities);
		bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds, map<string,string> &mapEntities);
        bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds);
		bool ReadMssageFromService(void* pSocket, vector<string> &vecMsg, wstring path);

        // Summary  : 判断某个DataClass是否禁止和PC同步
        // Returns  : bool
        // Parameter: string dataClass 数据类型
        // Parameter: bool tryEnableIfDisabled 如果禁止,尝试开启
        bool IsDataClassDisableTetheredSync(string dataClass, bool tryEnableIfDisabled);

        // Summary  : 判断某个DataClass是否开启iCloud或者其他IMAP 邮件账户同步
        // Returns  : bool
        // Parameter: string dataClass 数据类型
        bool IsDataClassEnableiCloud(string dataClass);

		// 通过硬件型号如n90ap获取设备DeviceType 如iPhone3,1
		// 如n90ap,iphone4
		// 一些5.x设备lockdown获取DeviceType有问题;因此手工维护这张表
		// http://theiphonewiki.com/wiki/index.php?title=Models
		wstring GetDeviceTypeByHardwareModel(wstring strHardware);

		//判断设备是否越狱 
		int IsJailBreak();

        // 检测守护是否运行，并获得加密key
        bool PhoneDaemonIsStillRunning();

    private:
        enum DEVICE_COMMAND_TYPE
        {
            Device_Connect,
            Device_Disconnect,
            Device_Pair,
            Device_UnPair,
            Device_Validate_Pairing,
            Device_Start_Session,
            Device_Stop_Session,
			Device_is_Pair,
        };

        int iTunesCommand(DEVICE_COMMAND_TYPE command);

        // 取设备信息数据
        void GetDevInfo();

        // 启动对话
		int StartSession();

        // 停止对话
		int StopSession();

        // 取int类型的设备属性
        /*INT64*/int64_t GetDeivceValueAsInt64(const char* pDomainKey, const char* pKey);

		// 取byte类型的设备属性的base64编码串
		/*INT64*/wstring GetDeivceValueAsBase64(const char* pDomainKey, const char* pKey);

        // 取string类型的设备属性
		wstring GetDeviceValueAsString(const char* pDomainKey, const char* pKey);

        // 取map类型的设备属性
        map<string, string>* GetDeviceValueAsDict(const char* pDomainKey, const char* pKey);

        // Summary  : 判断某个DataClass是否禁止和PC同步
        // Returns  : bool
        // Parameter: map<string,string> mapDataClass 例如 "com.apple.mobile.tethered_sync","com.apple.bookmarks.Bookmark"
        bool IsDisableTethered(map<string, string> mapDataClass);

        // Summary  : 是否开启iCloud,开启则表示不让同步.
        // Returns  : bool
        // Parameter: string dataClass 数据类型
        bool IsDataSyncContainiCloudSources(string dataClass);

        // 取设备ID
        wstring GetDeviceIdentifier();

        // 调用iTunes_AFCConnectionOpen
        void OpenConnection();

	private:		
		bool                    _sessionStarted;       // 对话是否开启
		DeviceInfo*             _pDeviceInfo;          // 设备信息
		bool                    _bDeviceLock;          // 设备是否锁定
        map<string, string>     _mapDeviceInfo;        // 设备返回的信息容器
		CRITICAL_SECTION        _lock; 
    };
}

#endif // !defined(EA_6D93A705_E3B8_43d8_8C3E_380CA26E0552__INCLUDED_)
