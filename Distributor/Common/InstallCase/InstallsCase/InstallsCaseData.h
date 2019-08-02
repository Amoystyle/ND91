
#pragma once

#include "InstallsAppInfo.h"


enum _INSTALLS_CASE_DATA_TYPE
{
	_INSTALLS_CASE_DATA_ANDROID,
	_INSTALLS_CASE_DATA_IOS	
};
/// <summary>
/// 方案类型
/// </summary>
enum _INSTALL_PLAN_TYPE
{
	/// <summary>
	/// 本地
	/// </summary>
	_INSTALL_PLAN_TYPE_SYS =0,
	/// <summary>
	/// 网络
	/// </summary>
	_INSTALL_PLAN_TYPE_NET = 1,
	/// <summary>
	/// 升级
	/// </summary>
	_INSTALL_PLAN_TYPE_UPGRATE = 2,

	/// <summary>
	/// 用户
	/// </summary>
	_INSTALL_PLAN_TYPE_USER = 3,
	/// <summary>
	/// 正版升级
	/// </summary>
	_INSTALL_PLAN_TYPE_LEGALUPGRADE = 4,
};

enum _APP_UPGRADE_TYPE
{
	///升级至破解
	_APP_UPGRADE_TYPE_TOPIRACY = 0,
	//升级至正版
	_APP_UPGRADE_TYPE_TOLEGAL = 1,
	//正版升级至正版，破解升级至破解
	_APP_UPGRADE_TYPE_TOSAMELEGAL = 2,
};

class AppCountByFolder
{
public:
	AppCountByFolder()
	{
		AppPathList = new vector<wstring>();
		bHasChanged = false;
	}
	~AppCountByFolder()
	{
		if (AppPathList)
		{
			delete AppPathList;
			AppPathList = NULL;
		}
	}
public:
	vector<wstring>* AppPathList;
	UINT m_id;
	wstring FolderName;
	UINT Filecount;
	unsigned long TotalFileSize;
	_INSTALLS_CASE_DATA_TYPE Filetype;
	bool bHasChanged;
};

//方案
class InstallsCaseData
{
public:
	InstallsCaseData(UINT id);
	~InstallsCaseData();
	//根据ID获得应用程序信息
	//************************************
	// Method:    GetAppByID
	// FullName:  InstallsCaseData::GetAppByID
	// Access:    public 
	// Returns:   InstallsAppInfo *
	// Qualifier:
	// Parameter: UINT id 数据库里面的ID
	//************************************
	InstallsAppInfo * GetAppByID(UINT id);
	InstallsAppInfo * GetAppByAppInfo(InstallsAppInfo * oldappinfo);
	InstallsAppInfo * GetAppBySavePath(wstring path);

	//************************************
	// Method:    GetAppByName
	// FullName:  InstallsCaseData::GetAppByName
	// Access:    public 
	// Returns:   InstallsAppInfo *
	// Qualifier:
	// Parameter: wstring name 包名
	//************************************
	InstallsAppInfo * GetAppByName(wstring name);
	//取应用程序数量
	virtual UINT GetAppNum();
	//将应用程序添加到列表
	bool AddAppToList(InstallsAppInfo * appinfo);
	//列表中移除应用
	bool RemoveAppByid(UINT id);
	bool RemoveAppByPath(wstring path);

	bool setSelect(UINT id);
	bool setUnSelect(UINT id);
	//获得总文件大小
	virtual unsigned long GetSumFileSize();

	void AddinstallCount(){m_InstallCount++;};

	std::wstring CaseName() const { return m_strCaseName; }
	void CaseName(std::wstring val) { m_strCaseName = val; }

	_INSTALLS_CASE_DATA_TYPE CaseType() const { return m_CaseType; }
	void CaseType(_INSTALLS_CASE_DATA_TYPE val) { m_CaseType = val; }

	InstallsAppList * ListApp() const { return m_ListApp; }
	void ListApp(InstallsAppList * val) { m_ListApp = val; }

	UINT Id() const { return m_id; }
	void Id(UINT val) { m_id = val; }

	UINT InstallCount() const { return m_InstallCount; }
	void InstallCount(UINT val) { m_InstallCount = val; }

	UINT IsLegalPlan() const { return m_IsLegal; }
	void IsLegalPlan(UINT val) { m_IsLegal = val; }

	std::wstring GetFolderPath() const { return m_FolderPath; }
	void SetFolderPath(std::wstring val) { m_FolderPath = val; }

	_INSTALL_PLAN_TYPE PlanType() const { return mPlanType; }
	void PlanType(_INSTALL_PLAN_TYPE val) { mPlanType = val; }

	//最后使用时间，用来排序取得最近使用方案
	time_t lastUseTime;

	UINT PackId;
private:
	UINT m_id;
	
	wstring m_strCaseName; //方案名称

	_INSTALLS_CASE_DATA_TYPE m_CaseType; //方案类型 ：ios，android

	InstallsAppList *m_ListApp; //方案包含的应用列表

	UINT m_InstallCount;	//安装次数
	
	UINT m_IsLegal;		//是否正版装机方案，1为是，其他为否

	wstring m_FolderPath;

	_INSTALL_PLAN_TYPE mPlanType;

};
