#include "StdAfx.h"
#include "logic/InstallsCase/InstallCaseModelEx.h"
#include "MainView.h"
#include "InstallCaseAppFolderListUI.h"
#include "InstallCaseAppInfoListUI.h"
#include "InstallCaseListUI.h"
#include "AddEditCaseDialog.h"

INSTALLCASE_BEGIN

#define CaseTreeID					_T("Left.Tree")

#define AppListIOSBtnID				_T("Right.IOSLibSoftLayout.TabBar.AppListIOSBtn")
#define UpgrateIOSBtnID				_T("Right.IOSLibSoftLayout.TabBar.UpgrateIOSBtn")
#define AppListAndroidBtnID			_T("Right.AndroidLibSoftLayout.TabBar.AppListAndroidBtn")
#define UpgrateAndroidBtnID			_T("Right.AndroidLibSoftLayout.TabBar.UpgrateAndroidBtn")

#define FolderLayoutID				_T("Right.FolderLayout")
#define IOSLibSoftLayoutID			_T("Right.IOSLibSoftLayout")
#define AndroidLibSoftLayoutID		_T("Right.AndroidLibSoftLayout")
#define UserCaseListLayoutID		_T("Right.UserCaseListLayout")
#define UserCaseLayoutID			_T("Right.UserCaseLayout")

InstallCaseUI::InstallCaseUI(LeftView* pLeftView)
: Model(this)
{INTERFUN;
	_pLeftView = pLeftView;
	_pTree = NULL;
	_pSoftLibNode = NULL;
	_pCaseListNode = NULL;

	_pAddEditCaseDialog = NULL;

	UI_REGISTER_DYNCREATE(_T("CBigListComBoxUI"), CBigListComBoxUI);
}

InstallCaseUI::~InstallCaseUI(void)
{INTERFUN;
}

wstring InstallCaseUI::GetStyleID()
{
	return L"InstallerCaseMain";
}

void InstallCaseUI::GetCaseInfoFromLib( wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* pathList )
{INTERFUN;
	_pAddEditCaseDialog = new AddEditCaseDialog(this);

	_pAddEditCaseDialog->CreateView((InstallsCaseData*)_pAppListIOSBtn->GetUserData());
	_pAddEditCaseDialog->CreateView((InstallsCaseData*)_pAppListAndroidBtn->GetUserData());
	_pAddEditCaseDialog->Create(Singleton::GetMainWnd(),_T("AddAndEditCase"));
	_pAddEditCaseDialog->SetAttribute(_T("title"), CommonUIFunc::LoadString(L"InstallCase_Text_CaseNewCase"));
	_pAddEditCaseDialog->SetTitle(CommonUIFunc::LoadString(L"InstallCase_Text_CaseNewCase"));
	_pAddEditCaseDialog->CenterWindow(Singleton::GetMainWnd());
	_pAddEditCaseDialog->SetDefaultCaseName(caseName);
	_pAddEditCaseDialog->SetDefaultCaseType(caseType);
	int ret = _pAddEditCaseDialog->DoModal();
	if(ret!=1)
	{
		caseName = L"";
		SAFE_DELETE(_pAddEditCaseDialog);
		return;
	}

	_pAddEditCaseDialog->GetCaseInfo(caseName, caseType, pathList);

	SAFE_DELETE(_pAddEditCaseDialog);
}

void InstallCaseUI::GetEditCaseIdList( InstallsCaseData* caseData, wstring& caseName, vector<wstring>* pathList )
{INTERFUN;
	_pAddEditCaseDialog = new AddEditCaseDialog(this, caseData);
	_pAddEditCaseDialog->CreateView((InstallsCaseData*)_pAppListIOSBtn->GetUserData());
	_pAddEditCaseDialog->CreateView((InstallsCaseData*)_pAppListAndroidBtn->GetUserData());
	_pAddEditCaseDialog->Create(Singleton::GetMainWnd(),_T("AddAndEditCase"));
	_pAddEditCaseDialog->SetAttribute(_T("title"), CommonUIFunc::LoadString(L"InstallCase_Text_EditInstallCase"));
	_pAddEditCaseDialog->SetTitle(CommonUIFunc::LoadString(L"InstallCase_Text_EditInstallCase"));
	_pAddEditCaseDialog->CenterWindow(Singleton::GetMainWnd());
	_pAddEditCaseDialog->SetDefaultCaseName(caseName);
	int ret = _pAddEditCaseDialog->DoModal();

	_INSTALLS_CASE_DATA_TYPE caseType;
	_pAddEditCaseDialog->GetCaseInfo(caseName, caseType, pathList);
	if(ret !=1 || caseType != caseData->CaseType())
	{
		caseName = L"";
		SAFE_DELETE(_pAddEditCaseDialog);
		return;
	}
	SAFE_DELETE(_pAddEditCaseDialog);
}

void InstallCaseUI::GetCaseInfoFromFolder(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, wstring& folder)
{INTERFUN;
	bool bCancel = false;
	wstring ret = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"InstallCase_Text_InputNewCaseName"), CommonUIFunc::LoadString(L"InstallCase_Text_CaseNewCase"), caseName, bCancel);
	caseName = L"";
	if(!bCancel) 
	{
		if (CStrOperation::trimLeftW(ret,L" ").empty())
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CaseCreateFailure")), ICON_WARNING);
			return;
		}
		caseName = CStrOperation::trimLeftW(ret,L" ");
		folder = GetFolder(caseType);
	}
}

