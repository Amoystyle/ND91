#ifndef _UIDEF_H
#define _UIDEF_H

#ifdef DIRECTUI_LAYOUT_RTL
	#define DT_LEFT_EX DT_RIGHT
	#define DT_RIGHT_EX DT_LEFT
#else
	#define DT_LEFT_EX DT_LEFT
	#define DT_RIGHT_EX DT_RIGHT
#endif	

#ifndef tstring
#ifdef UNICODE	
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#endif

#ifndef uint8
typedef unsigned char uint8;
#endif

#ifndef int8
typedef signed char int8;
#endif


class CControlUI;
class IRenderDC;
class ImageObj;
class FontObj;
//namespace{
typedef enum
{
	UINOTIFY__FIRST,
	// 皮肤事件
	UINOTIFY_SKINCHANGE,
	// 通用事件
	UINOTIFY_CLICK,
	UINOTIFY_CLICK_LINK,
	UINOTIFY_CLICK_CTRL,
	UINOTIFY_RCLICK,
	UINOTIFY_DBCLICK,
	UINOTIFY_SELCHANGE,
	UINOTIFY_SETFOCUS,
	UINOTIFY_KILLFOCUS,
	UINOTIFY_HOVER,
	UINOTIFY_TIMER,
	UINOTIFY_DROPFILES,
	// 编辑框事件
	UINOTIFY_EN_CHANGE,
	UINOTIFY_EN_IMESTART,
	UINOTIFY_EN_IMEEND,
	UINOTIFY_EN_IMEING,
	UINOTIFY_EN_LIMIT,
	UINOTIFY_EN_KEYDOWN,  // add by zhangxin 2011.9.7
	UINOTIFY_EN_LISTCLICK,
	UINOTIFY_EN_LISTRETURN,
	// 树形控件事件
	UINOTIFY_TVN_ITEMEXPENDED,
	UINOTIFY_TVN_ITEMEXPENDING,
	UINOTIFY_TVN_BEGINDRAG,
	UINOTIFY_TVN_ENDDRAG,
	UINOTIFY_TVN_CHECK,
	// 列表控件
	UINOTIFY_LB_HLCHANGE,
	// Slider事件
	UINOTIFY_TRBN_POSCHANGE,
	UINOTIFY_TRBN_SELCHANGE,
	UINOTIFY_TRBN_BEGINCHANGE,
	UINOTIFY_TRBN_ENDCHANGE,
	// richedit控件事件 
	UINOTIFY_RE_DBCLICK_IMAGE,
	UINOTIFY_RE_CLICK_IMAGE_THUMBNAIL_BTN,
	UINOTIFY_RE_CLICK_LINK,
	// ActiveX控件
	UINOTIFY_ACTIVEX_CREATE,
	UINOTIFY_ACTIVEX_SHOWCOMPLETE,
	UINOTIFY_ACTIVEX_NAVIGATE_BEFORE,
	UINOTIFY_ACTIVEX_NAVIGATE_AFTER,
	UINOTIFY_ACTIVEX_LODING,
	UINOTIFY_ACTIVEX_LODE_DONE,
	UINOTIFY_ACTIVEX_LOAD_ERROR,
	UINOTIFY_ACTIVEX_CLOSE,
	// Animation
	UINOTIFY_ANIMATION_END,
	// ComboBox
	UINOTIFY_CBN_BEFOREDROPDOWN,
	UINOTIFY_CBN_DROPDOWN,
	UINOTIFY_CBN_CHECK,
	// Tab
	UINOTIFY_TAB_ADD,
	UINOTIFY_TAB_CLOSE,
	UINOTIFY_TAB_DBCLICK,
	//热键
	UINOTIFY_HOTKEY, 
	
	// 快捷菜单
	UINOTIFY_GET_CONTEXTMENU,

	// 拖拉事件
	UINOTIFY_DRAG_ENTER,
	UINOTIFY_DROP,

	UINOTIFY_BEGIN_DRAG,
	UINOTIFY_DRAG_QUERYDATA,

	UINOTIFY__LAST = UINOTIFY__FIRST + 1000,
	UINOTIFY__USER,	/// 其他自定义控件消息的起点.
	UINOTIFY__USER_LAST = UINOTIFY__USER + 1000,
	// 皮肤事件

}UITYPE_NOTIFY;

