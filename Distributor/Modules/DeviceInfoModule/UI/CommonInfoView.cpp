#include "stdafx.h"
#include "CommonInfoView.h"

CommonInfoView::CommonInfoView(wstring deviceID, void* phone)
{
	_deviceID = deviceID;
	_pPhone = phone;
}

CommonInfoView::~CommonInfoView()
{
}

std::wstring CommonInfoView::GetStyleID()
{
	RequestDeviceTypeMsg msg;
	msg.deviceID = this->_deviceID;
	msg.SendAndGetReturn();

	RequestIsRecoveryMode recMsg;
	recMsg.deviceID = this->_deviceID;
	recMsg.SendAndGetReturn();

	if(!recMsg.bRecoverMode)
	{
		if(msg.deviceType == ePhoneOS_Android)
		{
			return L"DeviceInfo_View_AndroidCommonInfo";
		}
		else
		{
			return L"DeviceInfo_View_IOSCommonInfo";
		}
	}
	else
	{
		return L"DeviceInfo_View_RecoverMode";
	}
}

void CommonInfoView::OnCreate()
{
	RequestDeviceImageMsg imgMsg;
	imgMsg.deviceID = this->_deviceID;
	imgMsg.Send();

	RequestDeviceInfoMsg infoMsg;
	infoMsg.deviceID = this->_deviceID;
	infoMsg.nType = RequestDeviceInfoType_All;
	infoMsg.Send();

	this->LoadSpaceAndPower();
}

bool CommonInfoView::OnTopButtonClipBoardClick( TNotifyUI* pNotify /*= NULL*/ )
{
	wstring infoText = this->CreateInfoString();

	BaseOperation::CopyToClipboard(Singleton::GetMainWnd(), infoText);

	return true;
}

bool CommonInfoView::OnButtonNotePadClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CStaticsticsMessage statMsg("10018");
	statMsg.Send();

	wstring infoText = this->CreateInfoString();
	
	wstring id = CStrOperation::ReplaceW(this->_deviceID, L"\\", L"");
	id = CStrOperation::ReplaceW(id, L"?", L"Serial");
	
	wstring strFilePath = CONFIG(_strTemp); 
	strFilePath += id;
	strFilePath += L".txt";
	
	string strGb = CCodeOperation::UnicodeToGB2312(infoText);
	CFileOperation::WriteBinFile(strFilePath, strGb.c_str(), strGb.length());
	BaseOperation::Open(strFilePath);

	return true;
}

bool CommonInfoView::OnSaveInfoClick( TNotifyUI* pNotify /*= NULL*/ )
{
	RequestSaveUserInfoMsg msg;
	msg.deviceID = this->_deviceID;
	msg.Send();
	return true;
}

bool CommonInfoView::OnComboBoxNotify( TNotifyUI* pNotify )
{
	CComboBoxUI* pCombo = dynamic_cast<CComboBoxUI*>(GetItem(_T("Layout_Info.Info_InstallSpace.InstallSpace")));
	if (pCombo)
	{		
		CStaticsticsMessage statMsg("10001");
		statMsg.Send();

		wstring id = pCombo->GetCurSel()->GetId();
		bool pos = id == L"phone";

		if(pCombo->GetCount() > 1)
		{
			RequestChangeInstallPositionMsg msg;
			msg.bInstallOnPhone =  pos;
			msg.deviceID = this->_deviceID;
			msg.Send();
		}
	}
	return true;
} 

bool CommonInfoView::Handle( const ResponseDeviceBaseInfoMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	if(pMsg->nOsType == ePhoneOS_Android)
	{
		this->InitAndroidInfo(const_cast<ResponseDeviceBaseInfoMsg*>(pMsg));
	}
	else
	{
		this->InitIOSInfo(const_cast<ResponseDeviceBaseInfoMsg*>(pMsg));
	}
	return true;
}

bool CommonInfoView::Handle( const NotifyLoadDeviceInfoFinishMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	RequestUserInfoMsg msg;
	msg.deviceID = this->_deviceID;
	msg.Send();

	return true;
}

