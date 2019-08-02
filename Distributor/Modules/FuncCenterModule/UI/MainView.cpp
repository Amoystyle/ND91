#include "stdafx.h"
#include "MainView.h"
#include "PcSuitFuncUIItem.h"

#define PCSUIT_LAYOUT_ID L"Layout"
#define FUNC_TIMER_ID 100

bool MainView::_bCanBtnClick = true;
MainView::MainView(wstring deviceID, void* phone)
{
	_deviceID = deviceID;
	_pPhone = phone;
	_CurPlatform = ePhoneOS_None;
	_bInitPcSuitFunction = false;
	_nPageIndex = 0;
	_nPageSum = 1;
	_pFunc_panel = NULL;
	_bCanBtnClick = true;
	_Reparelay = NULL;
}

MainView::~MainView()
{
	for (vector<PcSuitFuncUIItem*>::iterator iter = _vec_FuncUIItem.begin();
		iter != _vec_FuncUIItem.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	_vec_FuncUIItem.clear();
}

std::wstring MainView::GetStyleID()
{
	return L"FuncCenter_View_Main";
}

void MainView::OnCreate()
{
	_pFunc_panel = (CAutoGridLayoutUI *)this->GetItem(_T("Func_panel"));
	if (NULL != _pFunc_panel)
	{
		assert(_pFunc_panel);
		_pFunc_panel->SetAttribute(L"GridWidth",L"80");
		_pFunc_panel->SetAttribute(L"GridHeight",L"92");
		_pFunc_panel->SetAttribute(L"EnablePage",L"1");
		_pFunc_panel->SetAttribute(L"ShowRadio",L"0");
		_pFunc_panel->SetINotifyUI(this);

		this->CreateFuncAutoGridItem();

		CControlUI* pControlUI = GetItem(L"Left_Func");
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
			pControlUI->Invalidate();
			pControlUI->Resize();
		}
		pControlUI = GetItem(L"Right_Func");
		if (pControlUI)
		{
			pControlUI->SetEnabled(false);
			pControlUI->Invalidate();
			pControlUI->Resize();
		}

		RequestDeviceTypeMsg msg;
		msg.deviceID = this->_deviceID;
		msg.SendAndGetReturn();
		_CurPlatform = msg.deviceType;

		if(_CurPlatform == ePhoneOS_Android)
		{
			this->InitAndroidButton();
		}
		else
		{
			this->InitIOSButton();
		}	

		//初始化功能大全
		GetPcSuitFunctionMsg GetMsg;
		GetMsg.wsPlatform = ePhoneOS_Ios == _CurPlatform ? L"iphone" : L"android";
		GetMsg.SendAndGetReturn();
		if (!GetMsg.vec_Function.empty())
		{
			_vecPcSuitFunction = GetMsg.vec_Function;
			this->InitPcSuitFunc();	
		}

		if (_CurPlatform == ePhoneOS_Ios)
		{
			RequestNeedInstallAppSyncMsg appSyncMsg;
			appSyncMsg.deviceID = this->_deviceID;
			appSyncMsg.SendAndGetReturn();
		}
	}

}

void MainView::Show()
{
	__super::Show();
}

void MainView::CreateFuncAutoGridItem()
{

	this->CreateBtnFuncItem(L"Layout_InstallApp", L"Button_InstallApp", CommonUIFunc::LoadString(L"FuncCenter_Text_InstallApp"), L"#FuncCenter_Image_InstallApp");
	this->CreateBtnFuncItem(L"Layout_Reboot", L"Button_Reboot", CommonUIFunc::LoadString(L"FuncCenter_Text_Reboot"), L"#FuncCenter_Image_Reboot");
	this->CreateBtnFuncItem(L"Layout_Recovery", L"Button_Recovery", CommonUIFunc::LoadString(L"FuncCenter_Text_Recovery"), L"#FuncCenter_Image_Recovery");

	this->CreateContainerFuncItem(L"ClearMemory", L"Button_ClearMemory", L"ClearMemory_Loading", CommonUIFunc::LoadString(L"FuncCenter_Text_ClearMemory"), \
		L"#FuncCenter_Image_ClearMemory", L"#FuncCenter_Image_Loading");
	this->CreateContainerFuncItem(L"Repair", L"Button_Repair", L"Repair_Loading", CommonUIFunc::LoadString(L"FuncCenter_Text_Repair"), \
		L"#FuncCenter_Image_Repair", L"#FuncCenter_Image_Loading");
	

}

