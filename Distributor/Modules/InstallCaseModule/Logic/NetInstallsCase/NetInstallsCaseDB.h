#ifndef NetInstallsCaseDB_h__
#define NetInstallsCaseDB_h__
#include "UbdgLib.h"
#include "RevertData.h"
#include "Web.h"

INSTALLCASE_BEGIN

#define NET_CASE_START_INDEX   4

//方案管理数据库操作类
class InstallsCaseManager;
class NetInstallsCaseDB
	: public BaseHandler<RefreshNetPlanCaseDataMsg>
	, public BaseHandler<UpdateNetInstallPlanItemsStatusMsg>
{
private:
	static NetInstallsCaseDB* _pInstance;   // 单例
public:
	// 取得单例对象
	//static CConfig* GetInstance();
	NetInstallsCaseDB();
	~NetInstallsCaseDB();

	void InitData();
    void LoadNetInstallPlans(bool clearDataBase);
	void LoadData(bool loadDataBase = true, int nUserId = 0, bool bCpa = false);

	static bool UpdateNetInstallPlanItemsStatus(NetInstallsCaseData* planItem, _NET_INSTALL_PLAN_STATUS Status);
   
	static DWORD WINAPI RefreshNetPlanCaseData(LPVOID pTemp);

	static wstring FormatSQLValue(wstring val);
	//************************************
	// Method:    LoadNetPlansFromWeb
	// FullName:  NetInstallsCaseDB::LoadNetPlansFromWeb
	// Access:    public 
	// Returns:   void
	// Qualifier:从服务端获取数据
	//************************************
	void LoadNetPlansFromWeb(int nUserId = 0, bool bCpa = false);
	
	//检查网络上的数据是否改变，改变则更新数据库（只对于数据库中已经存在网络上的装机包ID）
	//param：appdata要检查的应用程序信息
	//return: 数据信息改变的包id
	map<int,int> CheckNetAppDataChange();

	//@zjf 被修改过的软件信息在NetInstallPlans和mDataBasePlanList要删除原来重新载入网络上的软件信息
	void UpdateNetInstallPlans(map<int,int>& mapChangePackId, PackageData* PackItem);

	void LoadNetCpaInstallPlans();

	virtual bool Handle(const RefreshNetPlanCaseDataMsg* pMsg);

	virtual bool Handle(const UpdateNetInstallPlanItemsStatusMsg* pMsg);
protected:
	//建表
	void CreateTableNetInstallPlan();
	void CreateTableNetInstallPlanSofts();
	void UpdateTableInstallPlan();
	void InitTable();


	NetInstallsCaseData* BuildNetInstallPlanItem(UINT planId, wstring PackName,time_t LastUseTime, wstring Category, UINT PackId, _NET_INSTALL_PLAN_STATUS planStatus,UINT isLegal);
	static InstallsAppInfo* BuildUndownloadAppItem(UINT planId, wstring Name, wstring downUrl, long size, wstring Version, wstring Extension, wstring Identifier, wstring iconUrl);
	void InsertNetPlanItemToDB();
	static long FormatFileSize(string fileSize);
	bool AddNetInstallPlanNetAppItems(NetInstallsCaseData* planItem, vector<SoftData*> softs);
	int GetMaxAppId();
	
private:
	//设置方案包的图片
	void SetPackageIcon();

	//退出账户，删除之前账户的方案包
	void DelPackageItem();
private:
	CppSQLite3Table mDtPlans;
	InstallsCaseManager* caseManager;

	//所有网络装机方案集合，包括本地数据库中存的和Web获取的
	vector<NetInstallsCaseData*> mDataBasePlanList;
	//Web获取网络装机方案，方案集合
	static vector<PackageData*> mNetPackageDataList;

	//Web获取网络装机方案，方案中文件集合
	static map<int,vector<SoftData*>> mNetPlanItems;

	//Web获取网络Cpa装机方案，方案集合
	static vector<PackageData*> _vecNetCpaPackageDataList;
	//Web获取网络Cpa装机方案，方案中文件集合
	static map<int,vector<SoftData*>> mNetCpaPlanItems;

	//Web默认账户，用于账户切换实时刷新客户端表现
	vector<int> _vecDefautUserPackage;

	CRITICAL_SECTION	_csLock;
};

INSTALLCASE_END

#endif // InstallsCaseDB_h__