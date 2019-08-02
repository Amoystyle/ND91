#include "StdAfx.h"
#include <CommDlg.h>
#include "AppModel.h"
#include "Common/BoolLock.h"
#include "Module/Server/GetAppUpdateInfo.h"
#include "AppDataUpgrade.h"
#include "Logic/WebResModel.h"

#include "AppThread/LoadAppThread.h"
#include "AppThread/UnInstallAppThread.h"
#include "AppThread/InstallAppThread.h"
#include "AppThread/UpGradeSinglePriacyThread.h"
#include "AppThread/AppLoadIconThread.h"
#include "AppThread/UpgradeAllThread.h"
#include "AppThread/NoticeMsgThread.h"

#include "Common/UserCenter/UserCenterMsg.h"
#include <algorithm>

bool sortByName(const AppData* lh, const AppData* rh)
{
	return lh->_AppName<rh->_AppName;
}

App_CC_BEGIN
#define CONTINUE_IF(x)            if ( x ) { continue; }
#define CHECK_EXIT(x)             if( x ){return true;}
AppModel::AppModel(ApplicationInfoInterface* pUI) 
: _pUI(pUI)
{INTERFUN;

	_strIconDir = CONFIG(_strIconPath);

    CFileOperation::MakeLocalDir(_strIconDir);
    InitializeCriticalSection(&_lock);
	InitializeCriticalSection(&_Uninstall_lock);
    _haveLoad       = false;
    _bLoadingData   = false;

	_UnInstallAppIng = false;

	_curDeviceID = L"";
	_bExit = false;
}

AppModel::~AppModel()
{INTERFUN;
    DeleteCriticalSection(&_lock);
	DeleteCriticalSection(&_Uninstall_lock);
	_bExit = true;
}

void AppModel::LoadAndDisplayData( wstring id )
{INTERFUN;
    _curDeviceID = id;
    if (!_haveLoad)
	{
		LoadAppThread* pLoad = new LoadAppThread(this);
		pLoad->Run();
	}
}

bool AppModel::LoadAppData()
{INTERFUN;
    SetBLockReturn(_bLoadingData, true);
    SetCSLock(_lock);
	bool result=true;
	try
	{
		if (_haveLoad)
		{
			return true;
		}

		GET_DEVICE(_curDeviceID, false);
        int nTick = ::GetTickCount();
        while (!pDevice->IsActivate() && ::GetTickCount() - nTick < 20000)
        {
            Sleep(100);
		}
		if(!pDevice->IsActivate())
		{
			LOG->WriteDebug(L"LoadAppData出错, pDevice->IsActivate() == false");
			return false;
		}
CHECK_EXIT(_bExit);
		_pUI->GetSDCardState();
CHECK_EXIT(_bExit);
		_pUI->SendToUI(WM_APP_SETAttribute);
CHECK_EXIT(_bExit);
		_pUI->ClearData();

		TRACE(_T("AppModel::PreLoadData 1"));   
CHECK_EXIT(_bExit);
		_pUI->ShowLoadingPicture(true);

		const vector<AppData*>* pDatas = NULL;
		vector<AppData*> vecTempData;//临时变量
		pDevice->GetAppManager()->Load();
		TRACE(_T("AppModel::PreLoadData e"));
		pDatas = pDevice->GetAppManager()->GetDatas();

		// 防止机制失败或者读取失败，再读一次
		if (pDatas->size() == 0)    
		{

			RETURN_TRUE_IF(_pUI == NULL);
CHECK_EXIT(_bExit);
			_pUI->ShowLoadingPicture(true);

			pDevice->GetAppManager()->Load();

			pDatas = pDevice->GetAppManager()->GetDatas();
		}     

		BeginInstallDeamonMsg DeamonMsg;
		DeamonMsg.deviceID = pDevice->GetId();
		DeamonMsg.bAppSync = false;
		DeamonMsg.SendToUI();

		// 显示
		RETURN_TRUE_IF(_pUI == NULL);
CHECK_EXIT(_bExit);
		_pUI->ShowLoadingPicture(false);
		this->BuildeTempData(pDatas, vecTempData);
		if (vecTempData.size() > 0)
		{
			LOG->WriteDebug(L"AppModel::LoadAppData Have data. NO is "+ CStrOperation::IntToWString(vecTempData.size()));
		}
		////先进行排序
		sort(vecTempData.begin(), vecTempData.end(), sortByName);
CHECK_EXIT(_bExit);
        _pUI->InsertToAppView(&vecTempData);
		this->ClearTempData(vecTempData);
		
		// 载入图标
		RETURN_TRUE_IF(_pUI == NULL);
		CFileOperation::MakeLocalDir(_strIconDir);

		this->BuildeTempData(pDatas, vecTempData);
CHECK_EXIT(_bExit);
		_pUI->SetAppLogoWithoutLoad(&vecTempData, pDevice);//第一次加载的时候图标先不加载到最后开一个线程加载
	    //_pUI->SetAppLogo(&vecTempData, pDevice);	
		this->ClearTempData(vecTempData);

		// 获取应用大小
		if (pDevice->GetAppManager()->GetAppDiskUsage())
		{
			pDatas = pDevice->GetAppManager()->GetDatas();
			this->BuildeTempData(pDatas, vecTempData);
CHECK_EXIT(_bExit);
			_pUI->SetAppSize(&vecTempData, pDevice);
			this->ClearTempData(vecTempData);
		}
CHECK_EXIT(_bExit);
		// 数据更新
		_pUI->UpdateData();

		
		this->BuildeTempData(pDatas, vecTempData);

		CheckPiracyUpgradeAppList(&vecTempData);
CHECK_EXIT(_bExit);
		_pUI->SetUpgradeBtn(&_PiracyUpgradeApps, pDevice);
		wstring s = L"";
		CheckLegalUpgradeAppList(&vecTempData);
CHECK_EXIT(_bExit);
		_pUI->SetLegalUpgradeBtn(&_LegalUpgradeApps, pDevice);
		_haveLoad = true;
		LoadAppIconInThread(&vecTempData);
		this->ClearTempData(vecTempData);
	}
	catch(...)
	{
		CHECK_EXIT(_bExit);
		_pUI->ShowLoadingPicture(false);
		result= false;
	}
	
    return result;
}

