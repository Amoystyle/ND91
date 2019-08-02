#include "pch_module.h"

#ifdef MODULE_APP

#include "Module/System/IosScreenOperation.h"

#include "Core/DeviceManager.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"
#include "Device/ITunesMobileDevice.h"
#include "Common/Path.h"

IosScreenOperation::IosScreenOperation(void* pDevPtr, wstring strProductVersion, bool biPad)
{
	_pSpringBoardServiceHandle  = NULL;
	_pDevPtr                    = pDevPtr;
	_strProductVersion          = strProductVersion;
	_nVer                       = IosPublicFunc::GetVerNum(strProductVersion);
	_biPad                      = biPad;	
	_pIosIconSet                = NULL;
    InitializeCriticalSection(&_cs);
}

IosScreenOperation::~IosScreenOperation(void)
{
	FreeIconSet();
    DeleteCriticalSection(&_cs);
}

// 获取图标状态信息
IosIconSet* IosScreenOperation::GetIconState()
{
    //5.x开始增加了一个字段destinationState,还不知道做啥的.
    //<key>command</key>
    //<string>getIconState</string>
    //<key>destinationState</key>
    //<false/>
    //<key>formatVersion</key>
    //<string>2</string>

    EnterCriticalSection(&_cs);
    RETURN_VALUE_IF(_pIosIconSet, _pIosIconSet);

	_eInterfaceOrientationType = GetInterfaceOrientation();

	StartSpringBoardService();
    RETURN_VALUE_IF(!_pSpringBoardServiceHandle, NULL);

	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("command", "getIconState"));
	if (_nVer >= 400)
		mapMsg.insert(pair<string, string>("formatVersion", "2"));

	if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
	{
		_pIosIconSet = ReadMssageFromService(_pSpringBoardServiceHandle);
	}
    else
    {
        StartSpringBoardService();
        if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
            _pIosIconSet = ReadMssageFromService(_pSpringBoardServiceHandle);
    }

    //获取图标
    if (_pIosIconSet)
    {
        for (int i=0; i<_pIosIconSet->_nBottomBarMaxIconCount; i++)
            InitIconImageFile(_pIosIconSet->_BottomBar[i]);

        vector<IosScreen*>::iterator it;
        for (it = _pIosIconSet->_vtScreen.begin(); it != _pIosIconSet->_vtScreen.end(); it++)
        {
            IosScreen* pScreenTemp = *it;
            if (pScreenTemp)
            {
                for (int i = 0; i < _pIosIconSet->_nScreenLineCount; i++)
                {
                    for (int j = 0; j < _pIosIconSet->_nScreenRowCount; j++)
                        InitIconImageFile(pScreenTemp->_screenInfo[i][j]);
                }
            }
        }

        // 取临时目录
        DeviceData* pDevice = DEVICE_MANAGER->FindIosDevice(_pDevPtr);
        if (pDevice)
        {
            wstring strScreenWallpaper = pDevice->GetPath()->GetLocalIconPath();
            CFileOperation::MakeLocalDir(strScreenWallpaper);
            strScreenWallpaper += L"\\Wallpaper" + CFileOperation::GetGuidW() + L".png";
            if (GetHomeScreenWallpaper(strScreenWallpaper))
                _pIosIconSet->_strWallPaperFullName = strScreenWallpaper;
        }			
    }

	mapMsg.clear();
    LeaveCriticalSection(&_cs);

	return _pIosIconSet;
}

// 设置新的图标状态
bool IosScreenOperation::SetIconState()
{
    EnterCriticalSection(&_cs);
	StartSpringBoardService();

    RETURN_FALSE_IF(!_pIosIconSet || !_pSpringBoardServiceHandle);

	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 2, NULL, NULL);

	void* pKey = CCFConverter::StringtoCFString("command");
	void* pVaule = CCFConverter::StringtoCFString("setIconState");
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	pKey = CCFConverter::StringtoCFString("iconState");
	pVaule = _pIosIconSet->PackToCFData();
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	bool bRet = false;
    if (IosPublicFunc::SendMessageToService(_pSpringBoardServiceHandle, pCFDict))
    {
        bRet = true;
    }
    else
    {
        StartSpringBoardService();
        if (IosPublicFunc::SendMessageToService(_pSpringBoardServiceHandle, pCFDict))
            bRet = true;
    }

	CCFConverter::NDCFRelease(pCFDict);

	CloseSpringBoardService();
    LeaveCriticalSection(&_cs);

	return bRet;
}

