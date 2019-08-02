#pragma once
#include "NDMMPubDef.h"
#ifdef WIN32
#else
    #define __stdcall
    #include <unistd.h>
#endif
#define MODULE_360
namespace ND91Assistant
{
	// Srv接口类
	class NDMobileSrvCommu
	{
	private:
		~NDMobileSrvCommu();
	public:
		NDMobileSrvCommu();

		// 回收内存，在进程退出的时候调用
		virtual void __stdcall ReleaseMemory();

		// 销毁接口类
		virtual void __stdcall DestroyInstanceSrvCommu();

		// 判断环境是否就绪
		virtual /*BOOL*/bool __stdcall IosEnvironmentSuc(/*LPWSTR*/wchar_t* pUrl, size_t nSize);

#ifdef MODULE_DRIVER
        // 判断是否Ios设备
        virtual /*BOOL*/bool __stdcall IsIosDevice(/*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID);

		// Qualifier: 获取驱动操作类
		virtual /*BOOL*/bool GetDriverInfo(PMOBILE_DEVICE_INFO lpDeviceInfo, /*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID);
#endif
		// Summary  : 获取电量
		// Returns  : UINT __stdcall 返回电量百分比值，如80，则电量为80%
		// Parameter: LPCWSTR lpSn ：设备唯一标识
		virtual /*UINT*/unsigned int __stdcall GetBatteryLever(/*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : 获取设备状态
		// Returns  : NDASSISTANT_DEVICE_STATE 返回设备状态
		// Parameter: LPCWSTR lpSn ：设备唯一标识
		virtual NDASSISTANT_DEVICE_STATE __stdcall GetDeviceState(/*LPCWSTR*/const wchar_t* lpSn);

		virtual void __stdcall SetLogPath(/*LPCWSTR*/const wchar_t* lpPath);

	private:
		// 连接设备
		// nTime 超时时间
		/*BOOL*/bool ConnectDevice(int nWaitTime, /*LPCWSTR*/const wchar_t* lpSn);
	};
	// Mgr接口类
    class NDMobileMgrCommu
    {
   
	private:
		~NDMobileMgrCommu();

	public:
		NDMobileMgrCommu();

		// 回收内存，在进程退出的时候调用
		virtual void __stdcall ReleaseMemory();

		// 销毁接口实例
		virtual void __stdcall DestroyInstanceMgrCommu();
		// 初始化, 反初始化
        virtual /*BOOL*/bool __stdcall BeginConnect (/*LPCWSTR*/const wchar_t* lpSn); 
        //virtual void __stdcall Uninit();

#pragma region 信息
        
        // Summary  : 获取基本信息
        // Returns  : BOOL 成功返回TRUE
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        // Parameter: PMOBILE_BASE_INFO lpMobileInfo 存储设备信息
        virtual /*BOOL*/bool __stdcall GetBaseInfo(PMOBILE_BASE_INFO lpMobileInfo, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : 获取手机空间信息
		// Returns  : BOOL __stdcall 成功返回TRUE
		// Parameter: PDISK_INFO lpDiskInfo 存储空间信息
		// Parameter: LPCWSTR lpSn ：设备唯一标识
		virtual /*BOOL*/bool __stdcall GetSpaceInfo(PDISK_INFO lpDiskInfo, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取电量
        // Returns  : UINT __stdcall 返回电量百分比值，如80，则电量为80%
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*UINT*/unsigned int __stdcall GetBatteryLever(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取应用个数
        // Returns  : INT  返回应用个数
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetAppCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取音乐个数
        // Returns  : INT  返回音乐个数
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetMusicCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取图片个数
        // Returns  : INT  返回图片个数(若未越狱则不包括壁纸数量)
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetImageCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取视频个数
        // Returns  : INT  返回视频个数
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetVideoCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取电子书个数
        // Returns  : INT 指定目录下的文件个数(不含子目录)
        // Parameter: LPCWSTR lpAppPath ：设备上的目录
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GeteBookCount(/*LPCWSTR*/const wchar_t* lpAppPath, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取联系人个数
        // Returns  : INT  返回联系人个数
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetContactCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取短消息个数
        // Returns  : INT  返回短消息个数
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*INT*/int __stdcall GetMessageCount(/*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : 设置设备名称
		// Returns  : BOOL __stdcall 成功返回TRUE
		// Parameter: LPCWSTR lpNewName ：设备的新名称
		// Parameter: LPCWSTR lpSn ：设备唯一标识
		virtual /*BOOL*/bool __stdcall SetDevName(/*LPCWSTR*/const wchar_t* lpNewName, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

#pragma region 程序

        // Summary  : 安装程序
        // Returns  : UINT __stdcall 成功返回0，否则返回错误代码
        // Parameter: LPCWSTR lpAppPath ：程序的本地路径
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*UINT*/unsigned int __stdcall InstallApp(/*LPCWSTR*/const wchar_t* lpAppPath, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 卸载程序
        // Returns  : UINT __stdcall 成功返回0，否则返回错误代码
        // Parameter: LPCWSTR lpAppId   ：MOBILE_APP_INFO里的lpAppId
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*UINT*/unsigned int __stdcall UnInstallApp( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取所有程序列表
        // Returns  : BOOL 成功返回TRUE
        // Parameter: MOBILE_APP_INFO * * lpAppInfo ：存储程序列表
        // Parameter: int * lpSize ：列表大小
        // Parameter: LPCWSTR lpSn ：设备唯一标识
        virtual /*BOOL*/bool __stdcall GetAllApps( MOBILE_APP_INFO** lpAppInfo, int* lpSize, /*LPCWSTR*/const wchar_t* lpSn); 

        // Summary  : 释放程序列表信息
        // Returns  : BOOL 成功返回TRUE
        // Parameter: MOBILE_APP_INFO * lpAppInfo
        virtual /*BOOL*/bool __stdcall ReleaseAllApps(MOBILE_APP_INFO* lpAppInfo, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : 获取程序图标
        // Returns  : BOOL 成功返回TRUE
        // Parameter: LPCWSTR lpAppId    ：MOBILE_APP_INFO里的szPackageName
        // Parameter: LPCWSTR lpSavePath ：本地保存的名称
        virtual /*BOOL*/bool __stdcall GetPackageIcon( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSavePath, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : PXL转为IPA
		// Returns  : BOOL __stdcall 成功返回TRUE
		// Parameter: LPCWSTR lpSrcPxlFullName ：PXL文件名（含路径）
		// Parameter: LPWSTR lpDesIpaFullName ：转换后的IPA文件名
		// Parameter: int nMaxPath ：转换后的IPA文件名最大长度
		// Parameter: LPCWSTR lpSn ：设备唯一标识
		virtual /*BOOL*/bool __stdcall PxlToIpa(/*LPCWSTR*/const wchar_t* lpSrcPxlFullName, /*LPWSTR*/wchar_t* lpDesIpaFullName, int nMaxPath, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

#pragma region 业务

        // Summary  : 文件上传
        // Returns  : 是否成功
        // Parameter: LPCWSTR lpLocalPath  本地文件名(全路径)
        // Parameter: LPCWSTR lpDevicePath 设备上的目录路径
        // Parameter: LPCWSTR lpSn 设备唯一标识
        virtual /*BOOL*/bool __stdcall UploadFile(/*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpDevicePath, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : 上传壁纸
		// Returns  : 是否成功
		// Parameter: LPCWSTR lpLocalPath 图片文件
		// Parameter: LPCWSTR lpSn 设备唯一标识
		virtual /*BOOL*/bool __stdcall UploadWallpaper(/*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

		virtual void __stdcall SetLogPath(/*LPCWSTR*/const wchar_t* lpPath);
    };
}