#include "stdafx.h"
#ifdef MODULE_IPOD

#include "xmltoPlist.h"
#include "Device/iPhoneInterfaceDef.h"

CFPropertyListCreateFromXMLData PlistConver::m_CFPropertyListCreateFromXMLData = NULL;
CFPropertyListCreateXMLData PlistConver::m_CFPropertyListCreateXMLData = NULL;
HINSTANCE PlistConver::m_ndiPhoneUSB = NULL;

CFDataCreate PlistConver::m_CFDataCreate = NULL;
CFRelease PlistConver::m_CFRelease = NULL;
CFPropertyListCreateData PlistConver::m_CFPropertyListCreateData = NULL;
CFPropertyListCreateWithData PlistConver::m_CFPropertyListCreateWithData = NULL;
CFDataGetBytePtr PlistConver::m_CFDataGetBytePtr = NULL;
CFDataGetLength PlistConver::m_CFDataGetLength = NULL;
bool PlistConver::LoadLib()
{
	if (m_ndiPhoneUSB)
		return true;

	wstring dllPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
	TCHAR szCoreFoundationDll[MAX_PATH];
	ZeroMemory(szCoreFoundationDll, MAX_PATH*sizeof(TCHAR));
	_tcscat_s(szCoreFoundationDll, MAX_PATH, dllPath.c_str());
	_tcscat_s(szCoreFoundationDll, MAX_PATH, DLLCoreFoundation);

	m_ndiPhoneUSB = ::LoadLibrary(szCoreFoundationDll);

	if (!m_ndiPhoneUSB)
		return false;

	// 	m_qtClient = ::LoadLibrary(DLLQTMLCLIENT);
	// 	if (!m_qtClient)
	// 		return false;

	return true;
}

void PlistConver::FreeLib()
{
	if (m_ndiPhoneUSB)
		FreeLibrary(m_ndiPhoneUSB);
	m_ndiPhoneUSB = NULL;

	// 	if (m_qtClient)
	// 		FreeLibrary(m_qtClient);
	// 	m_qtClient = NULL;
}

void* PlistConver::NDCFPropertyListCreateFromXMLData(void* allocator, void* xmlData, 
													  CFPropertyListMutabilityOptions mutabilityOption, void** errorString)
{
	if (!LoadLib())
		return NULL;
	m_CFPropertyListCreateFromXMLData = (CFPropertyListCreateFromXMLData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateFromXMLData");
	if (!m_CFPropertyListCreateFromXMLData)
		return NULL;
	return m_CFPropertyListCreateFromXMLData(allocator, xmlData, mutabilityOption, errorString);
}

void* PlistConver::NDCFPropertyListCreateXMLData(void* xmlData)
{
	if (!LoadLib())
		return NULL;
	m_CFPropertyListCreateXMLData = (CFPropertyListCreateXMLData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateXMLData");
	if (!m_CFPropertyListCreateXMLData)
		return NULL;
	return m_CFPropertyListCreateXMLData(xmlData);

}

string PlistConver::ConvertPlist( string inputPlistData, bool outputBinary )
{
	// 生成CFDataRef
	void* cfBinaryData = PlistConver::NDCFDataCreate(
		NULL, (unsigned char*)inputPlistData.c_str(), inputPlistData.size());

	// 生成CFPropertyList
	void* cfPropertyList = PlistConver::NDCFPropertyListCreateWithData
		(cfBinaryData, kCFPropertyListImmutable);
	PlistConver::NDCFRelease(cfBinaryData);
	RETURN_VALUE_IF( ! cfPropertyList, "");

	// 将CFPropertyList输出为指定格式
	void* cfXmlData = PlistConver::NDCFPropertyListCreateData
		(NULL, cfPropertyList, 
		outputBinary ? kCFPropertyListBinaryFormat_v1_0 : kCFPropertyListXMLFormat_v1_0,
		0, NULL);

	PlistConver::NDCFRelease(cfPropertyList);
	RETURN_VALUE_IF( ! cfXmlData, "" );

	// 数据存放到string
	string result((char*)
		PlistConver::NDCFDataGetBytePtr(cfXmlData),
		PlistConver::NDCFDataGetLength(cfXmlData));
	PlistConver::NDCFRelease(cfXmlData);

	return result;
}

void* PlistConver::NDCFDataCreate(void* allocator, unsigned char* buf, int count)
{
	if (!LoadLib())
		return NULL;
	m_CFDataCreate = (CFDataCreate)GetProcAddress(m_ndiPhoneUSB, "CFDataCreate");
	if (!m_CFDataCreate)
		return NULL;
	return m_CFDataCreate(allocator, buf, count);
}

void  PlistConver::NDCFRelease(void* cf)
{
	if (!LoadLib())
		return ;
	m_CFRelease = (CFRelease)GetProcAddress(m_ndiPhoneUSB, "CFRelease");
	if (!m_CFRelease)
		return ;
	return m_CFRelease(cf);
}

void* PlistConver::NDCFPropertyListCreateData(void* allocator, void* propertyList, CFPropertyListFormat format, 
											   unsigned __int32 options, void* errorPtr)
{
	if (!LoadLib())
		return NULL;
	m_CFPropertyListCreateData = (CFPropertyListCreateData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateData");
	if (!m_CFPropertyListCreateData)
		return NULL;
	return m_CFPropertyListCreateData(allocator, propertyList, format, options, errorPtr);
}

void* PlistConver::NDCFPropertyListCreateWithData(void* data, CFPropertyListMutabilityOptions options)
{
	RETURN_NULL_IF ( ! LoadLib());

	m_CFPropertyListCreateWithData = (CFPropertyListCreateWithData)GetProcAddress(m_ndiPhoneUSB, "CFPropertyListCreateWithData");
	if (!m_CFPropertyListCreateWithData)
		return NULL;
	void* ptr = m_CFPropertyListCreateWithData(NULL, data, options, NULL, NULL);
	return ptr;
}

void* PlistConver::NDCFDataGetBytePtr(void* cfdata)
{
	if (!LoadLib())
		return NULL;
	m_CFDataGetBytePtr = (CFDataGetBytePtr)GetProcAddress(m_ndiPhoneUSB, "CFDataGetBytePtr");
	if (!m_CFDataGetBytePtr)
		return NULL;
	return m_CFDataGetBytePtr(cfdata);
}
int PlistConver::NDCFDataGetLength(void* cfdata)
{
	if (!LoadLib())
		return -1;
	m_CFDataGetLength = (CFDataGetLength)GetProcAddress(m_ndiPhoneUSB, "CFDataGetLength");
	if (!m_CFDataGetLength)
		return -1;
	return m_CFDataGetLength(cfdata);
}


#endif