typedef enum
{
	UISTATE_NORMAL=0, //uncheched, normal 
	UISTATE_OVER,     //uncheched, the mouse is hover
	UISTATE_DOWN,    //uncheched, the left button of mouse is press 
	UISTATE_FOCUS,    //unchecked, the control get the focus of keyboard
	UISTATE_DISABLE,//unchecked, disable 

	UISTATE_CKNORMAL = 5, //checked, normal 
	UISTATE_CKOVER,//checked, mouse is hover 
	UISTATE_CKDOWN,//checked, the left button of mouse is press 
	UISTATE_CKFOCUS,//checked, the control get the focus of keyboard 
	UISTATE_CKDISABLE,//checked, disable 
}UITYPE_STATE;


// 字体效果
typedef enum
{
	UIFONT_NORMAL = 0,
	UIFONT_GLOW,       //发光特效  
	UIFONT_GDIPLUS   //使用GDI+
} UITYPE_FONT;


// 图片拉伸类型
typedef enum
{
	UIPAINT_NORMAL = 0,
	UIPAINT_STRETCH,   
	UIPAINT_CENTER
} UITYPE_PAINT;

//样式类型
typedef enum
{
	UISTYLE_ANY = 0,
	UISTYLE_IMAGE, // 图像
	UISTYLE_TEXT,      //文字
	UISTYLE_FILL,      //填充
	UISTYLE_LINE,      //线
} UITYPE_STYLE;


typedef enum
{
UIEVENT__FIRST = 0,
UIEVENT__MOUSEBEGIN,
	UIEVENT_MOUSEMOVE,
	UIEVENT_MOUSELEAVE,
	UIEVENT_MOUSEENTER,
	UIEVENT_MOUSEHOVER,
	UIEVENT_BUTTONDOWN,
	UIEVENT_BUTTONUP,
	UIEVENT_LDBCLICK,
	UIEVENT_RBUTTONDOWN,
	UIEVENT_RBUTTONUP,
	UIEVENT_RDBCLICK,
	UIEVENT_SCROLLWHEEL,
	UIEVENT_SCROLLWHEELFOCUS,//当有滚轮事件时，发送至当前焦点控件
UIEVENT__MOUSEEND,
UIEVENT__KEYBEGIN,
	UIEVENT_KEYDOWN,
	UIEVENT_KEYUP,
	UIEVENT_CHAR,
	UIEVENT_IME_CHAR,
	UIEVENT_IME_START,
	UIEVENT_IME_END,
	UIEVENT_IME_ING,
	UIEVENT_SYSKEY,
 UIEVENT__KEYEND,
	UIEVENT_KILLFOCUS,
	UIEVENT_SETFOCUS,
	UIEVENT_CONTEXTMENU,
	UIEVENT_VSCROLL,
	UIEVENT_HSCROLL,
	UIEVENT_WINDOWSIZE,
	UIEVENT_SETCURSOR,
	UIEVENT_MEASUREITEM,
	UIEVENT_DRAWITEM,
	UIEVENT_TIMER,
	UIEVENT_COMMAND,
	UIEVENT_SELECT,
	UIEVENT_DROPFILES,
	UIEVENT_MENUITEM_CLICK,
	// 拖放
	UIEVENT_DRAG_ENTER,
	UIEVENT_DRAG_OVER,
	UIEVENT_DRAG_LEAVE,
	UIEVENT_DROP,

	UIEVENT_BEGIN_DRAG,
	UIEVENT_DRAG_SETDATA_DELAY,
UIEVENT__LAST,
	UIEVENT_UNUSED,
}UITYPE_EVENT;


#define ID_MSGBOX_BTN  50


// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004
#define UIFLAG_SETFOCUS      0x00000008 //能够设置光标
#define UIFLAG_SETFOCUS_AWLAYS      0x00000010 
#define UIFLAG_ENABLE_DROP      0x00000020 
#define UIFLAG_ENABLE_DRAG      0x00000040

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_MOUSE_ENABLED 0x00000010
#define UIFIND_MOUSE		 0x00000020 //add by hanzp
#define UIFIND_CLICK         0x00020000 //add by hanzp 用于区分点击事件
#define UIFIND_ME_FIRST      0x80000000

//光标键值
#define DUI_IDC_ARROW           (32512)
#define DUI_IDC_IBEAM           (32513)
#define DUI_IDC_WAIT            (32514)
#define DUI_IDC_CROSS           (32515)
#define DUI_IDC_UPARROW         (32516)
#define DUI_IDC_SIZE            (32640)  /* OBSOLETE: use IDC_SIZEALL */
#define DUI_IDC_ICON            (32641)  /* OBSOLETE: use IDC_ARROW */
#define DUI_IDC_SIZENWSE        (32642)
#define DUI_IDC_SIZENESW        (32643)
#define DUI_IDC_SIZEWE          (32644)
#define DUI_IDC_SIZENS          (32645)
#define DUI_IDC_SIZEALL         (32646)
#define DUI_IDC_NO              (32648) /*not in win3.1 */
#if(WINVER >= 0x0500)
#define DUI_IDC_HAND            (32649)
#endif /* WINVER >= 0x0500 */
#define DUI_IDC_APPSTARTING     (32650) /*not in win3.1 */
#if(WINVER >= 0x0400)
#define DUI_IDC_HELP            (32651)
#endif /* WINVER >= 0x0400 */

//时钟ID
#define DUI_TIMERID_BASE 0x1000
#define DUI_TIMERID_USER 0x2000

//布局类型
typedef enum
{
	UILAYOUT_NORMAL=0,// Does not layout the children controls.  
	UILAYOUT_LEFTTORIGHT,// Sets the default horizontal layout to be left to right. 
	UILAYOUT_RIGHTTOLEFT ,//Sets the default horizontal layout to be right to left. 
	UILAYOUT_TOPTOBOTTOM ,//Sets the default vertical layout to be top to bottom. 
	UILAYOUT_BOTTOMTOTOP ,//Sets the default vertical layout to be bottom to top. 
	UILAYOUT_HORZFILL ,//Sets the default horizontal layout to fill the layout with the child controls.  
	UILAYOUT_VERTFILL ,//Sets the default vertical layout to fill the layout with the child controls. 
	UILAYOUT_GRID ,//Layouts the child controls like the grids in a table and goes to newline automatically. 
	UILAYOUT_LEFTTORIGHT_EXTEND,// Sets the default horizontal layout to be left to right. 
}UITYPE_LAYOUT;


/////////////////////////////////////////////////////////////////////////////////////
//定时器结构
typedef struct tagTIMERINFO
{
	CControlUI* pSender;
	UINT nLocalID;
	HWND hWnd;
	UINT uWinTimer;
} TIMERINFO;

//对内事件通知结构
typedef struct tagTEventUI
{
	int Type;
	CControlUI* pSender;
	POINT ptMouse;
	WCHAR chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
} TEventUI;

//对外事件通知结构
typedef struct tagTNotifyUI 
{
	short sType;
	CControlUI* pSender;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} TNotifyUI;


typedef struct tagTDragDropNotifyUI
{
	IDataObject * pDataObject;
	DWORD grfKeyState;
	POINTL pt;
	DWORD * pdwEffect;
	bool  bFiles;

} TDragDropNotifyUI;


class DUI_API UIMENUINFO;

#define BY_COMMAND 			0			/**< 插入菜单位置指定方式为菜单项ID */
#define BY_POSITION 		1			/**< 插入菜单位置指定方式为菜单序号 */

