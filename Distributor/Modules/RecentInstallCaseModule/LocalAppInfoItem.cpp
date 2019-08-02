#include "stdafx.h"
#include "LocalAppInfoItem.h"
#include "UI/LocalAppinfoList.h"

RENCENT_INSTALLCASE_BEGIN

CSimpleListItemUI* LocalAppInfoItem::CreateAppInfoItem( InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis,CCompositeUI *compui,bool bupdate,bool busercase, bool bsystemcase,int isSelect, bool insertToFront, vector<wstring>* upidlist)
{INTERFUN;
	CSimpleListItemUI*   containerItem = new CSimpleListItemUI();
	
	CAutoShowTipLabelUI*           plabelVersion = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileSize = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileType = new CAutoShowTipLabelUI();
	CButtonUI*          pbuttoninstall = new CButtonUI();
	CButtonUI*          pbuttonRemove = new CButtonUI();

	CControlUI* pcontainerItem = (CControlUI*)containerItem;
	if(!insertToFront)
		pAppInfoLis->GetAppList()->Add(pcontainerItem);
	else
	{
		CControlUI* first = NULL;
		int index = 0;
		int count = pAppInfoLis->GetAppList()->GetCount();

		if(count > 0)
		{
			while (index < count && pAppInfoLis->GetAppList()->IsCheckItem(index))
			{
				first = pAppInfoLis->GetAppList()->GetItem(index);
				index++;
			}

			pAppInfoLis->GetAppList()->Insert(pcontainerItem, first);
			count = pAppInfoLis->GetAppList()->GetCount();
		}
		else
		{
			pAppInfoLis->GetAppList()->Add(pcontainerItem);
		}

		if(insertToFront)
		{		
			pAppInfoLis->GetAppList()->CheckItem(true, index);
		}
	}

	if(!insertToFront)
	{
		if(isSelect>-1)
			pAppInfoLis->GetAppList()->CheckItem(isSelect==0?false:true,pAppInfoLis->GetAppList()->GetCount()-1);
		else
			pAppInfoLis->GetAppList()->CheckItem(appinfo->m_bSelected,pAppInfoLis->GetAppList()->GetCount()-1);
	}

	if(pAppInfoLis->GetAppList()->IsCheckItem(pcontainerItem->GetIndex()))
		pcontainerItem->Select(true, false);
	else
		pcontainerItem->Select(false, false);

	CContainerUI*  pContainer = new CContainerUI();
	pContainer->SetId(L"LabContainer");
		 pContainer->SetInitPos(7,0,0,0,8);
	pcontainerItem->Add(pContainer);

	CControlUI* pControl = new CControlUI();
	pControl->SetId(_T("AppImg"));
	pControl->SetInitPos(2,5,32,5,7);
	pControl->SetStyle(_T("Picture"));
	if(CFileOperation::IsFileExist(appinfo->m_Iconpath.c_str()))
	{
		pControl->SetAttribute(_T("bk.image"), appinfo->m_Iconpath.c_str());		
	}
	else
		pControl->SetAttribute(_T("bk.image"), _T("#InstallCase_Image_Appicon_Default_32"));
	pControl->SetText(appinfo->m_Iconpath.c_str());
	pControl->SetAttribute(_T("bk.paintmode"),_T("center"));
	pContainer->Add(pControl);

	if(appinfo->m_isLegalSoft && appinfo->m_strFileType.compare(L"ipa") == 0)
	{
		CControlUI* pLegalControl = new CControlUI();
		pLegalControl->SetId(_T("LegalImg"));
		pLegalControl->SetInitPos(22,0,38,16,0);
		pLegalControl->SetStyle(_T("Picture"));
		pLegalControl->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_LegalSoftIcon"));
		pLegalControl->SetAttribute(_T("bk.paintmode"),_T("center"));
		pContainer->Add(pLegalControl);
	}

	CAutoShowTipLabelUI*           plabelAppName = new CAutoShowTipLabelUI();
	CControlUI* pLabelAppName = (CControlUI*)plabelAppName;		
	pLabelAppName->SetId(_T("LabelAppName"));
	pLabelAppName->SetStyle(_T("Label"));
	wstring text = appinfo->m_strName;
	if(appinfo->m_id < 0)
		text += L"【未下载】";
	pLabelAppName->SetText(text.c_str());
	pLabelAppName->SetAttribute(_T("txt.autosize"),_T("1"));
	pLabelAppName->SetAttribute(_T("txt.endellipsis"),_T("1"));
	if(appinfo->m_Purchaser.length()>0)
		pLabelAppName->SetInitPos(40,5,8,20,4);
	else
		pLabelAppName->SetInitPos(40,0,0,40,4);
	pContainer->Add(plabelAppName);

	if(appinfo->m_Purchaser.length()>0)
	{
		CAutoShowTipLabelUI*           plabelPurchaser = new CAutoShowTipLabelUI();
		CControlUI* pLabelpPurchaser = (CControlUI*)plabelPurchaser;		
		pLabelpPurchaser->SetId(_T("LabelAppName"));
		pLabelpPurchaser->SetStyle(_T("Label"));
		pLabelpPurchaser->SetText(appinfo->m_Purchaser.c_str());
		pLabelpPurchaser->SetInitPos(40,20,0,35,4);
		pLabelpPurchaser->SetAttribute(L"txt.textcolor",L"666666");
		pLabelpPurchaser->SetAttribute(_T("txt.autosize"),_T("1"));
		pLabelpPurchaser->SetAttribute(_T("txt.endellipsis"),_T("1"));
		pContainer->Add(plabelPurchaser);
	}

	CContainerUI*  pVersionContainer = new CContainerUI();
	pVersionContainer->SetId(L"VersionContainer");
	pVersionContainer->SetInitPos(0,0,0,0,8);
	pcontainerItem->Add(pVersionContainer);

	plabelVersion->SetId(_T("LabelAppVer"));
	plabelVersion->SetStyle(_T("Label"));
	plabelVersion->SetAttribute(_T("txt.endellipsis"),_T("1"));
	if( appinfo->m_bUpdateable==INSTALLS_CASE_UPDATEABLE_YES)
	{
		plabelVersion->SetInitPos(7,0,51,40,4);
	}
	else
	{
		plabelVersion->SetInitPos(7,0,20,40,4);
	}
	if(bupdate)
	{
		plabelVersion->SetText(appinfo->m_strNewVersion.c_str());
	}
	else
	{
		plabelVersion->SetText(appinfo->m_strVersion.c_str());
	}
	pVersionContainer->Add(plabelVersion);

	if( appinfo->m_bUpdateable==INSTALLS_CASE_UPDATEABLE_YES)
	{
		CButtonUI* btnup=new CButtonUI();
		btnup->SetId(_T("UpgradeBtn"));
		btnup->SetStyle(_T("ThreeBack_Button"));
		btnup->SetAttribute(L"bk.image",_T("#RecentInstallCase_Image_piracy_upgrade"));
		btnup->SetAttribute(_T("pos"),_T("5|50,10,39,14"));
		btnup->SetAttribute(_T("cursor"),_T("hand"));
		btnup->SetToolTipShow(true);
		wstring tooltip = CommonUIFunc::LoadStringW(_T("RecentInstallCase_Text_UpgradeTo")) + appinfo->m_strNewVersion;
		btnup->SetToolTip(tooltip.c_str());
		btnup->SetINotifyUI(compui);
		pVersionContainer->Add(btnup);
	}

	if( appinfo->m_bLegalUpdateable==INSTALLS_CASE_LEGALUPDATEABLE_YES)
	{
		CButtonUI* btnlegalup=new CButtonUI();
		btnlegalup->SetId(_T("LegalUpgradeBtn"));
		btnlegalup->SetStyle(_T("ThreeBack_Button"));
		btnlegalup->SetAttribute(L"bk.image",_T("#RecentInstallCase_Image_legal_upgrade"));
		btnlegalup->SetAttribute(_T("pos"),_T("5|34, 9, 9, 13"));
		btnlegalup->SetAttribute(_T("cursor"),_T("hand"));
		btnlegalup->SetToolTipShow(true);
		wstring tooltip = CommonUIFunc::LoadStringW(_T("RecentInstallCase_Text_UpgradeTo")) + appinfo->m_strNewLegalVersion;
		btnlegalup->SetToolTip(tooltip.c_str());
		btnlegalup->SetINotifyUI(compui);
		pVersionContainer->Add(btnlegalup);
	}


	plabelFileSize->SetId(_T("LabelAppSize"));
	plabelFileSize->SetStyle(_T("Label"));
	plabelFileSize->SetText(CStrOperation::FormatFileSize((int64_t)appinfo->m_FileSize).c_str());
	plabelFileSize->SetUserData(appinfo->m_FileSize);
	plabelFileSize->SetInitPos(7,0,100,40,0);
	((CControlUI*)plabelFileSize)->SetAttribute(_T("txt.halign"),_T("left"));
	pcontainerItem->Add(plabelFileSize);

	CContainerUI*  pSDKVerContainer = new CContainerUI();
	pSDKVerContainer->SetInitPos(0,0,130,40,0);
	pSDKVerContainer->SetId(L"SDKVerContainer");
	CControlUI* pSDKImage = new CControlUI();
	pSDKImage->SetStyle(_T("Picture"));
	wstring fileType = CStrOperation::toLowerW(appinfo->m_strFileType);
	if (fileType.find(L"ipa")!= wstring::npos || fileType.find(L"pxl")!= wstring::npos)
	{
		wstring supposeDevice = CStrOperation::toLowerW(appinfo->m_SupportDevice);
		wstring imageId = L"#RecentInstallCase_Image_app_iphone";
		bool isSupposeiPad = false;
		bool isSupposeiPhone = false;
		if (supposeDevice.find(L"ipad")!= wstring::npos)
		{
			isSupposeiPad = true;
		}

		if (supposeDevice.find(L"iphone")!=wstring::npos || supposeDevice.find(L"ipod")!=wstring::npos)
		{
			isSupposeiPhone = true;
		}

		if (isSupposeiPhone&&isSupposeiPad)
		{
			imageId = L"#RecentInstallCase_Image_app_ipadiphone";
		}
		else if (isSupposeiPhone)
		{
			imageId = L"#RecentInstallCase_Image_app_iphone";
		}
		else if (isSupposeiPad)
		{
			imageId = L"#RecentInstallCase_Image_app_ipad";
		}
		pSDKImage->SetAttribute(_T("bk.image"), imageId.c_str());
	}
	else
	{
		pSDKImage->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_app_android"));
	}
	
	pSDKImage->SetInitPos(3,7,25,29,0);
	pSDKVerContainer->Add(pSDKImage);

	plabelFileType->SetId(_T("LabelAppType"));
	plabelFileType->SetStyle(_T("Label"));
	//by chenxy List文件格式列改为适用设备
	if (appinfo->m_SDKVersion == L"")
	{
		plabelFileType->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_UnKnow"));
	}
	else
	{
		wstring wsSdkVerTmp = CStrOperation::toLowerW(appinfo->m_SDKVersion);
		wsSdkVerTmp = CStrOperation::ReplaceW(wsSdkVerTmp,L"android",L"");
		plabelFileType->SetText(wsSdkVerTmp.c_str());
	}
	
	//plabelFileType->SetText(appinfo->m_strFileType.c_str());
	plabelFileType->SetInitPos(28,0,100,40,0);
	((CControlUI*)plabelFileType)->SetAttribute(_T("txt.halign"),_T("left"));
	pSDKVerContainer->Add(plabelFileType);

	pcontainerItem->Add(pSDKVerContainer);
	
	CAutoShowTipLabelUI* donwloadtime = new CAutoShowTipLabelUI();
	donwloadtime->SetId(_T("LabelDownloadTime"));
	donwloadtime->SetStyle(_T("Label"));
	donwloadtime->SetText(UIStringFormat::DateStringYearMonDay(appinfo->m_tDownloadTime).c_str());
	donwloadtime->SetUserData((UINT)appinfo->m_tDownloadTime);
	donwloadtime->SetInitPos(7,0,100,40,0);
	((CControlUI*)donwloadtime)->SetAttribute(_T("txt.halign"),_T("left"));
	pcontainerItem->Add(donwloadtime);

	if(bupdate)
	{
		CContainerUI*	pcontainerBtn =  new CContainerUI();
		pcontainerBtn->SetId(_T("ContainerBtn"));
		pcontainerBtn->SetVisible(false);
		pcontainerBtn->SetInitPos(10,8,150,32,0);
		pcontainerItem->Add(pcontainerBtn);

		bool update=false;
		for (vector<wstring>::iterator it=(*upidlist).begin(); it != (*upidlist).end(); it++)
		{
			if (*it == appinfo->m_strSavePath)
			{
				update = true;
				break;
			}
		}
		pbuttoninstall->SetId(_T("CaseFormBtnUpdate"));
		pbuttoninstall->SetInitPos(0,0,60,23,0);
		pbuttoninstall->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
		if(update)
		{
			pbuttoninstall->SetEnabled(false);
			pbuttoninstall->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_UpGrading"));
		}
		else
			pbuttoninstall->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_UpGrade"));
		pbuttoninstall->SetINotifyUI(compui);
		pcontainerBtn->Add(pbuttoninstall);
	}
	else
	{
		CContainerUI*	pcontainerBtn =  new CContainerUI();
		pcontainerBtn->SetVisible(false);
		pcontainerBtn->SetId(_T("ContainerBtn"));
		pcontainerBtn->SetInitPos(7,0,150,40,0);
		pcontainerItem->Add(pcontainerBtn);

		pbuttoninstall->SetId(_T("CaseFormBtnInstall"));
		pbuttoninstall->SetInitPos(0,8,60,31,0);
		pbuttoninstall->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
		pbuttoninstall->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Install"));
		pbuttoninstall->SetINotifyUI(compui);
		pcontainerBtn->Add(pbuttoninstall);

		pbuttonRemove->SetId(_T("CaseFormBtnRemove"));
		pbuttonRemove->SetInitPos(70,8,130,31,0);
		pbuttonRemove->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
		pbuttonRemove->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Delete"));
		if (busercase)
		{
			pbuttonRemove->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Remove"));
		}
		pbuttonRemove->SetINotifyUI(compui);
		pcontainerBtn->Add(pbuttonRemove);
	}
	containerItem->SetId(appinfo->m_strSavePath.c_str());

	containerItem->SetInitPos(0,0,0,40,5);
	containerItem->SetUserData((UINT)appinfo);
	containerItem->AddEventDelegate(&MakeDelegate(pAppInfoLis, &LocalAppinfoList::ListContainerItemEvent));

	SetToolTipText(appinfo, containerItem);

	return containerItem;
}

