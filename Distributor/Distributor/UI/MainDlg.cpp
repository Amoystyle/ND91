/********************************************************************
	主窗口，负责处理消息和非控件事件
*********************************************************************/
#include "StdAfx.h"

MainDlg* MainDlg::_pInstance = NULL;

MainDlg::MainDlg(void)
{
	SetWindowClassName(WNDTITLE_CLASSNAME);
    _bInitShow = false;
	_pSelectModule = NULL;
}

MainDlg::~MainDlg(void)
{
	LOG->WriteDebug(L"MainDlg::~MainDlg(void)");
	ExitProcess(0);
}

MainDlg* MainDlg::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new MainDlg();

	return _pInstance;
}

void MainDlg::OnInitShow()
{    
	if (_bInitShow)
		return;

	_bInitShow = true;

	CContainerUI* pTitleToolBar = dynamic_cast<CContainerUI*>(GetItem(L"TitleToolBar"));
	CContainerUI* pMainContainer = dynamic_cast<CContainerUI*>(GetItem(L"MainContainer"));
	if(pMainContainer != NULL && pTitleToolBar != NULL)
	{
		ModuleManager::GetInstance()->Init(pTitleToolBar, pMainContainer);
		ModuleManager::GetInstance()->Show();
	}
}

void MainDlg::OnClose()
{
	Singleton::GetEventManager()->ExitSystem();

	ModuleManager::Release();

	__super::OnClose();
LOG->WriteDebug(L"MainDlg::OnClose()");
	::PostQuitMessage(-1);
}

bool MainDlg::Handle(const InvalidateMainMsg* pMsg)
{

	this->Invalidate();
	return true;
}

LRESULT MainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_GROUP_BEGIN:
		{
			IMessage* pMessage = reinterpret_cast<IMessage*>(lParam);
			Singleton::GetMessageCenter()->Begin(pMessage);
			delete pMessage;
		}
		break;
	case WM_GROUP_END:
		{
			IMessage* pMessage = reinterpret_cast<IMessage*>(lParam);
			Singleton::GetMessageCenter()->End(pMessage);
			delete pMessage;
		}
		break;

	case WM_SEND_TO_MAIN:
		{
			IMessage* pMessage = reinterpret_cast<IMessage*>(lParam);
			Singleton::GetMessageCenter()->Send(pMessage);
			delete pMessage;
		}
		break;

	case WM_ICON_NOTIFY:
		{
			IconNotifyMsg msg;
			msg.lParam = lParam;
			msg.wParam = wParam;
			msg.Send();
		}
		break;
	case WM_ONLY:
		{
			this->ShowWindow(SW_MINIMIZE);
			this->ShowWindow(SW_RESTORE);
			this->Invalidate();
		}
		break;
	case WM_UPDATE_MSG:
		{
			UpdateVersion_Msg UpdateMsg;
			UpdateMsg.SendToUI();
		}
		break;
	default:
		break;
	}

	LRESULT ret = CWindowUI::WindowProc(message,wParam,lParam);


	if (WM_NCDESTROY == message)
	{
		LOG->WriteDebug(L"WM_NCDESTROY == message");
		// 析构已经做完了，直接退吧（退出处理在OnClose方法和析构函数）
		GetUIEngine()->UnInitSkin();    //结束界面
		ExitProcess(0);
	}
	return ret;
}

void MainDlg::Init()
{
	this->Create(NULL, _T("main"));
	this->CenterWindow();
	this->ShowWindow();
	this->SetEnableDragDrop(true);

	Singleton::Init(GetHWND());

}

void MainDlg::OnMinimize()
{
	UIMiniSizeMsg SizeMsg;
	SizeMsg.Send();
}

bool MainDlg::OnClosing(WPARAM wParam, LPARAM lParam)
{
	if( wParam != 2 && wParam !=0 )
	return true;
	if(CONFIG(_bAppClose))
	{
		return true;
	}
	else
	{
		this->ShowWindow(SW_HIDE);
		this->Invalidate();
		return false;
	}
}

bool MainDlg::Handle( const RequestShowResourceLoadingMsg* pMsg )
{
	CControlUI* pLoading = this->GetItem(L"LayoutLoading");
	if(pLoading)
	{
		OSVERSIONINFO osinfo; 
		osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
		if (!GetVersionEx(&osinfo)) 
			return false;

		if(osinfo.dwMajorVersion == 5)
			pLoading->SetAttribute(L"fill.image", L"#Image_Window_DefaultBg_XP");
		else
			pLoading->SetAttribute(L"fill.image", L"#Image_Window_DefaultBg_Win7");

		this->SetAttribute(L"resize",pMsg->bShow? L"0" : L"1");
		pLoading->SetVisible(pMsg->bShow);
		this->Invalidate();
	}
	return true;
}
