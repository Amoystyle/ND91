#ifndef _UIUSERCONTROL_H
#define _UIUSERCONTROL_H

class DUI_API CCompositeUI: public CLayoutUI , public INotifyUI
{
public:
	CCompositeUI();
	virtual ~CCompositeUI();
	UI_DECLARE_DYNCREATE();
public:
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	virtual bool Create(LPCTSTR lpszTemplateName, CContainerUI* pParentCtn);

	virtual bool CreateEx(LPCTSTR lpszTemplateName, CContainerUI* pParentCtn, LPCTSTR lpszCtrlId);

	virtual bool ProcessNotify(TNotifyUI* pNotify){ return false;}

	virtual bool FilterProcessNotify(TNotifyUI* pNotify);

	virtual void OnCreate(){}
	virtual void OnDestory();

	void AddNotifier(INotifyUI* pNotifyUI);

	void RemoveNotifier(INotifyUI* pNotifyUI);
protected:
	CStdPtrArray m_aNotifiers;
};



#endif // _UIUSERCONTROL_H