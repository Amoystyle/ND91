#include "StdAfx.h"
#include "WebCpaView.h"

struct WebCapStruct
{
	int key;
	
	/*platform这个按这个枚举来定义
	enum E_PHONE_TYPE
	{
		E_None = 0,
		E_IPHONE,
		E_WINDOW_MOBILE,
		E_SYMBIAN_S60,
		E_ANDROID,
		E_ANDROID_OMS,
		E_M8,
		E_IPAD,
		E_WP7,
		E_ANDROID_PAD,
		E_ANDROID_TV,
	};
	*/
	int platform;
};
HRESULT __stdcall StartChangeButtonState(DISPPARAMS* pDispParams, VARIANT* pVarResult, LPVOID pControlUI)
{
 	/*CWebIEUI2* p = (CWebIEUI2*)pControlUI;
	
	VARIANT vtResult;
	::VariantInit(&vtResult);
	HRESULT result =CActiveXUI::InvokeMethod(p->GetHtmlWindow(), L"GetPageHashValue",&vtResult, NULL, 0);
	if (S_OK == result)
	{
		if (NULL != vtResult.bstrVal)
		{
			WebCapStruct* st = (WebCapStruct*)p->GetUserData();
			AddCpaButtonStatusChangedMsg msg;
			msg.strParam = vtResult.bstrVal;
			msg.nParam = st->key;
			msg.platform = st->platform;
			msg.Send();
		}
	}*/
	HomeWebCpaButtonStatusChangedMsg msg;
	msg.SendToUI();
	return S_OK;
}

CWebCpaView::CWebCpaView(wstring deviceId, wstring url)
{

	_deviceId = deviceId;
	_nDownloadFlag = 0;
	_nPlatForm     = 4;//默认安装平台
	_wsUrl = url;
}

CWebCpaView::~CWebCpaView(void)
{
}

void CWebCpaView::OnCreate()
{
}

void CWebCpaView::OnInitShow()
{
	this->ShowIE(_wsUrl);
}
void CWebCpaView::ShowIE(wstring url)
{

	//this->ShowWindow();
	CAnimationUI *pAnimationUI = dynamic_cast<CAnimationUI *>(GetItem(_T("Web.Loading")));
	if (pAnimationUI)
		pAnimationUI->SetVisible(true);
	
	LoginIE = new CWebIEUI2();
	LoginIE->AddExtFunc(L"StartChangeButtonState",StartChangeButtonState);
	LoginIE->SetContextMenuMode(1);		//针对右键菜单设置   目前设置为禁用右键菜单

	CreateCpaDispatherMsg guidMsg;
	guidMsg.nParam = 0;
	guidMsg.SendAndGetReturn();

	CLayoutUI* pLayout=	(CLayoutUI*)GetItem(_T("Web.LoadWeb"));
assert(pLayout);

	LoginIE->_pOb=this;
	LoginIE->SetAttribute(_T("id"),_T("WebBrowser2"));
	LoginIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
	LoginIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	pLayout->Add(LoginIE);

	_nPlatForm = 4;
	if (wstring::npos!=url.find(L"platform"))
	{
		string strTemp = CCodeOperation::UnicodeToGB2312(url.substr(url.find(L"platform=")+9,1));
		_nPlatForm = atoi(const_cast<char*>(strTemp.c_str()));
	}
	WebCapStruct* stIE = new WebCapStruct();
	stIE->key = guidMsg.nParam;
	_cpadispatherGUID = guidMsg.nParam;
	stIE->platform = _nPlatForm;
	LoginIE->SetUserData((UINT)stIE);
	LoginIE->Show((BSTR)(url.c_str()));

	pLayout->Resize();
	pLayout->Invalidate();
}

bool CWebCpaView::OnBeforeNavigate( LPCWSTR pszURL)
{
	_nDownloadFlag = 0;
	wstring action = pszURL;
	if(action.find(_T("batchdownload")) != string::npos)
	{
		CpaPackageMsg msg;
		msg._strUrl = CCodeOperation::UnicodeToGB2312(pszURL);
		msg._nPlatForm = _nPlatForm;
		msg._eType = E_Mutile_Download;
		msg.Send();
		return false;
	}

	return true;
}

bool CWebCpaView::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	/*CLayoutUI* pLayout=	(CLayoutUI*)GetItem(_T("Web.LoadWeb"));
assert(pLayout);
	pLayout->Resize();
	pLayout->Invalidate();*/
	return true;
}


