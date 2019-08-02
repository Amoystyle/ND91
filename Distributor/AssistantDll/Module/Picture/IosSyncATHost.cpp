#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosSyncATHost.h"
#include "Device/IosUsbSync.h"
#include "Device/AirTrafficHost.h"
#include "Common/version.h"
#include "Device/CFConverter.h"
#include "Common/plist/PlistOperation.h"
#include "Device/ITunesService.h"

char* ConverWstringtoChar(wstring source)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen <= 0)
	{
		LOG_DEBUG(L"WideCharToMultiByte return invalid length");
		return NULL;
	}
	char* dest = new char[nLen];
	if (NULL == dest)
	{
		LOG_DEBUG(L"Memory run out of use!");
		return NULL;
	}

	WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, dest, nLen, NULL, NULL);
	dest[nLen-1] = 0;

	return dest;
}

string ConverWstringtoString(wstring source)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen <= 0)
	{
		LOG_DEBUG(L"WideCharToMultiByte return invalid length");
		return std::string("");
	}
	char* destString = new char[nLen];
	if (NULL == destString)
	{
		LOG_DEBUG(L"Memory run out of use!");
		return std::string("");
	}
	WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, destString, nLen, NULL, NULL);
	destString[nLen-1] = 0;
	std::string strTemp(destString);
	SAFE_DELETE_ARRAY(destString);
	return strTemp;
}

wstring ConverStringtoWstring(string source)
{
	int nLen = source.length();
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)source.c_str(), nLen, NULL, 0);
	if (nSize <= 0)
	{
		LOG_DEBUG(L"WideCharToMultiByte return invalid length");
		return L"";
	}
	WCHAR *destWchar = new WCHAR[nSize+1];
	if (NULL == destWchar)
	{
		LOG_DEBUG(L"Memory run out of use!");
		return L"";
	}
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)source.c_str(), nLen, destWchar, nSize);
	destWchar[nSize] = 0;
	if(destWchar[0] == 0xFEFF) // Skip 0xfeff
	{
		for(int i = 0; i < nSize; ++i) 
			destWchar[i] = destWchar[i+1]; 
	}
	wstring destWstring(destWchar);
	SAFE_DELETE_ARRAY(destWchar);
	return destWstring;
}

int CopyFileToDevice(wstring localFile,wstring remoteFile)
{
	IIosFileSystem* pFileStream;
	if (g_devInfo->_connectType == USB)
	{
		pFileStream = new AFCFileSysemBase(g_devInfo);
	}
	else
	{
		//return -1;
	}
	pFileStream->Ios_CopyToPhone(localFile, remoteFile);
	return 0;
}

bool InputParamsResolve(sConnDeviceInfo* sDeviceData, map<wstring, wstring>* photoData, vector<char*> pictUuids)
{
	if((sDeviceData) && (sDeviceData->DevInfo->_ios_afcHandle) && (!sDeviceData->DevInfo->_ios_deviceIdentifier.empty()) )
	{
		if(sDeviceData->OperateMode == PHOTO_ADD)
		{
			if((sDeviceData->ItemCount> 0 ) && (photoData) && (pictUuids.size() > 0) )
			{
				g_photo_count = sDeviceData->ItemCount;
				g_pPhotoLibrary = new sPhotoData[g_photo_count];
				memset(g_pPhotoLibrary,0,sizeof(sPhotoData) * g_photo_count);

				unsigned int i = 0;
				for (map<wstring, wstring>::iterator itr = photoData->begin(); itr != photoData->end(); ++itr)
				{
					g_pPhotoLibrary[i].photo_local_path = itr->first;
					g_pPhotoLibrary[i].photo_remote_path = itr->second;
					i++;
				}
				i = 0;
				for(vector<char*>::iterator itr = pictUuids.begin(); itr != pictUuids.end(); ++itr)
				{
					g_pPhotoLibrary[i++].photo_uuid = *itr;
				}
			}
			else
			{
				return false;
			}				
		}
		else
		{
			return false;
		}

		g_PhotoOperate = sDeviceData->OperateMode;
		g_AFcConnection = sDeviceData->DevInfo->_ios_afcHandle;
		g_devInfo = sDeviceData->DevInfo;
		return true;
	}
	return false;
}

