#include "StdAfx.h"
#include "MessageBoxUI.h"
MessageBoxUI::MessageBoxUI(void)
{
    _nDelayTime = 2000;
	m_nRoundRgn=10;
}

MessageBoxUI::~MessageBoxUI(void)
{

}
void CALLBACK boxTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	//检测是否创建了关闭信号量
	::KillTimer(hwnd,idEvent);    
   
    PostMessage(hwnd,WM_CLOSE,IDCLOSE,0);
}

LRESULT MessageBoxUI::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_CLOSE)
	{
		//解决关闭后，界面没有刷新问题。原因：在线程中（boxTimerProc）调用Invalidate导致
		RECT rcWindow;
		::GetWindowRect(this->GetHWND(), &rcWindow);
		HWND hParent = ::GetParent(this->GetHWND());
		::GetWindowRect(hParent, &rcWindow);
		POINT pt = {rcWindow.left, rcWindow.top};
		::ScreenToClient(hParent, &pt);
		CWindowUI* pWindow = GetUIEngine()->GetWindow(hParent);
		if (pWindow)
		{        
			RECT rcDraw = {pt.x, pt.y, pt.x+rcWindow.right-rcWindow.left, pt.y+rcWindow.bottom-rcWindow.top};
			pWindow->Invalidate(&rcDraw);
		}
	}
	return CWindowUI::WindowProc(message,wParam,lParam);
}
int MessageBoxUI::Show(HWND hParent, wstring stateIcon, wstring notifycontent, int textWidth, int nDelayTime)
{
    _nDelayTime = nDelayTime;
	if(!m_hWnd)
		Create(hParent, _T("MessageBox"));
	RETURN_VALUE_IF(!m_hWnd, 0);
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	this->SetTitle(L"");

	if(GetItem(_T("centerlayout.message")))
	{
		GetItem(_T("centerlayout.message"))->SetText(notifycontent.c_str());
	}
	

	CControlUI *pimg = GetItem(_T("Imgicon"));
	if (pimg) 
	{
		pimg->SetAttribute(L"bk.image" ,stateIcon.c_str());
		pimg->Resize();
		pimg->Invalidate();
	}


	RECT parentR;
	::GetWindowRect(hParent, &parentR);
	int length=textWidth/7;
	if(notifycontent.find(L"\r\n")!=wstring::npos)
	{
		string contentstr=CCodeOperation::ws2s(notifycontent);
		vector<string> strlines=CStrOperation::parseLines(contentstr);
		for(size_t i=0;i<strlines.size();i++)
		{
			int linelen=strlines[i].length();
			length=length<linelen?linelen:length;
		}
	}

	if(length<1)
		length=notifycontent.length()*2;
	RECT mySize;
	::GetWindowRect(m_hWnd, &mySize);
	int width = length*7+28+42+10+28+50;
	int height = mySize.bottom - mySize.top;
	MoveWindow(m_hWnd,(parentR.right+parentR.left+200 - width)/2,
		(parentR.bottom+parentR.top+100 -height)/2,width,height,true);

	::SetTimer(m_hWnd,0,_nDelayTime,&boxTimerProc);
//	PublicFunc::SetWindowsRgnEx(m_hWnd,5);
	this->WindowBringToTop();
	ShowWindow(SW_SHOW);
	

	return 0;
}
