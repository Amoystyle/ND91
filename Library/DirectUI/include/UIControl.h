/*********************************************************************
 *       Copyright (C) 2010,Ӧ���������
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-25       xqb
*********************************************************************/
#ifndef _UICONTROL_H
#define _UICONTROL_H

class CWindowUI;
class DUI_API CContainerUI;
typedef CControlUI* (__stdcall* FINDCONTROLPROC)(CControlUI*, LPVOID);
class DUI_API CControlUI
{
	friend class CContainerUI;
public:
	CControlUI();
	virtual ~CControlUI();
	static CControlUI* CreateObject();
//////////////////////////////////////////////////////////////////////////
// �ؼ�������Ϣ
protected:
	UINT m_uControlFlags;            //�ؼ������������ڵ�����
public:
	UINT GetControlFlags();
	void ModifyControlFlags(UINT uAddFlags, UINT uDelFlags);

protected:
	tstring m_strId;                    //�ؼ�ID
public:
	//************************************
	// @brief:    ����ID
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   void 
	//************************************
	void SetId(LPCTSTR lpszId);

	//************************************
	// @brief:    ��ȡID
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetId();

protected:
	tstring m_strType;                  //�ؼ����ͣ�����������ʽ������ʽ������ͬ��
public:
	//************************************
	// @brief:    ���ÿؼ���������
	// @note:     ����������xml�����б�ǩ��һ��
	// @param: 	  LPCTSTR lpszType 
	// @return:   void 
	//************************************
	void SetType(LPCTSTR lpszType);

	//************************************
	// @brief:    ��ȡ��������
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetType();

	//************************************
	// @brief:    �жϴ�������
	// @note:     
	// @param: 	  LPCTSTR lpszType 
	// @return:   bool 
	//************************************
	bool TypeOf(LPCTSTR lpszType);

protected:
	CStdPtrArray* m_pStyleList;         //��ʽ�б�
	tstring m_strStyleName;				//��ʽ����
public:
	//************************************
	// @brief:    ������ʽ����
	// @note:     ����ʽ�������ù�������ʧ��
	// @param: 	  LPCTSTR lpszName 
	// @return:   bool ����ʧ��
	//************************************
	bool SetStyle(LPCTSTR lpszName);

	//************************************
	// @brief:    ��ȡ��ʽ����
	// @note:     
	// @return:   CStdPtrArray* ָ��Ϊ��ʱ��û��������ʽ
	//************************************
	CStdPtrArray* GetStyle();

	//************************************
	// @brief:    ����ID��ȡ��ʽ
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   StyleObj* 
	//************************************
	StyleObj* GetStyle(LPCTSTR lpszId);

	//************************************
	// @brief:    ����������ȡ��ʽ
	// @note:     
	// @param: 	  int nIndex 
	// @return:   StyleObj* 
	//************************************
	StyleObj* GetStyle(int nIndex);

	//************************************
	// @brief:    �����ʽ
	// @note:     ��Ҫȷ���ÿؼ����ӵ�ж��ٸ���ʽ
	// @param: 	  StyleObj * pStyle 
	// @param: 	  int nIndex Ŀ������
	// @param: 	  int nMax 
	// @return:   void 
	//************************************
	void AddStyle(StyleObj* pStyle, int nIndex, int nMax);

	//************************************
	// @brief:    ��ȡ��ʽ����
	// @note:     
	// @return:   int 
	//************************************
	int GetStyleCount();

	//************************************
	// @brief:    ����ID��ȡͼƬ��ʽ
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   ImageStyle* 
	//************************************
	ImageStyle* GetImageStyle(LPCTSTR lpszId);

	//************************************
	// @brief:    ����ID��ȡ�ı���ʽ
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   TextStyle* 
	//************************************
	TextStyle* GetTextStyle(LPCTSTR lpszId);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �ؼ�������Ϣ
protected:
	tstring m_strText;                  //�ؼ�����������Ϣ
	LPTSTR m_lpszTextKey;
public:
	//************************************
	// @brief:    ��ȡ�ı�
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	virtual LPCTSTR GetText();

