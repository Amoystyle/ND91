#include "stdafx.h"
#include "WebResourceController.h"
#include "UI/MainView.h"
#include "UI/LeftView.h"
#include "WebResourceLoadThread.h"
#include "WebResourceManager.h"
#include "Singleton.h"
#include "Manager/ThreadManager.h"
#include "UI/TopView.h"
#include "Logic/DownProtocolAnalysis.h"
#include "UI/BatchDownloadThread.h"
#include <cstdlib>
#include <algorithm>
#include "UI/InstallDeviceListWnd.h"
#include "CCallJSHelper.h"
#include "ChkCpaDispatcher.h"
#include <atlconv.h>
#include "CpaDataHelper.h"
#include "LocalCpaCheckThread.h"

WebResourceController::WebResourceController()
{
	_pLeftView = new LeftView();
	_TopOperateView = new TopView();
	_pMainView = new MainView();
	_bIsLeftDataLoaded = false;
	_bIsShow = false;
	//_nDownloadFlag = 0;
	guid = L"";
	_pCpaDispatcher = new CChkCpaDispatcher();
	_pOutCpaDispathers = new map<CPaDispatcherID, CChkCpaDispatcher*>();
	_iAndroidDeviceCount = 0;
	_iIOSDeviceCount = 0;
	_isReloadLeftNode = false;
	_isRefreshCpaButtonStatus = false;
	
}

WebResourceController::~WebResourceController()
{
	if (_pCpaDispatcher!=NULL)
	{
		delete _pCpaDispatcher;
		_pCpaDispatcher = NULL;
	}
	if (NULL != _pOutCpaDispathers)
	{
		for (map<CPaDispatcherID, CChkCpaDispatcher*>::iterator iter = _pOutCpaDispathers->begin(); iter != _pOutCpaDispathers->end(); ++iter)
		{
			if (NULL != iter->second)
			{
				delete iter->second;
				iter->second = NULL;
			}
		}
		_pOutCpaDispathers->clear();
		delete _pOutCpaDispathers;
		_pOutCpaDispathers = NULL;
	}
}

bool WebResourceController::Init(CContainerUI* pLeft,  CContainerUI* pContainer )
{
	Init();
	CCpaDataHelper::GetInstance()->Init();
	_pLeftView->Init(pLeft);
	_TopOperateView->Init(pContainer);
	_pMainView->Init(pContainer);

	return true;
}

void WebResourceController::Init()
{
	_mapMsgRecFuns[WebResourceMsg_UrlChanged] = &WebResourceController::LeftTreeNodeChanged;
	_mapMsgRecFuns[WebResourceMsg_DownloadResource] = &WebResourceController::DownloadResource;
	_mapMsgRecFuns[WebResourceMsg_LeftLoad] = &WebResourceController::LoadLeftTreeData;
	_mapMsgRecFuns[WebResourceMsg_LeftLoad_UI] = &WebResourceController::AddLeftTreeData;
	_mapMsgRecFuns[WebResourceMsg_LeftTreeLoaded] = &WebResourceController::LeftTreeLoaded;
	_mapMsgRecFuns[WebResourceMsg_TopOperate] = &WebResourceController::TopOperate;
	_mapMsgRecFuns[WebResourceMsg_TopSearch] = 	&WebResourceController::WebSearch;
	_mapMsgRecFuns[WebResourceMsg_HideTopSearch] = 	&WebResourceController::HideTopSearch;
	_mapMsgRecFuns[WebResourceMsg_AddCpaTask] = &WebResourceController::AddCpaTask;
}

void WebResourceController::Show()
{
	_pLeftView->Show();
	_TopOperateView->Show();
	_pMainView->Show();
	if (_isReloadLeftNode)
	{
		_isReloadLeftNode = false;
		WebResourceInnserMsg innerMsg;
		innerMsg.type = WebResourceMsg_LeftLoad_UI;
		innerMsg.wParam = Etype_Application;
		ENUM_PLATFORM platform = PLATFORM_Android;
		string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
		if (!url.empty())
		{
			platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
		}
		innerMsg.lParam = platform;
		innerMsg.PostToUI();

	}

	if (_isRefreshCpaButtonStatus)
	{
		_isRefreshCpaButtonStatus = false;
		AddCpaTask();
	}
	
	_bIsShow = true;
	
}

void WebResourceController::Hide()
{
	_pLeftView->Hide();
	_TopOperateView->Hide();
	_pMainView->Hide();
	_bIsShow = false;
}

