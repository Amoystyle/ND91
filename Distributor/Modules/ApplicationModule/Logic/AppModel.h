#pragma once

#include "ApplicationInfoInterface.h"
App_CC_BEGIN
class AppDataUpgrade;


class AppModel 
	: public BaseHandler<WebSoftInstallMsg>
	, BaseHandler<NotifyTaskItemMsg>
{
public:
	AppModel(ApplicationInfoInterface* pInterface);
    ~AppModel();

    // 加载数据
    virtual void LoadAndDisplayData(wstring id);

    // 刷新数据
    virtual void RefreshData();

    // 载入App数据
    bool LoadAppData();

	//加载App的Icon
	void LoadAppIcon( vector<AppData*>* pTemp );

	void LoadAppIconInThread(vector<AppData*>* vecTempData);

    // 安装App
    bool InstallApp(bool isFile, wstring wsTitle=L"");
    bool InstallApp(vector<wstring> files);
    bool InstallAppThread(const vector<wstring>& vecfiles, const wstring strSn = L"");

    // 移动程序
    bool MoveToApp(AppData* pApp);

    // 卸载程序
    bool UnInstallApp(vector<AppData*>* datas); 
    bool UnInstallApp(AppData* pApp);
    bool UnInstallAppAction(AppData* pApp); // 具体操作

    // 获取当前设备类型
    DEVICE_OS_TYPE GetCurDeviceType();

    // 获取当前设备SD卡状态
    SDCardState GetSDCardState();

    // 获取但前设备是否已ROOT
    bool IsRootDevice();

	// 获取当前设备是否已越狱
	bool IsJailBreak();

	//获取可更新的盗版软件List
	void CheckPiracyUpgradeAppList(const vector<AppData*>* datas);

	//获取可更新的正版软件List
	void CheckLegalUpgradeAppList(const vector<AppData*>* datas);

    wstring GetIconDir() { return _strIconDir; }
    void SetLoadState(bool haveLoad) { _haveLoad = haveLoad; }
	bool GetLoadState() { return _haveLoad; }
	bool GetLoadingState() { return _bLoadingData; }
	bool GetUninstallState() { return _UnInstallAppIng; }

	vector<AppDataUpgrade*>* GetAppDataUpgrade(){ return &_PiracyUpgradeApps; }
	vector<AppDataUpgrade*>* GetLegalAppDataUpgrade(){ return &_LegalUpgradeApps; }

	void UpgradeSinglePiracyAppData(AppDataUpgrade* appdata);
	void UpgradeSingleLegalAppData(AppDataUpgrade* appdata);

	void UpgradePiracyAppDatas(vector<AppDataUpgrade*>* appdataVec);
	void UpgradeLegalAppDatas(vector<AppDataUpgrade*>* appdataVec);

	void UpgradeAllAppData(_APP_UPGRADE_TYPE upgradeType);
	void UpgradeAllAppDataInternal(_APP_UPGRADE_TYPE upgradeType);

	void UpgradeSingleAppDataInternal(AppDataUpgrade* appdata, bool islegal);

	int GetPiracyUpgradeAppsCount(){ return _PiracyUpgradeApps.size(); }
	int GetLegalUpgradeAppsCount(){ return _LegalUpgradeApps.size(); }
	int GetSameUpgradeAppsCount();
	void GetAllInstallAndUpgradeApps(vector<AppDataUpgrade*>* upgradeApps, bool islegal, bool mustsamelegal, vector<wstring>* installFilePaths);

	void TaskRemove(int taskid, wstring deviceId);
	void TaskFinished( CTaskItem* item, wstring deviceId );

	void GetAppFilePath(vector<wstring>& file);

	//@zjf summary:解决debug下，ios设备插上去经常崩溃问题.有调用者主动调用ClearTempData释放内存
	void BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData);
	void ClearTempData(vector<AppData*>& vecTempData);

	bool Handle(const WebSoftInstallMsg* pMsg);

	bool UnInstallApp(const vector<void*>* _st_pVoidList=NULL);

	// 设置当前设备的ID
	void SetCurrentDeviceID(wstring id);;

	// 取得当前设备的ID
	wstring GetCurrentDeviceID();

	// 取当前设备指针
	DeviceData* FindCurDevice(); 

	bool Handle(const NotifyTaskItemMsg* pMsg);
private:
    // 检查AppData是否有效
    bool CheckAppData(AppData* pApp);

	// 检查AppData是否在安装中
    bool CheckAppIsInUnstall(AppData* pApp);

    // 获取安装包路径
    void GetAppPackagePaths(vector<wstring>& files, bool isFile, wstring wsTitle);

	//设置是否正在升级（由于区分正版盗版，故有一个升级，另外一个升级软件集合中的相同软件也应设置）
	void SetUpgrading(AppDataUpgrade* appdata, bool isupgrading);

    wstring                     _strIconDir;        // 应用ICON保存的目录
    ApplicationInfoInterface*   _pUI;               // 所有需要的界面操作
	

    bool                        _haveLoad;  // 是否已经加载
    CRITICAL_SECTION            _lock;      // 列表是否加载判断的锁
	CRITICAL_SECTION            _Uninstall_lock;      // 卸载锁
    bool                        _bLoadingData;  //是否正在加载数据

	// 保存正在卸载的应用数据
	vector<AppData*> _UnInstallAppDatas;
	vector<AppDataUpgrade*> _PiracyUpgradeApps;	//盗版升级软件集合
	vector<AppDataUpgrade*> _LegalUpgradeApps;	//正版升级软件集合

	map<int, AppDataUpgrade*> _InstallTasks;
	map<int, AppDataUpgrade*> _UpgradeTasks;

	vector<wstring>			  _files;//保存打开安装的目录程序

	bool _UnInstallAppIng;		//是否正在卸载应用

	wstring _curDeviceID;

	bool  _bExit;
};
App_CC_END