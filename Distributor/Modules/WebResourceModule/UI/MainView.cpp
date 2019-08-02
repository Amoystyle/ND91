#include "stdafx.h"
#include "MainView.h"
#include "Logic/DownProtocolAnalysis.h"
#include "Message/Message.h"
#include "NetResourceStruct.h"
#include "BatchDownloadThread.h"
#include "Manager/ThreadManager.h"
#include "ChkCpaDispatcher.h"
#include "InstallDeviceListWnd.h"
#include "Common/WebResourceCommonFunc.h"
UfslibHandle _pUfsHandle = NULL;

HRESULT __stdcall StartChangeButtonState(DISPPARAMS* pDispParams, VARIANT* pVarResult, LPVOID pControlUI)
{
	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_AddCpaTask;
	msg.Send();
	return S_OK;
}

MainView::MainView()
{
	_webIeMap.clear();
	_webMain = NULL;
	_webIeUi = NULL;
	_isShowIe = true;
	_nDownloadFlag = 0;
}

MainView::~MainView()
{

}

wstring MainView::GetStyleID()
{
	return L"WebResource_View_Main";
}

void MainView::HideWebControl()
{
	LOG->WriteDebug(_T("显示IE控件前隐藏所有已生成的IE控件"));
	map<wstring,WebIeUIStr>::iterator iter;
	for (iter = _webIeMap.begin();iter != _webIeMap.end();iter++)
	{
		iter->second.web->SetVisible(false);
	}
	if (_webMain)
		_webMain->SetVisible(false);
}

void MainView::Loading(bool flag)
{
	if (!_isShowIe)
	{
		LOG->WriteDebug(_T("当前不允许显示IE控件  Return"));
		return;
	}
	CAnimationUI *pAnimationUI = dynamic_cast<CAnimationUI *>(GetItem(_T("Web.Loading")));
	if (pAnimationUI)
		pAnimationUI->SetVisible(flag);

	CLayoutUI *pLayoutUI = dynamic_cast<CLayoutUI *>(GetItem(_T("Web.LoadWeb")));
	if (pLayoutUI)
		pLayoutUI->SetVisible(!flag);

	this->Resize();
	this->Invalidate();
}

bool MainView::OnBeforeNavigate( LPCWSTR pszURL)
{
	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_ClickGotoDes;
	msg.Send();
	_nDownloadFlag = 0;
	wstring action = pszURL;
	if(action.find(_T("batchdownload")) != string::npos)
	{
		const string threadid = "BatchDownloadThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		BatchDownloadThread * batchdownload= new BatchDownloadThread(threadid);
		batchdownload->url = action;
		batchdownload->Run();
		return false;
	}
	
	return true;
}

bool MainView::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	return true;
}