void MainView::CreateBtnFuncItem(wstring wsLayoutID, wstring wsID, wstring wsText, wstring wsImg)
{
	CLayoutUI* lay=new CLayoutUI();
	lay->SetId(wsLayoutID.c_str());
	CButtonUI* _pBtnFuncItem = new CButtonUI;
	_pBtnFuncItem->SetStyle(_T("FuncCenter_Style_FuncButton"));
	_pBtnFuncItem->SetId(wsID.c_str());
	_pBtnFuncItem->SetInitPos(0,-1,80,92, 9);
	_pBtnFuncItem->SetAttribute(_T("cursor"),L"hand");
	_pBtnFuncItem->SetAttribute(_T("text"),wsText.c_str());
	_pBtnFuncItem->SetAttribute(_T("icon.image"), wsImg.c_str());

	_pBtnFuncItem->SetINotifyUI(this);
	lay->Add(_pBtnFuncItem);

	_pFunc_panel->Add(lay);
}

void MainView::CreateContainerFuncItem(wstring wsLayoutID, wstring wsBtnID, wstring wsAnimationID, wstring wsText, wstring wsImg, wstring wsAniImg, bool bAppPos, CControlUI* pControlUI)
{
	CLayoutUI* lay=new CLayoutUI();
	lay->SetId(wsLayoutID.c_str());
	lay->SetInitPos(0,-1,80,92, 9);

	if (L"Repair" == wsLayoutID)
	{
		_Reparelay = lay;
	}

	CButtonUI* _pBtnFuncItem = new CButtonUI;
	_pBtnFuncItem->SetStyle(_T("FuncCenter_Style_FuncButton"));
	_pBtnFuncItem->SetId(wsBtnID.c_str());
	_pBtnFuncItem->SetInitPos(0,-1,80,92, 9);
	_pBtnFuncItem->SetAttribute(_T("cursor"),L"hand");
	_pBtnFuncItem->SetAttribute(_T("text"),wsText.c_str());
	_pBtnFuncItem->SetINotifyUI(this);
	lay->Add(_pBtnFuncItem);

	CAnimationUI* _pAnimation = new CAnimationUI;
	_pAnimation->SetId(wsAnimationID.c_str());
	_pAnimation->SetInitPos(-1,0,66,66, 9);
	_pAnimation->SetAttribute(_T("image"), wsAniImg.c_str());
	_pAnimation->SetAttribute(_T("Gif"), L"1");
	_pAnimation->SetAttribute(_T("FrameDelay"), L"200");
	_pAnimation->SetVisible(false);
	_pAnimation->SetINotifyUI(this);
	lay->Add(_pAnimation);

	CControlUI* pIcon = new CControlUI();
	pIcon->SetInitPos(-1,0,66,66, 9);
	pIcon->SetStyle(L"Style_ImageFill");
	pIcon->SetAttribute(_T("fill.image"), wsImg.c_str());
	pIcon->SetAttribute(_T("fill.paintmode"), L"center");
	lay->Add(pIcon);

	if (bAppPos)
	{
		if (_Reparelay)
		{
			_pFunc_panel->Insert(lay, _Reparelay);
		}
	}
	else
		_pFunc_panel->Add(lay);
}
bool MainView::OnInstallAppClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CStaticsticsMessage statMsg("");
	if(_CurPlatform == ePhoneOS_Ios)
		statMsg._id = "10008";
	else
		statMsg._id = "10009";
	statMsg.Send();

	NotifyInstallAppMsg installMsg;
	installMsg.deviceID = this->_deviceID;
	installMsg.nType = NotifyInstallAppType_SelectFiles;
	installMsg.wsText = CommonUIFunc::LoadString(L"FuncCenter_Text_InstallApp_Text");
	installMsg.PostToUI();

	return true;
}

