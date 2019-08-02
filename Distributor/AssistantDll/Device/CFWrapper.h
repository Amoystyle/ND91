///////////////////////////////////////////////////////////
//  
//  ios数据编码格式转换
//  
///////////////////////////////////////////////////////////
#pragma once
#ifndef CFWRAPPER_H
#define  CFWRAPPER_H

#include <CoreFoundation/CoreFoundation.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class CFWrapper
{
public:

#pragma region ios数据格式转换为C++数据格式

    // summary  : 从ios设备取得指定数据(只支持string、数值、bool类型)
    //            若需要支持其他类型，需要写类似下面bool、string的处理方法
    // parameter: void * pIosDevice     ios设备指针
    // parameter: const char* domain    数据类别 (ansi编码)
    // parameter: const char* key       数据名   (ansi编码)
    // parameter: bool & value          数据值
    // return   : true = 成功 ,  false = 失败
    template<class T>
    static bool GetIosData(void* pIosDevice, const char* domain, const char* key, T& value)
    {
        void* pData = GetDeviceData(pIosDevice, domain, key);
        if ( ! pData )
            return false;

        bool bReturn = GetIosData(pData, value);

        NDCFRelease(pData);
        return bReturn;
    }

    // summary  : 从ios格式数据，取得C++格式数据
    // parameter: void * pData     ios设备数据
    // parameter: bool & value          数据值
    // return   : true = 成功 ,  false = 失败
    static bool GetIosData(void* pIosData, bool& value);
	static bool GetIosData(void* pIosData, map<std::string, std::string>& dict);
	static bool GetIosData(void* pIosData, map<std::string, bool>& dict);
	static bool GetIosData(void* pIosData, map<std::string, void*>& dict);
	static bool GetIosData(void* pIosData, vector<void*>& arr);
    static bool GetIosData(void* pIosData, time_t& time);
    static bool GetIosData(void* pIosData, std::string& value);
    static bool GetIosData(void* pIosData, time_t& time, std::string& timeStr);
	static bool GetIosData(void* pIosData, vector<string>& arr);
	static bool GetIosData(void* pIosData, vector<char>& buf);
    static bool GetIosEntitiesData(void* pIosData, vector<string>& arr, map< string, map<string,void*> > &mapEntities, void* devPtr);
    static bool GetIosEntitiesData(void* pIosData, map< string, map<string,void*> >& mapEntities, void* devPtr);
    static bool GetIosEntitiesData(void* pIosData, vector<string>& arr, map<string, string>& mapIds);

    // summary  : ios格式数据，转为数值类型
    // parameter: void * pIosDevice     ios设备指针
    // parameter: const char* domain    数据类别 (ansi编码)
    // parameter: const char* key       数据名   (ansi编码)
    // parameter: T& value              数据值
    // return   : true = 成功 ,  false = 失败
    template<class T>
    static bool GetIosData(void* pIosData, T& value)
    {
        if (! pIosData)
            return false;

        if (NDCFGetTypeID(pIosData) != NDCFNumberGetTypeID())
            return false;

        switch (NDCFNumberGetType(pIosData) - 1)
        {
        case 0:
        case 1:
        case 2:
        case 6:
        case 7:
        case 8:
        case 9:
        case 13:
        case 14:
            return (0 != NDCFNumberGetValue(pIosData, kCFNumberIntType, (unsigned char *)&value));

        case 3:
        case 10:
            return (0 != NDCFNumberGetValue(pIosData, kCFNumberSInt64Type, (unsigned char *)&value));

        case 4:
        case 11:
        case 15:
            return (0 != NDCFNumberGetValue(pIosData, kCFNumberFloatType, (unsigned char *)&value));

        case 5:
        case 12:
            return (0 != NDCFNumberGetValue(pIosData, kCFNumberDoubleType, (unsigned char *)&value));

        default:
            return false;
        }
    }

#pragma endregion

    static void* BoolToCFBool(bool bValue);
    static void* StringtoCFString(string cStr);
	static void* DictionarytoCFDictionary(map<string, string> srcDict);
    static void* DictionarytoCFDictionary(map<string, void*> srcDict);
	static void* TimeToCFDate(time_t time);
	static void* ArrayToCFArray(vector<void*> srcArray);
    static void* Int8ToCFInt8(int8_t num);
    static void* Int16ToCFInt16(int16_t num);
    static void* Int32ToCFInt32(int32_t num);
    static void* Int64ToCFInt64(int64_t num);

    static void* ConvertStringVector(vector<string> source);
    static void* ConvertStringVectorMap(map< string, vector<string> > source);
    static void* ConvertStringVectorMap(map< string, vector<void*> > source);
    static void* ConvertStringVectorMap(map< string, map<string,void*> > source);

    static void ClearMap(map<string, void*>& mapInfos);
    static void ClearVec(vector<void*>& vecInfo);
    static void ClearMapRecursive(map<string, void*>& mapInfos);
    static void ClearVecRecursive(vector<void*>& vecInfo);

    // Summary  : plist数据
    // Returns  : 输出的plist
    // Parameter: string inputPlistData 二进制或XML格式plist数据（xml格式若包含中文需为UTF8)
    // Parameter: bool outputBinary 指定输出格式，true为二进制，false为XML
    static string ConvertPlist(string inputPlistData, bool outputBinary);

#pragma region 封装函数

	static void* NDCFDataCreate(void* allocator, unsigned char* buf, int count);
	static void* NDCFStringCreateFromExternalRepresentation(void* allocator, void* data, uint32_t count);
	static void  NDCFRelease(void* cf);
	static void NDCFArrayAppendValue(void* cfArray, void* value);
	static void* NDCFDictionaryCreate(void* allocator, void** ptrKeys, void** ptrVal, int count, void* callbackKey, void* callbackValueAs);
	static void* NDCFArrayCreateMutable(void* allocator, int size, void* callback);
	static void* NDCFDictionaryCreateMutable(void* allocator, int size, void* callbackKey, void* callbackValue);
	static void NDCFDictionaryAddValue(void* cfDict, void* key, void* value);
	static void* NDCFNumberCreate(void* allocator, CFNumberType theType, void *valuePtr);
	static void* NDCFTimeZoneCopySystem();
	static double NDCFGregorianDateGetAbsoluteTime(CFGregorianDate dt, void* timezone);
	static void* NDCFDateCreate(void* allocator, double timetick);
	static int NDCFArrayGetCount(void* cfArray);
	static void NDCFArrayGetValues(void* cfarray, CFRange range, void** ret);
	static int NDCFDictionaryGetCount(void* cfDict);
	static void NDCFDictionaryGetKeysAndValues(void* cfDict, void** ptrKey, void** ptrVal);
	static int NDCFGetTypeID(void* ptr);
	static int NDCFStringGetLength(void* cfString);
	static int NDCFStringGetBytes(void* cfStr, CFRange range, uint32_t ukn1, unsigned char ukn2, unsigned char ukn3, char* buf, int bufCount, void* ukn4);
	static uint32_t NDCFStringGetTypeID();// 7 
	static uint32_t NDCFNumberGetTypeID();// 22
	static uint32_t NDCFBooleanGetTypeID();//21
	static uint32_t NDCFDateGetTypeID();// 256
	static uint32_t NDCFDataGetTypeID();// 19
	static uint32_t NDCFNullGetTypeID();// 20
	static uint32_t NDCFBagGetTypeID(); // 4
	static uint32_t NDCFSetGetTypeID();// 16
	static uint32_t NDCFTreeGetTypeID();// 28
	static uint32_t NDCFDictionaryGetTypeID();// 17
	static uint32_t NDCFArrayGetTypeID();// 18
	static double NDCFDateGetAbsoluteTime(void* cfDate);
	static CFGregorianDate NDCFAbsoluteTimeGetGregorianDate(double tick, void* cfTimezone);
	static int  NDCFDataGetLength(void* cfdata);
	static void* NDCFDataGetBytePtr(void* cfdata);
	static int  NDCFNumberGetType(void* cfNum);
	static unsigned char NDCFNumberGetValue(void* cfNum, int size, unsigned char* ret);
	static void* NDCFDataCreateMutable(void* allocator, int32_t length);
	static void NDCFDataAppendBytes(void* cfData, unsigned char* src, int32_t length);
	static bool NDCFURLWriteDataAndPropertiesToResource(void* fileURL, void* xmlData, void* propertiesToWrite, int errorCode);

	static void* NDkCFBooleanTrue();
	static void* NDkCFBooleanFalse();
	static void* NDkCFAllocatorSystemDefault();
	static void* NDkCFAllocatorNull();

	static void* NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
											uint32_t options, void* errorPtr);
	static void* NDCFDataCreateWithBytesNoCopy(void* allocator, unsigned char* bytes, int32_t length, void* bytesDeallocator);
	static void* NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
												CFPropertyListMutabilityOptions mutabilityOption, void** errorString);

    static void* NDCFPropertyListCreateXMLData(void* allocator, void* xmlData);

	// Creates a property list from a given CFData object.
	static void* NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options);

#pragma endregion

private:

    // summary  : 取IOS设备数据（原始格式）
    // parameter: void * pIosDevice     ios设备指针
    // parameter: const char* domain    数据类别 (ansi编码)
    // parameter: const char* key       数据名   (ansi编码)
    // return   : 数据区指针。为NULL表示失败
    static void* GetDeviceData(void* pIosDevice, const char* domain, const char* key);
};
#endif