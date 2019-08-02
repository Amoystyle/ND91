#ifdef WIN32

#include "ND_CF.h"
#include "windows.h"

HMODULE g_dll = NULL;	// handle to kernel32.dll 

inline void* Load(const char * funcName)
{
    if ( ! g_dll )
    {
        g_dll = LoadLibraryA("CoreFoundation.dll");
    }
    return GetProcAddress(g_dll, funcName);
}

// load function pointer
#define LOAD_FUNC(x) 	static FP_##x func = (FP_##x)Load(""#x"");

/************************************************************************/
/*    下面是每个函数的定义。需要使用新的函数请自行增加           */
/*                                                                      */
/*    为了避免混乱，请按照头文件分类                             */
/************************************************************************/

/*****************  CFBase.h                ******************/
typedef CFTypeID (*FP_CFNullGetTypeID)(void);
CFTypeID ND_CF::CFNullGetTypeID(void)
{
    LOAD_FUNC(CFNullGetTypeID);
    return func ? func() : NULL;
}

typedef void (*FP_CFRelease)(CFTypeRef cf);
void ND_CF::CFRelease(CFTypeRef cf)
{
    LOAD_FUNC(CFRelease);
    return func ? func(cf) : NULL;
}

typedef CFTypeID (*FP_CFGetTypeID)(CFTypeRef cf);
CFTypeID ND_CF::CFGetTypeID(CFTypeRef cf)
{
    LOAD_FUNC(CFGetTypeID);
    return func ? func(cf) : NULL;
}

/*****************  CFArray.h               ******************/
typedef CFTypeID (*FP_CFArrayGetTypeID)(void);
CFTypeID ND_CF::CFArrayGetTypeID(void)
{
    LOAD_FUNC(CFArrayGetTypeID);
    return func ? func() : NULL;
}

typedef CFIndex (*FP_CFArrayGetCount)(CFArrayRef theArray);
CFIndex ND_CF::CFArrayGetCount(CFArrayRef theArray)
{
    LOAD_FUNC(CFArrayGetCount);
    return func ? func(theArray) : NULL;
}

typedef void (*FP_CFArrayAppendValue)(CFMutableArrayRef theArray, const void *value);
void ND_CF::CFArrayAppendValue(CFMutableArrayRef theArray, const void *value)
{
    LOAD_FUNC(CFArrayAppendValue);
    return func ? func(theArray, value) : NULL;
}

typedef CFMutableArrayRef (*FP_CFArrayCreateMutable)(CFAllocatorRef allocator, CFIndex capacity, const CFArrayCallBacks *callBacks);
CFMutableArrayRef ND_CF::CFArrayCreateMutable(CFAllocatorRef allocator, CFIndex capacity, const CFArrayCallBacks *callBacks)
{
    LOAD_FUNC(CFArrayCreateMutable);
    return func ? func(allocator, capacity, callBacks) : NULL;
}

typedef void (*FP_CFArrayGetValues)(CFArrayRef theArray, CFRange range, const void **values);
void ND_CF::CFArrayGetValues(CFArrayRef theArray, CFRange range, const void **values)
{
    LOAD_FUNC(CFArrayGetValues);
    return func ? func(theArray, range, values) : NULL;
}

/*****************  CFBag.h                 ******************/
typedef CFTypeID (*FP_CFBagGetTypeID)(void);
CFTypeID ND_CF::CFBagGetTypeID(void)
{
    LOAD_FUNC(CFBagGetTypeID);
    return func ? func() : NULL;
}

/*****************  CFBinaryHeap.h          ******************/

/*****************  CFBitVector.h           ******************/

/*****************  CFByteOrder.h           ******************/

/*****************  CFCalendar.h            ******************/

/*****************  CFCharacterSet.h        ******************/

/*****************  CFData.h                ******************/
typedef CFTypeID(*FP_CFDataGetTypeID)(void);
CFTypeID ND_CF::CFDataGetTypeID(void)
{
    LOAD_FUNC(CFDataGetTypeID);
    return func ? func() : NULL;
}

