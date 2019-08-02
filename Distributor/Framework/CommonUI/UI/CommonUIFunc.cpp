#include "stdafx.h"

CommonUIFunc* CommonUIFunc::_pInstance = NULL;
CommonUIFunc* CommonUIFunc::GetInstance()
{
	if (_pInstance)
		return _pInstance;
	_pInstance = new CommonUIFunc();
	return _pInstance;
}

void CommonUIFunc::Release()
{
	if (_pInstance)
		delete _pInstance;
	_pInstance = NULL;
}


CommonUIFunc::CommonUIFunc()
{
	_hLastNoticeWnd = NULL;
}	

void CommonUIFunc::DeleteMapData(HANDLE event_lock)
{
	AutoLock;
	map<HANDLE,CommonUIMsgStruct*> ::iterator iter;
	iter = _msgReturn.find(event_lock);
	if (iter != _msgReturn.end())
	{
		delete iter->second;
		_msgReturn.erase(iter);
	}
}

CommonUIMsgStruct* CommonUIFunc::FindMapData(HANDLE event_lock)
{
	AutoLock;
	CommonUIMsgStruct* pData = _msgReturn[event_lock];
	if(pData != NULL)
	{
		return pData;
	}
	return NULL;
}

void CommonUIFunc::InsertMapData(HANDLE event_lock,CommonUIMsgStruct *data)
{
	AutoLock;
	_msgReturn[event_lock] = data;
}

std::wstring CommonUIFunc::ShowInputDialog( wstring strMsg, wstring strTitle, wstring strDefault, bool& bCancel, int maxChar , HWND hOwnerWnd)
{
	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	HANDLE event_lock = CreateEvent(NULL, false, false, NULL);	

	InputDialogMsg msg;
	msg.nRequestID = atoi(request.c_str());
	msg.hOwnerWnd = hOwnerWnd;
	if (msg.hOwnerWnd == NULL)
		msg.hOwnerWnd = Singleton::GetMainWnd();
	msg.event_lock = event_lock;
	msg.strMsg = strMsg;
	msg.strTitle = strTitle;
	msg.strDefault = strDefault;
	msg.maxChar = maxChar;
	msg.SendToUI();

	WaitForSingleObject(event_lock, INFINITE);
	CloseHandle(event_lock);

	CommonUIMsgStruct* pdata = FindMapData(event_lock);
	if (pdata)
	{
		bCancel = pdata->return_bool;
		return pdata->return_string;
	}
	else 
		return L"";
}

void CommonUIFunc::ShowNoticeDialog(wstring strMsg,  wstring strIcon, int nDelayTime, HWND hOwnerWnd)
{
	NoticeDialogMsg msg;
	msg.hOwnerWnd = hOwnerWnd;
	if (msg.hOwnerWnd == NULL)
		msg.hOwnerWnd = Singleton::GetMainWnd();
	msg.strMsg = strMsg;
	msg.strIcon = strIcon;
	msg.nDelayTime = nDelayTime;
	msg.SendToUI();
}

void CommonUIFunc::ShowMessageDialog(wstring strMsg, wstring strTitle, wstring strIcon, wstring tip, wstring okText, HWND hOwnerWnd)
{
	MessageDialogMsg msg;
	msg.hOwnerWnd = hOwnerWnd;
	if (msg.hOwnerWnd == NULL)
		msg.hOwnerWnd = Singleton::GetMainWnd();
	msg.strMsg = strMsg;
	msg.strTitle = strTitle;
	msg.strIcon = strIcon;
	msg.tip = tip;
	msg.okText = okText;
	msg.SendToUI();
}

bool CommonUIFunc::ShowConfirmDialog(wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag,bool isWarnning, HWND hOwnerWnd)
{
	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	HANDLE event_lock = CreateEvent(NULL, false, false, NULL);	

	ConfirmDialogMsg msg;
	msg.hOwnerWnd = hOwnerWnd;
	if (msg.hOwnerWnd == NULL)
		msg.hOwnerWnd = Singleton::GetMainWnd();
	msg.nRequestID = atoi(request.c_str());
	msg.event_lock = event_lock;
	msg.strMsg = strMsg;
	msg.strTitle = strTitle;
	msg.strIcon = strIcon;
	msg.height = height;
	msg.flag = flag;
	msg.isWarnning = isWarnning;
	msg.SendToUI();

	WaitForSingleObject(event_lock, INFINITE);
	CloseHandle(event_lock);

	CommonUIMsgStruct* pdata = FindMapData(event_lock);
	if (pdata)
		return pdata->return_bool;
	else
		return true;
}

bool CommonUIFunc::ShowConfirmWarnDialog( wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag , HWND hOwnerWnd)
{
	return ShowConfirmDialog(strMsg, strTitle, strIcon, height,flag ,true );
}

