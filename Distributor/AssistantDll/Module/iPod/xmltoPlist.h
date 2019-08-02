#pragma once
#ifndef CFCONVERTER_H
typedef enum 
{
	kCFPropertyListImmutable = 0,
	kCFPropertyListMutableContainers = 1,
	kCFPropertyListMutableContainersAndLeaves = 2
}CFPropertyListMutabilityOptions;

typedef enum 
{
	kCFPropertyListOpenStepFormat = 1,
	kCFPropertyListXMLFormat_v1_0 = 100,
	kCFPropertyListBinaryFormat_v1_0 = 200
}CFPropertyListFormat;


typedef void* (*CFPropertyListCreateFromXMLData)(void* allocator, void* xmlData, 
												 CFPropertyListMutabilityOptions mutabilityOption, void** errorString);

typedef void* (*CFPropertyListCreateXMLData)(void* xmlData);

typedef void* (*CFDataCreate)(void* allocator, unsigned char* buf, int count);

typedef void  (*CFRelease)(void* cf);

typedef void* (*CFPropertyListCreateData)(void* allocator, void* propertyList, CFPropertyListFormat format, 
										  unsigned __int32 options, void* errorPtr);
typedef void* (*CFDataGetBytePtr)(void* cfdata);

typedef /*CFPropertyListRef*/ void*
(*CFPropertyListCreateWithData)(/*CFAllocatorRef*/ void* allocator, // Pass NULL to use the current default allocator.
								/*CFDataRef*/ void* data,
								CFPropertyListMutabilityOptions options,
								CFPropertyListFormat* format,
								/*CFErrorRef*/ void* error);
typedef int (*CFDataGetLength)(void* cfdata);



#endif
class PlistConver
{
public:

	static bool LoadLib();
	static void FreeLib();
	static void* NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
		CFPropertyListMutabilityOptions mutabilityOption, void** errorString);

	static void* NDCFPropertyListCreateXMLData(void* xmlData);
	static string ConvertPlist( string inputPlistData, bool outputBinary );
	static void* NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options);
	static void* NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
		unsigned __int32 options, void* errorPtr);
	static void  NDCFRelease(void* cf);
	static void* NDCFDataCreate(void* allocator, unsigned char* buf, int count);
	static void* PlistConver::NDCFDataGetBytePtr(void* cfdata);
	static int NDCFDataGetLength(void* cfdata);

	static HINSTANCE m_ndiPhoneUSB;
	static CFPropertyListCreateFromXMLData m_CFPropertyListCreateFromXMLData;
	static CFPropertyListCreateXMLData m_CFPropertyListCreateXMLData;

	static CFDataCreate m_CFDataCreate;
	static CFRelease m_CFRelease;
	static CFPropertyListCreateData m_CFPropertyListCreateData;
	static CFPropertyListCreateWithData m_CFPropertyListCreateWithData;
	static CFDataGetBytePtr m_CFDataGetBytePtr;
	static CFDataGetLength m_CFDataGetLength;

private:

};