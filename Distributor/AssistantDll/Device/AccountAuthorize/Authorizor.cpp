#include "stdafx.h"
#include "Authorizor.h"
#include <dbghelp.h>
#include <excpt.h>

#include "Core/DeviceData.h"
#include "Device/IosPublicFunc.h"
#include "Common/FileOperation.h"
#include "SCInfo.h"
#include "SCInfoDB.h"
#include "Device/AFCFileSysemBase.h"
#include "Common/Path.h"
#include "Common/NDBase64.h"
#include "Module/Common/IosPList/IosPlistHelper.h"
#include "Device/ITunesService.h"
#include "Device/CFConverter.h"
#include "Common/NetOperation.h"
#include "Module/Common/IosPList/IosPlistConverter.h"
#include "Module/Application/IosApp/Authorize/iTunesAuthorize.h"

wstring Authorizor::_CacheFolder            = L"91 Harbor\\";
//wstring Authorizor::_RNG                    = L"C36D423A-A21F-43F5-9C3B-DDB6A50A5661";
bool                                Authorizor::_isSimpleiTunes = false;
FileVersionInfo*    Authorizor::_pITunesVersionInfo = NULL;

#define HARBORPLIST L"_Fairplay_Harbor.plist"
#define  iTunesReg  L"SOFTWARE\\Apple Computer, Inc.\\iPod\\RegisteredApps\\4"

Authorizor::Authorizor(const DeviceData* pDevice)
{
    _pDevice                = pDevice;
    _ConfigFolderOnDevice   = (_pDevice->GetDevInfo()->_ios_bJailBreaked ? 
                                L"/private/var/mobile/Media/iTunes_Control/iTunes/" 
                                : L"/iTunes_Control/iTunes/");
    _infoFileOnDevice       = _ConfigFolderOnDevice + HARBORPLIST;
    //_md5RNG                 = L"39251E72-38ED-49CF-8ABE-814A2F4302E0";
    _defaultPath            = L"http://itunesapp2.sj.91.com/xmlController.ashx";
    _pathMap                = L"ijk123efghpq45lmyz678nowx90abcdrstuv";
    _helperName             = L"helper_16";
    _isInitialized          = false;
    _externalAuthorize  = new iTunesAuthorize();

    GetItunesVersionInfo();
}

Authorizor::~Authorizor()
{
    SAFE_DELETE(_pITunesVersionInfo);
    for (size_t i = 0; i < _authorizations.size(); i++)
        SAFE_DELETE(_authorizations.at(i));
    SAFE_DELETE(_externalAuthorize);
}

int filter(unsigned int code, struct _EXCEPTION_POINTERS *p)
{	
	return EXCEPTION_EXECUTE_HANDLER;
}

bool Authorizor::Init()
{
	__try
	{
		InitInternal();
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		_isInitialized = false;
	}
	return _isInitialized;
}

void Authorizor::InitInternal( ) 
{
	try
	{   
		RETURN_IF(_isInitialized);
		RETURN_IF(0 != IsValideToAuthorize(_pDevice));

		// AUTHORIZATIONDB->GetAuthorizedAccounts(_pDevice->GetDevInfo()->_ios_deviceIdentifier, _authorizedAccounts);
		AUTHORIZATIONDB->GetAllAuthorizations(_authorizations);

		AFCFileSysemBase pFileStream(_pDevice->GetDevInfo());
		if (pFileStream.Ios_Exists(_infoFileOnDevice))
		{
			// 设备上有缓存授权信息时,要和数据库比对下,以免设备接入的PC不同,导致PC没有缓存信息
			wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
			wstring plistPath = tempDir+HARBORPLIST;
			RETURN_IF(pFileStream.Ios_DownFromPhone(_infoFileOnDevice, plistPath) != Transfer_Success);
			IosPlistConverter::GetXmlFromPlist(plistPath, plistPath);
			string text = CFileOperation::ReadFile(plistPath, true);

			char* temptemp = (char*)text.c_str();
			void* pXmlData = CCFConverter::NDCFDataCreateWithBytesNoCopy(NULL, (unsigned char*)temptemp, (int)text.length(), CCFConverter::NDkCFAllocatorNull()); 
			void* pReplay = CCFConverter::NDCFPropertyListCreateFromXMLData(NULL, pXmlData, kCFPropertyListImmutable, NULL);

			vector<string> accounts;
			CCFConverter::GetIosData(pReplay, accounts);
			for (size_t i = 0; i < accounts.size(); i++)
			{
				wstring decryptAccount = AUTHORIZATIONDB->DecryptData(CCodeOperation::UTF_8ToUnicode(accounts.at(i)));
				if (find(_authorizedAccounts.begin(), _authorizedAccounts.end(), decryptAccount) != _authorizedAccounts.end())
					continue;

				_authorizedAccounts.push_back(decryptAccount);
				//AUTHORIZATIONDB->InsertAuthorizedAccount(decryptAccount, _pDevice->GetDevInfo()->_ios_deviceIdentifier);
			}

			CFileOperation::DeleteDirectory(tempDir);
		}
		else
		{
			if (!pFileStream.Ios_Exists(_ConfigFolderOnDevice))
				pFileStream.Ios_CreateDirectory(_ConfigFolderOnDevice);

			// 设备被刷机过了?清空原先数据库记录
			_authorizedAccounts.clear();
			// AUTHORIZATIONDB->DeleteAuthorizedAccounts(_pDevice->GetDevInfo()->_ios_deviceIdentifier);
		}

		_isInitialized = true;
	}
	catch (...)
	{
		LOG_DEBUG(L"Init AuthorizitionDB Error");
	}
}

