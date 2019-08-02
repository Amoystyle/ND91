//#include "stdafx.h"
#include "Common/MyFileOperation.h"
#include "Device/IosPublicFunc.h"
#include "Common/CodeOperation.h"
#include "Common/FileOperation.h"
#include "common/Log.h"
using namespace ND91Assistant;
#pragma warning(disable:4748)

#include "CFConverter.h"
#include "Device/IosPublicFunc.h"
#include "Device/iPhoneInterfaceDef.h"
#include "Device/ITunesMobileDevice.h"
#include "Common/plist/PlistOperation.h"
#include "Core/DeviceManager.h"
#include "Common/Path.h"

#pragma region 函数指针初始化为NULL
HINSTANCE CCFConverter::m_ndiPhoneUSB = NULL;
CFDataCreate CCFConverter::m_CFDataCreate = NULL;
CFStringCreateFromExternalRepresentation CCFConverter::m_CFStringCreateFromExternalRepresentation = NULL;
CFRelease CCFConverter::m_CFRelease = NULL;
CFArrayAppendValue CCFConverter::m_CFArrayAppendValue = NULL;
CFDictionaryCreate CCFConverter::m_CFDictionaryCreate = NULL;
CFDictionaryCreateCopy CCFConverter::m_CFDictionaryCreateCopy = NULL;
CFArrayCreateMutable CCFConverter::m_CFArrayCreateMutable = NULL;
CFDictionaryCreateMutable CCFConverter::m_CFDictionaryCreateMutable = NULL;
CFDictionaryAddValue CCFConverter::m_CFDictionaryAddValue = NULL;
CFDictionarySetValue CCFConverter::m_CFDictionarySetValue = NULL;
CFDictionaryGetValue CCFConverter::m_CFDictionaryGetValue = NULL;
CFNumberCreate CCFConverter::m_CFNumberCreate = NULL;
CFTimeZoneCopySystem CCFConverter::m_CFTimeZoneCopySystem = NULL;
CFGregorianDateGetAbsoluteTime CCFConverter::m_CFGregorianDateGetAbsoluteTime = NULL;
CFDateCreate CCFConverter::m_CFDateCreate = NULL;
CFArrayGetCount CCFConverter::m_CFArrayGetCount = NULL;
CFArrayGetValues CCFConverter::m_CFArrayGetValues = NULL;
CFDictionaryGetCount CCFConverter::m_CFDictionaryGetCount = NULL;
CFDictionaryGetKeysAndValues CCFConverter::m_CFDictionaryGetKeysAndValues = NULL;
CFGetTypeID CCFConverter::m_CFGetTypeID = NULL;
CFStringGetLength CCFConverter::m_CFStringGetLength = NULL;
CFStringGetBytes CCFConverter::m_CFStringGetBytes = NULL;
CFStringGetTypeID CCFConverter::m_CFStringGetTypeID = NULL;
CFNumberGetTypeID CCFConverter::m_CFNumberGetTypeID = NULL;
CFBooleanGetTypeID CCFConverter::m_CFBooleanGetTypeID = NULL;
CFDateGetTypeID CCFConverter::m_CFDateGetTypeID = NULL;
CFDataGetTypeID CCFConverter::m_CFDataGetTypeID = NULL;
CFNullGetTypeID CCFConverter::m_CFNullGetTypeID = NULL;
CFBagGetTypeID CCFConverter::m_CFBagGetTypeID = NULL;
CFSetGetTypeID CCFConverter::m_CFSetGetTypeID = NULL;
CFTreeGetTypeID CCFConverter::m_CFTreeGetTypeID = NULL;
CFDictionaryGetTypeID CCFConverter::m_CFDictionaryGetTypeID = NULL;
CFArrayGetTypeID CCFConverter::m_CFArrayGetTypeID = NULL;
CFDateGetAbsoluteTime CCFConverter::m_CFDateGetAbsoluteTime = NULL;
CFAbsoluteTimeGetGregorianDate CCFConverter::m_CFAbsoluteTimeGetGregorianDate = NULL;
CFDataGetLength CCFConverter::m_CFDataGetLength = NULL;
CFDataGetBytePtr CCFConverter::m_CFDataGetBytePtr = NULL;
CFNumberGetType CCFConverter::m_CFNumberGetType = NULL;
CFNumberGetValue CCFConverter::m_CFNumberGetValue = NULL;
CFDataCreateMutable CCFConverter::m_CFDataCreateMutable = NULL;
CFDataAppendBytes CCFConverter::m_CFDataAppendBytes = NULL;
CFURLWriteDataAndPropertiesToResource CCFConverter::m_CFURLWriteDataAndPropertiesToResource = NULL;

kCFBooleanTrue CCFConverter::m_kCFBooleanTrue = NULL;
kCFBooleanFalse CCFConverter::m_kCFBooleanFalse = NULL;
kCFAllocatorSystemDefault CCFConverter::m_kCFAllocatorSystemDefault = NULL;
kCFAllocatorNull CCFConverter::m_kCFAllocatorNull = NULL;

CFPropertyListCreateData CCFConverter::m_CFPropertyListCreateData = NULL;
CFDataCreateWithBytesNoCopy CCFConverter::m_CFDataCreateWithBytesNoCopy = NULL;
CFPropertyListCreateFromXMLData CCFConverter::m_CFPropertyListCreateFromXMLData = NULL;
CFPropertyListCreateXMLData CCFConverter::m_CFPropertyListCreateXMLData = NULL;

CFPropertyListCreateWithData CCFConverter::m_CFPropertyListCreateWithData = NULL;

CFGetAllocator CCFConverter::m_CFGetAllocator = NULL;
CFStringCreateWithCString CCFConverter::m_CFStringCreateWithCString = NULL;
__CFStringMakeConstantString CCFConverter::m___CFStringMakeConstantString = NULL;
kCFTypeDictionaryKeyCallBacks CCFConverter::m_kCFTypeDictionaryKeyCallBacks = NULL;
kCFTypeDictionaryValueCallBacks CCFConverter::m_kCFTypeDictionaryValueCallBacks = NULL;
kCFTypeArrayCallBacks CCFConverter::m_kCFTypeArrayCallBacks = NULL;
kCFNumberPositiveInfinity CCFConverter::m_kCFNumberPositiveInfinity = NULL;

#pragma endregion
int G_intTemp = 0;
bool CCFConverter::LoadLib()
{
    if (m_ndiPhoneUSB)
        return true;

    wstring path = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
	wstring dllPath = IosPublicFunc::GetItunesDllRealPath(path, DLLCoreFoundation);
    TCHAR szCoreFoundationDll[MAX_PATH];
    ZeroMemory(szCoreFoundationDll, MAX_PATH*sizeof(TCHAR));
    _tcscat_s(szCoreFoundationDll, MAX_PATH, dllPath.c_str());
    _tcscat_s(szCoreFoundationDll, MAX_PATH, DLLCoreFoundation);

    m_ndiPhoneUSB = ::LoadLibrary(szCoreFoundationDll);
    if (!m_ndiPhoneUSB)
        return false;

	PlistOperation::Init(szCoreFoundationDll);
    return true;
}

void CCFConverter::FreeLib()
{
    if (m_ndiPhoneUSB)
        ::FreeLibrary(m_ndiPhoneUSB);
    m_ndiPhoneUSB = NULL;
}

void* CCFConverter::GetDeviceData(void* pIosDevice, const char* domain, const char* key)
{
    void* p1 = domain ? StringtoCFString(domain) : NULL;
    void* p2 = key ? StringtoCFString(key) : NULL;

    return CITunesMobileDevice::iTunes_AMDeviceCopyValue(pIosDevice, p1, p2);
}

bool CCFConverter::GetIosData(void* pIosData, bool& value)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFBooleanGetTypeID() )
        return false;

    // 取得数据
    value = NDkCFBooleanTrue() == pIosData;

    return true;
}

