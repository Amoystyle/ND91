#ifndef ANIMATIONWND_H_
#define ANIMATIONWND_H_

typedef BOOL (WINAPI *UPDATELAYEREDWINDOW)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD); 

class CAnimationWnd : public CWindowBase
{
public:
	CAnimationWnd();
	virtual ~CAnimationWnd();
	bool ShowWindow( int nCmdShow = SW_SHOW );
	void CloseWindow( );

protected:
	UINT SetTimer( UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillTimer( UINT nIDEvent );
	void Create( HWND hParent=NULL, LPCTSTR lpWindowID=_T("default"), int x=0, int y=0, int nWidth=0, int nHeight=0 );
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(UINT_PTR nIDEvent);

protected:
	HDC m_hPaintDC;
	RECT m_rcWindow;
	UINT m_iWindowWidth;
	UINT m_iWindowHeight;
	DWORD m_dwExStyle;
	DWORD m_dwStyle;
	BLENDFUNCTION m_BlendFun;
	UPDATELAYEREDWINDOW UpdateLayeredWindow;

private:
	HINSTANCE m_hInstance;
};

#endif