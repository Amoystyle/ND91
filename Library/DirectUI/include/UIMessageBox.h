/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-11-5		 hanzp
*********************************************************************/

#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H
class DUI_API CMessageBoxUI : public CWindowUI
{
public:
	typedef enum _tagMessageBoxType
	{
		enMessageBoxTypeSucceed = 0,
		enMessageBoxTypeInfo,
		enMessageBoxTypeWarn,
		enMessageBoxTypeError,
		enMessageBoxTypeQuestion
	}ENMessageBoxType;
	CMessageBoxUI();
	virtual ~CMessageBoxUI();
	UINT MessageBox(HWND hParent, LPCTSTR lpszText, LPCTSTR lpszCaption, LPCTSTR lpszBtnType, ENMessageBoxType enIconType , LPCTSTR lpszXml = NULL, int nWidth = 340, int nHeight = 150);
	static UINT MessageBoxS(HWND hParent, LPCTSTR lpszText, LPCTSTR lpszCaption, LPCTSTR lpszBtnType, ENMessageBoxType enIconType = enMessageBoxTypeInfo, LPCTSTR lpszXml = NULL, int nWidth = 340, int nHeight = 150);
protected:
	virtual void Notify(TNotifyUI* pNotify);
	UINT m_uIconType;
};
#endif