wstring InstallCaseUI::GetFolder(_INSTALLS_CASE_DATA_TYPE type)
{INTERFUN;
	wstring defPath;
	if(type == _INSTALLS_CASE_DATA_IOS)
		defPath=CONFIG(_strIosDownPath);
	else
		defPath=CONFIG(_strAndroidDownPath);

	if(!CFileOperation::IsDirExist(defPath.c_str()))
		CFileOperation::MakeLocalDir(defPath);
	HWND hWnd;
	if(_pAddEditCaseDialog != NULL)
		hWnd = _pAddEditCaseDialog->GetHWND();
	else
		hWnd = Singleton::GetMainWnd();

	wstring path = BaseOperation::DirDialog(hWnd, defPath);
	if(path.length()==0 )
		return _T("");
	return path;
}

void InstallCaseUI::GetAppPath( _INSTALLS_CASE_DATA_TYPE type, vector<wstring>* pathList )
{INTERFUN;
	vector<wstring> files;
	if(type == _INSTALLS_CASE_DATA_IOS)
		files = BaseOperation::MultiFileDialog(Singleton::GetMainWnd(),  L"苹果应用程序(*.ipa)\0*.ipa\0苹果应用程序(*.pxl)\0*.pxl\0All Files (*.ipa;*.pxl)\0*.ipa;*.pxl\0\0");
	else
		files = BaseOperation::MultiFileDialog(Singleton::GetMainWnd(), L"安卓应用程序(*.apk)\0*.apk\0安卓应用程序(*.npk)\0*.npk\0All Files (*.apk;*npk)\0*.apk;*npk\0\0");

	if(files.size() > 0)
	{
		pathList->insert(pathList->end(), files.begin(), files.end());
	}
}

void InstallCaseUI::GetCaseName( wstring& caseName )
{INTERFUN;
	bool bCancel = false;
	wstring ret = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"InstallCase_Text_InputNewCaseName"), CommonUIFunc::LoadString(L"InstallCase_Text_CaseNewCase"), caseName, bCancel, 25);
	caseName = L"";
	if(!bCancel) 
	{
		while(CStrOperation::trimLeftW(ret, L" ").empty() && !bCancel)
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CaseCreateFailure")), ICON_WARNING);
			ret = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"InstallCase_Text_InputNewCaseName"), CommonUIFunc::LoadString(L"InstallCase_Text_CaseNewCase"), caseName, bCancel, 25);	
		}
		caseName = CStrOperation::trimLeftW(ret, L" ");
	}
}

bool InstallCaseUI::RemoveAppConfirm(bool isRemoveFile)
{INTERFUN;
	if(isRemoveFile)
		return CommonUIFunc::ConfirmDialog(L"您确认要从磁盘彻底删除选中的应用吗？", L"提示", L"#CommonUi_icon_TipWarning");
	else
		return CommonUIFunc::ConfirmDialog(L"您确认要移除选中的应用吗？", L"提示");
}

bool InstallCaseUI::RemoveCaseConfirm()
{INTERFUN;
	return CommonUIFunc::ConfirmDialog(L"你确认要删除选中的方案吗？", L"提示");
}

bool InstallCaseUI::RemoveFolderConfirm()
{INTERFUN;
	return CommonUIFunc::ConfirmDialog(L"你确认要移除该应用库目录吗？", L"提示");
}

bool InstallCaseUI::IsNeedInstalliTunesConfirm()
{INTERFUN;
	CommonUIFunc::MessageDialog(L"您的电脑上未安装iTunes，请安装后再进行操作！", L"提示");
	return true;
}

void InstallCaseUI::SelectCaseNode( wstring caseId )
{INTERFUN;
	for(int i =0;i< _pCaseListNode->num_child();i++ )
	{
		CTreeNodeUI* pNode = (CTreeNodeUI*)_pCaseListNode->get_child(i);
		if(pNode->GetId() == caseId)
		{
			_pTree->SelectItem(pNode);
			break;
		}
	}

}

void InstallCaseUI::ShowCaseList()
{INTERFUN;
	_pTree->SelectItem(3);
}


