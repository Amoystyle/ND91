#include "stdafx.h"

#include "AirTrafficHost.h"
#include "Device/IosPublicFunc.h"

#pragma region 函数指针初始化为NULL
HINSTANCE CAirTrafficHost::m_ndiPhoneUSB = NULL;
ATHostConnectionCreate  CAirTrafficHost::m_ATHostConnectionCreate = NULL;
ATHostConnectionCreateWithLibrary  CAirTrafficHost::m_ATHostConnectionCreateWithLibrary = NULL;
ATHostConnectionSendPowerAssertion CAirTrafficHost::m_ATHostConnectionSendPowerAssertion = NULL;
ATHostConnectionRetain CAirTrafficHost::m_ATHostConnectionRetain = NULL;
ATHostConnectionSendHostInfo CAirTrafficHost::m_ATHostConnectionSendHostInfo = NULL;
ATHostConnectionSendSyncRequest CAirTrafficHost::m_ATHostConnectionSendSyncRequest = NULL;
ATHostConnectionReadMessage CAirTrafficHost::m_ATHostConnectionReadMessage = NULL;
ATHostConnectionGetGrappaSessionId CAirTrafficHost::m_ATHostConnectionGetGrappaSessionId = NULL;
ATHostConnectionSendMetadataSyncFinished CAirTrafficHost::m_ATHostConnectionSendMetadataSyncFinished = NULL;
ATHostConnectionSendAssetCompleted CAirTrafficHost::m_ATHostConnectionSendAssetCompleted = NULL;
ATHostConnectionSendFileError CAirTrafficHost::m_ATHostConnectionSendFileError = NULL;
ATHostConnectionSendFileProgress CAirTrafficHost::m_ATHostConnectionSendFileProgress = NULL;
#pragma endregion

bool CAirTrafficHost::MyLoadLibrary()
{
	RETURN_TRUE_IF(m_ndiPhoneUSB);

	wstring strPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLAirTrafficHostReg, L"InstallDir");
	wstring szPath = IosPublicFunc::GetItunesDllRealPath(strPath, DLLAirTrafficHost);

	TCHAR szAirTrafficHostDll[MAX_PATH];
	ZeroMemory(szAirTrafficHostDll, MAX_PATH*sizeof(TCHAR));
	_tcscat_s(szAirTrafficHostDll, MAX_PATH, szPath.c_str());
	_tcscat_s(szAirTrafficHostDll, MAX_PATH, DLLAirTrafficHost);
	m_ndiPhoneUSB = ::LoadLibrary(szAirTrafficHostDll);

	return NULL != m_ndiPhoneUSB;
}

void CAirTrafficHost::iTunes_ATHostConnectionCreate()
{

}

void* CAirTrafficHost::iTunes_ATHostConnectionCreateWithLibrary(void* prefsId, void* uuId, int a)
{
	if (!MyLoadLibrary())
		return NULL;
	m_ATHostConnectionCreateWithLibrary = (ATHostConnectionCreateWithLibrary)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionCreateWithLibrary");
	if (!m_ATHostConnectionCreateWithLibrary)
		return NULL;

	try {
		return m_ATHostConnectionCreateWithLibrary(prefsId, uuId, a);
	}
	catch(...)
	{
		return NULL;
	}
}

int CAirTrafficHost::iTunes_ATHostConnectionSendPowerAssertion(void* a1, void* a2)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionSendPowerAssertion = (ATHostConnectionSendPowerAssertion)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendPowerAssertion");
	if (!m_ATHostConnectionSendPowerAssertion)
		return -1;

	try {
		return m_ATHostConnectionSendPowerAssertion(a1, a2);
	}
	catch(...)
	{
		return -1;
	}
}

int CAirTrafficHost::iTunes_ATHostConnectionRetain(void* a1)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionRetain = (ATHostConnectionRetain)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionRetain");
	if (!m_ATHostConnectionRetain)
		return -1;

	try {
		return m_ATHostConnectionRetain(a1);
	}
	catch(...)
	{
		return -1;
	}
}

