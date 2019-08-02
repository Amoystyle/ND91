#include "stdafx.h"

void* CFWrapper::GetDeviceData(void* pIosDevice, const char* domain, const char* key)
{
    void* p1 = domain ? StringtoCFString(domain) : NULL;
    void* p2 = key ? StringtoCFString(key) : NULL;

    return MobileDevice::iTunes_AMDeviceCopyValue(pIosDevice, p1, p2);
}

bool CFWrapper::GetIosData(void* pIosData, bool& value)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFBooleanGetTypeID() )
        return false;

    // 取得数据
    value = NDkCFBooleanTrue() == pIosData;

    return true;
}

bool CFWrapper::GetIosData(void* pIosData, time_t& time)
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

bool CFWrapper::GetIosData(void* pIosData, std::string& value)
{
    // 检查数据类型
    if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFStringGetTypeID() )
        return false;

	// 取得数据
	value = "";
	int nUsedBufferLen = 0;
	int nTextLen = NDCFStringGetLength(pIosData);
	CFRange range = CFRangeMake(0, nTextLen);

	if (NDCFStringGetBytes(pIosData, range, kCFStringEncodingUTF8, '?', 0, NULL, 1024, &nUsedBufferLen) > 0)
	{
		char* pBuffer = new char[nUsedBufferLen+1];
        memset(pBuffer, 0, nUsedBufferLen+1);

		if (NDCFStringGetBytes(pIosData, range, kCFStringEncodingUTF8, '?', 0, pBuffer, nUsedBufferLen, NULL) > 0)
		{
			 value = pBuffer;
			 value = CCodeOperation::UTF_8ToGB2312(value);
		}

		delete[] pBuffer;
	}	

	return true;
}

bool CFWrapper::GetIosData(void* pIosData, time_t& time, std::string& timeStr)
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

bool CFWrapper::GetIosData(void* pIosData, vector<string>& arr)
{
	arr.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;

	void** ptr = new void*[nCount];
	CFRange range = CFRangeMake(0, nCount);
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

bool CFWrapper::GetIosEntitiesData(void* pIosData, vector<string>& arr, map< string, map<string,void*> > &mapEntities, void* devPtr)
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
    CFRange range = CFRangeMake(0, nCount);
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
            GetIosEntitiesData(ptr[i], mapEntities, devPtr);
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

bool CFWrapper::GetIosEntitiesData(void* pIosData, vector<string>& arr, map<string, string>& mapIds)
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
    CFRange range = CFRangeMake(0, nCount);
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

bool CFWrapper::GetIosData(void* pIosData, vector<void*>& arr)
{
	arr.clear();

	// 检查数据类型
	if ( ! pIosData || NDCFGetTypeID(pIosData) != NDCFArrayGetTypeID() )
		return false;

	int nCount = NDCFArrayGetCount(pIosData);
	if (nCount <= 0)
		return false;
	
	void** ptr = new void*[nCount];
	CFRange range = CFRangeMake(0, nCount);
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

bool CFWrapper::GetIosData(void* pIosData, map<std::string, void*>& dict)
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

bool CFWrapper::GetIosEntitiesData(void* pIosData, map< string, map<string,void*> >& mapEntities, void* devPtr)
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
                GetIosData(iter->second, intTemp);
                bbb = Int32ToCFInt32(intTemp);
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
                //DeviceData* deviceDate = DEVICE_MANAGER->FindIosDevice(devPtr);
                //wstring photoPath = deviceDate->GetPath()->GetLocalPimPhotoPath();
                //photoPath += L"\\" + CFileOperation::GetGuidW() + L".png";
								wstring photoPath = L"./head.png";
								
                temp.clear();
                CFWrapper::GetIosData(bbb,temp);
                for (vector<void*>::iterator iter1 = temp.begin(); iter1 != temp.end(); iter1++)
                {
                    int nLen = CFWrapper::NDCFDataGetLength(*iter1);
                    void* pValue = CFWrapper::NDCFDataGetBytePtr(*iter1);
                    unsigned char* uszValue = new unsigned char[nLen];
                    memset(uszValue, 0, nLen);
                    memcpy(uszValue, pValue, nLen);

                    FILE* f = fopen(CCodeOperation::WstringToString(photoPath).c_str(), "w+b");
                    if (f) 
                    {
                        fwrite(uszValue, nLen, 1, f);
                        fflush(f);
                        fclose(f);					
                    }

                    SAFE_DELETE_ARRAY(uszValue);
                }
                bbb = StringtoCFString(CCodeOperation::UnicodeToUTF_8(photoPath));
            }
            temp3.insert(pair<string,void*>(iter->first,bbb));
        }
        mapEntities.insert(pair< string, map<string, void*> >(str,temp3));
    }
    
    return true;
}

