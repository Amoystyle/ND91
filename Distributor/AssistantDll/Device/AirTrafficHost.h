///////////////////////////////////////////////////////////
//  AirTrafficHost.h
//  airTrafficHost.dll库的导出函数类
//  用于IOS设备的USB通讯
//  需要安装ITUNES，ITUNES版本不同，导出函数可能有增减
///////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable:4200)

#include "iPhoneInterfaceDef.h"

typedef int (*ATHostConnectionCreate)(void* device);
typedef void* (*ATHostConnectionCreateWithLibrary)(void* prefsId, void* uuId, int a);
typedef int (*ATHostConnectionSendPowerAssertion)(void* a1, void* a2);
typedef int (*ATHostConnectionRetain)(void* a1);
typedef int (*ATHostConnectionSendHostInfo)(void* a1, void* a2);
typedef void* (*ATHostConnectionSendSyncRequest)(void* a1, void* a2, void* a3, void* a4);
typedef void* (*ATHostConnectionReadMessage)(void* a1);
typedef int (*ATHostConnectionGetGrappaSessionId)(void* a1);
typedef	int (*ATHostConnectionSendMetadataSyncFinished)(void* pHandle, void* pCFDict, void* pCFDict2);
typedef long (*ATHostConnectionSendAssetCompleted)(void* pHandle, void* pName, void* pType, void* pPath);
typedef long (*ATHostConnectionSendFileError)(void* pHandle, void* pName, void* pType, char number);
typedef long (*ATHostConnectionSendFileProgress)(void* pHandle, void* pName, void* pType, double a1, double a2);

class CAirTrafficHost  
{
public:
	CAirTrafficHost();
	virtual ~CAirTrafficHost();

	static bool MyLoadLibrary();

	static void iTunes_ATHostConnectionCreate();
	static void* iTunes_ATHostConnectionCreateWithLibrary(void* prefsId, void* uuId, int a);
	static int iTunes_ATHostConnectionSendPowerAssertion(void* a1, void* a2);
	static int iTunes_ATHostConnectionRetain(void* a1);
	static int iTunes_ATHostConnectionSendHostInfo(void* a1, void* a2);
	static void* iTunes_ATHostConnectionSendSyncRequest(void* a1, void* a2, void* a3, void* a4);
	static void* iTunes_ATHostConnectionReadMessage(void* a1);
	static int iTunes_ATHostConnectionGetGrappaSessionId(void* a1);
	static int iTunes_ATHostConnectionSendMetadataSyncFinished(void* pHandle, void* pCFDict, void* pCFDict2);
	static long iTunes_ATHostConnectionSendAssetCompleted(void* pHandle, void* pName, void* pType, void* pPath);
	static long iTunes_ATHostConnectionSendFileError(void* pHandle, void* pName, void* pType, char number);
	static long iTunes_ATHostConnectionSendFileProgress(void* pHandle, void* pName, void* pType, double a1, double a2);

private:
	static HINSTANCE m_ndiPhoneUSB;
	static ATHostConnectionCreate  m_ATHostConnectionCreate;
	static ATHostConnectionCreateWithLibrary m_ATHostConnectionCreateWithLibrary;
	static ATHostConnectionSendPowerAssertion m_ATHostConnectionSendPowerAssertion;
	static ATHostConnectionRetain m_ATHostConnectionRetain;
	static ATHostConnectionSendHostInfo m_ATHostConnectionSendHostInfo;
	static ATHostConnectionSendSyncRequest m_ATHostConnectionSendSyncRequest;
	static ATHostConnectionReadMessage m_ATHostConnectionReadMessage;
	static ATHostConnectionGetGrappaSessionId m_ATHostConnectionGetGrappaSessionId;
	static ATHostConnectionSendMetadataSyncFinished m_ATHostConnectionSendMetadataSyncFinished;
	static ATHostConnectionSendAssetCompleted m_ATHostConnectionSendAssetCompleted;
	static ATHostConnectionSendFileError m_ATHostConnectionSendFileError;
	static ATHostConnectionSendFileProgress m_ATHostConnectionSendFileProgress;

};