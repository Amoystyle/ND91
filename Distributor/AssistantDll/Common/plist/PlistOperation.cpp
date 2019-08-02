#include "stdafx.h"

#include "Common/plist/PlistOperation.h"
#include "CoreFoundation/CoreFoundation.h"
#include "Module/Application/AppData.h"
#include "Common/NDZlib.h"
#include "Module/Common/IosPList/IosPlistHelper.h"
#include "Module/Application/IosAppXmlHelper.h"

#ifdef WIN32
#include <io.h>

typedef void (*FP_CFRelease)(CFTypeRef cf);
typedef CFTypeID (*FP_CFGetTypeID)(CFTypeRef cf);
typedef CFTypeID (*FP_CFDictionaryGetTypeID)(void);
typedef CFTypeID (*FP_CFStringGetTypeID)(void);
typedef CFTypeID (*FP_CFArrayGetTypeID)(void);
typedef CFTypeID (*FP_CFNumberGetTypeID)(void);

typedef CFDataRef (*FP_CFDataCreate) (CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length);
typedef const UInt8 *(*FP_CFDataGetBytePtr)(CFDataRef theData);
typedef CFIndex (*FP_CFDataGetLength)(CFDataRef theData);

typedef CFStringRef (*FP_CFStringCreateWithCString)(CFAllocatorRef alloc, const char *cStr, CFStringEncoding encoding);
typedef CFPropertyListRef (*FP_CFPropertyListCreateWithData)(CFAllocatorRef allocator, CFDataRef data, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error) CF_AVAILABLE(10_6, 4_0);
typedef CFDataRef (*FP_CFPropertyListCreateData)(CFAllocatorRef allocator, CFPropertyListRef propertyList, CFPropertyListFormat format, CFOptionFlags options, CFErrorRef *error) CF_AVAILABLE(10_6, 4_0);
typedef void (*FP_CFDictionarySetValue)(CFMutableDictionaryRef theDict, const void *key, const void *value);
typedef void (*FP_CFDictionaryAddValue)(CFMutableDictionaryRef theDict, const void *key, const void *value);
typedef const void* (*FP_CFDictionaryGetValue)(CFDictionaryRef theDict, const void *key);
typedef const char* (*FP_CFStringGetCStringPtr)(CFStringRef theString, CFStringEncoding encoding);
typedef Boolean (*FP_CFStringGetCString)(CFStringRef theString,  char *buffer, CFIndex bufferSize, CFStringEncoding encoding);
typedef const CFBooleanRef (*FP_kCFBooleanTrue)();
typedef const CFBooleanRef (*FP_kCFBooleanFalse)();
typedef Boolean (*FP_CFDictionaryContainsKey)(CFDictionaryRef theDict, const void *key);
typedef CFErrorRef (*FP_CFErrorCreate)(CFAllocatorRef allocator, CFStringRef domain, CFIndex code, CFDictionaryRef userInfo);
typedef void (*FP_CFDictionaryGetKeysAndValues)(CFDictionaryRef theDict, const void **keys, const void **values);

typedef CFIndex (*FP_CFArrayGetCount)(CFArrayRef theArray);
typedef const void* (*FP_CFArrayGetValueAtIndex)(CFArrayRef theArray, CFIndex idx);
typedef Boolean (*FP_CFNumberGetValue)(CFNumberRef number, CFNumberType theType, void *valuePtr);
typedef CFNumberType (*FP_CFNumberGetType)(CFNumberRef number);

#define DEFINE_FUNCTION_PTR(x) FP_##x p##x = NULL;
#define ASSIGN_FUNCTION_PTR(x) 	p##x = (FP_##x) GetProcAddress(g_dynamicLib, ""#x""); if (! p##x) { g_dynamicLib = NULL; return false; }

DEFINE_FUNCTION_PTR(CFDataCreate);
DEFINE_FUNCTION_PTR(CFRelease);

DEFINE_FUNCTION_PTR(CFGetTypeID);
DEFINE_FUNCTION_PTR(CFStringGetTypeID);
DEFINE_FUNCTION_PTR(CFArrayGetTypeID);
DEFINE_FUNCTION_PTR(CFDictionaryGetTypeID);
DEFINE_FUNCTION_PTR(CFNumberGetTypeID);