bool AppModel::MoveToApp(AppData* pApp)
{INTERFUN;
    RETURN_FALSE_IF(!pApp);
    GET_DEVICE(_curDeviceID, false);

	MoveTaskItemMsg MoveMsg;
	MoveMsg.pData = pApp;
	MoveMsg.curDeviceID = _curDeviceID;
	MoveMsg.staticDiskUsage = (long)pApp->_staticDiskUsage;
	MoveMsg.SendToUI();

	NoticeMsgThread* pNoticeMsgThread = new NoticeMsgThread;
	pNoticeMsgThread->SetNoticeMsg(CommonUIFunc::LoadStringW(_T("Application_Text_APP_add_to_moveList")), L"", 2000);
	pNoticeMsgThread->Run();
    return true;
}

void AppModel::GetAppPackagePaths(vector<wstring>& files, bool isFile, wstring wsTitle)
{INTERFUN;
    if (isFile)//选择文件
    {
		vector<wstring> tempFiles;
        if (GetCurDeviceType() == Ios)
			tempFiles = BaseOperation::MultiFileDialog(Singleton::GetMainWnd(), L"苹果应用程序(*.ipa;*.pxl)\0*.ipa;*.pxl\0\0", OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ALLOWMULTISELECT, wsTitle);
        else
			tempFiles = BaseOperation::MultiFileDialog(Singleton::GetMainWnd(), L"安卓应用程序(*.apk;*.npk)\0*.apk;*.npk\0\0", OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ALLOWMULTISELECT, wsTitle);

		bool bShow = true;
		//检查选择文件格式
		for (vector<wstring>::iterator it = tempFiles.begin();it < tempFiles.end();it++)
		{
			wstring ext = CFileOperation::GetFileExt(*it);
			
			if (ext!=L"ipa"&&ext!=L"deb"&&ext!=L"pxl"&&ext!=L"apk"&&ext!=L"npk")
			{
				if (bShow)
				{
					wchar_t buffer[1024];
					const wchar_t* pTemplate = CommonUIFunc::LoadString(_T("Application_Text_APP_error_format_tip"));
					wstring filename = CFileOperation::GetFileName(*it);
					swprintf_s(buffer, 1024, pTemplate, L"");
				    CommonUIFunc::NoticeDialog(buffer, ICON_WARNING);
					bShow = false;
				}
				continue;
			}
			files.push_back(*it);
			
		}

		InvalidateMainMsg MainMsg;
		MainMsg.SendToUI();
    }
    else//选择文件夹
    {
        wstring defaultDir = L"";
		wstring strIosDownPath = L"";
		wstring strAndroidDownPath = L"";

		strIosDownPath = CONFIG(_strIosDownPath);

		strAndroidDownPath = CONFIG(_strAndroidDownPath);

        defaultDir = GetCurDeviceType() == Ios ? strIosDownPath : strAndroidDownPath;
		wstring dir = BaseOperation::DirDialog(Singleton::GetMainWnd(), defaultDir);
        RETURN_IF(dir.empty());
		RETURN_IF(!FindCurDevice());
//add by zjf 优化代码
        vector<wstring> tempFiles;
        if (GetCurDeviceType() == Ios)
        {
            tempFiles.clear();
            CFileOperation::FindAllFileInDir(dir, tempFiles, L"ipa");
            //for (size_t i = 0; i < tempFiles.size(); i++)
            //    files.push_back(tempFiles.at(i));
			if (!tempFiles.empty())
			{
				files.resize(tempFiles.size());
				copy(tempFiles.begin(), tempFiles.end(), files.begin());
			}

			tempFiles.clear();
			CFileOperation::FindAllFileInDir(dir, tempFiles, L"pxl");
            //for (size_t i = 0; i < tempFiles.size(); i++)
            //    files.push_back(tempFiles.at(i));
			if (!tempFiles.empty())
			{
				copy(tempFiles.begin(), tempFiles.end(), back_inserter(files));
			}
        }
        else
        {
            tempFiles.clear();
            CFileOperation::FindAllFileInDir(dir, tempFiles, L"apk");
            //for (size_t i = 0; i < tempFiles.size(); i++)
            //    files.push_back(tempFiles.at(i));
			if (!tempFiles.empty())
			{
				files.resize(tempFiles.size());
				copy(tempFiles.begin(), tempFiles.end(), files.begin());
			}
            tempFiles.clear();
            CFileOperation::FindAllFileInDir(dir, tempFiles, L"npk");
            //for (size_t i = 0; i < tempFiles.size(); i++)
            //    files.push_back(tempFiles.at(i));

			if (!tempFiles.empty())
			{
				copy(tempFiles.begin(), tempFiles.end(), back_inserter(files));
			}
        }
        
        if (files.empty())
            CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_no_change_to_list")), ICON_WARNING);
    }
}

void AppModel::GetAppFilePath(vector<wstring>& file)
{
	file = _files;
}
bool AppModel::InstallApp(bool isFile, wstring wsTitle)
{INTERFUN;
    GET_DEVICE(_curDeviceID, false);
    vector<wstring> files;
    GetAppPackagePaths(files, isFile, wsTitle);
    RETURN_FALSE_IF(files.empty());   
	RETURN_FALSE_IF(!FindCurDevice());
	_files = files;
	if (files.size()<=0)
	{
		return false;
	}
	else
	{
		bool bRes = InstallApp(files);

		if (files.size()>1)
		{
			AddCaseMsg addCaseMsg;
			addCaseMsg.pPathList = (void*)&files;
			addCaseMsg.SendToUI();
		}

		return bRes;
	}
    
}