bool MainView::OnButtonRebootClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CStaticsticsMessage statMsg("");
	if(_CurPlatform == ePhoneOS_Ios)
		statMsg._id = "10010";
	else
		statMsg._id = "10011";
	statMsg.Send();

	if(CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(L"FuncCenter_Text_RebootConfirm"), CommonUIFunc::LoadString(L"FuncCenter_Text_MsgTitle")))
	{
		RequestRebootDeviceMsg msg;
		msg.deviceID = _deviceID;
		msg.Send();
	}

	return true;
}

bool MainView::OnButtonRecoveryClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CStaticsticsMessage statMsg("10012");
	statMsg.Send();

	if(CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(L"FuncCenter_Text_RecoveryConfirm"), CommonUIFunc::LoadString(L"FuncCenter_Text_MsgTitle")))
	{
		RequestRecoveryDeviceMsg msg;
		msg.deviceID = _deviceID;
		msg.Send();
	}

	return true;
}

bool MainView::OnButtonClearMemoryClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CControlUI* pBtn = GetItem(L"Func_panel.ClearMemory.ClearMemory_Loading");
	
	if(pBtn != NULL)
	{
		pBtn->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}

	CStaticsticsMessage statMsg("");
	if(_CurPlatform == ePhoneOS_Ios)
		statMsg._id = "10014";
	else
		statMsg._id = "10015";
	statMsg.Send();

	RequestClearDeviceMemoryMsg clearMsg;
	clearMsg.deviceID = _deviceID;
	clearMsg.Send();

	return true;
}

bool MainView::OnButtonRepairClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CControlUI* pBtn = GetItem(L"Func_panel.Repair.Repair_Loading");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}

	RequestRepairDeviceMsg msg;
	msg.deviceID = _deviceID;
	msg.Send();

	CStaticsticsMessage statMsg("10016");
	statMsg.Send();

	return true;
}

bool MainView::OnButtonAppSyncClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CControlUI* pBtn = GetItem(L"Func_panel.AppSync.AppSync_Loading");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}

	RequestInstallAppSyncMsg msg;
	msg.deviceID = _deviceID;
	msg.Send();

	return true;
}

void WINAPI MainView::TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
if (FUNC_TIMER_ID == nTimerid)
{
	MainView::_bCanBtnClick = true;
	//检测是否创建了关闭信号量
	::KillTimer(hWnd,nTimerid);  
}
  
}

bool MainView::OnLeftFunc(TNotifyUI* pNotify)
{
	if (0 == _nPageIndex || !MainView::_bCanBtnClick)
	{
		return true;
	}
	_nPageIndex--;

	_pFunc_panel->SelectPage(_nPageIndex);

	this->FuncPageChange();

	CControlUI* pControlUI = GetItem(L"Left_Func");
	if (pControlUI)
	{
		if (0 == _nPageIndex)
		{
			pControlUI->SetEnabled(false);
			pControlUI->Invalidate();
			pControlUI->Resize();
		}
	}
	pControlUI = GetItem(L"right_Func");
	if (pControlUI && _nPageIndex<_nPageSum)
	{
		pControlUI->SetEnabled(true);
		pControlUI->Invalidate();
		pControlUI->Resize();
	}

	_pFunc_panel->Invalidate();
	_pFunc_panel->Resize();

	MainView::_bCanBtnClick = false;
	::SetTimer(Singleton::GetMainWnd(), FUNC_TIMER_ID, 700, MainView::TimerProc);
	return true;
}