bool CCFConverter::GetIosData(void* pIosData, time_t& time)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFDateGetTypeID() )
        return false;

    // 取得数据
    double num5 = NDCFDateGetAbsoluteTime(pIosData);
    void* zonePtr = NDCFTimeZoneCopySystem();
    CFGregorianDate dt = NDCFAbsoluteTimeGetGregorianDate(num5, zonePtr);

    CDealTime type((int)dt.year, (int)dt.month, (int)dt.day, (int)dt.hour, (int)dt.minute, (int)dt.second);	
    time = type.GetTime();

    time += 8*3600;
    return true;
}

bool CCFConverter::GetIosData(void* pIosData, std::string& value, bool toGB2312)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFStringGetTypeID() )
        return false;

	// 取得数据
	value = "";
	int nUsedBufferLen = 0;
	int nTextLen = NDCFStringGetLength(pIosData);
	CFRange range(0, nTextLen);

	if (NDCFStringGetBytes(pIosData, range, kCFStringEncodingUTF8, '?', 0, NULL, 1024, &nUsedBufferLen) > 0)
	{
		char* pBuffer = new char[nUsedBufferLen+1];
        memset(pBuffer, 0, nUsedBufferLen+1);

		if (NDCFStringGetBytes(pIosData, range, kCFStringEncodingUTF8, '?', 0, pBuffer, nUsedBufferLen, NULL) > 0)
		{
			 value = pBuffer;
             if (toGB2312)
			    value = CCodeOperation::UTF_8ToGB2312(value);
		}

		delete[] pBuffer;
	}	

	return true;
}

bool CCFConverter::GetIosData(void* pIosData, time_t& time, std::string& timeStr)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFDateGetTypeID() )
        return false;

    // 取得数据
    double num5 = NDCFDateGetAbsoluteTime(pIosData);
    void* zonePtr = NDCFTimeZoneCopySystem();
    CFGregorianDate dt = NDCFAbsoluteTimeGetGregorianDate(num5, zonePtr);

    CDealTime type((int)dt.year, (int)dt.month, (int)dt.day, (int)dt.hour, (int)dt.minute, (int)dt.second);	
    time = type.GetTime();

    if (time != 0)
        time += 8*3600;
    else
    {
        int hour = dt.hour;
        int day = dt.day;
        if (hour+8 >= 24)
            day += 1;
        char temp[128] = {0};
        sprintf(temp,"%d-%d-%d",(int)dt.year,(int)dt.month,day);
        timeStr = temp;
    }

    return true;
}

bool CCFConverter::GetIosData(void* pIosData, vector<string>& arr)
{
	arr.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;

	void** ptr = new void*[nCount];
	CFRange range(0, nCount);
	NDCFArrayGetValues(pIosData, range, ptr);
    for (int i=0; i<nCount; i++)
    {
        string strTemp = "";
        // 检查数据类型
        int type = NDCFGetTypeID(ptr[i]);
        if (type == NDCFNumberGetTypeID())
        {
            int    intTemp = 0;
            GetIosData(ptr[i], intTemp);
            char temp[10] = {0};
            sprintf(temp,"%d",intTemp);
            strTemp = temp;
        }
        else if (type == NDCFStringGetTypeID())
        {
            GetIosData(ptr[i], strTemp);
        }
        else if (type == NDCFDictionaryGetTypeID())
        {
            strTemp = "Entities";
        }
        else if (type == NDCFBooleanGetTypeID())
        {
            bool value = NDkCFBooleanTrue() == ptr[i];
            if (value)
                strTemp = "true";
            else
                strTemp = "false";
        }
        arr.push_back(strTemp);
    }

	if (ptr)
	{
		delete []ptr;
		ptr = NULL;
	}

	return true;
}

bool CCFConverter::GetIosEntitiesData( void* pIosData, vector<string>& arr, map<string, string>& mapIds, map<string, string>& mapEntities )
{
	arr.clear();
	mapIds.clear();
	mapEntities.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;

	void** ptr = new void*[nCount];
	CFRange range(0, nCount);
	NDCFArrayGetValues(pIosData, range, ptr);
	for (int i=0; i<nCount; i++)
	{
		string strTemp = "";
		// 检查数据类型
		int type = NDCFGetTypeID(ptr[i]);
		if (type == NDCFNumberGetTypeID())
		{
			int    intTemp = 0;
			GetIosData(ptr[i], intTemp);
			char temp[10] = {0};
			sprintf(temp,"%d",intTemp);
			strTemp = temp;
		}
		else if (type == NDCFStringGetTypeID())
		{
			GetIosData(ptr[i], strTemp);
		}
		else if (type == NDCFDictionaryGetTypeID())
		{
			strTemp = "Entities";
			int type = NDCFGetTypeID(ptr[i]);
			GetIosEntitiesData(ptr[i], mapIds, mapEntities);
		}
		else if (type == NDCFBooleanGetTypeID())
		{
			bool value = NDkCFBooleanTrue() == ptr[i];
			if (value)
				strTemp = "true";
			else
				strTemp = "false";
		}
		else if (type == NDCFDataGetTypeID())//data数据
		{
			int nLen = CCFConverter::NDCFDataGetLength(ptr[i]);
			char* buf = new char[nLen];
			memcpy( buf, ptr[i], nLen );
			string str( buf, nLen );
			strTemp = str;
		}
		arr.push_back(strTemp);
	}

	if (ptr)
	{
		delete []ptr;
		ptr = NULL;
	}


	return true;
}

bool CCFConverter::GetIosEntitiesData(void* pIosData, vector<string>& arr, map< string, map<string,void*> > &mapEntities, void* devPtr)
{
    arr.clear();
    mapEntities.clear();

    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
        return false;

    int nCount = NDCFArrayGetCount(pIosData);
    if (nCount <= 0)
        return false;

    void** ptr = new void*[nCount];
    CFRange range(0, nCount);
    NDCFArrayGetValues(pIosData, range, ptr);
    for (int i=0; i<nCount; i++)
    {
        string strTemp = "";
        // 检查数据类型
        int type = NDCFGetTypeID(ptr[i]);
        if (type == NDCFNumberGetTypeID())
        {
            int    intTemp = 0;
            GetIosData(ptr[i], intTemp);
            char temp[10] = {0};
            sprintf(temp,"%d",intTemp);
            strTemp = temp;
        }
        else if (type == NDCFStringGetTypeID())
        {
            GetIosData(ptr[i], strTemp);
        }
        else if (type == NDCFDictionaryGetTypeID())
        {
            strTemp = "Entities";
            if ( ! GetIosEntitiesData(ptr[i], mapEntities, devPtr) )
            {
                SAFE_DELETE_ARRAY (ptr);
                return false;
            }
        }
        else if (type == NDCFBooleanGetTypeID())
        {
            bool value = NDkCFBooleanTrue() == ptr[i];
            if (value)
                strTemp = "true";
            else
                strTemp = "false";
        }
        arr.push_back(strTemp);
    }

    if (ptr)
    {
        delete []ptr;
        ptr = NULL;
    }

    return true;
}

