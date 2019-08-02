#include "StdAfx.h"
#include "WebUIReceiver.h"

WebUIReceiver::WebUIReceiver(void)
{
	_pReceiveUI = NULL;
}

WebUIReceiver::~WebUIReceiver(void)
{
}

void WebUIReceiver::SetReceive( INotifyUI* pINotify )
{
	_pReceiveUI = pINotify;
}

void WebUIReceiver::Send( UINT message, CControlUI* pSender /*= NULL*/, WPARAM wParam /*= NULL*/, LPARAM lParam /*= NULL*/ )
{
	if(_pReceiveUI == NULL)
		return;

	TNotifyUI* pNotify = new TNotifyUI();
	pNotify->pSender = pSender;
	pNotify->sType = message;
	pNotify->wParam = wParam;
	pNotify->lParam = lParam;
	_pReceiveUI->ProcessNotify(pNotify);
}
vector<WebSoftMessageReceiver*> WebSoftMessageReceiver::Receivers;
CRITICAL_SECTION WebSoftMessageReceiver::_pLock;

void WebSoftMessageReceiver::DoMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
//	SetCSLock(_pLock);
	for(vector<WebSoftMessageReceiver*>::iterator it = Receivers.begin(); it != Receivers.end(); it++)
	{
		WebSoftMessageReceiver* receiver = *it;
		if(receiver != NULL)
			receiver->MessageProc(message, wParam, lParam);
	}
}

WebSoftMessageReceiver::WebSoftMessageReceiver()
{
//	SetCSLock(_pLock);
	Receivers.push_back(this);
}

WebSoftMessageReceiver::~WebSoftMessageReceiver( void )
{
//	SetCSLock(_pLock);
	for(vector<WebSoftMessageReceiver*>::iterator it = Receivers.begin(); it != Receivers.end(); it++)
	{
		WebSoftMessageReceiver* receiver = *it;
		if ( receiver == this )
		{
			Receivers.erase(it);
			break;
		}
	}
}