	//************************************
	// @brief:    �����ı�
	// @note:     
	// @param: 	  LPCTSTR lpszText 
	// @return:   void 
	//************************************
	virtual void SetText(LPCTSTR lpszText);

	//************************************
	// @brief:    ��ȡ���ֶ���
	// @note:     �ö�������ȫ�ֶ��󣬽����޸�
	// @return:   FontObj* 
	//************************************
	virtual FontObj* GetFontObj();

	//************************************
	// @brief:    ������ʾ��Ϣ
	// @note:     
	// @param: 	  LPCTSTR lpszTips 
	// @return:   void 
	//************************************
	void SetToolTip(LPCTSTR lpszTips);

	//************************************
	// @brief:    ��ȡ��ʾ��Ϣ
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetToolTip();

	//************************************
	// @brief:    �Ƿ���ʾ��ʾ��Ϣ
	// @note:     
	// @return:   bool 
	//************************************
	virtual bool IsToolTipShow();

	//************************************
	// @brief:    �����Ƿ���ʾ��ʾ��Ϣ
	// @note:     
	// @param: 	  bool bShow 
	// @return:   void 
	//************************************
	void SetToolTipShow(bool bShow);
protected:
	tstring m_strToolTips;              //��ʾ��������Ϣ
	LPTSTR m_lpszToolTipsKey;
	bool m_bShowToolTips;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �ؼ�λ����Ϣ
protected:
	uint8 m_iAnchor;                  //��㣬Ŀǰ0��9����10��
	RECT m_rcCfg;                    //xml��������Ϣ��������������ʹ��
public:
	//************************************
	// @brief:    ������Ը����������λ��
	// @note:     
	// @param: 	  int l left
	// @param: 	  int t top
	// @param: 	  int r right
	// @param: 	  int b bottom
	// @param: 	  int a ��㷽ʽ
	// @return:   void 
	//************************************
	void SetInitPos(int l,int t,int r,int b, int a = -1);

	//************************************
	// @brief:    ��ȡ��Ը����������λ��
	// @note:     
	// @param: 	  int & l ���left
	// @param: 	  int & t ���top
	// @param: 	  int & r ���right
	// @param: 	  int & b ���bottom
	// @return:   void 
	//************************************
	void GetInitPos(int& l,int& t,int& r,int& b);

	//************************************
	// @brief:    ��ȡ��Ը���������㷽ʽ
	// @note:     
	// @return:   int ��㷽ʽ
	//************************************
	int GetAnchor();

protected:
	RECT m_rcInset;                  //�ؼ��������µĿ�϶
public:
	//************************************
	// @brief:    ���ÿͻ����봰�����ļ��
	// @note:     
	// @param: 	  RECT & rect 
	// @return:   void 
	//************************************
	void SetInset(RECT& rect);

	//************************************
	// @brief:    ��ȡ�ͻ����봰�����ļ��
	// @note:     
	// @return:   RECT 
	//************************************
	RECT GetInset();

protected:
	RECT m_rcItem;                   //�ؼ���������
public:
	//************************************
	// @brief:    ���ÿؼ������������е�λ��
	// @note:     
	// @param: 	  RECT & rectRegion 
	// @return:   void 
	//************************************
	virtual void SetRect(RECT& rectRegion);

	//************************************
	// @brief:    ���ÿؼ������������е�λ��
	// @note:     ���������Ż������ؼ��ÿ�ʱ����ʽ����Ҫ����λ��
	// @param: 	  RECT & rectRegion 
	// @param: 	  bool bStyle �Ƿ�������ʽλ��
	// @param: 	  bool bRTL ���ڰ������汾
	// @return:   void 
	//************************************
	virtual void SetRect(RECT& rectRegion, bool bStyle, bool bRTL = false);

	//************************************
	// @brief:    ��ȡ�ؼ������������е�λ��
	// @note:     
	// @return:   RECT 
	//************************************
	virtual RECT GetRect();

	//************************************
	// @brief:    ��ȡ�ͻ�����С
	// @note:     �����ؼ�ʹ��
	// @return:   RECT 
	//************************************
	virtual RECT GetClientRect();

