#include "stdafx.h"
#include "MainView.h"

MainView::MainView(void* pPhone)
{
	_pPhone = pPhone;
}

MainView::~MainView()
{

}

std::wstring MainView::GetStyleID()
{
	return L"ConnectGuide_View_Main";
}

void MainView::SetStep( ConnectGuideStep step )
{
	CRadioUI* pStepButton = NULL;
	CLayoutUI* pContainer = NULL;
	switch(step)
	{
	case ConnectGuideStep_LinkIn:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step1"));
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Step1"));
		}
		break;
	case ConnectGuideStep_ConnectInit:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step1"));
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Step1"));
		}
		break;
	case ConnectGuideStep_ConnectWaitWindow:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step1"));
			if(pStepButton == NULL || !pStepButton->IsChecked())
				return;
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Step1"));
			this->ShowInstallWaitView();
		}
		break;
	case ConnectGuideStep_DriverInstall:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step2"));
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Step2"));
			this->ShowInstallWaitView();
		}
		break;
	case ConnectGuideStep_Connecting:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step3"));
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Step3"));
		}
		break;
	case ConnectGuideStep_Error:
		{
			pStepButton = dynamic_cast<CRadioUI*>(GetItem(L"LayoutStepButton.Step3"));
			pContainer = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo.Error"));
		}
		break;
	default:
		break;
	}
	if(!pStepButton || !pContainer)
		return;

	pStepButton->SetChecked(true);

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"LayoutStepInfo"));
	if(!pLayout)
		return;

	int nCount = pLayout->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CControlUI* pControl = pLayout->GetItem(i);
		if(pContainer != pControl)
			pControl->SetVisible(false);
	}
	if(!pContainer->IsVisible())
		pContainer->SetVisible(true);

	pLayout->Resize();
	pLayout->Invalidate();
}

void MainView::OnDriverInstallInit()
{
	this->SetControlVisible(L"LayoutStepInfo.Step2.Installing", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Downloading", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Init", true);
}

void MainView::OnDriverProgress( DriverInstallState nState, int nStep, int nProgress )
{
	CLabelUI* pLabel = NULL;

	wchar_t strProgress[128] ={0};
	if(nStep <= 0)
	{
		if(nState == DriverInstallState_Downloading)
		{
			pLabel = dynamic_cast<CLabelUI*>(GetItem(L"LayoutStepInfo.Step2.Downloading"));
			wsprintfW(strProgress, CommonUIFunc::LoadString(L"ConnectGuide_Text_DownloadingProgress"), nProgress);
			this->OnDriverDownloading();
		}
		else
		{
			pLabel = dynamic_cast<CLabelUI*>(GetItem(L"LayoutStepInfo.Step2.Installing"));
			wsprintfW(strProgress, CommonUIFunc::LoadString(L"ConnectGuide_Text_Installing"));
			this->OnDriverInstalling();
		}

	}
	else
	{
		if(nState == DriverInstallState_Downloading)
		{
			pLabel = dynamic_cast<CLabelUI*>(GetItem(L"LayoutStepInfo.Step2.Downloading"));
			wsprintfW(strProgress, CommonUIFunc::LoadString(L"ConnectGuide_Text_DownloadingProgress_Retry"), nStep, nProgress);
			this->OnDriverDownloading();
		}
		else
		{
			pLabel = dynamic_cast<CLabelUI*>(GetItem(L"LayoutStepInfo.Step2.Installing"));
			wsprintfW(strProgress, CommonUIFunc::LoadString(L"ConnectGuide_Text_Installing_Retry"), nStep);
			this->OnDriverInstalling();
		}
	}
	if(pLabel == NULL)
		return;

	pLabel->SetText(strProgress);

	pLabel->Resize();
	pLabel->Invalidate();
}

void MainView::OnDriverDownloading()
{
	this->SetControlVisible(L"LayoutStepInfo.Step2.Init", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Installing", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Downloading", true);
}