DEFINE_FUNCTION_PTR(CFDataGetBytePtr);
DEFINE_FUNCTION_PTR(CFDataGetLength);
DEFINE_FUNCTION_PTR(CFStringCreateWithCString);
DEFINE_FUNCTION_PTR(CFPropertyListCreateWithData);
DEFINE_FUNCTION_PTR(CFPropertyListCreateData);
DEFINE_FUNCTION_PTR(CFDictionarySetValue);
DEFINE_FUNCTION_PTR(CFDictionaryAddValue);
DEFINE_FUNCTION_PTR(CFDictionaryGetValue);
DEFINE_FUNCTION_PTR(CFStringGetCStringPtr);
DEFINE_FUNCTION_PTR(CFStringGetCString);
DEFINE_FUNCTION_PTR(kCFBooleanTrue);
DEFINE_FUNCTION_PTR(kCFBooleanFalse);
DEFINE_FUNCTION_PTR(CFDictionaryContainsKey);
DEFINE_FUNCTION_PTR(CFErrorCreate);
DEFINE_FUNCTION_PTR(CFDictionaryGetKeysAndValues);

DEFINE_FUNCTION_PTR(CFArrayGetCount);
DEFINE_FUNCTION_PTR(CFArrayGetValueAtIndex);
DEFINE_FUNCTION_PTR(CFNumberGetValue);
DEFINE_FUNCTION_PTR(CFNumberGetType);

HINSTANCE g_dynamicLib = NULL;

	#define WF(x) (*p##x)
#else
	#define WF(x) x
#endif

bool PlistOperation::Init(wchar_t* dllPath)
{
#ifdef WIN32
	RETURN_TRUE_IF ( g_dynamicLib );
	RETURN_FALSE_IF ( _waccess(dllPath, 0) );

	g_dynamicLib = LoadLibrary(dllPath);
	if ( NULL == g_dynamicLib)
		return false;	// 加载失败

	ASSIGN_FUNCTION_PTR(CFDataCreate);
	ASSIGN_FUNCTION_PTR(CFRelease);
	ASSIGN_FUNCTION_PTR(CFGetTypeID);
    ASSIGN_FUNCTION_PTR(CFStringGetTypeID);
    ASSIGN_FUNCTION_PTR(CFArrayGetTypeID);
	ASSIGN_FUNCTION_PTR(CFDictionaryGetTypeID);
    ASSIGN_FUNCTION_PTR(CFNumberGetTypeID);

	ASSIGN_FUNCTION_PTR(CFDataGetBytePtr);
	ASSIGN_FUNCTION_PTR(CFDataGetLength);
	ASSIGN_FUNCTION_PTR(CFStringCreateWithCString);
	ASSIGN_FUNCTION_PTR(CFPropertyListCreateWithData);
	ASSIGN_FUNCTION_PTR(CFPropertyListCreateData);
	ASSIGN_FUNCTION_PTR(CFDictionarySetValue);
	ASSIGN_FUNCTION_PTR(CFDictionaryAddValue);
	ASSIGN_FUNCTION_PTR(CFDictionaryGetValue);
	ASSIGN_FUNCTION_PTR(CFStringGetCStringPtr);
	ASSIGN_FUNCTION_PTR(CFStringGetCString);
	ASSIGN_FUNCTION_PTR(kCFBooleanTrue);
	ASSIGN_FUNCTION_PTR(kCFBooleanFalse);
	ASSIGN_FUNCTION_PTR(CFDictionaryContainsKey);
	ASSIGN_FUNCTION_PTR(CFErrorCreate);
	ASSIGN_FUNCTION_PTR(CFDictionaryGetKeysAndValues);

    ASSIGN_FUNCTION_PTR(CFArrayGetCount);
    ASSIGN_FUNCTION_PTR(CFArrayGetValueAtIndex);
    ASSIGN_FUNCTION_PTR(CFNumberGetValue);
    ASSIGN_FUNCTION_PTR(CFNumberGetType);
#endif

	return true;
}

void PlistOperation::Release()
{
	if ( g_dynamicLib )
	{
		FreeLibrary(g_dynamicLib);
		g_dynamicLib = NULL;
	}
}