void LocalAppInfoItem::UpdateAppInfoItem(InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis, int isSelect)
{INTERFUN;
	if(pAppInfoLis == NULL || pAppInfoLis->GetAppList() == NULL)
		return;

	CListContainerItemUI* item = (CListContainerItemUI*)pAppInfoLis->GetAppList()->GetItem(appinfo->m_strSavePath.c_str());
	if(item == NULL)
		return;

	item->SetUserData((UINT)appinfo);
	item->Select(isSelect != 0, false);

	CContainerUI*  pContainer = (CContainerUI*)item->GetItem(0);

	int index = 0;
	CControlUI* pImgControl = pContainer->GetItem(0);
	if(CFileOperation::IsFileExist(appinfo->m_Iconpath.c_str()))
	{
		pImgControl->SetAttribute(_T("bk.image"), appinfo->m_Iconpath.c_str());		
	}
	else
		pImgControl->SetAttribute(_T("bk.image"), _T("#InstallCase_Image_Appicon_Default_32"));

	if(appinfo->m_isLegalSoft && appinfo->m_strFileType.compare(L"ipa") == 0)
	{
		index++;
		CControlUI* pLegalControl = pContainer->GetItem(index);
		pLegalControl->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_LegalSoftIcon"));
	}

	index++;
	CControlUI* pLabelAppName = pContainer->GetItem(index);
	wstring text = appinfo->m_strName;
	if(appinfo->m_id < 0)
		text += L"【未下载】";
	pLabelAppName->SetText(text.c_str());

	if(appinfo->m_Purchaser.length()>0)
	{
		index++;
		CAutoShowTipLabelUI* plabelPurchaser = (CAutoShowTipLabelUI*)pContainer->GetItem(index);
		plabelPurchaser->SetText(appinfo->m_Purchaser.c_str());
	}
	
	CAutoShowTipLabelUI* plabelFileSize =(CAutoShowTipLabelUI*)item->GetItem(2);
	CAutoShowTipLabelUI* plabelFileType =(CAutoShowTipLabelUI*)item->GetItem(3);
	CAutoShowTipLabelUI* plabelDownTime =(CAutoShowTipLabelUI*)item->GetItem(5);

	plabelFileSize->SetText(CStrOperation::FormatFileSize((int64_t)appinfo->m_FileSize).c_str());

	plabelFileType->SetText(appinfo->m_SDKVersion.c_str());
	plabelDownTime->SetText(UIStringFormat::DateStringYearMonDay(appinfo->m_tDownloadTime).c_str());

	plabelFileSize->SetUserData(appinfo->m_FileSize);
	plabelDownTime->SetUserData((UINT)appinfo->m_tDownloadTime);

	SetToolTipText(appinfo, item);
}