void MainView::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{
	Loading(false);
	wstring action = pszURL;
	
	if(WebResourceCommonFunc::GetUrlActionValue(CCodeOperation::ws2s(action), "action") == "download")
	{
		AnalysisData *data = new AnalysisData;
		bool flag = DownProtocolAnalysis::AnalysisUrl(*data,pszURL,_T("download"));
		if (flag && data->url.length() != 0 && _nDownloadFlag == 0)
		{
			data->islegal = false;
			WebResourceInnserMsg msg;
			msg.type = WebResourceMsg_DownloadResource;
			msg.wParam = (WPARAM)data;
			msg.Send();
			LOG->WriteDebug(_T("查找到Download页面,发送下载消息"));
			LOG->WriteDebug(pszURL);
			_nDownloadFlag++;
		}
		else
			SAFE_DELETE(data);
	}
	else if (WebResourceCommonFunc::GetUrlActionValue(CCodeOperation::ws2s(action), "action") == "install")// action.find(_T("install")) != wstring::npos)
	{
		if (wstring::npos != action.find(_T("hashvalue"))
			&& wstring::npos!=action.find(L"platform")
			&& _nDownloadFlag == 0)
		{
			_nDownloadFlag++;
			if (wstring::npos == action.find(_T("hashvalue=")))
			{
				return;
			}
			int nPos = action.find(_T("hashvalue="))+10;

			wstring wsTemp = action.substr(nPos, action.length()-nPos);
			wstring wsHash = wsTemp.substr(0, wsTemp.find(_T("&")));
			vector<wstring> vec_Hash;
			vec_Hash.push_back(wsHash);
			if (vec_Hash.empty())
			{
				return;
			}

			int nPlatForm = 4;
			string strTemp = CCodeOperation::UnicodeToGB2312(action.substr(action.find(L"platform=")+9,1));
			nPlatForm = atoi(const_cast<char*>(strTemp.c_str()));

			DEVICE_OS_TYPE eType=Ios;
			if (E_ANDROID==E_PHONE_TYPE(nPlatForm))
			{
				eType = Android;
			}
			int nDevice = DEVICE_MANAGER->GetSuitPlateForm(eType);
			if (0 == nDevice)
			{
				//0台设备直接返回
				return;
			}
			else if (1 == nDevice)
			{
				//1台设备直接安装返回
				CInstallDeviceListWnd::DirectInstallOne(vec_Hash, E_PHONE_TYPE(nPlatForm));
				this->CheckReDownload(action, nPlatForm, vec_Hash);
				return;
			}
			if (E_ANDROID == E_PHONE_TYPE(nPlatForm))
			{
				if (CInstallDeviceListWnd::IsAnroideNoTips())
				{
					CInstallDeviceListWnd::DirectInstall(vec_Hash, E_PHONE_TYPE(nPlatForm));
					this->CheckReDownload(action, nPlatForm, vec_Hash);
					return;
				}
			}
			else
			{
				if (CInstallDeviceListWnd::IsIosNoTips())
				{
					CInstallDeviceListWnd::DirectInstall(vec_Hash, E_PHONE_TYPE(nPlatForm));
					this->CheckReDownload(action, nPlatForm, vec_Hash);
					return;
				}
			}

			CInstallDeviceListWnd* pWnd = new CInstallDeviceListWnd(E_PHONE_TYPE(nPlatForm));
			pWnd->Create(Singleton::GetMainWnd(), _T("Install_Devicelist_Wnd"));
			pWnd->CenterWindow(Singleton::GetMainWnd());
			pWnd->ShowDeviceList(E_PHONE_TYPE(nPlatForm));
			pWnd->SetHashValue(vec_Hash);
			int ret=pWnd->DoModal();

			if (CInstallDeviceListWnd::IsOnKeyInstall())
			{
				this->CheckReDownload(action, nPlatForm, vec_Hash);
			}
		}
	}
}

bool MainView::CheckReDownload(wstring wsUrl, int nPlatForm, vector<wstring>& vec_Hash)
{
	LocalCpaPathMsg CpaMsg;
	CpaMsg._vec_Hash = vec_Hash;
	CpaMsg.SendAndGetReturn();
	vector<wstring> vec_files = CpaMsg._vec_Path;
	if (vec_files.size()>0&&!CFileOperation::IsFileExist(vec_files[0].c_str()))
	{
		//先删除任务队列要重新下载软件的项目、
		AnalyUrlMsg AnalyMsg;
		AnalyMsg._strUrl = wsUrl;
		AnalyMsg.Send();

		Sleep(50);//避免，还未删除，先下载
		//重新下载软件并安装
		CpaPackageMsg msg;
		msg._strUrl = CCodeOperation::UnicodeToGB2312(wsUrl);
		msg._nPlatForm = nPlatForm;
		msg._eType = E_Single_Download;
		CpaReDownLoadAndInstall* pMsg = new CpaReDownLoadAndInstall;
		pMsg->st_bIsDownLoadAndInstal = true;
		pMsg->st_vec_Hash = vec_Hash;
		vector<wstring> vec_sn;
		DEVICE_OS_TYPE eType=Ios;
		if (E_ANDROID==E_PHONE_TYPE(nPlatForm))
		{
			eType = Android;
		}
		int nDevice = DEVICE_MANAGER->GetSuitPlateForm(eType);
		if (1==nDevice)
		{
			vec_sn = CInstallDeviceListWnd::GetInstallOne(E_PHONE_TYPE(nPlatForm));
		}
		vec_sn = CInstallDeviceListWnd::GetSnByPlate(E_PHONE_TYPE(nPlatForm));
		pMsg->st_vec_sn = vec_sn;
		msg._lparam = (void*)pMsg;
		msg.Send();
		return true;
	}

	return false;
}

