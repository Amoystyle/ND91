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

	void SetBtnState(bool cancel = true,bool con = true);		//������������ֹ��ť�Ƿ����   trueΪ����

	void SetRequestId(int id);

	void OnCreate();
private:
	CLabelUI*       _pMainProgressName; // ����������
	CLabelUI*       _pSubProgressName;  // ����������
	CProgressBarUI* _pMainProgress;     // �����������   
	CProgressBarUI* _pSubProgress;      // �����������
	CButtonUI*      _pBtnCancel;        // ��ֹ��ť
	CButtonUI*      _pBtnContinue;      // ������ť
	int				_RequestID;
	bool			_cancel;
	bool			_con;
	
};