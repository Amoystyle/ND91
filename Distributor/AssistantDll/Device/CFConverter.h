///////////////////////////////////////////////////////////
//  
//  CCFConverter: CoreFoundation.dll库的封装类
//  
//  ios数据编码格式转换
//  
///////////////////////////////////////////////////////////
#pragma once
#ifndef CFCONVERTER_H
#define  CFCONVERTER_H
#include "Common/DealTime.h"
#include <Windows.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#pragma region 函数接口定义

// 目前代码中只用到了UTF8类型 2011.6.17
typedef enum {
	kCFStringEncodingMacRoman = 0,
	kCFStringEncodingWindowsLatin1 = 0x0500, /* ANSI codepage 1252 */
	kCFStringEncodingISOLatin1 = 0x0201, /* ISO 8859-1 */
	kCFStringEncodingNextStepLatin = 0x0B01, /* NextStep encoding*/
	kCFStringEncodingASCII = 0x0600, /* 0..127 (in creating CFString, values greater than 0x7F are treated as corresponding Unicode value) */
	kCFStringEncodingUnicode = 0x0100, /* kTextEncodingUnicodeDefault  + kTextEncodingDefaultFormat (aka kUnicode16BitFormat) */
	kCFStringEncodingUTF8 = 0x08000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF8Format */
	kCFStringEncodingNonLossyASCII = 0x0BFF /* 7bit Unicode variants used by Cocoa & Java */
} CFStringBuiltInEncodings;

typedef enum 
{
	kCFPropertyListOpenStepFormat = 1,
	kCFPropertyListXMLFormat_v1_0 = 100,
	kCFPropertyListBinaryFormat_v1_0 = 200
}CFPropertyListFormat;

typedef enum 
{
	kCFPropertyListImmutable = 0,
	kCFPropertyListMutableContainers = 1,
	kCFPropertyListMutableContainersAndLeaves = 2
}CFPropertyListMutabilityOptions;

typedef struct CFGregorianDate
{
	INT32 year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	double second;
}CFGregorianDate;

typedef struct CFRange
{
	INT32 m_location;
	INT32 m_length;

	CFRange(INT32 loc, INT32 len)
	{
		m_location = loc;
		m_length = len;
	}
}CFRange;

typedef void * CFTypeRef;
typedef CFTypeRef CFDictionaryRef;
typedef CFTypeRef CFStringRef;

typedef enum {
	/* Types from MacTypes.h */
	kCFNumberSInt8Type = 1,
	kCFNumberSInt16Type = 2,
	kCFNumberSInt32Type = 3,
	kCFNumberSInt64Type = 4,
	kCFNumberFloat32Type = 5,
	kCFNumberFloat64Type = 6,	/* 64-bit IEEE 754 */
	/* Basic C types */
	kCFNumberCharType = 7,
	kCFNumberShortType = 8,
	kCFNumberIntType = 9,
	kCFNumberLongType = 10,
	kCFNumberLongLongType = 11,
	kCFNumberFloatType = 12,
	kCFNumberDoubleType = 13,
	/* Other */
	kCFNumberCFIndexType = 14,
	kCFNumberMaxType = 14
} CFNumberType;

