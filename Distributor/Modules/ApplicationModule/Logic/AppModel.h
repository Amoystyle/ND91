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

    // ��������
    virtual void LoadAndDisplayData(wstring id);

    // ˢ������
    virtual void RefreshData();

    // ����App����
    bool LoadAppData();

	//����App��Icon
	void LoadAppIcon( vector<AppData*>* pTemp );

	void LoadAppIconInThread(vector<AppData*>* vecTempData);

    // ��װApp
    bool InstallApp(bool isFile, wstring wsTitle=L"");
    bool InstallApp(vector<wstring> files);
    bool InstallAppThread(const vector<wstring>& vecfiles, const wstring strSn = L"");

    // �ƶ�����
    bool MoveToApp(AppData* pApp);

    // ж�س���
    bool UnInstallApp(vector<AppData*>* datas); 
    bool UnInstallApp(AppData* pApp);
    bool UnInstallAppAction(AppData* pApp); // �������

    // ��ȡ��ǰ�豸����
    DEVICE_OS_TYPE GetCurDeviceType();

    // ��ȡ��ǰ�豸SD��״̬
    SDCardState GetSDCardState();

    // ��ȡ��ǰ�豸�Ƿ���ROOT
    bool IsRootDevice();

	// ��ȡ��ǰ�豸�Ƿ���Խ��
	bool IsJailBreak();

	//��ȡ�ɸ��µĵ������List
	void CheckPiracyUpgradeAppList(const vector<AppData*>* datas);

	//��ȡ�ɸ��µ��������List
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

	//@zjf summary:���debug�£�ios�豸����ȥ������������.�е�������������ClearTempData�ͷ��ڴ�
	void BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData);
	void ClearTempData(vector<AppData*>& vecTempData);

	bool Handle(const WebSoftInstallMsg* pMsg);

	bool UnInstallApp(const vector<void*>* _st_pVoidList=NULL);

	// ���õ�ǰ�豸��ID
	void SetCurrentDeviceID(wstring id);;

	// ȡ�õ�ǰ�豸��ID
	wstring GetCurrentDeviceID();

	// ȡ��ǰ�豸ָ��
	DeviceData* FindCurDevice(); 

	bool Handle(const NotifyTaskItemMsg* pMsg);
private:
    // ���AppData�Ƿ���Ч
    bool CheckAppData(AppData* pApp);

	// ���AppData�Ƿ��ڰ�װ��
    bool CheckAppIsInUnstall(AppData* pApp);

    // ��ȡ��װ��·��
    void GetAppPackagePaths(vector<wstring>& files, bool isFile, wstring wsTitle);

	//�����Ƿ�������������������������棬����һ������������һ��������������е���ͬ���ҲӦ���ã�
	void SetUpgrading(AppDataUpgrade* appdata, bool isupgrading);

    wstring                     _strIconDir;        // Ӧ��ICON�����Ŀ¼
    ApplicationInfoInterface*   _pUI;               // ������Ҫ�Ľ������
	

    bool                        _haveLoad;  // �Ƿ��Ѿ�����
    CRITICAL_SECTION            _lock;      // �б��Ƿ�����жϵ���
	CRITICAL_SECTION            _Uninstall_lock;      // ж����
    bool                        _bLoadingData;  //�Ƿ����ڼ�������

	// ��������ж�ص�Ӧ������
	vector<AppData*> _UnInstallAppDatas;
	vector<AppDataUpgrade*> _PiracyUpgradeApps;	//���������������
	vector<AppDataUpgrade*> _LegalUpgradeApps;	//���������������

	map<int, AppDataUpgrade*> _InstallTasks;
	map<int, AppDataUpgrade*> _UpgradeTasks;

	vector<wstring>			  _files;//����򿪰�װ��Ŀ¼����

	bool _UnInstallAppIng;		//�Ƿ�����ж��Ӧ��

	wstring _curDeviceID;

	bool  _bExit;
};
App_CC_END