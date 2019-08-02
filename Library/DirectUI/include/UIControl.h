/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
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
// 控件基本信息
protected:
	UINT m_uControlFlags;            //控件对于所属窗口的类型
public:
	UINT GetControlFlags();
	void ModifyControlFlags(UINT uAddFlags, UINT uDelFlags);

protected:
	tstring m_strId;                    //控件ID
public:
	//************************************
	// @brief:    设置ID
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   void 
	//************************************
	void SetId(LPCTSTR lpszId);

	//************************************
	// @brief:    获取ID
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetId();

protected:
	tstring m_strType;                  //控件类型，若无特殊样式，则样式与类型同名
public:
	//************************************
	// @brief:    设置控件创建类型
	// @note:     该类型名与xml配置中标签名一致
	// @param: 	  LPCTSTR lpszType 
	// @return:   void 
	//************************************
	void SetType(LPCTSTR lpszType);

	//************************************
	// @brief:    获取创建类型
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetType();

	//************************************
	// @brief:    判断创建类型
	// @note:     
	// @param: 	  LPCTSTR lpszType 
	// @return:   bool 
	//************************************
	bool TypeOf(LPCTSTR lpszType);

protected:
	CStdPtrArray* m_pStyleList;         //样式列表
	tstring m_strStyleName;				//样式名称
public:
	//************************************
	// @brief:    设置样式类型
	// @note:     该样式需有配置过，否则失败
	// @param: 	  LPCTSTR lpszName 
	// @return:   bool 设置失败
	//************************************
	bool SetStyle(LPCTSTR lpszName);

	//************************************
	// @brief:    获取样式数组
	// @note:     
	// @return:   CStdPtrArray* 指针为空时即没有设置样式
	//************************************
	CStdPtrArray* GetStyle();

	//************************************
	// @brief:    依据ID获取样式
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   StyleObj* 
	//************************************
	StyleObj* GetStyle(LPCTSTR lpszId);

	//************************************
	// @brief:    依据索引获取样式
	// @note:     
	// @param: 	  int nIndex 
	// @return:   StyleObj* 
	//************************************
	StyleObj* GetStyle(int nIndex);

	//************************************
	// @brief:    添加样式
	// @note:     需要确定该控件最多拥有多少个样式
	// @param: 	  StyleObj * pStyle 
	// @param: 	  int nIndex 目标索引
	// @param: 	  int nMax 
	// @return:   void 
	//************************************
	void AddStyle(StyleObj* pStyle, int nIndex, int nMax);

	//************************************
	// @brief:    获取样式个数
	// @note:     
	// @return:   int 
	//************************************
	int GetStyleCount();

	//************************************
	// @brief:    依据ID获取图片样式
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   ImageStyle* 
	//************************************
	ImageStyle* GetImageStyle(LPCTSTR lpszId);

	//************************************
	// @brief:    依据ID获取文本样式
	// @note:     
	// @param: 	  LPCTSTR lpszId 
	// @return:   TextStyle* 
	//************************************
	TextStyle* GetTextStyle(LPCTSTR lpszId);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 控件文字信息
protected:
	tstring m_strText;                  //控件基本文字信息
	LPTSTR m_lpszTextKey;
public:
	//************************************
	// @brief:    获取文本
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	virtual LPCTSTR GetText();

	//************************************
	// @brief:    设置文本
	// @note:     
	// @param: 	  LPCTSTR lpszText 
	// @return:   void 
	//************************************
	virtual void SetText(LPCTSTR lpszText);

	//************************************
	// @brief:    获取文字对象
	// @note:     该对象属于全局对象，谨慎修改
	// @return:   FontObj* 
	//************************************
	virtual FontObj* GetFontObj();

	//************************************
	// @brief:    设置提示信息
	// @note:     
	// @param: 	  LPCTSTR lpszTips 
	// @return:   void 
	//************************************
	void SetToolTip(LPCTSTR lpszTips);

	//************************************
	// @brief:    获取提示信息
	// @note:     
	// @return:   LPCTSTR 
	//************************************
	LPCTSTR GetToolTip();

	//************************************
	// @brief:    是否显示提示信息
	// @note:     
	// @return:   bool 
	//************************************
	virtual bool IsToolTipShow();

	//************************************
	// @brief:    设置是否显示提示信息
	// @note:     
	// @param: 	  bool bShow 
	// @return:   void 
	//************************************
	void SetToolTipShow(bool bShow);
