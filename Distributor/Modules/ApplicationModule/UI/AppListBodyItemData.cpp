#include "stdafx.h"
#include "AppListBodyItemData.h"
#include "ApplicationInfoUI.h"
#include "Logic/AppDataUpgrade.h"
App_CC_BEGIN
AppListBodyItemData::AppListBodyItemData(CListUI* pList,
                                        const AppData* pApp,
                                        bool isAndroid,
                                        bool hasSDCard,
                                        ApplicationInfoUI* pUI)
{INTERFUN;
    InitPointers();
    _pList  = pList,
    _appId  = pApp->_packageName;
	_pApp = new AppData( pApp );
	_buttonPiracyUp = NULL;
	_buttonLegalUp = NULL;
		
    // new完后要先添加
    CControlUI* pContainerItem = CreateContainerItemUI();
    CControlUI* pContainer = CreateContainerUI(pContainerItem);
    CreateIconControlUI(pContainer, L"");
    CreateAppNameLabelUI(pContainer, pApp->_AppName, isAndroid);
    if (isAndroid && (pApp->_eAppType == APP_TYPE_APK || pApp->_eAppType == APP_TYPE_NPK ) )
        CreateAppPhonePathLabelUI(pContainer, pApp->_strAppPhonePath);

    wstring staticDiskUsage = pApp->_staticDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_staticDiskUsage);
    wstring dynamicDiskUsage = pApp->_dynamicDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_dynamicDiskUsage);

	CControlUI* pVersionContainer = CreateVersionContainerUI(pContainerItem);
    if (isAndroid)
    {
        wstring onSDCard = pApp->_bOnSDCard ? CommonUIFunc::LoadString(_T("Application_Text_APP_sdcart")):CommonUIFunc::LoadString(_T("Application_Text_APP_phone"));
        if (pApp->_eAppOwner == APP_OWNER_SYS)
            CreateVersionLabelUI(pVersionContainer, pApp->_strVersion);
		else
        {
			CreateVersionLabelUI(pVersionContainer, pApp->_strVersion);
			CreatePiracyUpBtnUI(pVersionContainer, pUI);
			CreateLegalUpBtnUI(pVersionContainer, pUI);
            CreateStaticDiskLabelUI(pContainerItem, staticDiskUsage);
            _labelStaticDiskUsage->SetUserData((UINT)pApp->_staticDiskUsage);
            CreateInstallPosLabelUI(pContainerItem, onSDCard);
        }
    }
    else
    {
		CreateVersionLabelUI(pVersionContainer, pApp->_strVersion);
		CreatePiracyUpBtnUI(pVersionContainer, pUI);
		CreateLegalUpBtnUI(pVersionContainer, pUI);
        CreateStaticDiskLabelUI(pContainerItem, staticDiskUsage);
        _labelStaticDiskUsage->SetUserData((UINT)pApp->_staticDiskUsage);
        CreateDynamicDiskLabelUI(pContainerItem, dynamicDiskUsage);
        _labelDynamicDiskUsage->SetUserData((UINT)pApp->_dynamicDiskUsage);
    }

    CControlUI* pContainer1 = CreateContainerUI1(pContainerItem);
    CreateUninstallButtonUI(pContainer1, pApp->_eAppOwner != APP_OWNER_SYS, isAndroid, hasSDCard, pUI);
    if (isAndroid && pApp->_eAppOwner != APP_OWNER_SYS && hasSDCard)
        CreateMoveToButtonUI(pContainer1, pApp->_bOnSDCard, pUI);

	SetToolTipText(pApp,isAndroid,pContainerItem);
}


