#pragma once

INSTALLCASE_BEGIN
//方案管理数据库操作类
class InstallsCaseManager;
class InstallsCaseDB
{
public:
	InstallsCaseDB();
	~InstallsCaseDB();
	//加载数据
	void LoadData(InstallsCaseManager* caseManager);
	//插入新建方案
	void InsertNewCase(InstallsCaseData* casedata);
	//方案表增加应用
	void InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//方案表移除应用
	void RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	void RemoveAllCaseFile(InstallsCaseData* casedata);
	//移除方案
	void RemoveCase(InstallsCaseData* casedata);
	//方案改名
	void RenameCase(InstallsCaseData* casedata,wstring newName);
	//库增加文件
	void InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//库增移除文件
	void RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);
	//配置信息表
	void CreateTableProfile();
protected:
	//建表
	void CreateTableSoftLibFolder();
	void CreateTableSoftLibFile();
	void CreateTableInstallPlan();
	void CreateTableInstallPlanFile();
	void InitTable();
	//wstring m_dbName;

	void LoadLibApps(InstallsCaseData* casedata);
	void LoadCaseApps(InstallsCaseData* casedata);
};
INSTALLCASE_END
