#pragma once

#include "AssistantDll.h"
#include "DeviceData.h"

#include <vector>
#include <map>
#include <string>
using namespace std;

#include "Monitor/DeviceSubject.h"

#define DEVICE_MANAGER  DeviceManager::GetInstance()
#define Wifi_Connect_Psw    L"nd.com.cn"

namespace ND91Assistant
{
	class ISubject;
	class DeviceSubject;
	class IosjailBreakMonitorThread;
    // 91手机助手底层库的入口类
    class CLASSINDLL_CLASS_DECL  DeviceManager : public DeviceSubject
    {
	friend class MonitorThread;
    private:
        static DeviceManager* _pInstance;   // 单例

        DeviceManager();
        virtual ~DeviceManager();
    public:
        // 取得单例对象
        static DeviceManager* GetInstance();

        static void Release();

		//adb是否工作
		bool isAdbWork();

		//重启adb
        void StopAdb();
		bool RestartADB();

        // 增加新设备（需先连接）
        DeviceData* AddDevice(const std::wstring id, const DeviceInfo* pInfo);

        // 删除设备（需先断开连接）
        void RemoveDevice(wstring ID);

		void OnLinkout(wstring ID);

        // 取得所有设备ID的列表
        vector<wstring> GetAllDevice() const;

		//获得ios设备列表
		std::string GetIosDevice() const;

        // 取得设备连接个数
        int GetDeviceCount();
        int GetDeviceCount(ND91Assistant::DEVICE_OS_TYPE nType);

		// 通过ID查找设备
		DeviceData* FindDevice(std::wstring id) const;

        // IOS设备查找：通过USB回调返回的设备句柄查找设备
        DeviceData* FindIosDevice(void* pDevPtr) const;

		//WIFI连接时，通过IP查找设备
		DeviceData* FindIosDevice(wstring strIP) const;

        // ios初始化：环境变量、注册回调函数
        bool IosInit();

        // 扫描USB硬件设备
        // 返回true表示设备有变化，false表示无变化
        bool ScanUsbDevice();

        //连接到指定的IP地址
		// strPreIP : 局域网IP的前两位
		// strCheckCode : 校验码
		bool Wifi_Connect(wstring strPreIP, wstring strCheckCode);
        int Wifi_IsNeedConnectConfirm(wstring strPreIP, wstring  strCheckCode);

        // 心跳线程通知消息
        static void WifiTickCallBack(int nNotify, void* pVoid);
		// Android 设备的wifi连接
        bool Android_Wifi_Connect(wstring strHost);

		//IOS设备的wifi连接
        bool Ios_WiFi_Connect(wstring strIP, wstring strPSW = Wifi_Connect_Psw, int nConfig=-1);

		//移除所有以USB方式连接的IOS设备数据，IOS驱动被卸载掉时用(需先断开连接)
		void RemoveAllIosUSBDevice();

		//注销失去连接的设备
		void ReleaseDisconnectDevice(wstring id);
		void ReleaseAllDisconnectDevice();

		// 获取iTunes下载地址
		wstring GetITunesUrl();

        // 获取RootData
        RootData* GetRootData(const DeviceData* pDeviceData);
		//锁屏设备接入
		 void PassWordLockDeviceConnect(wstring devicename);
		//添加锁屏未输入密码的IOS设备
		void AddPasswordLockIosDevice(void* pConnHandle);

		//移除锁屏未舒服密码的IOS设备
		void RemovePasswordLockIosDevice(void* pConnHandle, bool bReleaseConn = false);

		//重新连接所有锁屏未输入密码的IOS设备
		void ReConnectAllPasswordLockIosDevice();

        // 获取Android设备连接列表（所有状态）
        const map<wstring, wstring>* GetAndroidDeviceMap() { return &_androidDeviceMap;};

		//合适两台设备及以上则弹出设备
		int GetSuitPlateForm(DEVICE_OS_TYPE eType);
    private:
        // 初始化
        void Init();		

        // 删除所有设备数据
        void RemoveAllDevice();

		void OnDeviceDisconnect(wstring id);

		//删除所有锁屏未输入密码的IOS设备
		void RemoveAllPasswordLockIosDevice();

#pragma warning(disable : 4251)     // 禁用警告4251

        typedef map<wstring, DeviceData*>    DEVICE_MAP;

        // 设备列表
        DEVICE_MAP  _devices;

		// 断开连接的设备列表
		vector<DeviceData*>	_disConnectDevices;

		bool _bIosInitSuccess;

		vector<void*> _iosPasswordLockDevices;

        // android 设备列表 <id,连接状态>
        map<wstring, wstring> _androidDeviceMap;

#pragma warning(default : 4251)

		// Android 设备的wifi连接
		bool Android_Wifi_Connect(wstring strHost, wstring strCheckCode);

		void InsertDisconnectDevices(DeviceData* pDeviceData);

		//越狱监控线程
		IosjailBreakMonitorThread* _IosjailBreakMonitorThread;

		DWORD _AdbScanTimer;

	public:
		//adb服务启动命令锁
		CRITICAL_SECTION _csADBSSessionLock;
		CRITICAL_SECTION _csIosInitLock;


    };
}