void AppListBodyItemData::SetToolTipText(const AppData* pApp,bool isAndroid, CControlUI* item)
{
	wstring toolTipText;
	
	if (!item)
		return;
	toolTipText += _T("名称：");
	toolTipText += pApp->_AppName;
	toolTipText += _T("\r\n");

	toolTipText += _T("版本：");
	toolTipText += pApp->_strVersion;
	toolTipText += _T("\r\n");


	wstring staticDiskUsage = pApp->_staticDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_staticDiskUsage);
	wstring dynamicDiskUsage = pApp->_dynamicDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_dynamicDiskUsage);

 	if (isAndroid)		//安卓设备
	{
		if (pApp->_eAppOwner != APP_OWNER_SYS)		//不是系统应用
		{
			wstring onSDCard = pApp->_bOnSDCard ? CommonUIFunc::LoadString(_T("Application_Text_APP_sdcart")):CommonUIFunc::LoadString(_T("Application_Text_APP_phone"));

			toolTipText += _T("应用大小：");
			toolTipText += staticDiskUsage;
			toolTipText += _T("\r\n");
		
			toolTipText += _T("安装位置：");
			toolTipText += onSDCard;
			toolTipText += _T("\r\n");
		}	
		
		toolTipText += _T("路径：");
		toolTipText += pApp->_strAppPhonePath;

		
	}
	else
	{	//ios的应用大小与文档大小由后续读取，初始为  “未知”
		toolTipText += _T("应用大小：");
		toolTipText += staticDiskUsage;
		toolTipText += _T("\r\n");

		toolTipText += _T("文档大小：");
		toolTipText += dynamicDiskUsage;
	}
	


	item->SetToolTipShow(true);
	item->SetToolTip(toolTipText.c_str());
}


AppListBodyItemData::~AppListBodyItemData()
{INTERFUN;
    InitPointers();
	SAFE_DELETE( _pApp );
}

void AppListBodyItemData::InitPointers()
{INTERFUN;
    _pList                      = NULL;
    _containerItem              = NULL;
    _container                  = NULL;
    _control                    = NULL;
    _labelAppName               = NULL;
    _labelAppPhonePath          = NULL;
    _labelStaticDiskUsage       = NULL;
    _labelDynamicDiskUsage      = NULL;
    _labelVersion               = NULL;
    _labelInstallPos            = NULL;
    _container1                 = NULL;
    _buttonUninstall            = NULL;
    _buttonMoveTo               = NULL;
}

CControlUI* AppListBodyItemData::CreateContainerItemUI()
{INTERFUN;
    _containerItem = new CSimpleListItemUI();
    CControlUI* pContainerItem = (CControlUI*)_containerItem;
    pContainerItem->SetId(_appId.c_str());  
    pContainerItem->SetInitPos(0,0,0,45,4);
    _pList->Add(pContainerItem);
    return pContainerItem;
}

CControlUI* AppListBodyItemData::CreateContainerUI(CControlUI* pContainerItem)
{INTERFUN;
    _container = new CContainerUI();
    CControlUI* pContainer = (CControlUI*)_container;
    pContainer->SetId(L"Container");
    pContainer->SetInitPos(0,0,0,0,8);
    pContainerItem->Add(pContainer);
    return pContainer;
}

void AppListBodyItemData::CreateIconControlUI(CControlUI* pContainer, wstring iconPath)
{INTERFUN;
    _control = new CControlUI();
    CControlUI* pControl = (CControlUI*)_control;
    pControl->SetId(L"Control");
    pControl->SetInitPos(10,5,44,40,0);
    pControl->SetStyle(L"Picture");
    pControl->SetAttribute(L"bk.image", iconPath.c_str());
    
    pContainer->Add(pControl);

    if (_pApp->_isLegalSoft && _pApp->_eAppType == APP_TYPE_IPA)
    {
        CControlUI* pLegalControl = new CControlUI();
        pLegalControl->SetId(L"LegalControl");
        pLegalControl->SetInitPos(35,2,51,18,0);
        pLegalControl->SetStyle(_T("Picture"));
        pLegalControl->SetAttribute(_T("bk.image"), _T("#Application_Images_LegalSoftIcon"));        
        pContainer->Add(pLegalControl);
    }
}

void AppListBodyItemData::CreateAppNameLabelUI(CControlUI* pContainer, wstring appName, bool isAndroid)
{INTERFUN;
    _labelAppName = new CAutoShowTipLabelUI();
    CControlUI* pLabelAppName = (CControlUI*)_labelAppName;
    pLabelAppName->SetId(L"LabelAppName");
    pLabelAppName->SetStyle(L"Label");
    pLabelAppName->SetText(appName.c_str());
    pLabelAppName->SetAttribute(_T("txt.font"),_T("font13"));
    if (isAndroid)
        pLabelAppName->SetInitPos(55,5,0,22,4);
    else
        pLabelAppName->SetInitPos(55,0,0,0,8);
    pContainer->Add(pLabelAppName);
}