string PlistOperation::ConvertPlist( string inputPlistData, bool outputBinary )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, "");

	CFDataRef binaryData = WF(CFDataCreate)(
		NULL, (unsigned char*)inputPlistData.c_str(), inputPlistData.size());

	CFPropertyListRef propertyList = WF(CFPropertyListCreateWithData)(NULL, 
		binaryData, kCFPropertyListImmutable, NULL, NULL);
	WF(CFRelease)(binaryData);
	RETURN_VALUE_IF( ! propertyList, "");

	// 将CFPropertyList输出为指定格式
	CFDataRef xmlData = WF(CFPropertyListCreateData)(NULL, propertyList, 
		outputBinary ? kCFPropertyListBinaryFormat_v1_0 : kCFPropertyListXMLFormat_v1_0,
		0, NULL);

	WF(CFRelease)(propertyList);
	RETURN_VALUE_IF( ! xmlData, "" );

	// 数据存放到string
	string result((char*)WF(CFDataGetBytePtr)(xmlData), WF(CFDataGetLength)(xmlData));
	WF(CFRelease)(xmlData);
	return result;
}

CFPropertyListRef ReadPlist(wstring filename )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, NULL);

	string data = CFileOperation::ReadFile(filename, true);

	CFDataRef dr = WF(CFDataCreate)(
		NULL, (unsigned char*)data.c_str(), data.size());
	RETURN_FALSE_IF( ! dr);

	CFPropertyListRef pl = WF(CFPropertyListCreateWithData)(NULL, dr, NULL, NULL, NULL);

	WF(CFRelease)(dr);
	return pl;
}

bool WritePlist( CFPropertyListRef pl, wstring pFilename )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	// 将CFPropertyList输出为指定格式
	CFDataRef dr = WF(CFPropertyListCreateData)(NULL, pl, 
		kCFPropertyListXMLFormat_v1_0, 0, NULL);
	RETURN_FALSE_IF( ! dr);

	CFileOperation::WriteBinFile(pFilename,
		(char*)WF(CFDataGetBytePtr)(dr), WF(CFDataGetLength)(dr));
	WF(CFRelease)(dr);
	return true;
}

void AddDict( CFMutableDictionaryRef md, const char* key, const char* value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, );

	CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
	CFStringRef sv = WF(CFStringCreateWithCString)(NULL, value, kCFStringEncodingUTF8);

	WF(CFDictionarySetValue)(md, sk, sv);

	WF(CFRelease)(sk);
	WF(CFRelease)(sv);
}

void AddDict( CFMutableDictionaryRef md, const char* key, bool value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, );

	CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
	CFBooleanRef sv = (value == true ? (CFBooleanRef)WF(kCFBooleanTrue) : (CFBooleanRef)WF(kCFBooleanFalse));

	WF(CFDictionarySetValue)(md, sk, sv);

	WF(CFRelease)(sk);
	WF(CFRelease)(sv);
}

bool PlistOperation::AddDictPlist( wstring filename, string key, string value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	RETURN_FALSE_IF( ! g_dynamicLib );

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	AddDict((CFMutableDictionaryRef)pl, key.c_str(), value.c_str());

	RETURN_FALSE_IF( ! WritePlist(pl, filename));

	WF(CFRelease)(pl);
	return true;
}

bool PlistOperation::AddDictPlist( wstring filename, string key, bool value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	AddDict((CFMutableDictionaryRef)pl, key.c_str(), value);

	RETURN_FALSE_IF( ! WritePlist(pl, filename));

	WF(CFRelease)(pl);
	return true;
}

bool PlistOperation::AddDictPlist( wstring filename, string key, map<string,string> mapInfos )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	return true;
}

void GetDict( CFDictionaryRef md, const char* key, string& value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, );

	CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
    RETURN_IF( ! sk );

	CFStringRef sv = (CFStringRef)WF(CFDictionaryGetValue)(md, sk);
    if ( ! sv || WF(CFGetTypeID)(sv) != WF(CFStringGetTypeID)() )
    {
        WF(CFRelease)(sk);
        return;
    }
	
	char buffer[128];
	memset(buffer, 0, 128);
	WF(CFStringGetCString)(sv, buffer, 128, kCFStringEncodingUTF8);

	value.assign(buffer);
    WF(CFRelease)(sk);
    WF(CFRelease)(sv);
}

std::wstring GetStringFromDict( CFDictionaryRef dr, const char* key )
{
    RETURN_VALUE_IF ( ! g_dynamicLib || ! dr, L"");

    string s;
    GetDict(dr, key, s);
    return CCodeOperation::UTF_8ToUnicode(s);
}

void GetDict( CFDictionaryRef md, const char* key, CFDictionaryRef& value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib || ! md, );

	CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
	value = (CFDictionaryRef)WF(CFDictionaryGetValue)(md, sk);
}