bool MainView::OnRightFunc(TNotifyUI* pNotify)
{
	if (_nPageIndex+1 >= _nPageSum || !MainView::_bCanBtnClick)
	{
		return true;
	}
	_nPageIndex++;

	_pFunc_panel->SelectPage(_nPageIndex);

	this->FuncPageChange();

	CControlUI* pControlUI = GetItem(L"right_Func");
	if (pControlUI)
	{
		if (_nPageSum == _nPageIndex+1)
		{
			pControlUI->SetEnabled(false);
			pControlUI->Invalidate();
			pControlUI->Resize();
		}
	}
	pControlUI = GetItem(L"Left_Func");
	if (pControlUI && _nPageIndex>0)
	{
		pControlUI->SetEnabled(true);
		pControlUI->Invalidate();
		pControlUI->Resize();
	}

	_pFunc_panel->Invalidate();
	_pFunc_panel->Resize();

	MainView::_bCanBtnClick = false;
	::SetTimer(Singleton::GetMainWnd(), FUNC_TIMER_ID, 700, MainView::TimerProc);
	return true;
}

bool MainView::OnFuncPageChanged( TNotifyUI* pNotify )
{
	int pageindex=pNotify->lParam;
	if (pageindex == _nPageIndex + 1)
	{
		return true;
	}

	_nPageIndex = pageindex - 1;

	CControlUI* pControlUI = GetItem(L"Left_Func");
	if (pControlUI)
	{
		if (pageindex == 1)
		{
			pControlUI->SetEnabled(false);
		}
		else
		{
			pControlUI->SetEnabled(true);
		}
		pControlUI->Invalidate();
		pControlUI->Resize();
	}
	pControlUI = GetItem(L"right_Func");
	if (pControlUI )
	{
		if(pageindex<_nPageSum) 
		{
			pControlUI->SetEnabled(true);
		}
		else
		{
			pControlUI->SetEnabled(false);
		}
		pControlUI->Invalidate();
		pControlUI->Resize();
	}

	return true;
}

void MainView::FuncPageChange()
{

}

void MainView::InitAndroidButton()
{
	CControlUI* pLayout = GetItem(L"Func_panel.Layout_Recovery");
	if(pLayout != NULL)
		_pFunc_panel->Remove(pLayout);

	pLayout = GetItem(L"Func_panel.AppSync");
	if(pLayout != NULL)
		_pFunc_panel->Remove(pLayout);

	pLayout = GetItem(L"Func_panel.Repair");
	if(pLayout != NULL)
		_pFunc_panel->Remove(pLayout);

	_pFunc_panel->Resize();
	_pFunc_panel->Invalidate();
}

void MainView::InitIOSButton()
{
}

bool MainView::Handle( const ResponseClearDeviceMemoryMsg* pMsg )
{
	CControlUI* pBtn = GetItem(L"Func_panel.ClearMemory.ClearMemory_Loading");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(false);
		this->Resize();
		this->Invalidate();
	}

	wstring resultText = L"";
	if(pMsg->nSize > 0)
	{
		resultText = CommonUIFunc::LoadString(L"FuncCenter_Text_ClearMemory_Result");
		resultText += CStrOperation::FormatFileSize(pMsg->nSize);
	}
	else
	{
		resultText = CommonUIFunc::LoadString(L"FuncCenter_Text_ClearMemory_Success");
	}
	CommonUIFunc::NoticeDialog(resultText, ICON_SUCCESS, 1000);

	return true;
}

bool MainView::Handle( const ResponseRepairDeviceMsg* pMsg )
{
	CControlUI* pBtn = GetItem(L"Func_panel.Repair.Repair_Loading");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(false);
		this->Resize();
		this->Invalidate();
	}

	if(pMsg->bResult)
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"CommonUi_icon_TipSuccess") , ICON_SUCCESS, 1000);
	else
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"CommonUi_icon_TipError") , ICON_WARNING, 1000);
	return true;
}

