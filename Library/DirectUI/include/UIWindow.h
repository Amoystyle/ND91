#ifndef _UIWINDOW_H
#define _UIWINDOW_H
#include "WindowBase.h"
#include <CommCtrl.h>

#define WM_ASNYC_NOTIFY  (WM_USER+8003)
#define WM_MENU_NOTIFY  (WM_USER+8004)
class  DUI_API CWindowUI : public CWindowBase
{
	friend  class CControlUI;
public:
	CWindowUI();
	virtual ~CWindowUI();

public: 
	UIBEGIN_MSG_MAP
	UIEND_MSG_MAP
public:
	LPCTSTR GetId();
	LPCTSTR GetSubId();
	void SetSubId(LPCTSTR lpszSubId);
	//
	void Create(HWND hParent = NULL, LPCTSTR lpszId = _T("default"),  int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);
	void SkinWindow(HWND  hWnd, LPCTSTR lpszId = _T("default"),  int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);

	int  DoModal();
	bool IsModal();

	virtual bool ShowWindow(int nCmdShow = SW_SHOW);
	virtual bool ShowWindow(Animation* pAnimation);

	void CloseWindow(Animation *pAnimation);
	void CloseWindow(int nResult = IDOK);
	void EndDialog(int nResult = IDOK);

	virtual void CenterWindow(HWND hAlternateOwner = NULL, bool bOverlapped = false, int cx = 0, int cy = 0);
	void SetCloseAnimation(Animation* pAnimation);

	void MaximizeWindow();
	void MinimizeWindow();	
	void RestoreWindow();
	bool IsMaximized();
	bool IsMinimized();

	void ActiveWindow();
	void ShowAndActiveWindow();
	

	void BlinkWindow();
	void WindowBringToTop();
	void WindowBringToNoTop();
	//   ����������
	
	virtual void SetTitle(LPCTSTR lpszText);
	LPCTSTR GetTitle();
	void SetIcon(LPCTSTR lpszIcon);
	void SetBigIcon(LPCTSTR lpszIcon);

	void SetEnableResize(bool bResize);
	bool IsEnableResize();
	void SetMinSize();
	void SetMinSize(int nWidth, int nHeight);
	void GetMinSize(int& nWidth, int& nHeight);
	SIZE GetMinSize();
	

	CContainerUI* GetRoot();
	CControlUI * GetItem(LPCTSTR lpszName);
	

	int SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
	bool KillTimer(CControlUI* pControl, UINT nTimerID);

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue); 
	virtual LPCTSTR GetAttribute(LPCTSTR lpszName);

	void SetAlpha(int nAlpha = 255);

	bool SendNotify(CControlUI* pControl, short sType, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsyn = false);
	void RegisterDraw(ISkinDrawUI* pDraw);

	void AddNotifier(INotifyUI* pNotifyUI);
	void RemoveNotifier(INotifyUI* pNotifyUI);  

	void AddMessageFilter(IMessageFilterUI* pFilter);
	void RemoveMessageFilter(IMessageFilterUI* pFilter);

	CControlUI* GetFocus();
	virtual void SetFocusWnd(HWND hwnd);
	void SetFocus(CControlUI* pControl);
	void KillFocus(CControlUI* pControl);

	void SetCapture();
	void ReleaseCapture();
	bool IsCaptured();

	virtual void SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	void Invalidate(RECT* lpRect = NULL, bool bUpdate = false);

	HDC GetPaintDC();
	HDC GetMemDC();

	CControlUI * GetEventClickControl();

	
	void SetLayerWindow(bool bLayer, bool bRedraw = true);
	bool IsLayerWindow();

	void SetAnimating(bool bAnimating);
	bool IsAnimating();

	void SetLayerInset(RECT& rect);
	void SetBorder(RECT& rect);
	

	void SetOkControl(CControlUI* pControl);
	void EnableChangeSkin(bool bEnalbe);
	bool IsEnableChangeSkin();
	
	
	CControlUI* FindControl(POINT& pt);
	CControlUI* FindControl(POINT& pt, UINT uFlags);
	void SetEnableFocus(bool bEnableFocus);
	bool GetEnableFocus();

	void HideToolTips();

	void SetEnableDragDrop(bool bDragDrop);
	HRESULT DoDrag(CControlUI* pControl, IN LPDATAOBJECT pDataObj,  IN DWORD dwOKEffects, OUT LPDWORD pdwEffect);

	virtual void SetBkColor(DWORD dwColor);
	virtual void SetBkImage(LPCTSTR lpszFileName, int nStretchMode, bool bAnimation = false );
	void SetDefaultSkin();
	void SetDefaultColor();

	void SetMsgHandled(bool bMsgHanled);