bool InstallCaseUI::Handle(const InstallCaseMsg* pMsg)
{INTERFUN;
	if (NULL == pMsg)
	{
		return false;
	}

	switch (pMsg->message)
	{
	case WM_CASE_ADD:
		{
			InstallsCaseData* caseData = (InstallsCaseData*)pMsg->wParam;
			this->AddCaseToView(caseData);
		}
		break;
	case WM_CASE_REMOVE:
		{
			int caseId = (int)pMsg->wParam;
			this->RemoveCaseFromView(caseId);
		}
		break;
	case WM_CASE_IOSAPPCHECKED:
		{
			int count = (int)pMsg->wParam;
			this->UpdateUpgrateCount(_INSTALLS_CASE_DATA_IOS, count);
		}
		break;
	case WM_CASE_ANDAPPCHECKED:
		{
			int count = (int)pMsg->wParam;
			this->UpdateUpgrateCount(_INSTALLS_CASE_DATA_ANDROID, count);
		}
		break;
	case WM_CASE_FOLDER_ADD:
		{
			AppCountByFolder* appFolder = (AppCountByFolder*)pMsg->wParam;
			_pFolderListUI->AddFolderToView(appFolder);
		}
		break;
	case WM_CASE_FOLDER_REMOVE:
		{
			AppCountByFolder* appFolder = (AppCountByFolder*)pMsg->wParam;
			_pFolderListUI->RemoveFolderFromView(appFolder);
		}
		break;
	case WM_CASE_TIMECHANGE:
	case WM_CASE_SET_NUMBER:
		{
			InstallsCaseData* caseData = (InstallsCaseData*)pMsg->lParam;
			this->UpdateCaseView(caseData);
		}
		break;
	case WM_CASE_APP_ADD:
		{
			int caseId = (int)pMsg->wParam;
			InstallsAppInfo* appInfo = (InstallsAppInfo*)pMsg->lParam;
			if(appInfo)
			{
				this->AddAppToView(caseId, appInfo);
			}
		}
		break;
	case WM_CASE_APP_ADD_RESULT:
		{
			int count = (int)pMsg->wParam;
			Model.AddAppToCaseByPathNotify(count);
		}
		break;
	case WM_CASE_APP_REMOVE:
		{
			int caseId = (int)pMsg->wParam;
			int appId = (int)pMsg->lParam;
			InstallsCaseData* casedata =  Model.GetCaseById(caseId);
			if(casedata)
			{
				InstallsAppInfo* info = casedata->GetAppByID(appId);
				if(info)
				{
					this->RemoveAppFromView(caseId, info->m_strSavePath);
				}
			}
		}
		break;
	case WM_CASE_CLEARAPPIST:
		{
			int caseId = (int)pMsg->wParam;
			AppSearchContextEx* context = (AppSearchContextEx*)pMsg->lParam;
			this->ClearAppFromView(caseId, context);
		}
		break;
	case WM_CASE_APP_UPGRATECANCEL:
		{
			int caseId = (int)pMsg->wParam;
			UpgrateCancel* param = (UpgrateCancel*)pMsg->lParam;
			if (NULL != param)
			{
				wstring appID = param->wstrAppId;
				TASK_UPGRADE_TYPE type = (TASK_UPGRADE_TYPE)param->nUpgradetype;
				wstring oldfullpath = param->wstrOldappfullpath;
				InstallsCaseData* casedata =  Model.GetCaseById(caseId);
				if(casedata)
				{
					this->CancelAppUpgrate(caseId, oldfullpath);
				}
			}
		}
		break;
	case WM_CASE_FINISHREFRESH:
		{
			int caseId = (int)pMsg->wParam;
			FinishRefresh(caseId);
		}
		break;
	case WM_CASE_STARTUPREFRESH:
		{
			int caseId = (int)pMsg->wParam;
			this->ClearAppFromView(caseId);
		}
		break;
	case WM_CASE_APP_ADD_FILTER:
		{
			int caseId = (int)pMsg->wParam;
			map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
			if(it == _mapUserCaseAppList.end())
				return true;

			InstallCaseAppInfoListUI* appListUI = it->second;	
			appListUI->OnFilter();
		}
		break;
	}

	return true;
}

void InstallCaseUI::AddCaseToView( InstallsCaseData* caseData )
{INTERFUN;
	if(caseData == NULL)
		return;

	switch (caseData->PlanType())
	{
	case _INSTALL_PLAN_TYPE_NET:
	case _INSTALL_PLAN_TYPE_LEGALUPGRADE:
		break;
	case _INSTALL_PLAN_TYPE_UPGRATE:
		{
			if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
				_pUpgrateIOSBtn->SetUserData((UINT)caseData);
			else
				_pUpgrateAndroidBtn->SetUserData((UINT)caseData);
		}
		break;
	default:
		this->AddUserCaseToView(caseData);
		break;
	}	
}

void InstallCaseUI::RemoveCaseFromView( int caseId )
{INTERFUN;
	//如果删除的方案是当前树节点，则选中装机方案根节点
	if(_wtoi(_pTree->GetCurSel()->GetId()) == caseId )
	{
		_pTree->SelectItem(_pCaseListNode);
	}

	for(int i=0;i<_pCaseListNode->num_child();i++)
	{
		CTreeNodeUI* item = (CTreeNodeUI*)_pCaseListNode->get_child(i);
		if(_wtoi(item->GetId()) == caseId)
		{
			//先删除树节点，应为树节点选中之后才会创建list，所以删除list放在后面
			_pTree->Remove(item);
			_pTree->Resize();
			_pTree->Invalidate();

			//如果list创建过就删除
			map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
			if(it == _mapUserCaseAppList.end())
				break;

			InstallCaseAppInfoListUI* appList = it->second;	

			this->_pUserCaseLayout->Remove(appList);

			_mapUserCaseAppList.erase(it);

			break;
		}
	}
	this->_pCaseListUI->RemoveCaseFromView(caseId);
}

