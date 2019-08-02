#pragma once

#ifndef WIN32
    #define ND_CF 
    // ��MAC�£����þ�ת����::CFRelease(...)��ʽ
#else   // ND_CF����windows�µ�ʵ��

#include "CoreFoundation/CoreFoundation.h"


/************************************************************************
   ND_CF ��CoreFoundation�ķ�װ��
   ÿ���������ֶ���ԭ��������ͬ����ND_CF::CFRelease(...)��ʽ����
************************************************************************/
class ND_CF
{
public:
    /************************************************************************/
    /*    ������ÿ�������Ķ��塣��Ҫʹ���µĺ�������������           */
    /*                                                                      */
    /*    Ϊ�˱�����ң��밴��ͷ�ļ�����                             */
    /************************************************************************/

    /*****************  CFBase.h                ******************/

    void CFRelease(CFTypeRef cf);
    CFTypeID CFGetTypeID(CFTypeRef cf);

    /*****************  CFArray.h               ******************/

    /*****************  CFBag.h                 ******************/

    /*****************  CFBinaryHeap.h          ******************/

    /*****************  CFBitVector.h           ******************/

    /*****************  CFByteOrder.h           ******************/

    /*****************  CFCalendar.h            ******************/

    /*****************  CFCharacterSet.h        ******************/

    /*****************  CFData.h                ******************/

    CFDataRef CFDataCreate(CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length);
    const UInt8 * CFDataGetBytePtr(CFDataRef theData);
    CFIndex CFDataGetLength(CFDataRef theData);

    /*****************  CFDate.h                ******************/

    /*****************  CFDateFormatter.h       ******************/

    /*****************  CFDictionary.h          ******************/

	const CFBooleanRef kCFBooleanTrue();
	const CFBooleanRef kCFBooleanFalse();
	const char *CFStringGetCStringPtr(CFStringRef theString, CFStringEncoding encoding);

    CFTypeID CFDictionaryGetTypeID(CFTypeRef cf);
    CFTypeID CFDictionaryGetTypeID(void);
    void CFDictionarySetValue(CFMutableDictionaryRef theDict, const void *key, const void *value);
	const void *CFDictionaryGetValue(CFDictionaryRef theDict, const void *key);

	Boolean CFDictionaryContainsKey(CFDictionaryRef theDict, const void *key);

    /*****************  CFError.h               ******************/

    /*****************  CFLocale.h              ******************/

    /*****************  CFNumber.h              ******************/

    /*****************  CFNumberFormatter.h     ******************/

    /*****************  CFPreferences.h         ******************/

    /*****************  CFPropertyList.h        ******************/

    CFPropertyListRef CFPropertyListCreateWithData(CFAllocatorRef allocator, CFDataRef data, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error);

    /*****************  CFSet.h                 ******************/

    /*****************  CFString.h              ******************/

    CFStringRef CFStringCreateWithCString(CFAllocatorRef alloc, const char *cStr, CFStringEncoding encoding);
    CFDataRef CFPropertyListCreateData(CFAllocatorRef allocator, CFPropertyListRef propertyList, CFPropertyListFormat format, CFOptionFlags options, CFErrorRef *error);
    CFTypeID CFNullGetTypeID(void);
    CFTypeID CFArrayGetTypeID(void);
    CFIndex CFArrayGetCount(CFArrayRef theArray);
    void CFArrayAppendValue(CFMutableArrayRef theArray, const void *value);
    CFMutableArrayRef CFArrayCreateMutable(CFAllocatorRef allocator, CFIndex capacity, const CFArrayCallBacks *callBacks);
    void CFArrayGetValues(CFArrayRef theArray, CFRange range, const void **values);
    CFTypeID CFBagGetTypeID(void);
    CFTypeID CFDataGetTypeID(void);
    CFMutableDataRef CFDataCreateMutable(CFAllocatorRef allocator, CFIndex capacity);
    CFDataRef CFDataCreateWithBytesNoCopy(CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length, CFAllocatorRef bytesDeallocator);
    void CFDataAppendBytes(CFMutableDataRef theData, const UInt8 *bytes, CFIndex length);
    CFTypeID CFDateGetTypeID(void);
    CFAbsoluteTime CFGregorianDateGetAbsoluteTime(CFGregorianDate gdate, CFTimeZoneRef tz);
    CFAbsoluteTime CFDateGetAbsoluteTime(CFDateRef theDate);
    CFGregorianDate CFAbsoluteTimeGetGregorianDate(CFAbsoluteTime at, CFTimeZoneRef tz);
    void CFDictionaryAddValue(CFMutableDictionaryRef theDict, const void *key, const void *value);
    CFDictionaryRef CFDictionaryCreate(CFAllocatorRef allocator, const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks);
    CFMutableDictionaryRef CFDictionaryCreateMutable(CFAllocatorRef allocator, CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks);
    CFIndex CFDictionaryGetCount(CFDictionaryRef theDict);
    void CFDictionaryGetKeysAndValues(CFDictionaryRef theDict, const void **keys, const void **values);
    CFTypeID CFNumberGetTypeID(void);
    CFTypeID CFBooleanGetTypeID(void);
    CFNumberType CFNumberGetType(CFNumberRef number);
    Boolean CFNumberGetValue(CFNumberRef number, CFNumberType theType, void *valuePtr);
    CFNumberRef CFNumberCreate(CFAllocatorRef allocator, CFNumberType theType, const void *valuePtr);
    CFDataRef CFPropertyListCreateXMLData(CFAllocatorRef allocator, CFPropertyListRef propertyList);
    CFPropertyListRef CFPropertyListCreateFromXMLData(CFAllocatorRef allocator, CFDataRef xmlData, CFOptionFlags mutabilityOption, CFStringRef *errorString);
    CFTypeID CFSetGetTypeID(void);
    CFTypeID CFStringGetTypeID(void);
    CFStringRef CFStringCreateFromExternalRepresentation(CFAllocatorRef alloc, CFDataRef data, CFStringEncoding encoding);
    CFIndex CFStringGetLength(CFStringRef theString);
    CFIndex CFStringGetBytes(CFStringRef theString, CFRange range, CFStringEncoding encoding, UInt8 lossByte, Boolean isExternalRepresentation, UInt8 *buffer, CFIndex maxBufLen, CFIndex *usedBufLen);
    CFTimeZoneRef CFTimeZoneCopySystem(void);
    CFTypeID CFTreeGetTypeID(void);
    Boolean CFURLWriteDataAndPropertiesToResource(CFURLRef url, CFDataRef dataToWrite, CFDictionaryRef propertiesToWrite, SInt32 *errorCode);
    /*****************  CFStringEncodingExt.h   ******************/

    /*****************  CFTimeZone.h            ******************/

    /*****************  CFTree.h                ******************/

    /*****************  CFURL.h                 ******************/

    /*****************  CFURLAccess.h           ******************/

    /*****************  CFUUID.h                ******************/

    /*****************  CFBundle.h              ******************/

    /*****************  CFPlugIn.h              ******************/

    /*****************  CFRunLoop.h             ******************/

    /*****************  CFStream.h              ******************/

    /*****************  CFSocket.h              ******************/

};

#endif