bool GlobalParamsInit()
{
	if(!g_piTunesPrefsID)
	{
		g_piTunesPrefsID = new char[BUFFER_SIZE_PREFSID];
		if(!g_piTunesPrefsID)
			return false;
	}

	if(!g_pMediaBuffer)
	{
		g_pMediaBuffer = new char[BUFFER_SIZE_MEDIA];
		if(!g_pMediaBuffer)
			return false;
	}
	g_hCon = 0;
	g_hThread_Recv = 0;
	g_hThread_Send = 0;
	g_iStatus = STATUS_INITIALIZE;					
	g_SendRequest = STATUS_NO;
	return true;
}

bool GlobalParamsUninit()
{
	if(g_piTunesPrefsID)
	{
		SAFE_DELETE(g_piTunesPrefsID);
	}
	if(g_pMediaBuffer)
	{
		SAFE_DELETE(g_pMediaBuffer);
	}
	if(g_hMediaDictionarData)
	{
		CCFConverter::NDCFRelease(g_hMediaDictionarData);
	}
	if(g_pPhotoLibrary)
	{
		SAFE_DELETE_ARRAY(g_pPhotoLibrary);
	}
	if(g_BplistBuffer)
	{
		SAFE_DELETE_ARRAY(g_BplistBuffer);
	}
	if(g_pHostName)
	{
		SAFE_DELETE(g_pHostName);
	}
	if(g_pItunesVersion)
	{
		SAFE_DELETE(g_pItunesVersion);
	}
	if(g_pErrorFileData)
	{
		SAFE_DELETE(g_pErrorFileData);
	}
	g_BplistSize = 0;
	g_hCon = 0;
	return true;
}

bool GetiTunesInfo()
{
	if (g_pItunesVersion)
	{
		SAFE_DELETE(g_pItunesVersion);
	}
	if(g_pHostName)
	{
		SAFE_DELETE(g_pHostName);
	}

	//获取iTunes.exe的版本
	wstring iTunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Apple Computer, Inc.\\iPod\\RegisteredApps\\4", _T("PATH"));
	wstring iTunesVer;
	int nLen = iTunesPath.length() + 1;
	TCHAR* szDLLiTunes = new TCHAR[nLen];
	ZeroMemory(szDLLiTunes, nLen*sizeof(TCHAR));
	if (szDLLiTunes)
	{
		_tcscat_s(szDLLiTunes, nLen, iTunesPath.c_str());
		CVersion Ver = CVersion::GetFileVersion(szDLLiTunes);
		iTunesVer = Ver.ToStringW();
	}
	delete []szDLLiTunes;
	szDLLiTunes = NULL;

	int nLenITunesVer = WideCharToMultiByte(CP_ACP,0,iTunesVer.c_str(),-1,NULL,0,NULL,NULL);
	g_pItunesVersion = new char[nLenITunesVer];  // 转换后长度已经自动加1
	if (NULL == g_pItunesVersion)
		return false;
	WideCharToMultiByte(CP_ACP,0,iTunesVer.c_str(),-1,g_pItunesVersion,nLenITunesVer,NULL,NULL);
	g_pItunesVersion[nLenITunesVer-1] = 0;  // 字符串结尾

	// 获取host name
	DWORD dwSize=100;	 			
	GetComputerName((LPWSTR)iTunesVer.c_str(),&dwSize); 
	int nLenHostName = WideCharToMultiByte(CP_ACP,0,iTunesVer.c_str(),-1,NULL,0,NULL,NULL);
	g_pHostName = new char[nLenHostName];	
	if (NULL == g_pHostName)
		return false;
	WideCharToMultiByte(CP_ACP,0,iTunesVer.c_str(),-1,g_pHostName,nLenHostName,NULL,NULL);
	g_pHostName[nLenHostName-1] = 0;

	return true;
}

