/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef _UILABEL_H
#define _UILABEL_H

class DUI_API CLabelUI:public CControlUI
{
public:
	CLabelUI();
	virtual ~CLabelUI();
	UI_DECLARE_DYNCREATE();
public:
	void  SetText(LPCTSTR lpszText);
	virtual void  SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void  SetRect(RECT& rect);
protected:
	bool m_bSizeWithContent;
};

#endif