bool AppModel::InstallAppThread(const vector<wstring>& vecfiles, const wstring strSn)
{INTERFUN;
    vector<wstring> files;
    files.insert(files.end(), vecfiles.begin(), vecfiles.end());
	wstring wsSn = strSn;
    for (vector<wstring>::iterator it = files.begin(); it != files.end(); it++)
    {
        wstring strFilePath = *it;
        wstring strName;
		wstring version,iconpath,plat;
		bool   isLegalSoft=false; 

		AppData* pData=AppManager::GetAppInfoByFile(strFilePath,DataPath::GetTempDir()+L"\\",true);
		if (!pData)	//应用程序包有问题时不进行继续操作
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadStringW(_T("Application_Text_APP_Install_faile")), ICON_WARNING);
			return false;
		}
		if(pData)
		{
			strName=pData->_AppName;
			version=pData->_strVersion;
			iconpath=pData->_strIconPCPath;
			plat=pData->_eAppType<=APP_TYPE_NPK?L"Android":L"iPhone";
			isLegalSoft=pData->_isLegalSoft==1;
		}
		wstring strOptIni;

		strOptIni = CONFIG(_strOptionIni);
       
		RequestInstallPositionMsg requestInstallPositionMsg(strSn);
		requestInstallPositionMsg.SendAndGetReturn();
		//MyFileOperation::GetIniFileValue(strOptIni, _T("Android"), _T("installonphone"));

        bool bOnSDCard = requestInstallPositionMsg.bInstallOnPhone;//(strInstall == L"0" ? true : false);
		bool bCurDevice = false;
		if (wsSn.empty())
		{
			bCurDevice = true;
			RETURN_FALSE_IF(!FindCurDevice());
		}

		InstallTaskItemMsg Installmsg;
		Installmsg.appname = strName;
		Installmsg.appfilepath = strFilePath;
		Installmsg.version = version;
		Installmsg.filesize = CFileOperation::GetFileSizeW(strFilePath);
		Installmsg.deviceId = bCurDevice ? FindCurDevice()->GetId() : wsSn;
		Installmsg.onSDcard = bOnSDCard;
		Installmsg.plat = plat;
		Installmsg.fid  = 0;
		Installmsg.casename = L"";
		Installmsg.icon = iconpath;
		Installmsg.isLegalSoft = isLegalSoft;
		Installmsg.SendAndGetReturn();

		int taskid=Installmsg.taskid;

		if(taskid>0)
		{
			map<int,AppDataUpgrade*>::iterator it = _InstallTasks.find(taskid);
			if(it == _InstallTasks.end())
			{
				vector<AppDataUpgrade*>* appdataVec = isLegalSoft? &_LegalUpgradeApps:&_PiracyUpgradeApps;
				for(vector<AppDataUpgrade*>::iterator it = appdataVec->begin();it!=appdataVec->end();it++)
				{
					AppDataUpgrade* appdata = *it;
					if(appdata->_packageName==pData->_packageName)
					{
						_InstallTasks[taskid] = appdata;
						break;
					}
				}
			}
		}

		//通过路径获得哈希值
		string strHash = CFileOperation::GetFileMD5(strFilePath);
		if (!strHash.empty())
		{
			//向服务端验证是否已经装过cpa软件
			CheckCpaInstalledMsg ChkMsg;
			ChkMsg._strHash = strHash;
			GET_DEVICE(_curDeviceID, false);
			DeviceInfo* pInfo = pDevice->GetMutableDevInfo();
			if (NULL == pInfo)
			{
				return false;
			}
			if (L"Android" == plat)
			{
				wstring wsTemp = pInfo->_deviceIMEI.empty() ? pInfo->_deviceWiFiMac :  pInfo->_deviceIMEI;

				if ( wsTemp.empty() )
					wsTemp = pInfo->_strSerialNumber;

				ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
			}
			else
			{
				wstring wsTemp = wsSn.empty() ? pInfo->_deviceIMEI : wsSn;
				ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
			}

			ChkMsg.SendAndGetReturn();
			if (!ChkMsg._strReturn.empty())
			{//吐司
				CommonUIFunc::NoticeDialog(CCodeOperation::GB2312ToUnicode(ChkMsg._strReturn),ICON_WARNING);
			}
		}
    }
    return true;
}

bool AppModel::InstallApp( vector<wstring> files )
{INTERFUN;
    InstallAppDataThread* pInstallAppThread = new InstallAppDataThread(this);
    for (vector<wstring>::iterator it = files.begin(); it != files.end(); it++)
        pInstallAppThread->AddStrParam(*it);

	pInstallAppThread->SetStr(_curDeviceID);
	pInstallAppThread->Run();

    CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_add_to_list")), ICON_SUCCESS);
    return true;
}


bool AppModel::UnInstallApp(vector<AppData*>* datas)
{INTERFUN;
    RETURN_FALSE_IF(datas->empty());

    UnInstallAppThread* pUnInstallAppThread = new UnInstallAppThread(this);
    for (vector<AppData*>::iterator it = datas->begin(); it != datas->end(); it++)
	{
		
		// 判断应用是否在卸载，如果在卸载则不重复卸载
		if (!CheckAppIsInUnstall(*it))
		{
			pUnInstallAppThread->AddVoidParam(*it);
			EnterCriticalSection(&_Uninstall_lock);
			_UnInstallAppDatas.push_back(*it);
			LeaveCriticalSection(&_Uninstall_lock);
		}
	}
	if (pUnInstallAppThread->GetVoidParam()->size() <= 0) return false;
	
	pUnInstallAppThread->Run();

    return true;
}

bool AppModel::UnInstallApp( AppData* pApp )
{INTERFUN;
    RETURN_FALSE_IF(!pApp);
    RETURN_FALSE_IF(!CheckAppData(pApp));
	RETURN_FALSE_IF(CheckAppIsInUnstall(pApp));

	EnterCriticalSection(&_Uninstall_lock);
	_UnInstallAppDatas.push_back(pApp);
	LeaveCriticalSection(&_Uninstall_lock);

	UnInstallAppThread* pUnInstallAppThread = new UnInstallAppThread(this);
	pUnInstallAppThread->AddVoidParam(pApp);

	pUnInstallAppThread->Run();

    return true;
}

bool AppModel::UnInstallAppAction( AppData* pApp)
{INTERFUN;
    RETURN_FALSE_IF(!pApp);
	
	wstring packagename = pApp->_packageName;
    GET_DEVICE(_curDeviceID, false);
    if (!CheckAppData(pApp)) return false;
    CommandExecuteResult res = pDevice->GetAppManager()->UnInstallApp(pApp);
    RETURN_FALSE_IF(!pApp);
    if (COMMAND_EXECUTE_SUCCESS == res)
	{
		CHECK_EXIT(_bExit);
        _pUI->RemoveFromAppView(pApp);
	}
    else
	{
		NoticeMsgThread* pNoticeMsgThread = new NoticeMsgThread;
		pNoticeMsgThread->SetNoticeMsg(CommonUIFunc::LoadString(L"Application_Text_APP_uninstall_faile")+pApp->_AppName, L"", 2000);
		pNoticeMsgThread->Run();
	}

	//从安装的应用中移除
    SetCSLock(_Uninstall_lock);	
	vector<AppData*>::iterator it = _UnInstallAppDatas.begin();
	it = _UnInstallAppDatas.begin();
	while (it != _UnInstallAppDatas.end())
	{
		if (pApp == (*it)) _UnInstallAppDatas.erase(it);
		break;
	}

	bool isupdateapp = false;
	for(vector<AppDataUpgrade*>::iterator it = _PiracyUpgradeApps.begin(); it!=_PiracyUpgradeApps.end();it++)
	{
		AppDataUpgrade* appit = *it;
		if(appit->_packageName==packagename)
		{
			_PiracyUpgradeApps.erase(it);
			isupdateapp = true;
			break;
		}
	}

	for(vector<AppDataUpgrade*>::iterator it = _LegalUpgradeApps.begin(); it!=_LegalUpgradeApps.end();it++)
	{
		AppDataUpgrade* appit = *it;
		if(appit->_packageName==packagename)
		{
			_LegalUpgradeApps.erase(it);
			isupdateapp = true;
			break;
		}
	}
	if(isupdateapp)
	{
		DeviceData* pDevice = BaseOperation::FindDevice(_curDeviceID);
		CHECK_EXIT(_bExit);
		_pUI->SetUpgradeBtn(&_PiracyUpgradeApps, pDevice);
		CHECK_EXIT(_bExit);
		_pUI->SetLegalUpgradeBtn(&_LegalUpgradeApps, pDevice);
	}

    return true;
}