void InstallCaseUI::AddAppToView( int caseId, InstallsAppInfo* appInfo )
{INTERFUN;
	if(_pAddEditCaseDialog != NULL)
		_pAddEditCaseDialog->AddAppInfoToView(caseId, appInfo);

	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it == _mapUserCaseAppList.end())
		return;

	InstallCaseAppInfoListUI* appListUI = it->second;	
	appListUI->AddAppInfoToView(caseId, appInfo);
	if( 2==caseId || 3==caseId)
	{
		int count = appListUI->GetAppInfoItemsCount();
		UpdateSystemCaseCount(caseId, count);
	}
}

void InstallCaseUI::RemoveAppFromView( int caseId, wstring appPath )
{INTERFUN;
	if(_pAddEditCaseDialog != NULL)
		_pAddEditCaseDialog->RemoveAppInfoFromView(caseId, appPath);

	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it == _mapUserCaseAppList.end())
		return;

	InstallCaseAppInfoListUI* appListUI = it->second;	
	appListUI->RemoveAppInfoFromView(appPath);
	if( 2==caseId || 3==caseId)
	{
		int count = appListUI->GetAppInfoItemsCount();
		UpdateSystemCaseCount(caseId, count);
	}
}

void InstallCaseUI::ClearAppFromView( int caseId, AppSearchContextEx* context )
{INTERFUN;
	if(_pAddEditCaseDialog != NULL)
		_pAddEditCaseDialog->ClearAppFromView(caseId, context);

	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it == _mapUserCaseAppList.end())
		return;

	InstallCaseAppInfoListUI* appListUI = it->second;	
	appListUI->ClearAppFromView(context);
	if( 2==caseId || 3==caseId)
	{
		UpdateSystemCaseCount(caseId, 0);
	}
}

void InstallCaseUI::ClearAppFromView( int caseId )
{INTERFUN;
	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it == _mapUserCaseAppList.end())
		return;
	InstallCaseAppInfoListUI* appListUI = it->second;	
	appListUI->ClearAppFromView();
}
void InstallCaseUI::UpdateUpgrateCount( _INSTALLS_CASE_DATA_TYPE caseType, int count )
{INTERFUN;
	wstring tabName = CommonUIFunc::LoadStringW(L"InstallCase_Text_SofeWareUpgradeable");
	if(count > 0)
	{
		tabName += L"(";
		tabName += CStrOperation::IntToWString(count);
		tabName += L")";
	}

	if(caseType == _INSTALLS_CASE_DATA_IOS)
	{
		_pUpgrateIOSBtn->SetText(tabName.c_str());
		_pUpgrateIOSBtn->Resize();
		_pUpgrateIOSBtn->Invalidate();
	}
	else
	{
		_pUpgrateAndroidBtn->SetText(tabName.c_str());
		_pUpgrateAndroidBtn->Resize();
		_pUpgrateAndroidBtn->Invalidate();
	}
}

void InstallCaseUI::UpdateCaseView( InstallsCaseData* caseData )
{INTERFUN;
	if(_pAddEditCaseDialog != NULL)
		_pAddEditCaseDialog->UpdateCaseView(caseData->Id());
	CTreeNodeUI* pNode = NULL;
	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		caseData = Model.GetCaseById(caseData->Id());
		for(int i =0;i< _pSoftLibNode->num_child();i++ )
		{
			CTreeNodeUI* tmp = (CTreeNodeUI*)_pSoftLibNode->get_child(i);
			if(_wtoi(tmp->GetId()) == caseData->Id())
			{
				pNode = tmp;
				break;
			}
		}
	}
	else if(caseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
	{
		for(int i =0;i< _pCaseListNode->num_child();i++ )
		{
			CTreeNodeUI* tmp = (CTreeNodeUI*)_pCaseListNode->get_child(i);
			if(_wtoi(tmp->GetId()) == caseData->Id())
			{
				pNode = tmp;
				break;
			}
		}
	}
	else
	{
		if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			this->UpdateUpgrateCount(_INSTALLS_CASE_DATA_IOS, caseData->ListApp()->size());

		}
		else
		{
			this->UpdateUpgrateCount(_INSTALLS_CASE_DATA_ANDROID, caseData->ListApp()->size());
		}
	}
	if(pNode != NULL)
	{
		pNode->SetText(caseData->CaseName().c_str());
		pNode->SetAttribute(L"counttxt.text",GetCountStr(caseData).c_str()); 
		pNode->SetToolTip((caseData->CaseName()+GetCountStr(caseData)).c_str());
		_pTree->Invalidate();

		if(caseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
			this->_pCaseListUI->UpdateCaseToView(caseData);
	}

	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseData->Id());
	if(it == _mapUserCaseAppList.end())
		return;
	InstallCaseAppInfoListUI* appListUI = it->second;
	appListUI->UpdateCaseInfo();
}

