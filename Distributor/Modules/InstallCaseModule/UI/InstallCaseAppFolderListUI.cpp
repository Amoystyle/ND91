#include "StdAfx.h"

#include "LocalAppinfoList.h"
#include "MainView.h"
#include "InstallCaseAppFolderListUI.h"

INSTALLCASE_BEGIN

//¡–±Ì≈≈–Ú
bool __stdcall InstallCaseAppFolderCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{INTERFUN;
	RETURN_FALSE_IF(!pControlA || !pControlB);


	CListItemUI* pItemA = dynamic_cast<CListItemUI*>(pControlA);
	CListItemUI* pItemB = dynamic_cast<CListItemUI*>(pControlB);
	wstring aId = pItemA->GetId();
	wstring bId = pItemB->GetId();
	if(aId.find(L"Group_") == 0 || 
		bId.find(L"Group_") == 0)
		return (!exchange);

	AppCountByFolder* folderA = (AppCountByFolder*)pControlA->GetUserData();
	AppCountByFolder* folderB = (AppCountByFolder*)pControlB->GetUserData();
	if(folderA != NULL && folderB != NULL && folderA->Filetype != folderB->Filetype)
		return(!exchange);

	switch (nCol)
	{
	case 0://√˚≥∆
		return PublicFunc::CaseSortRult(pControlA, pControlB,L"LabContainer.LabelFolderName");
		break;
	case 1:// 
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelFolderCount");
		break;
	case 2://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelFolderSize");
		break;
	case 3:// 
		//return PublicFunc::CaseSortRult(pControlA, pControlB, L"LabelFolderType");
		break;
	case 4://
		break;
	default:
		break;
	}
	return !exchange;
}

InstallCaseAppFolderListUI::InstallCaseAppFolderListUI(InstallCaseUI* caseUI)
{INTERFUN;
	_pInstallCaseUI = caseUI;
}

InstallCaseAppFolderListUI::~InstallCaseAppFolderListUI(void)
{INTERFUN;

}

void InstallCaseAppFolderListUI::OnCreate()
{INTERFUN;

	_pList = dynamic_cast<CListUI*>(GetItem(_T("List")));
	_pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);
	_pList->SetSortCallBack(InstallCaseAppFolderCompare);
	_pListOwner = new LocalAppinfoList(_pList, this);
}

void InstallCaseAppFolderListUI::AddFolderToView( AppCountByFolder* appFolder )
{INTERFUN;
	if(_pListOwner == NULL || appFolder == NULL)
		return;

	int index = this->AddListGroupTittle(appFolder->Filetype, appFolder->Filetype == _INSTALLS_CASE_DATA_IOS? L"IOS":L"Android");
	
	_pListOwner->AddFolderItem(appFolder, index);
}


int InstallCaseAppFolderListUI::AddListGroupTittle(_INSTALLS_CASE_DATA_TYPE type, wstring text)
{INTERFUN;
	wstring Id = L"Group_" + CStrOperation::IntToWString(type);
	CListUI* taskList= _pList;

	if(taskList->GetItem(Id.c_str()) != NULL)
		return taskList->GetItem(Id.c_str())->GetIndex();

	CListContainerItemUI* unfinishitem=new CListContainerItemUI();
	unfinishitem->SetId(Id.c_str());
	CControlUI* unfinishctrl=new CControlUI();
	unfinishctrl->SetInitPos(0,0,0,0,8);
	unfinishctrl->SetStyle(_T("myIconText"));
	unfinishctrl->SetAttribute(_T("icon.pos"),_T("6|0,10,0,9"));
	unfinishctrl->SetAttribute(_T("txt.pos"),_T("8|6,0,5,10"));
	unfinishctrl->SetAttribute(_T("text"),text.c_str());
	unfinishctrl->SetAttribute(_T("txt.font"),_T("font12b"));
	unfinishctrl->SetAttribute(_T("icon.image"),_T("#InstallCase_Image_listsplitline"));
	unfinishitem->Add(unfinishctrl);
	taskList->Add(unfinishitem);
	taskList->CheckItemVisible(false,unfinishitem->GetIndex());
	unfinishitem->SetEnabled(false);
	return unfinishitem->GetIndex();
}