void LocalAppInfoItem::UpdateAppInfoItemUpgradeState( InstallsAppInfo * appinfo,LocalAppinfoList* pAppInfoLis,CCompositeUI *compui )
{
	CListContainerItemUI* item = (CListContainerItemUI*)pAppInfoLis->GetAppList()->GetItem(appinfo->m_strSavePath.c_str());
	if(item == NULL)
		return;
	CContainerUI*  pVersionContainer = (CContainerUI*)item->GetItem(1);
	CButtonUI* btnup = (CButtonUI*)pVersionContainer->GetItem(_T("UpgradeBtn"));
	CButtonUI* btnlegalup = (CButtonUI*)pVersionContainer->GetItem(_T("LegalUpgradeBtn"));
	if( appinfo->m_bUpdateable==INSTALLS_CASE_UPDATEABLE_YES)
	{
		if(btnup)
			btnup->SetEnabled(true);
		else
		{
			btnup=new CButtonUI();
			btnup->SetId(_T("UpgradeBtn"));
			btnup->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
			btnup->SetAttribute(L"bk.image",_T("#RecentInstallCase_Image_piracy_upgrade"));
			btnup->SetAttribute(_T("pos"),_T("5|50,10,39,14"));
			btnup->SetAttribute(_T("cursor"),_T("hand"));
			btnup->SetToolTipShow(true);
			wstring tooltip = CommonUIFunc::LoadStringW(_T("RecentInstallCase_Text_UpgradeTo")) + appinfo->m_strNewVersion;
			btnup->SetToolTip(tooltip.c_str());
			btnup->SetINotifyUI(compui);
			pVersionContainer->Add(btnup);
		}
	}

	if( appinfo->m_bLegalUpdateable==INSTALLS_CASE_LEGALUPDATEABLE_YES)
	{
		if(btnlegalup)
			btnlegalup->SetEnabled(true);
		else
		{
			btnlegalup=new CButtonUI();
			btnlegalup->SetId(_T("LegalUpgradeBtn"));
			btnlegalup->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
			btnlegalup->SetAttribute(L"bk.image",_T("#RecentInstallCase_Image_legal_upgrade"));
			btnlegalup->SetAttribute(_T("pos"),_T("5|34, 9, 9, 13"));
			btnlegalup->SetAttribute(_T("cursor"),_T("hand"));
			btnlegalup->SetToolTipShow(true);
			wstring tooltip = CommonUIFunc::LoadStringW(_T("RecentInstallCase_Text_UpgradeTo")) + appinfo->m_strNewLegalVersion;
			btnlegalup->SetToolTip(tooltip.c_str());
			btnlegalup->SetINotifyUI(compui);
			pVersionContainer->Add(btnlegalup);
		}
	}
}
void LocalAppInfoItem::SetToolTipText(InstallsAppInfo * appinfo, CListContainerItemUI* item)
{INTERFUN;
	wstring support;
	if(appinfo->m_strFileType == L"ipa" || appinfo->m_strFileType == L"pxl")
	{
		if(appinfo->m_SupportDevice.empty())
		{
			support = L"IOS";
		}
		else
			support = appinfo->m_SupportDevice;
	}
	else
	{
		support = L"Android";
	}

	wstring toolTipText;
	toolTipText += L"名    称：";
	toolTipText += appinfo->m_strName;
	toolTipText += L"\r\n版    本：";
	toolTipText += appinfo->m_strVersion;
	toolTipText += L"\r\n大    小：";
	toolTipText += CStrOperation::FormatFileSize((int64_t)appinfo->m_FileSize);
	toolTipText += L"\r\n软件类型：";
	toolTipText += appinfo->m_strFileType;
	toolTipText += L"\r\n支持平台：";
	toolTipText += support.c_str();
	toolTipText += L"\r\n下载时间：";
	if(appinfo->m_tDownloadTime > 0)
		toolTipText += UIStringFormat::DateStringYearMonDay(appinfo->m_tDownloadTime);
	else 
		toolTipText +=  L"未知";
	toolTipText += L"\r\n软件路径：";
	if((int)appinfo->m_strSavePath.find(L"http:")>-1)
	{
		toolTipText += L"还未下载";
	}
	else
	{
		toolTipText += appinfo->m_strSavePath;
	}
	item->SetToolTipShow(true);
	item->SetToolTip(toolTipText.c_str());
}

