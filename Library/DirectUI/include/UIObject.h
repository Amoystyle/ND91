#ifndef UIOBJECT_H
#define UIOBJECT_H
class DUI_API CObjectUI:public CControlUI
{
public:
	CObjectUI(void);
	virtual ~CObjectUI(void);
	UI_DECLARE_DYNCREATE();
public:
	void Docked(HWND hWnd);
	virtual void SetRect(RECT& rectRegion);
private:
	HWND m_hWnd;
};
#endif