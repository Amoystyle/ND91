#pragma once
#include "Base/BaseCSLock.h"
//ֻ������Dropʱ��ģ̬���ڵĲ���
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
	// Qualifier: ��Ϣ�ַ������ã�Ŀǰ����MainDlg��WindowProc�У�
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
	// Qualifier: �ӿ���Ϣ 
	// Parameter: UINT message		��Ϣ���ͣ�message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END ��
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
	// Qualifier: ������Ϣ
	// Parameter: HWND hWnd								������
	// Parameter: UINT message							��Ϣ���ͣ�message > WM_DRAGDROP_BEGIN && message < WM_DRAGDROP_END ��
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	// Parameter: DragDropMessageReceiver * receiver	������
	//************************************
	static void SendMessage(HWND hWnd, UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL, DragDropMessageReceiver* receiver = NULL);
};