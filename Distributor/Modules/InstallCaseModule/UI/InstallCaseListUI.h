#pragma once
#include "UI/DragDropMessageReceiver.h"
#include "MainView.h"
INSTALLCASE_BEGIN

#define IDR_MENU_NEW_CASE_FROM 54040
#define IDR_MENU_NEW_CASE_FROM_LIB 54041
//#define IDR_MENU_NEW_CASE_FROM_LIB_IOS 18001001
//#define IDR_MENU_NEW_CASE_FROM_LIB_ANDROID 18001002
#define IDR_MENU_NEW_CASE_FROM_FOLDER 54042
#define IDR_MENU_NEW_CASE_FROM_FOLDER_IOS 54042001
#define IDR_MENU_NEW_CASE_FROM_FOLDER_ANDROID 54042002

//#define IDR_MENU_CASE_LIST 54030
#define IDR_MENU_EDIT_CASE 54032
#define IDR_MENU_DELETE_CASE 54031
#define IDR_MENU_OPEN_CASE 54033

class InstallCaseUI;
class LocalAppinfoList;

class InstallCaseListUI
	: public CCompositeUI
	//, public MessageReceiver
	//, public DragDropMessageReceiver
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CreateCaseBtn", OnCreateCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AddNewCase", OnAddNewCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"RemoveCaseBtn", OnRemoveCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnShow", OnShowCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnDeleteCase", OnRemoveSingleCaseBtnClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_NEW_CASE_FROM, OnAddNewCaseFrom)
		EVNET_ID_HANDLER(UINOTIFY_DBCLICK, L"List", OnCaseListDBClick)
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"List", OnShowMenuCaseList)
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"List", OnDropFiles)
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"ListEmpty", OnDropFiles)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_CASE_LIST, OnClickMenuCaseList)
	UIEND_MSG_MAP

	InstallCaseListUI(InstallCaseUI* caseUI);
	~InstallCaseListUI(void);

public:
	//向列表添加方案
	void AddCaseToView(InstallsCaseData* caseData);

	//更新方案
	void UpdateCaseToView(InstallsCaseData* caseData);

	//从列表中删除方案
	void RemoveCaseFromView(int caseId);

	// 接受消息
	//快捷键的消息暂时不用
	//virtual void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);

	//消息处理
	//virtual bool OnDragDropMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	bool  ListFocused();

	void OnCreate();

	bool OnCreateCaseBtnClick(TNotifyUI* pNotify);

	bool OnAddNewCaseBtnClick(TNotifyUI* pNotify);

	bool OnRemoveCaseBtnClick(TNotifyUI* pNotify);

	bool OnRemoveSingleCaseBtnClick(TNotifyUI* pNotify);

	bool OnShowCaseBtnClick(TNotifyUI* pNotify);

	bool OnAddNewCaseFrom(TNotifyUI* pNotify);

	bool OnCaseListDBClick(TNotifyUI* pNotify);

	bool OnShowMenuCaseList(TNotifyUI* pNotify);

	bool OnClickMenuCaseList(TNotifyUI* pNotify);

	bool OnDropFiles(TNotifyUI* pNotify);

private:

	InstallCaseUI* _pInstallCaseUI;

	CListUI* _pList;	
	LocalAppinfoList* _pListOwner;
	CLayoutUI* _pListPanel;
	CLayoutUI* _pListEmpty;
};

INSTALLCASE_END