typedef CFDataRef (*FP_CFDataCreate) (CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length);
CFDataRef ND_CF::CFDataCreate(CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length)
{
    LOAD_FUNC(CFDataCreate);
    return func ? func(allocator, bytes, length) : NULL;
}

typedef CFMutableDataRef (*FP_CFDataCreateMutable)(CFAllocatorRef allocator, CFIndex capacity);
CFMutableDataRef ND_CF::CFDataCreateMutable(CFAllocatorRef allocator, CFIndex capacity)
{
    LOAD_FUNC(CFDataCreateMutable);
    return func ? func(allocator, capacity) : NULL;
}

typedef const UInt8 *(*FP_CFDataGetBytePtr)(CFDataRef theData);
const UInt8 * ND_CF::CFDataGetBytePtr(CFDataRef theData)
{
    LOAD_FUNC(CFDataGetBytePtr);
    return func ? func(theData) : NULL;
}

typedef CFIndex (*FP_CFDataGetLength)(CFDataRef theData);
CFIndex ND_CF::CFDataGetLength(CFDataRef theData)
{
    LOAD_FUNC(CFDataGetLength);
    return func ? func(theData) : NULL;
}

typedef CFDataRef (*FP_CFDataCreateWithBytesNoCopy)(CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length, CFAllocatorRef bytesDeallocator);
CFDataRef ND_CF::CFDataCreateWithBytesNoCopy(CFAllocatorRef allocator, const UInt8 *bytes, CFIndex length, CFAllocatorRef bytesDeallocator)
{
    LOAD_FUNC(CFDataCreateWithBytesNoCopy);
    return func ? func(allocator, bytes, length, bytesDeallocator) : NULL;
}

typedef void (*FP_CFDataAppendBytes)(CFMutableDataRef theData, const UInt8 *bytes, CFIndex length);
void ND_CF::CFDataAppendBytes(CFMutableDataRef theData, const UInt8 *bytes, CFIndex length)
{
    LOAD_FUNC(CFDataAppendBytes);
    return func ? func(theData, bytes, length) : NULL;
}

/*****************  CFDate.h                ******************/
typedef CFTypeID (*FP_CFDateGetTypeID)(void);
CFTypeID ND_CF::CFDateGetTypeID(void)
{
    LOAD_FUNC(CFDateGetTypeID);
    return func ? func() : NULL;
}

typedef CFAbsoluteTime (*FP_CFGregorianDateGetAbsoluteTime)(CFGregorianDate gdate, CFTimeZoneRef tz);
CFAbsoluteTime ND_CF::CFGregorianDateGetAbsoluteTime(CFGregorianDate gdate, CFTimeZoneRef tz)
{
    LOAD_FUNC(CFGregorianDateGetAbsoluteTime);
    return func ? func(gdate, tz) : NULL;
}

typedef CFAbsoluteTime (*FP_CFDateGetAbsoluteTime)(CFDateRef theDate);
CFAbsoluteTime ND_CF::CFDateGetAbsoluteTime(CFDateRef theDate)
{
    LOAD_FUNC(CFDateGetAbsoluteTime);
    return func ? func(theDate) : NULL;
}

typedef CFGregorianDate (*FP_CFAbsoluteTimeGetGregorianDate)(CFAbsoluteTime at, CFTimeZoneRef tz);
CFGregorianDate ND_CF::CFAbsoluteTimeGetGregorianDate(CFAbsoluteTime at, CFTimeZoneRef tz)
{
    LOAD_FUNC(CFAbsoluteTimeGetGregorianDate);
    return func ? func(at, tz) : CFGregorianDate();
}

/*****************  CFDateFormatter.h       ******************/

/*****************  CFDictionary.h          ******************/

typedef Boolean (*FP_CFDictionaryContainsKey)(CFDictionaryRef theDict, const void *key);
Boolean ND_CF::CFDictionaryContainsKey(CFDictionaryRef theDict, const void *key)
{
	LOAD_FUNC(CFDictionaryContainsKey);
	return func ? func(theDict, key) : NULL;
}