CSimpleListItemUI* LocalAppInfoItem::CreateCaseItem( InstallsCaseData * casedata,LocalAppinfoList* pAppInfoList,CCompositeUI *compui)
{INTERFUN;
	CSimpleListItemUI*   containerItem = new CSimpleListItemUI();
	CControlUI* pcontainerItem = (CControlUI*)containerItem;

	CAutoShowTipLabelUI*           plabelCaseName = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelInstallCount = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileCount = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabeltotalsize = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelneedtime = new CAutoShowTipLabelUI();
	CButtonUI*          pbuttoninstall = new CButtonUI();
	CButtonUI*          pbuttonRemove = new CButtonUI();

	pAppInfoList->GetAppList()->Add(pcontainerItem);

	CContainerUI*	pcontainerName =  new CContainerUI();
	pcontainerName->SetId(L"LabContainer");
	pcontainerName->SetInitPos(7,0,0,40,4);
	pcontainerItem->Add(pcontainerName);

	CControlUI* pControl = new CControlUI();
	pControl->SetId(_T("FolderImg"));
	pControl->SetInitPos(0,12,16,28,0);
	pControl->SetStyle(_T("Picture"));
	if(casedata->CaseType() == _INSTALLS_CASE_DATA_IOS)
		pControl->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_icon_Apple"));
	else
		pControl->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_icon_Android"));
	pcontainerName->Add(pControl);

	plabelCaseName->SetId(_T("LabelCaseName"));
	plabelCaseName->SetStyle(_T("Label"));
	plabelCaseName->SetText(casedata->CaseName().c_str());

	plabelCaseName->SetAttribute(_T("txt.endellipsis"),_T("1"));
	plabelCaseName->SetInitPos(17,0,0,40,4);
	pcontainerName->Add(plabelCaseName);

	plabelInstallCount->SetId(_T("LabelInstallCount"));
	plabelInstallCount->SetStyle(_T("Label"));
	plabelInstallCount->SetText(CStrOperation::IntToWString(casedata->InstallCount()).c_str());
	plabelInstallCount->SetUserData(casedata->InstallCount());
	plabelInstallCount->SetInitPos(7,0,60,40,0);
	pcontainerItem->Add(plabelInstallCount);

	plabelFileCount->SetId(_T("LabelFileCount"));
	plabelFileCount->SetStyle(_T("Label"));
	plabelFileCount->SetText(CStrOperation::IntToWString(casedata->ListApp()->size()).c_str());
	plabelFileCount->SetUserData(casedata->ListApp()->size());
	plabelFileCount->SetInitPos(7,0,60,40,0);
	pcontainerItem->Add(plabelFileCount);

	plabeltotalsize->SetId(_T("LabelTotalSize"));
	plabeltotalsize->SetStyle(_T("Label"));
	plabeltotalsize->SetText(CStrOperation::FormatFileSize(casedata->GetSumFileSize(),1).c_str());
	plabeltotalsize->SetUserData(casedata->GetSumFileSize());
	plabeltotalsize->SetInitPos(7,0,100,40,0);
	pcontainerItem->Add(plabeltotalsize);

	plabelneedtime->SetId(_T("Labelneedtime"));
	plabelneedtime->SetStyle(_T("Label"));
	int num=0;
	plabelneedtime->SetText(UIStringFormat::InstallNeedTime(casedata->ListApp()->size(),casedata->GetSumFileSize(),num).c_str());
	plabelneedtime->SetInitPos(7,0,100,40,0);
	pcontainerItem->Add(plabelneedtime);

	CContainerUI*	pcontainerBtn =  new CContainerUI();
	pcontainerBtn->SetVisible(false);
	pcontainerBtn->SetId(_T("ContainerBtn"));
	pcontainerBtn->SetInitPos(7,0,150,40,0);
	pcontainerItem->Add(pcontainerBtn);

	pbuttoninstall->SetId(_T("CaseFormBtnShow"));
	pbuttoninstall->SetInitPos(0,8,60,31,0);
	pbuttoninstall->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
	pbuttoninstall->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Show"));
	pbuttoninstall->SetINotifyUI(compui);
	pcontainerBtn->Add(pbuttoninstall);

	pbuttonRemove->SetId(_T("CaseFormBtnDeleteCase"));
	pbuttonRemove->SetInitPos(70,8,130,31,0);
	pbuttonRemove->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
	pbuttonRemove->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Delete"));
	pbuttonRemove->SetINotifyUI(compui);
	pcontainerBtn->Add(pbuttonRemove);
	
	containerItem->SetId(CStrOperation::IntToWString(casedata->Id()).c_str());
	//containerItem->SetINotifyUI(compui);
	containerItem->SetInitPos(0,0,0,40,5);
	containerItem->AddEventDelegate(&MakeDelegate(pAppInfoList, &LocalAppinfoList::ListContainerItemEvent));
	return containerItem;
}

