#pragma once

enum Messagebox_Button		//提示框按钮显示文字
{
	MY_MB_OKCANCEL,		//确定取消
	MY_MB_YESNO		//是否
};

enum Common_EXIST_ACTION
{
	Common_OVERWRITE,              // 覆盖
	Common_OVERWRITE_ALL,          // 覆盖所有
	Common_RENAME,                 // 重命名
	Common_RENAME_ALL,             // 重命名所有
	Common_RENAME_COPY,            // 重命名(文件名前‘复件’)
	Common_RENAME_COPY_ALL,        // 重命名所有(文件名前‘复件’)
	Common_CANCEL,                 // 取消
	Common_NONE,                   // 初值，无含义
};

struct CommonUIMsgStruct 
{
 	bool		return_bool;		//bool返回值
 	wstring		return_string;		//string返回值
	int			return_int;			//int返回值
	int			nRequestID;			//RequestID
};

class COMMONUI_API CommonUIFunc 
	: public BaseHandler<InputDialogMsg>
	, public BaseHandler<CommonUIReturnMsg>
	, public BaseHandler<NoticeDialogMsg>
	, public BaseHandler<MessageDialogMsg>
	, public BaseHandler<ConfirmDialogMsg>
	, public BaseHandler<GetExistActionMsg>
	, public BaseCSLock
{

private:
	static CommonUIFunc* _pInstance;   // 单例

	// 取得单例对象
	static CommonUIFunc* GetInstance();
public:
	// 释放单力对象
	static void Release();

public:

	static void RegeditControl();

	// 对话框：输入字符串
	static std::wstring InputDialog(wstring strMsg, wstring strTitle, wstring strDefault, bool& bCancel, int maxChar = 100,HWND hOwnerWnd=NULL);

	// 浮出提示对话框（自动消失）
	static void NoticeDialog(wstring strMsg, NotifyIcon strIcon = ICON_SUCCESS, int nDelayTime = 2000,HWND hOwnerWnd=NULL);

	// 消息对话框
	static void MessageDialog(wstring strMsg, wstring strTitle=L"", wstring strIcon=L"", wstring tip=L"", wstring okText=L"",HWND hOwnerWnd=NULL);

	// 确认对话框
	static bool ConfirmDialog(wstring strMsg, wstring strTitle=L"", wstring strIcon=L"", int height = 0,int flag = 0,bool isWarnning = false,HWND hOwnerWnd=NULL);

	// 确认对话框（警告型）
	static bool ConfirmWarnDialog(wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag,HWND hOwnerWnd=NULL);

	// 重名提示对话框
	static Common_EXIST_ACTION GetExistAction( std::wstring name, HWND hOwnerWnd=NULL);

	// 打开菜单
	// menuID : 菜单的ID
	// hWnd : 所属窗口
	// pNotify : 消息接收者，一般为调用者本身
	// pControl : 要左对齐的控件，为空则按鼠标位置显示
	// wParam/lParam:附加参数
	static void ShowMenu(UINT menuID, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);
	static void ShowMenu(CMenuUI *pMenu, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);

	// 取得字符串
	static const wchar_t* LoadString(const wchar_t* id) { return I18NSTR(id); }
private:
	CommonUIFunc();
	//所有方法具体实现
	std::wstring ShowInputDialog( wstring strMsg, wstring strTitle, wstring strDefault, bool& bCancel, int maxChar, HWND hOwnerWnd=NULL);

	void ShowNoticeDialog(wstring strMsg, wstring strIcon, int nDelayTime, HWND hOwnerWnd=NULL);

	void ShowMessageDialog(wstring strMsg, wstring strTitle, wstring strIcon, wstring tip, wstring okText=L"", HWND hOwnerWnd=NULL);

	bool ShowConfirmDialog(wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag,bool isWarnning = false, HWND hOwnerWnd=NULL);

	bool ShowConfirmWarnDialog(wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag, HWND hOwnerWnd=NULL);

	Common_EXIST_ACTION ShowGetExistAction( std::wstring name, HWND hOwnerWnd=NULL);

	void ShowShowMenu(UINT menuID, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);
	void ShowShowMenu(CMenuUI *pMenu, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);
public:
	virtual bool Handle(const InputDialogMsg* pMsg);

	virtual bool Handle(const CommonUIReturnMsg* pMsg);

	virtual bool Handle(const NoticeDialogMsg* pMsg);

	virtual bool Handle(const MessageDialogMsg* pMsg);

	virtual bool Handle(const ConfirmDialogMsg* pMsg);

	virtual bool Handle(const GetExistActionMsg* pMsg);

	void DeleteMapData(HANDLE event_lock);

	CommonUIMsgStruct* FindMapData(HANDLE event_lock);

	void InsertMapData(HANDLE event_lock,CommonUIMsgStruct *data);

public:
	HWND _hLastNoticeWnd;

	map<HANDLE,CommonUIMsgStruct*> _msgReturn;
};