int CreatePrefsID()
{
	if (g_piTunesPrefsID)
	{
		SAFE_DELETE(g_piTunesPrefsID);
	}

	g_piTunesPrefsID = new char[BUFFER_SIZE_PREFSID];
	memset(g_piTunesPrefsID,0,BUFFER_SIZE_PREFSID);

	afc_file_ref ref;
	afc_error_t ret = CITunesMobileDevice::iTunes_AFCFileRefOpen(g_devInfo->_ios_afcHandle, L"/private/var/mobile/Media/iTunes_Control/iTunes/iTunesPrefs", 2, &ref);
	if (ret)
	{
		LOG_DEBUG(L"AFCFileRefOpen iTunesPrefs failed: return " + CStrOperation::IntToWString(ret));
		return ret;
	}

	t_PrefsID s_PrefsID;
	unsigned int len = sizeof(t_PrefsID);
	ret = CITunesMobileDevice::iTunes_AFCFileRefRead(g_devInfo->_ios_afcHandle, ref, (char*)&s_PrefsID, &len);
	if (ret)
	{
		LOG_DEBUG(L"AFCFileRefRead iTunesPrefs failed: return " + CStrOperation::IntToWString(ret));
		return ret;
	}

	sprintf(g_piTunesPrefsID, "%08X%08X", s_PrefsID.str1_1^s_PrefsID.str1_2, s_PrefsID.str2_1^s_PrefsID.str2_2);

	ret = CITunesMobileDevice::iTunes_AFCFileRefClose(g_devInfo->_ios_afcHandle, ref);
	if (ret)
	{
		LOG_DEBUG(L"AFCFileRefClose iTunesPrefs failed: return " + CStrOperation::IntToWString(ret));
		return ret;
	}

	return 0;
}

HRESULT ConnectATH(char *PrefsID, wstring Uuid)
{
	void* pPrefsID = CCFConverter::ND__CFStringMakeConstantString(PrefsID);
	char* tempUuid = ConverWstringtoChar(Uuid);
	void* pUuid = CCFConverter::ND__CFStringMakeConstantString(tempUuid);
	if ( (pPrefsID == NULL) || (pUuid == NULL) )
	{
		LOG_DEBUG(L"Null prefsId or uuid after call CFStringMakeConstantString");
		return -1;
	}
	void* hCon = CAirTrafficHost::iTunes_ATHostConnectionCreateWithLibrary( pPrefsID, pUuid, 0 );

	if(!hCon)
	{
		LOG_DEBUG(L"ConnectATH failed ");
	}
	LOG_DEBUG(L"ConnectATH success");

	CCFConverter::NDCFRelease(pPrefsID);
	CCFConverter::NDCFRelease(pUuid);
	SAFE_DELETE_ARRAY(tempUuid);

	return (HRESULT)hCon; 
}

void SendPowerAssertion()
{
	int hr;
	void* iParam = CCFConverter::NDkCFBooleanTrue();
	if(g_hCon)
	{
		hr = CAirTrafficHost::iTunes_ATHostConnectionSendPowerAssertion( g_hCon, iParam );
	}
	LOG_DEBUG(L"Return SendPowerAssertion = " + CStrOperation::IntToWString(hr));
}

void SendConnectionRetain()
{
	int hr;
	if(g_hCon)
	{	
		hr = CAirTrafficHost::iTunes_ATHostConnectionRetain( g_hCon );
	}
	LOG_DEBUG(L"Return SendConnectionRetain = " + CStrOperation::IntToWString(hr));
}

int SendHostInfo()
{
	if (!g_hCon)
	{
		return -1;
	}
	
	int hr = -1;
	void* dictKeyCallBacks = CCFConverter::NDkCFTypeDictionaryKeyCallBacks();
	void* dictValueCallBacks = CCFConverter::NDkCFTypeDictionaryValueCallBacks();
	void* typeArrayCallBacks = CCFConverter::NDkCFTypeArrayCallBacks();

	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
	void* pKey = CCFConverter::ND__CFStringMakeConstantString("LibraryID");
	void* pValue = CCFConverter::ND__CFStringMakeConstantString(g_piTunesPrefsID);
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);
	pKey = CCFConverter::ND__CFStringMakeConstantString("SyncHostName");
	pValue = CCFConverter::ND__CFStringMakeConstantString(g_pHostName);
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);
	void* pCFArray = CCFConverter::NDCFArrayCreateMutable(NULL, 0, typeArrayCallBacks);
	pKey = CCFConverter::ND__CFStringMakeConstantString("SyncedDataclasses");
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pCFArray);
	pKey = CCFConverter::ND__CFStringMakeConstantString("Version");
	pValue = CCFConverter::ND__CFStringMakeConstantString(g_pItunesVersion);
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);

	hr = CAirTrafficHost::iTunes_ATHostConnectionSendHostInfo( g_hCon, pCFDict );

	LOG_DEBUG(L"Return SendHostInfo = " + CStrOperation::IntToWString(hr));

	return hr;
}