typedef void* (*CFDataCreate)(void* allocator, unsigned char* buf, int count);
typedef void* (*CFStringCreateFromExternalRepresentation)(void* allocator, void* data, unsigned __int32 count);
typedef void  (*CFRelease)(void* cf);
typedef void (*CFArrayAppendValue)(void* cfArray, void* value);
typedef void* (*CFDictionaryCreate)(void* allocator, void** ptrKeys, void** ptrVal, int count, void* callbackKey, void* callbackValueAs);
typedef void* (*CFDictionaryCreateCopy)(void* allocator, void* cfDict);
typedef void* (*CFArrayCreateMutable)(void* allocator, int size, void* callback);
typedef void* (*CFDictionaryCreateMutable)(void* allocator, int size, void* callbackKey, void* callbackValue);
typedef void (*CFDictionaryAddValue)(void* cfDict, void* key, void* value);
typedef void (*CFDictionarySetValue)(void* cfDict, void* key, void* value);
typedef void* (*CFDictionaryGetValue)(void* cfDict, void* key);
typedef void* (*CFNumberCreate)(void* allocator, CFNumberType theType, void *valuePtr);
typedef void* (*CFTimeZoneCopySystem)();
typedef double (*CFGregorianDateGetAbsoluteTime)(CFGregorianDate dt, void* timezone);
typedef void* (*CFDateCreate)(void* allocator, double timetick);
typedef int (*CFArrayGetCount)(void* cfArray);
typedef void (*CFArrayGetValues)(void* cfarray, CFRange range, void** ret);
typedef int (*CFDictionaryGetCount)(void* cfDict);
typedef void (*CFDictionaryGetKeysAndValues)(void* cfDict, void** ptrKey, void** ptrVal);
typedef int (*CFGetTypeID)(void* ptr);
typedef int (*CFStringGetLength)(void* cfString);
typedef int (*CFStringGetBytes)(void* cfStr, CFRange range, unsigned __int32 ukn1, unsigned char ukn2, unsigned char ukn3, char* buf, int bufCount, void* ukn4);
typedef unsigned __int32 (*CFStringGetTypeID)();
typedef unsigned __int32 (*CFNumberGetTypeID)();
typedef unsigned __int32 (*CFBooleanGetTypeID)();
typedef unsigned __int32 (*CFDateGetTypeID)();
typedef unsigned __int32 (*CFDataGetTypeID)();
typedef unsigned __int32 (*CFNullGetTypeID)();
typedef unsigned __int32 (*CFBagGetTypeID)();
typedef unsigned __int32 (*CFSetGetTypeID)();
typedef unsigned __int32 (*CFTreeGetTypeID)();
typedef unsigned __int32 (*CFDictionaryGetTypeID)();
typedef unsigned __int32 (*CFArrayGetTypeID)();
typedef double (*CFDateGetAbsoluteTime)(void* cfDate);
typedef CFGregorianDate (*CFAbsoluteTimeGetGregorianDate)(double tick, void* cfTimezone);
typedef int (*CFDataGetLength)(void* cfdata);
typedef void* (*CFDataGetBytePtr)(void* cfdata);
typedef int (*CFNumberGetType)(void* cfNum);
typedef unsigned char (*CFNumberGetValue)(void* cfNum, int size, unsigned char* ret);
typedef void* (*CFDataCreateMutable)(void* allocator, __int32 length);
typedef void (*CFDataAppendBytes)(void* cfData, unsigned char* src, __int32 length);
typedef bool (*CFURLWriteDataAndPropertiesToResource)(void* fileURL, void* xmlData, void* propertiesToWrite, int errorCode);

typedef void* (*kCFBooleanTrue)();
typedef void* (*kCFBooleanFalse)();
typedef void* (*kCFAllocatorSystemDefault)();
typedef void* (*kCFAllocatorNull)();

typedef void* (*CFPropertyListCreateData)(void* allocator, void* propertyList, CFPropertyListFormat format, 
										  unsigned __int32 options, void* errorPtr);
typedef void* (*CFDataCreateWithBytesNoCopy)(void* allocator, unsigned char* bytes, __int32 length, void* bytesDeallocator);
typedef void* (*CFPropertyListCreateFromXMLData)(void* allocator, void* xmlData, 
									   CFPropertyListMutabilityOptions mutabilityOption, void** errorString);

typedef void* (*CFPropertyListCreateXMLData)(void* xmlData);
// Creates a property list from a given CFData object.
typedef /*CFPropertyListRef*/ void*
(*CFPropertyListCreateWithData)(/*CFAllocatorRef*/ void* allocator, // Pass NULL to use the current default allocator.
								/*CFDataRef*/ void* data,
								CFPropertyListMutabilityOptions options,
								CFPropertyListFormat* format,
								/*CFErrorRef*/ void* error);

typedef void* (*CFGetAllocator)(void* cfTypeRef);
typedef void* (*CFStringCreateWithCString)(void* allocator, unsigned char* cstr, UINT32 cfStringEncoding);
typedef void* (*__CFStringMakeConstantString)(const char* str);

typedef void* (*kCFTypeDictionaryKeyCallBacks);
typedef void* (*kCFTypeDictionaryValueCallBacks);
typedef void* (*kCFTypeArrayCallBacks);
typedef void* (*kCFNumberPositiveInfinity);

#pragma endregion

class CCFConverter
{
	friend class CActivator;
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
    static bool GetIosData(void* pIosData, std::string& value, bool toGB2312 = true);
    static bool GetIosData(void* pIosData, time_t& time, std::string& timeStr);
	static bool GetIosData(void* pIosData, vector<string>& arr);
	static bool GetIosData(void* pIosData, vector<char>& buf);
    static bool GetIosEntitiesData(void* pIosData, vector<string>& arr, map< string, map<string,void*> > &mapEntities, void* devPtr);
    static bool GetIosEntitiesData(void* pIosData, map< string, map<string,void*> >& mapEntities, void* devPtr);
    static bool GetIosEntitiesData(void* pIosData, vector<string>& arr, map<string, string>& mapIds);
	static bool GetIosEntitiesData(void* pIosData, map<string, string>& mapIds, map<string, string>& mapEntities);
	static bool GetIosEntitiesData(void* pIosData, vector<string>& arr, map<string, string>& mapIds, map<string, string>& mapEntities );
	static bool GetIosScreenSnapEntitiesData(void* pIosData, vector<string>& arr, wstring screensnappath);

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
    static void* StringtoCFString(string cStr, bool change = true);
	static void* DictionarytoCFDictionary(map<string, string> srcDict);
    static void* DictionarytoCFDictionary(map<string, void*> srcDict);
	static void* TimeToCFDate(time_t time);
	static void* ArrayToCFArray(vector<void*> srcArray);
    static void* Int8ToCFInt8(INT8 num);
    static void* Int16ToCFInt16(INT16 num);
    static void* Int32ToCFInt32(INT32 num);
    static void* Int64ToCFInt64(INT64 num);

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
	static string ConvertPlist( void* cfBinaryData, bool outputBinary );

#pragma region DLL封装函数
    static bool LoadLib();
    static void FreeLib();