int Authorizor::IsValideToAuthorize(const DeviceData* pDevice)
{
    RETURN_VALUE_IF(_isSimpleiTunes, -5);

    if (IosPublicFunc::GetVerNum(pDevice->GetDevInfo()->_deviceProductVersion) < 400)
        return -1;
    else if (pDevice->GetDevInfo()->_connectType != USB)
        return -2;
    else
    {
        GetItunesVersionInfo();
        RETURN_VALUE_IF(_pITunesVersionInfo == NULL, -4);
        wstring productVersion = _pITunesVersionInfo->_productVersion;
			if(IosPublicFunc::GetVerNum(productVersion)<105)
//         if (productVersion.find(L"10.5") == wstring::npos
//             && productVersion.find(L"10.6") == wstring::npos
//             && productVersion.find(L"10.7") == wstring::npos
//             && productVersion.find(L"11.0") == wstring::npos)
            return -3;
        //return iTunesAuthorize::IsAssistance();
    }

	return 0;
}

void Authorizor::GetItunesVersionInfo()
{
    RETURN_IF(_pITunesVersionInfo != NULL);
    wstring path = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, iTunesReg, _T("path"));
    if (!CFileOperation::IsFileExist(path.c_str()))
        _isSimpleiTunes = true;
    else
        _pITunesVersionInfo = MyFileOperation::GetExeFileInfo(path);
}

bool Authorizor::IsAuthorizationInCache(Authorization* au, Authorization* localCache)
{
    LOG_DEBUG(L"bool Authorizor::IsAuthorizationInCache(Authorization* au, Authorization* localCache)");
    for (size_t i = 0; i < _authorizations.size(); i++)
    {
        Authorization* cache = _authorizations.at(i);
        if (cache && cache->GetAccountID() == au->GetAccountID() 
            && cache->GetIsLocalCached() 
            && cache->GetITunesVersion() == au->GetITunesVersion() 
            && !cache->GetServerParams().empty())
        {
            localCache = cache;
            return true;
        }
        else if (cache && !cache->GetIDS().empty())
        {
            // 多账号模式下,判断下已经缓存的授权文件有没有当前要授权的账户.
            if (!au->GetAccountID().empty())
            {
                if (cache->GetIDS().find(au->GetAccountID()) != string::npos)
                {
                    localCache = cache;
                    return localCache->GetIsLocalCached();
                }
            }
            else
            {
                // 多账号模式下,如果当前要授权的也是多账号,则比对缓存的授权文件是否和当前要授权的账户一一对应
                // 如果没有,需要下载新的.
                vector<wstring> inputIds = CStrOperation::parseStringsW(au->GetIDS(), L",");
                RETURN_FALSE_IF(inputIds.empty());

                bool allIdIsSame = true ;
                for (size_t j = 0; j < inputIds.size(); j++)
                {
                    wstring id = inputIds.at(j);
                    if (cache->GetIDS().find(id) != string::npos)
                        continue;

                    allIdIsSame = false;
                    break;
                }

                if (allIdIsSame)
                {
                    localCache = cache;
                    return localCache->GetIsLocalCached();
                }
            }
        }
    }

    return false;
}

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

void PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = {0};

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE, processID);

    // Get the process name.
    if (NULL != hProcess)
    {
        HMODULE hMod = NULL;
        DWORD cbNeeded = 0;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
    }

    // Print the process name and identifier.
    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

    CloseHandle( hProcess );
}