void InstallCaseAppFolderListUI::RemoveFolderFromView( AppCountByFolder* appFolder )
{INTERFUN;
	if(_pListOwner == NULL)
		return;
	
	CControlUI* item = _pList->GetItem(CStrOperation::IntToWString(appFolder->m_id).c_str());
	if(item == NULL)
		return;

	_pList->Remove(item);

	_pList->Resize();
	_pList->Invalidate(true);
}

bool InstallCaseAppFolderListUI::OnRefreshCaseBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	_pInstallCaseUI->Model.RefreshCase();
	return true;
}

bool InstallCaseAppFolderListUI::OnAddFolderBtnClick( TNotifyUI* pNotify )
{INTERFUN;
CommonUIFunc::ShowMenu(IDR_MENU_ADD_FOLDER, Singleton::GetMainWnd(), this, pNotify->pSender);
	return true;
}

bool InstallCaseAppFolderListUI::OnAddFolderMenuItemClick( TNotifyUI* pNotify )
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;

	switch (pInfo->uID)
	{
	case IDR_MENU_ADD_FOLDER_IOS:
		{
			_pInstallCaseUI->StatisAddIOSFolder();
			_pInstallCaseUI->Model.AddLibFolder(_INSTALLS_CASE_DATA_IOS);
		}
		break;
	case IDR_MENU_ADD_FOLDER_ANDROID:
		{
			_pInstallCaseUI->StatisAddAndroidFolder();
			_pInstallCaseUI->Model.AddLibFolder(_INSTALLS_CASE_DATA_ANDROID);
		}
		break;
	case IDR_MENU_FOLDER_OPEN:
		{
			CControlUI* item = (CControlUI*)pInfo->lParamNotify;

			this->OpenDir( item);
		}
		break;
	case IDR_MENU_FOLDER_REMOVE:
		{
			this->RemoveSelectFolder();
		}
		break;
	}
	return true;
}

bool InstallCaseAppFolderListUI::OnOpenFolderBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	CListContainerItemUI* pCurSel = (CListContainerItemUI*)pNotify->pSender->GetParent()->GetParent();
	if(pCurSel == NULL)
		return false;
	
	this->OpenDir( pCurSel);
	return true;
}

bool InstallCaseAppFolderListUI::OnListDBClick( TNotifyUI* pNotify )
{INTERFUN;
	CListContainerItemUI* item = (CListContainerItemUI*)pNotify->lParam;
	RETURN_TRUE_IF(!item);
	this->OpenDir( item);
	return true;
}

bool InstallCaseAppFolderListUI::OnRemoveFolderBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	this->RemoveSelectFolder();
	return true;	
}

bool InstallCaseAppFolderListUI::OnDropFiles( TNotifyUI* pNotify )
{INTERFUN;
	HDROP hDrop = (HDROP)pNotify->wParam;
	wchar_t szFileName[MAX_PATH + 1] = {0};

	UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	vector<wstring> iosFiles, andFiles;

	for(UINT i=0;i<nFiles;i++)
	{
		DragQueryFile(hDrop, i, szFileName, MAX_PATH);
		if(CFileOperation::IsDirExist(szFileName))
		{
			vector<wstring> subFiles;
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("ipa"));
			iosFiles.insert(iosFiles.end(),subFiles.begin(), subFiles.end());
			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("pxl"));
			iosFiles.insert(iosFiles.end(),subFiles.begin(), subFiles.end());

			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("apk"));
			andFiles.insert(andFiles.end(),subFiles.begin(), subFiles.end());
			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("npk"));
			andFiles.insert(andFiles.end(),subFiles.begin(), subFiles.end());

		}
		else
		{
			wstring ext = CFileOperation::GetFileExt(szFileName);
			ext = CStrOperation::toLowerW(ext);
			if(ext == L"ipa" || ext == L"pxl")
			{
				iosFiles.push_back(wstring(szFileName));
			}
			else if(ext == L"apk" || ext == L"npk")
			{
				andFiles.push_back(wstring(szFileName));
			}
		}
	}


	DragFinish(hDrop);
	if(iosFiles.size() > 0 || andFiles.size() > 0)
	{
		_pInstallCaseUI->Model.AddFilesToLib(&iosFiles, &andFiles);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_FileNoFound")), ICON_WARNING);
	}
	
	return true;
}

