#ifndef DevicePanelCaselistUI_h__
#define DevicePanelCaselistUI_h__

#include "Logic/NetInstallsCase/NetInstallPlanTaskManager.h"


#define WELCOME_CASE_PER_PAGE_COUNT  10
#define DEVICE_CASE_PER_PAGE_COUNT   7

class DevicePanelCaselistUI 
	: public BaseHandler<InstallCaseMsg>
	//, public DragDropMessageReceiver
	, public BaseView
	, public BaseHandler<LoadFinshedMsg>
	, public BaseHandler<UIMiniSizeMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CasePanelBtnAddCase", OnAddnewCase)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CasePanelBtnShowCase", OnShowCase)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CasePanelBtnInstallCase", OnInstallCase)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btnPageButton", OnPageChange);
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"moresolution", OnMoreCase);
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"moresolution_net", OnMoreCase);
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"EmptyCaseTipClose", OnEmptyCaseTipClose);
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"EmptyCaseBK", OnBtnEmptyNotify);
	EVNET_ID_HANDLER(UINOTIFY__AutoGrid_PageChanged,L"case_panel",OnCasePageChanged);

	UIEND_MSG_MAP
		DevicePanelCaselistUI();
	~DevicePanelCaselistUI();

	wstring GetStyleID();

	virtual void OnCreate();
	void SetCurrentDeviceID(wstring id);
	virtual void RefreshSolution(bool addplan=false);
	void CloseShowEmptyTip();

	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	//���װ��������ԭOnDropFiles�в���
	void AddCase(vector<wstring> * files);

	void Release();

private:

	void InsertCase(InstallsCaseData *CaseData, wstring name, size_t nSize,UINT nCount , wstring nInstallTime, CContainerUI* casePanel, bool showNetDeviceNull=false);
	void InsertAddCaseButton();
	void InsertPages(bool netCase = false, bool addplan = false);
	void ShowCurrPageCaseData(bool addplan = false);

	void GetCurDeviceCaseList(vector<InstallsCaseData *> caselist,vector<InstallsCaseData *>& Validatecaselist, const DeviceData* pDevData, bool bCpa=false);

	virtual void InsertCase(InstallsCaseData *CaseData);

	bool OnAddnewCase(TNotifyUI* pNotify);
	bool OnShowCase(TNotifyUI* pNotify);
	bool OnInstallCase(TNotifyUI* pNotify);
	bool OnDropFiles(TNotifyUI* pNotify);
	bool OnPageChange(TNotifyUI* pNotify);
	bool OnMoreCase(TNotifyUI* pNotify);
	bool OnEmptyCaseTipClose(TNotifyUI* pNotify);
	bool OnBtnEmptyNotify(TNotifyUI* pNotify) {return true;};
	bool OnCasePageChanged(TNotifyUI* pNotify);
	bool OnNetCasePageChanged(TNotifyUI* pNotify);

	//��Ϣ����
	virtual bool Handle(const InstallCaseMsg* pMsg);

	virtual bool Handle(const LoadFinshedMsg* pMsg);

	virtual bool Handle(const UIMiniSizeMsg* pMsg);
	//virtual bool OnDragDropMessage( UINT message, WPARAM wParam, LPARAM lParam );

	//���autogrid �ؼ���ʾ��û��ɣ�ֱ�ӵ�����µĿؼ���ʾ����
	static void WINAPI TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);
private:
	CLayoutUI* _pCase_Bottom;
	CLayoutUI* _pNetCase_Bottom;
	CLayoutUI * _pEmptyCaseTip;
	CAutoGridLayoutUI *_CasePanel;
	CLayoutUI* _CasePages;
	int mCurrPage;
	int nLoadCaseNum;
	CButtonUI *_BtnAddNewCase;
	wstring _curdeviceId;

	CAutoGridLayoutUI *_NetCasePanel;
	CLayoutUI* _NetCasePages;
	int mNetCurrPage;
	int nLoadNetCaseNum;

	bool m_AddPlanStart;	//�Ƿ񱾵����װ�����������ڱ�����ӳɹ���ѡ�����һҳ��
	bool m_AddPlanSuccess;	//�������װ�������Ƿ�ɹ������ڱ�����ӳɹ���ѡ�����һҳ��

	bool m_islocked;

	static bool _bCanBtnClick;

	bool _bUIMiniSize;
};
#endif