Common_EXIST_ACTION CommonUIFunc::ShowGetExistAction( std::wstring name, HWND hOwnerWnd )
{
	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	HANDLE event_lock = CreateEvent(NULL, false, false, NULL);	

	GetExistActionMsg msg;
	msg.hOwnerWnd = hOwnerWnd;
	if (msg.hOwnerWnd == NULL)
		msg.hOwnerWnd = Singleton::GetMainWnd();
	msg.nRequestID = atoi(request.c_str());
	msg.event_lock = event_lock;
	msg.name = name;
	msg.SendToUI();

	WaitForSingleObject(event_lock, INFINITE);
	CloseHandle(event_lock);

	int nReturn = FileExistConfirmReturn_Cancel;
	CommonUIMsgStruct* pdata = FindMapData(event_lock);
	if (pdata)
		nReturn = pdata->return_int;

	switch (nReturn)
	{
	case FileExistConfirmReturn_Rewrite:
		return Common_OVERWRITE;
		break;
	case FileExistConfirmReturn_Rename:
		return Common_RENAME;
		break;
	case FileExistConfirmReturn_Cancel:
		return Common_CANCEL;
		break;
	case FileExistConfirmReturn_RewriteAll:
		return Common_OVERWRITE_ALL;
		break;
	case FileExistConfirmReturn_RenameAll:
		return Common_RENAME_ALL;
		break;
	}
	return Common_CANCEL;
}

void CommonUIFunc::ShowShowMenu( UINT menuID, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl)
{
	CMenuUI* pMenu = CMenuUI::Load(menuID);
	ShowShowMenu(pMenu, hWnd, pNotify, pControl);
}

void CommonUIFunc::ShowShowMenu( CMenuUI *pMenu, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl )
{
	POINT pos;
	if (pControl)
	{   // 控件对齐
		RECT rect = pControl->GetRect();
		pos.x = rect.left - 15;      // -15是因为最大化时候有可能超过屏幕的大小，所以左移15像素
		pos.y = rect.bottom;    
		ClientToScreen(hWnd, &pos);
	}
	else
	{
		// 鼠标对齐
		GetCursorPos(&pos);
		pos.x-=65;
		pos.y+=15;
	}
	pMenu->Show(hWnd, &pos, pNotify);
}

bool CommonUIFunc::Handle( const InputDialogMsg* pMsg )
{
	if (pMsg)
	{
		wstring strRet = pMsg->strDefault;
		InputDlg* inputDlg = new InputDlg(pMsg->strMsg,&strRet);
		inputDlg->Create(pMsg->hOwnerWnd,_T("InputDlg"));
		inputDlg->SetTitle(pMsg->strTitle.c_str());
		if(pMsg->maxChar != -1)
		{
			inputDlg->SetMaxChar(pMsg->maxChar);
		}
		inputDlg->CenterWindow(pMsg->hOwnerWnd);
		bool bCancel = IDOK != inputDlg->DoModal();
		

		CommonUIReturnMsg msg;		//发送返回值消息
		msg.nRequestID = pMsg->nRequestID;
		msg.event_lock = pMsg->event_lock;
		CommonUIMsgStruct *data = new CommonUIMsgStruct;
		data->return_bool = bCancel;
		data->return_string = strRet;
		data->nRequestID = pMsg->nRequestID;
		InsertMapData(pMsg->event_lock,data);
		msg.SendToUI();
	}
	return true;
}

bool CommonUIFunc::Handle( const NoticeDialogMsg* pMsg )
{
	if(_hLastNoticeWnd && IsWindow(_hLastNoticeWnd))
		PostMessage(_hLastNoticeWnd,WM_CLOSE,IDCLOSE,0);

	MessageBoxUI* Noticebox=new MessageBoxUI();
	int len = CCodeOperation::ws2s(pMsg->strMsg).length()*7;
	Noticebox->Show(pMsg->hOwnerWnd,pMsg->strIcon,pMsg->strMsg,len, pMsg->nDelayTime);

	_hLastNoticeWnd=Noticebox->GetHWND();

	return true;
}

bool CommonUIFunc::Handle( const MessageDialogMsg* pMsg )
{
	MessageWnd(pMsg->strMsg, pMsg->strTitle, pMsg->strIcon, MB_OK, 0, pMsg->okText).Show(pMsg->hOwnerWnd, pMsg->tip);
	return true;
}

