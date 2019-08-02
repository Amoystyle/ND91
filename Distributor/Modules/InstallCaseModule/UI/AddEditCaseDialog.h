#pragma 

INSTALLCASE_BEGIN

class InstallCaseUI;
class InstallCaseAppInfoListUI;

class AddEditCaseDialog 
	: public CWindowUI
	, public CaseAppSelectObserve
{
public:
	virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"IosAppList", OnSwitchIOS)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndroidAppList", OnSwitchAndroid)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnOk", OnBtnOk)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnCancel", OnBtnCancel)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"LastVerChk", OnBtnShowLaseVerOnly)
	UIEND_MSG_MAP

	AddEditCaseDialog(InstallCaseUI* owner, InstallsCaseData* editCaseData = NULL);
	~AddEditCaseDialog();

	void CreateView(InstallsCaseData* caseData);

	void AddAppInfoToView(int caseId, InstallsAppInfo* appInfo );

	void RemoveAppInfoFromView(int caseId, wstring appPath);

	void ClearAppFromView(int caseId, AppSearchContextEx* context);

	void UpdateCaseView(int caseId);

	void SetDefaultCaseName(wstring caseName);

	void SetDefaultCaseType(_INSTALLS_CASE_DATA_TYPE caseType);

	void GetCaseInfo(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* pathList);

	virtual void Selected();
private:
	void OnCreate();

	bool OnBtnShowLaseVerOnly(TNotifyUI* pNotify);

	bool OnSwitchIOS(TNotifyUI* pNotify);

	bool OnSwitchAndroid(TNotifyUI* pNotify);

	bool OnBtnOk(TNotifyUI* pNotify);

	bool OnBtnCancel(TNotifyUI* pNotify);

	void Search(InstallCaseAppInfoListUI* listUI);

	void SetTabName(CRadioUI* tabCtrl, wstring text, InstallsCaseData* caseData);

private:
	InstallCaseUI* _pOwner;

	InstallsCaseData* _pEditCaseData;

	bool _bCanLoad;
	wstring _mDefaultCaseName;
	_INSTALLS_CASE_DATA_TYPE _mDefaultType;

	CContainerUI* _pContainer;

	CRadioUI* _pIosAppRadio;
	CRadioUI* _pAndroidAppRadio;

	InstallCaseAppInfoListUI* _pIOSAppList;
	InstallCaseAppInfoListUI* _pAndroidAppList;

	CCheckBoxUI* _pLastVerChk;

	CEditUI* _pEdit;
	CLayoutUI* _pTabBar;
	CLabelUI* _pCaseInfo;
};

INSTALLCASE_END