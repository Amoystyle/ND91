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
	int Init(HWND wnd,CContainerUI* pContainer = NULL,HANDLE nHandle = NULL);		//id:��ϢΨһ��ʶ

	void SetBtnState(bool cancel = true,bool con = true);		//������������ֹ��ť�Ƿ����   trueΪ����

private:

	virtual bool Handle(const ProgressMsg* pMsg);

	virtual bool Handle(const ProgressShowMsg* pMsg);

	virtual bool Handle(const ProgressShowExistMsg* pMsg);		//�����ļ��ظ��Ի���

	void Show(int id);	//�ڴ���ģʽ�½�����id��    ��Containerʱ����show����

	void Hide();
private:
	CommonProgressWnd *_progressWnd;
	CommonProgressContainer *_progressUI;
	CContainerUI* _pContainer;
	int _RequestID;
public:
	wstring _deviceId;
};