typedef CFTypeID (*FP_CFDictionaryGetTypeID)(void);
CFTypeID ND_CF::CFDictionaryGetTypeID(void)
{
    LOAD_FUNC(CFDictionaryGetTypeID);
    return func ? func() : NULL;
}

typedef CFTypeID (*FP_CFDictionaryGetTypeID)(void);
CFTypeID ND_CF::CFDictionaryGetTypeID(CFTypeRef cf)
{
    LOAD_FUNC(CFDictionaryGetTypeID);
    return func ? func() : NULL;
}

typedef void (*FP_CFDictionaryAddValue)(CFMutableDictionaryRef theDict, const void *key, const void *value);
void ND_CF::CFDictionaryAddValue(CFMutableDictionaryRef theDict, const void *key, const void *value)
{
    LOAD_FUNC(CFDictionaryAddValue);
    return func ? func(theDict, key, value) : NULL;
}

typedef const char* (*FP_CFStringGetCStringPtr)(CFStringRef theString, CFStringEncoding encoding);
const char* ND_CF::CFStringGetCStringPtr(CFStringRef theString, CFStringEncoding encoding)
{
	LOAD_FUNC(CFStringGetCStringPtr);
	return func ? func(theString, encoding) : NULL;
}

typedef const CFBooleanRef (*FP_kCFBooleanTrue)();
const CFBooleanRef ND_CF::kCFBooleanTrue()
{
	LOAD_FUNC(kCFBooleanTrue);
	return func ? func() : NULL;
}

typedef const CFBooleanRef (*FP_kCFBooleanFalse)();
const CFBooleanRef ND_CF::kCFBooleanFalse()
{
	LOAD_FUNC(kCFBooleanFalse);
	return func ? func() : NULL;
}

typedef void (*FP_CFDictionarySetValue)(CFMutableDictionaryRef theDict, const void *key, const void *value);
void ND_CF::CFDictionarySetValue(CFMutableDictionaryRef theDict, const void *key, const void *value)
{
    LOAD_FUNC(CFDictionarySetValue);
    return func ? func(theDict, key, value) : NULL;
}

typedef const void* (*FP_CFDictionaryGetValue)(CFDictionaryRef theDict, const void *key);
const void* ND_CF::CFDictionaryGetValue(CFDictionaryRef theDict, const void *key)
{
	LOAD_FUNC(CFDictionaryGetValue);
	return func ? func(theDict, key) : NULL;
}

