/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-12-2		 hanzp				  Create.
*********************************************************************/

#ifndef UISLIDER_H
#define UISLIDER_H

class DUI_API CSliderUI:public CControlUI
{
public:
	CSliderUI();
	virtual ~CSliderUI();
	UI_DECLARE_DYNCREATE();
public:
	enum
	{
		enDefaultTimerID = 10,
		enDefaultTimerElapse = 100
	};
public:
 	int				GetRange();
 	void			SetRange(int nRange,bool bRedraw = true);
 	int				GetPos();
 	void			SetPos(int nPos,bool bRedraw = true);
	int				GetForePos();
	void			SetForePos(int nPos, bool bRedraw = true);
	virtual void	SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

protected:
	virtual bool	Event(TEventUI& event);
	virtual void	Render(IRenderDC* pRenderDC, RECT& rcPaint);
	void			CalcRect(POINT ptMouse);
private:
	bool			m_bHorizontal;
	bool			m_bMovingScroll;
	bool			m_bProcess;
	int				m_nRange;
	int				m_nPos;
	int				m_nForePos;
	RECT			m_rcThumb;
	RECT			m_rcImageRender;
	RECT			m_rcForeImage;
	RECT			m_rcPosImageRender;
};
#endif