void Authorizor::KillProcessHelper_16()
{
    // Get the list of process identifiers.
    DWORD aProcesses[1024]  = {0};
    DWORD cbNeeded          = 0;
    DWORD cProcesses        = 0;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return ;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // get the name and process identifier for each process.
    for (DWORD i = 0; i < cProcesses; i++)
    {
        if(aProcesses[i] == 0)
            continue;

        DWORD processID = aProcesses[i];

        TCHAR szProcessName[MAX_PATH] = {0};

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE, processID);

        // Get the process name.
        if (NULL != hProcess)
        {
            HMODULE hMod = NULL;
            DWORD cbNeeded = 0;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
        }

        // Print the process name and identifier.
        _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );
        string strProcessName = (const char*)szProcessName;
        if (strProcessName == CCodeOperation::WstringToString(_helperName))
            TerminateProcess(hProcess, 0);

        CloseHandle(hProcess);
    }
}

int Authorizor::CreateProcessHelper_16(Authorization* au)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si,0,sizeof(si)); 
    memset(&pi,0,sizeof(pi)); 
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW; 
    si.wShowWindow = SW_HIDE; 
    SECURITY_ATTRIBUTES psa={sizeof(psa),NULL,TRUE}; 
    psa.bInheritHandle=TRUE; 
    HANDLE houtfile = NULL;
    //houtfile=CreateFile(m_pResultFile,GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE, &psa,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);	
    //if (houtfile==NULL)
    //	return ;
    si.hStdError = houtfile;
    si.hStdOutput  = houtfile;
    
    wstring strExe = _helperName;// = Path.Combine(Environment.CurrentDirectory, helperName);
    wstring strParam = _pDevice->GetDevInfo()->_ios_deviceIdentifier
                        +AUTHORIZATIONDB->EncryptData(au->GetFolder())+L" "
                        +AUTHORIZATIONDB->EncryptData(CStrOperation::ReplaceW(au->GetServerParams(), L"\r\n", L","))+L" "
                        +AUTHORIZATIONDB->EncryptData(au->GetIDS());
    wchar_t pBuffer[256] = {0};
    memcpy(pBuffer, strParam.c_str(), strParam.length() > 255 ? 255 : strParam.length());
    if (CreateProcess(strExe.c_str(), pBuffer, NULL, NULL,TRUE,0,NULL,NULL,&si,&pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        if (houtfile != NULL)
            CloseHandle(houtfile);
    }

    return GetLastError();
}

int Authorizor::AuthorizeFromExternal(Authorization* au)
{
    try
    {
        KillProcessHelper_16();
        return CreateProcessHelper_16(au);
    }
    catch (...)
    {
    	LOG_DEBUG(L"AuthorizeFromExternal error");
    }
    return AuthorizeException;
}

void Authorizor::SerilizeCacheAccountToDevice()
{
    try
    {
        RETURN_IF(_authorizedAccounts.empty());

        vector<string> ids;
        for (size_t i = 0; i < _authorizedAccounts.size(); i++)
        {
            wstring id = AUTHORIZATIONDB->EncryptData(_authorizedAccounts.at(i));
            ids.push_back(CCodeOperation::UnicodeToUTF_8(id));
        }

        void* pMsgDict = CCFConverter::ConvertStringVector(ids);
        //转换CFDic To CFPropertyList
        void* pMsgAsXML = CCFConverter::NDCFPropertyListCreateData(NULL, pMsgDict, kCFPropertyListBinaryFormat_v1_0, 0, NULL);
        int nXmlLen = CCFConverter::NDCFDataGetLength(pMsgAsXML);
        char* temp = (char*)CCFConverter::NDCFDataGetBytePtr(pMsgAsXML);

        wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
        wstring plistPath = tempDir+HARBORPLIST;
        CFileOperation::WriteBinFile(plistPath, temp, nXmlLen);

        AFCFileSysemBase pFileStream(_pDevice->GetDevInfo());
        pFileStream.Ios_CopyToPhone(plistPath, _infoFileOnDevice);
        CFileOperation::DeleteDirectory(tempDir);
    }
    catch (...)
    {
        LOG_DEBUG(L"Serialize AccountID  to device Error");
    }
}

void Authorizor::CacheAccount(Authorization* au)
{
    LOG_DEBUG(L"void Authorizor::CacheAccount(Authorization* au)");
    if (au->GetIDS().empty() 
        && !au->GetAccountID().empty() 
        && find(_authorizedAccounts.begin(),_authorizedAccounts.end(),au->GetAccountID()) == _authorizedAccounts.end())
    {
        LOG_DEBUG(L"AUTHORIZATIONDB->InsertAuthorizedAccount(au->GetAccountID(), _pDevice->GetDevInfo()->_ios_deviceIdentifier)");
      //  bool result = AUTHORIZATIONDB->InsertAuthorizedAccount(au->GetAccountID(), _pDevice->GetDevInfo()->_ios_deviceIdentifier);
        _authorizedAccounts.push_back(au->GetAccountID());
        SerilizeCacheAccountToDevice();
    }
    else
    {
        vector<wstring> accounts = CStrOperation::parseStringsW(au->GetIDS(), L",");
        RETURN_IF(accounts.empty());

        for (size_t i = 0; i < accounts.size(); i++)
        {
            wstring account = accounts.at(i);
            if (find(_authorizedAccounts.begin(),_authorizedAccounts.end(),account) != _authorizedAccounts.end())
                continue;

     //       bool result = AUTHORIZATIONDB->InsertAuthorizedAccount(account, _pDevice->GetDevInfo()->_ios_deviceIdentifier);
            _authorizedAccounts.push_back(account);
        }
        SerilizeCacheAccountToDevice();
    }
}