bool CFWrapper::GetIosData(void* pIosData, map<std::string, std::string>& dict)
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

bool CFWrapper::GetIosData(void* pIosData, map<std::string, bool>& dict)
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

bool CFWrapper::GetIosData( void* pIosData, vector<char>& buf )
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

void* CFWrapper::BoolToCFBool(bool bValue)
{
    if (bValue)
        return NDkCFBooleanTrue();
    return NDkCFBooleanFalse();
}

void* CFWrapper::ArrayToCFArray(vector<void*> srcArray)
{
	if (srcArray.empty())
		return NULL;

	int nCount = srcArray.size();
	void* ptr = NDCFArrayCreateMutable(NULL, nCount, NULL);
	for (int i=0; i<nCount; i++)
		NDCFArrayAppendValue(ptr, srcArray.at(i));
	
	return ptr;
}

void* CFWrapper::TimeToCFDate(time_t time)
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

void* CFWrapper::Int8ToCFInt8(int8_t num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt8Type, (void*)&num);
}

void* CFWrapper::Int16ToCFInt16(int16_t num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt16Type, (void*)&num);
}

void* CFWrapper::Int32ToCFInt32(int32_t num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt32Type, (void*)&num);
}

void* CFWrapper::Int64ToCFInt64(int64_t num)
{
    return NDCFNumberCreate(NULL, kCFNumberSInt64Type, (void*)&num);
}