	//************************************
	// @brief:    ��ȡ�ͻ�����С
	// @note:     
	// @return:   RECT 
	//************************************
	virtual RECT GetInterRect();

	//************************************
	// @brief:    ���ؼ��ƶ���ָ��λ��
	// @note:     
	// @param: 	  RECT & rc 
	// @param: 	  bool bRedraw �Ƿ�ǿ��ˢ��
	// @return:   void 
	//************************************
	void MoveTo(RECT& rc, bool bRedraw = true);

public:
	//************************************
	// @brief:    
	// @note:     ���ڰ������汾
	// @return:   void 
	//************************************
	inline void FlipRect();

	//************************************
	// @brief:    
	// @note:     ���ڰ������汾
	// @return:   void 
	//************************************
	virtual inline void FlipClientRect();
	bool m_bFlipRect;                //���ڰ������汾
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �ؼ����ƹ���
protected:
	bool m_bFocused;                 //�Ƿ񽹵�ؼ�
public:
	//************************************
	// @brief:    �Ƿ񽹵�ؼ�
	// @note:     
	// @return:   bool 
	//************************************
	bool IsFocused();

	//************************************
	// @brief:    ����Ϊ����ؼ�
	// @note:     
	// @return:   void 
	//************************************
	void SetFocus();

	//************************************
	// @brief:    ʹ�ؼ�ʧȥ����
	// @note:     
	// @return:   void 
	//************************************
	void KillFocus();

protected:
	bool m_bEnable;                  //�Ƿ����ؼ��¼���Ӧ
public:
	//************************************
	// @brief:    �Ƿ����ؼ��¼���Ӧ
	// @note:     �ؼ�disable���Ƶ�
	// @return:   bool 
	//************************************
	bool IsEnabled();

	//************************************
	// @brief:    �Ƿ����ؼ��¼���Ӧ
	// @note:     
	// @param: 	  bool bEnable 
	// @return:   void 
	//************************************
	void SetEnabled(bool bEnable);

protected:
	bool m_bMouseEnable;             //ControlĬ�ϲ���Ӧ����ƶ��¼�, ��������������Ӧ����¼�
public:
	//************************************
	// @brief:    �Ƿ��������Ӧ��tooltip��ʾ
	// @note:     
	// @return:   bool 
	//************************************
	bool IsMouseEnabled();

