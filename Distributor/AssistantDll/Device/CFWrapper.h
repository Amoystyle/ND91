///////////////////////////////////////////////////////////
//  
//  ios���ݱ����ʽת��
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

#pragma region ios���ݸ�ʽת��ΪC++���ݸ�ʽ

    // summary  : ��ios�豸ȡ��ָ������(ֻ֧��string����ֵ��bool����)
    //            ����Ҫ֧���������ͣ���Ҫд��������bool��string�Ĵ�����
    // parameter: void * pIosDevice     ios�豸ָ��
    // parameter: const char* domain    ������� (ansi����)
    // parameter: const char* key       ������   (ansi����)
    // parameter: bool & value          ����ֵ
    // return   : true = �ɹ� ,  false = ʧ��
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

    // summary  : ��ios��ʽ���ݣ�ȡ��C++��ʽ����
    // parameter: void * pData     ios�豸����
    // parameter: bool & value          ����ֵ
    // return   : true = �ɹ� ,  false = ʧ��
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

    // summary  : ios��ʽ���ݣ�תΪ��ֵ����
    // parameter: void * pIosDevice     ios�豸ָ��
    // parameter: const char* domain    ������� (ansi����)
    // parameter: const char* key       ������   (ansi����)
    // parameter: T& value              ����ֵ
    // return   : true = �ɹ� ,  false = ʧ��
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

    // Summary  : plist����
    // Returns  : �����plist
    // Parameter: string inputPlistData �����ƻ�XML��ʽplist���ݣ�xml��ʽ������������ΪUTF8)
    // Parameter: bool outputBinary ָ�������ʽ��trueΪ�����ƣ�falseΪXML
    static string ConvertPlist(string inputPlistData, bool outputBinary);

#pragma region ��װ����

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

    // summary  : ȡIOS�豸���ݣ�ԭʼ��ʽ��
    // parameter: void * pIosDevice     ios�豸ָ��
    // parameter: const char* domain    ������� (ansi����)
    // parameter: const char* key       ������   (ansi����)
    // return   : ������ָ�롣ΪNULL��ʾʧ��
    static void* GetDeviceData(void* pIosDevice, const char* domain, const char* key);
};
#endif