#pragma once

INSTALLCASE_BEGIN

class InstallCaseInterface
{
public:
	//************************************
	// Method:    GetEditCaseIdList
	// FullName:  InstallCaseInterface::GetEditCaseIdList
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring & caseName �༭��ķ�������
	// Parameter: vector<int> * appList	�༭������Id�б�
	// Explaint:	�༭�����������༭��������
	//************************************
	virtual void GetEditCaseIdList(InstallsCaseData* caseData, wstring& caseName, vector<wstring>* AppPathList) = 0;

	//************************************
	// Method:    GetCaseInfoFromLib
	// FullName:  InstallCaseInterface::GetCaseInfoFromLib
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: wstring & caseName
	// Parameter: _INSTALLS_CASE_DATA_TYPE & caseType	�½���������
	// Parameter: vector<wstring> * AppPathList					�½��������·���б�
	// Explaint:	�ӿ��д��������������½���������
	//************************************
	virtual void GetCaseInfoFromLib(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* AppPathList) = 0;

	//************************************
	// Method:    GetCaseInfoFromFolder
	// FullName:  InstallCaseInterface::GetCaseInfoFromFolder
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: wstring & caseName
	// Parameter: _INSTALLS_CASE_DATA_TYPE & caseType
	// Parameter: wstring & folder
	// Explaint:	���ļ����д������������û����뷽�����Ʋ�ѡ���ļ���·��
	//************************************
	virtual void GetCaseInfoFromFolder(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, wstring& folder) = 0;

	//************************************
	// Method:    GetCaseName
	// FullName:  InstallCaseInterface::GetCaseName
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: wstring & caseName
	// Explaint:	��ȡ��������
	//************************************
	virtual void GetCaseName(wstring& caseName) = 0;

	//************************************
	// Method:    RemoveConfirm
	// FullName:  InstallCaseInterface::RemoveConfirm
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: /*= 0
	// Parameter: bool isRemoveFile	�Ƿ�ɾ���ļ�
	// Explaint:	ȷ���Ƿ�Ҫɾ��
	//************************************
	virtual bool RemoveAppConfirm(bool isRemoveFile) = 0;
	
	virtual bool RemoveCaseConfirm() = 0;

	virtual bool RemoveFolderConfirm() = 0;

	virtual bool IsNeedInstalliTunesConfirm() = 0;

	//************************************
	// Method:    GetFolder
	// FullName:  InstallCaseInterface::GetFolder
	// Access:    virtual public 
	// Returns:   std::wstring
	// Qualifier: /*= 0
	// Parameter: _INSTALLS_CASE_DATA_TYPE type
	// Explaint:	��ȡ�ļ���·��
	//************************************
	virtual wstring GetFolder(_INSTALLS_CASE_DATA_TYPE type) = 0;

	//************************************
	// Method:    GetAppPath
	// FullName:  InstallCaseInterface::GetAppPath
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: _INSTALLS_CASE_DATA_TYPE type
	// Parameter: vector<wstring> * pathList �ļ�·���б�
	// Explaint:	��ȡ�ļ�·��
	//************************************
	virtual void GetAppPath(_INSTALLS_CASE_DATA_TYPE type, vector<wstring>* pathList) = 0;

	//������
	virtual void AddAppToView(int caseId, InstallsAppInfo* appInfo) = 0;

	//���·�����Ϣ
	virtual void UpdateCaseView(InstallsCaseData* caseData) = 0;

	virtual void CancelAppUpgrate( int caseId, wstring appPath ) = 0;
};

INSTALLCASE_END