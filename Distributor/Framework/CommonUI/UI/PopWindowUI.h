#pragma once
#define POP_DIRECTION_LEFTTOP 0
#define POP_DIRECTION_LEFTCENTER 1
#define POP_DIRECTION_LEFTBOTTOM 2

#define POP_DIRECTION_TOPLEFT 4
#define POP_DIRECTION_TOPCENTER 5
#define POP_DIRECTION_TOPRIGHT 6

#define POP_DIRECTION_RIGHTTOP 7
#define POP_DIRECTION_RIGHTCENTER 8
#define POP_DIRECTION_RIGHTBOTTOM 9

#define POP_DIRECTION_BOTTOMLEFT 10
#define POP_DIRECTION_BOTTOMCENTER 11
#define POP_DIRECTION_BOTTOMRIGHT 12

#define POP_DIRECTION_DEFAULT POP_DIRECTION_RIGHTCENTER

class COMMONUI_API CPopWindowUI :
	public CWindowUI
{
public:
	static CPopWindowUI* GetInstance(LPCTSTR lpWindowID = _T("pop"),LPCTSTR lpBackgroundID = _T("pop_bk"));
	
	CPopWindowUI(void);
	~CPopWindowUI(void);

	// Show之前调用
	virtual void SetWindowID(LPCTSTR lpWindowID = _T("pop"));
	virtual void SetBackgroundID(LPCTSTR lpBackgroundID = _T("pop_bk"));
	virtual void SetKillFocus(bool bKillFocus = true);
	virtual void SetPopDirection(UINT uDirection = POP_DIRECTION_DEFAULT,int nOffsetX = 0,int nOffsetY = 0);
	virtual void SetPopBkImage(LPCTSTR lpbkimg,LPCTSTR imgseq,RECT& imgInset);
	virtual void Show(CControlUI* pOwner = NULL,Animation* pAnimation=NULL);
	// Show之后调用
	virtual void Close();
	virtual RECT GetPopRect();
	virtual void SetPopRect( const RECT& rcPop,UINT uFlags );
	virtual void SetPopRect(const RECT& rcPop,bool bMove = true,bool bResize=true);

protected:
	virtual void ApplyBkImageStyle();
	virtual void ApplyPopPos();
	virtual void OnCreate();
	virtual bool ShowWindow(int nCmdShow = SW_SHOW);
	virtual bool ShowWindow(Animation* pAnimation);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	static CPopWindowUI* ms_Instance;
	bool m_bKillFocus;
	LPCTSTR m_lpWindowID;
	LPCTSTR m_lpBackgroundID;
	Animation* m_pShowAnimation;
	CControlUI* m_pSender;

	// 显示位置
	UINT m_uDirection;
	int m_nOffsetX;
	int m_nOffsetY;

	// 背景图片
	LPCTSTR m_bkImg;
	LPCTSTR m_imgseq;
	RECT m_imgInset;
	bool m_bImgInsetEnable;
};