protected:
	tstring m_strToolTips;              //提示框文字信息
	LPTSTR m_lpszToolTipsKey;
	bool m_bShowToolTips;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 控件位置信息
protected:
	uint8 m_iAnchor;                  //描点，目前0～9，共10种
	RECT m_rcCfg;                    //xml中配置信息，与描点配置配合使用
public:
	//************************************
	// @brief:    设置相对父容器的描点位置
	// @note:     
	// @param: 	  int l left
	// @param: 	  int t top
	// @param: 	  int r right
	// @param: 	  int b bottom
	// @param: 	  int a 描点方式
	// @return:   void 
	//************************************
	void SetInitPos(int l,int t,int r,int b, int a = -1);

	//************************************
	// @brief:    获取相对父容器的描点位置
	// @note:     
	// @param: 	  int & l 输出left
	// @param: 	  int & t 输出top
	// @param: 	  int & r 输出right
	// @param: 	  int & b 输出bottom
	// @return:   void 
	//************************************
	void GetInitPos(int& l,int& t,int& r,int& b);

	//************************************
	// @brief:    获取相对父容器的描点方式
	// @note:     
	// @return:   int 描点方式
	//************************************
	int GetAnchor();

protected:
	RECT m_rcInset;                  //控件边沿留下的空隙
public:
	//************************************
	// @brief:    设置客户区与窗口区的间隔
	// @note:     
	// @param: 	  RECT & rect 
	// @return:   void 
	//************************************
	void SetInset(RECT& rect);

	//************************************
	// @brief:    获取客户区与窗口区的间隔
	// @note:     
	// @return:   RECT 
	//************************************
	RECT GetInset();

protected:
	RECT m_rcItem;                   //控件绘制区域
public:
	//************************************
	// @brief:    设置控件在所属窗口中的位置
	// @note:     
	// @param: 	  RECT & rectRegion 
	// @return:   void 
	//************************************
	virtual void SetRect(RECT& rectRegion);

	//************************************
	// @brief:    设置控件在所属窗口中的位置
	// @note:     用于性能优化，当控件置空时，样式不需要设置位置
	// @param: 	  RECT & rectRegion 
	// @param: 	  bool bStyle 是否设置样式位置
	// @param: 	  bool bRTL 用于阿拉伯版本
	// @return:   void 
	//************************************
	virtual void SetRect(RECT& rectRegion, bool bStyle, bool bRTL = false);

	//************************************
	// @brief:    获取控件在所属窗口中的位置
	// @note:     
	// @return:   RECT 
	//************************************
	virtual RECT GetRect();

	//************************************
	// @brief:    获取客户区大小
	// @note:     容器控件使用
	// @return:   RECT 
	//************************************
	virtual RECT GetClientRect();

	//************************************
	// @brief:    获取客户区大小
	// @note:     
	// @return:   RECT 
	//************************************
	virtual RECT GetInterRect();

	//************************************
	// @brief:    将控件移动至指定位置
	// @note:     
	// @param: 	  RECT & rc 
	// @param: 	  bool bRedraw 是否强制刷新
	// @return:   void 
	//************************************
	void MoveTo(RECT& rc, bool bRedraw = true);

public:
	//************************************
	// @brief:    
	// @note:     用于阿拉伯版本
	// @return:   void 
	//************************************
	inline void FlipRect();

	//************************************
	// @brief:    
	// @note:     用于阿拉伯版本
	// @return:   void 
	//************************************
	virtual inline void FlipClientRect();
	bool m_bFlipRect;                //用于阿拉伯版本
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 控件控制功能
protected:
	bool m_bFocused;                 //是否焦点控件
