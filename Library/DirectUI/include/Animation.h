#ifndef _ANIMATION_H
#define _ANIMATION_H


class DUI_API Animation
{
public:
	Animation();
	virtual ~Animation();
	virtual void Start( CWindowUI *pWindow ) = 0;
	virtual void Stop();

protected:
	HWND m_hWnd;
	RECT m_rcWindow;
	SIZE m_szWindow;
	RECT m_rcAnimationDC;
	SIZE m_szAnimationDC;
	bool m_bLayerWindow;
	CWindowUI *m_pWindow;

	BLENDFUNCTION m_BlendFun;
	DWORD m_hThreadID;
	HANDLE m_hThread;
	int m_iAnimationIndex;
	DibObj* m_pDibObj;
};

class DUI_API CTurnObjectAnimation : public Animation
{
public:
	CTurnObjectAnimation();
	~CTurnObjectAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();
};

class DUI_API CInflateAnimation : public Animation
{
public:
	CInflateAnimation();
	~CInflateAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();
};

class DUI_API CScatterAnimation : public Animation
{
public:
	CScatterAnimation();
	~CScatterAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();

	POINT m_ptCenter;
	SIZE m_szWnd;
};

class DUI_API CBlindAnimation : public Animation
{
public:
	CBlindAnimation( int iBlindType = 0, DWORD dwTimer = 50, bool bActive = false, bool bIn = true );
	~CBlindAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

protected:
	DWORD m_dwTimer;

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();

	POINT m_ptCenter;
	SIZE m_szWnd;
	int m_iBlindType;

	bool m_bActive;
	bool m_bIn;

	enum{ TOP, BOTTOM, LEFT, RIGHT };
};

class DUI_API CCloseAnimation : public Animation
{
public:
	CCloseAnimation();
	~CCloseAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();
};

class DUI_API CPopupAnimation : public Animation
{
public:
	CPopupAnimation();
	~CPopupAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();
};


class DUI_API CMenuAnimation :  public Animation
{
public:
	CMenuAnimation( int iPopupType = 0);
	~CMenuAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();

	int m_iBlindType;
	enum{ LT, RT, LB, RB };
};

class DUI_API CTurnButtonIconAnimation : public Animation
{
public:
	CTurnButtonIconAnimation();
	~CTurnButtonIconAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

protected:
	double m_dAcceleration;

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();
};

class DUI_API CPosChangeAnimation : public Animation
{
public:
	CPosChangeAnimation( int iDisX, int iDisY );
	~CPosChangeAnimation();
	void Start( CWindowUI *pWindow );
	void Stop();

private:
	static DWORD CALLBACK AnimationProc(LPVOID lp);
	void UpdateAnimation();

	int m_iDisX;
	int m_iDisY;
;
};

#endif //_ANIMATION_H