int CAirTrafficHost::iTunes_ATHostConnectionSendHostInfo(void* a1, void* a2)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionSendHostInfo = (ATHostConnectionSendHostInfo)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendHostInfo");
	if (!m_ATHostConnectionSendHostInfo)
		return -1;

	try {
		return m_ATHostConnectionSendHostInfo(a1, a2);
	}
	catch(...)
	{
		return -1;
	}
}

void* CAirTrafficHost::iTunes_ATHostConnectionSendSyncRequest(void* a1, void* a2, void* a3, void* a4)
{
	if (!MyLoadLibrary())
		return NULL;
	m_ATHostConnectionSendSyncRequest = (ATHostConnectionSendSyncRequest)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendSyncRequest");
	if (!m_ATHostConnectionSendSyncRequest)
		return NULL;

	try {
		return m_ATHostConnectionSendSyncRequest(a1, a2, a3, a4);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CAirTrafficHost::iTunes_ATHostConnectionReadMessage(void* a1)
{
	if (!MyLoadLibrary())
		return NULL;
	m_ATHostConnectionReadMessage = (ATHostConnectionReadMessage)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionReadMessage");
	if (!m_ATHostConnectionReadMessage)
		return NULL;

	try {
		return m_ATHostConnectionReadMessage(a1);
	}
	catch(...)
	{
		return NULL;
	}
}

int CAirTrafficHost::iTunes_ATHostConnectionGetGrappaSessionId(void* a1)
{
	if (!MyLoadLibrary())
		return NULL;
	m_ATHostConnectionGetGrappaSessionId = (ATHostConnectionGetGrappaSessionId)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionGetGrappaSessionId");
	if (!m_ATHostConnectionGetGrappaSessionId)
		return NULL;

	try {
		return m_ATHostConnectionGetGrappaSessionId(a1);
	}
	catch(...)
	{
		return NULL;
	}
}

int CAirTrafficHost::iTunes_ATHostConnectionSendMetadataSyncFinished(void* pHandle, void* pCFDict, void* pCFDict2)
{
	if (!MyLoadLibrary())
		return NULL;
	m_ATHostConnectionSendMetadataSyncFinished = (ATHostConnectionSendMetadataSyncFinished)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendMetadataSyncFinished");
	if (!m_ATHostConnectionSendMetadataSyncFinished)
		return NULL;

	try {
		return m_ATHostConnectionSendMetadataSyncFinished(pHandle, pCFDict, pCFDict2);
	}
	catch(...)
	{
		return NULL;
	}
}

long CAirTrafficHost::iTunes_ATHostConnectionSendAssetCompleted(void* pHandle, void* pName, void* pType, void* pPath)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionSendAssetCompleted = (ATHostConnectionSendAssetCompleted)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendAssetCompleted");
	if (!m_ATHostConnectionSendAssetCompleted)
		return -1;

	try {
		return m_ATHostConnectionSendAssetCompleted(pHandle, pName, pType, pPath);
	}
	catch(...)
	{
		return -1;
	}
}


long CAirTrafficHost::iTunes_ATHostConnectionSendFileError(void* pHandle, void* pName, void* pType, char number)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionSendFileError = (ATHostConnectionSendFileError)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendFileError");
	if (!m_ATHostConnectionSendFileError)
		return -1;

	try {
		return m_ATHostConnectionSendFileError(pHandle, pName, pType, number);
	}
	catch(...)
	{
		return -1;
	}
}

long CAirTrafficHost::iTunes_ATHostConnectionSendFileProgress(void* pHandle, void* pName, void* pType, double a1, double a2)
{
	if (!MyLoadLibrary())
		return -1;
	m_ATHostConnectionSendFileProgress = (ATHostConnectionSendFileProgress)GetProcAddress(m_ndiPhoneUSB, "ATHostConnectionSendFileProgress");
	if (!m_ATHostConnectionSendFileProgress)
		return -1;

	try {
		return m_ATHostConnectionSendFileProgress(pHandle, pName, pType, a1, a2);
	}
	catch(...)
	{
		return -1;
	}
}