bool CommonInfoView::Handle( const ResponseDeviceStateMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	CLayoutUI* pLayout = NULL;
	if(pMsg->nOsType == ePhoneOS_Android)
	{
		pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
		if(pLayout == NULL)
			return true;

		this->SetItemInfo(pLayout, L"Info_IsRoot", CommonUIFunc::LoadString(pMsg->bIsRoot?L"DeviceInfo_Text_Info_Root":L"DeviceInfo_Text_Info_UnRoot"));
	}
	else
	{
		pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
		if(pLayout == NULL)
			return true;

		this->SetItemInfo(pLayout, L"Info_IsActive", CommonUIFunc::LoadString(pMsg->bIsActive?L"DeviceInfo_Text_Info_Actived":L"DeviceInfo_Text_Info_UnActive"));
	}

	pLayout->Resize();
	pLayout->Invalidate();

	return true;
}

bool CommonInfoView::Handle( const ResponseDeviceNameMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	this->SetDeviceName(pMsg->deviceName);
	return true;
}

bool CommonInfoView::Handle( const ResponseDeviceSpaceMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	if(pMsg->nOsType == ePhoneOS_Android)
	{
		this->UpdateAndroidSpace(pMsg->info);
	}
	else
	{
		this->UpdateIOSSpace(pMsg->info);
	}

	return true;
}

bool CommonInfoView::Handle( const ResponseDevicePowerInfoMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	CLayoutUI* pLayout = NULL;
	if(pMsg->nOsType == ePhoneOS_Android)
	{

		pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Layout_Space"));
	}
	else
	{
		pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Layout_Space"));
	}

	if(pLayout != NULL)
	{
		this->UpdateProgressBar(pLayout, L"Progress_Power", 100, pMsg->nPower);

		this->UpdatePowerLbl(pLayout, pMsg->nPower);
	}
	return true;
}

bool CommonInfoView::Handle( const ResponseDeviceImageMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	CControlUI* pImageCtrl = GetItem(L"Ctrl_DeviceImage");
	if(pImageCtrl != NULL && !pMsg->imagePath.empty())
	{
		pImageCtrl->SetAttribute(L"fill.image", pMsg->imagePath.c_str());
		pImageCtrl->Resize();
		pImageCtrl->Invalidate();
	}
	return true;
}

bool CommonInfoView::Handle( const DeviceActivatedMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	this->LoadSpaceAndPower();

	RequestDeviceTypeMsg msg;
	msg.deviceID = this->_deviceID;
	msg.SendAndGetReturn();

	if(msg.deviceType == ePhoneOS_Android)
	{
		RequestDeviceInfoMsg infoMsg;
		infoMsg.deviceID = this->_deviceID;
		infoMsg.nType = RequestDeviceInfoType_All;
		infoMsg.Send();
	}

	return true;
}

bool CommonInfoView::Handle( const ResponseSDCardStateMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
	if(pLayout == NULL)
		return true;

	CControlUI* pItem = pLayout->GetItem(L"Info_InstallSpace");
	if(pItem == NULL)
		return true;

	pItem->SetVisible(true);
	if(!pMsg->bHaveSDCard)
	{
		CComboBoxUI* pCom = dynamic_cast<CComboBoxUI*>(pItem->GetItem(L"InstallSpace"));
		if(pCom != NULL)
		{
			pCom->Remove(pCom->GetItem(0));
		}
	}

	pItem = this->GetItem(L"Layout_Space.Lbl_SDCardTitle");
	if(pItem == NULL)
		return true;

	pItem->SetVisible(pMsg->bHaveSDCard);

	pItem = this->GetItem(L"Layout_Space.Progress_SDCard");
	if(pItem == NULL)
		return true;
	pItem->SetVisible(pMsg->bHaveSDCard);

	pItem = this->GetItem(L"Layout_Space.SDCard_Left");
	if(pItem == NULL)
		return true;

	pItem->SetVisible(pMsg->bHaveSDCard);

	pLayout->Resize();
	pLayout->Invalidate();

	return true;
}