bool CCFConverter::GetIosEntitiesData(void* pIosData, vector<string>& arr, map<string, string>& mapIds)
{
    arr.clear();
    mapIds.clear();

    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
        return false;

    int nCount = NDCFArrayGetCount(pIosData);
    if (nCount <= 0)
        return false;

    void** ptr = new void*[nCount];
    CFRange range(0, nCount);
    NDCFArrayGetValues(pIosData, range, ptr);
    for (int i=0; i<nCount; i++)
    {
        string strTemp = "";
        // 检查数据类型
        int type = NDCFGetTypeID(ptr[i]);
        if (type == NDCFNumberGetTypeID())
        {
            int    intTemp = 0;
            GetIosData(ptr[i], intTemp);
            char temp[10] = {0};
            sprintf(temp,"%d",intTemp);
            strTemp = temp;
        }
        else if (type == NDCFStringGetTypeID())
        {
            GetIosData(ptr[i], strTemp);
        }
        else if (type == NDCFDictionaryGetTypeID())
        {
            strTemp = "Entities";
            GetIosData(ptr[i], mapIds);
        }
        else if (type == NDCFBooleanGetTypeID())
        {
            bool value = NDkCFBooleanTrue() == ptr[i];
            if (value)
                strTemp = "true";
            else
                strTemp = "false";
        }
        arr.push_back(strTemp);
    }

    if (ptr)
    {
        delete []ptr;
        ptr = NULL;
    }

    return true;
}

bool CCFConverter::GetIosData(void* pIosData, vector<void*>& arr)
{
	arr.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;
	
	void** ptr = new void*[nCount];
	CFRange range(0, nCount);
	NDCFArrayGetValues(pIosData, range, ptr);
	for (int i=0; i<nCount; i++)
	{
		arr.push_back(ptr[i]);
	}

	if (ptr)
	{
		delete []ptr;
		ptr = NULL;
	}

	return true;
}

bool CCFConverter::GetIosData(void* pIosData, map<std::string, void*>& dict)
{
	dict.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFDictionaryGetTypeID() )
		return false;

	int nCount = NDCFDictionaryGetCount(pIosData);
	if (nCount > 0)
	{
		void** ptrKeys = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		void** ptrVals = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		NDCFDictionaryGetKeysAndValues(pIosData, ptrKeys, ptrVals);
		for (int i=0; i<nCount; i++)
		{
			std::string strKey;

			//map<string, string> dictValue;
			if (GetIosData(ptrKeys[i], strKey))// && GetIosData( ptrVals[i], dictValue))
				dict.insert(pair<std::string, void*>(strKey,  ptrVals[i]));
		}
		free(ptrKeys);
        free(ptrVals);
	}

	return true;
}

bool CCFConverter::GetIosEntitiesData(void* pIosData, map< string, map<string,void*> >& mapEntities, void* devPtr)
{
    //map<string, map<string, void*>> temp;
    string str = "";
    map<string, void*> temp2;
    temp2.clear();
    map<string, void*> temp3;
    temp3.clear();

    map<string, void*> temp1;
    temp1.clear();
    GetIosData(pIosData, temp1);
    for (map<string, void*>::iterator it = temp1.begin(); it != temp1.end(); it++)
    {
        str = it->first;
        temp2.clear();
        GetIosData(it->second, temp2);
        temp3.clear();
        for (map<string,void*>::iterator iter = temp2.begin(); iter != temp2.end(); iter++)
        {
            void* bbb = NULL;
            if (NDCFGetTypeID(iter->second) == NDCFStringGetTypeID())
            {
                string aaa = "";
                GetIosData(iter->second, aaa);
                bbb = StringtoCFString(aaa);
                LOG->WriteDebug(L"GetIosEntitiesData:"+CCodeOperation::UTF_8ToUnicode(aaa));
            }
            else if (NDCFGetTypeID(iter->second) == NDCFDateGetTypeID())
            {
                time_t time = 0;
                string timeStr;
                GetIosData(iter->second, time, timeStr);
                if (time != 0 )
                    bbb = TimeToCFDate(time);
                else
                    bbb = StringtoCFString(timeStr);
            }
            else if (NDCFGetTypeID(iter->second) == NDCFNumberGetTypeID())
            {
                int intTemp = 0;
                GetIosData(iter->second, G_intTemp);
                bbb = Int32ToCFInt32(G_intTemp);
            }
            else if (NDCFGetTypeID(iter->second) == NDCFBooleanGetTypeID())
            {
                bool value = NDkCFBooleanTrue() == iter->second;
                bbb = BoolToCFBool(value);
            }
            else if (NDCFGetTypeID(iter->second) == NDCFArrayGetTypeID())
            {
                vector<string> arr;
                arr.clear();
                GetIosData(iter->second,arr);
                if (arr.size() == 1)
                    bbb = StringtoCFString(arr.at(0));
                else
                    bbb = ConvertStringVector(arr);
            }
            else if (NDCFGetTypeID(iter->second) == NDCFDataGetTypeID())//联系人头像
            {
                vector<void*> temp;
                temp.clear();
                temp.push_back(iter->second);
                bbb = ArrayToCFArray(temp);

                // 取临时目录
                DeviceData* deviceData = DEVICE_MANAGER->FindIosDevice(devPtr);
                if ( ! deviceData )
                    return false;
                wstring photoPath = deviceData->GetPath()->GetLocalPimPhotoPath();
                photoPath += CFileOperation::GetGuidW() + L".png";

                temp.clear();
                CCFConverter::GetIosData(bbb,temp);
                for (vector<void*>::iterator iter1 = temp.begin(); iter1 != temp.end(); iter1++)
                {
                    LOG->WriteDebug(L"GetIosEntitiesData:Start WriteHeadPicture");

                    int nLen = CCFConverter::NDCFDataGetLength(*iter1);
                    LOG->WriteDebug(L"HeadPicture Len:"+CStrOperation::IntToWString(nLen));

                    void* pValue = CCFConverter::NDCFDataGetBytePtr(*iter1);
                    unsigned char* uszValue = new unsigned char[nLen];
                    memset(uszValue, 0, nLen);
                    memcpy(uszValue, pValue, nLen);

                    FILE* f = fopen(CCodeOperation::WstringToString(photoPath).c_str(), "w+b");
                    if (f) 
                    {
                        LOG->WriteDebug(L"HeadPicture Path:"+photoPath);
                        fwrite(uszValue, nLen, 1, f);
                        fflush(f);
                        fclose(f);					
                    }

                    SAFE_DELETE_ARRAY(uszValue);

                    LOG->WriteDebug(L"GetIosEntitiesData:End WriteHeadPicture");
                }
                bbb = StringtoCFString(CCodeOperation::UnicodeToUTF_8(photoPath));
            }
            temp3.insert(pair<string,void*>(iter->first,bbb));
        }
        mapEntities.insert(pair< string, map<string, void*> >(str,temp3));
    }
    
    return true;
}

