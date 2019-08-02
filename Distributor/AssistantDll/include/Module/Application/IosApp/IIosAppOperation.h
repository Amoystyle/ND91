#pragma once

#include "Monitor/ProgressSubject.h"
#include "Module/Application/IosAppXmlHelper.h"

using namespace std;

namespace ND91Assistant
{
	typedef enum eArchiveMode
	{
		eArchiveMode_All, // 备份整个APP所有相关
		eArchiveMode_DocumentsOnly, // 只备份APP对应的Documents(如游戏存档,程序设置等)
		eArchiveMode_ApplicationOnly, // 只备份APP,不备份APP对应的Documents(即备份后类似新的APP)
		eArchiveMode_Unknow,
	};

#define DAEMON_APP_ID_ONE L"netdragon.mobile.iphone.pandadaemon"
#define DAEMON_APP_ID_TWO L"tqdigit.iphone.pandadaemon"

#define IPA_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/ndDaemon/ipa/"
#define IPA_OPERATION_COMMAND_NAME		L"ipa_install"
#define IPA_OPERATION_COMMAND_FAIL_NAME L"ipa_install.f"

#define PXL_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/PXL/Dropoff/"
#define PXL_OPERATION_COMMAND_NAME		L"PxlPickup"

#define DEB_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/ndDaemon/deb/"
#define DEB_OPERATION_COMMAND_NAME		L"debpickup"
#define DEB_OPERATION_COMMAND_FAIL_NAME L"debpickup.f"

#define APPSYNC_OPERATION_COMMAND_FOLDER	L"/var/root/Media/ndDaemon/appsync/"

	class App2ExeInfo;
	//IOS设备的APP操作基类，子类必须实现获取、安装、卸载和备份4个功能函数
	class CLASSINDLL_CLASS_DECL IIosAppOperation
	{
        friend class IosAppOperationFactory;
        friend class AppManager;
	public:
		IIosAppOperation(const DeviceData* pDevice, ProgressSubject& subject = NullProgressSubject::Instance());
		~IIosAppOperation(void);

		//获取指定类型的所有程序的信息列表
		//eType		: 应用程序类型
		virtual bool GetAppInfos(vector<AppData*> &vtInfos) = 0;

		//安装程序
		//strAppFullName: 应用程序PC上的文件名（含路径）
        //string& errorMsg 安装失败返回错误信息
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL) = 0;

		//卸载程序
		//strIdentifier	: 为AppData里的_PackageName
		virtual bool UnInstallApp(wstring strIdentifier) = 0;

		//备份程序
		//strIdentifier	: 为AppData里的_PackageName
		//strAppPhonePath ：为AppData里的_strAppPhonePath
		//strAppName ：为AppData里的_AppName
		//strVersion ：为AppData里的_strVersion
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名）
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, 
			wstring strVersion, eArchiveMode eMode, wstring strFileOnPc) = 0;

		// Summary  : 单纯获取应用的数量
		// Returns  : int 返回应用数量，无或失败均返回0
		virtual int GetAppCount(APP_OWNER appOwner) = 0;

	protected:		

		// 在iPad等设备上进行PXL软件安装或者图标修复操作以后，
        // com.apple.mobile.installation.plist文件权限会变成root.600，
        // 目前暂时没有找到造成此问题的原因，在此处进行一次权限修正。
		// nTimeOut 超时等待时间, 以秒为单位
		void NotifySB(int nTimeOut = 30);

		//执行安装命令，通过守护安装时使用,仅用于IPA、PXL和DEB的安装
		bool InstallAppService(APP_TYPE eType);		

		// Summary  : 执行卸载命令
		// Returns  : bool				：成功为TRUE
		// Parameter: APP_TYPE eType	：程序类型，仅用于IPA、PXL和DEB的安装
		// Parameter: wstring  strIdentifier	：程序唯一标识
		bool UnInstallAppService(APP_TYPE eType, wstring strIdentifier);		

		//修复安装或卸载后程序图标
		void RepairAppIcon();

		// Summary  : 下载文件夹里的内容到指定目录
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strSrcOnPhone ：设备上的文件路径
		// Parameter: wstring strDesOnPc ：PC上的文件路径
		bool DownLoadFolder(wstring strSrcOnPhone, wstring strDesOnPc);		

	private:
		wstring GetInstallCmdFileOnPC(APP_TYPE eType);
		wstring GetCmdFileOnPhone(APP_TYPE eType);
		wstring GetBackFileOnPhone(APP_TYPE eType);
		wstring GetFailFileOnPhone(APP_TYPE eType);
		bool ExcuteAppService(wstring strCmdFileOnPC, wstring strCmdFileOnPhone, wstring strBackFile, wstring strFailFile);
		wstring GetUnInstallCmdFileOnPC(APP_TYPE eType, wstring strIdentifier);
		wstring CreateCmdFileOnPC(wstring strName, wstring strContent);		

	protected:
		const DeviceInfo* _pDevInfo; //设备信息
		ProgressSubject* _pProgressSubject; //进度反馈的subject
		IIosFileSystem* _pFileStream; //设备上文件操作指针
		const DeviceData* _pDevData; //设备数据
	};
}