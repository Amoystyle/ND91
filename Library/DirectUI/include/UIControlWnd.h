/*********************************************************************
 *       Copyright (C) 2011,Ӧ���������
 **********************************************************************
 *   Date             Name                 Description
 *   2011-06-07       hanzp
*********************************************************************/

#ifndef __UICONTROLWND_H_
#define __UICONTROLWND_H_

class CControlWndUI;
class CControlWnd;

class DUI_API CControlWndUI : public CControlUI
{
	friend class CControlWnd;
public:
	CControlWndUI(void);
	virtual ~CControlWndUI(void);
	UI_DECLARE_DYNCREATE();

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SetVisible(bool bShow, bool bRedraw = true);
	virtual void SetInternVisible(bool bShow);
	HWND GetHWND();
	bool ShowWnd(bool bShow);
	//************************************
	// @brief:    �󶨴��ھ��
	// @note:     
	// @param: 	  HWND hWnd ����ǰ�󶨾����hWnd��һ��ʱ��һ���ͷ�
	// @param: 	  bool bFloatWindow �Ƿ�Ϊ��������
	// @param: 	  bool bAutoDel �Ƿ����ͷſؼ�ʱ�ݻٴ���
	// @param: 	  bool bBindOffset �Ƿ���ؼ�λ��һ��
	// @return:   void 
	//************************************
	void Docked(HWND hWnd, bool bFloatWindow = false, bool bAutoDel = false);
	void FloatWindow(bool bFloatWindow);
	bool IsFloatWindow();
protected:
	virtual void SetRect(RECT& rectRegion);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void Init();
	void InitControl();
	bool DoCreateControl();
	void ReleaseControl();
	bool IsTop();
	//************************************
	// @brief:    ���ڿ��ƾ���Ƿ���ʾ
	// @note:     �ͷſؼ�ʱ�������أ������øú���
	// @param: 	  bool bShow 
	// @return:   void 
	//************************************
	void ShowWindow(bool bShow);
private:
	bool m_bShowWnd;
	bool m_bAttach;
	bool m_bCreated;
	bool m_bFloatWindow;
	bool m_bAutoDel;
	HWND m_hWindow;
	bool m_bTop;
};

#endif