void InstallCaseUI::OnCreate()
{INTERFUN;
	if (_pLeftView)
	{
		_pTree   = _pLeftView->_pTree;
	assert(_pTree);
		_pTree->SetINotifyUI(this);
	}

	_pAppListIOSBtn = dynamic_cast<CRadioUI*>(GetItem(AppListIOSBtnID));
	_pUpgrateIOSBtn = dynamic_cast<CRadioUI*>(GetItem(UpgrateIOSBtnID));
	_pAppListAndroidBtn = dynamic_cast<CRadioUI*>(GetItem(AppListAndroidBtnID));
	_pUpgrateAndroidBtn = dynamic_cast<CRadioUI*>(GetItem(UpgrateAndroidBtnID));

	_pFolderLayout = dynamic_cast<CLayoutUI*>(GetItem(FolderLayoutID));
	_pIOSLibSoftLayout = dynamic_cast<CLayoutUI*>(GetItem(IOSLibSoftLayoutID));
	_pAndroidLibSoftLayout = dynamic_cast<CLayoutUI*>(GetItem(AndroidLibSoftLayoutID));
	_pUserCaseListLayout = dynamic_cast<CLayoutUI*>(GetItem(UserCaseListLayoutID));
	_pUserCaseLayout = dynamic_cast<CLayoutUI*>(GetItem(UserCaseLayoutID));

	this->OnInitComposite();

	_pTree->SetGroupHeight(39);
	_pTree->SetItemHeight(39);
	_pTree->SetSelectedHeight(39);

	_pSoftLibNode = new CTreeNodeUI(true);
	_pSoftLibNode->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_SoftWareLib"));
	_pSoftLibNode->SetStyle(_T("Style_US_GroupTreeNode"));
	_pSoftLibNode->SetId(_T("SoftLib"));		
	_pTree->InsertItem(_pSoftLibNode);
	_pSoftLibNode->Expand(true);

	_pCaseListNode = new CTreeNodeUI(true);
	_pCaseListNode->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_InstallCase"));
	_pCaseListNode->SetStyle(_T("Style_US_GroupTreeNode"));
	_pCaseListNode->SetId(_T("CaseList"));		
	_pTree->InsertItem(_pCaseListNode);
	_pCaseListNode->Expand(true);

	_pTree->SelectItem(_pSoftLibNode);

	Model.Load();
}

void InstallCaseUI::OnInitComposite()
{INTERFUN;
_pFolderListUI					= new InstallCaseAppFolderListUI(this);
	_pCaseListUI						= new InstallCaseListUI(this);

	_pFolderListUI->Create(L"InstallCaseAppFolderList", _pFolderLayout);
	_pCaseListUI->Create(L"InstallCaseList", _pUserCaseListLayout);
}

wstring InstallCaseUI::GetCountStr( InstallsCaseData* caseData )
{INTERFUN;
	wstring countStr;
	countStr	=	L"(";
	countStr	+=	CStrOperation::IntToWString(caseData->ListApp()->size());
	countStr	+=	L")";
	return countStr;
}

bool InstallCaseUI::OnShowTreeMenu(TNotifyUI* pNotify)
{INTERFUN;
	if(pNotify->lParam==0)
		return true;
	CTreeNodeUI* pCur = (CTreeNodeUI*)pNotify->lParam;
	if(pCur == NULL)
		return true;

	CMenuUI* pMenu=NULL;
	if(pCur == _pCaseListNode)
	{
		pMenu = CMenuUI::Load(IDR_MENU_NEW_CASE);
	}
	else if(_pCaseListNode == pCur->get_parent())
	{
		pMenu = CMenuUI::Load(IDR_MENU_CASE_LIST);
		pMenu->Remove(0);
	}
	if (pMenu != NULL)
	{		
		pMenu->SetParam(pNotify->wParam,pNotify->lParam);
		pMenu->Show(Singleton::GetMainWnd(), NULL, this);
	}
	return true;
}

bool InstallCaseUI::OnClickMenuNewCase(TNotifyUI* pNotify)
{INTERFUN;

	Model.AddCase(_INSTALLS_CASE_DATA_IOS, NULL, true);
	return true;
}

bool InstallCaseUI::OnClickMenuCaseList(TNotifyUI* pNotify)
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;

	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	CControlUI* item = (CControlUI*)pInfo->lParamNotify;
	if(item == NULL)
		return true;

	int nID=_wtoi(item->GetId());
	switch (pInfo->uID)
	{
	case IDR_MENU_EDIT_CASE:
		Model.EditCase(nID);
		break;
	case IDR_MENU_DELETE_CASE:
		{
			StatisRemoveCase();

			vector<int> idList;
			idList.push_back(nID);

			Model.RemoveCase(&idList);
		}
		break;
	}
	return true;
}

