#include "stdafx.h"
#include "DevicePanelCaselistUI.h"
#include "CaseListInfoUI.h"
#include "math.h"
#include "ui/InstallCaseItem.h"
#define CASE_TIMER_ID 101

bool DevicePanelCaselistUI::_bCanBtnClick = true;
DevicePanelCaselistUI::DevicePanelCaselistUI()
:_CasePanel(NULL)
,_CasePages(NULL)
{INTERFUN;
	nLoadCaseNum=0;
	mCurrPage = 1;
	_BtnAddNewCase = NULL;

	_NetCasePanel = NULL;
	nLoadNetCaseNum=0;
	mNetCurrPage = 1;
	m_islocked= true;
	DevicePanelCaselistUI::_bCanBtnClick = true;

	_curdeviceId = L"";

	_bUIMiniSize = false;
}

wstring DevicePanelCaselistUI::GetStyleID()
{
	return L"CaseListPanel";
}

void DevicePanelCaselistUI::OnCreate()
{
	_CasePanel = (CAutoGridLayoutUI *)this->GetItem(_T("case_Layout.case_panel"));
	_CasePanel->SetAttribute(L"GridWidth",L"97");
	_CasePanel->SetAttribute(L"GridHeight",L"164");
	_CasePanel->SetAttribute(L"EnablePage",L"1");
	_CasePanel->SetAttribute(L"ShowRadio",L"0");
	_CasePanel->SetINotifyUI(this);
	_CasePages = (CLayoutUI *)this->GetItem(_T("case_Layout.Case_Bottom.Case_Pases"));

	_NetCasePanel = (CAutoGridLayoutUI *)this->GetItem(_T("netcase_Layout.netcase_panel"));

	_NetCasePanel->SetAttribute(L"GridWidth",L"97");
	_NetCasePanel->SetAttribute(L"GridHeight",L"164");
	_NetCasePanel->SetAttribute(L"EnablePage",L"1");
	_NetCasePanel->SetAttribute(L"ShowRadio",L"0");
	_NetCasePanel->SetINotifyUI(this);

	_NetCasePages = (CLayoutUI *)this->GetItem(_T("netcase_Layout.NetCase_Bottom.NetCase_Pases"));

	_pEmptyCaseTip = (CLayoutUI *)this->GetItem(_T("EmptyCaseTip"));
	_pCase_Bottom = (CLayoutUI *)this->GetItem(_T("case_Layout.Case_Bottom"));
	_pNetCase_Bottom = (CLayoutUI *)this->GetItem(_T("netcase_Layout.NetCase_Bottom"));


	_CasePanel->AddStaticItem(_pCase_Bottom->GetItem(L"moresolution"));
	_NetCasePanel->AddStaticItem(_pNetCase_Bottom->GetItem(L"moresolution"));


	RefreshSolution();

	// 拖放文件夹创建提示框
	wstring showEmptyTip = MyFileOperation::GetIniFileValue(CONFIG(_strBizDictionaryIni), _T("Default"), _T("ShowEmptyTip"));
	if (_wtoi(showEmptyTip.c_str()) <= 0 && showEmptyTip != L"")
	{
		CLayoutUI* EmptyCaseTip = (CLayoutUI *)this->GetItem(_T("EmptyCaseTip"));
		EmptyCaseTip->SetVisible(false);
	}

}
void DevicePanelCaselistUI::RefreshSolution(bool addplan)
{INTERFUN;

	ShowCurrPageCaseData(addplan);
	InsertPages(addplan);

	this->Resize();
	this->Invalidate();

}


