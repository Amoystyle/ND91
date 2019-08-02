#include "StdAfx.h"
#include "TrayIcon.h"

CTrayIcon::CTrayIcon(void)
{
}

CTrayIcon::~CTrayIcon(void)
{
}

//��ʼ������ͼ��
void CTrayIcon::InitTrayIcon()
{
	////�����Դʵ��
	//HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON1),RT_GROUP_ICON);
	////���ͼ����
	//hIcon = (HICON)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

	//���ͼ����
	IUIRes* res = GetUIRes();
	HICON hIcon = res->LoadIcon(CONFIG(_strTrayIconPath).c_str());
	//��NOTIFYICONDATA�ṹ��ֵ
	//�ṹ����
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);

	//���ھ��
	m_tnid.hWnd = Singleton::GetMainWnd();

	//�˵�ID
	//	m_tnid.uID = IDR_MENU1;
	//���Ա�־��NIF_MESSAGE����NOTIFYICONDATA�ṹָ������ϢuCallbackMessage��
	//���Ա�־��NIF_ICON�����½���ʾͼ�ꡣ
	//���Ա�־��NIF_TIP�����ͣ��ʱ��ʾ��ʾ��
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON |NIF_TIP;

	//���͵���Ϣ
	m_tnid.uCallbackMessage = WM_ICON_NOTIFY;

	//�����½���ʾ��ͼ�ꡣ
	m_tnid.hIcon = hIcon;

	//�������ͣ��ʱ��ʾ��������ʾ��
	lstrcpynW(m_tnid.szTip,CommonUIFunc::LoadString(_T("DependenceFuncCenter_App_title")),sizeof(m_tnid.szTip));

	Shell_NotifyIcon(NIM_ADD,&m_tnid);
}


bool CTrayIcon::OnTrayClick( WPARAM wParam,LPARAM lParam )
{
	//��������Ϣ
	UINT uMouseMsg = (UINT)lParam;

	switch(uMouseMsg)
	{
		//�����������
	case WM_LBUTTONDBLCLK:
		//����Windows���塣SW_RESTORE���ֹر�ʱ�����ӡ�
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
		//�����Ҽ�����
	case WM_RBUTTONUP:
		{
			CMenuUI* pMenu = CMenuUI::Load(MENU_ID_SYSTEMMENU);
			if (pMenu)
			{
				//���ط���
				pMenu->Remove(2 ,BY_POSITION);
				//----���ع���----
				pMenu->Remove(2 ,BY_POSITION);
				pMenu->Remove(2 ,BY_POSITION);
				//�����¹�����
				pMenu->Remove(3 ,BY_POSITION);
				pMenu->SetActiveShow(true);
				pMenu->SetParam(NULL,0);
				pMenu->Show(Singleton::GetMainWnd(), NULL, (INotifyUI*)this);
			}
		}
		break;
	}
	return true;

//	//�����Ҽ����£�������ݲ˵���
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