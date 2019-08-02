#pragma once

#include "Logic/NetInstallsCase/NetInstallPlanTaskManager.h"

class InstallCaseItem :
	public CCompositeUI,
	public NetInstallCaseObersever,
	public BaseHandler<NotifyTaskItemMsg>
{
public:

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"closebtn", OnColseBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseStatusbtn", OnBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnHover", OnBtnShowDetailClick);
	UIEND_MSG_MAP


	InstallCaseItem(InstallsCaseData* pCaseData,wstring deviceId);
	~InstallCaseItem(void);

	void OnCreate();

	void RefreshCase();

	void UpdateCaseName();//刷新方案名称

	void UpdateNetCaseStatus();//刷新方案状态

	void InitCaseImage();//刷新方案图标

	void SetCaseTotalSize();// 刷新方案大小

	void LockCase(bool bLock);//锁定删除按钮

	void InstallFinished();

	bool OnColseBtnClick(TNotifyUI*  pNotify);

	bool OnBtnClick(TNotifyUI*  pNotify);

	bool OnBtnShowDetailClick(TNotifyUI*  pNotify);

	bool OnEventHover( TEventUI& event );

	void NetInstallCaseStatusUpdated();

	virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId);
	virtual void TaskFailNotify(CTaskItem* item, wstring deviceId);
	virtual void TaskRemoveNotify(int taskid, wstring deviceId);
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId);//任务进度变化

	void LocalInstalling();

	void UpdateLocalInstallCaseProgress();

	void ResetInstall();

	virtual bool Handle(const NotifyTaskItemMsg* pMsg);
private:

	InstallsCaseData* _pCaseData;

	CLayoutUI* _pMainLayout;

	CLayoutUI* _pCaseImageCtrl;

	CButtonUI* _pBtnClose;

	CControlUI* _pNetCaseFlag;

	CControlUI* _pLegalflag;

	CControlUI* _pCpaflag;

	CLabelUI*  _pTotalSizeCtrl;

	CProgressBarUI* _pProgBar;

	CButtonUI* _pBtnStatus;

	CLabelUI*  _pCaseNameCtrl;

	CButtonUI* _pBtnHover;

	bool       _bIsNetCase;

	wstring    _deviceId;

	NetInstallPlanTaskManager* mNetInstallPlanTaskManager;

	vector<int>  _InstallTaskIds;

	map<int, int> _installtaskpercents;

	int          _totalInstallApp;

	int          _curInstallpercent;

	int          _lastpercent;
	bool         _bExit;
};