// 获取主屏幕壁纸(固件4.0开始才有这个功能,3GS机器开始,3G需要手动开启)
bool IosScreenOperation::GetHomeScreenWallpaper(wstring strSavePath)
{
    EnterCriticalSection(&_cs);
	StartSpringBoardService();

    RETURN_FALSE_IF(_nVer < 400 || !_pSpringBoardServiceHandle);

	bool bRet = false;
	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("command", "getHomeScreenWallpaperPNGData"));

    void* pReplay = NULL;
	if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
	{
        pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }
    else
    {
        StartSpringBoardService();
        if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
            pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }

	map<std::string, void*> dict;
	if (pReplay && CCFConverter::GetIosData(pReplay, dict))
	{
		map<std::string, void*>::iterator it = dict.find("pngData");
		if (it != dict.end())
		{
			int nLen = CCFConverter::NDCFDataGetLength(it->second);
			void* pValue = CCFConverter::NDCFDataGetBytePtr(it->second);
			unsigned char* uszValue = new unsigned char[nLen];
			//ZeroMemory(uszValue, nLen);
			memset(uszValue, 0, nLen);
			memcpy(uszValue, pValue, nLen);

			//FILE* f = _wfopen(strSavePath.c_str(), L"w+b");
			FILE* f = fopen(CCodeOperation::WstringToString(strSavePath).c_str(), "w+b");
			if (f) 
			{
				if(fwrite(uszValue, nLen, 1, f))
					bRet = true;

				fflush(f);
				fclose(f);					
			}	
			SAFE_DELETE_ARRAY(uszValue);
		}
		
		dict.clear();
	}

	mapMsg.clear();

	CloseSpringBoardService();
    LeaveCriticalSection(&_cs);

	return bRet;
}

// 获取设备屏幕方向
eInterfaceOrientation IosScreenOperation::GetInterfaceOrientation()
{
    EnterCriticalSection(&_cs);
	StartSpringBoardService();

// 	if (!_biPad)
// 		return eInterfaceOrientation_LandScape;

	eInterfaceOrientation eType = eInterfaceOrientation_LandScape;

	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("command", "getInterfaceOrientation"));

    void* pReplay = NULL;
	if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
	{
		pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }
    else
    {
        StartSpringBoardService();
        if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
            pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }

    map<std::string, void*> dict;
	if (pReplay && CCFConverter::GetIosData(pReplay, dict))
	{
		/*__int32*/int32_t nType;
		map<std::string, void*>::iterator it = dict.find("interfaceOrientation");
		if (it != dict.end() && CCFConverter::GetIosData(it->second, nType))
		{
			switch (nType)
			{
			//case 2:
			case 3:
            case 4:
				eType = eInterfaceOrientation_LandScape;
				break;
            case 1:
			default:
				eType = eInterfaceOrientation_Porttrait;
				break;
			}
		}
	}

	mapMsg.clear();

	CloseSpringBoardService();
    LeaveCriticalSection(&_cs);

	return eType;
}