bool AppModel::CheckAppData( AppData* pApp )
{INTERFUN;
    return true;

    GET_DEVICE(_curDeviceID, false);
    const vector<AppData*>* pDatas = pDevice->GetAppManager()->GetDatas();
    for (vector<AppData*>::const_iterator it = pDatas->begin(); it != pDatas->end(); it++)
    {
        if (pApp == *it)
        {
            return true;
        }
    }
    return false;
}

DEVICE_OS_TYPE AppModel::GetCurDeviceType()
{INTERFUN;
    DeviceData* curDevice = FindCurDevice();
    RETURN_VALUE_IF(!curDevice, Android);
    return curDevice->GetDevInfo()->_deviceType;
}

SDCardState AppModel::GetSDCardState()
{INTERFUN;
    DeviceData* curDevice = FindCurDevice();
    RETURN_VALUE_IF(!curDevice, UNMOUNTED);
    return curDevice->GetSystemManager()->GetSDCardState();
}

bool AppModel::IsRootDevice()
{INTERFUN;
    DeviceData* curDevice = FindCurDevice();
    RETURN_TRUE_IF(!curDevice);
    RETURN_TRUE_IF(curDevice->GetDevInfo()->_deviceType == Ios)
    return curDevice->GetDevInfo()->_bRoot;
}

bool AppModel::IsJailBreak()
{INTERFUN;
	DeviceData* curDevice = FindCurDevice();
	RETURN_FALSE_IF(!curDevice);
	RETURN_FALSE_IF(curDevice->GetDevInfo()->_deviceType == Android)
	return curDevice->GetDevInfo()->_ios_bJailBreaked;
}

void AppModel::RefreshData() 
{INTERFUN;
	if (_UnInstallAppIng)
		return;
    if (_bLoadingData) 
        return;
    RETURN_IF(!FindCurDevice());
    _haveLoad = false;
    LoadAndDisplayData(_curDeviceID);
}

// 检查AppData是否在安装中
bool AppModel::CheckAppIsInUnstall( AppData* pApp )
{INTERFUN;
	SetCSLock(_Uninstall_lock);
	vector<AppData*>::iterator it = _UnInstallAppDatas.begin();
	while (it != _UnInstallAppDatas.end())
	{
		if (pApp == (*it)) return true;
		it++;
	}
	return false;
}

void AppModel::CheckPiracyUpgradeAppList( const vector<AppData*>* datas )
{
	_PiracyUpgradeApps.clear();
	DeviceData* curDevice = FindCurDevice();
	RETURN_IF(!curDevice);
	RETURN_IF(curDevice->GetDevInfo()->_deviceType == Ios && !curDevice->GetDevInfo()->_ios_bJailBreaked);
	DEVICE_OS_TYPE ostype = GetCurDeviceType();
	vector<AppUpdateInfo*> updateapps;
	for (size_t i = 0; i < datas->size(); i++)
	{
		const AppData* data = datas->at(i);
		if (data == NULL) continue;
		if (data->_packageName.empty()) continue;
		if (data->_eAppOwner == APP_OWNER_SYS) continue;
		if (data->_eAppType == APP_TYPE_DEB) continue;
		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
		updateappinfo->_identifier = data->_packageName;
		updateappinfo->_oldVersion = data->_strVersion;
		updateappinfo->_verCode = ostype==Ios?L"0":CStrOperation::IntToWString(data->_nVersionCode);
		updateapps.push_back(updateappinfo);
	}
	DeviceData* devdata = FindCurDevice();
	CGetAppUpdateInfo* getappinfo = new CGetAppUpdateInfo(devdata);
	bool Getinforesult = false;
	Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);
	if(Getinforesult == false)
		Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);
	if(Getinforesult == true)
	{
		for(size_t i=0;i<updateapps.size();i++)
		{
			AppUpdateInfo* updateappinfo = updateapps.at(i);
			if(updateappinfo->_newVersion.length() > 0)
			{
				for (size_t i = 0; i < datas->size(); i++)
				{
					const AppData* data = datas->at(i);
					if(data->_packageName==updateappinfo->_identifier)
					{
						if(data->_strVersion==updateappinfo->_newVersion)
							break;
						AppDataUpgrade* appupgrade = new AppDataUpgrade(data);
						appupgrade->PiracyUrl(updateappinfo->_url);
						appupgrade->PiracyNewVersion(updateappinfo->_newVersion);
						appupgrade->FileSize(updateappinfo->_nsize);
						appupgrade->FID(updateappinfo->_nfid);
						_PiracyUpgradeApps.push_back(appupgrade);
						break;
					}
				}
			}
		}
	}
}

void AppModel::UpgradeSinglePiracyAppData( AppDataUpgrade* appdata )
{INTERFUN;
	RETURN_IF(!appdata);

	UpGradeSinglePriacyThread* pUpGradeSinglePriacyThread = new UpGradeSinglePriacyThread(this);
	pUpGradeSinglePriacyThread->SetVoid(appdata);
	pUpGradeSinglePriacyThread->SetbLegal(false);
	pUpGradeSinglePriacyThread->Run();

	return;
}