bool CommonUIFunc::Handle( const ConfirmDialogMsg* pMsg )
{
	bool flag = true;
	if(pMsg)
	{
		if (pMsg->flag == MY_MB_OKCANCEL)
		{
			MessageWnd mw(pMsg->strMsg, pMsg->strTitle, pMsg->strIcon, MB_OKCANCEL, pMsg->height);
			flag =  IDOK == mw.Show(pMsg->hOwnerWnd, L"", pMsg->isWarnning); 
		}
		else if (pMsg->flag == MY_MB_YESNO)
		{
			MessageWnd mw(pMsg->strMsg, pMsg->strTitle, pMsg->strIcon, MB_YESNO, pMsg->height);
			flag =  IDOK == mw.Show(pMsg->hOwnerWnd, L"", pMsg->isWarnning); 
		}

		CommonUIReturnMsg msg;		//发送返回值消息
		msg.nRequestID = pMsg->nRequestID;
		msg.event_lock = pMsg->event_lock;
		CommonUIMsgStruct *data = new CommonUIMsgStruct;
		data->return_bool = flag;
		data->nRequestID = pMsg->nRequestID;
		InsertMapData(pMsg->event_lock,data);
		msg.SendToUI();
	}
	

	return true;
}

bool CommonUIFunc::Handle( const GetExistActionMsg* pMsg )
{
	if (pMsg)
	{
		int nReturn = FileExistConfirmReturn_Cancel;
		FileExistConfirmWnd *pWnd = new FileExistConfirmWnd(
			CommonUIFunc::LoadString(_T("CommonUi_FIL_Title")),
			pMsg->name);
		nReturn = pWnd->Show(pMsg->hOwnerWnd);

		CommonUIReturnMsg msg;		//发送返回值消息
		msg.nRequestID = pMsg->nRequestID;
		msg.event_lock = pMsg->event_lock;
		CommonUIMsgStruct *data = new CommonUIMsgStruct;
		data->return_int = nReturn;
		data->nRequestID = pMsg->nRequestID;
		InsertMapData(pMsg->event_lock,data);
		msg.SendToUI();
	}
	return true;
}


bool CommonUIFunc::Handle( const CommonUIReturnMsg* pMsg )
{
	if (pMsg)
	{
		CommonUIMsgStruct *pdata;
		pdata = FindMapData(pMsg->event_lock);
		if (pdata && pdata->nRequestID == pMsg->nRequestID)
			SetEvent(pMsg->event_lock);	
	}
	return true;
}

std::wstring CommonUIFunc::InputDialog(wstring strMsg, wstring strTitle, wstring strDefault, bool& bCancel, int maxChar, HWND hOwnerWnd )
{
	return GetInstance()->ShowInputDialog(strMsg, strTitle, strDefault, bCancel, maxChar ,hOwnerWnd);
}

void CommonUIFunc::NoticeDialog( wstring strMsg, NotifyIcon strIcon, int nDelayTime, HWND hOwnerWnd )
{
	wstring buf;
	switch (strIcon)
	{
	case ICON_SUCCESS:
		buf = L"#CommonUi_CommonUi_Success";
		break;
	case ICON_WARNING:
		buf = L"#CommonUi_CommonUi_warning1";
		break;
	default:
		buf = L"#CommonUi_CommonUi_Success";
		break;
	}
	GetInstance()->ShowNoticeDialog( strMsg, buf, nDelayTime,hOwnerWnd);
}

void CommonUIFunc::MessageDialog( wstring strMsg, wstring strTitle, wstring strIcon, wstring tip, wstring okText/*=L""*/, HWND hOwnerWnd )
{
	GetInstance()->ShowMessageDialog( strMsg, strTitle, strIcon, tip, okText ,hOwnerWnd);
}

bool CommonUIFunc::ConfirmDialog( wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag,bool isWarnning /*= false*/, HWND hOwnerWnd )
{
	return GetInstance()->ShowConfirmDialog(strMsg, strTitle, strIcon, height,flag,isWarnning,hOwnerWnd);
}

bool CommonUIFunc::ConfirmWarnDialog( wstring strMsg, wstring strTitle, wstring strIcon, int height,int flag, HWND hOwnerWnd)
{
	return GetInstance()->ShowConfirmWarnDialog(strMsg, strTitle, strIcon, height,flag ,hOwnerWnd);
}

void CommonUIFunc::ShowMenu( UINT menuID, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl )
{
	return GetInstance()->ShowShowMenu(menuID, hWnd, pNotify, pControl);
}

void CommonUIFunc::ShowMenu( CMenuUI *pMenu, HWND hWnd, INotifyUI* pNotify, CControlUI* pControl )
{
	return GetInstance()->ShowShowMenu(pMenu, hWnd, pNotify, pControl );
}

Common_EXIST_ACTION CommonUIFunc::GetExistAction( std::wstring name, HWND hOwnerWnd/*=NULL*/ )
{
	return GetInstance()->ShowGetExistAction(name,hOwnerWnd);
}

void CommonUIFunc::RegeditControl()
{
	//UI_REGISTER_DYNCREATE(_T("RichButton"), CRichButtonUI);
	UI_REGISTER_DYNCREATE(_T("AutoGridLayout"), CAutoGridLayoutUI);
	UI_REGISTER_DYNCREATE(_T("ToggleLayout"), CToggleLayoutUI);
	UI_REGISTER_DYNCREATE(_T("SimpleListItem"), CSimpleListItemUI);
}