typedef CControlUI* (_cdecl *CONTROLCREATEPROC)();
typedef bool (WINAPI *PRETRANSLAGEMESSAGEPROC)(const LPMSG lpMsg);
typedef std::map<tstring, tstring> MAPAttr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局接口
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class INotifyUI
{
public:
	virtual bool ProcessNotify(TNotifyUI* pNotify) = 0;
};

//消息过滤
class IMessageFilterUI
{
public:
	virtual LRESULT MessageFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

class ISkinDrawUI
{
public:
	virtual  void InitDraw()=0;
	virtual  void DrawBefore(IRenderDC* pRenderDC, RECT& rc, RECT& rcPaint)=0;
	virtual  void DrawAfter(IRenderDC* pRenderDC, RECT& rc, RECT& rcPaint)=0;
};

class IUIRes
{
public:
	// 设置语言
	virtual void SetAutoMultiLanguage(bool bAutoMultiLanguage) = 0;
	virtual bool IsAutoMultiLanguage() = 0;

	virtual void SetLanguage(LPCTSTR lpszLanguage, bool bReloadRes = false) = 0;

	// 设置默认字体
	virtual void SetDefaultFontName(LPCTSTR lpszFontName) = 0;
	virtual LPCTSTR GetDefaultFontName() = 0;

	// 初始化目录
	virtual void InitResDir(LPCTSTR lpszPath, bool bDefaultDir = false) = 0;
	virtual void InitResDir(UINT uResID, LPCTSTR lpszType) = 0;
	// 临时使用，用于独立exe的窗口加载
	virtual bool LoadSkin(UINT uResID, LPCTSTR lpszType, LPCTSTR lpszName = NULL, UINT uResType = 0x0000FFFF) = 0;

	virtual ImageObj*  LoadImage(LPCTSTR lpszPath, bool bCache = true) = 0;
	virtual ImageObj*  LoadImage(UINT uResID, LPCTSTR lpszType) = 0;
	virtual HICON LoadIcon(LPCTSTR lpszPath) = 0;
	virtual HICON LoadIcon(LPVOID lpData, long nSize) = 0;
	virtual HICON LoadIcon(UINT uResID, LPCTSTR lpszType) = 0;
	virtual UIMENUINFO GetMenu( UINT uMenuID, int nIndex = -1, UINT uFlag = BY_POSITION ) = 0;
	virtual void AdjustHSL(int nHue, int nSaturation, int lightness) = 0;
	virtual LPCTSTR GetImageFileFullPath(LPCTSTR lpszPath) = 0;
	virtual void GetTemplateObjAttr(LPCTSTR lpszTemplateID, MAPAttr & mapattr) = 0;
	virtual void  AddFont(FontObj* pFontObj) = 0;
	virtual FontObj* GetFont(int index) = 0;
	virtual int GetFontIndex(LPCTSTR lpszId) = 0;
};

class IUIEngine
{
public:
	virtual void InitSkin() = 0;
	virtual void UnInitSkin() = 0;
	virtual void SetInstanceHandle(HINSTANCE hInstance) = 0;
	virtual HINSTANCE GetInstanceHandle()= 0;
	virtual void GetOSVersion(DWORD& dwOSVer, DWORD& dwOSMinorVer) = 0;
	virtual tstring  GetRunDir() = 0;

	virtual int GetWindowCount() = 0;
	virtual CWindowUI* GetWindow(int nIndex) = 0;
	virtual CWindowUI* GetWindow(HWND hWnd) = 0;
	virtual CWindowUI* GetWindow(LPCTSTR lpszId) = 0;
	virtual CWindowUI* GetWindow(LPCTSTR lpszId, HWND hWndParent) = 0;
	virtual CWindowUI* GetWindow(LPCTSTR lpszId, LPCTSTR lpszSubId) = 0;

	virtual void RegisterControl(LPCTSTR lpszType, CONTROLCREATEPROC lfCreateFn) = 0;
	virtual bool AddControl(CControlUI* pParent, CControlUI* pNext, LPCTSTR lpszId) = 0;