bool MainView::Handle( const ResponseNeedInstallAppSyncMsg* pMsg )
{
	if(pMsg->deviceID != this->_deviceID)
		return true;
	if (!pMsg->bNeedInstall)
		return true;


	this->CreateContainerFuncItem(L"AppSync", L"Button_AppSync", L"AppSync_Loading", CommonUIFunc::LoadString(L"FuncCenter_Text_InstallAppSync"), \
		L"#FuncCenter_Image_Appsync", L"#FuncCenter_Image_Loading", true, _Reparelay);

	CControlUI* pBtn = GetItem(L"Func_panel.AppSync");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(true);
		this->Resize();
		this->Invalidate();
	}

	_nPageSum = (6 + _vecPcSuitFunction.size())/8 + 1;
	CControlUI* pControlUI = GetItem(L"Right_Func");
	if (pControlUI && _nPageSum>1)
	{
		pControlUI->SetEnabled(true);
		pControlUI->Invalidate();
		pControlUI->Resize();
	}

	return true;
}

bool MainView::Handle( const RequestInstallAppsyncMsg* pMsg )
{
	
	CControlUI* pBtn = GetItem(L"Func_panel.AppSync.AppSync_Loading");
	if(pBtn != NULL)
	{
		pBtn->SetVisible(false);
		this->Resize();
		this->Invalidate();
	}

	if (pMsg->installReturn)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"FuncCenter_Text_InstallAppsyncSuccess") , ICON_SUCCESS, 1000);
		CControlUI* pLayout = GetItem(L"Func_panel.AppSync");
		if(pLayout != NULL)
			_pFunc_panel->Remove(pLayout);
	}
	else
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"FuncCenter_Text_InstallAppsyncfail") , ICON_WARNING, 1000);
	return true;
}

void MainView::InitPcSuitFunc()
{
	if (_bInitPcSuitFunction)
	{
		return;
	}

	UINT i=0;
	for (vector<FunctionItem>::iterator iter = _vecPcSuitFunction.begin();\
		iter != _vecPcSuitFunction.end(); ++iter)
	{
		CLayoutUI* pLayout = new CLayoutUI;
		pLayout->SetId(PCSUIT_LAYOUT_ID);
		_pFunc_panel->Add(pLayout);
		PcSuitFuncUIItem* pPcItem = new PcSuitFuncUIItem(pLayout, *iter, _CurPlatform);
		_vec_FuncUIItem.push_back(pPcItem);
	}

	_bInitPcSuitFunction = true;

	//判断右边箭头是否可用
	if(_CurPlatform == ePhoneOS_Android)
	{//一行7个，安卓本身自带有3个，
		_nPageSum = (3 + _vecPcSuitFunction.size())/8 + 1;
	}
	else 
	{//一行7个，IOS本身自带有6个，安装appsyn功能有的话则7个
		_nPageSum = (5 + _vecPcSuitFunction.size())/8 + 1;
	}
	CControlUI* pControlUI = GetItem(L"Right_Func");
	if (pControlUI && _nPageSum>1)
	{
		pControlUI->SetEnabled(true);
		pControlUI->Invalidate();
		pControlUI->Resize();
	}

	return;
}

bool MainView::Handle( const ReponsePcSuitFunctionMsg* pMsg )
{
	if (NULL == pMsg 
		|| (ePhoneOS_Ios==_CurPlatform && L"iphone"!= pMsg->wsPlatform)
		|| (ePhoneOS_Android==_CurPlatform && L"android"!= pMsg->wsPlatform))
	{
		return false;
	}

	Sleep(50);//避免InitPcSuitFunc未执行完切到本线程，导致 功能大全重复加载

	if (_bInitPcSuitFunction)
	{
		return true;
	}
	_vecPcSuitFunction = pMsg->vec_Function;
	this->InitPcSuitFunc();

	return true;
}
