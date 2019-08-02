#pragma once

class DeviceController;
class LeftView
	: public BaseView
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Tree", OnTreeSelChanged);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Refresh", OnRefreshClick);
	UIEND_MSG_MAP

	LeftView(DeviceController* pController);
	~LeftView();

	virtual wstring GetStyleID();

	void OnCreate();

	bool OnTreeSelChanged(TNotifyUI* pNotify);

	bool OnRefreshClick(TNotifyUI* pNotify);

	void AddItem(void* key);

	void RemoveItem(void* key);

	void UpdateItem( void* key, wstring deviceName, bool bCreateModules = false );

	void ClearSubItem(void* key, wstring deviceName, bool bRecoveryMode = false);

	void UpdateErrorText(void* key);

	void UpdateRecoveryText(void* key);

private:
	CTreeNodeUI* FindConnectTreeNode( void* key );

	DeviceController* _pController;
};