void GetDict( CFDictionaryRef md, const char* key, bool& value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib || ! md, );

	CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
	CFBooleanRef sv = (CFBooleanRef)WF(CFDictionaryGetValue)(md, sk);   	
	value = (sv == (CFBooleanRef)WF(kCFBooleanTrue) ? true : false);   	
}

bool PlistOperation::GetDictPlist( wstring filename, string key, string& value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	GetDict((CFDictionaryRef)pl, key.c_str(), value);

	WF(CFRelease)(pl);
	return true;
}

bool PlistOperation::GetDictPlist(wstring filename, string key, string subKey, string& value)
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	CFDictionaryRef Dvalue=NULL;
	GetDict((CFDictionaryRef)pl, key.c_str(), Dvalue);
	GetDict( Dvalue, subKey.c_str(), value );

	WF(CFRelease)(pl);
	return true;
}

bool PlistOperation::GetDictPlist( wstring filename, string key, bool& value )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	GetDict((CFDictionaryRef)pl, key.c_str(), value);

	WF(CFRelease)(pl);
	return true;
}

bool PlistOperation::PlistContainsKey( wstring filename, string key )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	Boolean bl = WF(CFDictionaryContainsKey)((CFDictionaryRef)pl, key.c_str());
//	RETURN_FALSE_IF( ! WF(CFDictionaryContainsKey)((CFDictionaryRef)pl, key.c_str()) );

	WF(CFRelease)(pl);
	return true;
}
bool PlistOperation::NDCFDictionaryContainsKey( void* pl, string key )
{
	RETURN_VALUE_IF ( ! g_dynamicLib, false);	
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );
	Boolean bl = WF(CFDictionaryContainsKey)((CFDictionaryRef)pl, key.c_str());
	//	RETURN_FALSE_IF( ! WF(CFDictionaryContainsKey)((CFDictionaryRef)pl, key.c_str()) );

	WF(CFRelease)(pl);
	return true;
}
bool PlistOperation::PlistContainsKey( wstring filename, string key, string subKey )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	CFDictionaryRef Dvalue=NULL;
	GetDict((CFDictionaryRef)pl, key.c_str(), Dvalue);
	RETURN_FALSE_IF( ! WF(CFDictionaryContainsKey)(Dvalue, key.c_str()) );

	WF(CFRelease)(pl);
	return true;

}

bool PlistOperation::GetPlistAllValuesKeys(wstring filename,
                                           vector<string> verDatas,
                                           map<string, string> mapInfos,
                                           map<string,string> mapDatas )
{
    RETURN_VALUE_IF ( ! g_dynamicLib, false);

	CFPropertyListRef pl = ReadPlist(filename);
	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	// 将CFPropertyList输出为指定格式
	CFDataRef xmlData = WF(CFPropertyListCreateData)(NULL, pl, kCFPropertyListBinaryFormat_v1_0,0, NULL);
	WF(CFRelease)(pl);
	RETURN_VALUE_IF( ! xmlData, "" );

	// 数据存放到void*
	void* data = (void*)WF(CFDataGetBytePtr)(xmlData);

//	CCFConverter::GetIosEntitiesData(data, verDatas, mapInfos, mapDatas );

	WF(CFRelease)(xmlData);

	return true;
}

bool PlistOperation::GenerateTempPlist(void* pCFDict)
{
	if ( WritePlist(pCFDict, L"D:\\latestCodes\\PhotoLibrary_new.plist") ) // To be updated
	{
		return true;
	}
	return false;
}

bool PlistOperation::XmlPlistGetKey(wstring filename, string key, string& value)
{
	RETURN_FALSE_IF( ! g_dynamicLib);

	string xmldata = CFileOperation::ReadFile(filename, false);
	CFDataRef dr = WF(CFDataCreate)(
		NULL, (unsigned char*)xmldata.c_str(), xmldata.size());
	RETURN_FALSE_IF( ! dr);

	CFPropertyListRef pl = WF(CFPropertyListCreateWithData)(NULL, dr, NULL, NULL, NULL);

	RETURN_FALSE_IF( ! pl );
	RETURN_FALSE_IF( WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() );

	GetDict((CFDictionaryRef)pl, key.c_str(), value);

	//WF(CFRelease)(pl);
	return true;
}

