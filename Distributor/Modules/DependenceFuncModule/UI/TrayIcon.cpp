#include "StdAfx.h"
#include "TrayIcon.h"

CTrayIcon::CTrayIcon(void)
{
}

CTrayIcon::~CTrayIcon(void)
{
}

//初始化托盘图标
void CTrayIcon::InitTrayIcon()
{
	////获得资源实例
	//HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON1),RT_GROUP_ICON);
	////获得图标句柄
	//hIcon = (HICON)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

	//获得图标句柄
	IUIRes* res = GetUIRes();
	HICON hIcon = res->LoadIcon(CONFIG(_strTrayIconPath).c_str());
	//给NOTIFYICONDATA结构赋值
	//结构长度
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);

	//窗口句柄
	m_tnid.hWnd = Singleton::GetMainWnd();

	//菜单ID
	//	m_tnid.uID = IDR_MENU1;
	//属性标志：NIF_MESSAGE发送NOTIFYICONDATA结构指明的消息uCallbackMessage。
	//属性标志：NIF_ICON在右下角显示图标。
	//属性标志：NIF_TIP当鼠标停留时显示提示。
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON |NIF_TIP;

	//发送的消息
	m_tnid.uCallbackMessage = WM_ICON_NOTIFY;

	//在右下角显示的图标。
	m_tnid.hIcon = hIcon;

	//设置鼠标停留时显示的文字提示。
	lstrcpynW(m_tnid.szTip,CommonUIFunc::LoadString(_T("DependenceFuncCenter_App_title")),sizeof(m_tnid.szTip));

	Shell_NotifyIcon(NIM_ADD,&m_tnid);
}


bool CTrayIcon::OnTrayClick( WPARAM wParam,LPARAM lParam )
{
	//获得鼠标消息
	UINT uMouseMsg = (UINT)lParam;

	switch(uMouseMsg)
	{
		//处理左键按下
	case WM_LBUTTONDBLCLK:
		//弹出Windows窗体。SW_RESTORE保持关闭时的样子。
		{
			CWindowUI* pUI = GetUIEngine()->GetWindow(Singleton::GetMainWnd());
			if (pUI)
			{
				pUI->ShowWindow(SW_MINIMIZE);
				pUI->ShowWindow(SW_RESTORE);
				pUI->Invalidate();

				BringWindowToTop(Singleton::GetMainWnd());
			}
		}
		break;
		//处理右键弹起
	case WM_RBUTTONUP:
		{
			CMenuUI* pMenu = CMenuUI::Load(MENU_ID_SYSTEMMENU);
			if (pMenu)
			{
				//隐藏反馈
				pMenu->Remove(2 ,BY_POSITION);
				//----隐藏官网----
				pMenu->Remove(2 ,BY_POSITION);
				pMenu->Remove(2 ,BY_POSITION);
				//隐藏新功能向导
				pMenu->Remove(3 ,BY_POSITION);
				pMenu->SetActiveShow(true);
				pMenu->SetParam(NULL,0);
				pMenu->Show(Singleton::GetMainWnd(), NULL, (INotifyUI*)this);
			}
		}
		break;
	}
	return true;

//	//处理右键按下，弹出快捷菜单。
//case WM_RBUTTONDOWN:
}

void CTrayIcon::Close()
{
	CWindowUI* pUI = GetUIEngine()->GetWindow(Singleton::GetMainWnd());
	if (pUI)
	{
		pUI->ShowWindow(SW_HIDE);
		Shell_NotifyIconW(NIM_DELETE,&m_tnid);
		PostMessage(Singleton::GetMainWnd(), WM_NCDESTROY, NULL, NULL);
	}
}

void CTrayIcon::CloseTray()
{
	Shell_NotifyIconW(NIM_DELETE,&m_tnid);
}

bool CTrayIcon::OnMenuClick( TNotifyUI* pNotify )
{
	if(pNotify->wParam == NULL)
		return true;

	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	if (NULL == pInfo)
	{
		return false;
	}
	int menutype = pInfo->lParamNotify;

	MenuNotifyMsg msg;
	msg.uMsgType  = pInfo->uID;
	msg.nMenutype = menutype;
	msg.SendToUI();

	return true;
}