void LocalAppInfoItem::UpdateCaseItem(InstallsCaseData* casedata, LocalAppinfoList* pAppInfoList)
{INTERFUN;
	if(pAppInfoList == NULL || pAppInfoList->GetAppList() == NULL)
		return;

	CListContainerItemUI* item = (CListContainerItemUI*)pAppInfoList->GetAppList()->GetItem(CStrOperation::IntToWString(casedata->Id()).c_str());
	if(item == NULL)
		return;
	CAutoShowTipLabelUI* plabelCaseName = (CAutoShowTipLabelUI*)((CContainerUI*)item->GetItem(0))->GetItem(1);
	if(plabelCaseName != NULL)
	{
		plabelCaseName->SetText(casedata->CaseName().c_str());
		wstring name = casedata->CaseName();
	}

	CAutoShowTipLabelUI* plabelInstallCount = (CAutoShowTipLabelUI*)item->GetItem(1);
	if(plabelInstallCount != NULL)
	{
		plabelInstallCount->SetText(CStrOperation::IntToWString(casedata->InstallCount()).c_str());
		plabelInstallCount->SetUserData(casedata->InstallCount());
	}

	CAutoShowTipLabelUI* plabelFileCount = (CAutoShowTipLabelUI*)item->GetItem(2);
	if(plabelFileCount != NULL)
	{
		plabelFileCount->SetText(CStrOperation::IntToWString(casedata->GetAppNum()).c_str());
		plabelFileCount->SetUserData(casedata->GetAppNum());
	}

	CAutoShowTipLabelUI* plabeltotalsize = (CAutoShowTipLabelUI*)item->GetItem(3);
	if(plabeltotalsize != NULL)
	{
		plabeltotalsize->SetText(CStrOperation::FormatFileSize(casedata->GetSumFileSize(),1).c_str());
		plabeltotalsize->SetUserData(casedata->GetSumFileSize());
	}

	int num=0;
	CAutoShowTipLabelUI* plabelneedtime = (CAutoShowTipLabelUI*)item->GetItem(4);
	if(plabelneedtime != NULL)
		plabelneedtime->SetText(UIStringFormat::InstallNeedTime(casedata->ListApp()->size(),casedata->GetSumFileSize(),num).c_str());
}