// 根据分页显示装机方案数据
void DevicePanelCaselistUI::ShowCurrPageCaseData(bool addplan)
{INTERFUN;
	if (!_CasePanel) return; //出错了

	_CasePanel->RemoveAll();
	nLoadCaseNum = 0;

	DeviceData *pDevData =	NULL;
	if(_curdeviceId.length()>0)
	pDevData=DEVICE_MANAGER->FindDevice(_curdeviceId);

	vector<InstallsCaseData *> netcaselist;
	vector<InstallsCaseData *> caselist;

	GetNetInstallsCaseDataListMsg GetDataListMsg;
	GetDataListMsg.SendAndGetReturn();

	GetSortByUseTimeCaseListMsg GetCaseListMsg;
	GetCaseListMsg.SendAndGetReturn();
	if (NULL != GetDataListMsg.pDataList)
	{
		netcaselist = *((vector<InstallsCaseData *>*)GetDataListMsg.pDataList);
	}
	if (NULL != GetCaseListMsg.pDataList)
	{
		caselist = *((vector<InstallsCaseData *>*)GetCaseListMsg.pDataList);
	}

	vector<InstallsCaseData *> nettempList;
	vector<InstallsCaseData *> netCpatempList;
	vector<InstallsCaseData *> tempList;
	GetCurDeviceCaseList(netcaselist,nettempList,pDevData);

	GetCurDeviceCaseList(caselist,tempList,pDevData);

	//有cpa权限，则显示cpa方案

	GetLoginUserInfoMsg msg;
	msg.SendAndGetReturn();
	if (msg.pLoginInfo.isCPACompetence)
	{
		int num = 0;
		GetCurDeviceCaseList(netcaselist,netCpatempList,pDevData, true);
		vector<InstallsCaseData*>::iterator iter = netCpatempList.begin();
		for (; iter != netCpatempList.end(); ++iter)
		{
			NetInstallsCaseData *casedata = dynamic_cast<NetInstallsCaseData*>(*iter);
			if (NULL==casedata)
			{
				continue;
			}
			bool deviceisnull = pDevData==NULL ? true : false;
			InsertCase(casedata,casedata->CaseName(),casedata->GetSumFileSize(),casedata->GetAppNum(),
				(UIStringFormat::InstallNeedTime(casedata->GetAppNum(),casedata->GetSumFileSize(),num)),
				_CasePanel, deviceisnull);
		}
	}


	int netstartindex=0;
	int startindex=0;
	int netendindex=nettempList.size();
	int endindex=tempList.size()-1;

	while(netstartindex<netendindex||startindex<endindex)
	{
		int netcnt=0, num=0;

		for (size_t no = netstartindex; no < nettempList.size() ; no++)
		{
			//删除网络推荐包加载
			/*NetInstallsCaseData *casedata = static_cast<NetInstallsCaseData*>(nettempList.at(no));
			bool deviceisnull = pDevData==NULL ? true : false;
			InsertCase(casedata,casedata->CaseName(),casedata->GetSumFileSize(),casedata->GetAppNum(),
				(UIStringFormat::InstallNeedTime(casedata->GetAppNum(),casedata->GetSumFileSize(),num)),
				_CasePanel, deviceisnull);*/
			netcnt++;
			if(netcnt>4)
				break;
		}
		netstartindex+=netcnt;
		int cnt=0;
		for (size_t no = startindex; no < tempList.size() ; no++)
		{
			InstallsCaseData *casedata = tempList.at(no);
			if(casedata->PlanType() != _INSTALL_PLAN_TYPE_USER)
			{
				startindex++;
				continue;
			}
			InsertCase(casedata,casedata->CaseName(),casedata->GetSumFileSize(),casedata->GetAppNum(),
				(UIStringFormat::InstallNeedTime(casedata->GetAppNum(),casedata->GetSumFileSize(),num)),
				_CasePanel
				);
			cnt++;
			if(cnt>4)
				break;
		}
		startindex+=cnt;
	}

	InsertAddCaseButton();

	_CasePanel->Resize();
	_CasePanel->Invalidate();
}