void AppListBodyItemData::CreateAppPhonePathLabelUI(CControlUI* pContainer, wstring appPhonePath)
{INTERFUN;
    _labelAppPhonePath = new CAutoShowTipLabelUI();
    CControlUI* pLabelAppPhonePath = (CControlUI*)_labelAppPhonePath;
    pLabelAppPhonePath->SetId(L"LabelAppPhonePath");
    pLabelAppPhonePath->SetStyle(L"Label");
    pLabelAppPhonePath->SetText(appPhonePath.c_str());
    pLabelAppPhonePath->SetInitPos(55,22,0,40,4);
    pLabelAppPhonePath->SetAttribute(L"txt.textcolor", L"999999");
    pLabelAppPhonePath->SetAttribute(_T("txt.font"),_T("AppPathfont"));
    pLabelAppPhonePath->SetAttribute(_T("txt.endellipsis"),_T("1"));
    pContainer->Add(pLabelAppPhonePath);
}

CControlUI* AppListBodyItemData::CreateVersionContainerUI( CControlUI* pContainerItem )
{INTERFUN;
	_containerversion = new CContainerUI();
	CControlUI* pContainer = (CControlUI*)_containerversion;
	pContainer->SetId(L"VersionContainer");
	pContainer->SetInitPos(0,0,0,0,8);
	pContainerItem->Add(pContainer);
	return pContainer;
}

void AppListBodyItemData::CreateVersionLabelUI(CControlUI* pContainerItem, wstring version)
{INTERFUN;
	_labelVersion = new CAutoShowTipLabelUI();
	CControlUI* pLabelVersion = (CControlUI*)_labelVersion;
	pLabelVersion->SetId(L"LabelVersion");
	pLabelVersion->SetStyle(L"Label");
	pLabelVersion->SetText(version.c_str());
	pLabelVersion->SetAttribute(L"txt.halign", L"left");
	pLabelVersion->SetAttribute(_T("txt.endellipsis"),_T("1"));
	pLabelVersion->SetInitPos(0,0,70,0,7);
	pContainerItem->Add(pLabelVersion);
}

void AppListBodyItemData::CreatePiracyUpBtnUI( CControlUI* pContainerItem, ApplicationInfoUI* pUI )
{INTERFUN;
	_buttonPiracyUp = new CButtonUI();
	CControlUI* pPiracyUpBtn = (CControlUI*)_buttonPiracyUp;
	pPiracyUpBtn->SetId(_T("PiracyUpBtn"));
	pPiracyUpBtn->SetStyle(L"Application_Style_ThreeBack_Button");
	pPiracyUpBtn->SetAttribute(L"bk.image",_T("#Application_Images_piracy_upgrade"));
	pPiracyUpBtn->SetAttribute(_T("cursor"),_T("hand"));
	pPiracyUpBtn->SetVisible(false);
	pPiracyUpBtn->SetToolTipShow(true);
	pPiracyUpBtn->SetINotifyUI((INotifyUI*)pUI);
	pPiracyUpBtn->SetInitPos(50,13,39,16,5);
	pContainerItem->Add(pPiracyUpBtn);
}

void AppListBodyItemData::CreateLegalUpBtnUI( CControlUI* pContainerItem, ApplicationInfoUI* pUI )
{INTERFUN;
	_buttonLegalUp = new CButtonUI();
	CControlUI* pLegalUpBtn = (CControlUI*)_buttonLegalUp;
	pLegalUpBtn->SetId(_T("LegalUpBtn"));
	pLegalUpBtn->SetStyle(L"Application_Style_ThreeBack_Button");
	pLegalUpBtn->SetAttribute(L"bk.image",_T("#Application_Images_legal_upgrade"));
	pLegalUpBtn->SetAttribute(_T("cursor"),_T("hand"));
	pLegalUpBtn->SetVisible(false);
	pLegalUpBtn->SetToolTipShow(true);
	pLegalUpBtn->SetINotifyUI((INotifyUI*)pUI);
	pLegalUpBtn->SetInitPos(34,12,9,15,5);
	pContainerItem->Add(pLegalUpBtn);
}