bool WebResourceController::Handle( const WebResourceInnserMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}

	//if (pMsg->type == WebResourceMsg_ClickGotoDes)
		//_nDownloadFlag = 0;

	MessageReceiverFun_MAP::iterator it(_mapMsgRecFuns.find(pMsg->type));
	if (it != _mapMsgRecFuns.end())
	{
		MessageReceiveFun pFun = it->second;
		(this->*pFun)(pMsg);
	}
	return true;
}


bool WebResourceController::Handle( const ResponseUserLoginMsg* pMsg )
{
	AutoLock;
	if (NULL == pMsg)
	{
		return true;
	}
	if (pMsg->nCpaUser == 1)
	{
		_CurLoginUserInfo.isCPACompetence = true;
	}
	else
	{
		_CurLoginUserInfo.isCPACompetence = false;
	}
	
	_CurLoginUserInfo.name = pMsg->userName;
	_isReloadLeftNode = true;
	if (!_bIsShow)
	{
		return true;
	}
	_isReloadLeftNode = false;
	WebResourceInnserMsg innerMsg;
	innerMsg.type = WebResourceMsg_LeftLoad_UI;
	innerMsg.wParam = Etype_Application;
	ENUM_PLATFORM platform = PLATFORM_Android;
	string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
	if (!url.empty())
	{
		platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
	}
	innerMsg.lParam = platform;
	innerMsg.PostToUI();

	return true;
}

bool WebResourceController::Handle( const QuitMsg* pMsg )
{
	AutoLock;
	if (NULL == pMsg)
	{
		return true;
	}
	_CurLoginUserInfo.isCPACompetence = false;
	_CurLoginUserInfo.name = L"";
	_isReloadLeftNode = true;
	if (!_bIsShow)
	{
		return true;
	}
	_isReloadLeftNode = false;
	WebResourceInnserMsg innerMsg;
	innerMsg.wParam = Etype_Application;
	ENUM_PLATFORM platform = PLATFORM_Android;
	string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
	if (!url.empty())
	{
		platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
	}
	innerMsg.lParam = platform;
	innerMsg.type = WebResourceMsg_LeftLoad_UI;
	innerMsg.PostToUI();
	return true;
}

bool WebResourceController::Handle( GetLoginUserInfoMsg* pMsg )
{
	AutoLock;
	pMsg->pLoginInfo.name = _CurLoginUserInfo.name;
	pMsg->pLoginInfo.isCPACompetence = _CurLoginUserInfo.isCPACompetence;
	return true;
}	

