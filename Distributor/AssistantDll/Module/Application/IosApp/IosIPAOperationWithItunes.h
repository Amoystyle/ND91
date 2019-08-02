#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"
#include "Device/IosUsbConnection.h"

using namespace std;

namespace ND91Assistant
{
    // ipa相关操作错误类型
    enum IPAOperationErrorType
    {
        None,                           // 没有错误
        ApplicationVerificationFailed,  // 应用程序签名失败
        IncorrectArchitecture,          // 错误的程序架构配置
        InstallProhibited,              // 设备禁止安装应用
        PackageExtractionFailed,        // 包解压失败(4.x安装器找不到安装路径的时候,会这样)
        PackageInspectionFailed,        // 包解析失败(ipa包有问题)
        APIInternalError,               // 安装API内部错误,请拔插手机后再试.(指定安装路径不存在app,6.x固件有这个问题)
        UnknowError
    };

    class ProgressSubject;

    #define ApplicationArchivesFolder L"ApplicationArchives" //备份文件存储的位置

	//IPA类型的APP的操作类，通过itunes接口实现
	class IosIPAOperationWithItunes
	{

	public:
		IosIPAOperationWithItunes(void);
		~IosIPAOperationWithItunes(void);

	public:

		ProgressSubject& GetSubject() {return *_progressSubject;}

		//获取指定类型的所有程序的信息列表
		//pDevPtr	: 为DeviceInfo里的_ios_devptr
		//pAfcHandle: 为DeviceInfo里的_ios_afcHandle
		//bJailbreaked: 是否越狱
		//eType		: 应用程序类型
		bool GetAppInfos(DeviceInfo* devInfo, APP_OWNER eType, vector<AppData*> &vtInfos);

		//安装IPA程序
		//pDevice		: 设备数据
		//strAppFullName: 应用程序PC上的文件名（含路径），非IPA文件返回FALSE
        //string& errorMsg 安装失败返回错误信息
		CommandExecuteResult InstallApp(const DeviceData* pDevice, wstring strAppFullName, string& errorMsg, ProgressSubject& subject = NullProgressSubject::Instance());

		//卸载IPA程序
		//pDevPtr		: 为DeviceInfo里的_ios_devptr
		//strIdentifier	: 为AppData里的_PackageName
		bool UnInstallApp(const DeviceInfo* pDevInfo, wstring strIdentifier);

		//备份IPA程序
		//pDevPtr		: 为DeviceInfo里的_ios_devptr
		//pAfcHandleApp	: 为DeviceInfo里的_ios_afcHandleApp
		//strIdentifier	: 为AppData里的_PackageName
		//eMode			: 备份类型
		//strFileOnPc	: 备份文件存放在PC上的位置（路径+名），为扩展名为IPA
		CommandExecuteResult ArchiveApp(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode,
            wstring strFileOnPc, ProgressSubject& subject = NullProgressSubject::Instance());

		// Summary  : 获取应用数量
		// Returns  : int 返回应用数量
		// Parameter: void * pDevPtr : 为DeviceInfo里的_ios_devptr
		// Parameter: APP_OWNER appOwner
		int GetAppCount(void* pDevPtr, APP_OWNER appOwner);

		//App2exe的安装方式
		CommandExecuteResult InstallAppEx(const DeviceData* pDevice, wstring strAppFullName, 
			string& errorMsg, ProgressSubject& subject,
			App2ExeInfo* exeInfo);

        // 获取应用的程序大小和文档大小
        // int64_t _dynamicDiskUsage;      // 文档大小
        // int64_t _staticDiskUsage;       // 程序大小
        bool GetAppDiskUsage(DeviceInfo* pDevInfo, map< wstring, vector<int64_t> >& diskUsages);

        // 获取应用ICON路径
        bool GetAppIconPhonePath(DeviceInfo* pDevInfo, AppData* pApp);

        // 获取应用购买者
        bool GetAppPurchaser(DeviceInfo* pDevInfo, AppData* pApp);

	private:

		// 安装IPA程序（采用直接上传ipa文件安装的方法
		// string& errorMsg 安装失败返回错误信息
		bool OldInstallApplication(wstring strAppFullName, string& errorMsg,unsigned __int64 AppOffsetInExe=0,unsigned __int64 AppSize=0);
        int InstallAppInZipMode(wstring strAppFullName, string& errorMsg);

