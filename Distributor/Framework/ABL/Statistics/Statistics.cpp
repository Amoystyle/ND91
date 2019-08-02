#include "StdAfx.h"
#include "Statistics.h"
#include "Ufslib.h"
#include "Base/Config.h"
#include "Base/ClientInfo.h"
#include "Thread/StaticsticsDeviceInfoThread.h"
#include "Thread/TaskStatisticsThread.h"
#include "Common/UserCenter/UserCenterMsg.h"
#include "DeviceDataHelp.h"

CStatistics::CStatistics(void)
{
	if (!CFileOperation::IsDirExist(CONFIG(_strCachePath).c_str()))
	{
		CFileOperation::MakeLocalDir(CONFIG(_strCachePath));
	}

	UfslibHandle pUfslibHandle = NULL;
	pUfslibHandle = ::Init( (wchar_t*)CONFIG(_strCachePath).c_str()
		,CClientInfo::GetInstance()->GetAppId()
		,CClientInfo::GetInstance()->GetAppKey().c_str()
		,CClientInfo::GetInstance()->GetVersion().c_str()
		,CClientInfo::GetInstance()->GetChangleId().c_str()
		,"zh-cn"
		, ::GetTickCount()
		);

	_pUfsHandle = pUfslibHandle;	
	if ( _pUfsHandle )
	{
		::SendDevInfo(_pUfsHandle,NULL);
	}
	
}

CStatistics::~CStatistics(void)
{
}

bool CStatistics::Handle( const CStaticsticsMessage* msg )
{
	Statistic_StatFunc(msg->_id.c_str());
	return true;
}

bool CStatistics::Handle( const ResponseUserLoginMsg* pMsg )
{
	CTaskStatisticsThread* pThread = new CTaskStatisticsThread("",Action_PostDataToService);
	pThread->SetStrParam(pMsg->userID);
	pThread->Run();
	return true;
}

bool CStatistics::Handle( const CStaticsticsDevice* msg )
{
	std::string temp;
	AblDeviceInfo* pData = (AblDeviceInfo*)msg->pAblDeviceInfo;
	DeviceDataHelp help(pData);

	temp = help.GetTrueImei();
	memset(_pdata.imei, '\0', 100);
	memcpy_s(_pdata.imei, 100, temp.c_str(), temp.length() > 99 ? 99 : temp.length() );
	temp = CCodeOperation::UnicodeToGB2312(pData->baseInfo.deviceProductVersion);
	memset(_pdata.osVersion, '\0', 50);
	memcpy_s(_pdata.osVersion, 50, temp.c_str(), temp.length() > 49 ? 49 : temp.length() );	
	temp = CCodeOperation::UnicodeToGB2312(pData->baseInfo.deviceCoreVersion);
	memset(_pdata.romVersion, '\0', 50);
	memcpy_s(_pdata.romVersion, 50, temp.c_str(), temp.length() > 49 ? 49 : temp.length() );
	temp = CCodeOperation::UnicodeToGB2312(pData->baseInfo.deviceModelName);
	memset(_pdata.model, '\0', 50);
	memcpy_s(_pdata.model, 50, temp.c_str(), temp.length() > 49 ? 49 : temp.length() );
	_pdata.platForm	= help.GetDeviceType();
	_pdata.jailbroken = help.GetJailbroken();

	StaticsticsDeviceInfoThread* pThread = new StaticsticsDeviceInfoThread("", this);
	pThread->Run();
	return true;
}

bool CStatistics::Handle( CStaticsticsMessageWithTab* pMsg )
{
	Statistic_StatFuncTab(pMsg->_id.c_str(),pMsg->_tab.c_str(),pMsg->ret);
	return true;
}

bool CStatistics::Handle( RequestWindowSystemInfoMsg* pMsg )
{
	CSystemInfo info;
	pMsg->bIs64 = info.is64();
	pMsg->bIsVista = info.IsVista();
	pMsg->bIsWindows7 = info.IsWindows7();
	pMsg->bIsXP = info.IsXP();
	pMsg->SystemInfo = info.GetSystemInfo();
	pMsg->Mac = info.GetMac();
	pMsg->Os = info.GetOs();
	pMsg->bIsNeedInstalliTunes = info.IsNeedInstalliTunes();

	return true;
}

void CStatistics::Exit()
{
	if (_pUfsHandle)
	{
		::SendData(_pUfsHandle);
	}
}