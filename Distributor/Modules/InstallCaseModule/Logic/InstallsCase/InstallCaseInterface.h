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
	// Parameter: wstring & caseName 编辑后的方案名称
	// Parameter: vector<int> * appList	编辑后的软件Id列表
	// Explaint:	编辑方案，弹出编辑方案窗体
	//************************************
	virtual void GetEditCaseIdList(InstallsCaseData* caseData, wstring& caseName, vector<wstring>* AppPathList) = 0;

	//************************************
	// Method:    GetCaseInfoFromLib
	// FullName:  InstallCaseInterface::GetCaseInfoFromLib
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: wstring & caseName
	// Parameter: _INSTALLS_CASE_DATA_TYPE & caseType	新建方案名称
	// Parameter: vector<wstring> * AppPathList					新建方案软件路径列表
	// Explaint:	从库中创建方案，弹出新建方案窗体
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
	// Explaint:	从文件夹中创建方案，让用户输入方案名称并选择文件夹路径
	//************************************
	virtual void GetCaseInfoFromFolder(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, wstring& folder) = 0;

	//************************************
	// Method:    GetCaseName
	// FullName:  InstallCaseInterface::GetCaseName
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: wstring & caseName
	// Explaint:	获取方案名称
	//************************************
	virtual void GetCaseName(wstring& caseName) = 0;

	//************************************
	// Method:    RemoveConfirm
	// FullName:  InstallCaseInterface::RemoveConfirm
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: /*= 0
	// Parameter: bool isRemoveFile	是否删除文件
	// Explaint:	确定是否要删除
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
	// Explaint:	获取文件夹路径
	//************************************
	virtual wstring GetFolder(_INSTALLS_CASE_DATA_TYPE type) = 0;

	//************************************
	// Method:    GetAppPath
	// FullName:  InstallCaseInterface::GetAppPath
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: /*= 0
	// Parameter: _INSTALLS_CASE_DATA_TYPE type
	// Parameter: vector<wstring> * pathList 文件路径列表
	// Explaint:	获取文件路径
	//************************************
	virtual void GetAppPath(_INSTALLS_CASE_DATA_TYPE type, vector<wstring>* pathList) = 0;

	//添加软件
	virtual void AddAppToView(int caseId, InstallsAppInfo* appInfo) = 0;

	//更新方案信息
	virtual void UpdateCaseView(InstallsCaseData* caseData) = 0;

	virtual void CancelAppUpgrate( int caseId, wstring appPath ) = 0;
};

INSTALLCASE_END