void MainView::OnCreate()
{
	
}

void MainView::Init(CContainerUI* pContainer)
{
	BaseView::Init(pContainer);

}

void MainView::Prve( )
{
	if (_webIeUi && _webIeUi->CanForward())
		_webIeUi->Forward();
}

void MainView::Next( )
{
	if (_webIeUi && _webIeUi->CanGoBack())
		_webIeUi->GoBack();
}

void MainView::Refresh()
{
	if (_webIeUi)
		_webIeUi->Refresh();
}

void MainView::ShowWebResourceMain(ENUM_PLATFORM plat)
{
	LOG->WriteDebug(_T("显示正版体验页面"));
	HideWebControl();
	if (!_webMain)
	{
		CLayoutUI* pLayout=	(CLayoutUI*)GetItem(_T("Web"));
		RETURN_IF (!pLayout);
		_webMain = new WebSoftMainUI(Singleton::GetMainWnd());
		_webMain->Init(pLayout);
	}
	else
	{
		_webMain->SetVisible(true);
		_webMain->Resize();
		_webMain->Invalidate();
	}
	_webMain->OnWebPageSelChanged(plat);
	
}

CWebIEUI* MainView::ShowNewIE(wstring url)
{
	LOG->WriteDebug(_T("打开新的IE页面"));
	CWebIEUI*	LoginIE;
	LoginIE = new CWebIEUI();
	CLayoutUI* pLayout=	(CLayoutUI*)GetItem(_T("Web.LoadWeb"));
	RETURN_NULL_IF (!pLayout);
	RETURN_NULL_IF(!LoginIE);

	LoginIE->_pOb=this;
	LoginIE->SetAttribute(_T("id"),_T("WebBrowser"));
	LoginIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
	LoginIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
#ifndef _DEBUG
	LoginIE->SetContextMenuMode(1);		//针对右键菜单设置   目前设置为禁用右键菜单
#endif
	pLayout->Add(LoginIE);
	LoginIE->AddExtFunc(L"StartChangeButtonState",StartChangeButtonState);
	pLayout->Resize();
	pLayout->Invalidate();

	LoginIE->OpenURL(url);

	return LoginIE;
}

void MainView::Home( NetNode node )
{
	HideWebControl();
	map<wstring,WebIeUIStr>::iterator iter;
	iter = _webIeMap.find(node.nodeUrl);
	
	Loading(true);
	if (iter == _webIeMap.end())
	{
		CWebIEUI*	LoginIE = ShowNewIE(node.nodeUrl);
		if (!LoginIE)
		{
			LOG->WriteDebug(_T("主页---创建新的ie空间失败无法打开页面"));
			return;
		}
		WebIeUIStr str;
		str.web = LoginIE;
		str.node = node;
		_webIeMap[node.nodeUrl] = str;

		_webIeUi = LoginIE;
	}
	else
	{
		iter->second.web->SetVisible(true);
		iter->second.web->OpenURL(node.nodeUrl);
		iter->second.web->Resize();
		iter->second.web->Invalidate();
		LOG->WriteDebug(_T("显示主页"));
		_webIeUi = iter->second.web;
	}

	LOG->WriteDebug(node.nodeUrl);
}