// UIDeviceFamily (Number or Array - iOS) specifies the underlying hardware
// type on which this application is designed to run.
// The value of this key is usually an integer but it can also be an array
// of integers.
//      1 = (Default) The application runs on iPhone and iPod touch devices.
//      2 = The application runs on iPad devices.
void ReadDeviceFamily(CFDictionaryRef dr, vector<wstring>* pVector) 
{
    RETURN_VALUE_IF ( ! g_dynamicLib, );

    CFStringRef sk = WF(CFStringCreateWithCString)(NULL,
        "UIDeviceFamily", kCFStringEncodingUTF8);
    const void* pDeviceFamily = WF(CFDictionaryGetValue)(dr, sk);
	if(!pDeviceFamily)
		return;

    if ( WF(CFGetTypeID)(pDeviceFamily) == WF(CFArrayGetTypeID)() )
    {
        CFArrayRef ar = (CFArrayRef)pDeviceFamily;
        CFIndex count = WF(CFArrayGetCount)(ar);
        for(int i = 0; i < count; ++ i)
        {
            const void* value = WF(CFArrayGetValueAtIndex)(ar, i);
            if( WF(CFGetTypeID)(value) == WF(CFStringGetTypeID)() )
            {
                char buffer[128];
                memset(buffer, 0, 128);
                WF(CFStringGetCString)((CFStringRef)value, buffer, 128, kCFStringEncodingUTF8);
                pVector->push_back(CCodeOperation::UTF_8ToUnicode(buffer));
            }
            else if( WF(CFGetTypeID)(value) == WF(CFNumberGetTypeID)() )
            {
                int v;
                CFNumberType t = WF(CFNumberGetType)((CFNumberRef)value);
                WF(CFNumberGetValue)((CFNumberRef)value, t, &v);
                wchar_t buffer[16];
                _itow_s(v, buffer, 16, 10);
                pVector->push_back(buffer);
            }
//             WF(CFRelease)(value);
        }
    }

  //  WF(CFRelease)(sk);
  //  WF(CFRelease)(pDeviceFamily);
}

wstring SearchIconFiles(CFDictionaryRef dr, const char* key, wstring ipaFile) 
{
    RETURN_VALUE_IF ( ! g_dynamicLib, L"");

    CFStringRef sk = WF(CFStringCreateWithCString)(NULL, key, kCFStringEncodingUTF8);
    const void* pIcons = WF(CFDictionaryGetValue)(dr, sk);

    wstring retFile;
    if ( pIcons )
    {
        if ( WF(CFGetTypeID)(pIcons) == WF(CFArrayGetTypeID)() )
        {
            CFArrayRef ar = (CFArrayRef)pIcons;
            CFIndex count = WF(CFArrayGetCount)(ar);
            for(int i = 0; i < count; ++ i)
            {
                const void* value = WF(CFArrayGetValueAtIndex)(ar, i);
                if( WF(CFGetTypeID)(value) == WF(CFStringGetTypeID)() )
                {
                    char buffer[128];
                    memset(buffer, 0, 128);
                    WF(CFStringGetCString)((CFStringRef)value, buffer, 128, kCFStringEncodingASCII);
					string name=buffer;
					if(name.find(".")==string::npos)
						name+=".png";
                    if ( NDZlib::Exist(ipaFile.c_str(), name.c_str()) )
                    {
                        retFile = CCodeOperation::GB2312ToUnicode(name);
                        break;
                    }
                }
//                 WF(CFRelease)(value);
            }
        }
        //WF(CFRelease)(pIcons);
    }

   // WF(CFRelease)(sk);
    return retFile;
}

