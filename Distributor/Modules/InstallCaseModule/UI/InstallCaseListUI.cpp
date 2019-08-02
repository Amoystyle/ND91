#include "StdAfx.h"
#include "LocalAppinfoList.h"
#include "InstallCaseListUI.h"
#include "MainView.h"

INSTALLCASE_BEGIN

bool __stdcall InstallCaseListCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{INTERFUN;
	RETURN_FALSE_IF(!pControlA || !pControlB);

	switch (nCol)
	{
	case 0://名称
		return PublicFunc::CaseSortRult(pControlA, pControlB, L"LabContainer.LabelCaseName");
		break;
	case 1:// 
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelInstallCount");
		break;
	case 2://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelFileCount");
		break;
	case 3:// 
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelTotalSize");
		break;
	case 4://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"Labelneedtime");
		break;
	default:
		break;
	}
	return !exchange;
}

InstallCaseListUI::InstallCaseListUI(InstallCaseUI* caseUI)
{INTERFUN;
	_pInstallCaseUI = caseUI;
}

InstallCaseListUI::~InstallCaseListUI(void)
{INTERFUN;

}

void InstallCaseListUI::AddCaseToView( InstallsCaseData* caseData )
{INTERFUN;
	if(_pListOwner == NULL || caseData == NULL)
		return;

	_pListOwner->AddCaseItem(caseData);
	_pList->Resize();
	_pList->Invalidate();


	if(_pList->GetCount() > 0)
	{
		_pListEmpty->SetVisible(false);
		_pListPanel->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}
}

void InstallCaseListUI::UpdateCaseToView(InstallsCaseData* caseData)
{INTERFUN;
	if(_pListOwner == NULL || caseData == NULL)
		return;

	_pListOwner->UpdateCaseItem(caseData);
	_pList->Resize();
	_pList->Invalidate();
}

void InstallCaseListUI::RemoveCaseFromView( int caseId )
{INTERFUN;
	if(_pListOwner == NULL)
		return;

	CControlUI* item = _pList->GetItem(CStrOperation::IntToWString(caseId).c_str());
	if(item == NULL)
		return;

	_pList->Remove(item);
	_pList->Resize();
	_pList->Invalidate();

	if(_pList->GetCount() <= 0)
	{
		//删除全部后，ListHeaderCheckBox仍然选中的问题
		_pList->CheckItem(false, -1, false);
		_pListPanel->SetVisible(false);
		_pListEmpty->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}
}

bool  InstallCaseListUI::ListFocused()
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

//void InstallCaseListUI::Receive( MESSAGE_TYPE msg, const BaseParam* pParm )
//{INTERFUN;
//	RETURN_IF(!ListFocused());
//	switch(pParm->GetInt())
//	{
//	case VK_F5:
//		break;
//	case VK_DELETE:
//	case VK_DECIMAL:
//		this->OnRemoveCaseBtnClick(NULL);
//		break;
//	}
//}

void InstallCaseListUI::OnCreate()
{INTERFUN;
	_pListPanel = dynamic_cast<CLayoutUI*>(GetItem(_T("ListPanel")));
	_pListEmpty = dynamic_cast<CLayoutUI*>(GetItem(_T("ListEmpty")));
	_pListEmpty->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);
	_pList = dynamic_cast<CListUI*>(GetItem(_T("ListPanel.List")));
	_pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);
	_pList->SetSortCallBack(InstallCaseListCompare);
	_pListOwner = new LocalAppinfoList(_pList, this);
}

bool InstallCaseListUI::OnRemoveCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisBatchRemove();

	vector<int> idList;
	for (int i=0;i<_pList->GetCount();i++)
	{
		if(_pList->IsCheckItem(i))
		{
			CControlUI* item = _pList->GetItem(i);
			int nID = _wtoi(item->GetId());

			idList.push_back(nID);
		}
	}
	if(idList.size() > 0)
	{
		_pInstallCaseUI->Model.RemoveCase(&idList);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("请选择需要删除的方案")), ICON_WARNING);
	}
	return true;
}

bool InstallCaseListUI::OnRemoveSingleCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisRemoveCase();

	CListContainerItemUI* pCurSel = (CListContainerItemUI*)pNotify->pSender->GetParent()->GetParent();
	if(pCurSel == NULL)
		return true;

	vector<int> idList;
	int nID = _wtoi(pCurSel->GetId());
	idList.push_back(nID);
	_pInstallCaseUI->Model.RemoveCase(&idList);

	return true;
}

bool InstallCaseListUI::OnShowCaseBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	CListContainerItemUI* pCurSel = (CListContainerItemUI*)pNotify->pSender->GetParent()->GetParent();
	if(pCurSel == NULL)
		return false;

	_pInstallCaseUI->SelectCaseNode(pCurSel->GetId());
	return true;
}

bool InstallCaseListUI::OnCaseListDBClick( TNotifyUI* pNotify )
{INTERFUN;
	CListContainerItemUI* item = (CListContainerItemUI*)pNotify->lParam;
	RETURN_TRUE_IF(!item);
	_pInstallCaseUI->SelectCaseNode(item->GetId());
	return true;
}