HRESULT SendSyncRequest( void )
{
	LOG_DEBUG(L"SendSyncRequest");
	HRESULT hr;	
	if(g_hCon)
	{
		void* dictKeyCallBacks = CCFConverter::NDkCFTypeDictionaryKeyCallBacks();
		void* dictValueCallBacks = CCFConverter::NDkCFTypeDictionaryValueCallBacks();
		void* typeArrayCallBacks = CCFConverter::NDkCFTypeArrayCallBacks();

		void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
		void* pKey = CCFConverter::ND__CFStringMakeConstantString("Media");
		void* pValue = CCFConverter::ND__CFStringMakeConstantString("0");
		CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pValue);

		void* pCFArray = CCFConverter::NDCFArrayCreateMutable(NULL, 0, typeArrayCallBacks);
		pValue = CCFConverter::ND__CFStringMakeConstantString("Photo");
		CCFConverter::NDCFArrayAppendValue(pCFArray, pValue);
		pValue = CCFConverter::ND__CFStringMakeConstantString("Keybag");
		CCFConverter::NDCFArrayAppendValue(pCFArray, pValue);

		void* pCFArray2 = CCFConverter::NDCFArrayCreateMutable(NULL, 0, typeArrayCallBacks);
		pValue = CCFConverter::ND__CFStringMakeConstantString("Data");
		CCFConverter::NDCFArrayAppendValue(pCFArray2, pValue);

		void* pCFDict2 = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
		pKey = CCFConverter::ND__CFStringMakeConstantString("LibraryID");
		pValue = CCFConverter::ND__CFStringMakeConstantString(g_piTunesPrefsID);
		CCFConverter::NDCFDictionarySetValue(pCFDict2, pKey, pValue);
		pKey = CCFConverter::ND__CFStringMakeConstantString("SyncHostName");
		pValue = CCFConverter::ND__CFStringMakeConstantString(g_pHostName);	
		CCFConverter::NDCFDictionarySetValue(pCFDict2, pKey, pValue);
		pKey = CCFConverter::ND__CFStringMakeConstantString("SyncedDataclasses");
		CCFConverter::NDCFDictionarySetValue(pCFDict2, pKey, pCFArray2);
		pKey = CCFConverter::ND__CFStringMakeConstantString("Version");
		pValue = CCFConverter::ND__CFStringMakeConstantString(g_pItunesVersion);	
		CCFConverter::NDCFDictionarySetValue(pCFDict2, pKey, pValue);
		
		hr = (HRESULT)CAirTrafficHost::iTunes_ATHostConnectionSendSyncRequest(g_hCon, pCFArray, pCFDict, pCFDict2);

		LOG_DEBUG(L"Return SendSyncRequest = " + CStrOperation::IntToWString(hr));

		CCFConverter::NDCFRelease(pKey);
		CCFConverter::NDCFRelease(pValue);
		CCFConverter::NDCFRelease(pCFDict2);
		CCFConverter::NDCFRelease(pCFArray2);
	}	
	return hr;
}

int HandleAssetManifest(void* hCopyDict)
{
	void* h_CFString_Params = CCFConverter::ND__CFStringMakeConstantString("Params");
	void* h_Data_Params = CCFConverter::NDCFDictionaryGetValue(hCopyDict, h_CFString_Params);
	if ( !h_Data_Params || (CCFConverter::NDCFGetTypeID(h_Data_Params) != CCFConverter::NDCFDictionaryGetTypeID()) )
	{
		return STATUS_NULL_MESSAGE;
	}

	void* h_CFString_AssetM = CCFConverter::ND__CFStringMakeConstantString("AssetManifest");
	void* h_Data_AssetM = CCFConverter::NDCFDictionaryGetValue(h_Data_Params, h_CFString_AssetM);
	if ( !h_Data_AssetM || (CCFConverter::NDCFGetTypeID(h_Data_AssetM) != CCFConverter::NDCFDictionaryGetTypeID()) )
	{
		return STATUS_NULL_MESSAGE;
	}

	if (g_hMediaDictionarData)
	{
		CCFConverter::NDCFRelease(g_hMediaDictionarData);
	}
	g_hMediaDictionarData = CCFConverter::NDCFDictionaryCreateCopy( NULL, h_Data_AssetM );

	CCFConverter::NDCFRelease(h_CFString_Params);
	CCFConverter::NDCFRelease(h_Data_Params);
	CCFConverter::NDCFRelease(h_CFString_AssetM);
	CCFConverter::NDCFRelease(h_Data_AssetM);

	return STATUS_SEND_FILE_ERROR;
}