wstring IosScreenOperation::GetIconImageData(string strBundleID)
{
    EnterCriticalSection(&_cs);
	StartSpringBoardService();

	wstring strFile;
    RETURN_VALUE_IF(!_pSpringBoardServiceHandle, strFile);

	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("command", "getIconPNGData"));
	mapMsg.insert(pair<string, string>("bundleId", strBundleID));

    void* pReplay = NULL;
	if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
	{
		pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }
    else
    {
        StartSpringBoardService();
        if (SendMessageToService(_pSpringBoardServiceHandle, mapMsg))
            pReplay = IosPublicFunc::ReadMssageFromService(_pSpringBoardServiceHandle);
    }

    map<std::string, void*> dict;
	if (pReplay && CCFConverter::GetIosData(pReplay, dict))
	{
		map<std::string, void*>::iterator it = dict.find("pngData");
		if (it != dict.end())
		{
			int nLen = CCFConverter::NDCFDataGetLength(it->second);
			void* pValue = CCFConverter::NDCFDataGetBytePtr(it->second);
			unsigned char* uszValue = new unsigned char[nLen];
			//ZeroMemory(uszValue, nLen);
			memset(uszValue, 0, nLen);
			memcpy(uszValue, pValue, nLen);
			
            // 取临时目录
            DeviceData* pDevice = DEVICE_MANAGER->FindIosDevice(_pDevPtr);
            RETURN_VALUE_IF(! pDevice, L"");
			wstring strSavePath = pDevice->GetPath()->GetLocalIconPath();
            CFileOperation::MakeLocalDir(strSavePath);

			strSavePath += L"\\" + CCodeOperation::GB2312ToUnicode(strBundleID) + L".png";

			//FILE* f = _wfopen(strSavePath.c_str(), L"w+b");
			FILE* f = fopen(CCodeOperation::WstringToString(strSavePath).c_str(), "w+b");
			if (f) 
			{
				if(fwrite(uszValue, nLen, 1, f))
					strFile = strSavePath;

				fflush(f);
				fclose(f);					
			}

			SAFE_DELETE_ARRAY(uszValue);
		}

		dict.clear();
	}

	mapMsg.clear();
    LeaveCriticalSection(&_cs);

	return strFile;
}

void IosScreenOperation::InitIconImageFile(IosIcon* pIcon)
{
    RETURN_IF(!pIcon);
	if (pIcon->GetIconType() == eIosIconType_Normal)
	{
		pIcon->_strIconImgFile = GetIconImageData(pIcon->_strDisplayIndentifier);
        RETURN_IF(CFileOperation::IsFileExist(pIcon->_strIconImgFile.c_str()));

		if (pIcon->_strDisplayIndentifier.find("mobile") < 0)
			pIcon->_strIconImgFile = GetIconImageData(pIcon->_strBundleIdentifier);
	}
	else 
	{
		IosFolderIcon* pFolderIcon = dynamic_cast<IosFolderIcon*>(pIcon);
		for (int i = 0; i < (int)pFolderIcon->_vtIconList.size(); i++)
		{
			InitIconImageFile(pFolderIcon->_vtIconList.at(i));
		}
	}
}

bool IosScreenOperation::SendMessageToService(void* pSocket, map<string, string> mapMsg)
{
    RETURN_FALSE_IF(pSocket == NULL || mapMsg.size() <= 0);

	void* pMsgDict = CCFConverter::DictionarytoCFDictionary(mapMsg);
    RETURN_FALSE_IF(!pMsgDict);

	bool bRet = IosPublicFunc::SendMessageToService(pSocket, pMsgDict);

	CCFConverter::NDCFRelease(pMsgDict);

	return bRet;
}

IosIconSet* IosScreenOperation::ReadMssageFromService(void* pSocket)
{
    RETURN_NULL_IF(pSocket == NULL);

	IosIconSet* pIconSet = NULL;
	void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
	if (pReplay)
	{
		pIconSet = new IosIconSet(pReplay, _nVer, _biPad, _eInterfaceOrientationType);//
/*		CCFConverter::NDCFRelease(pReplay);*/
	}

	return pIconSet;
}

void IosScreenOperation::StartSpringBoardService()
{
    RETURN_IF(_pSpringBoardServiceHandle);

	void* pInstallSocket = NULL;
	DeviceInfo devInfo;
	devInfo._ios_devptr = _pDevPtr;
	devInfo._deviceProductVersion = _strProductVersion;
	IosUsbConnection conn(&devInfo);
	if (!conn.StartSpringBoardService(&_pSpringBoardServiceHandle))
		_pSpringBoardServiceHandle = NULL;
}

void IosScreenOperation::CloseSpringBoardService()
{
	if (_pSpringBoardServiceHandle)
		CITunesMobileDevice::CloseSocket(_pSpringBoardServiceHandle);
	_pSpringBoardServiceHandle = NULL;
}

void IosScreenOperation::FreeIconSet()
{
	SAFE_DELETE(_pIosIconSet);
}
#endif