void AppListBodyItemData::CreateStaticDiskLabelUI(CControlUI* pContainerItem, wstring staticDiskUsage)
{INTERFUN;
    _labelStaticDiskUsage = new CAutoShowTipLabelUI();
    CControlUI* pLabelStaticDiskUsage = (CControlUI*)_labelStaticDiskUsage;
    pLabelStaticDiskUsage->SetId(L"LabelStaticDiskUsage");
    pLabelStaticDiskUsage->SetStyle(L"Label");
    pLabelStaticDiskUsage->SetText(staticDiskUsage.c_str());
    pLabelStaticDiskUsage->SetAttribute(L"txt.halign", L"left");
    pLabelStaticDiskUsage->SetAttribute(_T("txt.endellipsis"),_T("1"));
    pContainerItem->Add(pLabelStaticDiskUsage);
}

void AppListBodyItemData::CreateDynamicDiskLabelUI(CControlUI* pContainerItem, wstring dynamicDiskUsage)
{INTERFUN;
    _labelDynamicDiskUsage = new CAutoShowTipLabelUI();
    CControlUI* pLabelDynamicDiskUsage = (CControlUI*)_labelDynamicDiskUsage;
    pLabelDynamicDiskUsage->SetId(L"LabelDynamicDiskUsage");
    pLabelDynamicDiskUsage->SetStyle(L"Label");
    pLabelDynamicDiskUsage->SetText(dynamicDiskUsage.c_str());
    pLabelDynamicDiskUsage->SetAttribute(L"txt.halign", L"left");
    pLabelDynamicDiskUsage->SetAttribute(_T("txt.endellipsis"),_T("1"));
    pContainerItem->Add(pLabelDynamicDiskUsage);
}

void AppListBodyItemData::CreateInstallPosLabelUI(CControlUI* pContainerItem, wstring installPos)
{INTERFUN;
    _labelInstallPos = new CAutoShowTipLabelUI();
    CControlUI* pLabelInstallPos = (CControlUI*)_labelInstallPos;
    pLabelInstallPos->SetId(L"LabelInstallPos");
    pLabelInstallPos->SetStyle(L"Label");
    pLabelInstallPos->SetText(installPos.c_str());
    pLabelInstallPos->SetAttribute(L"txt.halign", L"left");
    pLabelInstallPos->SetAttribute(_T("txt.endellipsis"),_T("1"));
    pContainerItem->Add(pLabelInstallPos);
}

CControlUI* AppListBodyItemData::CreateContainerUI1(CControlUI* pContainerItem)
{INTERFUN;
    _container1 = new CContainerUI();
    CControlUI* pContainer1 = (CControlUI*)_container1;
    pContainer1->SetId(L"Container1");
    pContainer1->SetInitPos(0,0,130,0,7);
    pContainerItem->Add(pContainer1);
    return pContainer1;
}

void AppListBodyItemData::CreateUninstallButtonUI(CControlUI* pContainer1, bool isUserApp, bool isAndroid, bool hasSDCard, ApplicationInfoUI* pUI)
{INTERFUN;
    _buttonUninstall = new CButtonUI();
    CControlUI* pButtonUninstall = (CControlUI*)_buttonUninstall;
    isUserApp ? pButtonUninstall->SetId(L"UserButtonUninstall") : pButtonUninstall->SetId(L"SystemButtonUninstall");
    if (isUserApp && isAndroid && hasSDCard)
        pButtonUninstall->SetInitPos(80,10,125,30,0);
    else
        pButtonUninstall->SetInitPos(0,10,45,30,0);
    pButtonUninstall->SetStyle(L"Application_Style_ThreeBack_Button");
    pButtonUninstall->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")));
    pButtonUninstall->SetAttribute(L"visible", L"0");
    pButtonUninstall->SetINotifyUI((INotifyUI*)pUI);
    pContainer1->Add(pButtonUninstall);
}

void AppListBodyItemData::CreateMoveToButtonUI(CControlUI* pContainer1, bool bOnSDCard, ApplicationInfoUI* pUI)
{INTERFUN;
    _buttonMoveTo = new CButtonUI();
    CControlUI* pButtonMoveTo = (CControlUI*)_buttonMoveTo;
    pButtonMoveTo->SetId(L"ButtonMoveTo");
    pButtonMoveTo->SetInitPos(0,10,72,30,0);
    pButtonMoveTo->SetStyle(L"Application_Style_ThreeBack_Button");
    bOnSDCard ? pButtonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_phone"))):pButtonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_sd")));
    pButtonMoveTo->SetAttribute(L"visible", L"0");
    pButtonMoveTo->SetINotifyUI((INotifyUI*)pUI);
    pContainer1->Add(pButtonMoveTo);
}