bool WebResourceController::Handle(const CpaPackageMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return true;
	}

	if (E_Mutile_Download==pMsg->_eType)
	{
		const string threadid = "BatchDownloadThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		BatchDownloadThread * batchdownload= new BatchDownloadThread(threadid);
		batchdownload->url = CCodeOperation::GB2312ToUnicode(pMsg->_strUrl);
		batchdownload->SetCpa(true);
		batchdownload->_pCpaInfo = pMsg->_lparam;
		batchdownload->SetPlatForm(pMsg->_nPlatForm);
		batchdownload->Run();
	}
	else
	{
		AnalysisData *data = new AnalysisData;
		bool flag = DownProtocolAnalysis::AnalysisUrl(*data,CCodeOperation::GB2312ToUnicode(pMsg->_strUrl),_T("download"));
		if (flag && data->url.length() != 0 )
		{
			data->islegal = false;
			WebResourceInnserMsg DownLoadmsg;
			DownLoadmsg.type = WebResourceMsg_DownloadResource;
			DownLoadmsg.wParam = (WPARAM)data;
			DownLoadmsg.nParam = pMsg->_nPlatForm;
			DownLoadmsg.strParam=L"CPA";
			DownLoadmsg.pCpaInfo = pMsg->_lparam;
			DownLoadmsg.Send();
		}
		else
			SAFE_DELETE(data);
	}

	return true;
}
bool WebResourceController::Handle(const DeviceChangeMsg* pMsg)
{
	if (0 == pMsg->phoneType)
	{
		CInstallDeviceListWnd::SetAnroideNoTips(false);
	}
	else
	{
		CInstallDeviceListWnd::SetIosNoTips(false);
	}
	bool bIsPostToUI = false;
	bool bIsOUTPostToUI = false;
	if (pMsg->_bLinkOut)
	{
		if (pMsg->phoneType == 0)
		{
			if (_iAndroidDeviceCount > 0)
			{
				_iAndroidDeviceCount--;
			}
			if (_iAndroidDeviceCount <= 0)
			{
				string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
				ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
				if (platform == PLATFORM_Android)
				{
					bIsPostToUI = true;
					
				}
				bIsOUTPostToUI = true;
			}
		}
		else
		{
			if (_iIOSDeviceCount > 0)
			{
				_iIOSDeviceCount--;
			}
			if (_iIOSDeviceCount <= 0)
			{
				string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
				ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
				if (platform != PLATFORM_Android)
				{
					bIsPostToUI = true;
				}
				bIsOUTPostToUI = true;
			}
		}
	}
	else
	{
		if (pMsg->phoneType == 0)
		{
			if (_iAndroidDeviceCount <= 0)
			{
				string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
				ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
				if (platform == PLATFORM_Android)
				{
					bIsPostToUI = true;
				}
				bIsOUTPostToUI = true;
			}
			_iAndroidDeviceCount++;
		}
		else
		{
			if (_iIOSDeviceCount <= 0)
			{
				string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
				ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
				if (platform != PLATFORM_Android)
				{
					bIsPostToUI = true;
				}
				bIsOUTPostToUI = true;
			}
			_iIOSDeviceCount++;
		}
	}

	if (bIsPostToUI)
	{
		WebResourceInnserMsg msg;
		msg.type = WebResourceMsg_AddCpaTask;
		msg.PostToUI();
		LOG->WriteDebug(L"DeviceChangeMsg To WebResourceMsg_AddCpaTask");
	}
	if( bIsOUTPostToUI )
	{
		HomeWebCpaButtonStatusChangedMsg homeWebCpaMsg;
		homeWebCpaMsg._st_sn = pMsg->_st_sn;
		homeWebCpaMsg.phoneType = pMsg->phoneType;
		homeWebCpaMsg._bLinkOut = pMsg->_bLinkOut;
		homeWebCpaMsg.PostToUI();
	}
	
	return true;
}

bool WebResourceController::Handle( CpaWebIsCancelMsg* pMsg )
{
	if (guid != pMsg->strParam && L"0" != pMsg->strParam)//0表示一定要执行
	{
		pMsg->bIsCancel = true;
	}
	return true;
}

bool WebResourceController::Handle( const CpaWebButtonStatusChangedMsg* pMsg )
{
	if (pMsg->strParam == guid || pMsg->nParam == 0 ||  L"0" == pMsg->strParam)//pMsg->strParam == 0表示外部dispatcher一定要执行
	{
		VARIANT vt[2];
		vt[0].vt = VT_I4;
		vt[0].intVal = CpaButonStatus_DOWNLOAD;
		if (pMsg->bIsExist)
		{ 
			vt[0].intVal = CpaButonStatus_DOWNLOADFINISH;
			if (PLATFORM_Android == pMsg->platform)
			{
				if (_iAndroidDeviceCount > 0)
				{
					vt[0].intVal = CpaButonStatus_INSTALL;
				}
			}
			else
			{
				if (_iIOSDeviceCount > 0)
				{
					vt[0].intVal = CpaButonStatus_INSTALL;
				}
			}

		}
		vt[1].vt = VT_BSTR;
		vt[1].bstrVal = W2BSTR(pMsg->strHashValue.c_str());
		CpaWebButtonStatusChangedMsgEx msg;
		msg.param[0] = vt[0];
		msg.param[1] = vt[1];
		msg.cArgs = 2;
		msg.PostToUI();
		if (!_bIsShow)
		{
			_isRefreshCpaButtonStatus = true;
		}
	}
	
	return true;
}

bool WebResourceController::Handle(LocalCpaPathMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return false;
	}
	pMsg->_vec_Path = CCpaDataHelper::GetInstance()->GetMultiItemPath(pMsg->_vec_Hash);
	return true;
}


void WebResourceController::LoadLeftTreeData(const WebResourceInnserMsg* msg )
{
	if (_bIsLeftDataLoaded)
	{
		WebResourceInnserMsg msgToUI;
		msgToUI.type = WebResourceMsg_LeftTreeLoaded;
		msgToUI.wParam = msg->wParam;
		msgToUI.lParam = msg->lParam;
		msgToUI.Send();
	}
	else
	{
		string threadId = "WebResourceController_WebResourceMsg_LeftLoad_" +  CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		CWebResourceLoadThread *pleftTreeLoadInThread = new CWebResourceLoadThread(threadId);
		ENUM_PLATFORM platform = (ENUM_PLATFORM) msg->lParam;
		NetEnumAppSort appSort = (NetEnumAppSort) msg->wParam;
		pleftTreeLoadInThread->setAppSort(appSort);
		pleftTreeLoadInThread->setPlatform(platform);
		pleftTreeLoadInThread->Run();
	}
	
	
}