	//************************************
	// @brief:    �Ƿ��������Ӧ��tooltip��ʾ
	// @note:     
	// @param: 	  bool bEnable 
	// @return:   void 
	//************************************
	void SetMouseEnabled(bool bEnable);

protected:
	int m_nIndex;                   //�ڸ������е����
public:
	int	GetIndex();
	void SetIndex(int nIndex);

protected:
	bool m_bWrapSize;                //�ؼ��Ƿ�����������С���������С
public:
	bool IsWrapSize();
	void SetWrapSize(bool bWrapSize);

protected:
	uint8 m_nAlpha;                   //�ؼ�͸����
public:
	uint8 GetAlpha();
	void SetAlpha(uint8 nAlpha);

protected:
	UINT m_uUserData;                //Ԥ�����û��洢
public:
	UINT GetUserData();
	void SetUserData(UINT uData);

protected:
	bool m_bNeedScroll;              //�Ƿ������������α���ʾλ��
public:
	bool IsScroll();

protected:
	bool m_bSelect;                  //�Ƿ�ѡ��
public:
	virtual bool IsSelect();
	virtual void Select(bool bSelect = true, bool bRedraw = true);

protected:
	bool m_bVisible;                 //�ؼ��Ƿ���ʾ�����ø�ֵʱ��ˢ�¸ÿؼ���λ����Ϣ
public:
	virtual bool IsVisible();
	virtual void SetVisible(bool bShow, bool bRedraw = true);

protected:
	bool m_bInterVisible;            //�ؼ��Ƿ���ʾ�����ø�ֵʱ��ˢ�¸ÿؼ���λ����Ϣ
public:
	virtual bool IsInternVisible();
	virtual void SetInternVisible(bool bVisible);

public:
	virtual CControlUI*	GetOwner();
	virtual void SetOwner(CControlUI* pOwner);

protected:
	UITYPE_STATE m_nState;                   //�ؼ���ǰ����״̬����style���ʹ��
public:
	void SetState(UITYPE_STATE nState);
	UITYPE_STATE GetState();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �ؼ���������Ϣ
protected:
	CControlUI* m_pParent;              //������
	CWindowUI* m_pManager;              //��������
public:
	virtual void SetManager(CWindowUI* pManager, CControlUI* pParent);
	CWindowUI* GetWindow();
	CControlUI* GetParent();

protected:
	INotifyUI* m_pINotifyUI;            //�¼�����
public:
	INotifyUI* GetINotifyUI();
	virtual void SetINotifyUI(INotifyUI* pINotify);

protected:
	CEventDelegate* m_pEventDelegateList;                   //�¼�Ԥ����
	CEventDelegate* GetEventDelegate();
public:
	bool AddEventDelegate(CDelegateBase* d);
	void RemoveEventDelegate(CDelegateBase* d = NULL);

protected:
	bool m_bDelegateChild;
public:
	bool IsDelegateChild();
	void SetDelegateChild(bool bDelegate);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ͨ�ù���
public:	
	virtual bool IsContainer();
	bool    SendNotify(short sType, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);
	void	Invoke(TEventUI& event);
	bool	HitTest(POINT point);
	virtual bool HitClientTest(POINT& point);
	virtual void Invalidate(bool bUpdate = false);
	virtual bool Resize();
	virtual int SetTimer(UINT nTimerID, UINT uElapse);
	virtual bool KillTimer(UINT nTimerID);
	virtual bool Add(CControlUI* pControl);
	virtual bool Insert(CControlUI* pControl, CControlUI* pNext);
	virtual bool Remove(CControlUI* pControl);
	virtual int GetCount();
	virtual CControlUI* GetItem(LPCTSTR lpszId);
	virtual CControlUI* GetItem(int iIndex);//modify by hanzp. ��Container�Ľӿڱ���һ��
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void OnSize(const RECT& rectRegion);
	virtual bool Event(TEventUI& event);
	virtual bool ChildEvent(TEventUI& event);
	virtual void Notify(TNotifyUI* msg);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	void RenderStyle(IRenderDC* pRenderDC, RECT& rcPaint, int nBegin = 0, int nEnd = -1);
	virtual void ReleaseImage();
	virtual bool Activate();
	virtual bool IsActivateByReturn();
	virtual void Init();
	virtual void OnDestory();
	virtual void OnLanguageChange();
	virtual void CalcControl(RECT & rectCalc);
protected:
	UINT    m_uCursor;                  //�α����ͣ�����μ�uidef.h��DUI_IDC_ARROW
//////////////////////////////////////////////////////////////////////////
public:
	LPCTSTR GetHotKey();
protected:
	tstring m_strHotKey;

public:
	static SIZE OnNormalRect(RECT& rcWinow, RECT& rcClient, CStdPtrArray& items, bool bEnableScroll = true);
	static void OnNormalRender(IRenderDC* pRenderDC, RECT& rcPaint, RECT& rcContainer, RECT& rcClient, CStdPtrArray& items, int nBegin = 0, int nEnd = -1);
	static bool OnNormalAdd(CControlUI* pControl, CControlUI* pParent, CStdPtrArray& items);
	static bool OnNormalInsert(CControlUI* pControl, CControlUI* pAfter, CControlUI* pParent, CStdPtrArray& items);
	static bool OnNormalRemove(CControlUI* pControl, CControlUI* pParent, CStdPtrArray& items);
	static void OnNormalRemoveAll(CStdPtrArray& items);
	static CControlUI* OnNormalFindControl(CStdPtrArray& items, FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags = UIFIND_ENABLED | UIFIND_MOUSE_ENABLED | UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
};

bool DUI_API Event(TEventUI& event, CControlUI* pControl);

#endif // _UICONTROL_H
