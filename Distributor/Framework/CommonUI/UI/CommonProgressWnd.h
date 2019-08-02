#pragma once
#include "CommonProgressContainer.h"

class COMMONUI_API CommonProgressWnd : public CWindowUI
{
public:
	CommonProgressWnd();
public:
	int Init(HWND wnd ,HANDLE handle);
	void SetMainPos(int pos);
	void SetSubPos(int pos);

	void SetMainText(wstring text);
	void SetSubText(wstring text);

	void Show();

	void SetBtnState(bool cancel = true,bool con = true);		//设置跳过与终止按钮是否可用   true为可用
	void SetRequestId(int id);

	void OnInitShow();
private:
	CommonProgressContainer *_progress;
	CLayoutUI* _pPrgContainer;//进度条
	bool _cancel;	//终止按钮是否可用
	bool _con;		//跳过按钮是否可用
	HANDLE _handle;	//启动线程的handle

};