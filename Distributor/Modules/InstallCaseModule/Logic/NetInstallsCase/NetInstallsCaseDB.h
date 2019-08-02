#ifndef NetInstallsCaseDB_h__
#define NetInstallsCaseDB_h__
#include "UbdgLib.h"
#include "RevertData.h"
#include "Web.h"

INSTALLCASE_BEGIN

#define NET_CASE_START_INDEX   4

//�����������ݿ������
class InstallsCaseManager;
class NetInstallsCaseDB
	: public BaseHandler<RefreshNetPlanCaseDataMsg>
	, public BaseHandler<UpdateNetInstallPlanItemsStatusMsg>
{
private:
	static NetInstallsCaseDB* _pInstance;   // ����
public:
	// ȡ�õ�������
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
	// Qualifier:�ӷ���˻�ȡ����
	//************************************
	void LoadNetPlansFromWeb(int nUserId = 0, bool bCpa = false);
	
	//��������ϵ������Ƿ�ı䣬�ı���������ݿ⣨ֻ�������ݿ����Ѿ����������ϵ�װ����ID��
	//param��appdataҪ����Ӧ�ó�����Ϣ
	//return: ������Ϣ�ı�İ�id
	map<int,int> CheckNetAppDataChange();

	//@zjf ���޸Ĺ��������Ϣ��NetInstallPlans��mDataBasePlanListҪɾ��ԭ���������������ϵ������Ϣ
	void UpdateNetInstallPlans(map<int,int>& mapChangePackId, PackageData* PackItem);

	void LoadNetCpaInstallPlans();

	virtual bool Handle(const RefreshNetPlanCaseDataMsg* pMsg);

	virtual bool Handle(const UpdateNetInstallPlanItemsStatusMsg* pMsg);
protected:
	//����
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
	//���÷�������ͼƬ
	void SetPackageIcon();

	//�˳��˻���ɾ��֮ǰ�˻��ķ�����
	void DelPackageItem();
private:
	CppSQLite3Table mDtPlans;
	InstallsCaseManager* caseManager;

	//��������װ���������ϣ������������ݿ��д�ĺ�Web��ȡ��
	vector<NetInstallsCaseData*> mDataBasePlanList;
	//Web��ȡ����װ����������������
	static vector<PackageData*> mNetPackageDataList;

	//Web��ȡ����װ���������������ļ�����
	static map<int,vector<SoftData*>> mNetPlanItems;

	//Web��ȡ����Cpaװ����������������
	static vector<PackageData*> _vecNetCpaPackageDataList;
	//Web��ȡ����Cpaװ���������������ļ�����
	static map<int,vector<SoftData*>> mNetCpaPlanItems;

	//WebĬ���˻��������˻��л�ʵʱˢ�¿ͻ��˱���
	vector<int> _vecDefautUserPackage;

	CRITICAL_SECTION	_csLock;
};

INSTALLCASE_END

#endif // InstallsCaseDB_h__