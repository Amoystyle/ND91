#include "stdafx.h"
#include "ProgressRecvMsg.h"

ProgressRecvMsg::ProgressRecvMsg()
{
	_progressWnd = new CommonProgressWnd;
	_progressUI = new CommonProgressContainer;
	_pContainer = NULL; 
}

int ProgressRecvMsg::Init( HWND wnd ,CContainerUI* pContainer,HANDLE nHandle)
{
	_pContainer = pContainer;
	if (_pContainer)
		_progressUI->Init(pContainer);
	else
		_progressWnd->Init(wnd,nHandle);
	return 0;
}


bool ProgressRecvMsg::Handle( const ProgressMsg* pMsg )
{
	if (pMsg && pMsg->nRequestID == _RequestID)
	{
		if (_pContainer)
		{
			_progressUI->SetMainPos(pMsg->mainPos);
			_progressUI->SetMainText(pMsg->mainText);
			_progressUI->SetSubPos(pMsg->subPos);
			_progressUI->SetSubText(pMsg->subText);
		}
		else
		{
			if (pMsg->wndHide)
			{
				Hide();
				return true;
			}
			_progressWnd->SetMainPos(pMsg->mainPos);
			_progressWnd->SetMainText(pMsg->mainText);
			_progressWnd->SetSubPos(pMsg->subPos);
			_progressWnd->SetSubText(pMsg->subText);
		}
		
	}
	return true;
}

bool ProgressRecvMsg::Handle( const ProgressShowMsg* pMsg)
{
	if (pMsg->deviceId == _deviceId)
		Show(pMsg->RequestID);
	return true;
}

bool ProgressRecvMsg::Handle( const ProgressShowExistMsg* pMsg )
{
	HWND hwnd = NULL;
	if (_RequestID == pMsg->nRequestID)
	{
		if (_pContainer)
			hwnd = _progressUI->GetWindow()->GetHWND();
		else
			hwnd = _progressWnd->GetHWND();


		Common_EXIST_ACTION nReturn = CommonUIFunc::GetExistAction(pMsg->name,hwnd);

		ProgressShowExistReturnMsg msg;
		msg.nRequestID = pMsg->nRequestID;
		msg.return_value = (int)nReturn;
		msg.SendToUI();
	}
	
	return true;
}

void ProgressRecvMsg::SetBtnState( bool cancel /*= true*/,bool con /*= true*/ )
{
	if (_pContainer)
	{
		_progressUI->SetBtnState(cancel,con);
	}
	else
	{
		_progressWnd->SetBtnState(cancel,con);
	}
}

void ProgressRecvMsg::Show( int id)
{
	_RequestID = id;
	if (_pContainer)
	{
		_progressUI->SetRequestId(id);
		_progressUI->Show();
	}
	else
	{
		_progressWnd->SetRequestId(id);
		_progressWnd->Show();
	}
}

void ProgressRecvMsg::Hide()
{
	if (_pContainer)
	{
		_progressUI->Hide();
	}
	else
	{
		_progressWnd->CloseWindow();
	}
}