        // parameter: bool isGetAppDiskUsage 是否获取安装程序的磁盘使用信息（当前只有USER类型的才会返回磁盘使用信息,固件要4.0及以上,该属性才生效）
		bool GetAppInfos(void* pDevPtr, APP_OWNER eType, map<std::string, void*> &mapInfos, bool isGetAppDiskUsage = false);
        // 设置获取安装程序的磁盘使用信息的发送信息
        void SetAppDiskUsageSendInfo(map<string, void*>* srcDict);
		void PraseAppInfos(DeviceInfo* devInfo, map<std::string, void*> mapInfos, vector<AppData*> &vtInfos);

		//将程序备份在设备上
		bool ArchiveAppOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall = false);
		//删除备份
		bool RemoveAppArchiveOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier);

		wstring GetPurchaser(AFCFileSysemBase* pFileStream, wstring strMetadataFullName);

		//安装APP进度回调
		friend void OnPackInstallCallBack(void* pSender, void* pEvent);
		//使用ITUNES默认上传APP包的进度回调
		friend void OnPackDataTransferCallBack(void* pEvent);
		//删除APP备份的进度回调
		friend void OnRemoveIPAArchiveCallback(void* pSender, void* pEvent);
		//备份APP的进度回调
		friend void OnArchiveIPACallback(void* pSender, void* pEvent);

		friend void PraseProgress(void* pCFData);

        // 根据KEY从XML文件获取VALUE
        wstring GetStringValue(map<string, void*> appInfoNormal, string key);
        vector<wstring> GetVectorStringValue(map<string, void*> appInfoNormal, string key);
        bool GetBoolValue(map<string, void*> appInfoNormal, string key);
        int64_t GetInt64Value(map<string, void*> appInfoNormal, string key);

        // 解析程序信息中的图标路径
        void PraseIconPhonePath(DeviceInfo* devInfo, AppData* pData);

        // 解析程序信息中的购买者信息
        void PrasePurchaser(DeviceInfo* devInfo, AppData* pData);

        // 先判断AFC服务是否正常,异常的话重启该服务.
        // <remarks>由于一些操作会导致AFC挂掉,因此后续需要afc的地方可能需要重启afc服务.
        // 比如安装压缩包格式不规范 ND591up0712.ipa后会导致AFC服务挂掉,因此后续的安装需要重启AFC服务</remarks>
        void RestartAFCServiceIfNeed(IosUsbConnection iosConet);

        // 安装IPA程序(内部自动选择一种方式安装，如失败会自动尝试其他方法）
        // string& errorMsg 安装失败返回错误信息
        bool InstallApplication(wstring strAppFullName, string& errorMsg);

        // 快速安装IPA程序（直接调用iTUnes提供的快速安装方法
        int ItunesFastInstallApplication(wstring strAppFullName);
        int InstallAppInUnzipModeWithiTunesInterface(wstring strAppFullName, string& errorMsg);

        // 快速安装IPA程序（助手解压文件后多线程上传-然后调用iTunes安装服务
        int FastInsallApplication(wstring strAppFullName);
        int InstallAppInUnzipMode(wstring strAppFullName, string& errorMsg);

        // 检查应用是否适合安装到当前固件设备
        // AppData* pApp 应用信息
        int PreparatoryCheckAppInstall(AppData* pApp);

        // 检查设备是否适合授权
        int CheckValideToAuthorize(int iValideToAuthorize);

        //  安装过程
        int InstallAppSubProcedure(wstring strAppInstallFileOnPhone, wstring strAppFullName, string& errorMsg);

        // 通过协议实现,安装已经上传到设备上的APP,实现和iTunes AMDeviceInstallApplication一样功能
        bool InstallAppOnDeviceWithProtocol(wstring strAppInstallFileOnPhone, void* clientOptionsDict, string& errorMsg);
        // 安装,备份.卸载,升级等执行的操作
        bool InstallationProxyPerformOperation(string operation, map<string, void*> msgDict, string& errorMsg);

        // 获取错误信息的语言包描述
        // string errorMsg 错误标识 
        // int iErrCode 错误码
        string GetAMDErrorByDescription(string errorMsg, int iErrCode);

        // 把info.ini文件塞到程序的共享文档下面,以便标识是正版免费体验IPA
        // wstring identifier 应用ID
        // wstring acountInfo 91IPA相关账户信息
        void Upload91IPAInfo2Documents(wstring identifier, wstring acountInfo);

	private:
        int                 _progressId;    // 回调管理id，参考IosIPAItunesManager
		ProgressSubject*    _progressSubject;
		APP_OWNER           _eType;

        const DeviceInfo*   _pDevInfoPtr;
        void*               _pAfcSocketApp;
        void*               _pAfcHandleApp;

        wstring             _identifier;    // 待安装的应用ID
		
	};
}