wstring Authorizor::GetMD5(wstring Data)
{
    Data += L"39251E72-38ED-49CF-8ABE-814A2F4302E0";//_md5RNG;
    string data = CStrOperation::GetMD5(CCodeOperation::UnicodeToUTF_8(Data));
    return CCodeOperation::UTF_8ToUnicode(data);
}

bool Authorizor::SortByCreateDate(Authorization *v1, Authorization *v2)
{
    return v1->GetCreateDate() > v2->GetCreateDate();
}

wstring Authorizor::GetNewestUrl()
{
    RETURN_VALUE_IF(_authorizations.empty(), _defaultPath);
    sort(_authorizations.begin(), _authorizations.end(), &Authorizor::SortByCreateDate);

    Authorization* au = DecryptAuthorization(_authorizations.at(0)->GetData());
    wstring url = (au == NULL ? _defaultPath : au->GetUrl());
    SAFE_DELETE(au);
	LOG_DEBUG(L"删除授权信息 au");

    return url;
}

wstring Authorizor::MapPath(wstring url)
{
    wstring map;
    try
    {
        for (size_t i = 0; i < url.length(); i++)
        {
            wchar_t temp[128] = {0};
            wsprintf(temp, L"%c", url.at(i));
            wsprintf(temp, L"%c", _pathMap.at(_wtoi(temp)));
            map += temp;
        }
    }
    catch (...)
    {
        return url;
    }

    return map;
}

bool Authorizor::GetAuthorizationFromServer(Authorization* authorization)
{
    LOG_DEBUG(L"bool Authorizor::GetAuthorizationFromServer(Authorization* authorization)");
    try
    {
        wstring strkey = GetMD5(authorization->GetAccountID()+authorization->GetITunesVersion());

        if (authorization->GetUrl().empty())
            authorization->SetUrl(GetNewestUrl());
        wstring url = authorization->GetUrl()+L"?action=getscinfo";
        wstring postData = L"appuid="+authorization->GetAccountID()
                           +L"&"+L"itunesver="+authorization->GetITunesVersion()
                           +L"&"+L"key="+strkey;
        LOG_DEBUG(L"CNetOperation::Post start url:"+url+L" postData:"+postData);
        string returnData = CNetOperation::Post(CCodeOperation::UnicodeToGB2312(url), CCodeOperation::UnicodeToUTF_8(postData));
        LOG_DEBUG(L"CNetOperation::Post end returnData:"+CCodeOperation::UTF_8ToUnicode(returnData));
        size_t start = returnData.find("href=")+6;
        size_t end = returnData.find("here")-2;
        string newUrl = returnData.substr(start, end-start);

        wstring saveFolder = AuthorizationDB::_DBFolder;
        wstring savePath = saveFolder+L"temp.zip";
        CFileOperation::MakeLocalDir(saveFolder);
        LOG_DEBUG(L"CNetOperation::DownloadFile start newUrl:"+CCodeOperation::UTF_8ToUnicode(newUrl)+L" savePath:"+savePath);
        RETURN_FALSE_IF(!CNetOperation::DownloadFile(newUrl, savePath));
        LOG_DEBUG(L"CNetOperation::DownloadFile end");

        wstring ulistFileName = L"ulist.ini";
        CFileOperation::UnZipFile(savePath, saveFolder, ulistFileName, false);
        string data = CFileOperation::ReadFile(saveFolder+ulistFileName, true);
        if (data.find(",") != string::npos)
            authorization->SetIDS(CCodeOperation::UTF_8ToUnicode(data));
        CFileOperation::DeleteFile(saveFolder+ulistFileName);

        // 多账号模式下,判断下当前请求的账号,是否在服务器列表里面,如果没有说明该账号不属于ND.
        if (!authorization->GetAccountID().empty() 
            && !authorization->GetIDS().empty() 
            && authorization->GetIDS().find(authorization->GetAccountID()) == wstring::npos)
            return false;

        if (authorization->GetIDS().empty())
            saveFolder += MapPath(authorization->GetAccountID())
                            +MapPath(CStrOperation::ReplaceW(authorization->GetITunesVersion(), L".", L""))
                                +L"\\";
        else
            saveFolder += MapPath(L"912026")
                            +MapPath(CStrOperation::ReplaceW(authorization->GetITunesVersion(), L".", L""))
                                +L"\\";

        if (CFileOperation::IsDirExist(saveFolder.c_str()))
            CFileOperation::DeleteDirectory(saveFolder);
        else
            CFileOperation::MakeLocalDir(saveFolder);

        wstring sidbFileName = L"SC Info.sidb";
        wstring siddFileName = L"SC Info.sidd";
        wstring txtFileName  = L"SC Info.txt";
        CFileOperation::UnZipFile(savePath, saveFolder, sidbFileName, false);
        CFileOperation::UnZipFile(savePath, saveFolder, siddFileName, false);
        CFileOperation::UnZipFile(savePath, saveFolder, txtFileName, false);

        wstring serverFileName = L"server.serInfo";
        CFileOperation::UnZipFile(savePath, saveFolder, serverFileName, false);
        data = CFileOperation::ReadFile(saveFolder+serverFileName, true);
        if (!data.empty())
            authorization->SetServerParams(CCodeOperation::UTF_8ToUnicode(data));
        CFileOperation::DeleteFile(saveFolder+serverFileName);

        authorization->SetFolder(saveFolder);
        CFileOperation::DeleteFile(savePath);

        return !authorization->GetServerParams().empty();
    }
    catch (...)
    {
        LOG_DEBUG(L"GetAuthorizationFromServer error");
    }
    
    return false;
}

