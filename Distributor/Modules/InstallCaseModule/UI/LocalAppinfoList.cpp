#include "stdafx.h"
#include "LocalAppinfoList.h"
#include "LocalAppInfoItem.h"

INSTALLCASE_BEGIN

LocalAppinfoList::LocalAppinfoList(CListUI* listui,CCompositeUI* compui)
{INTERFUN;
	_pAppInfoList = listui;
	_compUI = compui;
	UI_REGISTER_DYNCREATE(_T("CAutoShowTipLabelUI"), CAutoShowTipLabelUI);
}
void LocalAppinfoList::ClearAppList()
{INTERFUN;
	if(!_pAppInfoList)
		return;
	_pAppInfoList->RemoveAll();

		_pAppInfoList->CheckItem(false);
	UpdateControl(_pAppInfoList);
}

void LocalAppinfoList::AddAppItem(InstallsAppInfo* appinfo,bool isupdate, bool isuserdata, bool issystemdata,int select, bool insertToFront, vector<wstring>* upidlist) 
{INTERFUN;
	if(!_pAppInfoList)
		return;	

	CListContainerItemUI* item = (CListContainerItemUI*)_pAppInfoList->GetItem(appinfo->m_strSavePath.c_str());
	if( item == NULL)
	{
		LocalAppInfoItem::CreateAppInfoItem(appinfo,this,_compUI,isupdate,isuserdata,issystemdata,select, insertToFront, upidlist);

		_pAppInfoList->Resize();
		_pAppInfoList->Invalidate();
	}
	else
	{
		if(!isupdate)
		{
			CContainerUI* sItem = (CContainerUI*)item->GetItem(0);
			wstring iconPath = (wstring)sItem->GetItem(0)->GetText();
			LocalAppInfoItem::UpdateAppInfoItemUpgradeState(appinfo,this,_compUI);
			CContainerUI*  pVersionContainer = (CContainerUI*)item->GetItem(1);
			CButtonUI* btnup = (CButtonUI*)pVersionContainer->GetItem(_T("UpgradeBtn"));
			CButtonUI* btnlegalup = (CButtonUI*)pVersionContainer->GetItem(_T("LegalUpgradeBtn"));
			if( appinfo->m_bUpdateable==INSTALLS_CASE_UPDATEABLE_YES)
			{
				if(btnup)
				{
					btnup->SetEnabled(true);
					btnup->SetVisible(true);
				}
				else
				{
					btnup=new CButtonUI();
					btnup->SetId(_T("UpgradeBtn"));
					btnup->SetStyle(_T("ThreeBack_Button"));
					btnup->SetAttribute(L"bk.image",_T("#InstallCase_Image_piracy_upgrade"));
					btnup->SetAttribute(_T("pos"),_T("5|50,10,39,14"));
					btnup->SetAttribute(_T("cursor"),_T("hand"));
					btnup->SetToolTipShow(true);
					wstring tooltip = CommonUIFunc::LoadStringW(_T("InstallCase_Text_UpgradeTo")) + appinfo->m_strNewVersion;
					btnup->SetToolTip(tooltip.c_str());
					btnup->SetINotifyUI(_compUI);
					pVersionContainer->Add(btnup);
				}
			}

			if( appinfo->m_bLegalUpdateable==INSTALLS_CASE_LEGALUPDATEABLE_YES)
			{
				if(btnlegalup)
				{
					btnlegalup->SetEnabled(true);
					btnlegalup->SetVisible(true);
				}
				else
				{
					btnlegalup=new CButtonUI();
					btnlegalup->SetId(_T("LegalUpgradeBtn"));
					btnlegalup->SetStyle(_T("ThreeBack_Button"));
					btnlegalup->SetAttribute(L"bk.image",_T("#InstallCase_Image_legal_upgrade"));
					btnlegalup->SetAttribute(_T("pos"),_T("5|34, 9, 9, 13"));
					btnlegalup->SetAttribute(_T("cursor"),_T("hand"));
					btnlegalup->SetToolTipShow(true);
					wstring tooltip = CommonUIFunc::LoadStringW(_T("InstallCase_Text_UpgradeTo")) + appinfo->m_strNewLegalVersion;
					btnlegalup->SetToolTip(tooltip.c_str());
					btnlegalup->SetINotifyUI(_compUI);
					pVersionContainer->Add(btnlegalup);
				}
			}

			if( iconPath == appinfo->m_Iconpath && appinfo->m_bLegalUpdateable!=INSTALLS_CASE_LEGALUPDATEABLE_YES && appinfo->m_bUpdateable!=INSTALLS_CASE_UPDATEABLE_YES)
				return;

			LocalAppInfoItem::UpdateAppInfoItem(appinfo,this, select);

		}
	}
}