void WebResourceController::AddLeftTreeData(const WebResourceInnserMsg* msg )
{
	if(NULL == msg)
	{
		return;
	}
	_pLeftView->ClearLeftTree();
	vector<NetModuleData> stData = CWebResourceManager::GetInstance()->GetNetModuleDatas();
	ENUM_PLATFORM platform = (ENUM_PLATFORM) msg->lParam;
	NetEnumAppSort appSort = (NetEnumAppSort) msg->wParam;
	NetEnumPhoneOS phoneOS = ePhoneOS_None_Net;
	if (platform == PLATFORM_Android)
	{
		phoneOS = ePhoneOS_Android_Net;
	}
	else
	{
		phoneOS = ePhoneOS_Ios_Net;
	}
	wstring moduleId = CWebResourceManager::GetInstance()->GetModuleDataId(appSort);
	for (vector<NetModuleData>::iterator iter = stData.begin(); iter != stData.end(); ++iter)
	{
		NetModuleData netModuleData = *iter;
		//lParam=表示平台（类型=NetEnumPhoneOS），wParam表示分类（类型=NetEnumAppSort），如：应用，游戏等
		if (netModuleData.moduleId.compare(moduleId) == 0)
		{
			if (netModuleData.platform == phoneOS)
			{
				for (vector<NetSite>::iterator netSiteIter = netModuleData.sites.begin(); netSiteIter != netModuleData.sites.end(); netSiteIter++)
				{
					NetSite siteTemp = *netSiteIter;
					if (!_CurLoginUserInfo.isCPACompetence)
					{
						for (vector<NetNode>::iterator iterNode = siteTemp.nodes.begin(); iterNode != siteTemp.nodes.end(); ++iterNode)
						{
							if (iterNode->strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(Cpa))
							{
								siteTemp.nodes.erase(iterNode);
								break;
							}
						}
					}
					CWebResourceManager::ChangeNetNodeUrl(siteTemp, platform);
					this->_pLeftView->AddNode(siteTemp);
					break;
				}
			}
		}
	}
	_pLeftView->OnFinish();
}

void WebResourceController::LeftTreeNodeChanged( const WebResourceInnserMsg* msg )
{
	if (NULL == msg)
	{
		return;
	}
	if (!_bIsShow)
	{
		return;
	}
	SetGuid();
	NetNode* nodeTmp = (NetNode *)msg->lParam;
	if(NULL == nodeTmp)
	{
		return;
	}
	_CurNetNode = (*nodeTmp);
	wstring searchTmp =  _CurNetNode.searchUrl;
	string search = CCodeOperation::ws2s(_CurNetNode.searchUrl);
	string key = CWebResourceManager::GetInstance()->ClearSearchKey(search);
	if (key != "")
	{
		_CurNetNode.searchUrl = CCodeOperation::s2ws(search);
	}
	delete nodeTmp;
	nodeTmp = NULL;
	NetNode node(_CurNetNode);
	if (node.strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(ItunesApp2))
	{
		string url = CCodeOperation::ws2s(node.nodeUrl);
		ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
		_pMainView->ShowWebResourceMain(platform);
	}
	else
	{
		if (key != "")
		{
			node.initUrl = searchTmp;
			node.nodeUrl = searchTmp;
			node.searchUrl = CCodeOperation::s2ws(search);
		}
		_pMainView->Load(node);
	}
	
	_TopOperateView->ClickLeftTree(node);
	AddCpaTask();
}

void WebResourceController::WebSearch( const WebResourceInnserMsg* msg )
{
	if (NULL == msg)
	{
		return;
	}
	wstring text = (wstring)msg->strParam;
	if (_CurNetNode.strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(ItunesApp2))
	{
		if (text.empty())
		{
			_pMainView->WebSoftHome();
		}
		else
		{
			_pMainView->WebSoftSearch(text);
		}
	}
	else
	{
		NetNode node(_CurNetNode);
		if(!text.empty())
		{
			wstring textTmp = CCodeOperation::StringToWstring(CNetOperation::URLEncode(CCodeOperation::UnicodeToUTF_8(text)));
			node.searchUrl += textTmp;
		}
		else
		{
			node.searchUrl = node.initUrl;
		}
		if (_CurNetNode.strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(Search))
		{
			_pMainView->Search(node);
		}
		else
		{
			_pLeftView->SelectSearchNode(node);
		}
		
	}
	
}

