#include "stdafx.h"
#include "DependenceFuncCenterController.h"
#include "UI/FeedbackUI.h"
#include "UI/TrayIcon.h"
#include "UI/AboutWnd.h"
#include "UI/WhatsNewDlg.h"
#include "UI/Setting/SystemSetDlg.h"

DependenceFuncCenterController::DependenceFuncCenterController()
{
	_pTrayIcon = new CTrayIcon;
}

void DependenceFuncCenterController::Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar)
{
	_pTrayIcon->InitTrayIcon();

	WhatsNewDlgMsg WhatsMsg;
	WhatsMsg.SendToUI();
}

DependenceFuncCenterController::~DependenceFuncCenterController()
{
	SAFE_DELETE(_pTrayIcon);
}

bool DependenceFuncCenterController::Handle(const IconNotifyMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	_pTrayIcon->OnTrayClick(pMsg->wParam, pMsg->lParam);

	return true;
}

bool DependenceFuncCenterController::Handle(const MenuNotifyMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	switch (pMsg->uMsgType)
	{
	case MENU_ID_SYSTEMSET: //设置
		{
			CStaticsticsMessage StaticMsg( pMsg->nMenutype == 0 ? "80201" : "80102");
			StaticMsg.Send();

			SystemSetDlg* systemset = new SystemSetDlg;
			systemset->DoModal();
		}
		break;
	case MENU_ID_FEEDBACK: //问题反馈
		{
			CStaticsticsMessage StaticMsg("80202");
			StaticMsg.Send();

			RequestFeedbackMsg msg;
			msg.SendToUI();

			//return FeedBack();
		}
		break;
	case MENU_ID_OFFICIALWEB: // 软件官网
		{
			CStaticsticsMessage StaticMsg(pMsg->nMenutype == 0 ? "80203" : "80103");
			StaticMsg.Send();
	
			BaseOperation::Open(L"http://zj.91.com");
		}
		break;
	case MENU_ID_CHECKUP: // 检查更新
		{
			CStaticsticsMessage StaticMsg(pMsg->nMenutype == 0 ? "80204" : "80104");
			StaticMsg.Send();

			wstring source = CFileOperation::GetCurrWorkingDir() + L"update\\update.exe";
			wstring dest   = CFileOperation::GetCurrWorkingDir() + L"update.exe";
			CopyFile(source.c_str(), dest.c_str(), false);
#ifndef _DEBUG
			BaseOperation::Execute(CFileOperation::GetCurrWorkingDir() + _T("update.exe"), _T("/p2p"));
#endif
		}
		break;
	case MENU_ID_SHOWWHATSNEW: // 新功能向导
		{
			/*CStaticsticsMessage StaticMsg(pMsg->nMenutype == 0 ? "80206" : "80106");
			StaticMsg.Send();

			string strVer = CClientInfo::GetInstance()->GetVersion();
			wstring strLastVer = MyFileOperation::GetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("whatsnew_ver"));
			if ((int)CCodeOperation::GB2312ToUnicode(strVer).find(strLastVer) < 0 || strLastVer.length() == 0 || true)
			{
				MyFileOperation::SetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("whatsnew_ver"), CCodeOperation::GB2312ToUnicode(strVer));
				CWhatsNewDlg::GetInstance()->ShowWindow();
				CWhatsNewDlg::GetInstance()->CenterWindow(Singleton::GetMainWnd());
				CWhatsNewDlg::GetInstance()->WindowBringToTop();
			}*/
		}
		break;
	case MENU_ID_ABOUT: //关于
		{
			CStaticsticsMessage StaticMsg(pMsg->nMenutype == 0 ? "80206" : "80106");
			StaticMsg.Send();
	
			AboutWnd *aboutWnd = new AboutWnd;
			aboutWnd->Show(Singleton::GetMainWnd());
		}
		break;
	case MENU_ID_CLOSE:	//退出
		{
			CStaticsticsMessage StaticMsg("80207");
			StaticMsg.Send();

			_pTrayIcon->Close();
		}
		break;
	}

	return true;
}

bool DependenceFuncCenterController::Handle(const WhatsNewDlgMsg* pMsg)
{
	string strVer = CClientInfo::GetInstance()->GetVersion();
	wstring strLastVer = MyFileOperation::GetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("whatsnew_ver"));
	if ((int)CCodeOperation::GB2312ToUnicode(strVer).find(strLastVer) < 0 || strLastVer.length() == 0)
	{
		MyFileOperation::SetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("whatsnew_ver"), CCodeOperation::GB2312ToUnicode(strVer));
		CWhatsNewDlg::GetInstance()->ShowWindow();
		CWhatsNewDlg::GetInstance()->CenterWindow(Singleton::GetMainWnd());
		CWhatsNewDlg::GetInstance()->WindowBringToTop();
	}

	return true;
}

void DependenceFuncCenterController::Exit()
{
	_pTrayIcon->CloseTray();
}

// bool DependenceFuncCenterController::Handle( const NotifyRefreshModuleMsg* pMsg )
// {
// 	return true;
// }
// 