void* CFWrapper::StringtoCFString(string cStr)
{
	string utf8str = CCodeOperation::GB2312ToUTF_8(cStr);
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

void* CFWrapper::ConvertStringVector(vector<string> source)
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

void* CFWrapper::ConvertStringVectorMap(map< string, vector<string> > source)
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

void* CFWrapper::ConvertStringVectorMap(map< string, map<string,void*> > source)
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

void* CFWrapper::ConvertStringVectorMap(map< string, vector<void*> > source)
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

void* CFWrapper::DictionarytoCFDictionary(map<string, string> srcDict)
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

void* CFWrapper::DictionarytoCFDictionary(map<string, void*> srcDict)
{
    RETURN_NULL_IF(srcDict.empty());

    void* pCFDict = NDCFDictionaryCreateMutable(NULL, srcDict.size(), NULL, NULL);
    for (map<string, void*>::iterator it = srcDict.begin(); it != srcDict.end(); it++)
        NDCFDictionaryAddValue(pCFDict, StringtoCFString(it->first), it->second);

    return pCFDict;
}

void CFWrapper::ClearMap(map<string, void*>& mapInfos)
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

void CFWrapper::ClearVec(vector<void*>& vecInfo)
{
    for (size_t i = 0; i < vecInfo.size(); i++)
    {
        if (vecInfo.at(i))
            NDCFRelease(vecInfo.at(i));
        vecInfo.at(i) = NULL;
    }
    vecInfo.clear();
}

void CFWrapper::ClearMapRecursive(map<string, void*>& mapInfos)
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

void CFWrapper::ClearVecRecursive(vector<void*>& vecInfo)
{
    for (size_t i = 0; i < vecInfo.size(); i++)
    {
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

string CFWrapper::ConvertPlist( string inputPlistData, bool outputBinary )
{
    // 生成CFDataRef
    void* cfBinaryData = CFWrapper::NDCFDataCreate(
        NULL, (unsigned char*)inputPlistData.c_str(), inputPlistData.size());

    // 生成CFPropertyList
    void* cfPropertyList = CFWrapper::NDCFPropertyListCreateWithData
        (cfBinaryData, kCFPropertyListImmutable);
    CFWrapper::NDCFRelease(cfBinaryData);
    RETURN_VALUE_IF( ! cfPropertyList, "");

    // 将CFPropertyList输出为指定格式
    void* cfXmlData = CFWrapper::NDCFPropertyListCreateData
        (NULL, cfPropertyList, 
        outputBinary ? kCFPropertyListBinaryFormat_v1_0 : kCFPropertyListXMLFormat_v1_0,
        0, NULL);

    CFWrapper::NDCFRelease(cfPropertyList);
    RETURN_VALUE_IF( ! cfXmlData, "" );

    // 数据存放到string
    string result((char*)
        CFWrapper::NDCFDataGetBytePtr(cfXmlData),
        CFWrapper::NDCFDataGetLength(cfXmlData));
    CFWrapper::NDCFRelease(cfXmlData);

    return result;
}

void* CFWrapper::NDCFDataCreate(void* allocator, unsigned char* buf, int count)
{
	return (void*)CFDataCreate((const __CFAllocator *)allocator, buf, count);
}

void* CFWrapper::NDCFStringCreateFromExternalRepresentation(void* allocator, void* data, uint32_t count)
{
	return (void*)CFStringCreateFromExternalRepresentation((const __CFAllocator *)allocator, (const __CFData*)data, count);
}

void  CFWrapper::NDCFRelease(void* cf)
{
	return CFRelease(cf);
}

void CFWrapper::NDCFArrayAppendValue(void* cfArray, void* value)
{
	return CFArrayAppendValue((__CFArray*)cfArray, value);
}

void* CFWrapper::NDCFDictionaryCreate(void* allocator, void** ptrKeys, void** ptrVal, int count, void* callbackKey, void* callbackValueAs)
{
	return (void*)CFDictionaryCreate((const __CFAllocator *)allocator, (const void**)ptrKeys, (const void**)ptrVal, count, (const CFDictionaryKeyCallBacks*)callbackKey, (const CFDictionaryValueCallBacks*)callbackValueAs);
}

void* CFWrapper::NDCFArrayCreateMutable(void* allocator, int size, void* callback)
{
	return CFArrayCreateMutable((const __CFAllocator *)allocator, size, (const CFArrayCallBacks*)callback);
}

void* CFWrapper::NDCFDictionaryCreateMutable(void* allocator, int size, void* callbackKey, void* callbackValue)
{
	return CFDictionaryCreateMutable((const __CFAllocator *)allocator, size, (const CFDictionaryKeyCallBacks*)callbackKey, (const CFDictionaryValueCallBacks*)callbackValue);
}

void CFWrapper::NDCFDictionaryAddValue(void* cfDict, void* key, void* value)
{
	return CFDictionaryAddValue((__CFDictionary*)cfDict, key, value);
}

void* CFWrapper::NDCFNumberCreate(void* allocator, CFNumberType theType, void *valuePtr)
{
	return (void*)CFNumberCreate((const __CFAllocator *)allocator, theType, valuePtr);
}

void* CFWrapper::NDCFTimeZoneCopySystem()
{
	return (void*)CFTimeZoneCopySystem();
}

double CFWrapper::NDCFGregorianDateGetAbsoluteTime(CFGregorianDate dt, void* timezone)
{
	return CFGregorianDateGetAbsoluteTime(dt, (const __CFTimeZone*)timezone);
}

void* CFWrapper::NDCFDateCreate(void* allocator, double timetick)
{
	return (void*)CFDateCreate((const __CFAllocator *)allocator, timetick);
}

int CFWrapper::NDCFArrayGetCount(void* cfArray)
{
	return CFArrayGetCount((const __CFArray*)cfArray);
}

void CFWrapper::NDCFArrayGetValues(void* cfarray, CFRange range, void** ret)
{
	return CFArrayGetValues((const __CFArray*)cfarray, range, (const void**)ret);
}

int CFWrapper::NDCFDictionaryGetCount(void* cfDict)
{
	return CFDictionaryGetCount((const __CFDictionary*)cfDict);
}

void CFWrapper::NDCFDictionaryGetKeysAndValues(void* cfDict, void** ptrKey, void** ptrVal)
{
	return CFDictionaryGetKeysAndValues((const __CFDictionary*)cfDict, (const void**)ptrKey, (const void**)ptrVal);
}

int CFWrapper::NDCFGetTypeID(void* ptr)
{
	return CFGetTypeID(ptr);
}

int CFWrapper::NDCFStringGetLength(void* cfString)
{
	return CFStringGetLength((const __CFString*)cfString);
}

int CFWrapper::NDCFStringGetBytes(void* cfStr, CFRange range, uint32_t ukn1, unsigned char ukn2, unsigned char ukn3, char* buf, int bufCount, void* ukn4)
{
	return CFStringGetBytes((const __CFString*)cfStr, range, ukn1, ukn2, ukn3, (UInt8*)buf, bufCount, (CFIndex*)ukn4);
}

uint32_t CFWrapper::NDCFStringGetTypeID()
{
	return CFStringGetTypeID();
}

uint32_t CFWrapper::NDCFNumberGetTypeID()
{
	return CFNumberGetTypeID();
}

uint32_t CFWrapper::NDCFBooleanGetTypeID()
{
	return CFBooleanGetTypeID();
}

uint32_t CFWrapper::NDCFDateGetTypeID()
{
	return CFDateGetTypeID();
}

uint32_t CFWrapper::NDCFDataGetTypeID()
{
	return CFDataGetTypeID();
}

uint32_t CFWrapper::NDCFNullGetTypeID()
{
	return CFNullGetTypeID();
}

uint32_t CFWrapper::NDCFBagGetTypeID()
{
	return CFBagGetTypeID();
}

uint32_t CFWrapper::NDCFSetGetTypeID()
{
	return CFSetGetTypeID();
}

uint32_t CFWrapper::NDCFTreeGetTypeID()
{
	return CFTreeGetTypeID();
}

uint32_t CFWrapper::NDCFDictionaryGetTypeID()
{
	return CFDictionaryGetTypeID();
}

uint32_t CFWrapper::NDCFArrayGetTypeID()
{
	return CFArrayGetTypeID();
}

double CFWrapper::NDCFDateGetAbsoluteTime(void* cfDate)
{
	return CFDateGetAbsoluteTime((const __CFDate*)cfDate);
}

CFGregorianDate CFWrapper::NDCFAbsoluteTimeGetGregorianDate(double tick, void* cfTimezone)
{
	return CFAbsoluteTimeGetGregorianDate(tick, (const __CFTimeZone*)cfTimezone);
}

int CFWrapper::NDCFDataGetLength(void* cfdata)
{
	return CFDataGetLength((const __CFData*)cfdata);
}

void* CFWrapper::NDCFDataGetBytePtr(void* cfdata)
{
	return (void*)CFDataGetBytePtr((const __CFData*)cfdata);
}

int CFWrapper::NDCFNumberGetType(void* cfNum)
{
	return CFNumberGetType((const __CFNumber*)cfNum);
}

unsigned char CFWrapper::NDCFNumberGetValue(void* cfNum, int size, unsigned char* ret)
{
	return CFNumberGetValue((const __CFNumber*)cfNum, size, ret);
}

void* CFWrapper::NDCFDataCreateMutable(void* allocator, int32_t length)
{
	return CFDataCreateMutable((const __CFAllocator*)allocator, length);
}

void CFWrapper::NDCFDataAppendBytes(void* cfData, unsigned char* src, int32_t length)
{
	return CFDataAppendBytes((__CFData*)cfData, src, length);
}

bool CFWrapper::NDCFURLWriteDataAndPropertiesToResource(void* fileURL, void* xmlData, void* propertiesToWrite, int errorCode)
{
	return CFURLWriteDataAndPropertiesToResource((const __CFURL*)fileURL, (const __CFData*)xmlData, (const __CFDictionary*)propertiesToWrite, (SInt32*)errorCode);
}

void* CFWrapper::NDkCFBooleanTrue()
{
	return (void*)kCFBooleanTrue;
 }

void* CFWrapper::NDkCFBooleanFalse()
{
    return (void*)kCFBooleanFalse;
}

void* CFWrapper::NDkCFAllocatorSystemDefault()
{
    return (void*)kCFAllocatorSystemDefault;
}

void* CFWrapper::NDkCFAllocatorNull()
{
    return (void*)kCFAllocatorNull;
}

void* CFWrapper::NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
								 uint32_t options, void* errorPtr)
{
	return (void*)CFPropertyListCreateData((const __CFAllocator*)allocator, propertyList, format, options, (__CFError**)errorPtr);
}

void* CFWrapper::NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options)
{
	return (void*)CFPropertyListCreateWithData(NULL, (const __CFData*)data, options, NULL, NULL);
}

void* CFWrapper::NDCFDataCreateWithBytesNoCopy(void* allocator, unsigned char* bytes, int32_t length, void* bytesDeallocator)
{
	return (void*)CFDataCreateWithBytesNoCopy((const __CFAllocator*)allocator, bytes, length, (const __CFAllocator*)bytesDeallocator);
}

void* CFWrapper::NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
											   CFPropertyListMutabilityOptions mutabilityOption, void** errorString)
{
	return (void*)CFPropertyListCreateFromXMLData((const __CFAllocator*)allocator, (const __CFData*)xmlData, mutabilityOption, (const __CFString**)errorString);
}

void* CFWrapper::NDCFPropertyListCreateXMLData(void* allocator, void* xmlData)
{
    return (void*)CFPropertyListCreateXMLData((const __CFAllocator*)allocator, (const void*)xmlData);
}