int	MessageResolve()
{
	void* hData = CAirTrafficHost::iTunes_ATHostConnectionReadMessage( g_hCon );
	if (!hData)
	{
		LOG_DEBUG(L"ATHostConnectionReadMessage failed!");
		return STATUS_NULL_MESSAGE;
	}
	void* hCopyData = CCFConverter::NDCFDictionaryCreateCopy(NULL, hData);
	if( !hCopyData )
	{
		LOG_DEBUG(L"DictionaryCreateCopy failed!");
		return STATUS_NULL_MESSAGE;
	}
	if( CCFConverter::NDCFGetTypeID(hCopyData) != CCFConverter::NDCFDictionaryGetTypeID() )
	{
		CCFConverter::NDCFRelease(hCopyData);
		return STATUS_NULL_MESSAGE;
	}

	void* hCommand = CCFConverter::ND__CFStringMakeConstantString("Command");
	void* hDataCommand = CCFConverter::NDCFDictionaryGetValue(hCopyData, hCommand);
	if (!hDataCommand)
	{
		LOG_DEBUG(L"Get Command failed!");
		return STATUS_NULL_MESSAGE;
	}

	string commandValue;
	if ( !CCFConverter::GetIosData(hDataCommand, commandValue) )
	{
		LOG_DEBUG(L"Convert command failed!");
		CCFConverter::NDCFRelease(hCopyData);
		return STATUS_NULL_MESSAGE;
	}

	if (commandValue.empty())
	{
		LOG_DEBUG(L"Empty command!");
		CCFConverter::NDCFRelease(hCopyData);
		return STATUS_NULL_MESSAGE;
	}

	LOG_DEBUG(L"Command from ATHost: "+ ConverStringtoWstring(commandValue));

	if (commandValue == "SyncAllowed")
	{
		LOG_DEBUG(L"STATUS_SYNCALLOWED!");
		return STATUS_SYNCALLOWED;
	}
	else if (commandValue == "ReadyForSync")
	{
		return STATUS_READYFORSYNC;	
	}
	else if(commandValue == "SyncFailed")
	{
		return STATUS_SYNCFAILED;
	}
	else if(commandValue == "SyncFinished")
	{
		return STATUS_SYNCFINISHED;
	}
	else if(commandValue == "AssetManifest")
	{
		return HandleAssetManifest(hCopyData);
	}
	else
	{
		//put some trace here
	}

	CCFConverter::NDCFRelease(hCopyData);
	CCFConverter::NDCFRelease(hCommand);
	CCFConverter::NDCFRelease(hDataCommand);	

	return STATUS_NULL_MESSAGE;
}

HRESULT SendFinished()
{
	HRESULT hr = -1;
	int p = 1;
	if(g_hCon && g_pMediaBuffer)
	{
		// Step 1
		//void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
		void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, NULL, NULL);
		void* pKey = CCFConverter::ND__CFStringMakeConstantString("Photo");
		void* pValue = CCFConverter::Int64ToCFInt64(p);   // kCFNumberSInt64Type = 4
		CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);

		// Step 2
		//void* pCFDict2 = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
		void* pCFDict2 = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, NULL, NULL);
		hr = CAirTrafficHost::iTunes_ATHostConnectionSendMetadataSyncFinished(g_hCon, pCFDict, pCFDict2);

		LOG_DEBUG(L"ATHostConnectionSendMetadataSyncFinished<> = " + CStrOperation::IntToWString(hr));

		CCFConverter::NDCFRelease(pCFDict);
		CCFConverter::NDCFRelease(pKey);
		CCFConverter::NDCFRelease(pValue);
		CCFConverter::NDCFRelease(pCFDict2);
	}

	return hr;
}