void MainView::OnDriverInstalling()
{
	this->SetControlVisible(L"LayoutStepInfo.Step2.Init", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Downloading", false);
	this->SetControlVisible(L"LayoutStepInfo.Step2.Installing", true);
}

void MainView::OnDriverInstallEnd()
{
}

void MainView::OnError( DeviceConnectError error )
{
	wstring tipImg;
	wstring tipPos;
	switch(error)
	{
	case DeviceConnectError_FailToActive:
		tipImg = L"#ConnectGuide_Image_Error_InitDeamon";
		tipPos = L"9|-1, 0, 471,342";
		break;
	case DeviceConnectError_Check_OffDebug:
		{
			tipImg = L"#ConnectGuide_Image_Error_DebugMode";
			tipPos = L"9|-1, 0, 501,350";
			CControlUI* pViewMoreDebugLayout = dynamic_cast<CControlUI*>(GetItem(L"LayoutStepInfo.Error.ViewMoreDebugLayout"));
			if(pViewMoreDebugLayout != NULL)
			{
				pViewMoreDebugLayout->SetVisible(true);
			}
		}
		break;
	case DeviceConnectError_Check_offline:
		tipImg = L"#ConnectGuide_Image_Error_Offline";
		tipPos = L"9|-1, 0, 471,342";
		break;
	case DeviceConnectError_ACR_Offline:
		tipImg = L"#ConnectGuide_Image_Error_Offline";
		tipPos = L"9|-1, 0, 471,342";
		break;
	case DeviceConnectError_ACS_Offline:
		tipImg = L"#ConnectGuide_Image_Error_Offline";
		tipPos = L"9|-1, 0, 471,342";
		break;
	case DeviceConnectError_ACR_OffSpace:
		tipImg = L"#ConnectGuide_Image_Error_NoSpace";
		tipPos = L"9|-1, 0, 490, 168";
		break;
	case DeviceConnectError_ACR_NoSDCard:
		break;
	case DeviceConnectError_ACR_OffStorage:
		break;
	case DeviceConnectError_ICR_NeedUnLock:
		tipImg = L"#ConnectGuide_Image_Error_Password";
		tipPos = L"9|-1, 0, 490, 168";
		break;
	case DeviceConnectError_Check_DriverError:
	case DeviceConnectError_Check_DriverUnload:
		tipImg = L"#ConnectGuide_Image_Error_Driver";
		tipPos = L"9|-1, 0, 471, 342";
		break;
	case DeviceConnectError_Check_ItunesNeedReconnect:
	case DeviceConnectError_Check_ItunesNeedReconnect2:
		tipImg = L"#ConnectGuide_Image_Error_iTunesCompleted";
		tipPos = L"9|-1, 0, 490, 168";
		break;
	case DeviceConnectError_Check_ITunesNoFind:
	case DeviceConnectError_ICR_NoPhone:
	case DeviceConnectError_ACR_DeviceNoFound:
		{
			tipImg = L"#ConnectGuide_Image_Error_iTunesNoFound";
			tipPos = L"9|-1, 0, 471, 342";
			RequestWindowSystemInfoMsg msg;
			msg.SendAndGetReturn();
			if(msg.bIsNeedInstalliTunes)
			{
				CControlUI* pUpdateiTunesLayout = dynamic_cast<CControlUI*>(GetItem(L"LayoutStepInfo.Error.UpdateiTunesLayout"));
				if(pUpdateiTunesLayout != NULL)
				{
					pUpdateiTunesLayout->SetVisible(true);
				}
			}
		}
		break;
	case DeviceConnectError_Check_Exception:
	case DeviceConnectError_Check_ConnectTypeError:
	case DeviceConnectError_Check_ADBError:
	case DeviceConnectError_Check_ConnectModeError:
	case DeviceConnectError_ACR_ModeError:
	case DeviceConnectError_ICR_ParamError:
	case DeviceConnectError_UnKnow:
		tipImg = L"#ConnectGuide_Image_Error_Unknow";
		tipPos = L"9|-1, 0, 471,342";
		break;
	case DeviceConnectError_ACR_UpdateDeamonConfirm:
	case DeviceConnectError_ACR_InstallDeamonFailed:
	case DeviceConnectError_ACR_StartDeamonFailed:
	case DeviceConnectError_ACR_LongConnFailed:
	case DeviceConnectError_ACR_AppMgrNotStart:
	case DeviceConnectError_ACR_ForwardPortFailed:
	case DeviceConnectError_ACR_HigherVerDeamon:
	case DeviceConnectError_ACR_LowerVerDeamon:
	case DeviceConnectError_ACR_Failed:
		break;
	case DeviceConnectError_Check_Success:
	case DeviceConnectError_ICR_Success:
	case DeviceConnectError_ACR_Success:
	case DeviceConnectError_ACR_AlreadyConnect:
	case DeviceConnectError_ACS_DisConnect:
	case DeviceConnectError_ICR_ConnectConfirm:
	case DeviceConnectError_ACS_ConnectConfirm:
		break;
	case DeviceConnectError_Check_UnConnect:
		tipImg = L"#ConnectGuide_Image_Error_Unknow";
		tipPos = L"9|-1, 0, 471,342";
		break;
	}

	CControlUI* pErrorTip = dynamic_cast<CControlUI*>(GetItem(L"LayoutStepInfo.Error.ErrorTip"));
	if(!pErrorTip)
		return;

	pErrorTip->SetAttribute(L"fill.image", tipImg.c_str());
	pErrorTip->SetAttribute(L"pos", tipPos.c_str());
	pErrorTip->GetParent()->Resize();
	pErrorTip->GetParent()->Invalidate();
}

bool MainView::OnFeedbackButtonClick( TNotifyUI* pNotify )
{
	RequestFeedbackMsg msg;
	msg.Send();
	return true;
}

bool MainView::OnReConnectButtonClick( TNotifyUI* pNotify )
{
	RequestConnectMsg msg;
	msg.pPhone = this->_pPhone;
	msg.SendToUI();
	return true;
}

void MainView::SetControlVisible( wstring id, bool bVisible /*= true*/ )
{
	CControlUI* pControl = GetItem(id.c_str());
	if(!pControl)
		return;

	pControl->SetText(L"");
	pControl->SetVisible(bVisible);
	pControl->Resize();
	pControl->Invalidate();
}

void MainView::ShowInstallWaitView()
{
	RequestWindowSystemInfoMsg msg;
	msg.SendAndGetReturn();

	if(msg.bIsXP)
	{
		this->ShowXpTip(L"Step1", true);
		this->ShowXpTip(L"Step2", true);
	}
	else
	{
		this->ShowXpTip(L"Step1", false);
		this->ShowXpTip(L"Step2", false);
	}

	this->Resize();
	this->Invalidate();
}

void MainView::ShowXpTip( wstring id, bool bShow /*= true*/ )
{
	wstring tipId = L"LayoutStepInfo." + id + L".Layout_Tip.Layout_Xp";
	CLayoutUI* pLayoutXp = dynamic_cast<CLayoutUI*>(GetItem(tipId.c_str()));
	if(pLayoutXp != NULL)
	{
		pLayoutXp->SetVisible(bShow);
	}

	tipId = L"LayoutStepInfo." + id + L".Layout_Tip.Layout_Win7";
	CLayoutUI* pLayoutWin7 = dynamic_cast<CLayoutUI*>(GetItem(tipId.c_str()));
	if(pLayoutWin7 != NULL)
	{
		pLayoutWin7->SetVisible(!bShow);
	}
}

bool MainView::OnDownloadiTunesButtonClick( TNotifyUI* pNotify )
{
	RequestWindowSystemInfoMsg msg;
	msg.SendAndGetReturn();

	wstring url = L"http://nav.sj.91.com/launchDownload.aspx?os=";
	url += msg.bIsXP?L"0":L"1";
	url += L"&b=";
	url += (msg.bIs64?L"1":L"0");
	url +=  L"&lan=0&sn=0";
	BaseOperation::Open(url);
	return true;
}

bool MainView::OnViewMoreDebugSettings( TNotifyUI* pNotify )
{
	RequestWindowSystemInfoMsg msg;
	msg.SendAndGetReturn();

	wstring url = L"http://ibbs.91.com/forum.php?mod=viewthread&tid=791677&fromuid=54409";
	BaseOperation::Open(url);
	return true;
}

