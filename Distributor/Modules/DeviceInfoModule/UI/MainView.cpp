#include "stdafx.h"
#include "MainView.h"
#include "CommonInfoView.h"

MainView::MainView(wstring deviceID, void* phone)
{
	_deviceID = deviceID;
	_pPhone = phone;

	_bExitRecovery = false;

	_pFuncModule = NULL;
	_pRecentModule = NULL;
	_pCommonInfoView= new CommonInfoView(deviceID, phone);
}

MainView::~MainView()
{
	CControlUI* pContainer = _pCommonInfoView->GetParent();
	if(pContainer)
		pContainer->Remove(_pCommonInfoView);

	_pCommonInfoView = NULL;

	if(_pFuncModule)
		_pFuncModule->Release();

	if(_pRecentModule)
		_pRecentModule->Release();
}

std::wstring MainView::GetStyleID()
{
	return L"DeviceInfo_View_Main";
}

void MainView::OnCreate()
{
	CControlUI* pControl = GetItem(_T("Container"));
	if (pControl)
		pControl->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);

	RequestDeviceTypeMsg msg;
	msg.deviceID = this->_deviceID;
	msg.SendAndGetReturn();

	RequestIsRecoveryMode recMsg;
	recMsg.deviceID = this->_deviceID;
	recMsg.SendAndGetReturn();

	if(!recMsg.bRecoverMode)
	{
		CLayoutUI* pDevInfoLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Container.Layout_DeviceInfo"));
		if(pDevInfoLayout)
		{
			_pCommonInfoView->Init(pDevInfoLayout);
		}
		CLayoutUI* pFuncLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Container.Layout_FuncCenter"));
		if(pFuncLayout)
		{
			_pFuncModule = CreateFuncCenterModule(_deviceID, _pPhone);
			_pFuncModule->Init(pFuncLayout);
		}
		CLayoutUI* pRecentCaseLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Container.Layout_RecentCase"));
		if(pRecentCaseLayout)
		{
			_pRecentModule = CreateRecentInstallCaseModule(_deviceID, _pPhone);
			_pRecentModule->Init(pRecentCaseLayout);
		}
	}
	else
	{
		CLayoutUI* pContainerLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Container"));
		if(pContainerLayout != NULL)
		{
			pContainerLayout->SetVisible(false);
			pContainerLayout->Resize();
			pContainerLayout->Invalidate();
		}

		CLayoutUI* pRecoveryLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Layout_Recovery"));
		if(pRecoveryLayout != NULL)
		{
			pRecoveryLayout->SetVisible(true);
			pRecoveryLayout->Resize();
			pRecoveryLayout->Invalidate();
		}

		RequestDeviceInfoMsg infoMsg;
		infoMsg.deviceID = this->_deviceID;
		infoMsg.nType = RequestDeviceInfoType_All;
		infoMsg.Send();
	}
}

bool MainView::OnButtonExitRecoveryClick( TNotifyUI* pNotify )
{
	if(_bExitRecovery)
		return true;
	_bExitRecovery = true;

	CStaticsticsMessage statMsg("10013");
	statMsg.Send();

	RequestExitRecoveryDeviceMsg msg;
	msg.deviceID = this->_deviceID;
	msg.Send();

	return true;

}

bool MainView::OnDropFiles( TNotifyUI* pNotify )
{
	NotifyInstallAppMsg installMsg;
	installMsg.deviceID = this->_deviceID;
	installMsg.nType = NotifyInstallAppType_InstallFiles;

	vector<wstring> files;
	HDROP hDropInfo = (HDROP)pNotify->wParam;

	UINT count;
	wchar_t filePath[512];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{        
		for(UINT i=0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			installMsg.vetFiles.push_back(filePath);
		}
	}

	installMsg.PostToUI();

	DragFinish(hDropInfo);

	return true;
} 

bool MainView::Handle( const ResponseDeviceBaseInfoMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	if(pMsg->nOsType == ePhoneOS_Ios)
	{
		CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Recovery.Layout_Info"));
		if(pLayout != NULL)
		{
			this->SetItemInfo(pLayout, L"Info_ProductType", pMsg->deviceHWModel);
			this->SetItemInfo(pLayout, L"Info_SerialNumber", this->_deviceID);
		}
	}
	return true;
}

void MainView::SetItemInfo( CLayoutUI* pLayout, wstring id, wstring strValue )
{
	CControlUI* pCtrl = pLayout->GetItem(id.c_str());
	if(pCtrl != NULL)
	{
		CEditUI* pEdit = dynamic_cast<CEditUI*>(pCtrl->GetItem(L"value"));
		if(pEdit != NULL)
		{
			pEdit->SetText(strValue.c_str());
			pEdit->SetAttribute(L"tooltip", strValue.c_str());
		}
	}
}