CListContainerItemUI* LocalAppInfoItem::CreateFolderSumItem( AppCountByFolder * folderdata, int index,LocalAppinfoList* pAppInfoList,CCompositeUI *compui)
{INTERFUN;
	CListContainerItemUI*   containerItem = new CListContainerItemUI();
	CControlUI* pcontainerItem = (CControlUI*)containerItem;

	CAutoShowTipLabelUI*           plabelFolderName = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileCount = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileSize = new CAutoShowTipLabelUI();
	CAutoShowTipLabelUI*           plabelFileType = new CAutoShowTipLabelUI();
	CButtonUI*          pbuttonOpen = new CButtonUI();
	CButtonUI*          pbuttonDelete = new CButtonUI();

	CControlUI* pAfter = pAppInfoList->GetAppList()->GetItem(index);
	pAppInfoList->GetAppList()->Insert(pcontainerItem, pAfter);

	CContainerUI*	pcontainerName =  new CContainerUI();
	pcontainerName->SetId(_T("LabContainer"));

	pcontainerName->SetInitPos(7,0,0,40,4);
	pcontainerItem->Add(pcontainerName);

	CControlUI* pControl = new CControlUI();
	pControl->SetId(_T("FolderImg"));
	pControl->SetInitPos(0,11,20,28,0);
	pControl->SetStyle(_T("Picture"));
	pControl->SetAttribute(_T("bk.image"), _T("#RecentInstallCase_Image_FM_Folder"));
	pcontainerName->Add(pControl);

	wstring foldername = CStrOperation::trimRightW(folderdata->FolderName,L"\\");
	plabelFolderName->SetId(_T("LabelFolderName"));
	plabelFolderName->SetStyle(_T("Label"));

	bool bSys = false;
	if(folderdata->FolderName == CONFIG(_strAndroidDownPath) && folderdata->Filetype == _INSTALLS_CASE_DATA_ANDROID)
	{
		bSys = true;
		plabelFolderName->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_AndroidFolder"));
	}
	else if(folderdata->FolderName == CONFIG(_strIosDownPath) && folderdata->Filetype == _INSTALLS_CASE_DATA_IOS)
	{
		bSys = true;
		plabelFolderName->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_IosFolder"));
	}
	else
		plabelFolderName->SetText(CFileOperation::GetFileName(foldername).c_str());	
	plabelFolderName->SetInitPos(30,0,0,40,4);
	plabelFolderName->SetAttribute(_T("txt.autosize"),_T("1"));
	plabelFolderName->SetAttribute(_T("txt.endellipsis"),_T("1"));
	pcontainerName->Add(plabelFolderName);

	plabelFileCount->SetId(_T("LabelFolderCount"));
	plabelFileCount->SetStyle(_T("Label"));
	plabelFileCount->SetText(CStrOperation::IntToWString(folderdata->Filecount).c_str());
	plabelFileCount->SetUserData(folderdata->Filecount);
	plabelFileCount->SetInitPos(7,0,60,40,0);
	pcontainerItem->Add(plabelFileCount);

	plabelFileSize->SetId(_T("LabelFolderSize"));
	plabelFileSize->SetStyle(_T("Label"));
	plabelFileSize->SetText(CStrOperation::FormatFileSize(folderdata->TotalFileSize, 1).c_str());
	plabelFileSize->SetUserData(folderdata->TotalFileSize);
	plabelFileSize->SetInitPos(7,0,100,40,0);
	pcontainerItem->Add(plabelFileSize);

	wstring  Filetype = folderdata->Filetype==_INSTALLS_CASE_DATA_IOS?_T("IOS"):_T("Android");
	plabelFileType->SetId(_T("LabelFolderType"));
	plabelFileType->SetStyle(_T("Label"));
	plabelFileType->SetText(Filetype.c_str());
	plabelFileType->SetInitPos(7,0,60,40,0);
	pcontainerItem->Add(plabelFileType);

	CContainerUI*	pcontainerBtn =  new CContainerUI();
	pcontainerBtn->SetVisible(false);
	pcontainerBtn->SetId(_T("ContainerBtn"));
	pcontainerBtn->SetInitPos(7,0,150,40,0);
	pcontainerItem->Add(pcontainerBtn);

	pbuttonOpen->SetId(_T("CaseFormBtnOpen"));
	pbuttonOpen->SetInitPos(0,8,60,31,0);
	pbuttonOpen->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
	pbuttonOpen->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Open"));
	pbuttonOpen->SetINotifyUI(compui);
	pcontainerBtn->Add(pbuttonOpen);

	if(!bSys)
	{
		pbuttonDelete->SetId(_T("CaseFormBtnDeleteFolder"));
		pbuttonDelete->SetInitPos(70,8,130,31,0);
		pbuttonDelete->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
		pbuttonDelete->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Delete"));
		pbuttonDelete->SetINotifyUI(compui);
		pcontainerBtn->Add(pbuttonDelete);
	}
	
	containerItem->SetId(CStrOperation::IntToWString(folderdata->m_id).c_str());
	//containerItem->SetINotifyUI(compui);
	containerItem->SetUserData((UINT)folderdata);
	containerItem->SetInitPos(0,0,0,40,5);
	containerItem->AddEventDelegate(&MakeDelegate(pAppInfoList, &LocalAppinfoList::ListContainerItemEvent));
	return containerItem;
}