void LocalAppinfoList::UpdateAppItemByPath( wstring path )
{
	CListContainerItemUI* item = (CListContainerItemUI*)_pAppInfoList->GetItem(path.c_str());
	if(item)
	{
		InstallsAppInfo* appinfo = (InstallsAppInfo*)item->GetUserData();
		if(appinfo)
		{
			LocalAppInfoItem::UpdateAppInfoItemUpgradeState(appinfo, this, _compUI);
		}
	}
}

void LocalAppinfoList::RemoveAppItem(CControlUI* item)
{INTERFUN;
	if(!_pAppInfoList)
		return;
	_pAppInfoList->Remove(item);
	_pAppInfoList->Invalidate();
	if(_pAppInfoList->GetCount()==0)
		_pAppInfoList->CheckItem(false);

	_pAppInfoList->Resize();
	_pAppInfoList->Invalidate();
}
void LocalAppinfoList::RemoveAppItemByID(int id)
{INTERFUN;
	if(!_pAppInfoList)
		return;
	for(int i=0;i<_pAppInfoList->GetCount();i++)
	{
		if(_wtoi(_pAppInfoList->GetItem(i)->GetId()) == id)
		{
			RemoveAppItem(_pAppInfoList->GetItem(i));
		}
	}

}

void LocalAppinfoList::AddCaseItem(InstallsCaseData* CaseData)
{INTERFUN;
	if(!_pAppInfoList)
		return;
	LocalAppInfoItem::CreateCaseItem(CaseData,this,_compUI);
}

void LocalAppinfoList::UpdateCaseItem(InstallsCaseData *CaseData)
{INTERFUN;
	if(!_pAppInfoList)
		return;
	LocalAppInfoItem::UpdateCaseItem(CaseData,this);
}

void LocalAppinfoList::RemoveCaseItem(InstallsCaseData *CaseData)
{INTERFUN;
	if(!_pAppInfoList)
		return;
	for(int i=0;i<_pAppInfoList->GetCount();i++)
	{
		if(_wtoi(_pAppInfoList->GetItem(i)->GetId()) == CaseData->Id())
		{
			_pAppInfoList->Remove(_pAppInfoList->GetItem(i));
			break;
		}
	}
}
void LocalAppinfoList::ClearCaseList()
{INTERFUN;
	if(!_pAppInfoList)
		return;

	_pAppInfoList->RemoveAll();
	UpdateControl(_pAppInfoList);
}

void  LocalAppinfoList::AddFolderItem(AppCountByFolder* FolderItem, int index)
{INTERFUN;
	if(!_pAppInfoList)
		return;

	CControlUI* item = _pAppInfoList->GetItem(CStrOperation::IntToWString(FolderItem->m_id).c_str());

	if(item == NULL)
	{
		LocalAppInfoItem::CreateFolderSumItem(FolderItem, index, this, _compUI);
	}
	else
	{
		if(!FolderItem->bHasChanged)
			return;

		LocalAppInfoItem::UpdateFolderSumItem(FolderItem, this, _compUI);
	}

	_pAppInfoList->Resize();
	_pAppInfoList->Invalidate();
}
void LocalAppinfoList::ClearFolderList()
{INTERFUN;
	if(!_pAppInfoList)
		return;

	_pAppInfoList->RemoveAll();
	UpdateControl(_pAppInfoList);
}

bool LocalAppinfoList::ListContainerItemEvent(TEventUI& event)
{INTERFUN;
	CControlUI* pControl = event.pSender;
	if (pControl == NULL)
		return true;
	switch(event.Type)
	{
	case UIEVENT_MOUSEENTER:
		{
			pControl->GetItem(_T("ContainerBtn"))->SetVisible(true, false);
			dynamic_cast<CListContainerItemUI*>(pControl)->ReCalcRect();
			pControl->Invalidate();
		}
		break;
	case UIEVENT_MOUSELEAVE:
		{
			pControl->GetItem(_T("ContainerBtn"))->SetVisible(false, false);
			dynamic_cast<CListContainerItemUI*>(pControl)->ReCalcRect();
			pControl->Invalidate();
		}
		break;
	}
	return true;
}

INSTALLCASE_END