int SendPlistFile( char *BplistBuffer, int BplistSize )
{
	if ( (BplistSize == NULL) || (BplistSize <= 0) )
	{
		LOG_DEBUG(L"Plist file is empty");
		//return -1;
	}
	int ret = -1;
	int SessionID = CAirTrafficHost::iTunes_ATHostConnectionGetGrappaSessionId(g_hCon);
	LOG_DEBUG(L"ATHostConnectionGetGrappaSessionId: SessionId = " + CStrOperation::IntToWString(SessionID));

	if(SessionID)
	{
		ret = CopyFileToDevice(L"d:\\latestCodes\\PhotoLibrary_new.plist", L"/private/var/mobile/Media/Photos/Sync/PhotoLibrary.plist");
		if(!ret)
		{
			SendPowerAssertion();
			SendFinished();
		}					
	}
	else
	{
		LOG_DEBUG(L"SessionID is not valid!")
	}
	return ret;
}

static DWORD WINAPI sync_rev_thread( LPVOID lpParameter )
{
	LOG_DEBUG(L"Start sync_rev_thread");
	int Status;
	while( 1 )
	{		
		if(STATUS_CLOSE_THREAD == g_iStatus)
		{
			CloseHandle(g_hThread_Recv);
			g_hThread_Recv = 0;
			LOG_DEBUG(L"Close sync_rev_thread");
			return 0;
		}
		Status = MessageResolve();
		if(Status == STATUS_NULL_MESSAGE)
		{
			Sleep(5);
		}
		else
		{
			switch(Status)
			{
			case STATUS_NULL_MESSAGE:
				break;

			case STATUS_SYNCALLOWED:				//Sync Allowed
				if(g_SendRequest == STATUS_NO)
				{
					g_SendRequest = STATUS_SEND;
				}
				g_iStatus = STATUS_SYNCALLOWED;
				break;

			case STATUS_READYFORSYNC:			   //Sync Ready
				g_iStatus = STATUS_READYFORSYNC;	//Just Test
				break;

			case STATUS_SEND_FILE_ERROR:		  //Send File Error
				g_iStatus = STATUS_SEND_FILE_ERROR;
				break;
			case STATUS_SYNCFINISHED:
				CloseHandle(g_hThread_Recv);
				g_hThread_Recv = 0;
				g_iStatus = STATUS_SYNCFINISHED;
				return 1;				
				break;
			case STATUS_SYNCFAILED:
				CloseHandle(g_hThread_Recv);
				g_hThread_Recv = 0;
				g_iStatus = STATUS_SYNCFAILED;
				return 1;				
				break;				

			default:
				break;
			}
		}			
	}
}

bool GetAssetID_SendFile()
{
	bool ret = false;

	void* pKey = CCFConverter::ND__CFStringMakeConstantString("Photo");
	void* hMedia = CCFConverter::NDCFDictionaryGetValue(g_hMediaDictionarData, pKey);
	if (hMedia && ( CCFConverter::NDCFGetTypeID(hMedia) == CCFConverter::NDCFArrayGetTypeID()))
	{
		int num = CCFConverter::NDCFArrayGetCount(hMedia);
		if (num > 0)
		{
			g_error_file_count = num;
			if (g_pErrorFileData)
			{
				SAFE_DELETE(g_pErrorFileData);
			}
			g_pErrorFileData = new sErrorFileData[g_error_file_count];
			memset(g_pErrorFileData,0,sizeof(sErrorFileData)*g_error_file_count);

			void** ptr = new void*[g_error_file_count];
			CFRange range(0, g_error_file_count);
			CCFConverter::NDCFArrayGetValues(hMedia, range, ptr);

			for (int i = 0; i < num; ++i)
			{
				if ( CCFConverter::NDCFGetTypeID(ptr[i]) == CCFConverter::NDCFDictionaryGetTypeID() )
				{
					void* hCFStringAssetID = CCFConverter::ND__CFStringMakeConstantString("AssetID");
					void* hAssetID = CCFConverter::NDCFDictionaryGetValue(ptr[i], hCFStringAssetID);
					if( hAssetID && (CCFConverter::NDCFGetTypeID(hAssetID) == CCFConverter::NDCFStringGetTypeID()))
					{
						CCFConverter::GetIosData(hAssetID, g_pErrorFileData[i].photoAssetIDuuid);
						ret = true;
					}
				}
			}
		}
	}

	return ret;
}

