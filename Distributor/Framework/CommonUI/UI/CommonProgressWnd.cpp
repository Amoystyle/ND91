#include "stdafx.h"
#include "CommonProgressWnd.h"


CommonProgressWnd::CommonProgressWnd()
{
	_progress = new CommonProgressContainer;
}

int CommonProgressWnd::Init( HWND wnd,HANDLE handle)
{
	_handle = handle;
	if(!m_hWnd)
		Create(wnd, _T("CommonProgressWnd"));
	RETURN_VALUE_IF(!m_hWnd, 0);
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	CenterWindow(wnd);

	_pPrgContainer = (CLayoutUI*)GetItem(_T("ProgressContainer"));
	if (_pPrgContainer)
	{
		_progress->SetBtnState(_cancel,_con);
		_progress->Init(_pPrgContainer);
	}
	return 0;
}

void CommonProgressWnd::SetMainPos( int pos )
{
	if (_progress)
		_progress->SetMainPos(pos);
	Invalidate();
}

void CommonProgressWnd::SetSubPos( int pos )
{
	if (_progress)
		_progress->SetSubPos(pos);
	Invalidate();
}

void CommonProgressWnd::SetMainText( wstring text )
{
	if (_progress)
		_progress->SetMainText(text.c_str());
	Invalidate();
}

void CommonProgressWnd::SetSubText( wstring text )
{
	if (_progress)
		_progress->SetSubText(text.c_str());
	Invalidate();
}

void CommonProgressWnd::SetBtnState( bool cancel /*= true*/,bool con /*= true*/ )
{
	_cancel = cancel;
	_con = con;
}

void CommonProgressWnd::SetRequestId( int id )
{
	_progress->SetRequestId(id);
}

void CommonProgressWnd::Show()
{
	this->DoModal();
}

void CommonProgressWnd::OnInitShow()
{
	if (_handle)
		ResumeThread(_handle);
}
