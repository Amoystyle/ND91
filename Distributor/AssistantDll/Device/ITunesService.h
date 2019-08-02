#pragma once

#ifdef WIN32
	#include "ITunesMobileDevice.h"
	#include "AirTrafficHost.h"
#else
	#include "MobileDevice.h"
#endif
#include "IConnection.h"
#include <string>
#include <sstream>
using namespace std;

namespace ND91Assistant
{
    void OnAMDConnectionNotify(struct am_device_notification_callback_info *info);
    void OnDfuDeviceConnected(void * device);
    void OnDfuDeviceDisconnected(void * device);
    void OnrecoveryConnected(void * device);
    void OnrecoveryDisconnected(void * device);	

	typedef enum iTunesVersionType
	{
        // <summary>
        // 10.5开始的iTunes官方支持设备wifi连接,设备固件需要10.5
        // <remarks>注意这个枚举常量用于iTunesWifi是否自动连接判断,修复需注意</remarks>
        // </summary>
        iTunes105,
		iTunes92,
		iTunes91,
		iTunes82,
		iTunesOld
	};

	//
	// ios设备的环境初始化：启动iTunes服务并设置回调函数
	// 
	class CITunesService
	{
	public:
		//初始化IOS环境
		static bool InitIosEnviroment();
		static bool RegisterIosDeviceCallback();
		static void UnRegisterIosDeviceCallback();

		//获取ITUNES版本
		static iTunesVersionType GetiTunesVersionType(){return _itunesType;}

		static void InitStateInfo(); //初始化状态信息

		static void ReConnPasswordLockDevice(void* pConnHandle);

	private:		
		// 回调函数：正常状态下态，每台设备调用一次回调，拔插都会
		friend void OnAMDConnectionNotify(struct am_device_notification_callback_info *info);

		// 回调函数：Dfu状态
		friend void OnDfuDeviceConnected(void * device);
		friend void OnDfuDeviceDisconnected(void * device);

		// 回调函数：recovery状态
		friend void OnrecoveryConnected(void * device);
		friend void OnrecoveryDisconnected(void * device);	

		// 检查 iTunesMobileDevice.dll 文件版本号
		static void CheckiTunesMobileDeviceVersion(wstring path);

	private:
		// IOS环境初始化是否完成
		static bool _bIosEvnInited;

		// iTunesMobileDevice.dll 文件版本号
		static iTunesVersionType _itunesType;

		// 回调函数是否已设置
		static bool _bCallbackInited;

		// 连接设备（从回调函数中调用）
		static void Connect(ND91Assistant::IConnection* pConn, void * device);

		// 断开连接（从回调函数中调用）
		static void Disconnect(ND91Assistant::IConnection* pConn, void * device);

        // 连接设备（从回调函数中调用）
        static void DFUConnect(ND91Assistant::IConnection* pConn, void * device);

        // 断开连接（从回调函数中调用）
        static void DFUDisConnect(ND91Assistant::IConnection* pConn, void * device);

        // 连接设备（从回调函数中调用）
        static void RecoveryConnect(ND91Assistant::IConnection* pConn, void * device);

        // 断开连接（从回调函数中调用）
        static void RecoveryDisConnect(ND91Assistant::IConnection* pConn, void * device);

		static void* _subscribe_info_ptr;

		static bool _bDevConnAlready; //注册回调后是否有设备连接过
	};
}