void WebResourceController::DownloadResource( const WebResourceInnserMsg* msg )
{
	if (NULL == msg)
	{
		return;
	}
	
	//if (_nDownloadFlag == 1)
	//{
	//	_nDownloadFlag = 0;
	//	return;
	//}
	
	AnalysisData* pInfo = (AnalysisData*)msg->wParam;
	if (NULL==pInfo)//使用指针最好先判断下，避免崩溃
	{
		return;
	}

	wstring wsTemp=L"";
	ENUM_PLATFORM platform;
	if (E_None==(E_PHONE_TYPE)msg->nParam)
	{
		if (_CurNetNode.nodeUrl.empty())
		{
			return;
		}
		string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
		platform= CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
		wsTemp = _CurNetNode.strValue;
	}
	else
	{
		if (E_IPHONE==(E_PHONE_TYPE)msg->nParam)//iphone
		{
			platform = PLATFORM_Iphone;
		}
		else if (E_ANDROID==(E_PHONE_TYPE)msg->nParam)
		{
			platform = PLATFORM_Android;
		}
		else if (E_IPAD==(E_PHONE_TYPE)msg->nParam)
		{
			platform = PLATFORM_Ipad;
		}
		wsTemp = msg->strParam;
	}

	transform(wsTemp.begin(), wsTemp.end(), wsTemp.begin(), ::toupper);

	wstring strPlatform = L"Android";

	wstring path = WebResourceModuleCConfig::GetInstance()->_strAndroidDownPath;


	if (platform == PLATFORM_Android)
	{
		strPlatform = L"Android";
		if (wsTemp==L"CPA")
			path = CONFIG(_strAndroid_CpaDownPath);
		else
			path = CONFIG(_strAndroidDownPath);
		if (path == L"")
		{
			if (wsTemp!=L"CPA")
				path = WebResourceModuleCConfig::GetInstance()->_strAndroidDownPath;
		}
	}
	else if (platform == PLATFORM_Iphone)
	{
		strPlatform = L"iPhone";
		if (wsTemp==L"CPA")
			path = CONFIG(_strIos_CpaDownPath);
		else
			path = CONFIG(_strIosDownPath);
		if (path == L"")
		{
			if (wsTemp!=L"CPA")
				path =  WebResourceModuleCConfig::GetInstance()->_strIosDownPath;
		}
	}
	else if (platform == PLATFORM_Ipad)
	{
		strPlatform = L"iPad";
		if (wsTemp==L"CPA")
			path = CONFIG(_strIos_CpaDownPath);
		else
			path = CONFIG(_strIosDownPath);

		if (path == L"")
		{
			if (wsTemp!=L"CPA")
				path =  WebResourceModuleCConfig::GetInstance()->_strIosDownPath;
		}
	}
	CWebResourceDownload downloadMsg;
	downloadMsg._url = pInfo->url;
	downloadMsg._savepath = path;
	downloadMsg._appname = pInfo->name;
	downloadMsg._version = pInfo->vername;
	downloadMsg._platform = strPlatform;
	downloadMsg._bIsLegal = pInfo->islegal;
	downloadMsg._pCpaInfo = msg->pCpaInfo;
	string strFid = CCodeOperation::ws2s(pInfo->resid);
	int iFid = atoi(strFid.c_str());
	downloadMsg._fid = iFid;

	delete pInfo;
	pInfo = NULL;
	downloadMsg.PostToUI();

	//_nDownloadFlag ++;
}

void WebResourceController::LeftTreeLoaded( const WebResourceInnserMsg* msg )
{
	if (NULL == msg)
	{
		return;
	}
	_bIsLeftDataLoaded = true;
	WebResourceInnserMsg msgToUI;
	msgToUI.type = WebResourceMsg_LeftLoad_UI;
	msgToUI.wParam = msg->wParam;
	msgToUI.lParam = msg->lParam;
	msgToUI.PostToUI();
}