	static void* NDCFDataCreate(void* allocator, unsigned char* buf, int count);
	static void* NDCFStringCreateFromExternalRepresentation(void* allocator, void* data, unsigned __int32 count);
	static void  NDCFRelease(void* cf);
	static void NDCFArrayAppendValue(void* cfArray, void* value);
	static void* NDCFDictionaryCreate(void* allocator, void** ptrKeys, void** ptrVal, int count, void* callbackKey, void* callbackValueAs);
	static void* NDCFDictionaryCreateCopy(void* allocator, void* cfDict);
	static void* NDCFArrayCreateMutable(void* allocator, int size, void* callback);
	static void* NDCFDictionaryCreateMutable(void* allocator, int size, void* callbackKey, void* callbackValue);
	static void NDCFDictionaryAddValue(void* cfDict, void* key, void* value);
	static void NDCFDictionarySetValue(void* cfDict, void* key, void* value);
	static void* NDCFDictionaryGetValue(void* cfDict, void* key);
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
	static int NDCFStringGetBytes(void* cfStr, CFRange range, unsigned __int32 ukn1, unsigned char ukn2, unsigned char ukn3, char* buf, int bufCount, void* ukn4);
	static unsigned __int32 NDCFStringGetTypeID();// 7 
	static unsigned __int32 NDCFNumberGetTypeID();// 22
	static unsigned __int32 NDCFBooleanGetTypeID();//21
	static unsigned __int32 NDCFDateGetTypeID();// 256
	static unsigned __int32 NDCFDataGetTypeID();// 19
	static unsigned __int32 NDCFNullGetTypeID();// 20
	static unsigned __int32 NDCFBagGetTypeID(); // 4
	static unsigned __int32 NDCFSetGetTypeID();// 16
	static unsigned __int32 NDCFTreeGetTypeID();// 28
	static unsigned __int32 NDCFDictionaryGetTypeID();// 17
	static unsigned __int32 NDCFArrayGetTypeID();// 18
	static double NDCFDateGetAbsoluteTime(void* cfDate);
	static CFGregorianDate NDCFAbsoluteTimeGetGregorianDate(double tick, void* cfTimezone);
	static int  NDCFDataGetLength(void* cfdata);
	static void* NDCFDataGetBytePtr(void* cfdata);
	static int  NDCFNumberGetType(void* cfNum);
	static unsigned char NDCFNumberGetValue(void* cfNum, int size, unsigned char* ret);
	static void* NDCFDataCreateMutable(void* allocator, __int32 length);
	static void NDCFDataAppendBytes(void* cfData, unsigned char* src, __int32 length);
	static bool NDCFURLWriteDataAndPropertiesToResource(void* fileURL, void* xmlData, void* propertiesToWrite, int errorCode);

	static void* NDkCFBooleanTrue();
	static void* NDkCFBooleanFalse();
	static void* NDkCFAllocatorSystemDefault();
	static void* NDkCFAllocatorNull();

	static void* NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
											unsigned __int32 options, void* errorPtr);
	static void* NDCFDataCreateWithBytesNoCopy(void* allocator, unsigned char* bytes, __int32 length, void* bytesDeallocator);
	static void* NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
												CFPropertyListMutabilityOptions mutabilityOption, void** errorString);

    static void* NDCFPropertyListCreateXMLData(void* xmlData);

	// Creates a property list from a given CFData object.
	static void* NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options);

    // Returns the allocator used to allocate a Core Foundation object.
    static void* NDCFGetAllocator(void* cfTypeRef);

    // Creates an immutable string from a C string.
    // param cfStringEncoding CFStringEncoding utf8 的值0x8000100
    static void* NDCFStringCreateWithCString(void* allocator, unsigned char* cstr, UINT32 cfStringEncoding);
	static void* ND__CFStringMakeConstantString(const char* str);
	static void* NDkCFTypeDictionaryKeyCallBacks();
	static void* NDkCFTypeDictionaryValueCallBacks();
	static void* NDkCFTypeArrayCallBacks();
	static void* NDkCFNumberPositiveInfinity();