AppData* PlistOperation::GetIpaAppInfo(wstring tempDir, wstring ipaFile)
{
    RETURN_NULL_IF( ! g_dynamicLib );

    if ( * tempDir.rbegin() != '\\' )
        tempDir.push_back('\\');

    RETURN_NULL_IF( ! NDZlib::UnZipFile(ipaFile, tempDir, L"Info.plist", false, L".app"));
    wstring plist = tempDir + L"Info.plist";
    RETURN_NULL_IF( ! CFileOperation::IsFileExist(plist.c_str()));
  
    AppData* pAppData = PlistOperation::ParseIpaAppInfoPlist(plist, ipaFile);
    CFileOperation::DeleteFile(plist);
	RETURN_NULL_IF(!pAppData);
	plist = tempDir + L"iTunesMetadata.plist";
	if(NDZlib::UnZipFile(ipaFile, tempDir, L"iTunesMetadata.plist", false))
	{
		if(CFileOperation::IsFileExist(plist.c_str()))
		{
			IosPlistHelper plistHelper(plist, eXmlType_UTF8);
			XML_NODE_INDEX index;
			index.AddChild(0);
			wstring itemId= plistHelper.SearchKeyValue(index, "itemId");

			if(!itemId.empty())
				pAppData->_itemId = atoi(CCodeOperation::ws2s(itemId).c_str());

			CFileOperation::DeleteFile(plist);
		}
	}

    // 获取91IPA相关账户信息
    RETURN_VALUE_IF(!NDZlib::UnZipFile(ipaFile, tempDir, L"info.ini", false), pAppData);
    wstring infoPath = tempDir + L"info.ini";
    RETURN_VALUE_IF(!CFileOperation::IsFileExist(infoPath.c_str()), pAppData);
    string data = CFileOperation::ReadFile(infoPath, true);
    RETURN_VALUE_IF(data.empty(), pAppData);
    pAppData->_is91IPA = true;
    pAppData->_acountInfo = CCodeOperation::UTF_8ToUnicode(data);
    if (pAppData->_acountInfo.length() != data.length())
        pAppData->_acountInfo = pAppData->_acountInfo.substr(1, pAppData->_acountInfo.length());
    CFileOperation::DeleteFile(infoPath);

	

    return pAppData;
}

AppData* PlistOperation::ParseIpaAppInfoPlist(wstring strPath, wstring ipaFile)
{
    CFPropertyListRef pl = ReadPlist(strPath);
    if ( ! pl || WF(CFGetTypeID)(pl) != WF(CFDictionaryGetTypeID)() )
    {
        CFileOperation::DeleteFile(strPath);
    }

    CFDictionaryRef dr = (CFDictionaryRef)pl;
    AppData* pAppData = new AppData();
    pAppData->_strAppPCPath = ipaFile;
    pAppData->_eAppType = APP_TYPE_IPA;// CFBundlePackageType

	 pAppData->_strExecutable = GetStringFromDict(dr, "CFBundleExecutable");

	 if ( pAppData->_strExecutable.empty() )
	 {
		 pAppData->_strExecutable = GetStringFromDict(dr, "CFBundleName");
	 }

    pAppData->_AppName = GetStringFromDict(dr, "CFBundleDisplayName");
    if ( pAppData->_AppName.empty() )
    {
        pAppData->_AppName = GetStringFromDict(dr, "CFBundleName");
    }

    pAppData->_sdkVer = GetStringFromDict(dr, "DTPlatformVersion");
    pAppData->_packageName = GetStringFromDict(dr, "CFBundleIdentifier");

    pAppData->_strVersion = GetStringFromDict(dr, "CFBundleShortVersionString");
    if ( pAppData->_strVersion.empty() )
    {
        pAppData->_strVersion = GetStringFromDict(dr, "CFBundleVersion");
    }

    pAppData->_strIconPhonePath = GetStringFromDict(dr, "CFBundleIconFile");
    if ( pAppData->_strIconPhonePath.empty() )
    {
        //string zipFileName = CCodeOperation::UnicodeToGB2312(ipaFile);

        pAppData->_strIconPhonePath = SearchIconFiles(dr, "Icon files", ipaFile);
        if ( pAppData->_strIconPhonePath.empty() )
        {
            pAppData->_strIconPhonePath = SearchIconFiles(dr, "CFBundleIconFiles", ipaFile);
			if(pAppData->_strIconPhonePath.empty())
			{
				CFDictionaryRef Dvalue=NULL;
				GetDict(dr,"CFBundleIcons",Dvalue);
				if(Dvalue)
				{
					CFDictionaryRef DPrimaryIcon=NULL;
					GetDict(Dvalue,"CFBundlePrimaryIcon",DPrimaryIcon);
					if(DPrimaryIcon)
					{
						pAppData->_strIconPhonePath = SearchIconFiles(DPrimaryIcon, "CFBundleIconFiles", ipaFile);
					}
				}
			}
        }
    }

    pAppData->_minimumOSVersion = GetStringFromDict(dr, "MinimumOSVersion");

    ReadDeviceFamily(dr, &pAppData->_DeviceFamily);

    //WF(CFRelease)(pl);
    return pAppData;
}
