#include "stdafx.h"
#include "CommonProgressContainer.h"

CommonProgressContainer::CommonProgressContainer()
{
	_pMainProgressName = NULL; // ����������
	_pSubProgressName = NULL;  // ����������
	_pMainProgress = NULL;     // �����������   
	_pSubProgress = NULL;      // �����������
	_pBtnCancel = NULL;        // ��ֹ��ť
	_pBtnContinue = NULL;      // ������ť
}

wstring CommonProgressContainer::GetStyleID()
{
	return L"CommonProgressContainer";
}


void CommonProgressContainer::Init(CContainerUI* pContainer)
{
	BaseView::Init(pContainer);


	_pSubProgressName = (CLabelUI*)GetItem(_T("SubProgressName"));
	_pSubProgress = (CProgressBarUI*)GetItem(_T("SubProgress"));
	_pMainProgressName = (CLabelUI*)GetItem(_T("MainProgressName"));
	_pMainProgress = (CProgressBarUI*)GetItem(_T("MainProgress"));

	_pBtnCancel = (CButtonUI*)GetItem(_T("BtnCancel"));

	_pBtnContinue = (CButtonUI*)GetItem(_T("BtnContinue"));

	if (_pBtnCancel)
		_pBtnCancel->SetEnabled(_cancel);

	if (_pBtnContinue)
		_pBtnContinue->SetEnabled(_con);
}

void CommonProgressContainer::SetMainPos( int pos )
{
	if (_pMainProgress)
		_pMainProgress->SetPos(pos);
	Invalidate();
}

void CommonProgressContainer::SetSubPos( int pos )
{
	if (_pSubProgress)
		_pSubProgress->SetPos(pos);
	Invalidate();
}

void CommonProgressContainer::SetMainText( wstring text )
{
	if (_pMainProgressName)
		_pMainProgressName->SetText(text.c_str());
	Invalidate();
}

void CommonProgressContainer::SetSubText( wstring text )
{
	if (_pSubProgressName)
		_pSubProgressName->SetText(text.c_str());
	Invalidate();
}

bool CommonProgressContainer::OnCancelBtn( TNotifyUI* pNotify )
{
	ProgressBtnMsg msg;
	msg.cancelBtn = true;
	msg.continueBtn = false;
	msg.nRequestID = _RequestID;
	msg.PostToUI();
	return true;
}

bool CommonProgressContainer::OnContinueBtn( TNotifyUI* pNotify )
{
	ProgressBtnMsg msg;
	msg.cancelBtn = false;
	msg.continueBtn = true;
	msg.nRequestID = _RequestID;
	msg.PostToUI();
	return true;
}

void CommonProgressContainer::SetBtnState( bool cancel /*= true*/,bool con /*= true*/ )
{
	_con = con;
	_cancel = cancel;

	if (_pBtnCancel)
		_pBtnCancel->SetEnabled(_cancel);

	if (_pBtnContinue)
		_pBtnContinue->SetEnabled(_con);
}
// 
// void CommonProgressContainer::Show( int id )
// {
// 	BaseView::Show();
// }

void CommonProgressContainer::SetRequestId( int id )
{
	_RequestID = id;
}

void CommonProgressContainer::OnCreate()
{

}

