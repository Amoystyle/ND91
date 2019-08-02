#pragma once

#include "Module/Application/AppData.h"
#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"

// 程序管理操作类
namespace ND91Assistant
{
    class ProgressObserver;
	class CLASSINDLL_CLASS_DECL App2ExeInfo
	{
	public:
		wstring AppInExeType;
		unsigned __int64 OffsetAppInExe;
		unsigned __int64 len;
		wstring backdir;
		wstring deamondir;
	};
    class CLASSINDLL_CLASS_DECL AppManager
    {
    public:
        AppManager(DeviceData* pDevice);
        ~AppManager(void);

        // 加载App列表，加载的结果保存在_datas
        CommandExecuteResult Load();
        // 安装App
        // strAppFullName 程序在PC上的路径
        // string& errorMsg 安装失败返回错误信息
        // AppData* pRetData 安装成功返回安装的应用数据，失败返回空
        // bool bLock=true 是否锁定, 
        // bool bRecover=true 是否覆盖, 
        // bool bOnSDCard=true 是否安装在SdCard, Android2.2固件以上有效
		// AppInExeType 后缀名是exe的移动应用安装程序要说明exe里面包含的到底是哪种后缀名的app数据包
        CommandExecuteResult InstallApp(wstring strAppFullName, wstring& errorMsg, AppData* &pRetData, ProgressObserver* pObs=NULL, bool bLock=false, bool bRecover=true,
			bool bOnSDCard=true, bool isMove=false,App2ExeInfo* AppInExeInfo=NULL);
        // 卸载App
        CommandExecuteResult UnInstallApp(AppData* pAppData, ProgressObserver* pObs = NULL);
        // 备份程序
        CommandExecuteResult ArchiveApp(AppData* pAppData, wstring strFileOnPC, ProgressObserver* pObs = NULL);
        // 获取应用数 只获取数目，在速度上有做优化
        int  GetAppCount(APP_OWNER appOwner);
        int  GetAppCount(bool bOnSDCard);
        // 释放已加载的数据
        void ReleaseData();

        // 取得_datas
        const vector<AppData*>* GetDatas();
		
		 // 取得_datas
		void GetDatas(vector<AppData*>& vecData);
        void ReleaseGetDatas();
		AppData* FindAppInfoByID(wstring AppdataID);

        // 图标修复（仅ios设备）
        CommandExecuteResult RepairIcon();
        // Summary  : PXL安装包转IPA安装包
        // Returns  : std::wstring 返回转换后的IPA包名（含路径）
        // Parameter: wstring strPxlFullName ：PXL安装包名（含路径）
        wstring PxlToIpa(wstring strPxlFullName);
        //结束进程
        //IOS未有查看进程的接口，因此此命令只用于结束守护进程用(卸载守护用)
        CommandExecuteResult KillDeamonProcess(wstring strProcessName);
        // 安装守护程序 
        // strDaemonFullName:守护zip压缩包文件全名(含路径)
        // daemonVersionOnService:守护在服务器上的版本号
        // 安装完后需要重启
        CommandExecuteResult InstallDaemon(wstring strDaemonFullName, wstring daemonVersionOnService);
        //卸载守护
        //卸载后要重启才能完全卸载
        CommandExecuteResult UnInstallDaemon();

        // Summary  : 移动程序(android)
        // Returns  : ND91Assistant::CommandExecuteResult
        // Parameter: AppData * pAppData 要移动的应用
        // Parameter: string & errorMsg 失败返回错误信息
        // Parameter: AppData * pRetAppData 成功返回新安装的应用数据
        // Parameter: ProgressObserver * pObs
        CommandExecuteResult MoveToApp(AppData* pAppData, wstring& errorMsg, AppData* &pRetAppData, ProgressObserver* pObs = NULL);

		//根据安装包获取应用程序信息接口，最后一个参数表示是否要获取图标
		static AppData* GetAppInfoByFile(wstring filepath, wstring tempdir,  bool icon=false);

		//判断是否安装了某个软件 
		bool IsInstallApp(wstring packageName);

        // 闪退修复
        // 返回 0：支持正版体验
        // -1 设备固件版本低于4.0
        // -2 连接方式不是USB
        // -3 iTunes版本过低
        // -4 未安装iTunes
        // -5 iTunes版本为简易版
        int FixFlashExit();

        // 当前设备是否适合授权
        // 返回 0：适合授权
        // -1 设备固件版本低于4.0
        // -2 连接方式不是USB
        // -3 iTunes版本过低
        // -4 未安装iTunes
        // -5 iTunes版本为简易版
        int IsValideToAuthorize();

        // 获取应用的程序大小和文档大小
        bool GetAppDiskUsage();

        // Summary  : 安装多个相互依赖的DEB应用
        // Returns  : bool
        // Parameter: vector<wstring> remotePaths 多个deb文件在设备上的路径
        bool InstallSpecialDebs(vector<wstring> remotePaths);

        // Summary  : 安装Appsync
        bool InstallAppsyncService();

	private:

		//获取安装包中程序图标
		static wstring upzipIcon(wstring zipFilePath, wstring tempIcondir, wstring iconName,wstring appname,wstring appVersion);

        // Summary  : 再次判断应用安装结果
        // Returns  : 安装结果
        // Parameter: CommandExecuteResult installResult 调用安装接口返回的安装结果
        // Parameter: AppData* pPackData 应用包数据
        // Parameter: AppData* pRetData 安装成功返回安装的应用数据，失败返回空
        CommandExecuteResult JudgeInstallAppResult(CommandExecuteResult installResult, AppData* pPackData, AppData* &pRetData);

        // 卸载完成后从列表中移除
		void RemoveAppFromList(AppData* pAppData);
        void RemoveAppFromList(wstring appID);

		//如果文件名过长取正常的文件名
		static bool GetRightFileName(wstring &oldname,wstring &filename);

    private:
        DeviceData*	_pDevice;
        vector<AppData*> _datas;
        vector<AppData*> _newDatas;     // 新建一份供调用者使用
        static AppManager* _pInstance; // 单例实例
        CRITICAL_SECTION _cs;//访问临界值
        bool _bLoading;
    };
}