#pragma endregion

private:

    // summary  : 取IOS设备数据（原始格式）
    // parameter: void * pIosDevice     ios设备指针
    // parameter: const char* domain    数据类别 (ansi编码)
    // parameter: const char* key       数据名   (ansi编码)
    // return   : 数据区指针。为NULL表示失败
    static void* GetDeviceData(void* pIosDevice, const char* domain, const char* key);

#pragma region 成员变量：DLL函数指针

	static HINSTANCE m_ndiPhoneUSB;
	static CFDataCreate m_CFDataCreate;
	static CFStringCreateFromExternalRepresentation m_CFStringCreateFromExternalRepresentation;
	static CFRelease m_CFRelease;
	static CFArrayAppendValue m_CFArrayAppendValue;
	static CFDictionaryCreate m_CFDictionaryCreate;
	static CFDictionaryCreateCopy m_CFDictionaryCreateCopy;
	static CFArrayCreateMutable m_CFArrayCreateMutable;
	static CFDictionaryCreateMutable m_CFDictionaryCreateMutable;
	static CFDictionaryAddValue m_CFDictionaryAddValue;
	static CFDictionarySetValue m_CFDictionarySetValue;
	static CFDictionaryGetValue m_CFDictionaryGetValue;
	static CFNumberCreate m_CFNumberCreate;
	static CFTimeZoneCopySystem m_CFTimeZoneCopySystem;
	static CFGregorianDateGetAbsoluteTime m_CFGregorianDateGetAbsoluteTime;
	static CFDateCreate m_CFDateCreate;
	static CFArrayGetCount m_CFArrayGetCount;
	static CFArrayGetValues m_CFArrayGetValues;
	static CFDictionaryGetCount m_CFDictionaryGetCount;
	static CFDictionaryGetKeysAndValues m_CFDictionaryGetKeysAndValues;
	static CFGetTypeID m_CFGetTypeID;
	static CFStringGetLength m_CFStringGetLength;
	static CFStringGetBytes m_CFStringGetBytes;
	static CFStringGetTypeID m_CFStringGetTypeID;
	static CFNumberGetTypeID m_CFNumberGetTypeID;
	static CFBooleanGetTypeID m_CFBooleanGetTypeID;
	static CFDateGetTypeID m_CFDateGetTypeID;
	static CFDataGetTypeID m_CFDataGetTypeID;
	static CFNullGetTypeID m_CFNullGetTypeID;
	static CFBagGetTypeID m_CFBagGetTypeID;
	static CFSetGetTypeID m_CFSetGetTypeID;
	static CFTreeGetTypeID m_CFTreeGetTypeID;
	static CFDictionaryGetTypeID m_CFDictionaryGetTypeID;
	static CFArrayGetTypeID m_CFArrayGetTypeID;
	static CFDateGetAbsoluteTime m_CFDateGetAbsoluteTime;
	static CFAbsoluteTimeGetGregorianDate m_CFAbsoluteTimeGetGregorianDate;
	static CFDataGetLength m_CFDataGetLength;
	static CFDataGetBytePtr m_CFDataGetBytePtr;
	static CFNumberGetType m_CFNumberGetType;
	static CFNumberGetValue m_CFNumberGetValue;
	static CFDataCreateMutable m_CFDataCreateMutable;
	static CFDataAppendBytes m_CFDataAppendBytes;
	static CFURLWriteDataAndPropertiesToResource m_CFURLWriteDataAndPropertiesToResource;

	static kCFBooleanTrue m_kCFBooleanTrue;
	static kCFBooleanFalse m_kCFBooleanFalse;
	static kCFAllocatorSystemDefault m_kCFAllocatorSystemDefault;
	static kCFAllocatorNull m_kCFAllocatorNull;

	static CFPropertyListCreateData m_CFPropertyListCreateData;
	static CFDataCreateWithBytesNoCopy m_CFDataCreateWithBytesNoCopy;
	static CFPropertyListCreateFromXMLData m_CFPropertyListCreateFromXMLData;
    static CFPropertyListCreateXMLData m_CFPropertyListCreateXMLData;
	static CFPropertyListCreateWithData m_CFPropertyListCreateWithData;

    static CFGetAllocator m_CFGetAllocator;
    static CFStringCreateWithCString m_CFStringCreateWithCString;
	static __CFStringMakeConstantString m___CFStringMakeConstantString;
	static kCFTypeDictionaryKeyCallBacks m_kCFTypeDictionaryKeyCallBacks;
	static kCFTypeDictionaryValueCallBacks m_kCFTypeDictionaryValueCallBacks;
	static kCFTypeArrayCallBacks m_kCFTypeArrayCallBacks;
	static kCFNumberPositiveInfinity m_kCFNumberPositiveInfinity;

#pragma endregion
};
#endif