void AppModel::CheckLegalUpgradeAppList( const vector<AppData*>* datas )
{
	_LegalUpgradeApps.clear();
	DeviceData* curDevice = FindCurDevice();
	RETURN_IF(!curDevice);
	RETURN_IF(curDevice->GetDevInfo()->_deviceType == Android);
	vector<AppUpdateInfo*> updateapps;
	vector<AppUpdateInfo*> getitemidapps;
	for (size_t i = 0; i < datas->size(); i++)
	{
		const AppData* data = datas->at(i);
		if (data == NULL) continue;
		if (data->_packageName.empty()) continue;
		if (data->_eAppOwner == APP_OWNER_SYS) continue;
		if (data->_eAppType == APP_TYPE_DEB) continue;

		if(data->_itemId>1000000)
		{
			AppUpdateInfo* updateappinfo = new AppUpdateInfo();
			updateappinfo->_identifier = data->_packageName;
			updateappinfo->_oldVersion = data->_strVersion;
			updateappinfo->_nfid = data->_itemId;
			updateapps.push_back(updateappinfo);
		}
		else
		{
			AppUpdateInfo* getitemidappinfo = new AppUpdateInfo();
			getitemidappinfo->_identifier = data->_packageName;
			getitemidapps.push_back(getitemidappinfo);
		}
	}
	DeviceData* devdata = FindCurDevice();
	CGetAppUpdateInfo* getappinfo = new CGetAppUpdateInfo(devdata);
	getappinfo->GetItunesidByIdentify(&getitemidapps);
	for(vector<AppUpdateInfo*>::iterator itupapp=getitemidapps.begin(); itupapp!=getitemidapps.end();itupapp++)
	{
		AppUpdateInfo* upinfo = *itupapp;
		if(upinfo->_itunesid>100000)
		{
			for (size_t i = 0; i < datas->size(); i++)
			{
				const AppData* data = datas->at(i);
				if (data->_packageName.empty()) continue;
				if (data->_eAppOwner == APP_OWNER_SYS) continue;
				if (data->_eAppType == APP_TYPE_DEB) continue;
				if(data->_itemId<1000000 && CStrOperation::CompareUnCase(data->_packageName, upinfo->_identifier) ==0)
				{
					AppUpdateInfo* updateappinfo = new AppUpdateInfo();
					updateappinfo->_identifier = data->_packageName;
					updateappinfo->_oldVersion = data->_strVersion;
					updateappinfo->_nfid = upinfo->_itunesid;
					
					updateapps.push_back(updateappinfo);
				}
			}
		}
	}
	bool Getinforesult = false;
	Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, true);
	if(Getinforesult == false)
		Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, true);
	if(Getinforesult == true)
	{
		for(size_t i=0;i<updateapps.size();i++)
		{
			AppUpdateInfo* updateappinfo = updateapps.at(i);
			if(updateappinfo->_newVersion.length() > 0)
			{
				for (size_t i = 0; i < datas->size(); i++)
				{
					const AppData* data = datas->at(i);
					if(data->_packageName==updateappinfo->_identifier)
					{
						if(data->_strVersion==updateappinfo->_newVersion)
							break;
						AppDataUpgrade* appupgrade = new AppDataUpgrade(data);
						appupgrade->LegalUrl(updateappinfo->_url);
						appupgrade->LegalNewVersion(updateappinfo->_newVersion);
						appupgrade->FileSize(updateappinfo->_nsize);
						appupgrade->FID(updateappinfo->_nfid);
						_LegalUpgradeApps.push_back(appupgrade);
						break;
					}
				}
			}
		}
	}
}

void AppModel::UpgradeSingleLegalAppData( AppDataUpgrade* appdata )
{
	RETURN_IF(!appdata);
	UpGradeSinglePriacyThread* pUpGradeSinglePriacyThread = new UpGradeSinglePriacyThread(this);
	pUpGradeSinglePriacyThread->SetVoid(appdata);
	pUpGradeSinglePriacyThread->SetbLegal(true);
	pUpGradeSinglePriacyThread->Run();

	return;
}

void AppModel::UpgradeLegalAppDatas( vector<AppDataUpgrade*>* appdataVec )
{INTERFUN;
	DeviceData* curDevice = FindCurDevice();
	RETURN_IF(!curDevice);
	vector<AppUpdateInfo*> updateapps;
	for (vector<AppDataUpgrade*>::iterator it = appdataVec->begin(); it != appdataVec->end(); it++)
	{
		AppDataUpgrade* appInfo=  *it;
		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
		updateappinfo->_nfid = appInfo->FID();
		updateappinfo->_url = appInfo->LegalUrl();
		updateappinfo->_name = appInfo->_AppName;
		updateappinfo->_newVersion = appInfo->LegalNewVersion();
		updateappinfo->_identifier = appInfo->_packageName;
		updateapps.push_back(updateappinfo);
	}
	for(vector<AppUpdateInfo*>::iterator it = updateapps.begin();it != updateapps.end();it++)
	{
		AppUpdateInfo* appInfo = *it;
		int fidstr = appInfo->_nfid;
		wstring downurl = appInfo->_url;
		int legalindex = downurl.find(L"itunesapp2");
		if(legalindex>0)
		{
			string content=CNetOperation::Get(CCodeOperation::UnicodeToGB2312(downurl),false);
			int pos= content.find("href=");
			CONTINUE_IF(pos<0);
			content =content.substr(pos+6);
			int pos1=content.find("'");
			CONTINUE_IF(pos1<0);
			content =content.substr(0,pos1);
			content = CNetOperation::Get(content,false);
			pos = content.find("href=\"");
			CONTINUE_IF(pos<0);
			content =content.substr(pos+6);
			pos1=content.find("\"");
			CONTINUE_IF(pos1<0);
			content=content.substr(0,pos1);
			downurl = CCodeOperation::GB2312ToUnicode(content);
		}
		else
			continue;
		wstring platform = L"iPhone";
		wstring savepath;

		savepath = CONFIG(_strIosDownPath);

		TASK_UPGRADE_TYPE upgradetype = LEGAL2LEGAL;
		if(!appInfo->_bLegal)
			upgradetype=PIRACY2LEGAL;

		DownloadTaskItemMsg DownloadMsg;
		DownloadMsg.url = downurl;
		DownloadMsg.appname = appInfo->_name;
		DownloadMsg.savepath = savepath;
		DownloadMsg.version = appInfo->_newVersion;
		DownloadMsg.platform = platform;
		DownloadMsg.fid = fidstr;
		DownloadMsg.bUpdategrade = true;
		DownloadMsg.needInstall = true;
		DownloadMsg.oldappfullpath = L"";
		DownloadMsg.isLegalSoft = true;
		DownloadMsg.taskupgradetype = upgradetype;
		DownloadMsg.appID = appInfo->_identifier;
		DownloadMsg.SendAndGetReturn();
		int taskid = DownloadMsg.taskid;

		if(taskid>0)
		{
			map<int,AppDataUpgrade*>::iterator it = _UpgradeTasks.find(taskid);
			if(it == _UpgradeTasks.end())
			{
				for(vector<AppDataUpgrade*>::iterator it = appdataVec->begin();it!=appdataVec->end();it++)
				{
					AppDataUpgrade* appdata = *it;
					if(appdata->_packageName==appInfo->_identifier)
					{
						_UpgradeTasks[taskid] = appdata;
						break;
					}
				}
			}
		}
	}
}

