/*********************************************************************
 *       Copyright (C) 2010,Ӧ���������
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef UIRADIO_H
#define UIRADIO_H

class DUI_API CRadioUI:public CCheckBoxUI
{
public:
	CRadioUI();
	virtual ~CRadioUI();
	UI_DECLARE_DYNCREATE();
public:
	virtual void SetChecked(bool bChecked, bool bNotify = true);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue); //���ػ���

protected:
	virtual bool Event(TEventUI& event);
	tstring m_strGroupName;
};
#endif