bool CCFConverter::GetIosEntitiesData( void* pIosData, map<string, string>& mapIds, map<string, string>& mapEntities)
{
	string str ="", strVal="";
	mapIds.clear();
	map<string, void*> temp2;
	temp2.clear();
	mapEntities.clear();

	map<string, void*> temp1;
	temp1.clear();
	GetIosData(pIosData, temp1);
	for (map<string, void*>::iterator it = temp1.begin(); it != temp1.end(); it++)
	{
		str = it->first;
		if (NDCFGetTypeID(it->second) == NDCFStringGetTypeID())
		{
			GetIosData(it->second, strVal);
		}
		else if (NDCFGetTypeID(it->second) == NDCFDateGetTypeID())
		{
			time_t time = 0;
			string timeStr;
			GetIosData(it->second, time, timeStr);
			if (time != 0 )
				strVal = (char*)TimeToCFDate(time);
			else
				strVal = timeStr;
		}
		else if (NDCFGetTypeID(it->second) == NDCFNumberGetTypeID())
		{
			int intTemp = 0;
			GetIosData(it->second, intTemp);
			char temp[10] = {0};
			sprintf(temp,"%d",intTemp);
			strVal = temp;
		}
		else if (NDCFGetTypeID(it->second) == NDCFBooleanGetTypeID())
		{
			bool value = NDkCFBooleanTrue() == it->second;
			strVal = value ? "true" : "false";
		}
		else if (NDCFGetTypeID(it->second) == NDCFArrayGetTypeID())
		{
			vector<string> arr;
			arr.clear();
			GetIosData(it->second,arr);
			if (arr.size() == 1)
				strVal = arr.at(0);
			else
				strVal = (char*)ConvertStringVector(arr);
		}
		else
		{
			temp2.clear();
			GetIosData(it->second, temp2);
			for (map<string,void*>::iterator iter = temp2.begin(); iter != temp2.end(); iter++)
			{
				//void* bbb = NULL;
				string bbb;
				if (NDCFGetTypeID(iter->second) == NDCFStringGetTypeID())
				{
					string aaa = "";
					GetIosData(iter->second, aaa);
					bbb = aaa;
				}
				else if (NDCFGetTypeID(iter->second) == NDCFDateGetTypeID())
				{
					time_t time = 0;
					string timeStr;
					GetIosData(iter->second, time, timeStr);
					if (time != 0 )
						bbb = (char*)TimeToCFDate(time);
					else
						bbb = timeStr;
				}
				else if (NDCFGetTypeID(iter->second) == NDCFNumberGetTypeID())
				{
					int intTemp = 0;
					GetIosData(iter->second, intTemp);
					char intbf[10] = {0};
					sprintf(intbf,"%d",intTemp);
					bbb = intbf;
				}
				else if (NDCFGetTypeID(iter->second) == NDCFBooleanGetTypeID())
				{
					bool value = NDkCFBooleanTrue() == iter->second;
					bbb = value ? "true" : "false";
				}
				else if (NDCFGetTypeID(iter->second) == NDCFArrayGetTypeID())
				{
					vector<string> arr;
					arr.clear();
					GetIosData(iter->second,arr);
					if (arr.size() == 1)
						bbb = arr.at(0);
					else
						bbb = (char*)ConvertStringVector(arr);
				}
				else if (NDCFGetTypeID(iter->second) == NDCFDataGetTypeID())//data数据
				{
					vector<void*> temp;
					temp.clear();
					temp.push_back(iter->second);
					bbb = (char*)ArrayToCFArray(temp);
				}
				mapEntities.insert(pair<string,string>(iter->first,bbb));
			}
		}
		mapIds[str] = strVal;
	}

	return true;
}
bool CCFConverter::GetIosScreenSnapEntitiesData( void* pIosData, vector<string>& arr, wstring screensnappath )
{
	arr.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;

	bool result= false;
	void** ptr = new void*[nCount];
	CFRange range(0, nCount);
	NDCFArrayGetValues(pIosData, range, ptr);
	for (int i=0; i<nCount; i++)
	{
		string strTemp = "";
		// 检查数据类型
		int type = NDCFGetTypeID(ptr[i]);
		 if (type == NDCFStringGetTypeID())
		{
			GetIosData(ptr[i], strTemp);
		}
		else if (type == NDCFDictionaryGetTypeID())
		{
			strTemp = "Entities";
			map<string, void*> mapIds;
			GetIosData(ptr[i], mapIds);
			if(mapIds.find("MessageType")!=mapIds.end()&& mapIds["MessageType"]=="ScreenShotReply")
			{
				if(mapIds.find("ScreenShotData")!=mapIds.end())
				{
					int nLen = CCFConverter::NDCFDataGetLength(mapIds["ScreenShotData"]);
					void* pValue = CCFConverter::NDCFDataGetBytePtr(mapIds["ScreenShotData"]);
					unsigned char* uszValue = new unsigned char[nLen];
					memset(uszValue, 0, nLen);
					memcpy(uszValue, pValue, nLen);
					CFileOperation::WriteBinFile(screensnappath,(char*)uszValue,nLen);
					SAFE_DELETE_ARRAY(uszValue);
					result = true;
				}
			}
		}
		arr.push_back(strTemp);
	}

	delete []ptr;
	ptr = NULL;

	return result;
}

bool CCFConverter::GetIosData(void* pIosData, map<std::string, std::string>& dict)
{
	dict.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFDictionaryGetTypeID() )
		return false;

	int nCount = NDCFDictionaryGetCount(pIosData);
	if (nCount > 0)
	{
		void** ptrKeys = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		void** ptrVals = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		NDCFDictionaryGetKeysAndValues(pIosData, ptrKeys, ptrVals);
		for (int i=0; i<nCount; i++)
		{
			std::string strKey;
			std::string strValue;
			if (GetIosData(ptrKeys[i], strKey) && GetIosData( ptrVals[i], strValue))
				dict.insert(pair<std::string, std::string>(strKey, strValue));
		}
		free(ptrKeys);
		free(ptrVals);
	}

	return true;
}

bool CCFConverter::GetIosData(void* pIosData, map<std::string, bool>& dict)
{
	dict.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFDictionaryGetTypeID() )
		return false;

	int nCount = NDCFDictionaryGetCount(pIosData);
	if (nCount > 0)
	{
		void** ptrKeys = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		void** ptrVals = (void**)malloc(sizeof(void*)*nCount);//new void*[nCount];
		NDCFDictionaryGetKeysAndValues(pIosData, ptrKeys, ptrVals);
		for (int i=0; i<nCount; i++)
		{
			std::string strKey;
			bool bValue;
			if (GetIosData(ptrKeys[i], strKey) && GetIosData( ptrVals[i], bValue))
				dict.insert(pair<std::string, bool>(strKey, bValue));
		}
		free(ptrKeys);
		free(ptrVals);
	}

	return true;
}

bool CCFConverter::GetIosData( void* pIosData, vector<char>& buf )
{
	if (!pIosData || NDCFGetTypeID(pIosData) != NDCFDataGetTypeID())
		return false;
	int nLen = NDCFDataGetLength(pIosData);
	void* pData = NDCFDataGetBytePtr(pIosData);
	//buf.resize(nLen);
	//memcpy(buf.begin()._Myptr, pData, nLen);
    char* uszValue = (char*)pData;
    for (int i = 0; i < nLen; ++i, ++uszValue)
        buf.push_back(*uszValue);

	return true;
}

void* CCFConverter::BoolToCFBool(bool bValue)
{
    if (bValue)
        return NDkCFBooleanTrue();
    return NDkCFBooleanFalse();
}

void* CCFConverter::ArrayToCFArray(vector<void*> srcArray)
{
	if (srcArray.empty())
		return NULL;

	int nCount = srcArray.size();
	void* ptr = NDCFArrayCreateMutable(NULL, nCount, NULL);
	for (int i=0; i<nCount; i++)
		NDCFArrayAppendValue(ptr, srcArray.at(i));
	
	return ptr;
}

void* CCFConverter::TimeToCFDate(time_t time)
{
	CDealTime dt(time);
	CFGregorianDate cfdate;
	cfdate.year = dt.GetYear();
	cfdate.month = (unsigned char)dt.GetMonth();
	cfdate.day = (unsigned char)dt.GetDay();
	cfdate.hour = (unsigned char)dt.GetHour();
	cfdate.minute = (unsigned char)dt.GetMinute();
	cfdate.second = dt.GetSecond();
	
	void* zonePtr = NDCFTimeZoneCopySystem();
	double num = NDCFGregorianDateGetAbsoluteTime(cfdate, zonePtr);
	void* voidPtr = NDCFDateCreate(NULL, num);
	
	return voidPtr;
}

void* CCFConverter::Int8ToCFInt8(INT8 num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt8Type, (void*)&num);
}

void* CCFConverter::Int16ToCFInt16(INT16 num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt16Type, (void*)&num);
}

void* CCFConverter::Int32ToCFInt32(INT32 num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt32Type, (void*)&num);
}

void* CCFConverter::Int64ToCFInt64(INT64 num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt64Type, (void*)&num);
}