void MainView::Search( NetNode node )
{
	HideWebControl();
	map<wstring,WebIeUIStr>::iterator iter;
	iter = _webIeMap.find(node.nodeUrl);
	
	Loading(true);
	if (iter == _webIeMap.end())
	{
		CWebIEUI*	LoginIE = ShowNewIE(node.searchUrl);
		if (!LoginIE)
		{
			LOG->WriteDebug(_T("搜索---创建新的ie空间失败无法打开页面"));
			return;
		}

		WebIeUIStr str;
		str.web = LoginIE;
		str.node = node;
		_webIeMap[node.nodeUrl] = str;

		_webIeUi = LoginIE;
	}
	else
	{
		iter->second.web->SetVisible(true);
		iter->second.web->OpenURL(node.searchUrl);
		iter->second.web->Resize();
		iter->second.web->Invalidate();
		LOG->WriteDebug(_T("显示搜索"));
		_webIeUi = iter->second.web;
	}

	LOG->WriteDebug(node.searchUrl);
}


void MainView::Load( NetNode node )
{
	HideWebControl();
	map<wstring,WebIeUIStr>::iterator iter;
	iter = _webIeMap.find(node.nodeUrl);

	if (iter == _webIeMap.end())
	{
		CWebIEUI*	LoginIE = ShowNewIE(node.nodeUrl);
		if (!LoginIE)
		{
			LOG->WriteDebug(_T("Load---创建新的ie空间失败无法打开页面"));
			return;
		}

		WebIeUIStr str;
		str.web = LoginIE;
		str.node = node;
		_webIeMap[node.nodeUrl] = str;

		_webIeUi = LoginIE;
		Loading(true);
	}
	else
	{
		iter->second.web->SetVisible(true);
		iter->second.web->Resize();
		iter->second.web->Invalidate();
		LOG->WriteDebug(_T("Load---显示已有页面"));
		_webIeUi = iter->second.web;
		
	}

	LOG->WriteDebug(node.nodeUrl);
}

void MainView::WebSoftNext()
{
	if (_webMain)
		_webMain->OnGoBack();
}

void MainView::WebSoftRefresh()
{
	if (_webMain)
		_webMain->OnRefresh();
}

void MainView::WebSoftHome()
{
	if (_webMain)
		_webMain->OnBtnGoHome();
}

void MainView::WebSoftSearch( wstring searchbug )
{
	if (_webMain)
		_webMain->OnSearch(searchbug);
}


void MainView::Show()
{
	_isShowIe = true;

	CLayoutUI *pLayoutUI = dynamic_cast<CLayoutUI *>(GetItem(_T("Web.LoadWeb")));
	if (pLayoutUI)
		pLayoutUI->SetVisible(true);

	LOG->WriteDebug(_T("IE控件设置为允许显示"));

	BaseView::Show();
}

void MainView::Hide()
{
	_isShowIe = false;

	CLayoutUI *pLayoutUI = dynamic_cast<CLayoutUI *>(GetItem(_T("Web.LoadWeb")));
	if (pLayoutUI)
		pLayoutUI->SetVisible(false);

	LOG->WriteDebug(_T("IE控件设置为不允许显示"));
	BaseView::Hide();
}

wstring MainView::GetCpaHashValue()
{
	wstring strResult = L"";
	if (_webIeUi)
	{
		VARIANT vtResult;
		::VariantInit(&vtResult);
		HRESULT result =CActiveXUI::InvokeMethod(_webIeUi->GetHtmlWindow(), L"GetPageHashValue",&vtResult, NULL, 0);
		if (S_OK == result)
		{
			if (NULL != vtResult.pbstrVal)
			{
				strResult= (wchar_t*)(vtResult.pbstrVal);
			}
			
		}
	}
	return strResult;
		
}

void MainView::SetButtonStatus( VARIANT *param, int cArgs )
{
	if (_isShowIe)
	{
		if (_webIeUi)
		{
			VARIANT vtResult;
			HRESULT result =CActiveXUI::InvokeMethod(_webIeUi->GetHtmlWindow(), L"ChangeButtonStatus",&vtResult, param, cArgs);
			assert(result == S_OK);
		}
	}
	
}

bool MainView::Handle( const CpaWebButtonStatusChangedMsgEx* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	VARIANT paramTmp[2];
	paramTmp[0] = pMsg->param[0];
	paramTmp[1] = pMsg->param[1];
	SetButtonStatus(paramTmp,pMsg->cArgs);
	return true;
}