void CWebCpaView::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{
	CAnimationUI *pAnimationUI = dynamic_cast<CAnimationUI *>(GetItem(_T("Web.Loading")));
	if (pAnimationUI)
		pAnimationUI->SetVisible(false);

	wstring action = pszURL;
	if(GetUrlActionValue(CCodeOperation::ws2s(action), "action") == "download")
	{
		if (_nDownloadFlag == 0)
		{
			CpaPackageMsg msg;
			msg._strUrl = CCodeOperation::UnicodeToGB2312(pszURL);
			msg._nPlatForm = _nPlatForm;
			msg._eType = E_Single_Download;
			msg.Send();
			_nDownloadFlag++;
		}
	}
	else if (GetUrlActionValue(CCodeOperation::ws2s(action), "action") == "install")
	{
		if (wstring::npos != action.find(_T("hashvalue")) && _nDownloadFlag == 0)
		{
			if (wstring::npos == action.find(_T("hashvalue=")))
			{
				_nDownloadFlag++;
				return;
			}
			int nPos = action.find(_T("hashvalue="))+10;

			wstring wsTemp = action.substr(nPos, action.length()-nPos);
			wstring wsHash = wsTemp.substr(0, wsTemp.find(_T("&")));
			vector<wstring> vec_Hash;
			vec_Hash.push_back(wsHash);
			if (vec_Hash.empty())
			{
				_nDownloadFlag++;
				return;
			}
			
			//this->CheckInstalledCpa(CCodeOperation::UnicodeToGB2312(vec_Hash[0]));

			LocalCpaPathMsg CpaMsg;
			CpaMsg._vec_Hash = vec_Hash;
			CpaMsg.SendAndGetReturn();
			vector<wstring> vec_files = CpaMsg._vec_Path;
			if (vec_files.size()>0&&!CFileOperation::IsFileExist(vec_files[0].c_str()))
			{
				if (_nDownloadFlag == 0)
				{
					//先删除任务队列要重新下载软件的项目、
					AnalyUrlMsg AnalyMsg;
					AnalyMsg._strUrl = action;
					AnalyMsg.Send();

					Sleep(50);//避免，还未删除，先下载
					//重新下载软件
					CpaPackageMsg msg;
					msg._strUrl = CCodeOperation::UnicodeToGB2312(pszURL);
					msg._nPlatForm = _nPlatForm;
					msg._eType = E_Single_Download;
					CpaReDownLoadAndInstall* pMsg = new CpaReDownLoadAndInstall;
					pMsg->st_bIsDownLoadAndInstal = true;
					pMsg->st_vec_Hash = vec_Hash;
					vector<wstring> vec_sn;
					vec_sn.push_back(_deviceId);
					pMsg->st_vec_sn = vec_sn;
					msg._lparam = (void*)pMsg;
					msg.Send();
					_nDownloadFlag++;
					return;
				}
			}

			_nDownloadFlag++;

			vector<wstring> vec_sn;
			vec_sn.push_back(_deviceId);

			WebSoftInstallMsg msg;
			msg._st_Hash = vec_Hash;
			msg._st_Sn = vec_sn;
			msg.Send();
			
		}
	}
}

void CWebCpaView::CheckInstalledCpa(string strHash)
{
	//判断该软件是否已经装过该手机
	DeviceData* pData = DEVICE_MANAGER->FindDevice(_deviceId);
	DeviceInfo* pInfo = NULL;
	if (NULL == pData)
	{
		return;
	}
	pInfo = pData->GetMutableDevInfo();
	if (NULL == pInfo)
	{
		return;
	}

	CheckCpaInstalledMsg ChkMsg;
	ChkMsg._strHash = strHash;
	if (4 == _nPlatForm)
	{
		wstring wsTemp = pData->GetId().empty() ? pInfo->_deviceIMEI :  pData->GetId();

		if ( wsTemp.empty() )
			wsTemp = pInfo->_strSerialNumber;

		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
	}
	else
	{
		wstring wsTemp = pData->GetId().empty() ? pInfo->_deviceIMEI : pData->GetId();
		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
	}
	//ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(pInfo->_deviceIMEI);
	ChkMsg.SendAndGetReturn();
	if (!ChkMsg._strReturn.empty())
	{//吐司
		CommonUIFunc::NoticeDialog(CCodeOperation::GB2312ToUnicode(ChkMsg._strReturn), ICON_WARNING);
		return;
	}

	return;
}

bool CWebCpaView::Handle( const CpaWebButtonStatusChangedMsgEx* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	if (LoginIE)
	{
		VARIANT vtResult;
		VARIANT paramTmp[2];
		paramTmp[0] = pMsg->param[0];
		paramTmp[1] = pMsg->param[1];
		HRESULT result =CActiveXUI::InvokeMethod(LoginIE->GetHtmlWindow(), L"ChangeButtonStatus",&vtResult, paramTmp, pMsg->cArgs);
		assert(result == S_OK);
	}
	return true;
}

bool CWebCpaView::Handle( const HomeWebCpaButtonStatusChangedMsg* pMsg )
{

	VARIANT vtResult;
	::VariantInit(&vtResult);
	HRESULT result =CActiveXUI::InvokeMethod(LoginIE->GetHtmlWindow(), L"GetPageHashValue",&vtResult, NULL, 0);
	if (S_OK == result)
	{
		if (NULL != vtResult.bstrVal)
		{
			AddCpaButtonStatusChangedMsg msg;
			msg.strParam = vtResult.bstrVal;
			msg.nParam = 0;
			msg.platform = _nPlatForm;
			msg.PostToUI();
		}
	}
	return true;
}

string CWebCpaView::GetUrlActionValue( string url, string action )
{
	string result = "";
	vector<string> vecTmp1;
	Split(url,'?',vecTmp1);
	if (vecTmp1.size() >= 2)
	{
		vector<string> vecParam;
		Split(vecTmp1[1],'&',vecParam);
		for (vector<string>::iterator iter = vecParam.begin(); iter != vecParam.end(); ++iter)
		{
			vector<string> actionAndValue;
			Split((*iter),'=',actionAndValue);
			if (actionAndValue.size()>=2)
			{
				if (actionAndValue[0] == action)
				{
					result = actionAndValue[1];
					break;
				}
			}
		}
	}
	return result;
}

void CWebCpaView::Split( const string &s, char delim, vector<string> &elems )
{
	istringstream iss(s);
	string item;
	while(getline(iss,item,delim))
	{
		elems.push_back(item);
	}
}