HRESULT SendAssetCompleted(char* fileName, wstring fileType, wstring newFilePath)
{
	HRESULT hr;

	char* tempFileType = ConverWstringtoChar(fileType);
	char* tempNewFilePath = ConverWstringtoChar(newFilePath);
	void* h_name = CCFConverter::ND__CFStringMakeConstantString(fileName);
	void* h_type = CCFConverter::ND__CFStringMakeConstantString(tempFileType);
	void* h_new_file_path = CCFConverter::ND__CFStringMakeConstantString(tempNewFilePath);

	hr = CAirTrafficHost::iTunes_ATHostConnectionSendAssetCompleted(g_hCon, h_name, h_type, h_new_file_path);

	CCFConverter::NDCFRelease(h_name);
	CCFConverter::NDCFRelease(h_type);
	CCFConverter::NDCFRelease(h_new_file_path);

	return hr;
}

int SendPhotoProgress()
{
	wstring photoName;
	for(int i = 0; i < g_photo_count; i++)
	{
		photoName = L"/private/var/mobile/Media/Airlock/Photo/" + ConverStringtoWstring(g_pPhotoLibrary[i].photo_uuid);
		CopyFileToDevice(g_pPhotoLibrary[i].photo_local_path.c_str(), photoName);

		photoName = L"/Airlock/Thumbs/" + ConverStringtoWstring(g_pPhotoLibrary[i].photo_uuid) +L".mthmb";
		SendAssetCompleted(g_pPhotoLibrary[i].photo_uuid ,L"Photo",photoName);					
	}
	return 0;
}

HRESULT SendFileError( string fileName, char* fileType, char number)
{
	HRESULT hr;

	int nLen = fileName.length();
	char* tempFileName = new char[nLen+1];
	memset(tempFileName, 0, nLen+1);
	memcpy(tempFileName, fileName.c_str(), nLen+1);

	void* h_name = CCFConverter::ND__CFStringMakeConstantString(tempFileName);
	void* h_type = CCFConverter::ND__CFStringMakeConstantString(fileType);  // "Media"

	hr = CAirTrafficHost::iTunes_ATHostConnectionSendFileError( g_hCon, h_name, h_type, number);

	CCFConverter::NDCFRelease(h_name);
	CCFConverter::NDCFRelease(h_type);
	
	return hr;
}

HRESULT SendFileProgress( char *File_name, char *File_type,double a4, double a5)
{
	HRESULT hr;

	void* h_name = CCFConverter::ND__CFStringMakeConstantString(File_name);	
	void* h_type = CCFConverter::ND__CFStringMakeConstantString(File_type);

	hr = CAirTrafficHost::iTunes_ATHostConnectionSendFileProgress( g_hCon, h_name,h_type, (a4), (a5));

	CCFConverter::NDCFRelease(h_name);
	CCFConverter::NDCFRelease(h_type);
	return hr;
}

void HandleStatusSendFileError()
{
	if(!g_hMediaDictionarData)
		return;
	if(!GetAssetID_SendFile())
		return;

	if(PHOTO_ADD == g_PhotoOperate)
	{
		SendPhotoProgress();
	}

	if (g_error_file_count > 0)
	{
		int j;
		for(int i = 0; i < g_error_file_count; ++i)
		{
			for(j = 0; j < g_photo_count; ++j)
			{
				if (g_pErrorFileData[i].photoAssetIDuuid.c_str() == g_pPhotoLibrary[j].photo_uuid)
				{
					break;
				}
			}
			if(j > g_photo_count)
			{
				SendFileError(g_pErrorFileData[i].photoAssetIDuuid,"Photo",j); // Need further check
			}
		}
	}
}

