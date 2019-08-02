#ifndef _UISCROLLBAR_H
#define _UISCROLLBAR_H

//////////////////////////////////////////////////////////////////////////////////////////

class DUI_API CScrollbarUI:public CControlUI
{
public:
	CScrollbarUI();
	virtual ~CScrollbarUI();
	UI_DECLARE_DYNCREATE();
public:
	enum {
		MIN_THUMB_SIZE = 16,
		DEFAULT_SCROLLBAR_SIZE = 14,
		DEFAULT_SCROLL_TIMERID = 10,
		WHEEL_SCROLL_TIMERID  = 11
	};
public:
	void SetOwner(CControlUI* pOwner);
	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = false);
	int GetScrollRange();
	void SetScrollRange(int nRange);
	int GetScrollPos();
	void SetLineSize(int nLineSize);
	int GetLineSize();
	void PageUp();
	void PageDown();
	void LineSizeUp();
	void LineSizeDown();
	void SetScrollPos(int nPos, int nCode = SB_THUMBPOSITION, bool bRedraw = true);
	SIZE GetFixedSize();

	bool IsMousePress(){return m_bMousePress;};
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void SetRect(RECT& rectRegion);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue); //重载基类

protected:
	void CalcRect();
	void OffsetPos(int nOffset);//for rtl and SB_THUMBPOSITION
	RECT GetThumbRect();//rtl版本下用m_rcthum是相反的值

private:
	bool m_bHorizontal;

	long m_nRange,m_nScrollPos,m_nLastScrollPos,m_nLineSize;
	int m_nScrollRepeatDelay;
	long m_nLastScrollOffset;
	POINT m_ptLastMouse;

	SIZE m_cxyFixed;
	UITYPE_STATE m_iStatusBegin,m_iStatusEnd,m_iStatusThumb;
	RECT m_rcThumb;

	CControlUI* m_pOwner;

	//滚动滚动条中间滚动使用
	LONG  m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;

	bool  m_bMousePress;
};

#endif  //_UISCROLLBAR_H