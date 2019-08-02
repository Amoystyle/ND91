#pragma once

//通过路径获得哈希值
struct GetHashByPathMsg : BaseMessage<GetHashByPathMsg>
{
	wstring _st_Path;
	string  _st_Hash;
};


struct GetCaseDataMsg : BaseMessage<GetCaseDataMsg>
{
	GetCaseDataMsg()
	{
		pCase = NULL;
		nCaseType = 0;
		_isLegalSoft = 0;
	}
	int  nCaseType;
	wstring _packageName;
	wstring _strVersion;
	int _isLegalSoft;
	vector<wstring> installFilePaths;
	void* pCase;
};

struct UpgrateCancel
{
	wstring wstrAppId;
	int     nUpgradetype;
	wstring wstrOldappfullpath;
};
//UI线程发过来的关于装机方案的消息
struct InstallCaseMsg : BaseMessage<InstallCaseMsg>
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};


//网络装机方案完成
struct LoadFinshedMsg : BaseMessage<LoadFinshedMsg>
{
};

//struct AppUpdategradeCancelMsg : BaseMessage<GetCaseDataMsg>
//{
//	//nUserData 是CTaskItem*
//};


struct GetInstallPathByCaseIdMsg : BaseMessage<GetInstallPathByCaseIdMsg>
{
	GetInstallPathByCaseIdMsg()
	{
		caseId = 0;
		nListSize = 0;
	}
	int caseId;
	vector<wstring> InstallpathList;
	int nListSize;
};

//通过方案ID移除方案
struct RemoveCaseByCaseIdMsg : BaseMessage<RemoveCaseByCaseIdMsg>
{
	RemoveCaseByCaseIdMsg()
	{
		idList= NULL;
	}
	vector<int>* idList ;
};

//通过方案ID安装软件
struct InstallAppByCaseIdMsg : BaseMessage<InstallAppByCaseIdMsg>
{
	InstallAppByCaseIdMsg()
	{
		pathList= NULL;
		caseId = 0;
	}
	wstring sn;
	int caseId;
	vector<wstring>* pathList;
};

//刷新网络方案
struct RefreshNetPlanCaseDataMsg : BaseMessage<RefreshNetPlanCaseDataMsg>
{
	RefreshNetPlanCaseDataMsg()
	{
		pPlanItem = NULL;
	}

	void* pPlanItem;
};

//更新网络方案状态
struct UpdateNetInstallPlanItemsStatusMsg : BaseMessage<UpdateNetInstallPlanItemsStatusMsg>
{
	UpdateNetInstallPlanItemsStatusMsg()
	{
		planItem = NULL;
		Status = 0;
	}
	void* planItem;
	int Status;
};

//获得软件信息
struct AnalyzeAppMsg : BaseMessage<AnalyzeAppMsg>
{
	AnalyzeAppMsg()
	{
		filePath = L"";
		pAppInfo = NULL;
	}

	wstring filePath;
	void*   pAppInfo;
};

//增加方案
struct AddCaseMsg : BaseMessage<AddCaseMsg>
{
	AddCaseMsg()
	{
		caseType = -1;
		pPathList = NULL;
		fromLib = false;
	}

	int caseType;
	void* pPathList;
	bool fromLib;
};

//获得装机方案列表
struct GetNetInstallsCaseDataListMsg : BaseMessage<GetNetInstallsCaseDataListMsg>
{
	GetNetInstallsCaseDataListMsg()
	{
		pDataList = NULL;
	}
	void* pDataList;
};

//根据使用时间排序方案
struct GetSortByUseTimeCaseListMsg : BaseMessage<GetSortByUseTimeCaseListMsg>
{
	GetSortByUseTimeCaseListMsg()
	{
		pDataList = NULL;
	}
	void* pDataList;
};

//根据id获得装机方案
struct GetCaseByIdMsg : BaseMessage<GetCaseByIdMsg>
{
	GetCaseByIdMsg()
	{
		CaseDataid = 0;
		pDataCase = NULL;
	}
	int   CaseDataid;
	void* pDataCase;
};



#define SUPPORTDEVICE_ALL				L"iphone/ipod/ipad"
#define SUPPORTDEVICE_IPHONE			L"iphone/ipod"
#define SUPPORTDEVICE_IPAD				L"ipad"
//加载软件列表
struct LoadAppListMsg : BaseMessage<LoadAppListMsg>
{

	LoadAppListMsg()
	{
		type = SUPPORTDEVICE_ALL;
	}
	int caseId;
	wstring type;
};

//升级 应用
struct UpgrateAppMsg : BaseMessage<UpgrateAppMsg>
{

	UpgrateAppMsg()
	{
		caseId = 0;
		pPathList = NULL;
	}
	int caseId;
	vector<wstring>* pPathList;
};

//升级正版应用
struct UpgradeAppToLegalMsg : BaseMessage<UpgradeAppToLegalMsg>
{

	UpgradeAppToLegalMsg()
	{
		caseId = 0;
		pPathList = NULL;
	}
	int caseId;
	vector<wstring>* pPathList;
};

struct AddDownLoadFolderMsg : BaseMessage<AddDownLoadFolderMsg>
{
	wstring iosPath;
	wstring androidPath;
	wstring fromModular;
};
struct InstallCaseInfoMsg : BaseMessage<InstallCaseInfoMsg>
{
	//nparam 传caseId
	bool bIsExist;//返回是否存在
	int nCaseType;// 返回_INSTALLS_CASE_DATA_TYPE 枚举
	InstallCaseInfoMsg()
	{
		bIsExist = false;
		nCaseType = -1;
	}
};

struct AddAppToCaseByPathMsg : BaseMessage<AddAppToCaseByPathMsg>
{
	int nCaseId;
	vector<wstring>* pPathes;
	AddAppToCaseByPathMsg()
	{
		nCaseId = 0;
		pPathes = NULL;
	}
};

//SendAndGet
struct AddSingleAppFileToLibMsg : BaseMessage<AddSingleAppFileToLibMsg>
{
	wstring filePath;
	bool isReshUI;
	int fid;
	wstring oldFilePath;
	void* result;//返回值
	AddSingleAppFileToLibMsg()
	{
		result = NULL;
	}
};

//Send
struct AppUpdategradeCancelMsg : BaseMessage<AppUpdategradeCancelMsg>
{
	void* item;//为CTaskItem*
	AppUpdategradeCancelMsg()
	{
		item = NULL;
	}
};

//刷新网络方案完成
struct RefreshNetPlanCaseFinishMsg : BaseMessage<RefreshNetPlanCaseFinishMsg>
{

};

//接受界面最小化消息
struct UIMiniSizeMsg : BaseMessage<UIMiniSizeMsg>
{

};