typedef CFDictionaryRef (*FP_CFDictionaryCreate)(CFAllocatorRef allocator, const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks);
CFDictionaryRef ND_CF::CFDictionaryCreate(CFAllocatorRef allocator, const void **keys, const void **values, CFIndex numValues, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
{
    LOAD_FUNC(CFDictionaryCreate);
    return func ? func(allocator, keys, values, numValues, keyCallBacks, valueCallBacks)
        : NULL;
}

typedef CFMutableDictionaryRef (*FP_CFDictionaryCreateMutable)(CFAllocatorRef allocator, CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks);
CFMutableDictionaryRef ND_CF::CFDictionaryCreateMutable(CFAllocatorRef allocator, CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks)
{
    LOAD_FUNC(CFDictionaryCreateMutable);
    return func ? func(allocator, capacity, keyCallBacks, valueCallBacks) : NULL;
}

typedef CFIndex (*FP_CFDictionaryGetCount)(CFDictionaryRef theDict);
CFIndex ND_CF::CFDictionaryGetCount(CFDictionaryRef theDict)
{
    LOAD_FUNC(CFDictionaryGetCount);
    return func ? func(theDict) : NULL;
}

typedef void (*FP_CFDictionaryGetKeysAndValues)(CFDictionaryRef theDict, const void **keys, const void **values);
void ND_CF::CFDictionaryGetKeysAndValues(CFDictionaryRef theDict, const void **keys, const void **values)
{
    LOAD_FUNC(CFDictionaryGetKeysAndValues);
    return func ? func(theDict, keys, values) : NULL;
}

/*****************  CFError.h               ******************/

/*****************  CFLocale.h              ******************/

/*****************  CFNumber.h              ******************/

typedef CFTypeID (*FP_CFNumberGetTypeID)(void);
CFTypeID ND_CF::CFNumberGetTypeID(void)
{
    LOAD_FUNC(CFNumberGetTypeID);
    return func ? func() : NULL;
}


typedef CFTypeID (*FP_CFBooleanGetTypeID)(void);
CFTypeID ND_CF::CFBooleanGetTypeID(void)
{
    LOAD_FUNC(CFBooleanGetTypeID);
    return func ? func() : NULL;
}

typedef CFNumberType (*FP_CFNumberGetType)(CFNumberRef number);
CFNumberType ND_CF::CFNumberGetType(CFNumberRef number)
{
    LOAD_FUNC(CFNumberGetType);
    return func ? func(number) : NULL;
}

typedef Boolean (*FP_CFNumberGetValue)(CFNumberRef number, CFNumberType theType, void *valuePtr);
Boolean ND_CF::CFNumberGetValue(CFNumberRef number, CFNumberType theType, void *valuePtr)
{
    LOAD_FUNC(CFNumberGetValue);
    return func ? func(number, theType, valuePtr) : NULL;
}

typedef CFNumberRef (*FP_CFNumberCreate)(CFAllocatorRef allocator, CFNumberType theType, const void *valuePtr);
CFNumberRef ND_CF::CFNumberCreate(CFAllocatorRef allocator, CFNumberType theType, const void *valuePtr)
{
    LOAD_FUNC(CFNumberCreate);
    return func ? func(allocator, theType, valuePtr) : NULL;
}

/*****************  CFNumberFormatter.h     ******************/

/*****************  CFPreferences.h         ******************/

/*****************  CFPropertyList.h        ******************/

typedef CFPropertyListRef (*FP_CFPropertyListCreateWithData)(CFAllocatorRef allocator, CFDataRef data, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error);
CFPropertyListRef ND_CF::CFPropertyListCreateWithData(CFAllocatorRef allocator, CFDataRef data, CFOptionFlags options, CFPropertyListFormat *format, CFErrorRef *error)
{
    LOAD_FUNC(CFPropertyListCreateWithData);
    return func ? func(allocator, data, options, format, error) : NULL;
}

typedef CFDataRef (*FP_CFPropertyListCreateXMLData)(CFAllocatorRef allocator, CFPropertyListRef propertyList);
CFDataRef ND_CF::CFPropertyListCreateXMLData(CFAllocatorRef allocator, CFPropertyListRef propertyList)
{
    LOAD_FUNC(CFPropertyListCreateXMLData);
    return func ? func(allocator, propertyList) : NULL;
}

typedef CFPropertyListRef (*FP_CFPropertyListCreateFromXMLData)(CFAllocatorRef allocator, CFDataRef xmlData, CFOptionFlags mutabilityOption, CFStringRef *errorString);
CFPropertyListRef ND_CF::CFPropertyListCreateFromXMLData(CFAllocatorRef allocator, CFDataRef xmlData, CFOptionFlags mutabilityOption, CFStringRef *errorString)
{
    LOAD_FUNC(CFPropertyListCreateFromXMLData);
    return func ? func(allocator, xmlData, mutabilityOption, errorString) : NULL;
}

/*****************  CFSet.h                 ******************/
typedef CFTypeID (*FP_CFSetGetTypeID)(void);
CFTypeID ND_CF::CFSetGetTypeID(void)
{
    LOAD_FUNC(CFSetGetTypeID);
    return func ? func() : NULL;
}

/*****************  CFString.h              ******************/
typedef CFTypeID (*FP_CFStringGetTypeID)(void);
CFTypeID ND_CF::CFStringGetTypeID(void)
{
    LOAD_FUNC(CFStringGetTypeID);
    return func ? func() : NULL;
}

typedef CFStringRef (*FP_CFStringCreateWithCString)(CFAllocatorRef alloc, const char *cStr, CFStringEncoding encoding);
CFStringRef ND_CF::CFStringCreateWithCString(CFAllocatorRef alloc, const char *cStr, CFStringEncoding encoding)
{
    LOAD_FUNC(CFStringCreateWithCString);
    return func ? func(alloc, cStr, encoding) : NULL;
}

typedef CFDataRef (*FP_CFPropertyListCreateData)(CFAllocatorRef allocator, CFPropertyListRef propertyList, CFPropertyListFormat format, CFOptionFlags options, CFErrorRef *error);
CFDataRef ND_CF::CFPropertyListCreateData(CFAllocatorRef allocator, CFPropertyListRef propertyList, CFPropertyListFormat format, CFOptionFlags options, CFErrorRef *error)
{
    LOAD_FUNC(CFPropertyListCreateData);
    return func ? func(allocator, propertyList, format, options, error) : NULL;
}

typedef CFStringRef (*FP_CFStringCreateFromExternalRepresentation)(CFAllocatorRef alloc, CFDataRef data, CFStringEncoding encoding);
CFStringRef ND_CF::CFStringCreateFromExternalRepresentation(CFAllocatorRef alloc, CFDataRef data, CFStringEncoding encoding)
{
    LOAD_FUNC(CFStringCreateFromExternalRepresentation);
    return func ? func(alloc, data, encoding) : NULL;
}

typedef CFIndex (*FP_CFStringGetLength)(CFStringRef theString);
CFIndex ND_CF::CFStringGetLength(CFStringRef theString)
{
    LOAD_FUNC(CFStringGetLength);
    return func ? func(theString) : NULL;
}

typedef CFIndex (*FP_CFStringGetBytes)(CFStringRef theString, CFRange range, CFStringEncoding encoding, UInt8 lossByte, Boolean isExternalRepresentation, UInt8 *buffer, CFIndex maxBufLen, CFIndex *usedBufLen);
CFIndex ND_CF::CFStringGetBytes(CFStringRef theString, CFRange range, CFStringEncoding encoding, UInt8 lossByte, Boolean isExternalRepresentation, UInt8 *buffer, CFIndex maxBufLen, CFIndex *usedBufLen)
{
    LOAD_FUNC(CFStringGetBytes);
    return func ? func(theString, range, encoding, lossByte, isExternalRepresentation, buffer, maxBufLen, usedBufLen) : NULL;
}

/*****************  CFStringEncodingExt.h   ******************/

/*****************  CFTimeZone.h            ******************/
typedef CFTimeZoneRef (*FP_CFTimeZoneCopySystem)(void);
CFTimeZoneRef ND_CF::CFTimeZoneCopySystem(void)
{
    LOAD_FUNC(CFTimeZoneCopySystem);
    return func ? func() : NULL;
}

/*****************  CFTree.h                ******************/
typedef CFTypeID (*FP_CFTreeGetTypeID)(void);
CFTypeID ND_CF::CFTreeGetTypeID(void)
{
    LOAD_FUNC(CFTreeGetTypeID);
    return func ? func() : NULL;
}

/*****************  CFURL.h                 ******************/

/*****************  CFURLAccess.h           ******************/
typedef Boolean (*FP_CFURLWriteDataAndPropertiesToResource)(CFURLRef url, CFDataRef dataToWrite, CFDictionaryRef propertiesToWrite, SInt32 *errorCode);
Boolean ND_CF::CFURLWriteDataAndPropertiesToResource(CFURLRef url, CFDataRef dataToWrite, CFDictionaryRef propertiesToWrite, SInt32 *errorCode)
{
    LOAD_FUNC(CFURLWriteDataAndPropertiesToResource);
    return func ? func(url, dataToWrite, propertiesToWrite, errorCode) : NULL;
}

/*****************  CFUUID.h                ******************/

/*****************  CFBundle.h              ******************/

/*****************  CFPlugIn.h              ******************/

/*****************  CFRunLoop.h             ******************/

/*****************  CFStream.h              ******************/

/*****************  CFSocket.h              ******************/



#endif


