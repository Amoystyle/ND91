#pragma once

//方案管理数据库操作类

INSTALLCASE_BEGIN

class InstallsCaseDBEx
{
public:
	InstallsCaseDBEx();
	~InstallsCaseDBEx();

	//加载数据
	void LoadData(InstallCaseManagerEx* caseManager);

	//插入新建方案
	void InsertNewCase(InstallsCaseData* casedata);

	//方案表增加应用
	void InsertCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//方案表移除应用
	void RemoveCaseFile(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//移除方案中所以软件
	void RemoveAllCaseFile(InstallsCaseData* casedata);

	//移除方案
	void RemoveCase(InstallsCaseData* casedata);

	//方案改名
	void RenameCase(InstallsCaseData* casedata,wstring newName);

	//更新方案
	void UpdateCase(InstallsCaseData* casedata);

	//库增加文件
	void InsertAppToLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//库增移除文件
	void RemoveAppFromLib(InstallsCaseData* casedata,InstallsAppInfo* appdata);

	//配置信息表
	void CreateTableProfile();

	//获取最大方案id
	int  GetCaseMaxId();

	int  GetAppMaxId();

	int  GetInstallPlanFileMaxId();

	void UpdateItemIDByPath(wstring tableName, wstring path, int itemID);
protected:
	//建表
	void CreateTableSoftLibFolder();
	void CreateTableSoftLibFile();
	void CreateTableInstallPlan();
	void CreateTableInstallPlanFile();
	void InitTable();
	//更新表
	void UpdateTableInstallPlanFile();
	void UpdateTableSoftLibFile();
	void UpdateTableInstallPlan();
	void UpdateTableInstallPlanFile2();
	void UpdateTableSoftLibFile2();
	void UpdateTableInstallPlanFile3();
	void UpdateTableSoftLibFile3();
	//wstring m_dbName;

	void LoadLibApps(InstallsCaseData* casedata);
	void LoadCaseApps(InstallsCaseData* casedata);

private:
	int idFix;
public:
	bool needReAnalyzeApps;
};

INSTALLCASE_END