void* CCFConverter::StringtoCFString(string cStr, bool change)
{
    string utf8str = cStr;
    if (change)
	    utf8str = CCodeOperation::GB2312ToUTF_8(cStr);
	unsigned char * pBuffer = new unsigned char[utf8str.length() + 1];
    memset(pBuffer, 0, utf8str.length() + 1);
	memcpy(pBuffer, utf8str.c_str(), utf8str.length());

    void* dataPtr = NDCFDataCreate(NULL, pBuffer, utf8str.length());
	void* voidPtr = NDCFStringCreateFromExternalRepresentation(NULL, dataPtr, kCFStringEncodingUTF8);

	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}

    NDCFRelease(dataPtr);

	return voidPtr;
}

void* CCFConverter::ConvertStringVector(vector<string> source)
{
    if (source.empty())
        return NULL;

    int nCount = source.size();
    void* ptr = NDCFArrayCreateMutable(NULL, nCount, NULL);

    for (int i = 0; i < nCount; i++)
    {
        string temp = source[i];
        if ((int)temp.find(" ") < 0 && atoi(source[i].c_str()) > 0)// 字符串没空格并且转数字后大于0，认为是数字
            NDCFArrayAppendValue(ptr, Int32ToCFInt32(atoi(source[i].c_str())));
        else
            NDCFArrayAppendValue(ptr, StringtoCFString(source[i]));
    }

    return ptr;
}

void* CCFConverter::ConvertStringVectorMap(map< string, vector<string> > source)
{
    RETURN_NULL_IF(source.empty());

    void* pCFDict = NDCFDictionaryCreateMutable(NULL, source.size(), NULL, NULL);

    map< string, vector<string> >::iterator it;
    for (it = source.begin(); it != source.end(); it++)
    {
        void* pKey = StringtoCFString(it->first);
        void* pVaule = ConvertStringVector(it->second);

        NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
    }

    return pCFDict;
}

void* CCFConverter::ConvertStringVectorMap(map< string, map<string,void*> > source)
{
    RETURN_NULL_IF(source.empty());

    void* pCFDict = NDCFDictionaryCreateMutable(NULL, source.size(), NULL, NULL);

    map< string, map<string,void*> >::iterator it;
    for (it = source.begin(); it != source.end(); it++)
    {
        void* pKey = StringtoCFString(it->first);
        void* pVaule = DictionarytoCFDictionary(it->second);

        NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
    }

    return pCFDict;
}

void* CCFConverter::ConvertStringVectorMap(map< string, vector<void*> > source)
{
    RETURN_NULL_IF(source.empty());

    void* pCFDict = NDCFDictionaryCreateMutable(NULL, source.size(), NULL, NULL);

    map< string, vector<void*> >::iterator it;
    for (it = source.begin(); it != source.end(); it++)
    {
        void* pKey = StringtoCFString(it->first);
        void* pVaule = NULL;
        if (NDCFGetTypeID(it->second.at(0)) == NDCFBooleanGetTypeID())
            pVaule = BoolToCFBool((bool&)it->second.at(0));
        else 
            pVaule = ArrayToCFArray(it->second);

        NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
    }

    return pCFDict;
}

void* CCFConverter::DictionarytoCFDictionary(map<string, string> srcDict)
{
	void* pCFDict = NULL;

	if (srcDict.size() > 0)
	{
		pCFDict = NDCFDictionaryCreateMutable(NULL, srcDict.size(), NULL, NULL);
		map<string, string>::iterator it;
		for (it=srcDict.begin(); it!=srcDict.end(); it++)
		{
			void* pKey = StringtoCFString(it->first);
			void* pVaule = StringtoCFString(it->second);
			NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
		}
	}

	return pCFDict;
}

void* CCFConverter::DictionarytoCFDictionary(map<string, void*> srcDict)
{
    RETURN_NULL_IF(srcDict.empty());

    void* pCFDict = NDCFDictionaryCreateMutable(NULL, srcDict.size(), NULL, NULL);
    for (map<string, void*>::iterator it = srcDict.begin(); it != srcDict.end(); it++)
        NDCFDictionaryAddValue(pCFDict, StringtoCFString(it->first), it->second);

    return pCFDict;
}

void CCFConverter::ClearMap(map<string, void*>& mapInfos)
{
    map<string, void*>::iterator it;
    for (it = mapInfos.begin(); it != mapInfos.end(); it++)
    {
        if (it->second)
            NDCFRelease(it->second);
        it->second = NULL;
    }
    mapInfos.clear();
}

void CCFConverter::ClearVec(vector<void*>& vecInfo)
{
    for (size_t i = 0; i < vecInfo.size(); i++)
    {
        if (vecInfo.at(i))
            NDCFRelease(vecInfo.at(i));
        vecInfo.at(i) = NULL;
    }
    vecInfo.clear();
}

void CCFConverter::ClearMapRecursive(map<string, void*>& mapInfos)
{
    map<string, void*>::iterator it;
    for (it = mapInfos.begin(); it != mapInfos.end(); it++)
    {
        int type = NDCFGetTypeID(it->second);
        if (type == NDCFDictionaryGetTypeID())
        {
            map<string, void*> subMapInfo;
            GetIosData(it->second, subMapInfo);
            ClearMapRecursive(subMapInfo);
        }
        else if (type == NDCFArrayGetTypeID())    
        {
            vector<void*> subVecInfo;
            GetIosData(it->second, subVecInfo);
            ClearVecRecursive(subVecInfo);
        }

        NDCFRelease(it->second);
        it->second = NULL;
    }
    mapInfos.clear();
}

void CCFConverter::ClearVecRecursive(vector<void*>& vecInfo)
{
    for (size_t i = 0; i < vecInfo.size(); i++)
    {
        if (vecInfo.at(i) == NULL)
            continue;

        int type = NDCFGetTypeID(vecInfo.at(i));
        if (type == NDCFDictionaryGetTypeID())
        {
            map<string, void*> subMapInfo;
            GetIosData(vecInfo.at(i), subMapInfo);
            ClearMapRecursive(subMapInfo);
        }
        else if (type == NDCFArrayGetTypeID())    
        {
            vector<void*> subVecInfo;
            GetIosData(vecInfo.at(i), subVecInfo);
            ClearVecRecursive(subVecInfo);
        }

        NDCFRelease(vecInfo.at(i));
        vecInfo.at(i) = NULL;
    }
    vecInfo.clear();
}

string CCFConverter::ConvertPlist( string inputPlistData, bool outputBinary )
{
    // 生成CFDataRef
    void* cfBinaryData = CCFConverter::NDCFDataCreate(
        NULL, (unsigned char*)inputPlistData.c_str(), inputPlistData.size());

    // 生成CFPropertyList
    void* cfPropertyList = CCFConverter::NDCFPropertyListCreateWithData
        (cfBinaryData, kCFPropertyListImmutable);
    CCFConverter::NDCFRelease(cfBinaryData);
    RETURN_VALUE_IF( ! cfPropertyList, "");

    // 将CFPropertyList输出为指定格式
    void* cfXmlData = CCFConverter::NDCFPropertyListCreateData
        (NULL, cfPropertyList, 
        outputBinary ? kCFPropertyListBinaryFormat_v1_0 : kCFPropertyListXMLFormat_v1_0,
        0, NULL);

    CCFConverter::NDCFRelease(cfPropertyList);
    RETURN_VALUE_IF( ! cfXmlData, "" );

    // 数据存放到string
    string result((char*)
        CCFConverter::NDCFDataGetBytePtr(cfXmlData),
        CCFConverter::NDCFDataGetLength(cfXmlData));
    CCFConverter::NDCFRelease(cfXmlData);

    return result;
}

string CCFConverter::ConvertPlist( void* cfBinaryData, bool outputBinary )
{

    void* cfXmlData = CCFConverter::NDCFPropertyListCreateData
        (NULL, cfBinaryData, 
        outputBinary ? kCFPropertyListBinaryFormat_v1_0 : kCFPropertyListXMLFormat_v1_0,
        0, NULL);

    RETURN_VALUE_IF( ! cfXmlData, "" );

    // 数据存放到string
    string result((char*)
        CCFConverter::NDCFDataGetBytePtr(cfXmlData),
        CCFConverter::NDCFDataGetLength(cfXmlData));
    CCFConverter::NDCFRelease(cfXmlData);

    return result;
}

