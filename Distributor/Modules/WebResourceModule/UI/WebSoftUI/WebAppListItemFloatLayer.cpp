#include "StdAfx.h"
#include "WebAppListItemFloatLayer.h"
#include "Logic/WebSoftInfo.h"
#include "Logic/WebResUrlAnalysis.h"
#include "Logic/DownProtocolAnalysis.h"
WebAppListItemFloatLayer* WebAppListItemFloatLayer::_instance=NULL;

WebAppListItemFloatLayer::WebAppListItemFloatLayer()
{
	m_nRoundRgn = 0;
	m_bIsHide = true;
	m_pSoftData = NULL;
	_box = NULL;
	_instance = NULL;
}

WebAppListItemFloatLayer::~WebAppListItemFloatLayer(void)
{
}

void CALLBACK HideTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	if(!WebAppListItemFloatLayer::GetInstance()->m_bIsHide)
	{
		POINT pt;
		::GetCursorPos(&pt);
		RECT rect;
		::GetWindowRect(hwnd, &rect);
		if(pt.x>rect.left && pt.x<rect.right && pt.y>rect.top && pt.y<rect.bottom )
		{
			return;
		}
		WebAppListItemFloatLayer::GetInstance()->Hide();
	}
	::KillTimer(hwnd,idEvent);
	WebAppListItemFloatLayer::GetInstance()->m_bIsHide = true;
}

LRESULT WebAppListItemFloatLayer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_MOUSELEAVE==message)
	{
		POINT pt;
		::GetCursorPos(&pt);
		ScreenToClient(m_hparHWND, &pt);
		if(pt.x<m_rect.left + 5 || pt.x>m_rect.right - 5 || pt.y<m_rect.top + 5 || pt.y>m_rect.bottom - 5)
		{
			ShowWindow(SW_HIDE);
			m_bIsHide = true;
		}
	}
	else if(WM_LBUTTONUP == message)
	{
		POINT pt;
		::GetCursorPos(&pt);
		ScreenToClient(this->GetHWND(), &pt);
		CControlUI* pCtrl = FindControl(pt);
		CButtonUI* pBtn = (CButtonUI*)GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.left_layout.bottomlayout.BtnDownLoad");
		if(pCtrl != pBtn)
		{
			OnMainBtnClick(NULL);
		}
	}
	if(WM_MOUSEMOVE == message)
		::SetCursor(LoadCursor(NULL, IDC_HAND));
	return CWindowUI::WindowProc(message,wParam,lParam);
}

void WebAppListItemFloatLayer::Show(HWND hParent,const WebSoftData* pSoftBase,RECT rect)
{
	m_rect = rect;
	LONG posleft = rect.left;
	LONG  postop = rect.top;
	m_hparHWND = hParent;
	m_pSoftData = pSoftBase;
	if(!m_hWnd)
		Create(hParent,_T("WebAppListItemFloatLayer"));
	RETURN_IF(!m_hWnd);

	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	//SetTitle(L"");
	SetLayerWindow(true);
	//CenterWindow(MainDlg::GetInstance()->GetOwnerHWnd());
	initcontrols(m_pSoftData);

	RECT parentR;
	::GetWindowRect(hParent, &parentR);
	int width = 235;
	int height = 135;
	MoveWindow(m_hWnd,parentR.left+posleft-5,parentR.top+postop,width,height,true);
	::KillTimer(m_hWnd,0);
	::SetTimer(m_hWnd,0,200,&HideTimerProc);
	ShowWindow(SW_SHOW);
	m_bIsHide = false;
}

void WebAppListItemFloatLayer::Hide()
{
	ShowWindow(SW_HIDE);
	m_bIsHide = true;
}