bool CommonInfoView::Handle( const ResponseChangeInstallPositionMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
	if(pLayout == NULL)
		return true;

	CControlUI* pItem = pLayout->GetItem(L"Info_InstallSpace");
	if(pItem == NULL)
		return true;

	CComboBoxUI* pCombo = dynamic_cast<CComboBoxUI*>(pItem->GetItem(_T("InstallSpace")));
	if (pCombo && pCombo->GetCount() > 1)
	{
		if(pMsg->bInstallOnPhone)
		{
			if(pCombo->GetCurSel() != pCombo->GetItem(1))
				pCombo->SelectItem(pCombo->GetItem(1));
		}
		else if(!pMsg->bInstallOnPhone)
		{
			if(pCombo->GetCurSel() != pCombo->GetItem(0))
				pCombo->SelectItem(pCombo->GetItem(0));
		}
	}

	pLayout->Resize();
	pLayout->Invalidate();

	return true;
}

bool CommonInfoView::Handle( const ResponseUserInfoMsg* pMsg )
{
	// 用户退出，点击保存用户资料时pMsg->deviceID传空值
	if(pMsg->deviceID != this->_deviceID && !pMsg->deviceID.empty() )
		return true;

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"UserInfo"));
	if(pLayout == NULL)
		return true;
	
	CButtonUI* pSaveButton = dynamic_cast<CButtonUI*>(GetItem(L"UserInfo.SaveInfo"));
	if(pSaveButton != NULL)
	{
		if ( pMsg->lastTime.empty() )
		{
			pSaveButton->SetText(CommonUIFunc::LoadString(L"DeviceInfo_Text_SaveCustomInfo"));
		}
		else
		{
			wstring text = CommonUIFunc::LoadString(L"DeviceInfo_Text_LinkDay") + pMsg->lastTime;
			pSaveButton->SetText(text.c_str());
			pSaveButton->SetVisible(true);
		}

		wstring strTitle = pMsg->lastTime.empty() ? CommonUIFunc::LoadString(L"DeviceInfo_Text_SaveCustomInfo")
			:	CommonUIFunc::LoadString(L"DeviceInfo_Text_ChangeCustomInfo");
		pSaveButton->SetToolTip(strTitle.c_str());
	}

	//隐藏复制用户信息
	pSaveButton->SetVisible(false);


	pLayout->SetVisible(true);
	pLayout->Resize();
	pLayout->Invalidate();	

	return true;
}

bool CommonInfoView::Handle( const ResponseUserLoginMsg* pMsg )
{
	RequestUserInfoMsg msg;
	msg.deviceID = this->_deviceID;
	msg.Send();

	return true;
}

void CommonInfoView::InitIOSInfo( ResponseDeviceBaseInfoMsg* pMsg )
{
	if(pMsg == NULL)
		return;

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
	CLayoutUI* pRightLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info_Right"));
	if(pLayout == NULL || pRightLayout == NULL)
		return;

	this->SetDeviceName(pMsg->deviceHWModel);

	this->SetItemInfo(pLayout, L"Info_ProductType", pMsg->deviceProductType);
	this->SetItemInfo(pLayout, L"Info_ProductVersion", pMsg->deviceProductVersion);
	this->SetItemInfo(pLayout, L"Info_SerialNumber",	 pMsg->serialNumber);
	this->SetItemInfo(pLayout, L"Info_IsActive", CommonUIFunc::LoadString(pMsg->bIsActived?L"DeviceInfo_Text_Info_Actived":L"DeviceInfo_Text_Info_UnActive"));
	this->SetItemInfo(pLayout, L"Info_IsJialbreak", CommonUIFunc::LoadString(pMsg->bIsJailBreak?L"DeviceInfo_Text_Info_Jialbreaked":L"DeviceInfo_Text_Info_UnJialbreak"));
	this->SetItemInfo(pRightLayout, L"Info_Area", pMsg->strSellArea);
	this->SetItemInfo(pRightLayout, L"Info_UUID", pMsg->uuid);
	this->SetItemInfo(pRightLayout, L"Info_IMEI", pMsg->deviceIMEI);
	this->SetItemInfo(pRightLayout, L"Info_BaseVersion", pMsg->deviceBaseVersion);
	
	int index = CStrOperation::toLowerW(pMsg->deviceProductType).find(L"iphone");
	if( index < 0)
	{
		CControlUI* pCtrl = pRightLayout->GetItem(L"Info_BaseVersion");
		if(pCtrl != NULL)
			pCtrl->SetVisible(false);
		pCtrl = pRightLayout->GetItem(L"Info_IMEI");
		if(pCtrl != NULL)
			pCtrl->SetVisible(false);
	}

	pLayout->Resize();
	pLayout->Invalidate();

}

