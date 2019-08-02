#include "StdAfx.h"
#include "WebAppListItemControl.h"
#include "WebAppListItemFloatLayer.h"
#include "logic/WebSoftInfo.h"


WebAppListItemControl::WebAppListItemControl(HWND hWnd,const WebSoftData* pSoftData)
{
	m_iStyle = 0;
	m_pLayout = NULL;
	m_pIcon = NULL;
	m_pConScore = NULL;
	m_pLbName = NULL;
	m_pLbDownNum = NULL;
	m_pBtnHover = NULL;

	m_pSoftData = pSoftData;
}

WebAppListItemControl::~WebAppListItemControl(void)
{
	if(m_pBtnHover)
		m_pBtnHover->RemoveEventDelegate();
}


void WebAppListItemControl::SetStyle( int style )
{
	m_iStyle = style;
}

//初始化各控件数据
void WebAppListItemControl::InitControls()
{
	RETURN_IF(!m_pSoftData);

	if( 0 == m_iStyle )
	{
		if( m_pIcon )
		{
			if(CFileOperation::IsFileExist(m_pSoftData->_localpath.c_str()))
			{
				m_pIcon->SetAttribute(_T("bk.image"), m_pSoftData->_localpath.c_str());
			}
			else
			{
				m_pIcon->SetAttribute(_T("bk.image"), _T("#WebResourceUI_Appicon_Default_64"));
			}
		}
		if(m_pLbName)
		{
			m_pLbName->SetText(m_pSoftData->_name.c_str());
			m_pLbName->SetToolTip(m_pSoftData->_name.c_str());
		}
		if(m_pConScore)
		{
			for (int i=0;i<5;i++)
			{
				m_pConScore->GetItem(i)->SetAttribute(L"bk.image",i<m_pSoftData->_ratingScore?L"#WebResourceUI_WebSoft_star1":L"#WebResourceUI_WebSoft_star3");
			}
		}
		if(m_pLbDownNum)
		{
			wstring text = CStrOperation::IntToWString(m_pSoftData->_downNum) + L"人下载";
			m_pLbDownNum->SetText(text.c_str());
		}
	}
	else if( 1 == m_iStyle )
	{
		if( m_pIcon )
		{
			if(CFileOperation::IsFileExist(m_pSoftData->_imgSrc.c_str()))
			{
				m_pIcon->SetAttribute(_T("bk.image"), m_pSoftData->_imgSrc.c_str());
			}
			else
			{
				m_pIcon->SetAttribute(_T("bk.image"), _T("#WebResourceUI_Appicon_Default_64"));
			}
		}
		if(m_pLbName)
		{
			m_pLbName->SetText(m_pSoftData->_name.c_str());
		}
	}
	m_pLayout->Resize();
	m_pLayout->Invalidate();
}

//设置图片
void WebAppListItemControl::SetIcon( wstring path )
{
	RETURN_IF(!m_pSoftData);
	if(CFileOperation::IsFileExist(m_pSoftData->_localpath.c_str()))
	{
		m_pIcon->SetAttribute(_T("bk.image"), m_pSoftData->_localpath.c_str());
		RECT rect = m_pIcon->GetRect();
	}
	else
	{
		m_pIcon->SetAttribute(_T("bk.image"), _T("#WebResourceUI_Appicon_Default_64"));
	}
	m_pIcon->Resize();
	m_pIcon->Invalidate();
}

//鼠标HOVER事件
bool WebAppListItemControl::OnEventHover( TEventUI& event )
{
	//CButtonUI* pControl = (CButtonUI*)event.pSender;
	switch(event.Type)
	{
		case UIEVENT_MOUSEENTER:
			WebAppListItemFloatLayer::GetInstance()->Show(Singleton::GetMainWnd(), m_pSoftData,m_pLayout->GetRect());
			//修改为发送消息
			break;
	}
	return true;
}

void WebAppListItemControl::OnCreate()
{
	m_pLayout = dynamic_cast<CLayoutUI*>(GetItem(_T("WebAppListItem_Layout")));

	m_pBtnHover = dynamic_cast<CButtonUI*>(GetItem(_T("WebAppListItem_Layout.BtnHover")));
	if(m_pBtnHover)
		m_pBtnHover->AddEventDelegate(&MakeDelegate(this, &WebAppListItemControl::OnEventHover));

	m_pIcon = dynamic_cast<CControlUI*>(GetItem(_T("WebAppListItem_Layout.ConIcon")));

	m_pConScore = dynamic_cast<CLayoutUI*>(GetItem(_T("WebAppListItem_Layout.rightlayout.ConScore")));

	m_pLbName = dynamic_cast<CLabelUI*>(GetItem(_T("WebAppListItem_Layout.rightlayout.LbName")));

	m_pLbDownNum = dynamic_cast<CLabelUI*>(GetItem(_T("WebAppListItem_Layout.rightlayout.LbDownNum")));

	InitControls();
}

bool WebAppListItemControl::OnBtnClick( TNotifyUI* pNotify )
{
// 	BaseParam pParam;
// 	pParam.SetInt(m_pSoftData->_id);
// 	pParam.SetBool(m_pSoftData->_appType==HOT24APP);
// 	//转到详情页面
// 	SendMessage(MainDlg::GetInstance()->GetHWND(), WM_WebLegalSoft_GotoDes, (int)m_pSoftData->_bIpad, (LPARAM)&pParam);


	WebSoftMsg msg;
	msg.message = WM_WebLegalSoft_GotoDes;
	msg.platType = (int)m_pSoftData->_bIpad;
	msg.id = m_pSoftData->_id;
	msg.apptype = HOT24APP;
	msg.Send();

	return true;
}


std::wstring WebAppListItemControl::GetStyleID()
{
	return _T("WebAppListItemControl0");
}

void WebAppListItemControl::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}
