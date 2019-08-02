#ifndef _UIHTMLLABEL_H
#define _UIHTMLLABEL_H

class DUI_API CHtmlLabelUI:public CControlUI
{
public:
	CHtmlLabelUI();
	virtual ~CHtmlLabelUI();
	UI_DECLARE_DYNCREATE();

	void  SetText(LPCTSTR lpszText);
public:
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual bool Event(TEventUI& event);
private:
	enum {MAXLINK = 8};
	int m_iLinks;
	int m_iHoverLink;
	RECT m_rcLinks[MAXLINK];
	tstring m_strLinks[MAXLINK];
	FontObj *m_pFontObj;
};

#endif