void Authorizor::CacheAuthorization(Authorization* au)
{
    bool isCache = false;
    for (size_t i = 0; i < _authorizations.size(); i++)
    {
        Authorization* pTemp  = _authorizations.at(i);
        if (pTemp && au && pTemp->GetITunesVersion() != au->GetITunesVersion()
            || pTemp->GetAccountID() != au->GetAccountID())
            continue;

        isCache = true;
        break;
    }
    
    RETURN_IF(isCache);
    AUTHORIZATIONDB->InstertAuthorization(au);
	if (au)
	    _authorizations.push_back(au);
}

// 获取当前要授权的id数组,一个也传数组给底层.
void GetIDArray(Authorization* au, ND_UInt64* accounts, int& count)
{
    vector<wstring> ids = CStrOperation::parseStringsW(au->GetIDS(), L",");
    for (size_t i = 0; i < ids.size(); i++)
        accounts[i] = _wtol(ids.at(i).c_str());
    count = (int)ids.size();

    RETURN_IF(!ids.empty());
    accounts[0] = _wtol(au->GetAccountID().c_str());
    count = 1;
}

int Authorizor::AuthorizeFromExternalITunesAuthorize(Authorization* au)
{
    LOG_DEBUG(L"int Authorizor::AuthorizeFromExternalITunesAuthorize(Authorization* au)");
    SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csAuthorizeLock);
    RETURN_VALUE_IF(au == NULL, -1);
    vector<ND_UInt32> serverNumbers;
    vector<wstring> serverParams = CStrOperation::parseStringsW(au->GetServerParams(), L"\r\n");
    for (size_t i = 0; i < serverParams.size(); i++) 
        serverNumbers.push_back(strtoul(CCodeOperation::UnicodeToUTF_8(serverParams.at(i)).c_str(), NULL, 16));

    ND_UInt64 ids[] = {0};
    int count = 0;
    GetIDArray(au, ids, count);

    //struct am_device* devptr = (struct am_device*)_pDevice->GetDevInfo()->_ios_devptr;
    //string deviceIdentifier = CCodeOperation::UnicodeToUTF_8(_pDevice->GetDevInfo()->_ios_deviceIdentifier);
    //ND_UInt32 iOSVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
    //ND_UInt32 afcConn = (ND_UInt32)_pDevice->GetDevInfo()->_ios_afcHandle;

    LOG_DEBUG(L"_externalAuthorize->Authorize");
    return  _externalAuthorize->Authorize(/*devptr,*/ true, CCodeOperation::UnicodeToUTF_8(au->GetFolder()), 
        serverNumbers, ids, count, /*(char*)deviceIdentifier.c_str(), iOSVersion, afcConn*/_pDevice->GetDevInfo());
}

