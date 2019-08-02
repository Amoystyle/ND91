
#pragma once

#include "InstallsAppInfo.h"


enum _INSTALLS_CASE_DATA_TYPE
{
	_INSTALLS_CASE_DATA_ANDROID,
	_INSTALLS_CASE_DATA_IOS	
};
/// <summary>
/// ��������
/// </summary>
enum _INSTALL_PLAN_TYPE
{
	/// <summary>
	/// ����
	/// </summary>
	_INSTALL_PLAN_TYPE_SYS =0,
	/// <summary>
	/// ����
	/// </summary>
	_INSTALL_PLAN_TYPE_NET = 1,
	/// <summary>
	/// ����
	/// </summary>
	_INSTALL_PLAN_TYPE_UPGRATE = 2,

	/// <summary>
	/// �û�
	/// </summary>
	_INSTALL_PLAN_TYPE_USER = 3,
	/// <summary>
	/// ��������
	/// </summary>
	_INSTALL_PLAN_TYPE_LEGALUPGRADE = 4,
};

enum _APP_UPGRADE_TYPE
{
	///�������ƽ�
	_APP_UPGRADE_TYPE_TOPIRACY = 0,
	//����������
	_APP_UPGRADE_TYPE_TOLEGAL = 1,
	//�������������棬�ƽ��������ƽ�
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

//����
class InstallsCaseData
{
public:
	InstallsCaseData(UINT id);
	~InstallsCaseData();
	//����ID���Ӧ�ó�����Ϣ
	//************************************
	// Method:    GetAppByID
	// FullName:  InstallsCaseData::GetAppByID
	// Access:    public 
	// Returns:   InstallsAppInfo *
	// Qualifier:
	// Parameter: UINT id ���ݿ������ID
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
	// Parameter: wstring name ����
	//************************************
	InstallsAppInfo * GetAppByName(wstring name);
	//ȡӦ�ó�������
	virtual UINT GetAppNum();
	//��Ӧ�ó�����ӵ��б�
	bool AddAppToList(InstallsAppInfo * appinfo);
	//�б����Ƴ�Ӧ��
	bool RemoveAppByid(UINT id);
	bool RemoveAppByPath(wstring path);

	bool setSelect(UINT id);
	bool setUnSelect(UINT id);
	//������ļ���С
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

	//���ʹ��ʱ�䣬��������ȡ�����ʹ�÷���
	time_t lastUseTime;

	UINT PackId;
private:
	UINT m_id;
	
	wstring m_strCaseName; //��������

	_INSTALLS_CASE_DATA_TYPE m_CaseType; //�������� ��ios��android

	InstallsAppList *m_ListApp; //����������Ӧ���б�

	UINT m_InstallCount;	//��װ����
	
	UINT m_IsLegal;		//�Ƿ�����װ��������1Ϊ�ǣ�����Ϊ��

	wstring m_FolderPath;

	_INSTALL_PLAN_TYPE mPlanType;

};