	virtual void SetDefSkinImage() = 0;
	virtual void SetSkinImage(LPCTSTR lpszPath, int nStretchMode, bool bAnimation = true) = 0;
	virtual LPCTSTR GetSkinImage() = 0;

	virtual bool  PumpMessage() = 0;
	virtual int  MessageLoop() = 0;
	virtual void SetPreTranslateMessage(PRETRANSLAGEMESSAGEPROC pfn) = 0;
	virtual bool TranslateMessage(const LPMSG pMsg) = 0;


	virtual void SetAlpha(int nAlpha) = 0;
	virtual int GetAlpha() = 0;

	virtual void AdjustHSL(COLORREF clr) = 0;
	virtual void AdjustHSL( double dHue, double dSaturation, double dLightness) = 0;
	virtual void GetAdjustHSL( double& dHue, double& dSaturation, double& dLightness) = 0;
	virtual COLORREF GetAdjustColor() = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局函数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
	DUI_API IUIRes*  GetUIRes();
	DUI_API IUIEngine*  GetUIEngine();
#ifndef DIRECTUI_STATICLIB
	inline DUI_API LPCTSTR I18NSTR(LPCTSTR lpszStr, bool* bFind = NULL);
	inline DUI_API void GetAnchorPos(uint8 nAnchor, const RECT* lprcParent, RECT* lprcChild);
	inline DUI_API bool RtlRect(RECT & rcParent, LPRECT pRectChild);
#else
	DUI_API LPCTSTR I18NSTR(LPCTSTR lpszStr, bool* bFind = NULL);
	DUI_API void GetAnchorPos(uint8 nAnchor, const RECT* lprcParent, RECT* lprcChild);
	DUI_API bool RtlRect(RECT & rcParent, LPRECT pRectChild);
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预定义和消息映射
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define UIBEGIN_MSG_MAP virtual bool ProcessNotify(TNotifyUI* pNotify) { 
#define SUPER_HANDLER_MSG if (__super::ProcessNotify(pNotify)) return true;
#define ID_HANDLER(id, func) if(pNotify->pSender && _tcsicmp(pNotify->pSender->GetId(), id) == 0) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define EVNET_HANDLER(event, func) \
	if(pNotify->sType == event) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define EVNET_ID_HANDLER(event, id, func) if(pNotify->sType == event && pNotify->pSender && _tcsicmp(pNotify->pSender->GetId(), id) == 0) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define EVNET_MENU_ID_HANDLER(event, id, func) if(pNotify->sType == event && pNotify->lParam && (UINT)pNotify->lParam == id) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define TYPE_HANDLER(type, func) if(pNotify->pSender && pNotify->pSender->TypeOf(type)) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define EVENT_TYPE_HANDLER(event, type, func) if(pNotify->sType == event && pNotify->pSender && pNotify->pSender->TypeOf(type)) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define TYPE_ID_HANDLER(type, id, func) if(pNotify->pSender && pNotify->pSender->TypeOf(type) && _tcsicmp(pNotify->pSender->GetId(), id) == 0) \
{ \
	if (func(pNotify)) \
	return true; \
}

#define UIEND_MSG_MAP return false;} 

/////////////////////////////////////////////////////////////////////////////////////////////////
//声明
#define UI_DECLARE_DYNCREATE() \
	static CControlUI* CreateObject()\

//实现
#define UI_IMPLEMENT_DYNCREATE(class_name)\
	CControlUI* class_name::CreateObject()\
{ return new class_name; }\

//注册
#define UI_REGISTER_DYNCREATE(text_name, class_name)\
	GetUIEngine()->RegisterControl(text_name, &class_name::CreateObject)\

/////////////////////////////////////////////////////////////////////////////////////////////////
#define  IDR_MENU_UIEDIT			9191900
#define  IDR_MENU_UIEDIT_CUT		9191901
#define  IDR_MENU_UIEDIT_COPY		9191902
#define  IDR_MENU_UIEDIT_PASTE		9191903
#define  IDR_MENU_UIEDIT_SELALL		9191904
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//} 
#endif // _UIDEF_H