void  DevicePanelCaselistUI::GetCurDeviceCaseList(vector<InstallsCaseData *> caselist,vector<InstallsCaseData *>& Validatecaselist, const DeviceData* pDevData, bool bCpa)
{
	if(!pDevData)
	{
		for ( vector<InstallsCaseData*>::iterator it = caselist.begin(); it != caselist.end(); ++it ) 
		{
			InstallsCaseData* casedata = *it;

			NetInstallsCaseData* pData = dynamic_cast<NetInstallsCaseData*>(casedata);
			if (pData&&pData->IsbCpa()!=bCpa)
				continue;
			Validatecaselist.push_back(casedata);
		}
		return;
	}
	_INSTALLS_CASE_DATA_TYPE deviceType = pDevData->GetDevInfo()->_deviceType == Android ? _INSTALLS_CASE_DATA_ANDROID : _INSTALLS_CASE_DATA_IOS;
	for ( vector<InstallsCaseData*>::iterator it = caselist.begin(); it != caselist.end(); ++it ) 
	{
		InstallsCaseData* casedata = *it;

		if( casedata->CaseType() == deviceType)
		{
			if(casedata->CaseType() == _INSTALLS_CASE_DATA_IOS 
				&& !pDevData->GetDevInfo()->_ios_bJailBreaked 
				&& casedata->IsLegalPlan() != 1)
				continue;

			NetInstallsCaseData* pData = dynamic_cast<NetInstallsCaseData*>(casedata);
			if (pData&&pData->IsbCpa()!=bCpa)
				continue;

			Validatecaselist.push_back(casedata);
		}
	}

}

CControlUI* DevicePanelCaselistUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{INTERFUN;
	if(_pEmptyCaseTip->IsVisible())	
	{
		return _pEmptyCaseTip->FindControl(Proc, pData, uFlags);
	}
	else if(_CasePanel->IsVisible())
	{
		CControlUI* ctrl = _CasePanel->FindControl(Proc, pData, uFlags);
		if(ctrl == NULL)
		{
			ctrl = _pCase_Bottom->FindControl(Proc, pData, uFlags);
		}
		return ctrl;
	}
	else
	{
		CControlUI* ctrl = _NetCasePanel->FindControl(Proc, pData, uFlags);
		if(ctrl == NULL)
		{
			ctrl = _pNetCase_Bottom->FindControl(Proc, pData, uFlags);
		}
		return ctrl;
	}
}

void DevicePanelCaselistUI::InsertCase(InstallsCaseData *CaseData, wstring name, size_t nSize , UINT nCount , wstring nInstallTime , CContainerUI* casePanel, bool showNetDeviceNull)
{INTERFUN;

	//避免重复添加
	CControlUI* pUI=_CasePanel->GetItem(CStrOperation::IntToWString(CaseData->Id()).c_str());
	if (NULL!=pUI)
	{
		return;
	}
	int tempInt = 0;
	if(casePanel == _CasePanel)
	{
		tempInt = nLoadCaseNum;
	}
	else
	{
		tempInt = nLoadNetCaseNum;
	}


	CLayoutUI * Caseitem = new CLayoutUI();

	Caseitem->SetInitPos(0,0,97,164,0);
	Caseitem->SetId(CStrOperation::IntToWString(CaseData->Id()).c_str());

	InstallCaseItem* item=new InstallCaseItem(CaseData,_curdeviceId);
	item->Create(L"InstallCaseItemControl", Caseitem);
	item->LockCase(m_islocked);

	if(casePanel==_CasePanel)
	{
		CControlUI* after=_CasePanel->GetItem(L"CasePanelBtnAddCaselayout");
		if(after)
		{
			if(casePanel->GetCount()>1)
				casePanel->Insert(Caseitem,casePanel->GetItem(casePanel->GetCount()-2));
			else
				casePanel->Insert(Caseitem,NULL);
			item->SetINotifyUI(item);
		}
		else
			casePanel->Add(Caseitem);
	}
	else
	casePanel->Add(Caseitem);

	if(casePanel == _CasePanel)
	{
		nLoadCaseNum++;
	}
	else
	{
		nLoadNetCaseNum++;
	}
}

