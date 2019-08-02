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

	void SetBtnState(bool cancel = true,bool con = true);		//������������ֹ��ť�Ƿ����   trueΪ����
	void SetRequestId(int id);

	void OnInitShow();
private:
	CommonProgressContainer *_progress;
	CLayoutUI* _pPrgContainer;//������
	bool _cancel;	//��ֹ��ť�Ƿ����
	bool _con;		//������ť�Ƿ����
	HANDLE _handle;	//�����̵߳�handle

};