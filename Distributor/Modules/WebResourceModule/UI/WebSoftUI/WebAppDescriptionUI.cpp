#include "StdAfx.h"
#include "WebAppDescriptionUI.h"
#include "Logic/WebResUrlAnalysis.h"
#include <wininet.h>
#include "Logic/DownProtocolAnalysis.h"

#pragma comment(lib,"Wininet.lib")

WebAppDescriptionUI::WebAppDescriptionUI(HWND hWnd)
{
	_pIE = NULL;
	_box = NULL;
	_pLoading = NULL;
	m_sURL = L"http://itunesapp2.sj.91.com/softdetailforpcfirm.aspx?platform=iPhone&fw={0}&id={1}&ver=3.2.5";
	_IsHot24 = false;
	_bDownLoad = false;
}

WebAppDescriptionUI::~WebAppDescriptionUI(void)
{
}

void WebAppDescriptionUI::OnCreate()
{
	_pLoading = (CAnimationUI*)GetItem(L"Container.Loading");
	_pIE = new CWebIEUI();
	_pIE->_pOb=this;
	_pIE->SetAttribute(_T("id"),_T("webie"));
	_pIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
	_pIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	_pIE->SetAttribute(_T("enableScroll"),_T("1"));
	_pIE->SetAttribute(_T("enableTextSelect"),_T("1"));
	_pIE->SetAttribute(_T("enableMenu"),_T("0"));
	_pIE->SetAttribute(_T("enableNewWindow"),_T("1"));
	_pIE->SetAttribute(_T("enabledownload"),_T("0"));
	_pIE->SetAttribute(_T("enableloading"),_T("1"));
	_pIE->SetAttribute(_T("enableexternal"),_T("1"));
	this->Add(_pIE);
	_pIE->SetVisible(false);
}

void WebAppDescriptionUI::ShowAppItemDetail( int appID, PlatFrom platForm, bool Ishot24)
{
	_mPlatForm = platForm;
	_IsHot24= Ishot24;
	wstring urlTemp = CStrOperation::ReplaceW(m_sURL, L"{1}", CStrOperation::IntToWString(appID));
	wstring url = CStrOperation::ReplaceW(urlTemp, L"{0}", IPHONE==platForm? L"iPhone" : L"iPad");
	if(!_pIE)
		_pIE = new CWebIEUI();
	_pIE->SetVisible(true);
	_pIE->Resize();
	_pIE->Show((BSTR)url.c_str());
	_pIE->SetVisible(false);
}

void WebAppDescriptionUI::LoadCompleted()
{
	
}

bool WebAppDescriptionUI::OnBeforeNavigate( LPCWSTR pszURL )
{
	wstring strUrl=pszURL;
	if((int)strUrl.find(L"action=readfile")>-1 && !_bDownLoad)
	{
		_bDownLoad = true;
		if(_IsHot24)
		{
			Statistic_StatFunc(Statistic_ID_DownNumOf24Hot);
		}
		Statistic_StatFunc(Statistic_ID_DownBntOfDetail);
		Statistic_StatFunc(Statistic_ID_DownLoadTotal);
		wstring name, version,fid;
		WebResUrlAnalysis::ParseDownUrl(strUrl,name,version,fid);

		//int pos=_contenttype.find(L"_");
		wstring plat=_mPlatForm==IPHONE?L"iPhone":L"iPad";

		wstring path = WebResourceModuleCONFIG(_strIosDownPath);


		CFileOperation::MakeLocalDir(path.substr(0,path.length()-1));
		string response = CNetOperation::Get(CCodeOperation::WstringToString(strUrl),false);

		wstring downurl=WebResUrlAnalysis::GetDownLoadUrl(response);
		if(!downurl.empty())
		{
			LOG->WriteDebug(pszURL);
// 			if (IsWindow(_lastMsgBoxHanle))
// 				SendMessage(_lastMsgBoxHanle,WM_CLOSE,IDCLOSE,0);
// 			CTaskItem* finitem=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.findSameItem(downurl.c_str(),path.c_str(),TASK_KIND::URL_DOWNLAOD);
// 			_box=new MessageBoxUI();
// 			if(!finitem)
// 			{
// 				TaskCenterModel::GetInstance()->StartDownLoad(downurl,path,name,version,plat,_wtoi(appid.c_str()));
// 				if(CONFIG(_bDownLoadMsg))
// 					NoticeDialog(BaseUI::LoadStringW(L"Task_Added_to_list"),ICON_SUCCESS, 1000 );
// 			}
// 			else
// 			{
// 				NoticeDialog(BaseUI::LoadStringW(L"Task_Exist_in_list"),ICON_WARNING, 2000 );
// 			}
// 			_lastMsgBoxHanle = _box->GetHWND();
			


			AnalysisData *data = new AnalysisData;
			DownProtocolAnalysis::WebSoftAnalysisUrl(*data,strUrl);
			data->url = downurl;
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
		}
		return true;
	}
	return true;
}

bool WebAppDescriptionUI::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	if(_pLoading == NULL)
		return true;

	_pLoading->SetVisible(false);
	_pIE->SetVisible(true);
	_pIE->Resize();
	_pIE->Invalidate();
	this->Resize();
	this->Invalidate();
	return true;
}

void WebAppDescriptionUI::OnRefresh()
{
	_pIE->Refresh();
}

std::wstring WebAppDescriptionUI::GetStyleID()
{
	return _T("WebAppDescriptionCtrl");
}

void WebAppDescriptionUI::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}

void WebAppDescriptionUI::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{
	_bDownLoad = false;
}
