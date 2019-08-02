#pragma once

enum Messagebox_Button		//��ʾ��ť��ʾ����
{
	MY_MB_OKCANCEL,		//ȷ��ȡ��
	MY_MB_YESNO		//�Ƿ�
};

enum Common_EXIST_ACTION
{
	Common_OVERWRITE,              // ����
	Common_OVERWRITE_ALL,          // ��������
	Common_RENAME,                 // ������
	Common_RENAME_ALL,             // ����������
	Common_RENAME_COPY,            // ������(�ļ���ǰ��������)
	Common_RENAME_COPY_ALL,        // ����������(�ļ���ǰ��������)
	Common_CANCEL,                 // ȡ��
	Common_NONE,                   // ��ֵ���޺���
};

struct CommonUIMsgStruct 
{
 	bool		return_bool;		//bool����ֵ
 	wstring		return_string;		//string����ֵ
	int			return_int;			//int����ֵ
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
	static CommonUIFunc* _pInstance;   // ����

	// ȡ�õ�������
	static CommonUIFunc* GetInstance();
public:
	// �ͷŵ�������
	static void Release();

public:

	static void RegeditControl();

	// �Ի��������ַ���
	static std::wstring InputDialog(wstring strMsg, wstring strTitle, wstring strDefault, bool& bCancel, int maxChar = 100,HWND hOwnerWnd=NULL);

	// ������ʾ�Ի����Զ���ʧ��
	static void NoticeDialog(wstring strMsg, NotifyIcon strIcon = ICON_SUCCESS, int nDelayTime = 2000,HWND hOwnerWnd=NULL);

	// ��Ϣ�Ի���
	static void MessageDialog(wstring strMsg, wstring strTitle=L"", wstring strIcon=L"", wstring tip=L"", wstring okText=L"",HWND hOwnerWnd=NULL);

	// ȷ�϶Ի���
	static bool ConfirmDialog(wstring strMsg, wstring strTitle=L"", wstring strIcon=L"", int height = 0,int flag = 0,bool isWarnning = false,HWND hOwnerWnd=NULL);

	// ȷ�϶Ի��򣨾����ͣ�
	static bool ConfirmWarnDialog(wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag,HWND hOwnerWnd=NULL);

	// ������ʾ�Ի���
	static Common_EXIST_ACTION GetExistAction( std::wstring name, HWND hOwnerWnd=NULL);

	// �򿪲˵�
	// menuID : �˵���ID
	// hWnd : ��������
	// pNotify : ��Ϣ�����ߣ�һ��Ϊ�����߱���
	// pControl : Ҫ�����Ŀؼ���Ϊ�������λ����ʾ
	// wParam/lParam:���Ӳ���
	static void ShowMenu(UINT menuID, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);
	static void ShowMenu(CMenuUI *pMenu, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl);

	// ȡ���ַ���
	static const wchar_t* LoadString(const wchar_t* id) { return I18NSTR(id); }
private:
	CommonUIFunc();
	//���з�������ʵ��
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