bool InstallCaseUI::OnCaseSelChanged(TNotifyUI* pNotify)
{INTERFUN;
	StatisCaseNodeClick();

	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	bool folder = false;
	bool iosLst = false;
	bool androidLst  = false;
	bool userCaseLst  = false;
	bool userCaseGroup = false;

	CLayoutUI* refreshLayout = NULL;
	if(idStr == L"SoftLib")
	{
		folder = true;
		refreshLayout = _pFolderLayout;
	}
	else if(idStr == L"CaseList")
	{
		userCaseLst = true;
		refreshLayout = _pUserCaseListLayout;
	}
	else
	{
		InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();
		int id = caseData->Id();
		if(id == Sys_Case_IOS_ID)
		{
			iosLst = true;
			refreshLayout = _pIOSLibSoftLayout;
			if(!_pUpgrateIOSBtn->IsChecked())
				this->ShowUserCaseAppList(_pIOSLibSoftLayout, caseData);
		}
		else if(id == Sys_Case_Android_ID)
		{
			androidLst = true;
			refreshLayout = _pAndroidLibSoftLayout;
			if(!_pUpgrateAndroidBtn->IsChecked())
				this->ShowUserCaseAppList(_pAndroidLibSoftLayout, caseData);
		}
		else
		{
			userCaseGroup = true;
			refreshLayout = _pUserCaseLayout;
			this->ShowUserCaseAppList(_pUserCaseLayout, caseData);
		}
	}

	_pFolderLayout->SetVisible(folder);
	_pIOSLibSoftLayout->SetVisible(iosLst);
	_pAndroidLibSoftLayout->SetVisible(androidLst);
	_pUserCaseListLayout->SetVisible(userCaseLst);
	_pUserCaseLayout->SetVisible(userCaseGroup);
	refreshLayout->Resize();
	refreshLayout->Invalidate();

	return true;
}

bool InstallCaseUI::OnTabItemClick( TNotifyUI* pNotify )
{INTERFUN;
	StatisSwitchToUpgrate();

	CControlUI* pBtn = pNotify->pSender;
	InstallsCaseData* caseData = (InstallsCaseData*)pBtn->GetUserData();
	wstring id = pBtn->GetId();
	if(id == L"AppListIOSBtn" || id == L"UpgrateIOSBtn")
	{
		this->ShowUserCaseAppList(this->_pIOSLibSoftLayout, caseData);
	}
	else
	{
		this->ShowUserCaseAppList(this->_pAndroidLibSoftLayout, caseData);
	}
	return true;
}

void InstallCaseUI::AddUserCaseToView(InstallsCaseData* caseData)
{INTERFUN;
	CTreeNodeUI* pNode = new CTreeNodeUI();
	pNode->SetStyle(_T("US_CaseNode"));
	pNode->SetText(caseData->CaseName().c_str());
	pNode->SetId(CStrOperation::IntToWString(caseData->Id()).c_str());
	pNode->SetAttribute(L"counttxt.text",GetCountStr(caseData).c_str()); 
	pNode->SetToolTip((caseData->CaseName()+GetCountStr(caseData)).c_str());
	pNode->SetToolTipShow(true);
	pNode->SetUserData((UINT)caseData);
	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		pNode->SetImage(_T("#InstallCase_Image_icon_Apple"));
	else
		pNode->SetAttribute(_T("icon.image"),_T("#InstallCase_Image_icon_Android"));

	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
			_pAppListIOSBtn->SetUserData((UINT)caseData);
		else
			_pAppListAndroidBtn->SetUserData((UINT)caseData);

		_pTree->InsertItem(pNode, _pSoftLibNode);
	}
	else
	{
		_pTree->InsertItem(pNode, _pCaseListNode);

		unsigned int unTimeBegin = GetTickCount();
		this->_pCaseListUI->AddCaseToView(caseData);
		unsigned int unTimeEnd = GetTickCount();
		wstring wsTemp = L"AddCaseToView(caseData)--InstallCaseMsg";
		wsTemp = wsTemp + CStrOperation::IntToWString(unTimeEnd-unTimeBegin);
		LOG->WriteDebug(wsTemp.c_str());
	}

	_pTree->Invalidate();
}

void InstallCaseUI::CancelAppUpgrate( int caseId, wstring appPath )
{INTERFUN;
	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it != _mapUserCaseAppList.end())
	{
		InstallCaseAppInfoListUI* appListUI = _mapUserCaseAppList[caseId];
		appListUI->CancelAppUpgrate(appPath);
	}
	map<int,InstallCaseAppInfoListUI*>::iterator itplan;
	for(itplan = _mapUserCaseAppList.begin();itplan!=_mapUserCaseAppList.end();itplan++)
	{
		if(itplan->first == 0 || itplan->first ==1)
			continue;
		itplan->second->CancelAppUpgrateEx(appPath);
	}
}

void InstallCaseUI::RefreshCaseItemsIcon( int caseId )
{INTERFUN;
	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseId);
	if(it == _mapUserCaseAppList.end())
		return;

	InstallCaseAppInfoListUI* appListUI = it->second;
}

void InstallCaseUI::ShowUserCaseAppList( CLayoutUI* pContainer, InstallsCaseData* caseData )
{INTERFUN;
	InstallCaseAppInfoListUI* appListUI = NULL;
	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseData->Id());
	if(it == _mapUserCaseAppList.end())
	{
		appListUI = new InstallCaseAppInfoListUI(Singleton::GetMainWnd(), this, caseData);
		appListUI->Create(L"InstallCaseAppList", pContainer);
		appListUI->SetId(CStrOperation::IntToWString(caseData->Id()).c_str());
		_mapUserCaseAppList[caseData->Id()] = appListUI;
		_mapUserCaseAppList[caseData->Id()]->Search();
	}
	else
	{
		appListUI = _mapUserCaseAppList[caseData->Id()];
	}
	if(appListUI != NULL)
	{
		for (int i = 0; i < pContainer->GetCount(); i++)
		{
			InstallCaseAppInfoListUI* pUI = dynamic_cast<InstallCaseAppInfoListUI*>(pContainer->GetItem(i));
			if(pUI != NULL && pUI->GetId() != appListUI->GetId())
				pUI->SetVisible(false);
		}
		appListUI->SetVisible(true);

		pContainer->Resize();
		pContainer->Invalidate();
	}
}


