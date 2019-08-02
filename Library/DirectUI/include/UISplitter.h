/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-12-1		 hanzp				  Create.
*********************************************************************/

#ifndef UISPLITTER_H
#define UISPLITTER_H

class DUI_API CSplitterUI :public CControlUI
{
public:
	CSplitterUI();
	virtual ~CSplitterUI();
	UI_DECLARE_DYNCREATE();

	void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SetRect(RECT& rectRegion);
protected:
	bool Event(TEventUI& event);
	bool m_bHorizontal; 
	tstring m_szFirstName, m_szSecondName; //前一个控件和后一个控件的名字
	CControlUI* m_pFirst, *m_pSecond;  //第一个控件和第二控件
	int m_nMinFirst, m_nMinSecond;     //第一个控件和第二个控件的最小值
	POINT m_ptLastMouse;
	RECT m_rcFirstCfg, m_rcSecondCfg, m_rcSefCfg; //用于备份第一、第二个控件及自身的初始配置
};
#endif