// 添加装机方案按钮
void DevicePanelCaselistUI::InsertAddCaseButton()
{INTERFUN;

	CLayoutUI* lay=new CLayoutUI();
	lay->SetId(L"CasePanelBtnAddCaselayout");
	_BtnAddNewCase = new CButtonUI;
	_BtnAddNewCase->SetStyle(_T("RecentInstallCase_Stytle_CaseAdd_Button"));
	_BtnAddNewCase->SetId(_T("CasePanelBtnAddCase"));
	_BtnAddNewCase->SetInitPos(-1,25,85,129,9);
	_BtnAddNewCase->SetAttribute(_T("txt1.text"),CommonUIFunc::LoadString(L"RecentInstallCase_Text_Device_AddCase"));
	_BtnAddNewCase->SetINotifyUI(this);
	lay->Add(_BtnAddNewCase);
	_CasePanel->Add(lay);
}

// 方案分页
void DevicePanelCaselistUI::InsertPages(bool netCase, bool addplan)
{    INTERFUN;
return;
	if (!_CasePages) return;// 出错了
	if(netCase)
	{
		_NetCasePages->RemoveAll();
		mNetCurrPage = 1;
	}
	else
	{
		_CasePages->RemoveAll();
		//mCurrPage = 1;
	}

	DeviceData *pDevData =	NULL;
	if(_curdeviceId.length()>0)
	pDevData = DEVICE_MANAGER->FindDevice(_curdeviceId);

	vector<InstallsCaseData*> caselist;  //全部方案
	vector<InstallsCaseData*> Netcaselist; //网络方案

	GetNetInstallsCaseDataListMsg GetDataListMsg;
	GetDataListMsg.SendAndGetReturn();
	if (NULL != GetDataListMsg.pDataList)
	{
		Netcaselist = *((vector<InstallsCaseData*>*)GetDataListMsg.pDataList);
	}

	GetSortByUseTimeCaseListMsg NetGetCaseListMsg;
	NetGetCaseListMsg.SendAndGetReturn();
	if (NULL != NetGetCaseListMsg.pDataList)
	{
		caselist =*((vector<InstallsCaseData *>*)NetGetCaseListMsg.pDataList);
		for (vector<InstallsCaseData *>::iterator iter = caselist.begin(); iter != caselist.end();)
		{
			if((*iter)->PlanType() != _INSTALL_PLAN_TYPE_USER)
			{
				iter = caselist.erase(iter);
			}
			else
				++iter;
		}
		caselist.insert(caselist.end(), Netcaselist.begin(), Netcaselist.end());
	}

	vector<InstallsCaseData *> tempList;//适合当前条件的方案
	GetCurDeviceCaseList(caselist,tempList,pDevData);

	//有cpa权限，则显示cpa方案
	GetLoginUserInfoMsg msg;
	msg.SendAndGetReturn();
	if (msg.pLoginInfo.isCPACompetence)
	{
		GetCurDeviceCaseList(Netcaselist,tempList,pDevData, true);
	}

	int allCount = tempList.size();

	int Pages = 0;
	if (_curdeviceId.empty())
	{
		//首页显示方案
		Pages = (allCount+1)/WELCOME_CASE_PER_PAGE_COUNT + ((allCount+1)%WELCOME_CASE_PER_PAGE_COUNT == 0 ? 0 : 1);	
	}
	else
	{
		Pages = (allCount+1)/DEVICE_CASE_PER_PAGE_COUNT + ((allCount+1)%DEVICE_CASE_PER_PAGE_COUNT == 0 ? 0 : 1);	
	}


	if(Pages <= 1) 
	return;
	for(int no = 0;no < Pages;no++)
	{
		CButtonUI *BtnPage = new CButtonUI;
		BtnPage->SetId(_T("btnPageButton"));
		BtnPage->SetUserData((no+1));
		BtnPage->SetStyle(_T("RecentInstallCase_Stytle_DevicePanelCase_Page_Button"));
		BtnPage->SetInitPos(no*30,0,no*30+14,11,0);
		int currPage = netCase ? mNetCurrPage : mCurrPage;
		BtnPage->SetAttribute(_T("icon.imgseq"),currPage == no+1 ? L"1,0" : L"0,1");
		if(no == 0)
		{
			BtnPage->SetAttribute(_T("icon.image"),L"#RecentInstallCase_Image_Device_PanelCase_Page_Button_F");
		}
		else if(no == Pages -1)
		{
			BtnPage->SetAttribute(_T("icon.image"),L"#RecentInstallCase_Image_Device_PanelCase_Page_Button_L");
		}
		else
		{
			BtnPage->SetAttribute(_T("icon.image"),L"#RecentInstallCase_Image_Device_PanelCase_Page_Button_C");
		}

		BtnPage->SetAttribute(_T("cursor"),_T("hand"));
		BtnPage->SetINotifyUI(this);

		if(netCase)
			_NetCasePages->Add(BtnPage);
		else
			_CasePages->Add(BtnPage);
	}
	int iWidth = ((int)Pages-1) * 32 + 14 + 5;

	if(netCase)
	_NetCasePages->SetInitPos(9|-1,5,iWidth,18);
	else
	_CasePages->SetInitPos(9|-1,5,iWidth,18);
}