/**********************************************************************************************************************************************/
//																统计
/**********************************************************************************************************************************************/

void InstallCaseUI::StatisCaseNodeClick()
{INTERFUN;
	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();
	if(idStr == L"SoftLib")
	{
		//点击【本地资源】节点
		CStaticsticsMessage StaticMsg(Statistic_ID_LocalResBnt);
		StaticMsg.Send();

	}
	else if(idStr == L"CaseList")
	{
		//点击【装机方案】节点
		CStaticsticsMessage StaticMsg(Statistic_ID_InstallPlanBnt);
		StaticMsg.Send();

	}
	else
	{
		InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();
		int id = caseData->Id();
		if(id == Sys_Case_IOS_ID)
		{
			//点击【iOS软件库】节点
			CStaticsticsMessage StaticMsg(Statistic_ID_IosAppLibrary);
			StaticMsg.Send();
		}
		else if(id == Sys_Case_Android_ID)
		{
			//点击【Android软件库】节点
			CStaticsticsMessage StaticMsg(Statistic_ID_AndroidAppLibrary);
			StaticMsg.Send();
		}
		else
		{
		}
	}
}

void InstallCaseUI::StatisSingleInstall()
{INTERFUN;

	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		//iOS软件库：点击单个软件【安装】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_IosSingleInstall);
		StaticMsg.Send();
	}
	else if(id == Sys_Case_Android_ID)
	{
		//Android软件库：点击单个软件【安装】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_AnSingleInstall);
		StaticMsg.Send();
	}
	else
	{
		//装机方案-某个方案：点击单个软件【安装】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_AnAppAddBnt);
		StaticMsg.Send();
	}
}

void InstallCaseUI::StatisSingUpgrade()
{INTERFUN;

	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		if(_pUpgrateIOSBtn->IsChecked())
		{
			//iOS软件库-可升级的应用：点击单个软件【升级】按钮
			CStaticsticsMessage StaticMsg(Statistic_ID_IosUpSingleCheck);
			StaticMsg.Send();

		}
	}
	else if(id == Sys_Case_Android_ID)
	{
		if(_pUpgrateAndroidBtn->IsChecked())
		{
			//Android软件库-可升级的应用：点击单个软件【升级】按钮
			CStaticsticsMessage StaticMsg(Statistic_ID_AnUpSingleCheck);
			StaticMsg.Send();
		}
	}
	else
	{
	}
}

void InstallCaseUI::StatisSingleRemove()
{INTERFUN;

	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		//iOS软件库：点击单个软件【删除】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_IosSingleDelete);
		StaticMsg.Send();
	}
	else if(id == Sys_Case_Android_ID)
	{
		//Android软件库：点击单个软件【删除】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_AnSingleDelete);
		StaticMsg.Send();
	}
	else
	{
		//装机方案-某个方案：点击单个软件【移除】按钮
		CStaticsticsMessage StaticMsg(Statistic_ID_AnAppMoveBnt);
		StaticMsg.Send();
	}
}

void InstallCaseUI::StatisRemoveCase()
{INTERFUN;
	//装机方案-某个方案：点击单个软件【移除】按钮
	CStaticsticsMessage StaticMsg(Statistic_ID_AnAppMoveBnt);
	StaticMsg.Send();

}

void InstallCaseUI::StatisShowLaseVer()
{INTERFUN;
	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		if(_pAppListIOSBtn->IsChecked())
		{
			//iOS软件库：勾选【相同应用仅显示最新版本】
			CStaticsticsMessage StaticMsg(Statistic_ID_IosSameCheck);
			StaticMsg.Send();

		}
	}
	else if(id == Sys_Case_Android_ID)
	{
		if(_pAppListAndroidBtn->IsChecked())
		{
			//Android软件库：勾选【相同应用仅显示最新版本】
			CStaticsticsMessage StaticMsg(Statistic_ID_AnSameCheck);
			StaticMsg.Send();
		}
	}
	else
	{
	}
}

void InstallCaseUI::StateAddAppAndBatchUpgrate()
{INTERFUN;

	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		if(_pUpgrateIOSBtn->IsChecked())
		{
			//iOS软件库-可升级的应用：点击【升级】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_IosUpBatchCheck);
			StaticMsg.Send();
		}
		else
		{
			//iOS软件库：点击【添加】按钮
			CStaticsticsMessage StaticMsg(Statistic_ID_IosAddBnt);
			StaticMsg.Send();
		}
	}
	else if(id == Sys_Case_Android_ID)
	{
		if(_pUpgrateAndroidBtn->IsChecked())
		{
			//Android软件库-可升级的应用：点击【升级】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_AnUpBatchCheck);
			StaticMsg.Send();
		}
		else
		{
			//Android软件库：点击【添加】按钮
			CStaticsticsMessage StaticMsg(Statistic_ID_AnAddBnt);
			StaticMsg.Send();
		}
	}
	else
	{
	}
}

