#pragma once

INSTALLCASE_BEGIN

class InstallCaseUI;
class InstallCaseAppInfoListUI;

class InstallCaseButtonItem : public CCompositeUI
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"DeleteBtn", OnDeleteBtnClick)
	UIEND_MSG_MAP

	InstallCaseButtonItem(InstallCaseAppInfoListUI* ctrlList);
	~InstallCaseButtonItem(void);

	virtual void Create(wstring templete, wstring appPath, CLayoutUI* parent, InstallsAppInfo* appInfo);

	void SetText(wstring text);

	void SetIcon(wstring path);

private:
	bool OnDeleteBtnClick(TNotifyUI* pNotify);

private:
	CControlUI* _pIcon;
	CLabelUI* _pLbl;

	InstallCaseAppInfoListUI* _pCtrlList;
};

INSTALLCASE_END