bool InstallCaseListUI::OnCreateCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisCreateNewPlan();

	_pInstallCaseUI->Model.AddCase(_INSTALLS_CASE_DATA_IOS, NULL, true);
	return true;
}

bool InstallCaseListUI::OnAddNewCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	CommonUIFunc::ShowMenu(IDR_MENU_NEW_CASE_FROM, Singleton::GetMainWnd(), this, pNotify->pSender);

	return true;
}

bool InstallCaseListUI::OnAddNewCaseFrom(TNotifyUI* pNotify)
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;

	switch (pInfo->uID)
	{
	case IDR_MENU_NEW_CASE_FROM_LIB:
		_pInstallCaseUI->Model.AddCase(_INSTALLS_CASE_DATA_IOS, NULL, true);
		break;
	case IDR_MENU_NEW_CASE_FROM_FOLDER_IOS:
		_pInstallCaseUI->Model.AddCase(_INSTALLS_CASE_DATA_IOS, NULL, false);
		break;
	case IDR_MENU_NEW_CASE_FROM_FOLDER_ANDROID:
		_pInstallCaseUI->Model.AddCase(_INSTALLS_CASE_DATA_ANDROID, NULL, false);
		break;
	}

	return true;
}

bool  InstallCaseListUI::OnShowMenuCaseList(TNotifyUI* pNotify)
{INTERFUN;
	if(pNotify->lParam==0)
		return true;
	CControlUI *pCur = (CControlUI *)pNotify->lParam;
	if(!pCur) return true;

	_pList->SelectItem(pCur);
	int nID = _wtoi(pCur->GetId());
	CMenuUI* pMenu = CMenuUI::Load(IDR_MENU_CASE_LIST);
	if (pMenu)
	{
		pMenu->SetParam(pNotify->wParam,pNotify->lParam);
		pMenu->Show(Singleton::GetMainWnd(), NULL, this);
	}

	return true;
}

bool InstallCaseListUI::OnClickMenuCaseList(TNotifyUI* pNotify)
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;

	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;

	CControlUI* item = (CControlUI*)pInfo->lParamNotify;
	if(item == NULL)
		return false;

	int nID=_wtoi(item->GetId());
	int type = item->GetUserData();
	switch (pInfo->uID)
	{
	case IDR_MENU_EDIT_CASE:
		_pInstallCaseUI->Model.EditCase(nID);
		break;
	case IDR_MENU_DELETE_CASE:
		OnRemoveCaseBtnClick(pNotify);
		break;
	case IDR_MENU_OPEN_CASE:
		_pInstallCaseUI->SelectCaseNode(item->GetId());
		break;
	}
	return true;
}

bool InstallCaseListUI::OnDropFiles( TNotifyUI* pNotify )
{INTERFUN;
	HDROP hDrop = (HDROP)pNotify->wParam;
	wchar_t szFileName[MAX_PATH + 1] = {0};

	UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	vector<wstring>* Files = new vector<wstring>();

	for(UINT i=0;i<nFiles;i++)
	{
		DragQueryFile(hDrop, i, szFileName, MAX_PATH);
		if(CFileOperation::IsDirExist(szFileName))
		{
			vector<wstring> subFiles;
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("ipa"));
			Files->insert(Files->end(),subFiles.begin(), subFiles.end());
			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("pxl"));
			Files->insert(Files->end(),subFiles.begin(), subFiles.end());
			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("apk"));
			Files->insert(Files->end(),subFiles.begin(), subFiles.end());
			subFiles.clear();
			CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("npk"));
			Files->insert(Files->end(),subFiles.begin(), subFiles.end());

		}
		else
		{
			wstring ext = CFileOperation::GetFileExt(szFileName);
			ext = CStrOperation::toLowerW(ext);
			if(ext == L"ipa" || ext == L"pxl" || ext == L"apk" || ext == L"npk")
				Files->push_back(wstring(szFileName));
		}
	}
	DragFinish(hDrop);

	if(Files->size() > 0)
	{
		_pInstallCaseUI->Model.AddCase( Files);
		//DragDropMessageReceiver::SendMessage(Singleton::GetMainWnd(), WM_DRAGDROP_CASEMGR, (WPARAM)Files, NULL, this);

	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_FileNoFound")), ICON_WARNING);
	}
	
	return true;
}

//bool InstallCaseListUI::OnDragDropMessage( UINT message, WPARAM wParam, LPARAM lParam )
//{
//	switch(message)
//	{
//	case WM_DRAGDROP_CASEMGR:
//		{
//			vector<wstring>* parFiles = (vector<wstring>*)wParam;
//			vector<wstring> files;
//			if(parFiles != NULL)
//			{
//				files.insert(files.end(), parFiles->begin(), parFiles->end());
//				SAFE_DELETE(parFiles);	
//				_pInstallCaseUI->Model.AddCase( &files);
//				return true;
//			}	
//		}
//		break;
//	}
//	return false;
//}

INSTALLCASE_END