void InstallCaseUI::StatisCreateNewPlan()
{INTERFUN;
	//装机方案：点击【创建新方案】按钮
	CStaticsticsMessage StaticMsg(Statistic_ID_CreateNewPlan);
	StaticMsg.Send();
}

void InstallCaseUI::StatisAddAndroidFolder()
{INTERFUN;
	//本地资源：点击【添加】-【Android软件库】按钮
	CStaticsticsMessage StaticMsg(Statistic_ID_AddAnLibraryBnt);
	StaticMsg.Send();
}

void InstallCaseUI::StatisAddIOSFolder()
{INTERFUN;
	//本地资源：点击【添加】-【iOS软件库】按钮
	CStaticsticsMessage StaticMsg(Statistic_ID_AddIosLibraryBnt);
	StaticMsg.Send();
}

void InstallCaseUI::StatisSwitchToUpgrate()
{INTERFUN;
	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();

	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		if(_pUpgrateIOSBtn->IsChecked())
		{
			////点击【iOS软件库】-【可升级的应用】
			CStaticsticsMessage StaticMsg(Statistic_ID_IosLibraryUpDate);
			StaticMsg.Send();
		}
	}
	else if(id == Sys_Case_Android_ID)
	{
		if(_pUpgrateAndroidBtn->IsChecked())
		{
			//点击【Android软件库】-【可升级的应用】
			CStaticsticsMessage StaticMsg(Statistic_ID_AnLibraryUpDate);
			StaticMsg.Send();
		}
	}
	else
	{
	}
}

void InstallCaseUI::StatisBatchRemove()
{INTERFUN;
	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();
	if(idStr == L"CaseList")
	{
		//装机方案：点击【删除所选方案】按钮（批量）
		CStaticsticsMessage StaticMsg(Statistic_ID_DeleteAllSelect);
		StaticMsg.Send();
	}
	else
	{
		InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

		if(caseData == NULL)
			return;

		int id = caseData->Id();
		if(id == Sys_Case_IOS_ID)
		{
			//iOS软件库：点击【删除】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_IosBatchDelete);
			StaticMsg.Send();
		}
		else if(id == Sys_Case_Android_ID)
		{
			//Android软件库：点击【删除】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_AnBatchDelete);
			StaticMsg.Send();
		}
		else
		{
			//装机方案-某个方案：点击【移除】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_ABatchMoveBnt);
			StaticMsg.Send();
		}
	}
}

void InstallCaseUI::StatisOnKeyInstall()
{INTERFUN;
	CControlUI* pSel = _pTree->GetCurSel();
	wstring idStr = pSel->GetId();


	InstallsCaseData* caseData = (InstallsCaseData*)pSel->GetUserData();

	if(caseData == NULL)
		return;

	int id = caseData->Id();
	if(id == Sys_Case_IOS_ID)
	{
		if(_pUpgrateIOSBtn->IsChecked())
		{
			//iOS软件库-可升级的应用：点击【一键装机】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_IosUpBatchOneKey);
			StaticMsg.Send();
		}
		else
		{
			//iOS软件库：点击【一键装机】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_IosBatchInstall);
			StaticMsg.Send();
		}
	}
	else if(id == Sys_Case_Android_ID)
	{
		if(_pUpgrateAndroidBtn->IsChecked())
		{
			//Android软件库-可升级的应用：点击【一键装机】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_AnUpBatchOneKey);
			StaticMsg.Send();
		}
		else
		{
			//Android软件库：点击【一键装机】按钮（批量）
			CStaticsticsMessage StaticMsg(Statistic_ID_AnBatchInstall);
			StaticMsg.Send();
		}
	}
	else
	{
		//装机方案-某个方案：点击【一键装机】按钮（批量）
		CStaticsticsMessage StaticMsg(Statistic_ID_ABatchInstallBnt);
		StaticMsg.Send();
	}
}

void InstallCaseUI::UpdateSystemCaseCount( int caseID, int count )
{INTERFUN;
	wstring tabName = CommonUIFunc::LoadStringW(L"InstallCase_Text_SofeWareList");
	if(count > 0)
	{
		tabName += L"(";
		tabName += CStrOperation::IntToWString(count);
		tabName += L")";
	}

	if(caseID == 2)
	{
		_pAppListIOSBtn->SetText(tabName.c_str());
		_pAppListIOSBtn->Resize();
		_pAppListIOSBtn->Invalidate();
	}
	else
	{
		_pAppListAndroidBtn->SetText(tabName.c_str());
		_pAppListAndroidBtn->Resize();
		_pAppListAndroidBtn->Invalidate();
	}
}

void InstallCaseUI::FinishRefresh(int caseID)
{INTERFUN;
	InstallCaseAppInfoListUI* appListUI = NULL;
	map<int,InstallCaseAppInfoListUI*>::iterator it = _mapUserCaseAppList.find(caseID);
	if(it != _mapUserCaseAppList.end())
	{
		InstallCaseAppInfoListUI* appListUI = appListUI = _mapUserCaseAppList[caseID];
		appListUI->FinishRefresh();
	}
}

INSTALLCASE_END