bool Authorizor::Authorize(Authorization* au)
{
    LOG_DEBUG(L"bool Authorizor::Authorize(Authorization* au)");
    //SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csAuthorizeLock);
    RETURN_FALSE_IF(!_pDevice->GetDevInfo()->_ios_bAFCConn);

    // 判断下本地是否有缓存过该版本授权信息,有的话用缓存里面的数据,而不是从服务器请求
    Authorization* localCache = NULL;
	IsAuthorizationInCache(au, localCache);
    if (localCache)
    {
        //int ret = AuthorizeFromExternal(localCache);
        LOG_DEBUG(L"localCacheAu");
        int ret = AuthorizeFromExternalITunesAuthorize(localCache);
        if (ret == 0)
            CacheAccount(au);
        else
            localCache->SetFolder(L"");//删除本地缓存

        return (ret == 0);
    }
    else if (GetAuthorizationFromServer(au))
    {
        //int ret = AuthorizeFromExternal(au);
        LOG_DEBUG(L"ServerAu");
        int ret = AuthorizeFromExternalITunesAuthorize(au);
        RETURN_FALSE_IF(ret != 0);

        CacheAuthorization(au);
        CacheAccount(au);
        return true;
    }

    return false;
}

bool Authorizor::Authorize(wstring data, wstring ipaName)
{
	Init();
	_ipaName = ipaName;
	RETURN_FALSE_IF(0 != IsValideToAuthorize(_pDevice));

	Authorization* au = DecryptAuthorization(data);
	RETURN_FALSE_IF(au == NULL);
	_softID = au->GetSoftID();

	bool res = false;
	try
	{
		if (find(_authorizedAccounts.begin(),_authorizedAccounts.end(),au->GetAccountID()) != _authorizedAccounts.end())
			res = true;
		else
			res = Authorize(au);
	}
	catch (...)
	{

	}
	return res;
}

void Authorizor::GetAppDSIDS(vector<wstring>& dsids)
{
    map<string, void*> srcDict;
    srcDict.insert(pair<string, void*>("ApplicationType", CCFConverter::StringtoCFString("User")));
    vector<string> returnAttributes;
    returnAttributes.push_back("CFBundleIdentifier");
    returnAttributes.push_back("ApplicationDSID");
    srcDict.insert(pair<string, void*>("ReturnAttributes", CCFConverter::ConvertStringVector(returnAttributes)));

    map< string, map<string,void*> > mapEntities;

    void* pDict = CCFConverter::DictionarytoCFDictionary(srcDict);
    void* pDevPtr = _pDevice->GetDevInfo()->_ios_devptr;
	SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);

    RETURN_IF(0 != CITunesMobileDevice::iTunes_AMDeviceConnect(pDevPtr));
    if (0 == CITunesMobileDevice::iTunes_AMDeviceStartSession(pDevPtr))
    {
        void* pApps = NULL;
        if (0 == CITunesMobileDevice::iTunes_AMDeviceLookupApplications(pDevPtr, pDict, &pApps))
            CCFConverter::GetIosEntitiesData(pApps, mapEntities, pDevPtr);

        CITunesMobileDevice::iTunes_AMDeviceStopSession(pDevPtr);
    }
    CITunesMobileDevice::iTunes_AMDeviceDisconnect(pDevPtr);

    map< string, map<string,void*> >::iterator it;
    for (it = mapEntities.begin(); it != mapEntities.end(); it++)
    {
        map<string, void*> appInfo = it->second;
        map<string, void*>::iterator iter;
        for (iter = appInfo.begin(); iter != appInfo.end(); iter++)
        {
            if (iter->first != "ApplicationDSID")
                continue;

            int intTemp = 0;
            CCFConverter::GetIosData(iter->second, intTemp);
            wchar_t temp[128] = {0};
            wsprintf(temp, L"%d", intTemp);
            dsids.push_back(temp);
            break;
        }
    }
}

int Authorizor::FixFlashExit()
{
    int res = IsValideToAuthorize(_pDevice);
    RETURN_VALUE_IF(0 != res, res);

    Init();

    vector<wstring> dsids;
    GetAppDSIDS(dsids);

    for (size_t i = 0; i < dsids.size(); i++)
    {
        wstring id = dsids.at(i);
        if (!_pDevice->GetDevInfo()->_ios_bAFCConn)
            break;

        //闪退修复的时候,暂时不判断缓存是否存在,临时解决用户和itunes同步后授权信息被删问题
//         if (find(_authorizedAccounts.begin(),_authorizedAccounts.end(), id) != _authorizedAccounts.end())
//             continue;

        Authorization* au = new Authorization();
        au->SetAccountID(id);
        au->SetITunesVersion(GetiTunesVersion());

        if (!Authorize(au))
            LOG_DEBUG(L"Attempt to fix Error Account:"+id);

        Sleep(200);
    }

    return 0;
}