void AppModel::UpgradePiracyAppDatas( vector<AppDataUpgrade*>* appdataVec )
{
	DeviceData* curDevice = FindCurDevice();
	RETURN_IF(!curDevice);
	DEVICE_OS_TYPE ostype = curDevice->GetDevInfo()->_deviceType;
	vector<AppUpdateInfo*> updateapps;
	for (vector<AppDataUpgrade*>::iterator it = appdataVec->begin(); it != appdataVec->end(); it++)
	{
		AppDataUpgrade* appInfo=  *it;
		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
		updateappinfo->_nfid = appInfo->FID();
		updateappinfo->_url = appInfo->PiracyUrl();
		updateappinfo->_name = appInfo->_AppName;
		updateappinfo->_newVersion = appInfo->PiracyNewVersion();
		updateappinfo->_identifier = appInfo->_packageName;
		updateapps.push_back(updateappinfo);
	}
	DeviceData* devdata = FindCurDevice();
	//CGetAppUpdateInfo *getrealurls = new CGetAppUpdateInfo(devdata);
	CGetAppUpdateInfo* getrealurls = new CGetAppUpdateInfo(devdata->GetDevInfo()->_deviceType==Ios?1:0);
	bool Geturlsresult = false;
	Geturlsresult = getrealurls->GetAppsRealUpdateUrls( &updateapps );
	if(Geturlsresult)
	{
		for(vector<AppUpdateInfo*>::iterator it = updateapps.begin();it != updateapps.end();it++)
		{
			AppUpdateInfo* appInfo = *it;
			wstring fidstr;
			WebResModel::ParseDownUrl(appInfo->_url,appInfo->_name,appInfo->_newVersion,fidstr);
			string response = CNetOperation::Get(CCodeOperation::WstringToString(appInfo->_url),false);
			int pos = response.find("<a href=");
			if(pos>-1)
			{
				string str1=response.substr(pos);
				pos=str1.find(">");
				appInfo->_url=CCodeOperation::StringToWstring(str1.substr(9,pos-10));
			}
			wstring platform = ostype==Ios? L"iPhone":L"Android";
			wstring strIosDownPath = L"";
			wstring strAndroidDownPath = L"";

			strIosDownPath = CONFIG(_strIosDownPath);

			strAndroidDownPath = CONFIG(_strAndroidDownPath);

			wstring savepath = ostype==Ios? strIosDownPath : strAndroidDownPath;
			TASK_UPGRADE_TYPE upgradetype = PIRACY2PIRACY;
			if(L"iPhone"==platform)
			{
				if(appInfo->_bLegal)
					upgradetype=LEGAL2PIRACY;
			}

			DownloadTaskItemMsg DownloadMsg;
			DownloadMsg.url = appInfo->_url;
			DownloadMsg.savepath = savepath;
			DownloadMsg.appname =appInfo->_name;
			DownloadMsg.version = appInfo->_newVersion;
			DownloadMsg.platform = platform;
			DownloadMsg.fid = _wtoi(fidstr.c_str());
			DownloadMsg.bUpdategrade = true;
			DownloadMsg.needInstall = true;
			DownloadMsg.oldappfullpath = L"";
			DownloadMsg.isLegalSoft = false;
			DownloadMsg.taskupgradetype = upgradetype;
			DownloadMsg.appID = appInfo->_identifier;
			DownloadMsg.SendAndGetReturn();
			int taskid = DownloadMsg.taskid;

			if(taskid>0)
			{
				map<int,AppDataUpgrade*>::iterator it = _UpgradeTasks.find(taskid);
				if(it == _UpgradeTasks.end())
				{
					for(vector<AppDataUpgrade*>::iterator it = appdataVec->begin();it!=appdataVec->end();it++)
					{
						AppDataUpgrade* appdata = *it;
						if(appdata->_packageName==appInfo->_identifier)
						{
							_UpgradeTasks[taskid] = appdata;
							break;
						}
					}
				}
			}
		}
	}
}

int AppModel::GetSameUpgradeAppsCount()
{
	int num = 0;
	for (size_t i = 0; i < _PiracyUpgradeApps.size(); i++)
	{
		AppData* pApp = _PiracyUpgradeApps.at(i);
		if(!pApp->_isLegalSoft)
			num++;
	}
	for (size_t i = 0; i < _LegalUpgradeApps.size(); i++)
	{
		AppData* pApp = _LegalUpgradeApps.at(i);
		if(pApp->_isLegalSoft)
			num++;
	}
	return num;
}

void AppModel::UpgradeAllAppData(_APP_UPGRADE_TYPE upgradeType)
{
	UpgradeAllThread* pUpgradeAllThread = new UpgradeAllThread(this);
	pUpgradeAllThread->SetUpgradeType((int)upgradeType);
	pUpgradeAllThread->Run();
}

void AppModel::UpgradeAllAppDataInternal(_APP_UPGRADE_TYPE upgradeType)
{
	vector<wstring> installFilePaths;
	vector<AppDataUpgrade*>* upgradePiracyApps = new vector<AppDataUpgrade*>();
	vector<AppDataUpgrade*>* upgradeLegalApps = new vector<AppDataUpgrade*>();

	_INSTALLS_CASE_DATA_TYPE casetype = GetCurDeviceType() == Ios? _INSTALLS_CASE_DATA_IOS:_INSTALLS_CASE_DATA_ANDROID;

	if(upgradeType==_APP_UPGRADE_TYPE_TOPIRACY)
	{
		GetAllInstallAndUpgradeApps(upgradePiracyApps, false, false, &installFilePaths);
	}
	else if (upgradeType==_APP_UPGRADE_TYPE_TOLEGAL)
	{
		GetAllInstallAndUpgradeApps(upgradeLegalApps, true, false, &installFilePaths);
	}
	else
	{
		GetAllInstallAndUpgradeApps(upgradePiracyApps, false, true, &installFilePaths);
		GetAllInstallAndUpgradeApps(upgradeLegalApps, true, true, &installFilePaths);
	}
	if(installFilePaths.size()>0)
	{
		InstallApp(installFilePaths);
	}
	if(upgradePiracyApps->size()>0)
	{
		UpgradePiracyAppDatas(upgradePiracyApps);
	}
	if(upgradeLegalApps->size()>0)
	{
		UpgradeLegalAppDatas(upgradeLegalApps);
	}
}