void* CCFConverter::NDCFDataCreate(void* allocator, unsigned char* buf, int count)
{
	if (!LoadLib())
		return NULL;

    m_CFDataCreate = (CFDataCreate)GetProcAddress(m_ndiPhoneUSB, "CFDataCreate");
	if (!m_CFDataCreate)
		return NULL;

	try {
		return m_CFDataCreate(allocator, buf, count);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFStringCreateFromExternalRepresentation(void* allocator, void* data, unsigned __int32 count)
{
	if (!LoadLib())
		return NULL;
    m_CFStringCreateFromExternalRepresentation = (CFStringCreateFromExternalRepresentation)GetProcAddress(m_ndiPhoneUSB, "CFStringCreateFromExternalRepresentation");
	if (!m_CFStringCreateFromExternalRepresentation)
		return NULL;
	try {
		return m_CFStringCreateFromExternalRepresentation(allocator, data, count);
	}
	catch(...)
	{
		return NULL;
	}
}

void  CCFConverter::NDCFRelease(void* cf)
{
	if (!LoadLib())
		return ;
    m_CFRelease = (CFRelease)GetProcAddress(m_ndiPhoneUSB, "CFRelease");
	if (!m_CFRelease)
		return ;
	try {
		return m_CFRelease(cf);
	}
	catch(...)
	{
		return;
	}
}

void CCFConverter::NDCFArrayAppendValue(void* cfArray, void* value)
{
	if (!LoadLib())
		return ;
    m_CFArrayAppendValue = (CFArrayAppendValue)GetProcAddress(m_ndiPhoneUSB, "CFArrayAppendValue");
	if (!m_CFArrayAppendValue)
		return ;
	try {
		return m_CFArrayAppendValue(cfArray, value);
	}
	catch(...)
	{
		return;
	}
}

void* CCFConverter::NDCFDictionaryCreate(void* allocator, void** ptrKeys, void** ptrVal, int count, void* callbackKey, void* callbackValueAs)
{
	if (!LoadLib())
		return NULL;
    m_CFDictionaryCreate = (CFDictionaryCreate)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryCreate");
	if (!m_CFDictionaryCreate)
		return NULL;
	try {
		return m_CFDictionaryCreate(allocator, ptrKeys, ptrVal, count, callbackKey, callbackValueAs);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFDictionaryCreateCopy(void* allocator, void* cfDict)
{
	if (!LoadLib())
		return NULL;
	m_CFDictionaryCreateCopy = (CFDictionaryCreateCopy)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryCreateCopy");
	if (!m_CFDictionaryCreateCopy)
		return NULL;
	try {
		return m_CFDictionaryCreateCopy(allocator, cfDict);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFArrayCreateMutable(void* allocator, int size, void* callback)
{
	if (!LoadLib())
		return NULL;
    m_CFArrayCreateMutable = (CFArrayCreateMutable)GetProcAddress(m_ndiPhoneUSB, "CFArrayCreateMutable");
	if (!m_CFArrayCreateMutable)
		return NULL;
	try {
		return m_CFArrayCreateMutable(allocator, size, callback);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFDictionaryCreateMutable(void* allocator, int size, void* callbackKey, void* callbackValue)
{
	if (!LoadLib())
		return NULL;
    m_CFDictionaryCreateMutable = (CFDictionaryCreateMutable)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryCreateMutable");
	if (!m_CFDictionaryCreateMutable)
		return NULL;

	try {
		return m_CFDictionaryCreateMutable(allocator, size, callbackKey, callbackValue);
	}
	catch(...)
	{
		return NULL;
	}
}

void CCFConverter::NDCFDictionaryAddValue(void* cfDict, void* key, void* value)
{
	if (!LoadLib())
		return ;
    m_CFDictionaryAddValue = (CFDictionaryAddValue)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryAddValue");
	if (!m_CFDictionaryAddValue)
		return ;

	try {
		return m_CFDictionaryAddValue(cfDict, key, value);
	}
	catch(...)
	{
		return ;
	}
}

void CCFConverter::NDCFDictionarySetValue(void* cfDict, void* key, void* value)
{
	if (!LoadLib())
		return ;
	m_CFDictionarySetValue = (CFDictionarySetValue)GetProcAddress(m_ndiPhoneUSB, "CFDictionarySetValue");
	if (!m_CFDictionarySetValue)
		return ;
	try {
		return m_CFDictionarySetValue(cfDict, key, value);
	}
	catch(...)
	{
		return;
	}
}

void* CCFConverter::NDCFNumberCreate(void* allocator, CFNumberType theType, void *valuePtr)
{
	if (!LoadLib())
		return NULL;
    m_CFNumberCreate = (CFNumberCreate)GetProcAddress(m_ndiPhoneUSB, "CFNumberCreate");
	if (!m_CFNumberCreate)
		return NULL;
	try{
		return m_CFNumberCreate(allocator, theType, valuePtr);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFTimeZoneCopySystem()
{
	if (!LoadLib())
		return NULL;
    m_CFTimeZoneCopySystem = (CFTimeZoneCopySystem)GetProcAddress(m_ndiPhoneUSB, "CFTimeZoneCopySystem");
	if (!m_CFTimeZoneCopySystem)
		return NULL;
	try{
		return m_CFTimeZoneCopySystem();
	}
	catch(...)
	{
		return NULL;
	}
}

double CCFConverter::NDCFGregorianDateGetAbsoluteTime(CFGregorianDate dt, void* timezone)
{
	if (!LoadLib())
		return -1;
    m_CFGregorianDateGetAbsoluteTime = (CFGregorianDateGetAbsoluteTime)GetProcAddress(m_ndiPhoneUSB, "CFGregorianDateGetAbsoluteTime");
	if (!m_CFGregorianDateGetAbsoluteTime)
		return -1;
	try{
		return m_CFGregorianDateGetAbsoluteTime(dt, timezone);
	}
	catch(...)
	{
		return -1;
	}
}

void* CCFConverter::NDCFDateCreate(void* allocator, double timetick)
{
	if (!LoadLib())
		return NULL;
    m_CFDateCreate = (CFDateCreate)GetProcAddress(m_ndiPhoneUSB, "CFDateCreate");
	if (!m_CFDateCreate)
		return NULL;
	try{
		return m_CFDateCreate(allocator, timetick);
	}
	catch(...)
	{
		return NULL;
	}
}

int CCFConverter::NDCFArrayGetCount(void* cfArray)
{
	if (!LoadLib())
		return -1;
    m_CFArrayGetCount = (CFArrayGetCount)GetProcAddress(m_ndiPhoneUSB, "CFArrayGetCount");
	if (!m_CFArrayGetCount)
		return -1;
	try{
		return m_CFArrayGetCount(cfArray);
	}
	catch(...)
	{
		return -1;
	}
}

void CCFConverter::NDCFArrayGetValues(void* cfarray, CFRange range, void** ret)
{
	if (!LoadLib())
		return ;
    m_CFArrayGetValues = (CFArrayGetValues)GetProcAddress(m_ndiPhoneUSB, "CFArrayGetValues");
	if (!m_CFArrayGetValues)
		return ;
	try{
		return m_CFArrayGetValues(cfarray, range, ret);
	}
	catch(...)
	{
		return ;
	}
}

int CCFConverter::NDCFDictionaryGetCount(void* cfDict)
{
	if (!LoadLib())
		return -1;
    m_CFDictionaryGetCount = (CFDictionaryGetCount)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryGetCount");
	if (!m_CFDictionaryGetCount)
		return -1;
	try{
		return m_CFDictionaryGetCount(cfDict);
	}
	catch(...)
	{
		return -1;
	}
}

void CCFConverter::NDCFDictionaryGetKeysAndValues(void* cfDict, void** ptrKey, void** ptrVal)
{
	if (!LoadLib())
		return ;
    m_CFDictionaryGetKeysAndValues = (CFDictionaryGetKeysAndValues)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryGetKeysAndValues");
	if (!m_CFDictionaryGetKeysAndValues)
		return ;

		try{
			return m_CFDictionaryGetKeysAndValues(cfDict, ptrKey, ptrVal);
		}
		catch(...)
		{
			return ;
		}
}

int CCFConverter::NDCFGetTypeID(void* ptr)
{
	if (!LoadLib())
		return -1;
    m_CFGetTypeID = (CFGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFGetTypeID");
	if (!m_CFGetTypeID)
		return -1;
	try{
		return m_CFGetTypeID(ptr);
	}
	catch(...)
	{
		return -1;
	}
}

int CCFConverter::NDCFStringGetLength(void* cfString)
{
	if (!LoadLib())
		return -1;
    m_CFStringGetLength = (CFStringGetLength)GetProcAddress(m_ndiPhoneUSB, "CFStringGetLength");
	if (!m_CFStringGetLength)
		return -1;
	try{
		return m_CFStringGetLength(cfString);
	}
	catch(...)
	{
		return -1;
	}
}

int CCFConverter::NDCFStringGetBytes(void* cfStr, CFRange range, unsigned __int32 ukn1, unsigned char ukn2, unsigned char ukn3, char* buf, int bufCount, void* ukn4)
{
	if (!LoadLib())
		return -1;
    m_CFStringGetBytes = (CFStringGetBytes)GetProcAddress(m_ndiPhoneUSB, "CFStringGetBytes");
	if (!m_CFStringGetBytes)
		return -1;
	try{
		return m_CFStringGetBytes(cfStr, range, ukn1, ukn2, ukn3, buf, bufCount, ukn4);
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFStringGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFStringGetTypeID = (CFStringGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFStringGetTypeID");
	if (!m_CFStringGetTypeID)
		return 0;
	try{
		return m_CFStringGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFNumberGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFNumberGetTypeID = (CFNumberGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFNumberGetTypeID");
	if (!m_CFNumberGetTypeID)
		return 0;
	try{
		return m_CFNumberGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFBooleanGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFBooleanGetTypeID = (CFBooleanGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFBooleanGetTypeID");
	if (!m_CFBooleanGetTypeID)
		return 0;
	try{
		return m_CFBooleanGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFDateGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFDateGetTypeID = (CFDateGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFDateGetTypeID");
	if (!m_CFDateGetTypeID)
		return 0;

	try{
		return m_CFDateGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFDataGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFDataGetTypeID = (CFDataGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFDataGetTypeID");
	if (!m_CFDataGetTypeID)
		return 0;

	try{
		return m_CFDataGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFNullGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFNullGetTypeID = (CFNullGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFNullGetTypeID");
	if (!m_CFNullGetTypeID)
		return 0;
	try{
		return m_CFNullGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFBagGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFBagGetTypeID = (CFBagGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFBagGetTypeID");
	if (!m_CFBagGetTypeID)
		return 0;
	try{
		return m_CFBagGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFSetGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFSetGetTypeID = (CFSetGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFSetGetTypeID");
	if (!m_CFSetGetTypeID)
		return 0;
	try{
		return m_CFSetGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFTreeGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFTreeGetTypeID = (CFTreeGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFTreeGetTypeID");
	if (!m_CFTreeGetTypeID)
		return 0;
	try{
		return m_CFTreeGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFDictionaryGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFDictionaryGetTypeID = (CFDictionaryGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryGetTypeID");
	if (!m_CFDictionaryGetTypeID)
		return 0;
	try{
		return m_CFDictionaryGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

unsigned __int32 CCFConverter::NDCFArrayGetTypeID()
{
	if (!LoadLib())
		return 0;
    m_CFArrayGetTypeID = (CFArrayGetTypeID)GetProcAddress(m_ndiPhoneUSB, "CFArrayGetTypeID");
	if (!m_CFArrayGetTypeID)
		return 0;
	try{
		return m_CFArrayGetTypeID();
	}
	catch(...)
	{
		return -1;
	}
}

double CCFConverter::NDCFDateGetAbsoluteTime(void* cfDate)
{
	if (!LoadLib())
		return -1;
    m_CFDateGetAbsoluteTime = (CFDateGetAbsoluteTime)GetProcAddress(m_ndiPhoneUSB, "CFDateGetAbsoluteTime");
	if (!m_CFDateGetAbsoluteTime)
		return -1;
	try{
		return m_CFDateGetAbsoluteTime(cfDate);
	}
	catch(...)
	{
		return -1;
	}
}

CFGregorianDate CCFConverter::NDCFAbsoluteTimeGetGregorianDate(double tick, void* cfTimezone)
{
	if (!LoadLib())
		return CFGregorianDate();
    m_CFAbsoluteTimeGetGregorianDate = (CFAbsoluteTimeGetGregorianDate)GetProcAddress(m_ndiPhoneUSB, "CFAbsoluteTimeGetGregorianDate");
	if (!m_CFAbsoluteTimeGetGregorianDate)
		return CFGregorianDate();
	try{
		return m_CFAbsoluteTimeGetGregorianDate(tick, cfTimezone);
	}
	catch(...)
	{
		return CFGregorianDate();
	}
}

int CCFConverter::NDCFDataGetLength(void* cfdata)
{
	if (!LoadLib())
		return -1;
    m_CFDataGetLength = (CFDataGetLength)GetProcAddress(m_ndiPhoneUSB, "CFDataGetLength");
	if (!m_CFDataGetLength)
		return -1;
	try{
		return m_CFDataGetLength(cfdata);
	}
	catch(...)
	{
		return -1;
	}
}

void* CCFConverter::NDCFDataGetBytePtr(void* cfdata)
{
	if (!LoadLib())
		return NULL;
    m_CFDataGetBytePtr = (CFDataGetBytePtr)GetProcAddress(m_ndiPhoneUSB, "CFDataGetBytePtr");
	if (!m_CFDataGetBytePtr)
		return NULL;
	try{
		return m_CFDataGetBytePtr(cfdata);
	}
	catch(...)
	{
		return NULL;
	}
}

int CCFConverter::NDCFNumberGetType(void* cfNum)
{
	if (!LoadLib())
		return -1;
    m_CFNumberGetType = (CFNumberGetType)GetProcAddress(m_ndiPhoneUSB, "CFNumberGetType");
	if (!m_CFNumberGetType)
		return -1;
	try{
		return m_CFNumberGetType(cfNum);
	}
	catch(...)
	{
		return -1;
	}
}

unsigned char CCFConverter::NDCFNumberGetValue(void* cfNum, int size, unsigned char* ret)
{
	if (!LoadLib())
		return 0;
    m_CFNumberGetValue = (CFNumberGetValue)GetProcAddress(m_ndiPhoneUSB, "CFNumberGetValue");
	if (!m_CFNumberGetValue)
		return 0;
	try{
		return m_CFNumberGetValue(cfNum, size, ret);
	}
	catch(...)
	{
		return -1;
	}
}

void* CCFConverter::NDCFDataCreateMutable(void* allocator, __int32 length)
{
	if (!LoadLib())
		return NULL;
    m_CFDataCreateMutable = (CFDataCreateMutable)GetProcAddress(m_ndiPhoneUSB, "CFDataCreateMutable");
	if (!m_CFDataCreateMutable)
		return NULL;
	try{
		return m_CFDataCreateMutable(allocator, length);
	}
	catch(...)
	{
		return NULL;
	}
}

void CCFConverter::NDCFDataAppendBytes(void* cfData, unsigned char* src, __int32 length)
{
	if (!LoadLib())
		return ;
    m_CFDataAppendBytes = (CFDataAppendBytes)GetProcAddress(m_ndiPhoneUSB, "CFDataAppendBytes");
	if (!m_CFDataAppendBytes)
		return ;
	try{
		return m_CFDataAppendBytes(cfData, src, length);
	}
	catch(...)
	{
		return;
	}
}

bool CCFConverter::NDCFURLWriteDataAndPropertiesToResource(void* fileURL, void* xmlData, void* propertiesToWrite, int errorCode)
{
	if (!LoadLib())
		return false;
    m_CFURLWriteDataAndPropertiesToResource = (CFURLWriteDataAndPropertiesToResource)GetProcAddress(m_ndiPhoneUSB, "CFURLWriteDataAndPropertiesToResource");
	if (!m_CFURLWriteDataAndPropertiesToResource)
		return false;
	try{
		return m_CFURLWriteDataAndPropertiesToResource(fileURL, xmlData, propertiesToWrite, errorCode);
	}
	catch(...)
	{
		return false;
	}
}

void* CCFConverter::NDkCFBooleanTrue()
{
	if (!LoadLib())
		return NULL;

	return (*((void**)GetProcAddress(m_ndiPhoneUSB, "kCFBooleanTrue")));
 }

void* CCFConverter::NDkCFBooleanFalse()
{
	if (!LoadLib())
		return NULL;

    return (*((void**)GetProcAddress(m_ndiPhoneUSB, "kCFBooleanFalse")));
}

void* CCFConverter::NDkCFAllocatorSystemDefault()
{
	if (!LoadLib())
		return NULL;

    return (*((void**)GetProcAddress(m_ndiPhoneUSB, "kCFAllocatorSystemDefault")));
}

void* CCFConverter::NDkCFAllocatorNull()
{
	if (!LoadLib())
		return NULL;

    return (*((void**)GetProcAddress(m_ndiPhoneUSB, "kCFAllocatorNull")));
}

void* CCFConverter::NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
								 unsigned __int32 options, void* errorPtr)
{
	if (!LoadLib())
		return NULL;
    m_CFPropertyListCreateData = (CFPropertyListCreateData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateData");
	if (!m_CFPropertyListCreateData)
		return NULL;
	try{
		return m_CFPropertyListCreateData(allocator, propertyList, format, options, errorPtr);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options)
{
	RETURN_NULL_IF ( ! LoadLib());

    m_CFPropertyListCreateWithData = (CFPropertyListCreateWithData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateWithData");
	if (!m_CFPropertyListCreateWithData)
		return NULL;
	try{
		void* ptr = m_CFPropertyListCreateWithData(NULL, data, options, NULL, NULL);
		return ptr;
	}
	catch(...)
	{
		return NULL;
	}
	
}

void* CCFConverter::NDCFGetAllocator(void* cfTypeRef)
{
    RETURN_NULL_IF(!LoadLib());

    m_CFGetAllocator = (CFGetAllocator)GetProcAddress(m_ndiPhoneUSB, "CFGetAllocator");
    RETURN_NULL_IF(!m_CFGetAllocator);
	try{
	 return m_CFGetAllocator(cfTypeRef);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFStringCreateWithCString(void* allocator, unsigned char* cstr, UINT32 cfStringEncoding)
{
    RETURN_NULL_IF(!LoadLib());

    m_CFStringCreateWithCString = (CFStringCreateWithCString)GetProcAddress(m_ndiPhoneUSB, "CFStringCreateWithCString");
    RETURN_NULL_IF(!m_CFStringCreateWithCString);

	try{
	 return m_CFStringCreateWithCString(allocator, cstr, cfStringEncoding);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFDataCreateWithBytesNoCopy(void* allocator, unsigned char* bytes, __int32 length, void* bytesDeallocator)
{
	if (!LoadLib())
		return NULL;

    m_CFDataCreateWithBytesNoCopy = (CFDataCreateWithBytesNoCopy)GetProcAddress(m_ndiPhoneUSB, "CFDataCreateWithBytesNoCopy");
	if (!m_CFDataCreateWithBytesNoCopy)
		return NULL;
	try{
		return m_CFDataCreateWithBytesNoCopy(allocator, bytes, length, bytesDeallocator);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
											   CFPropertyListMutabilityOptions mutabilityOption, void** errorString)
{
	if (!LoadLib())
		return NULL;
    m_CFPropertyListCreateFromXMLData = (CFPropertyListCreateFromXMLData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateFromXMLData");
	if (!m_CFPropertyListCreateFromXMLData)
		return NULL;
	try{
		return m_CFPropertyListCreateFromXMLData(allocator, xmlData, mutabilityOption, errorString);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFPropertyListCreateXMLData(void* xmlData)
{
    if (!LoadLib())
        return NULL;
    m_CFPropertyListCreateXMLData = (CFPropertyListCreateXMLData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateXMLData");
    if (!m_CFPropertyListCreateXMLData)
        return NULL;
	try{
	 return m_CFPropertyListCreateXMLData(xmlData);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDCFDictionaryGetValue(void* cfDict, void* key)
{
	if (!LoadLib())
		return NULL;
	m_CFDictionaryGetValue = (CFDictionaryGetValue)GetProcAddress(m_ndiPhoneUSB, "CFDictionaryGetValue");
	if (!m_CFDictionaryGetValue)
		return NULL;
	try{
		return m_CFDictionaryGetValue(cfDict, key);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::ND__CFStringMakeConstantString(const char* str)
{
	if (!LoadLib())
		return NULL;
	m___CFStringMakeConstantString = (__CFStringMakeConstantString)GetProcAddress(m_ndiPhoneUSB, "__CFStringMakeConstantString");
	if (!m___CFStringMakeConstantString)
		return NULL;
	try{
		return m___CFStringMakeConstantString(str);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDkCFTypeDictionaryKeyCallBacks()
{
	if (!LoadLib())
		return NULL;
	m_kCFTypeDictionaryKeyCallBacks = (kCFTypeDictionaryKeyCallBacks)GetProcAddress(m_ndiPhoneUSB, "kCFTypeDictionaryKeyCallBacks");
	if (!m_kCFTypeDictionaryKeyCallBacks)
		return NULL;
	try{
		return m_kCFTypeDictionaryKeyCallBacks;
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDkCFTypeDictionaryValueCallBacks()
{
	if (!LoadLib())
		return NULL;
	m_kCFTypeDictionaryValueCallBacks = (kCFTypeDictionaryValueCallBacks)GetProcAddress(m_ndiPhoneUSB, "kCFTypeDictionaryValueCallBacks");
	if (!m_kCFTypeDictionaryValueCallBacks)
		return NULL;
	try{
		return m_kCFTypeDictionaryValueCallBacks;
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDkCFTypeArrayCallBacks()
{
	if (!LoadLib())
		return NULL;
	m_kCFTypeArrayCallBacks = (kCFTypeArrayCallBacks)GetProcAddress(m_ndiPhoneUSB, "kCFTypeArrayCallBacks");
	if (!m_kCFTypeArrayCallBacks)
		return NULL;
	try{
		return m_kCFTypeArrayCallBacks;
	}
	catch(...)
	{
		return NULL;
	}
}

void* CCFConverter::NDkCFNumberPositiveInfinity()
{
	if (!LoadLib())
		return NULL;
	m_kCFNumberPositiveInfinity = (kCFNumberPositiveInfinity)GetProcAddress(m_ndiPhoneUSB, "kCFNumberPositiveInfinity");
	if (!m_kCFNumberPositiveInfinity)
		return NULL;
	try{
		return m_kCFNumberPositiveInfinity;
	}
	catch(...)
	{
		return NULL;
	}
}