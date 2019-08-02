#pragma once

#define IDR_MENU_ADD_FOLDER 54050
#define IDR_MENU_ADD_FOLDER_IOS 54051
#define IDR_MENU_ADD_FOLDER_ANDROID 54052

#define  IDR_MENU_FOLDER_LIST 54010
#define  IDR_MENU_FOLDER_REMOVE 54011
#define  IDR_MENU_FOLDER_OPEN 54012

INSTALLCASE_BEGIN

class InstallCaseUI;
class LocalAppinfoList;

class InstallCaseAppFolderListUI
	: public CCompositeUI
	//, public MessageReceiver
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AddFolderBtn", OnAddFolderBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"RefreshCaseBtn", OnRefreshCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnOpen", OnOpenFolderBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnDeleteFolder", OnRemoveFolderBtnClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_ADD_FOLDER, OnAddFolderMenuItemClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_FOLDER_LIST, OnAddFolderMenuItemClick)
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"List", OnShowMenuFolderList)
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"List", OnDropFiles)
		EVNET_ID_HANDLER(UINOTIFY_DBCLICK, L"List", OnListDBClick)
	UIEND_MSG_MAP

	InstallCaseAppFolderListUI( InstallCaseUI* parent);
	~InstallCaseAppFolderListUI(void);

public:
	// 向列表添加Folder
	void AddFolderToView(AppCountByFolder* appFolder);

	//从列表移除项
	void RemoveFolderFromView(AppCountByFolder* appFolder);

private:
	void OnCreate();

	bool OnRefreshCaseBtnClick(TNotifyUI* pNotify);

	bool OnAddFolderBtnClick(TNotifyUI* pNotify);

	bool OnAddFolderMenuItemClick(TNotifyUI* pNotify);

	bool OnOpenFolderBtnClick(TNotifyUI* pNotify);

	bool OnListDBClick(TNotifyUI* pNotify);

	bool OnRemoveFolderBtnClick(TNotifyUI* pNotify);

	bool OnShowMenuFolderList(TNotifyUI* pNotify);

	bool OnDropFiles(TNotifyUI* pNotify);

	void OpenDir(CControlUI* folderItem);

	bool  ListFocused();

	void RemoveSelectFolder();

	// 接受消息
	//快捷键消息暂时不做处理
	//virtual void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);

	int AddListGroupTittle(_INSTALLS_CASE_DATA_TYPE type, wstring text);

private:
	InstallCaseUI* _pInstallCaseUI;

	CListUI* _pList;	
	LocalAppinfoList* _pListOwner;
};

INSTALLCASE_END