void AppModel::GetAllInstallAndUpgradeApps( vector<AppDataUpgrade*>* upgradeApps, bool islegal, bool mustsamelegal, vector<wstring>* installFilePaths )
{
	vector<AppDataUpgrade*>* devupgradeApps = islegal? &_LegalUpgradeApps : &_PiracyUpgradeApps;
	int islegali = islegal? 1:0;

	_INSTALLS_CASE_DATA_TYPE casetype = GetCurDeviceType() == Ios? _INSTALLS_CASE_DATA_IOS:_INSTALLS_CASE_DATA_ANDROID;

	for (size_t i = 0; i < devupgradeApps->size(); i++)
	{
		AppDataUpgrade* pApp = devupgradeApps->at(i);
		if(pApp->IsUpgrading())
			continue;
		if(mustsamelegal && pApp->_isLegalSoft!=islegali)
			continue;
		bool haslocalapp = false;
		wstring newversion = islegal?pApp->LegalNewVersion() : pApp->PiracyNewVersion();

		GetCaseDataMsg CaseMsg;
		CaseMsg.nCaseType = casetype;
		CaseMsg._packageName = pApp->_packageName;
		CaseMsg._isLegalSoft = islegali;
		CaseMsg._strVersion  = newversion;
		CaseMsg.SendAndGetReturn();
		vector<wstring> vec2FilePaths = CaseMsg.installFilePaths;
		if (vec2FilePaths.size()>0)
		{
			installFilePaths->push_back(vec2FilePaths[0]);
			haslocalapp = true;
		}

		if(!haslocalapp)
		{
			upgradeApps->push_back(pApp);
		}
		SetUpgrading(pApp, true);
	}
}

void AppModel::UpgradeSingleAppDataInternal( AppDataUpgrade* appdata, bool islegal )
{
	if(appdata && appdata->IsUpgrading())
		return;
	_INSTALLS_CASE_DATA_TYPE casetype = GetCurDeviceType() == Ios? _INSTALLS_CASE_DATA_IOS:_INSTALLS_CASE_DATA_ANDROID;

	int islegali = islegal? 1:0;
	wstring newversion = islegal?appdata->LegalNewVersion() : appdata->PiracyNewVersion();
	SetUpgrading(appdata, true);


	GetCaseDataMsg CaseMsg;
	CaseMsg.nCaseType = casetype;
	CaseMsg._packageName = appdata->_packageName;
	CaseMsg._isLegalSoft = islegali;
	CaseMsg._strVersion  = newversion;
	CaseMsg.SendAndGetReturn();
	vector<wstring> vec2FilePaths = CaseMsg.installFilePaths;

	this->InstallApp(vec2FilePaths);
	for (vector<wstring>::iterator iter = vec2FilePaths.begin(); iter!=vec2FilePaths.end(); ++iter)
	{
		vector<wstring> installFilePaths;
		installFilePaths.push_back(*iter);
		InstallApp(installFilePaths);
		return;
	}
	
	vector<AppDataUpgrade*>* upgradeApps = new vector<AppDataUpgrade*>();
	upgradeApps->push_back(appdata);
	if(!islegal)
		UpgradePiracyAppDatas(upgradeApps);
	else
		UpgradeLegalAppDatas(upgradeApps);
}

void AppModel::TaskRemove( int taskid, wstring deviceId )
{
	map<int, AppDataUpgrade*>* taskmap =  deviceId.empty()?&_UpgradeTasks:&_InstallTasks;
	map<int, AppDataUpgrade*>::iterator it = taskmap->find(taskid);
	if(it==taskmap->end())
		return;
	AppDataUpgrade* app = (*taskmap)[taskid];
	if (_bExit)
	{
		return;
	}
	_pUI->SetUpgradeBtnEnabled(app, true);
	if (_bExit)
	{
		return;
	}
	_pUI->SetAllUpgradeBtn(true);
	SetUpgrading(app, false);
	taskmap->erase(taskid);
}

void AppModel::TaskFinished( CTaskItem* item, wstring deviceId )
{
	map<int, AppDataUpgrade*>* taskmap=NULL;
	if(item->m_uTaskType == URL_DOWNLAOD)
	{
		taskmap=&_UpgradeTasks;
	}
	else if(item->m_uTaskType == INSTALL)
	{
		taskmap=&_InstallTasks;
	}
	if(taskmap!=NULL && taskmap->size()>0)
	{
		map<int, AppDataUpgrade*>::iterator it0 = taskmap->find(item->m_uID);
		if(it0==taskmap->end())
			return;
		AppDataUpgrade* app = (*taskmap)[item->m_uID];
		taskmap->erase(item->m_uID);
		if(item->m_uTaskType == URL_DOWNLAOD)
		{
			vector<wstring> installFilePaths;
			installFilePaths.push_back(item->m_TaskFileInfo->savePath);
			InstallApp(installFilePaths);
		}
		else if(item->m_uTaskType == INSTALL)
		{
			for(vector<AppDataUpgrade*>::iterator it = _PiracyUpgradeApps.begin(); it!=_PiracyUpgradeApps.end();it++)
			{
				AppDataUpgrade* appit = *it;
				if(appit->_packageName==app->_packageName)
				{
					_PiracyUpgradeApps.erase(it);
					break;
				}
			}
			for(vector<AppDataUpgrade*>::iterator it = _LegalUpgradeApps.begin(); it!=_LegalUpgradeApps.end();it++)
			{
				AppDataUpgrade* appit = *it;
				if(appit->_packageName==app->_packageName)
				{
					_LegalUpgradeApps.erase(it);
					break;
				}
			}
			DeviceData* pDevice = BaseOperation::FindDevice(deviceId);
			if(_bExit)
			{
				return;
			}
			_pUI->SetUpgradeBtn(&_PiracyUpgradeApps, pDevice);
			if(_bExit)
			{
				return;
			}
			_pUI->SetLegalUpgradeBtn(&_LegalUpgradeApps, pDevice);
		}
	}
}

