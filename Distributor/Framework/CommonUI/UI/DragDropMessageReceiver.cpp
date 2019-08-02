#include "StdAfx.h"
#include "DragDropMessageReceiver.h"

DragDropMessageReceiver* DragDropMessageReceiver::Receiver = NULL;
CRITICAL_SECTION DragDropMessageReceiver::_pLock;

void DragDropMessageReceiver::DoMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    SetCSLockExt(DragDropMessageReceiver::_pLock);

	if(message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END)
	{	
		if(DragDropMessageReceiver::Receiver != NULL)
		{
			DragDropMessageReceiver::Receiver->OnDragDropMessage(message, wParam, lParam);

			DragDropMessageReceiver::Receiver = NULL;
		}
	}
}

void DragDropMessageReceiver::SendMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, DragDropMessageReceiver* receiver)
{
	if(receiver == NULL)
		return;

	if(message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END)
	{	
		DragDropMessageReceiver::Receiver = receiver;
		PostMessage(hWnd, message, wParam, lParam);
	}
}
