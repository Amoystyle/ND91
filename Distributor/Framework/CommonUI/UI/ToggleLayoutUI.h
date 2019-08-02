#pragma once

// 切换触发的回调函数，用这个来接受更改的通知
typedef void (*ToggleLayout_OnSelectItem_CallBack)(int nIndex, void* pParam);
class COMMONUI_API CToggleLayoutUI : public CTabLayoutUI, public INotifyUI, public IStaticUI
{
public:
	UIBEGIN_MSG_MAP
		EVNET_HANDLER(UINOTIFY_CLICK, OnBtnRadio)
	UIEND_MSG_MAP

	CToggleLayoutUI();
	~CToggleLayoutUI();
	UI_DECLARE_DYNCREATE();

	virtual bool SelectItem(CControlUI* pControl);
	virtual bool SelectIndex(int index);

    void SetSelectItemCallBack(ToggleLayout_OnSelectItem_CallBack pCallBack, void* pParam);

    CLayoutUI* GetRadioLayout() { return m_pRadioLayout;};
protected:
	bool OnBtnRadio(TNotifyUI* pNotify);
	bool ChildEvent(TEventUI& event);

protected:
	virtual void Init();
	virtual void SetRect(RECT& rc);
	virtual bool Event(TEventUI& event);
	virtual bool Add(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	bool OnCurAnimation(int nMoveType);
	bool OnNextAnimation(int nMoveType);

    
private:
	CRITICAL_SECTION m_csLock;
	bool m_bLockRender;
	AnimationRender* m_pAnimationRenderCur;
	AnimationRender* m_pAnimationRenderNext;

	bool m_bShowRadio;
	CLayoutUI* m_pRadioLayout;
	int m_nRadioSize;

    ToggleLayout_OnSelectItem_CallBack _pCallBack;
    void* _pCallBackParam;
};