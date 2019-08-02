#pragma once

struct AblApplicationItem
{
	// ������Ϣ
	wstring _packageName;		// ��װ���� ISO��ʾ�����Ψһ��ʶ
	int     _nVersionCode;      // android�ڲ��汾�ţ����������                 
	wstring _strVersion;		// �汾����                   
	wstring _AppName;			// Ӧ�ó�����             
	wstring _strAppPhonePath;	// ����·�� (�ֻ���)              

	wstring _strIconPhonePath;	// ͼ��·�� (�ֻ���)
	wstring _strIconPCPath;		// ͼ��·�� (PC��)

	int		_isLegalSoft;		//�Ƿ�����

	int64_t _dynamicDiskUsage;	// �ĵ���С
	int64_t _staticDiskUsage;	// �����С
};

struct RequestLoadAppMsg : public BaseMessage<RequestLoadAppMsg>
{
	wstring deviceID;
	wstring searchKey;
};

struct ResponseLoadAppMsg : public GroupMessage<ResponseLoadAppMsg>
{
	AblApplicationItem item;
};

// ��ȡ�豸��Ϣ����
struct RequestInstallAppMsg : public BaseMessage<RequestInstallAppMsg>
{
	wstring deviceID;

	wstring appPath;
};

struct ResponseInstallAppMsg : public BaseMessage<ResponseInstallAppMsg>
{
	wstring deviceID;
};

enum NotifyInstallAppType
{
	NotifyInstallAppType_SelectFiles,
	NotifyInstallAppType_SelectFolder,
	NotifyInstallAppType_InstallFiles,
};

struct NotifyInstallAppMsg : public BaseMessage<NotifyInstallAppMsg>
{
	NotifyInstallAppMsg()
	{
		module = NULL;
		bRes = false;
		vetFiles.clear();
		bFile = true;
	}
	void* module;

	wstring deviceID;

	NotifyInstallAppType nType;
	wstring wsText;
	bool   bFile;
	vector<wstring> vetFiles;
	bool  bRes;
};

struct NotifyRefreshAppMsg : public BaseMessage<NotifyInstallAppMsg>
{
	NotifyRefreshAppMsg()
	{
		module = NULL;
	}
	void* module;
};

//Ӧ���̷߳���������Ϣ
struct AppThreadMsg : public BaseMessage<AppThreadMsg>
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

enum Path_Type
{
	E_TYPE_ICON,
	E_TYPE_IosDownPath,
	E_TYPE_AndroidDownPath,
	E_TYPE_OptionIni,
};
//��ȡ����ָ����·��
struct GetAppointPathMsg : public BaseMessage<GetAppointPathMsg>
{
	Path_Type st_eType;
	wstring   st_wsPath;
};

//ˢ��������
struct InvalidateMainMsg : BaseMessage<InvalidateMainMsg>
{
	InvalidateMainMsg(){}
};


//�̷߳���appmodel��������
struct LoadAppDataMsg : public BaseMessage<LoadAppDataMsg>
{
};

//�̷߳���appmodelж�����
struct UnInstallAppMsg : public BaseMessage<UnInstallAppMsg>
{
	UnInstallAppMsg()
	{
		_st_pVoidList = NULL;
	}
	vector<void*>* _st_pVoidList;
};

//�̷߳���appmodel��װ���
struct InstallAppMsg : public BaseMessage<InstallAppMsg>
{
	InstallAppMsg()
	{
		_str = L"";
	}
	vector<wstring> _strList;
	wstring _str;
};


//�̷߳���appmodel�����������
struct UpGradeSinglePriacyMsg : public BaseMessage<UpGradeSinglePriacyMsg>
{
	UpGradeSinglePriacyMsg()
	{
		pVoidParam = NULL;
		bislegal = false;
	}
	void* pVoidParam;
	bool  bislegal;
};

//�̷߳���appmodel�����������
struct UpgradeAllMsg : public BaseMessage<UpgradeAllMsg>
{
	UpgradeAllMsg()
	{
		nUpgradeType = 0;
	}
	int nUpgradeType;
};


//�̷߳���appmodel�������ͼ��
struct AppLoadIconMsg : public BaseMessage<AppLoadIconMsg>
{
	AppLoadIconMsg()
	{
		pVoidParam = NULL;
	}
	void* pVoidParam;
};