void WebResourceController::TopOperate( const WebResourceInnserMsg* msg )
{
	if (NULL == msg)
	{
		return;
	}
	SetGuid();
	ENUM_PLATFORM platform = (ENUM_PLATFORM)msg->lParam;
	ENUM_URL_OPREATE urlOperate = (ENUM_URL_OPREATE)msg->wParam;
	if (_CurNetNode.strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(ItunesApp2))
	{
		switch(urlOperate)
		{
		case Etype_UrlOperate_Back:    //向后
			_pMainView->WebSoftNext();	
			break;
		case Etype_UrlOperate_Reflash: //刷新
			_pMainView->WebSoftRefresh();
			break;
		case Etype_UrlOperate_Home:     //主页
			{
				_pMainView->WebSoftHome();
			}
			break;
		}
	}
	else
	{
		switch(urlOperate)
		{
		case Etype_UrlOperate_Forword: //向前
			{
				_pMainView->Prve();
			}
			break;
		case Etype_UrlOperate_Back:    //向后
			_pMainView->Next();
			break;
		case Etype_UrlOperate_Reflash: //刷新
			_pMainView->Refresh();
			break;
		case Etype_UrlOperate_Home:     //主页
			{
				NetNode node(_CurNetNode);
				_pMainView->Home(node);
			}
			break;
		}
	}
	
}

void WebResourceController::HideTopSearch(const WebResourceInnserMsg* msg)
{
	if (NULL == msg)
	{
		return;
	}
	if (1==msg->wParam)
	{
		_TopOperateView->HideSearch(true);
	}
	else
	{
		_TopOperateView->HideSearch(false);
	}
}


void WebResourceController::SetGuid()
{
	guid = CCodeOperation::s2ws(WebResourceCommonFunc::GetGuid());
}

void WebResourceController::AddCpaTask( const WebResourceInnserMsg* msg )
{
	AddCpaTask();
}

void WebResourceController::AddCpaTask()
{
	if (_CurNetNode.strValue == CWebResourceManager::GetInstance()->GetNetNodeValue(Cpa))
	{
		SetGuid();
		wstring hashValueTmp = this->_pMainView->GetCpaHashValue();
		string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
		ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
		_pCpaDispatcher->AddTask(hashValueTmp,platform,guid);
	}
}

bool WebResourceController::Handle( const LocalCpaInfoInitFinishedMsg* pMsg )
{
	string url = CCodeOperation::ws2s(_CurNetNode.nodeUrl);
	ENUM_PLATFORM platform = CWebResourceManager::GetInstance()->GetPlatformFromUrl(url);
	if (pMsg->bIsAndroidPlatform)
	{
		if (platform == PLATFORM_Android)
		{
			AddCpaTask();
		}
	}
	else
	{
		if (platform != PLATFORM_Android)
		{
			AddCpaTask();
		}
	}
	if (pMsg->bIsAllFinished)
	{
		string threadId = "CLocalCpaCheckThread_" +  CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		CLocalCpaCheckThread *pThread = new CLocalCpaCheckThread(threadId);
		pThread->Run();
	}
	return true;
}

bool WebResourceController::Handle( const AddCpaButtonStatusChangedMsg* pMsg )
{
	CPaDispatcherID key = (CPaDispatcherID)(pMsg->nParam);
	if (_pOutCpaDispathers->find(key) != _pOutCpaDispathers->end())
	{
		CChkCpaDispatcher* dispatch = (*_pOutCpaDispathers)[key];
		wstring hashValueTmp = pMsg->strParam;
		int platform = pMsg->platform;
		if (platform == 4)
		{
			dispatch->AddTask(hashValueTmp,PLATFORM_Android,L"0");
		}
		else
		{
			dispatch->AddTask(hashValueTmp,PLATFORM_Iphone,L"0");
		}
	}
	
	
	return true;
}

bool WebResourceController::Handle( CreateCpaDispatherMsg* pMsg )
{
	CPaDispatcherID key = (CPaDispatcherID)(pMsg->nParam);
	if (_pOutCpaDispathers->find(key) == _pOutCpaDispathers->end())
	{
		CChkCpaDispatcher* dispatcher = new CChkCpaDispatcher();
		(*_pOutCpaDispathers)[key] = dispatcher;
	}
	
	return true;
}

bool WebResourceController::Handle(const AnalyUrlMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	AnalysisData *data = new AnalysisData;
	bool flag = DownProtocolAnalysis::AnalysisUrl(*data,pMsg->_strUrl,_T("download"));
	if (flag && data->url.length() != 0 )
	{
		DelTaskItemMsg DelMsg;
		DelMsg._strUrl = data->url;
		DelMsg.SendToUI();
		SAFE_DELETE(data);
	}
	return true;
}