///////////////////////////////////////////////////////////
//  AppData.h
//  Implementation of the Class AppData
//  Created on:      01-六月-2011 9:42:49
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_)
#define EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_

#include "Module/BaseData.h"
#include "Core/DeviceData.h"
#include "Common/IosPngConver.h"
#include <deque>
#include <string>
#include <vector>
using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
#define PXL_APP_INFO_FOLDER					L"/private/var/root/Media/PXL/DB/"
#define IPA_APP_INFO_FILE					L"/private/var/mobile/Library/Caches/com.apple.mobile.installation.plist"
#define IPA_APP_INFO_FILE_NAME				L"ipaList.plist"
#define INSTALLER_APP_INFO_FOLDER			L"/private/var/mobile/Library/Installer/"// 低版本的存放文件的路径不同
#define INSTALLER_APP_INFO_FOLDER_LOW		L"/private/var/root/Library/Installer/"
#define INSTALLER_APP_INFO_FILE_NAME		L"LocalPackages.plist"
#define PANDA_APP_INFO_FILE					L"/private/var/root/Media/TQINSTALLER/installed.plist"
#define PANDA_APP_INFO_FILE_NAME			L"ndInstaller.plist"
#define DEB_APP_INFO_FILE					L"/private/var/lib/dpkg/status"
#define DEB_APP_INFO_FILE_NAME				L"cydialist"
#define IPA_APP_METADATA_FILE				L"iTunesMetadata.plist"

    class CLASSINDLL_CLASS_DECL AppData : public BaseData
    {
    friend class GetAppListCommand;
	friend class GetAppInfoCommand;
	friend class AdbPMListCommand;
    public:
        AppData();
		AppData(const AppData* pAppData);
        virtual ~AppData();

		/// <summary>
		/// apk配置文件上要求安装的路径
		/// android:installLocation=["auto" | "internalOnly" | "preferExternal"] >
		/// </summary>
		
		enum AndroidInstallLocation
		{
			None = -1,           //版本是2.2一下没有这个属性或者在2.2版本不设置该属性
			Auto = 0,           //0 The application may be installed on the external storage, but the system will install the application on the internal storage by default. If the internal storage is full, then the system will install it on the external storage. Once installed, the user can move the application to either internal or external storage through the system settings.
			InternalOnly = 1,       // 1 The application must be installed on the internal device storage only. If this is set, the application will never be installed on the external storage. If the internal storage is full, then the system will not install the application. This is also the default behavior if you do not define android:installLocation.
			PreferExternal = 2,       // 2 The application prefers to be installed on the external storage (SD card). There is no guarantee that the system will honor this request. The application might be installed on internal storage if the external media is unavailable or full, or if the application uses the forward-locking mechanism (not supported on external storage). Once installed, the user can move the application to either internal or external storage through the system settings.
		};

		// 安装包信息
		std::wstring _strAppPCPath;		// 安装包路径 (PC中) 
		bool _bLockApp;				// 安装时是否要锁定App
		bool _bRecover;				// 是否覆盖原程序
		bool _bOnSDCard;			// 是否安装在SDCard
		bool _bKeepData;			// 是否保留用户数据 (卸载时使用)
		//AndroidInstallLocation   _APKInstallLocation; // apk配置文件上要求安装的路径


		// 程序信息
		wstring _packageName;		// 安装包名 ISO表示程序的唯一标识
        int     _nVersionCode;      // android内部版本号，与升级相关                 
		wstring _strVersion;		// 版本描述                   
		wstring _AppName;			// 应用程序名             
		wstring _Description;		// 描述
		wstring _Url;				// 链接地址
		wstring _strAppPhonePath;	// 程序路径 (手机中)              

		wstring		_strIconPhonePath;	// 图标路径 (手机中)
		wstring		_strIconPCPath;		// 图标路径 (PC中)
		APP_TYPE	_eAppType;			// 软件分类（pxl,ipa,installer,panda,deb）
		APP_OWNER	_eAppOwner;			// 程序类型（系统，用户）
		bool		_bShare;			// 文件是否共享
		wstring		_strState;          // 程序状态
		wstring		_ExtName;			//扩展名-安装exe文件时用

		wstring _strPurchaser;          // 购买者
        int64_t _applicationDSID;       // 购买者ID 数字化的

		wstring _DebEssential;
		wstring _DebDepends;
		vector<wstring> _DeviceFamily;//支持设备列表 1 ipod itouch iphone 2 ipad
		wstring _sdkVer;//使用开发包版本
		int _isLegalSoft;//是否正版
		deque<wstring>  _PermissionList;     // 权限列表
		
        int64_t _dynamicDiskUsage;      // 文档大小
        int64_t _staticDiskUsage;       // 程序大小
        wstring _minimumOSVersion;      // 最低固件版本要求

		wstring _strExecutable;//包名

        // 是否是91账户购买的IPA,是的话,进行相应的授权
        bool _is91IPA;

		bool _isEnterpriseApp;//是否是企业级应用

        // 91IPA相关账户信息
        wstring _acountInfo;

		int _itemId; //itunes id

        wstring _strBundleExecutableName;
        wstring _strIconName;

	public:

		// 获取程序图标,并存为文件(strPCPath)，返回的图标文件由界面负责删除
		// 要保持的图标名
		bool LoadAppIcon(const DeviceData* pDeviceData, wstring strPCPath);
		// 载入程序的权限信息
		bool LoadPermissions(const DeviceData* pDeviceData);
        // 获取应用的权限列表
        bool GetPermissions(vector<wstring>& permissionList);
		// 查看是否用户程序
		bool IsUserApp();
        wstring GetAppExt();

        // 获取应用购买者
        bool GetAppPurchaser(const DeviceData* pDeviceData);

    private:
        size_t ParseAndroidData(const char* pData, size_t nSize);
		bool   ParseAndroidAdbData(string strLine);

        // 获取应用ICON路径
        bool GetAppIconPhonePath(const DeviceData* pDeviceData);

    };
}

#endif // !defined(EA_7330FA27_D4AD_40d8_A454_2C41E4A6C565__INCLUDED_)