public:
	virtual void OnCreate();
	virtual void OnInitShow();
	virtual bool OnEscape();
	virtual void OnMinimize();
	virtual void OnMaximize();
	virtual void OnRestored();
	virtual void OnSetWindowRgn();
	virtual void OnLanguageChange();
	virtual void Notify(TNotifyUI* pNotify);
	virtual bool PreMessageHandler(const LPMSG pMsg, LRESULT& lRes); //��Ϣ
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnShowFristSide();
	virtual void OnShowSecondSide();
	virtual void EnableIME();
	virtual void DisableIME();
	bool IsCreated(){return m_bCreated;}
protected:
	bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	virtual void OnActiveApp(UINT message, WPARAM wParam, LPARAM lParam);
	bool SetNextTabControl(bool bForward = true);
	
	void ReapObjects(CControlUI* pControl); //�ؼ�pControlɾ��ʱ�����д����������

	void FlushToBackBuffer(RECT rect, RECT rcClient, DibObj* pDibObj);
	void FlushToBackBuffer(RECT rect, RECT rcClient);
	void FlushToForeBuffer(RECT rect);
	LRESULT OnHitTest(WPARAM   wParam,   LPARAM   lParam);

	void SkinChangeAnimation();

	int m_iSkinAnimationIndex;
	LPCTSTR m_lpszSkinFileName;
	int m_nStretchMode;

protected:
	HDC m_hDcPaint;
	IRenderDC* m_pRenderDC;
	DibObj* m_pDibObj;
	CContainerUI* m_pRoot;
	CControlUI* m_pFocus;
	CControlUI* m_pEventHover;
	CControlUI* m_pEventClick;
	CControlUI* m_pEventKey;
	CControlUI* m_pOk;
	CStdPtrArray m_aMessageFilters;

	POINT m_ptLastMousePos, m_ptLastMousePosClick;
	bool m_bMouseTracking;
	bool m_bMouseCapture;
	bool m_bFocusNeeded;
	bool m_bUpdateNeeded;
	bool m_bSizeChanged;

	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;

	UINT m_uTimerID;
	CStdPtrArray m_aTimers;
    CStdPtrArray m_aNotifiers;
	bool m_bFirstLayout;
	ISkinDrawUI* m_pISkinDraw;

	int m_iMinWidth,m_iMinHeight,m_iMaxWidth,m_iMaxHeight,  m_iOrgWidth, m_iOrgHeight;
	tstring m_strFocusControl, m_strOkControl, m_strCancelControl; //��һ�δ�ʱ��궨λ�İ�ť��OK,Cancel��ť

	tstring m_strId, m_strSubId;
	tstring m_strTitle, m_strSmallIcon, m_strBigIcon;
	LPTSTR  m_lpszTitleKey;
	//CSkinWindow������

	bool m_bMaximized;
	bool m_bMinimized;
	RECT m_rcRestore;

	int  m_nRoundRgn;

	bool m_bModalDlg;    //�Ƿ���ģʽ����
	bool m_bEnableResize;   //�Ƿ��ܹ��ı��С
	bool m_bEnableMove;     //�Ƿ��ܹ��ƶ�λ��
	bool m_bLayerWindow; //�Ƿ��ǲ������
	int  m_nAlpha;
	//for background
	tstring m_strImageFileDef;     //Ĭ�ϵı���ͼƬ
	DWORD   m_dwBkColorDef;       //Ĭ�ϵı�����ɫ
	HICON   m_hIcon, m_hBigIcon; //����

	bool    m_bAinmating;
	RECT    m_rcLayerInset;
	RECT    m_rcBorder;
	bool    m_bEnableChangeSkin;
	Animation* m_pCloseAnimation;
	bool	  m_bEnableFocus;    //  add by zhangxin  at 2011.8.31

	//�����ر����뷨
	HIMC   m_hImc;

	bool m_bCreated;//�Ƿ���ù�oncreate
	bool m_bNotifyAfterCreated;//�Ƿ���create֮����������notify

	//������������ʾ��������

	bool  m_bFlashWindowActive;
	uint8 m_uFlashWindowInvert;

	IDropTarget* m_pDragTarget;
	bool m_bDraging;
	bool m_bEnableDragDrop;
	CControlUI* m_pEventDrag;
	bool m_bMsgHandled;
	RECT m_rectWork; // ������С������� rcwork�᲻׼ȷ����˱���ʹ����С��֮ǰ������

};

//////////////////////////////////////////////////////////////////////////
// NoActiveWindow : �Ǽ���
class  DUI_API CNoActiveWindowUI : public CWindowUI
{
public:
	CNoActiveWindowUI();
	virtual ~CNoActiveWindowUI();
	virtual void OnSetWindowRgn(){};
	virtual bool HideWindow();
	virtual bool ShowWindow(int nCmdShow = SW_SHOW);
	virtual bool ShowWindow(Animation* pAnimation);
	virtual void CenterWindow(HWND hAlternateOwner = NULL, bool bOverlapped = false, int cx = 0, int cy = 0);
	virtual void SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LPCTSTR GetWindowClassName();
};

#endif // _UIDIALOG_H





