void WebAppListItemFloatLayer::initcontrols( const WebSoftData* pSoftBase )
{
	RETURN_IF(!pSoftBase);
	CLabelUI* category=(CLabelUI*)GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.left_layout.top_layout.category");
	RETURN_IF(!category);
	CLabelUI* appname=(CLabelUI*)GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.left_layout.top_layout.appname");
	wstring cateste=L"["+pSoftBase->_categoryName;
	cateste+=L"]";
	if(category)
		category->SetText(cateste.c_str());
	if(appname)
	{
		appname->SetText(pSoftBase->_name.c_str());
		appname->SetToolTip(pSoftBase->_name.c_str());
	}

	CLayoutUI* starslayout=(CLayoutUI*)GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.left_layout.starslayout");
	for(int i=0;i<5;i++)
	{
		if(starslayout)
			starslayout->GetItem(i)->SetAttribute(L"bk.image",i<pSoftBase->_ratingScore?L"#WebResourceUI_WebSoft_star1":L"#WebResourceUI_WebSoft_star3");
	}
	starslayout->GetItem(L"appsize")->SetText(CStrOperation::FormatFileSize((int64_t)pSoftBase->_size).c_str());
	if(L"" == pSoftBase->_summary)
	{
		GetItem(L"AppItemFloat_Layout.disccontent")->SetText(L"<无>");
		GetItem(L"AppItemFloat_Layout.disccontent")->SetToolTip(L"<无>");
	}
	else
	{
		GetItem(L"AppItemFloat_Layout.disccontent")->SetText(pSoftBase->_summary.c_str());
		GetItem(L"AppItemFloat_Layout.disccontent")->SetToolTip(pSoftBase->_summary.c_str());
	}

	wstring  savemoney=L"节省"+CStrOperation::IntToWString(pSoftBase->_Price);
	savemoney+=L"元";
	GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.left_layout.bottomlayout.savemoney")->SetText((pSoftBase->_Price>0)?savemoney.c_str():L"");

	GetItem(L"AppItemFloat_Layout.simpleinfo_Layout.rightIcon")->SetAttribute(L"bk.image",pSoftBase->_localpath.c_str());
	GetItem(L"AppItemFloat_Layout")->Resize();
	GetItem(L"AppItemFloat_Layout")->Invalidate();

}

void WebAppListItemFloatLayer::OnCreate()
{
	initcontrols(m_pSoftData);
	//ShowWindow(SW_SHOW);
}

WebAppListItemFloatLayer* WebAppListItemFloatLayer::GetInstance()
{
	if(!_instance)
	{
		_instance =new WebAppListItemFloatLayer();
	}
	return _instance;
}

bool WebAppListItemFloatLayer::OnDownLoadBtnClick( TNotifyUI* pNotify )
{
	//CCodeOperation::GB2312ToUnicode(val);
	RETURN_TRUE_IF(!m_pSoftData);
	if(m_pSoftData->_appType==HOT24APP)
	{
		Statistic_StatFunc(Statistic_ID_DownNumOf24Hot);
	}
	Statistic_StatFunc(Statistic_ID_DownLoadTotal);
	wstring strUrl = CCodeOperation::UTF_8ToUnicode(m_pSoftData->_downUrl);

	
	//开始下载操作

// 	CTaskItem* finitem=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.findSameItem(downurl.c_str(),path.c_str(),TASK_KIND::URL_DOWNLAOD);
// 	_box=new MessageBoxUI();
// 	if(!finitem)
// 	{
// 		TaskCenterModel::GetInstance()->StartDownLoad(downurl,path,name,version,m_pSoftData->_bIpad?L"iPad":L"iPhone",m_pSoftData->_id);
// 		if(CONFIG(_bDownLoadMsg))
// 			_box->Show(MainDlg::GetInstance()->GetHWND(),ICON_SUCCESS,BaseUI::LoadString(L"Task_Added_to_list"),100);
// 	}
// 	else
// 	{
// 		_box->Show(MainDlg::GetInstance()->GetHWND(),ICON_WARNING,BaseUI::LoadString(L"Task_Exist_in_list"),200);  
// 	}

	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_ClickGotoDes;
	msg.Send();

	AnalysisData *data = new AnalysisData;
	DownProtocolAnalysis::WebSoftAnalysisUrl(*data,strUrl);

	if (data->url.length() != 0)
	{
		data->islegal = true;
		WebResourceInnserMsg msg;
		msg.type = WebResourceMsg_DownloadResource;
		msg.wParam = (WPARAM)data;
		msg.Send();
		LOG->WriteDebug(_T("正版体验查找到Download页面,发送下载消息"));
		LOG->WriteDebug(strUrl);
	}


	ShowWindow(SW_HIDE);
	m_bIsHide = true;
	return true;
}

bool WebAppListItemFloatLayer::OnMainBtnClick( TNotifyUI* pNotify )
{
	RETURN_TRUE_IF(!m_pSoftData);
	//转到详情页面
	Send(WebAppItemGotoDescription_Click, GetItem(L"AppItemFloat_Layout"), m_pSoftData->_id, m_pSoftData->_appType);
	ShowWindow(SW_HIDE);
	m_bIsHide = true;
	return true;
}
