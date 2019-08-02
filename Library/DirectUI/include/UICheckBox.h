/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/

#ifndef _UICHECKBOX_H
#define _UICHECKBOX_H
class DUI_API CCheckBoxUI:public CButtonUI
{
public:
	CCheckBoxUI();
	virtual ~CCheckBoxUI();
	UI_DECLARE_DYNCREATE();

public:
	virtual bool IsChecked();
	virtual void SetChecked(bool bChecked, bool bNotify = true);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue); //重载基类
protected:
	virtual bool Event(TEventUI& event);
};
#endif