void Authorizor::AttemptFix()
{
    RETURN_IF(0 != IsValideToAuthorize(_pDevice))

    Init();

    vector<wstring> dsids;
    GetAppDSIDS(dsids);

    for (size_t i = 0; i < dsids.size(); i++)
    {
        wstring id = dsids.at(i);
        if (!_pDevice->GetDevInfo()->_ios_bAFCConn)
            break;

        if (find(_authorizedAccounts.begin(),_authorizedAccounts.end(), id) != _authorizedAccounts.end())
            continue;

        //上次尝试修复的ID有问题,则不再尝试.除非手动修复.
        if (find(_attemptFixErrorIDs.begin(),_attemptFixErrorIDs.end(), id) != _attemptFixErrorIDs.end())
            continue;

        Authorization* au = new Authorization();
        au->SetAccountID(id);
        au->SetITunesVersion(GetiTunesVersion());

        try
        {
            if (Authorize(au))
                continue;
        }
        catch (...)
        {
        }

        _attemptFixErrorIDs.push_back(id);
        LOG_DEBUG(L"Attempt to fix Error in attemptFix func Account:"+id);
    }
}

#include <WinCrypt.h>
#pragma comment( lib, "Advapi32.lib" )

// 服务器端DES加密对应解密
BOOL DES(__in const BYTE* pbKey,
         __in DWORD cbKeyLen,
         __in const BYTE* pbData,
         __in DWORD cbDataLen,
         __out BYTE* pbBuf,
         __inout DWORD* pcbBufLen,
         __in BOOL bIsDecrypt = FALSE  
         )
         /*
         DES加密、解密函数，使用PKCS 5 padding，CBC模式

         参数：
         pbKey       DES密钥
         cbKeyLen    pbKey字节长度
         pbData      要加密、解密的数据
         cbDataLen   pbData字节长度
         pbBuf       输出缓冲区，输出加密后、解密后的数据，可以为NULL
         pcbBufLen   pbBuf字节长度
         当pbBuf不为NULL时，返回实际复制到pbBuf的字节长度
         当pbBuf为NULL时，返回需要的pbBuf字节长度
         bIsDecrypt  为TRUE时执行解密操作，否则执行加密操作

         返回：
         成功返回TRUE，否则返回FALSE
         */
{
    struct
    {
        BLOBHEADER hdr;
        DWORD cbKeySize;
        BYTE rgbKeyData[8];
    } keyBlob;    //结构参考MSDN - CryptImportKey

    keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
    keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
    keyBlob.hdr.reserved = 0;
    keyBlob.hdr.aiKeyAlg = CALG_DES;
    keyBlob.cbKeySize = 8;
    ZeroMemory(keyBlob.rgbKeyData, 8);
    CopyMemory(keyBlob.rgbKeyData, pbKey, cbKeyLen > 8 ? 8 : cbKeyLen);

    HCRYPTPROV hProv;
    if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0))
        return FALSE;

    HCRYPTKEY hKey;
    if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey))
    {
        CryptReleaseContext(hProv, 0);
        return FALSE;
    }

    BYTE IV[9] = {0};
    memcpy( IV, pbKey, 8 );
    CryptSetKeyParam(hKey, KP_IV, (BYTE*)IV, 0);

    BOOL bRet;
    BYTE pbBlock[16];
    DWORD dwBlock, dwOut = 0;
    BOOL bEOF;
    for (DWORD i = 0; i < cbDataLen; i += 8)
    {
        bEOF = cbDataLen - i <= 8;    //是否为最后一组
        dwBlock = bEOF ? cbDataLen - i : 8;

        CopyMemory(pbBlock, pbData + i, dwBlock);    //分组加密、解密，每组8字节
        if (bIsDecrypt)
            bRet = CryptDecrypt(hKey, NULL, bEOF, 0, pbBlock, &dwBlock);
        else
            bRet = CryptEncrypt(hKey, NULL, bEOF, 0, pbBlock, &dwBlock, sizeof(pbBlock));

        if (!bRet)
            break;

        if (pbBuf)
        {
            if (dwOut + dwBlock > *pcbBufLen)
            {
                bRet = FALSE;
                break;
            } 
            else
            {
                CopyMemory(pbBuf + dwOut, pbBlock, dwBlock);
                dwOut += dwBlock;
            }
        } 
        else    //当pbBuf为NULL时，仅计算需要多大的pbBuf
            dwOut += dwBlock;
    }

    *pcbBufLen = dwOut;
    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);

    return bRet;
}

wstring Authorizor::GetiTunesVersion()
{
    wstring version;
    if (_isSimpleiTunes)
    {
        if (CITunesService::GetiTunesVersionType() == iTunes105)
            version = L"10.6.3.25";
        else
            version = L"10.2.1";
    }
    else
    {
        if (NULL != _pITunesVersionInfo)
            version = _pITunesVersionInfo->_fileVersion;
    }

    return version;
}