void CommonInfoView::InitAndroidInfo( ResponseDeviceBaseInfoMsg* pMsg )
{
	if(pMsg == NULL)
		return;

	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
	CLayoutUI* pRightLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info_Right"));
	if(pLayout == NULL || pRightLayout == NULL)
		return;

	this->SetDeviceName(pMsg->deviceHWModel);

	this->SetItemInfo(pLayout, L"Info_ProductType", pMsg->deviceHWModel);
	this->SetItemInfo(pLayout, L"Info_ProductVersion", pMsg->deviceProductVersion);
	this->SetItemInfo(pLayout, L"Info_SerialNumber",	 pMsg->serialNumber);
	this->SetItemInfo(pRightLayout, L"Info_IMEI", pMsg->deviceIMEI);
	this->SetItemInfo(pRightLayout, L"Info_Mac", pMsg->macAddress);
	this->SetItemInfo(pRightLayout, L"Info_CoreVersion", pMsg->deviceCoreVersion);
	this->SetItemInfo(pRightLayout, L"Info_BaseVersion", pMsg->deviceBaseVersion);

	pLayout->Resize();
	pLayout->Invalidate();
	pRightLayout->Resize();
	pRightLayout->Invalidate();
}

void CommonInfoView::SetItemInfo( CLayoutUI* pLayout, wstring id, wstring strValue )
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

void CommonInfoView::SetDeviceName( wstring deviceHWModel )
{
	CLabelUI* pLabel = dynamic_cast<CLabelUI*>(GetItem(L"Label_DeviceName"));
	if(pLabel == NULL)
		return;

	pLabel->SetText(deviceHWModel.c_str());
	pLabel->SetAttribute(L"tooltip", deviceHWModel.c_str());
	pLabel->Resize();
	pLabel->Invalidate();
}

void CommonInfoView::UpdateAndroidSpace( AblSpaceInfoItem info )
{
	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Layout_Space"));
	if(pLayout == NULL)
		return;

	if(info.nLocation == eSpaceType_Location_Phone)
	{
		this->UpdateProgressBar(pLayout, L"Progress_Phone", info.nAll, info.nUsed);

		this->UpdateProgressLbl(pLayout, L"Phone_Left", info.nLeft);
	}
	else if(info.nLocation == eSpaceType_Location_SDCard)
	{
		this->UpdateProgressBar(pLayout, L"Progress_SDCard", info.nAll, info.nUsed);

		this->UpdateProgressLbl(pLayout, L"SDCard_Left", info.nLeft);
	}

	pLayout->Resize();
	pLayout->Invalidate();
}

void CommonInfoView::UpdateIOSSpace( AblSpaceInfoItem info )
{
	CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(GetItem(L"Layout_Space"));
	if(pLayout == NULL)
		return;

	this->UpdateProgressBar(pLayout, L"Progress_Sys", info.nSysAll, info.nSysUsed);

	this->UpdateProgressLbl(pLayout, L"Sys_Left", info.nSysLeft);

	this->UpdateProgressBar(pLayout, L"Progress_User", info.nUserAll, info.nUserUsed);

	this->UpdateProgressLbl(pLayout, L"User_Left", info.nUserLeft);
	
	pLayout->Resize();
	pLayout->Invalidate();
}

void CommonInfoView::UpdateProgressBar( CLayoutUI* pLayout, wstring id, __int64 maxValue, __int64 nValue )
{
	CProgressBarUI* pProgressUI = dynamic_cast<CProgressBarUI*>(pLayout->GetItem(id.c_str()));
	if(pProgressUI != NULL)
	{
		pProgressUI->SetRange(0, maxValue);
		pProgressUI->SetPos(nValue);
	}
}