void AppModel::SetUpgrading(AppDataUpgrade* appdata, bool isupgrading)
{
	for(vector<AppDataUpgrade*>::iterator it = _PiracyUpgradeApps.begin(); it!=_PiracyUpgradeApps.end();it++)
	{
		AppDataUpgrade* appit = *it;
		if(appit->_packageName==appdata->_packageName)
		{
			appit->IsUpgrading(isupgrading);
			break;
		}
	}
	for(vector<AppDataUpgrade*>::iterator it = _LegalUpgradeApps.begin(); it!=_LegalUpgradeApps.end();it++)
	{
		AppDataUpgrade* appit = *it;
		if(appit->_packageName==appdata->_packageName)
		{
			appit->IsUpgrading(isupgrading);
			break;
		}
	}
}

//@zjf --解决debug下，ios手机经常插上去会崩溃问题
//原因：由于其他模块用到传进去的pDatas变量，原本存放这个数据的容器里面地址已经被改变，导致pDatas失效
//解决办法：建立临时变量，由自己主动调用者释放指针避免内存泄露
void AppModel::BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData)
{
	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;
		AppData* pTemp = new AppData(pApp);
		if (pTemp == NULL) continue;
		vecTempData.push_back(pTemp);
	}

}

void AppModel::ClearTempData(vector<AppData*>& vecTempData)
{
	for (size_t i = 0; i < vecTempData.size(); i++)
	{
		SAFE_DELETE(vecTempData[i]);
	}
	vecTempData.clear();
}

void AppModel::LoadAppIcon( vector<AppData*>* pTemp )
{
	vector<AppData*>* vecTempData = pTemp;
	GET_DEVICE(_curDeviceID,);
	if(_bExit)
	{
		return;
	}
	_pUI->SetAppLogo(vecTempData,pDevice);
	this->ClearTempData(*vecTempData);
}

void AppModel::LoadAppIconInThread( vector<AppData*>* pDatas)
{
	vector<AppData*>* vecTempData = new vector<AppData*>();
	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;
		wstring strLogoPath = GetIconDir()+L"icon_"+pApp->_packageName+L".png";
		//为了防止手机端升级成为黄金版的本地缓存还是旧版本的，导致Icon不一致
		if (pApp->_packageName == L"com.dragon.android.pandaspace" || pApp->_packageName == L"NetDragon.Mobile.iPhone.91Space" ||pApp->_packageName==L"NetDragon.Mobile.iPad.91Space" || pApp->_packageName == L"NetDragon.Mobile.iPhone.PandaSpace")
		{
			if (CFileOperation::IsFileExist(strLogoPath.c_str()))
			{
				try
				{
					CFileOperation::DeleteFile(strLogoPath.c_str());
				}
				catch(...)
				{

				}
			}
		}
		if (CFileOperation::IsFileExist(strLogoPath.c_str()))
		{
			continue;
		}
		AppData* pTemp = new AppData(pApp);
		if (pTemp == NULL) continue;
		vecTempData->push_back(pTemp);
	}
	AppLoadIconThread* pAppLoadIconThread = new AppLoadIconThread(this);
	pAppLoadIconThread->SetVoidParam(vecTempData);

	pAppLoadIconThread->Run();
}

bool AppModel::Handle(const WebSoftInstallMsg* pMsg)
{
	if (NULL == pMsg || pMsg->_st_Hash.empty() || pMsg->_st_Sn.empty())
	{
		return true;
	}

	LocalCpaPathMsg CpaMsg;
	CpaMsg._vec_Hash = pMsg->_st_Hash;
	CpaMsg.SendAndGetReturn();
	vector<wstring> vec_files = CpaMsg._vec_Path;

	for (vector<wstring>::const_iterator iter = pMsg->_st_Sn.begin(); iter != pMsg->_st_Sn.end(); ++iter)
	{
		InstallAppDataThread* pInstallAppThread = new InstallAppDataThread(this);
		for (vector<wstring>::iterator it = vec_files.begin(); it != vec_files.end(); it++)
			pInstallAppThread->AddStrParam(*it);

		pInstallAppThread->SetStr(*iter);
		pInstallAppThread->Run();
	}

	return false;
}

bool AppModel::UnInstallApp(const vector<void*>* _st_pVoidList)
{
	if (NULL == _st_pVoidList)
	{
		return false;
	}

	SetCSLock(_lock);

	_UnInstallAppIng = true;

	vector<AppData*> vecTemp;
	for (vector<void*>::const_iterator it = _st_pVoidList->begin(); it != _st_pVoidList->end(); it++)
	{
		AppData* pData = new AppData((AppData*)(*it));
		if (NULL==pData)
			continue;
		vecTemp.push_back(pData);
	}

	for (vector<AppData*>::const_iterator iter = vecTemp.begin(); iter!=vecTemp.end(); ++iter)
	{
		UnInstallAppAction(*iter);
	}
	_UnInstallAppIng = false;

	//PostMessage(Singleton::GetMainWnd(), WM_DEVICE_SPACECHANGED, (WPARAM)BaseOperation::AllocateBuffer(GetCurrentDeviceID()), 0);
	
	RequestDeviceSpaceMsg spaceMsg;
	spaceMsg.deviceID = GetCurrentDeviceID();
	spaceMsg.Send();

	return true;
}

std::wstring AppModel::GetCurrentDeviceID()
{INTERFUN;
	return _curDeviceID;
}

void AppModel::SetCurrentDeviceID( wstring id )
{INTERFUN;
	_curDeviceID = id;
}

DeviceData* AppModel::FindCurDevice()
{INTERFUN;
	return DEVICE_MANAGER->FindDevice(_curDeviceID);
}

bool AppModel::Handle(const NotifyTaskItemMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	if(WM_TASK_FINISHED == pMsg->message)
	{
		CTaskItem* item = (CTaskItem*)pMsg->pTask;
		if(item)
		{
			this->TaskFinished(item, pMsg->curID);
		}
	}
	else if (WM_TASK_REMOVED == pMsg->message)
	{
		this->TaskRemove(pMsg->taskid, pMsg->curID);
	}

	return true;
}

App_CC_END