void LocalAppInfoItem::UpdateFolderSumItem( AppCountByFolder* appFolder, LocalAppinfoList* pAppInfoList,CCompositeUI *compui)
{INTERFUN;
	if(pAppInfoList == NULL || pAppInfoList->GetAppList() == NULL)
		return;

	CListContainerItemUI* item = (CListContainerItemUI*)pAppInfoList->GetAppList()->GetItem(CStrOperation::IntToWString(appFolder->m_id).c_str());
	if(item == NULL)
		return;

	item->SetUserData((UINT)appFolder);

	CContainerUI* pcontainerName = (CContainerUI*)item->GetItem(0);
	CAutoShowTipLabelUI* plabelFolderName = (CAutoShowTipLabelUI*)pcontainerName->GetItem(1);

	wstring foldername = CStrOperation::trimRightW(appFolder->FolderName,L"\\");
	plabelFolderName->SetId(_T("LabelFolderName"));
	plabelFolderName->SetStyle(_T("Label"));

	bool bSys = false;
	if(appFolder->FolderName == CONFIG(_strAndroidDownPath) && appFolder->Filetype == _INSTALLS_CASE_DATA_ANDROID)
	{
		bSys = true;
		plabelFolderName->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_AndroidFolder"));
	}
	else if(appFolder->FolderName == CONFIG(_strIosDownPath) && appFolder->Filetype == _INSTALLS_CASE_DATA_IOS)
	{
		bSys = true;
		plabelFolderName->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_IosFolder"));
	}
	else
		plabelFolderName->SetText(CFileOperation::GetFileName(foldername).c_str());	


	CAutoShowTipLabelUI* plabelInstallCount = (CAutoShowTipLabelUI*)item->GetItem(1);
	if(plabelInstallCount == NULL)
		return;
	plabelInstallCount->SetText(CStrOperation::IntToWString(appFolder->Filecount).c_str());
	plabelInstallCount->SetUserData(appFolder->Filecount);

	CAutoShowTipLabelUI* plabelFileSize = (CAutoShowTipLabelUI*)item->GetItem(2);
	plabelFileSize->SetText(CStrOperation::FormatFileSize(appFolder->TotalFileSize, 1).c_str());
	plabelFileSize->SetUserData(appFolder->TotalFileSize);

	CContainerUI* pcontainerBtn = (CContainerUI*)item->GetItem(4);
	if(!bSys && pcontainerBtn->GetCount() < 2)
	{
		CButtonUI* pbuttonDelete = new CButtonUI();
		pbuttonDelete->SetId(_T("CaseFormBtnDeleteFolder"));
		pbuttonDelete->SetInitPos(70,8,130,31,0);
		pbuttonDelete->SetStyle(_T("RecentInstallCase_Stytle_ThreeBack_Button"));
		pbuttonDelete->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Delete"));
		pbuttonDelete->SetINotifyUI(compui);
		pcontainerBtn->Add(pbuttonDelete);
	}
	else if(bSys && pcontainerBtn->GetCount() == 2)
	{
		CButtonUI* pbuttonDelete = (CButtonUI*)pcontainerBtn->GetItem(1);
		pcontainerBtn->Remove(pbuttonDelete);
	}
}

RENCENT_INSTALLCASE_END