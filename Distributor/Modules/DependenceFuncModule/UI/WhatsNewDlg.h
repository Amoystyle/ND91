#pragma once
#include "DirectUI.h"

class CWhatsNewDlg :
	public CWindowUI
{
public:
	static CWhatsNewDlg* GetInstance();

public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"btnClose",OnBtnRadio)
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"btnPrev",OnBtnPrev)
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"btnNext",OnBtnNext)        
	UIEND_MSG_MAP

	CWhatsNewDlg(void);
	~CWhatsNewDlg(void);

	void SetPrevOrNextVisible(int nCurSel=-1);		//第一页时隐藏prev 最后一页时隐藏next
	virtual void OnCreate();
    virtual void OnClose();
	bool OnBtnRadio(TNotifyUI* pNotify);
	bool OnBtnPrev(TNotifyUI* pNotify);
	bool OnBtnNext(TNotifyUI* pNotify);
    static void OnSelectItem(int nIndex, void* pParam);    
protected:
	static CWhatsNewDlg* ms_Instance;
};
