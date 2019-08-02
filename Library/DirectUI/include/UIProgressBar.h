/*********************************************************************
 *       Copyright (C) 2011,应用软件开发
 **********************************************************************
 *   Date             Name                 Description
 *   2011-03-26       sundb
*********************************************************************/

#ifndef UIPROGRESSBAR_H
#define UIPROGRESSBAR_H
class DUI_API CProgressBarUI:public CControlUI
{
public:
	CProgressBarUI();
	virtual ~CProgressBarUI();
	UI_DECLARE_DYNCREATE();
public:
	void SetRange(const unsigned long long& firstpos,const unsigned long long& lastpos);
	void SetPos(const unsigned long long& curpos, bool bUpdate = false); 
	void GetRange(unsigned long long& firstpos ,unsigned long long& lastpos);
	unsigned long long GetPos();
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
protected:
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	unsigned long long m_ulfirstPos;
	unsigned long long m_ullastPos;
	unsigned long long m_ulcurPos;
	bool m_bHorizontal;
};

#endif