void CommonInfoView::UpdateProgressLbl( CLayoutUI* pLayout, wstring id, __int64 nLeft )
{
	CLabelUI* pLabel = dynamic_cast<CLabelUI*>(pLayout->GetItem(id.c_str()));
	if(pLabel != NULL)
	{
		wstring strLeft = CommonUIFunc::LoadString(L"DeviceInfo_Text_Info_SpaceLeft");
		strLeft += CStrOperation::FormatFileSize(nLeft);
		pLabel->SetText(strLeft.c_str());
		pLabel->Resize();
		pLabel->Invalidate();
	}
}

void CommonInfoView::UpdatePowerLbl( CLayoutUI* pLayout, int nPower )
{
	CLabelUI* pLabel = dynamic_cast<CLabelUI*>(pLayout->GetItem(L"Power_Left"));
	if(pLabel != NULL)
	{
		nPower = nPower>100?100:nPower;
		wstring strPower = CommonUIFunc::LoadString(L"DeviceInfo_Text_Info_SpaceLeft");
		strPower += CStrOperation::IntToWString(nPower);
		strPower += L"%";
		pLabel->SetText(strPower.c_str());
		pLabel->Resize();
		pLabel->Invalidate();
	}
}

std::wstring CommonInfoView::CreateInfoString()
{
	RequestDeviceTypeMsg msg;
	msg.deviceID = this->_deviceID;
	msg.SendAndGetReturn();

	wstring infoText;
	if(msg.deviceType == ePhoneOS_Android)
	{
		CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
		if(pLayout != NULL)
		{
			infoText += this->GetItemInfo(pLayout, L"Info_ProductType");
			infoText += this->GetItemInfo(pLayout, L"Info_ProductVersion");
			infoText += this->GetItemInfo(pLayout, L"Info_SerialNumber");
			infoText += this->GetItemInfo(pLayout, L"Info_IsRoot");
		}
		pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info_Right"));
		if(pLayout != NULL)
		{
			infoText += this->GetItemInfo(pLayout, L"Info_IMEI");
			infoText += this->GetItemInfo(pLayout, L"Info_Mac");
			infoText += this->GetItemInfo(pLayout, L"Info_CoreVersion");
			infoText += this->GetItemInfo(pLayout, L"Info_BaseVersion");
		}
	}
	else
	{
		CLayoutUI* pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info"));
		if(pLayout != NULL)
		{
			infoText += this->GetItemInfo(pLayout, L"Info_ProductType");
			infoText += this->GetItemInfo(pLayout, L"Info_ProductVersion");
			infoText += this->GetItemInfo(pLayout, L"Info_SerialNumber");
			infoText += this->GetItemInfo(pLayout, L"Info_IsActive");
			infoText += this->GetItemInfo(pLayout, L"Info_IsJialbreak");
		}
		pLayout = dynamic_cast<CLayoutUI*>(this->GetItem(L"Layout_Info_Right"));
		if(pLayout != NULL)
		{
			infoText += this->GetItemInfo(pLayout, L"Info_Area");
			infoText += this->GetItemInfo(pLayout, L"Info_UUID");
			infoText += this->GetItemInfo(pLayout, L"Info_IMEI");
			infoText += this->GetItemInfo(pLayout, L"Info_BaseVersion");
		}
	}
	return infoText;
}

wstring CommonInfoView::GetItemInfo( CLayoutUI* pLayout, wstring id )
{
	wstring str = L"";
	CControlUI* pCtrl = pLayout->GetItem(id.c_str());
	if(pCtrl != NULL && pCtrl->IsVisible())
	{
		CControlUI* pTitle = pCtrl->GetItem(L"title");
		CEditUI* pEdit = dynamic_cast<CEditUI*>(pCtrl->GetItem(L"value"));
		if(pTitle != NULL || pEdit)
		{
			str = pTitle->GetText();
			str += L"    ";
			str += pEdit->GetText();
			str += L"\r\n";
			str += L"\r\n";
		}
	}
	return str;
}

void CommonInfoView::LoadSpaceAndPower()
{
	RequestDeviceSpaceMsg spaceMsg;
	spaceMsg.deviceID = this->_deviceID;
	spaceMsg.Send();

	RequestDevicePowerInfoMsg powerMsg;
	powerMsg.deviceID = this->_deviceID;
	powerMsg.Send();
}