Authorization* Authorizor::DecryptAuthorization(wstring data)
{
    RETURN_NULL_IF(data.empty());

    string strKey = CCodeOperation::UnicodeToUTF_8(L"C36D423A-A21F-43F5-9C3B-DDB6A50A5661"/*_RNG*/);
    string strPlain = CCodeOperation::UnicodeToUTF_8(data);

    BYTE* pbKey = new BYTE[strKey.length()];
    memset(pbKey, 0, strKey.length());
    memcpy(pbKey, strKey.c_str(), strKey.length());

    BYTE* pbPlain = new BYTE[strPlain.length()];
    memset(pbPlain, 0, strPlain.length());
    memcpy(pbPlain, strPlain.c_str(), strPlain.length());

    int nSizeOut = CNDBase64::Base64decode_len(strPlain.c_str());
    BYTE* plaintText = new BYTE[nSizeOut];
    memset(plaintText, 0, nSizeOut);
    int nSizeRet = CNDBase64::Base64decode((char*)plaintText, strPlain.c_str());

    DWORD dwLen = nSizeOut;
    ZeroMemory(pbPlain, sizeof(pbPlain));
    DES(pbKey, strKey.length(), plaintText, dwLen, pbPlain, &dwLen, TRUE);  //解密
    delete [] plaintText;
    delete [] pbKey;

    strPlain = (const char*)pbPlain;
    strPlain = strPlain.substr(0, strPlain.rfind('>')+1);

    Authorization* au = NULL;
    xml_document<char> doc;
    doc.parse<0>(const_cast<char *>(strPlain.c_str()));
    xml_node<char>* nodeplist = doc.first_node("ConList");
    if(nodeplist)
    {
        au = new Authorization();
        xml_node<char>* node = nodeplist->first_node();
        while(node)
        {
            string value = node->value();
            string name = node->name();
            if(name == "uid")
                au->SetAccountID(CCodeOperation::UTF_8ToUnicode(value));
            else if (name == "softid")
                au->SetSoftID(CCodeOperation::UTF_8ToUnicode(value));
            else if (name == "scurl")
                au->SetUrl(CCodeOperation::UTF_8ToUnicode(value));

            node = node->next_sibling();
        }
        au->SetData(data);
        au->SetITunesVersion(GetiTunesVersion());
    }

    delete [] pbPlain;
    return au;
}

/*
Authorization* Authorizor::DecryptAuthorization(wstring data)
{
    data = L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><ConList><uid>1978344488</uid><softid>307227301</softid><scurl>http://itunesapp2.sj.91.com/xmlController.ashx</scurl></ConList>";
    //data = L"UO/OsgqSfJ+r/4yrzjh6aMlh5CzyQRp3PeRBU//+BlKh4MIpdjf7jkb/v43PJyW9XXYOL2JtFTSSq9EBSLlcHP2625pLchJ5NeGJI6zEMtVQScRBVTpbvCCY8CdDMuldxyo3XTEaQo96rKRYviTHVkeg2YrHUPzNJJqVspU3MvdUByRvDlL0dYEDI9lFeQDYZNlpLcj/5vV4DLq5ESN0MkKobM47ntRk";

    RETURN_NULL_IF(data.empty());
    
    BYTE pbKey[] = "C36D423A-A21F-43F5-9C3B-DDB6A50A5661";
    BYTE pbPlain[] = "<?xml version=\"1.0\" encoding=\"utf-8\" ?><ConList><uid>1978344488</uid><softid>307227301</softid><scurl>http://itunesapp2.sj.91.com/xmlController.ashx</scurl></ConList>";
    DWORD dwLen = 0;

    string tempKey = (const char*)pbKey;
    string tempPlain = (const char*)pbPlain;

    DES(pbKey, tempKey.length(), pbPlain, tempPlain.length(), NULL, &dwLen);  //计算需要多大的输出缓冲区

    BYTE* pbCipher = new BYTE[dwLen];
    DES(pbKey, tempKey.length(), pbPlain, tempPlain.length(), pbCipher, &dwLen);  //加密

    int nSizeOut = CNDBase64::Base64encode_len(dwLen);
    char* szBase64 = new char[nSizeOut];
    memset(szBase64, 0, nSizeOut);
    CNDBase64::Base64encode(szBase64, (const char*)pbCipher, dwLen);


    string cipher = szBase64;
    nSizeOut = CNDBase64::Base64decode_len(cipher.c_str());
    char* plaintText = new char[nSizeOut];
    memset(plaintText, 0, nSizeOut);
    int nSizeRet = CNDBase64::Base64decode(plaintText, cipher.c_str());

    ZeroMemory(pbPlain, sizeof(pbPlain));
    DES(pbKey, tempKey.length(), (const BYTE*)plaintText, dwLen, pbPlain, &dwLen, TRUE);  //解密
    delete [] plaintText;


    return NULL;
}*/