static DWORD WINAPI sync_send_thread( LPVOID lpParameter )
{
	LOG_DEBUG(L"Start sync_send_thread");
	while(1)
	{
		if(STATUS_SYNCALLOWED == g_iStatus)
		{
			LOG_DEBUG(L"Handle STATUS_SYNCALLOWED");
			g_iStatus = STATUS_INITIALIZE;
			if(g_SendRequest == STATUS_SEND)
			{
				g_SendRequest = STATUS_SEND_FINISH;
				SendPowerAssertion();
				SendSyncRequest();
			}
		}
		else if(STATUS_READYFORSYNC == g_iStatus)
		{
			LOG_DEBUG(L"Handle STATUS_READYFORSYNC");
			SendPlistFile( g_BplistBuffer, g_BplistSize );
			g_iStatus = STATUS_INITIALIZE;
		}
		else if(STATUS_SEND_FILE_ERROR == g_iStatus)
		{
			LOG_DEBUG(L"Handle STATUS_SEND_FILE_ERROR");
			HandleStatusSendFileError();
			g_iStatus = STATUS_INITIALIZE;
		}
		else if(STATUS_SYNCFINISHED == g_iStatus || STATUS_SYNCFAILED == g_iStatus)
		{
			LOG_DEBUG(L"Handle STATUS_SYNCFINISHED or STATUS_SYNCFAILED");
			CloseHandle(g_hThread_Send);
			g_hThread_Send = 0;
			g_iStatus = STATUS_INITIALIZE;
			return 0;
		}
		else if(STATUS_CLOSE_THREAD == g_iStatus || 0 == g_hThread_Recv)
		{
			LOG_DEBUG(L"Handle STATUS_CLOSE_THREAD");
			CloseHandle(g_hThread_Send);
			g_hThread_Send = 0;
			return 0;
		}
		else
		{
			Sleep(5);
		}
	}
}


static void StarthThread()
{
	LOG_DEBUG(L"StartThread");
	if(g_hThread_Recv)
	{
		g_iStatus = STATUS_CLOSE_THREAD;
		Sleep(1000);
		CloseHandle(g_hThread_Recv);
		g_hThread_Recv = 0;
	}
	
	if(g_hThread_Send)
	{
		g_iStatus = STATUS_CLOSE_THREAD;
		Sleep(1000);
		CloseHandle(g_hThread_Send);
		g_hThread_Send = 0;
	}
	
	g_iStatus = STATUS_INITIALIZE;
	g_hThread_Recv = CreateThread(NULL, 0, &sync_rev_thread, NULL, 0, NULL);
	g_hThread_Send = CreateThread( NULL, 0, &sync_send_thread, NULL, 0, NULL );
	
}

int StartSyncPhoto(sConnDeviceInfo* sDeviceData, map<wstring, wstring>* photoData, vector<char*> pictUuids)
{
	LOG_DEBUG(L"StartSyncPhotoThroughATHost");

	bool newConn = true;
	if(!InputParamsResolve(sDeviceData, photoData, pictUuids))
	{
		return CALL_INPUT_PARAM_ERROR;
	}

	if(!g_uuid.empty() && (g_uuid ==sDeviceData->DevInfo->_ios_deviceIdentifier) )
	{
		newConn = false;
	}
	else
	{
		g_uuid = sDeviceData->DevInfo->_ios_deviceIdentifier; 
		newConn = true;
	}

	if(!g_hCon || newConn) 
	{
		if (!GlobalParamsInit())
		{
			GlobalParamsUninit();
			return CALL_DLL_INIT_ERROR;
		}

		if(!GetiTunesInfo())
		{
			GlobalParamsUninit();
			return 	CALL_ITUNES_VERSION_INVAILD;
		}
 
		if(CreatePrefsID())
		{
			GlobalParamsUninit();
			return CALL_AFC_OPEN_ERROR;
		}
		g_hCon = (HANDLE)ConnectATH( g_piTunesPrefsID, g_uuid);
		if (!g_hCon)
		{
			GlobalParamsUninit();
			return CALL_ATH_CONN_ERROR;
		}
		SendPowerAssertion();
		SendPowerAssertion();
		SendConnectionRetain();
		SendHostInfo();		
		StarthThread();
	}
	else
	{
		SendPowerAssertion();
		StarthThread();
	}

	Sleep(20000);

	return CALL_SUCCESS;
}

#endif