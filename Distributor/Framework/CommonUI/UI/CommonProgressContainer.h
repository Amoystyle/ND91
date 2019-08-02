#pragma once

class COMMONUI_API CommonProgressContainer : public BaseView
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnContinue", OnContinueBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnCancel", OnCancelBtn)
	UIEND_MSG_MAP
	CommonProgressContainer();
public:
	wstring GetStyleID();
	void Init(CContainerUI* pContainer);

//	int Show(HWND wnd);
	void SetMainPos(int pos);
	void SetSubPos(int pos);

	void SetMainText(wstring text);
	void SetSubText(wstring text);

	bool  OnCancelBtn(TNotifyUI* pNotify);
	bool  OnContinueBtn(TNotifyUI* pNotify);

	void SetBtnState(bool cancel = true,bool con = true);		//设置跳过与终止按钮是否可用   true为可用

	void SetRequestId(int id);

	void OnCreate();
private:
	CLabelUI*       _pMainProgressName; // 主任务名称
	CLabelUI*       _pSubProgressName;  // 子任务名称
	CProgressBarUI* _pMainProgress;     // 主任务进度条   
	CProgressBarUI* _pSubProgress;      // 子任务进度条
	CButtonUI*      _pBtnCancel;        // 终止按钮
	CButtonUI*      _pBtnContinue;      // 跳过按钮
	int				_RequestID;
	bool			_cancel;
	bool			_con;
	
};