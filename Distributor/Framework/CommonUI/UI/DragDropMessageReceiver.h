#pragma once
#include "Base/BaseCSLock.h"
//只适用于Drop时有模态窗口的操作
class COMMONUI_API DragDropMessageReceiver:public BaseCSLock
{
public:
	DragDropMessageReceiver(){}
	~DragDropMessageReceiver(void){}

public:
	//************************************
	// Method:    DoMessage
	// FullName:  DragDropMessageReceiver::DoMessage
	// Access:    public static 
	// Returns:   void
	// Qualifier: 消息分发处调用（目前是在MainDlg的WindowProc中）
	// Parameter: UINT message
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	static void DoMessage(UINT message, WPARAM wParam, LPARAM lParam);

    static CRITICAL_SECTION	_pLock;

protected:
	//************************************
	// Method:    OnDragDropMessage
	// FullName:  DragDropMessageReceiver::OnDragDropMessage
	// Access:    virtual protected 
	// Returns:   bool
	// Qualifier: 接口消息 
	// Parameter: UINT message		消息类型（message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END ）
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	virtual bool OnDragDropMessage(UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL){ return false;}

private:
	static DragDropMessageReceiver* Receiver;

public:	
	//************************************
	// Method:    SendMessage
	// FullName:  DragDropMessageReceiver::SendMessage
	// Access:    public static 
	// Returns:   void
	// Qualifier: 发送消息
	// Parameter: HWND hWnd								窗体句柄
	// Parameter: UINT message							消息类型（message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END ）
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	// Parameter: DragDropMessageReceiver * receiver	接收者
	//************************************
	static void SendMessage(HWND hWnd, UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL, DragDropMessageReceiver* receiver = NULL);
};