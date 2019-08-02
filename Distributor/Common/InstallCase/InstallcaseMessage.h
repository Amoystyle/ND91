#pragma once

//ͨ��·����ù�ϣֵ
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
//UI�̷߳������Ĺ���װ����������Ϣ
struct InstallCaseMsg : BaseMessage<InstallCaseMsg>
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};


//����װ���������
struct LoadFinshedMsg : BaseMessage<LoadFinshedMsg>
{
};

//struct AppUpdategradeCancelMsg : BaseMessage<GetCaseDataMsg>
//{
//	//nUserData ��CTaskItem*
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

//ͨ������ID�Ƴ�����
struct RemoveCaseByCaseIdMsg : BaseMessage<RemoveCaseByCaseIdMsg>
{
	RemoveCaseByCaseIdMsg()
	{
		idList= NULL;
	}
	vector<int>* idList ;
};

//ͨ������ID��װ���
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

//ˢ�����緽��
struct RefreshNetPlanCaseDataMsg : BaseMessage<RefreshNetPlanCaseDataMsg>
{
	RefreshNetPlanCaseDataMsg()
	{
		pPlanItem = NULL;
	}

	void* pPlanItem;
};

//�������緽��״̬
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

//��������Ϣ
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

//���ӷ���
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

//���װ�������б�
struct GetNetInstallsCaseDataListMsg : BaseMessage<GetNetInstallsCaseDataListMsg>
{
	GetNetInstallsCaseDataListMsg()
	{
		pDataList = NULL;
	}
	void* pDataList;
};

//����ʹ��ʱ�����򷽰�
struct GetSortByUseTimeCaseListMsg : BaseMessage<GetSortByUseTimeCaseListMsg>
{
	GetSortByUseTimeCaseListMsg()
	{
		pDataList = NULL;
	}
	void* pDataList;
};

//����id���װ������
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
//��������б�
struct LoadAppListMsg : BaseMessage<LoadAppListMsg>
{

	LoadAppListMsg()
	{
		type = SUPPORTDEVICE_ALL;
	}
	int caseId;
	wstring type;
};

//���� Ӧ��
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

//��������Ӧ��
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
	//nparam ��caseId
	bool bIsExist;//�����Ƿ����
	int nCaseType;// ����_INSTALLS_CASE_DATA_TYPE ö��
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
	void* result;//����ֵ
	AddSingleAppFileToLibMsg()
	{
		result = NULL;
	}
};

//Send
struct AppUpdategradeCancelMsg : BaseMessage<AppUpdategradeCancelMsg>
{
	void* item;//ΪCTaskItem*
	AppUpdategradeCancelMsg()
	{
		item = NULL;
	}
};

//ˢ�����緽�����
struct RefreshNetPlanCaseFinishMsg : BaseMessage<RefreshNetPlanCaseFinishMsg>
{

};

//���ܽ�����С����Ϣ
struct UIMiniSizeMsg : BaseMessage<UIMiniSizeMsg>
{

};