// 用于应用移动
void AppListBodyItemData::UpdateInfo( AppData* pData, bool isMoveAndroidApp )
{INTERFUN;
    if (_labelInstallPos)
    {
		wstring onSDCard = pData->_bOnSDCard ? CommonUIFunc::LoadString(_T("Application_Text_APP_sdcart")):CommonUIFunc::LoadString(_T("Application_Text_APP_phone"));
        _labelInstallPos->SetText(onSDCard.c_str());
    }

    if (_buttonMoveTo)
    {
		pData->_bOnSDCard ? _buttonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_phone"))):_buttonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_sd")));
		_buttonMoveTo->Resize();
		_buttonMoveTo->Invalidate();
    }

    if (_labelVersion && !pData->_strVersion.empty())
    {
        _labelVersion->SetText(pData->_strVersion.c_str());
    }

	if(!isMoveAndroidApp)
	{
		if(_buttonPiracyUp)
		{
			_buttonPiracyUp->SetVisible(false);
		}
	}
	if(_buttonLegalUp)
	{
		_buttonLegalUp->SetVisible(false);
	}

	if (_labelAppPhonePath && (pData->_eAppType == APP_TYPE_APK || pData->_eAppType == APP_TYPE_NPK) )
	{
		_labelAppPhonePath->SetText(pData->_strAppPhonePath.c_str());
	}

	PostMessage(Singleton::GetMainWnd(), WM_APP_REFRESHLINE, (WPARAM)this, 0);
	if(_pApp&&pData->_strIconPCPath.empty())
	{
		pData->_eAppType=_pApp->_eAppType;
		pData->_strIconPCPath=_pApp->_strIconPCPath;
	}
	SAFE_DELETE(_pApp);
    _pApp = pData;
    _appId = _pApp->_packageName;
	bool isandroid = false;
	if(_pApp->_eAppType == APP_TYPE_APK || _pApp->_eAppType == APP_TYPE_NPK)
		isandroid = true;
	wstring staticDiskUsage = _pApp->_staticDiskUsage == 0 ? CommonUIFunc::LoadString(_T("APP_unknow")):CStrOperation::FormatFileSize(_pApp->_staticDiskUsage);
	if(_labelStaticDiskUsage)
	{
		_labelStaticDiskUsage->SetText(staticDiskUsage.c_str());
		_labelStaticDiskUsage->SetUserData((UINT)_pApp->_staticDiskUsage);
	}
	if(isandroid)
	{
		wstring onSDCard = _pApp->_bOnSDCard ? CommonUIFunc::LoadString(_T("Application_Text_APP_sdcart")):CommonUIFunc::LoadString(_T("Application_Text_APP_phone"));
		if(_labelInstallPos)
			_labelInstallPos->SetText(onSDCard.c_str());
		if(_buttonMoveTo)
			_pApp->_bOnSDCard ? _buttonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_phone"))):_buttonMoveTo->SetText(CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_sd")));
	}
	else
	{
		wstring dynamicDiskUsage = _pApp->_dynamicDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(_pApp->_dynamicDiskUsage);
		if(_labelDynamicDiskUsage)
		{
			_labelDynamicDiskUsage->SetText(dynamicDiskUsage.c_str());
			_labelDynamicDiskUsage->SetUserData((UINT)_pApp->_dynamicDiskUsage);
		}
	}
	SetToolTipText(_pApp, isandroid, _containerItem);
}

void AppListBodyItemData::RefreshLine()
{INTERFUN;
    if (_buttonMoveTo)
    {
        _buttonMoveTo->Resize();
        _buttonMoveTo->Invalidate();
    }
    _containerItem->Resize();
    _containerItem->Invalidate();
}

void AppListBodyItemData::EnabledPiracyUpBtn( bool enabled )
{
	_buttonPiracyUp->SetEnabled(enabled);
}

void AppListBodyItemData::EnabledLegalUpBtn( bool enabled )
{
	_buttonLegalUp->SetEnabled(enabled);
}
App_CC_END