public:
	//************************************
	// @brief:    是否焦点控件
	// @note:     
	// @return:   bool 
	//************************************
	bool IsFocused();

	//************************************
	// @brief:    设置为焦点控件
	// @note:     
	// @return:   void 
	//************************************
	void SetFocus();

	//************************************
	// @brief:    使控件失去焦点
	// @note:     
	// @return:   void 
	//************************************
	void KillFocus();

protected:
	bool m_bEnable;                  //是否开启控件事件响应
public:
	//************************************
	// @brief:    是否开启控件事件响应
	// @note:     控件disable绘制等
	// @return:   bool 
	//************************************
	bool IsEnabled();

	//************************************
	// @brief:    是否开启控件事件响应
	// @note:     
	// @param: 	  bool bEnable 
	// @return:   void 
	//************************************
	void SetEnabled(bool bEnable);

protected:
	bool m_bMouseEnable;             //Control默认不响应鼠标移动事件, 设置这个后可以响应鼠标事件
public:
	//************************************
	// @brief:    是否开启鼠标响应、tooltip显示
	// @note:     
	// @return:   bool 
	//************************************
	bool IsMouseEnabled();

	//************************************
	// @brief:    是否开启鼠标响应、tooltip显示
	// @note:     
	// @param: 	  bool bEnable 
	// @return:   void 
	//************************************
	void SetMouseEnabled(bool bEnable);

protected:
	int m_nIndex;                   //在父容器中的序号
public:
	int	GetIndex();
	void SetIndex(int nIndex);

protected:
	bool m_bWrapSize;                //控件是否依据容器大小调整自身大小
public:
	bool IsWrapSize();
	void SetWrapSize(bool bWrapSize);

protected:
	uint8 m_nAlpha;                   //控件透明度
public:
	uint8 GetAlpha();
	void SetAlpha(uint8 nAlpha);

protected:
	UINT m_uUserData;                //预留给用户存储
public:
	UINT GetUserData();
	void SetUserData(UINT uData);

protected:
	bool m_bNeedScroll;              //是否依父容器的游标显示位置
public:
	bool IsScroll();

protected:
	bool m_bSelect;                  //是否被选中
public:
	virtual bool IsSelect();
	virtual void Select(bool bSelect = true, bool bRedraw = true);

protected:
	bool m_bVisible;                 //控件是否显示，重置该值时需刷新该控件的位置信息
public:
	virtual bool IsVisible();
	virtual void SetVisible(bool bShow, bool bRedraw = true);

protected:
	bool m_bInterVisible;            //控件是否显示，重置该值时需刷新该控件的位置信息
public:
	virtual bool IsInternVisible();
	virtual void SetInternVisible(bool bVisible);

public:
	virtual CControlUI*	GetOwner();
	virtual void SetOwner(CControlUI* pOwner);

protected:
	UITYPE_STATE m_nState;                   //控件当前绘制状态，与style配合使用
public:
	void SetState(UITYPE_STATE nState);
	UITYPE_STATE GetState();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 控件被管理信息
protected:
	CControlUI* m_pParent;              //父容器
	CWindowUI* m_pManager;              //所属窗口
public:
	virtual void SetManager(CWindowUI* pManager, CControlUI* pParent);
	CWindowUI* GetWindow();
	CControlUI* GetParent();

protected:
	INotifyUI* m_pINotifyUI;            //事件过滤
public:
	INotifyUI* GetINotifyUI();
	virtual void SetINotifyUI(INotifyUI* pINotify);

protected:
	CEventDelegate* m_pEventDelegateList;                   //事件预处理
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
// 通用功能
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
	virtual CControlUI* GetItem(int iIndex);//modify by hanzp. 与Container的接口保持一致
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
	UINT    m_uCursor;                  //游标类型，定义参见uidef.h，DUI_IDC_ARROW
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