bool DevicePanelCaselistUI::OnAddnewCase(TNotifyUI* pNotify)
{INTERFUN;
	m_AddPlanStart = true;
	// 统计功能

	CStaticsticsMessage StaticMsg("10002");
	StaticMsg.Send();

	DeviceData *pDevData =	DEVICE_MANAGER->FindDevice(_curdeviceId);

	_INSTALLS_CASE_DATA_TYPE caseType = _INSTALLS_CASE_DATA_IOS;
	if(pDevData != NULL && pDevData->GetDevInfo() != NULL)
	caseType = pDevData->GetDevInfo()->_deviceType == Android? _INSTALLS_CASE_DATA_ANDROID:_INSTALLS_CASE_DATA_IOS;

	AddCaseMsg addCaseMsg;
	addCaseMsg.caseType = caseType;
	addCaseMsg.pPathList = NULL;
	addCaseMsg.fromLib = true;
	addCaseMsg.SendToUI();

	return true;
}
bool DevicePanelCaselistUI::OnShowCase(TNotifyUI* pNotify)
{INTERFUN;
	m_AddPlanStart = false;
	int caseid = _wtoi(pNotify->pSender->GetParent()->GetId());
	CaseListInfoUI* infoDlg = new CaseListInfoUI(caseid, _curdeviceId, (CButtonUI*)pNotify->pSender);
	infoDlg->Create(Singleton::GetMainWnd(),_T("CaseInfoList"));
	infoDlg->CenterWindow(Singleton::GetMainWnd());
	infoDlg->DoModal();
	return true;
	}
	bool DevicePanelCaselistUI::OnInstallCase(TNotifyUI* pNotify)
	{INTERFUN;
	InstallsCaseData* caseData = (InstallsCaseData* )pNotify->pSender->GetUserData();
	wstring devID = _curdeviceId;
	DeviceData *pDevData =	DEVICE_MANAGER->FindDevice(_curdeviceId);
	if(pDevData)
	{		
		if(caseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
		{
			// 统计功能
			CStaticsticsMessage StaticMsg(Statistic_ID_InstallNetPlan);
			StaticMsg.Send();

			NetInstallPlanTaskManager* mNetInstallPlanTaskManager = new NetInstallPlanTaskManager(devID, static_cast<NetInstallsCaseData*>(caseData));
			mNetInstallPlanTaskManager->btnInstall = (CButtonUI* )pNotify->pSender;
			mNetInstallPlanTaskManager->Run();
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_HasbeenAddToTaskList"),ICON_SUCCESS);
		}
		else
		{
			// 统计功能

			CStaticsticsMessage StaticMsg(Statistic_ID_InstallPlan);
			StaticMsg.Send();

			vector<wstring> pathList;
			for (InstallsAppList::iterator it = caseData->ListApp()->begin(); it != caseData->ListApp()->end(); it++)
			{
				InstallsAppInfo* appInfo = *it;
				pathList.push_back(appInfo->m_strSavePath);
			}

			int appnumber = pathList.size();
			if(appnumber ==0)
			{
				CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_NoAppInCase"),ICON_WARNING);
				return true;
			}

			InstallAppByCaseIdMsg InstallMsg;
			InstallMsg.sn = devID;
			InstallMsg.caseId = caseData->Id();
			InstallMsg.pathList = &pathList;
			InstallMsg.Send();
		}
	}
	else
	{
		if(caseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
		{
			NetInstallPlanTaskManager* mNetInstallPlanTaskManager = new NetInstallPlanTaskManager(devID, static_cast<NetInstallsCaseData*>(caseData));
			mNetInstallPlanTaskManager->btnInstall = (CButtonUI* )pNotify->pSender;
			mNetInstallPlanTaskManager->Run();
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_HasbeenAddToDownLoadTaskList"),ICON_SUCCESS);
		}
		else
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_makeSureDevicehaslinkPls"),CommonUIFunc::LoadString(L"RecentInstallCase_Text_OneKeyInstall"));
	}
	this->Resize();
	this->Invalidate();
	return true;
}



void DevicePanelCaselistUI::InsertCase(InstallsCaseData *CaseData)
{INTERFUN;
	int num=0; 
	InsertCase(CaseData,CaseData->CaseName(),CaseData->GetSumFileSize(),CaseData->GetAppNum(),UIStringFormat::InstallNeedTime(CaseData->GetAppNum(),CaseData->GetSumFileSize(),num),_CasePanel);
}

void DevicePanelCaselistUI::SetCurrentDeviceID( wstring id )
{INTERFUN;
	_curdeviceId=id;
}

DevicePanelCaselistUI::~DevicePanelCaselistUI()
{INTERFUN;

}


bool DevicePanelCaselistUI::OnDropFiles(TNotifyUI* pNotify)
{INTERFUN;
	return true;
}

void DevicePanelCaselistUI::AddCase(vector<wstring> * files)
{INTERFUN;

	//DragDropMessageReceiver::SendMessage(Singleton::GetMainWnd(), WM_DRAGDROP_HOMECASE, (WPARAM)files, NULL, this);

	AddCaseMsg addCaseMsg;
	addCaseMsg.pPathList = (void*)files;
	addCaseMsg.SendToUI();
}

void WINAPI DevicePanelCaselistUI::TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
	if (CASE_TIMER_ID == nTimerid)
	{
		DevicePanelCaselistUI::_bCanBtnClick = true;
		//检测是否创建了关闭信号量
		::KillTimer(hWnd,nTimerid);   
	}
}

// 分页处理
bool DevicePanelCaselistUI::OnPageChange(TNotifyUI* pNotify)
{INTERFUN;
return true;
	if (!DevicePanelCaselistUI::_bCanBtnClick)
	{
		return true;
	}
	if(_CasePanel->IsVisible())
	{
		CControlUI* oriPage = _CasePages->GetItem(mCurrPage-1);
		if (oriPage)
		{
			oriPage->SetAttribute(_T("icon.imgseq"),L"0,1");
		}
		mCurrPage = pNotify->pSender->GetUserData();
		CControlUI* nowPage = _CasePages->GetItem(mCurrPage-1);
		if (nowPage)
		{
			nowPage->SetAttribute(_T("icon.imgseq"),L"1,0");
		}
		_CasePanel->SelectPage(mCurrPage-1);
	}
	else
	{
		CControlUI* oriPage = _NetCasePages->GetItem(mNetCurrPage-1);
		if (oriPage)
		{
			oriPage->SetAttribute(_T("icon.imgseq"),L"0,1");
		}
		mNetCurrPage = pNotify->pSender->GetUserData();
		CControlUI* nowPage = _NetCasePages->GetItem(mNetCurrPage-1);
		if (nowPage)
		{
			nowPage->SetAttribute(_T("icon.imgseq"),L"1,0");
		}
		_NetCasePanel->SelectPage(mNetCurrPage-1);

	}
	this->Resize();
	this->Invalidate();
	DevicePanelCaselistUI::_bCanBtnClick = false;
	::SetTimer(Singleton::GetMainWnd(), CASE_TIMER_ID, 700, DevicePanelCaselistUI::TimerProc);
	return true;
}
bool DevicePanelCaselistUI::OnMoreCase(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能

	CStaticsticsMessage StaticMsg("10006");
	StaticMsg.Send();

	m_islocked=!m_islocked;
	pNotify->pSender->SetAttribute(L"bk.image",m_islocked?L"#RecentInstallCase_Image_case_lock_icon":L"#RecentInstallCase_Image_case_edit_icon");

	for (int i=0;i<_CasePanel->GetCount()-1;i++)
	{
		InstallCaseItem* item=(InstallCaseItem*)_CasePanel->GetItem(i)->GetItem(0);
		item->LockCase(m_islocked);
	}
	this->Resize();
	this->Invalidate();

	return true;
}

bool DevicePanelCaselistUI::OnEmptyCaseTipClose( TNotifyUI* pNotify )
{INTERFUN;
	wstring val = L"0";
	WritePrivateProfileString(_T("Default"),_T("ShowEmptyTip"),val.c_str(),CONFIG(_strBizDictionaryIni).c_str());

	// 发消息给主界面，通知所有设备的该提示隐藏
	SendMessage(Singleton::GetMainWnd(), WM_MY_SHOWEMPTYTIP, 0, 0);

	CloseShowEmptyTip();
	return true;
	}

	void DevicePanelCaselistUI::CloseShowEmptyTip()
	{INTERFUN;
	CLayoutUI* EmptyCaseTip = (CLayoutUI *)this->GetItem(_T("EmptyCaseTip"));
	EmptyCaseTip->SetVisible(false);
}

bool DevicePanelCaselistUI::Handle(const LoadFinshedMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->RefreshSolution();

	this->RefreshSolution(true);

	return true;
}

bool DevicePanelCaselistUI::Handle(const UIMiniSizeMsg* pMsg)
{
	_bUIMiniSize = true;

	return true;
}

bool DevicePanelCaselistUI::Handle(const InstallCaseMsg* pMsg)
{INTERFUN;
	if (NULL == pMsg)
	{
		return false;
	}

	switch (pMsg->message)
	{
	case WM_CASE_ADD:
		{
			InstallsCaseData* casedata=(InstallsCaseData*)pMsg->wParam;
			RETURN_TRUE_IF(casedata == NULL || casedata->PlanType() != _INSTALL_PLAN_TYPE_USER);

			RETURN_TRUE_IF(_CasePanel&&_CasePanel->GetItem(CStrOperation::IntToWString(casedata->Id()).c_str()));
			DeviceData *pDevData =DEVICE_MANAGER->FindDevice(_curdeviceId);
			if(pDevData&&casedata)
			{
				_INSTALLS_CASE_DATA_TYPE deviceType = pDevData->GetDevInfo()->_deviceType == Android ? _INSTALLS_CASE_DATA_ANDROID : _INSTALLS_CASE_DATA_IOS;
				RETURN_TRUE_IF(casedata->CaseType()!=deviceType);
				RETURN_TRUE_IF(casedata->CaseType() == _INSTALLS_CASE_DATA_IOS && !pDevData->GetDevInfo()->_ios_bJailBreaked && casedata->IsLegalPlan()!=1)
			}

			InsertCase(casedata);
			this->InsertPages();
			this->Resize();
			this->Invalidate();

		}
		break;
	case WM_CASE_REMOVE:
		{
			int caseId=pMsg->wParam;
			_CasePanel->Remove(_CasePanel->GetItem(CStrOperation::IntToWString(caseId).c_str()));
			this->InsertPages();
			this->Resize();
			this->Invalidate();
		}
		break;
	case WM_CASE_SET_NUMBER:
		{
			int caseId=pMsg->wParam;
			InstallsCaseData* casedata=(InstallsCaseData*)pMsg->lParam;
			DeviceData *pDevData =DEVICE_MANAGER->FindDevice(_curdeviceId);
			if( _CasePanel&&!_CasePanel->GetItem(CStrOperation::IntToWString(caseId).c_str())
				&&casedata->PlanType() == _INSTALL_PLAN_TYPE_USER
				&&casedata&&casedata->CaseType()==_INSTALLS_CASE_DATA_IOS&&casedata->IsLegalPlan()==1
				&&casedata->CaseType()==_INSTALLS_CASE_DATA_IOS
				&&pDevData&&pDevData->GetDevInfo()->_deviceType==Ios
				&&!pDevData->GetDevInfo()->_ios_bJailBreaked)
			{
				InsertCase(casedata);
				this->Resize();
				this->Invalidate();
				return true;
			}
			RETURN_TRUE_IF(_CasePanel&&!_CasePanel->GetItem(CStrOperation::IntToWString(caseId).c_str()))

				CContainerUI* conta=(CContainerUI*)_CasePanel->GetItem(CStrOperation::IntToWString(caseId).c_str());

			if(conta)
			{
				InstallCaseItem* caseitem = (InstallCaseItem*)(conta->GetItem(0));
				if(caseitem)
					caseitem->RefreshCase();
			}
		}
		break;

	default:
		break;
	}
	return true;
}

//bool DevicePanelCaselistUI::OnDragDropMessage( UINT message, WPARAM wParam, LPARAM lParam )
//{
//	if(message == WM_DRAGDROP_HOMECASE)
//	{
//		vector<wstring>* parFiles = (vector<wstring>*)wParam;
//		if(parFiles != NULL)
//		{
//			vector<wstring> files;
//			files.insert(files.end(), parFiles->begin(), parFiles->end());
//			SAFE_DELETE(parFiles);		
//
//			AddCaseMsg addCaseMsg;
//			addCaseMsg.pPathList = (void*)&files;
//			addCaseMsg.SendToUI();
//
//			return true;
//		}
//	}
//	return false;
//}

bool DevicePanelCaselistUI::OnCasePageChanged( TNotifyUI* pNotify )
{
	return true;

	if (_bUIMiniSize)
	{
		_bUIMiniSize = false;
		return true;
	}
	if(pNotify->wParam!=0)
		InsertPages();
	RETURN_TRUE_IF(_CasePages->GetCount()<1);
	int pageindex=pNotify->lParam;
	CControlUI* oriPage = _CasePages->GetItem(mCurrPage-1);
	if (oriPage)
	{
		oriPage->SetAttribute(_T("icon.imgseq"),L"0,1");
	}

	mCurrPage = pageindex;
	CControlUI* nowPage = _CasePages->GetItem(mCurrPage-1);
	if(nowPage)
	{
		nowPage->SetAttribute(_T("icon.imgseq"),L"1,0");
	}
	this->Resize();
	this->Invalidate();
	return true;
}

bool DevicePanelCaselistUI::OnNetCasePageChanged( TNotifyUI* pNotify )
{
	return true;

	if(pNotify->wParam!=0)
		InsertPages(true);
	RETURN_TRUE_IF(_NetCasePages->GetCount()<1);
	int pageindex=pNotify->lParam;
	CControlUI* oriPage = _NetCasePages->GetItem(mNetCurrPage-1);
	oriPage->SetAttribute(_T("icon.imgseq"),L"0,1");

	mNetCurrPage = pageindex;
	CControlUI* nowPage = _NetCasePages->GetItem(mNetCurrPage-1);
	nowPage->SetAttribute(_T("icon.imgseq"),L"1,0");

	this->Resize();
	this->Invalidate();
	return true;
}

void DevicePanelCaselistUI::Release()
{
	if(_CasePanel)
		_CasePanel->RemoveAll();
	if(_NetCasePanel)
		_NetCasePanel->RemoveAll();
}