bool InstallCaseAppFolderListUI::OnShowMenuFolderList( TNotifyUI* pNotify )
{INTERFUN;
	if(pNotify->lParam==0)
		return true;    
	CMenuUI* pMenu = CMenuUI::Load(IDR_MENU_FOLDER_LIST);
	if (pMenu)
	{
		CControlUI* item = _pList->GetCurSel();

		AppCountByFolder* appFolder = (AppCountByFolder*)item->GetUserData();
		if(appFolder->Filetype == _INSTALLS_CASE_DATA_IOS && appFolder->FolderName ==CONFIG(_strIosDownPath) 
			|| appFolder->Filetype == _INSTALLS_CASE_DATA_ANDROID && appFolder->FolderName ==CONFIG(_strAndroidDownPath))
		{
			pMenu->Remove(1);
		}
		pMenu->SetParam(pNotify->wParam,pNotify->lParam);	
		pMenu->Show(Singleton::GetMainWnd(), NULL, this);
	}
	return true;
}

void InstallCaseAppFolderListUI::OpenDir( CControlUI* folderItem )
{INTERFUN;
	AppCountByFolder* appFolder = (AppCountByFolder*)folderItem->GetUserData();
	if(appFolder == NULL)
		return;

	if(!CFileOperation::IsDirExist(appFolder->FolderName.c_str()) && (appFolder->FolderName==CONFIG(_strIosDownPath)  || appFolder->FolderName==CONFIG(_strAndroidDownPath) ))
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_DownFolderNotExistWillRemove")), CommonUIFunc::LoadString(_T("InstallCase_Text_PROMPT")));
		return;
	}
	if(!CFileOperation::IsDirExist(appFolder->FolderName.c_str()))
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_FolderNotExistWillRemove")), CommonUIFunc::LoadString(_T("InstallCase_Text_PROMPT")));
		RemoveSelectFolder();
		return;
	}
	_pInstallCaseUI->Model.OpenDir(CStrOperation::trimRightW(appFolder->FolderName, L"\\"), false);
}

bool  InstallCaseAppFolderListUI::ListFocused()
{INTERFUN;
	CControlUI* pControl = this->GetWindow()->GetFocus();
	while ( pControl )
	{
		if ( pControl == _pList )
			return true;

		pControl = pControl->GetParent();
	}
	return false;
}

//void InstallCaseAppFolderListUI::Receive( MESSAGE_TYPE msg, const BaseParam* pParm )
//{INTERFUN;
//	RETURN_IF(!ListFocused());
//	switch(pParm->GetInt())
//	{
//	case VK_F5:
//		_pInstallCaseUI->Model.RefreshCase();
//		break;
//	case VK_DELETE:
//	case VK_DECIMAL:
//		RemoveSelectFolder();
//		break;
//	}
//}

void InstallCaseAppFolderListUI::RemoveSelectFolder()
{INTERFUN;
	CControlUI* pCurSel = _pList->GetCurSel();
	if(pCurSel == NULL)
		return;

	AppCountByFolder* appFolder = (AppCountByFolder*)pCurSel->GetUserData();
	if(appFolder == NULL)
		return;

	if(appFolder->Filetype == _INSTALLS_CASE_DATA_IOS && appFolder->FolderName ==CONFIG(_strIosDownPath)
		|| appFolder->Filetype == _INSTALLS_CASE_DATA_ANDROID && appFolder->FolderName ==CONFIG(_strAndroidDownPath))
		return;

	_pInstallCaseUI->Model.RemoveAppFolder(appFolder);
}

INSTALLCASE_END