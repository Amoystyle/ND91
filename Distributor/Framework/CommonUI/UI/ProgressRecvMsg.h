#pragma once
#include "CommonProgressWnd.h"
#include "CommonProgressContainer.h"

class COMMONUI_API ProgressRecvMsg 
	: public BaseHandler<ProgressMsg>
	, public BaseHandler<ProgressShowMsg>
	, public BaseHandler<ProgressShowExistMsg>
{
public:
	ProgressRecvMsg();
public:
	int Init(HWND wnd,CContainerUI* pContainer = NULL,HANDLE nHandle = NULL);		//id:消息唯一标识

	void SetBtnState(bool cancel = true,bool con = true);		//设置跳过与终止按钮是否可用   true为可用

private:

	virtual bool Handle(const ProgressMsg* pMsg);

	virtual bool Handle(const ProgressShowMsg* pMsg);

	virtual bool Handle(const ProgressShowExistMsg* pMsg);		//弹出文件重复对话框

	void Show(int id);	//在窗口模式下仅设置id用    在Container时才有show作用

	void Hide();
private:
	CommonProgressWnd *_progressWnd;
	CommonProgressContainer *_progressUI;
	CContainerUI* _pContainer;
	int _RequestID;
public:
	wstring _deviceId;
};