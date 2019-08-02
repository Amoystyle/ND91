#include "stdafx.h"
#include "StatisticHelp.h"
#include "Common/MyFileOperation.h"
#include "Statistics/Utils.h"
#include "Statistics.h"
StatisticHelp::StatisticHelp(CStatistics* pStatistics)
{//INTERFUN;
	_pStatistics = pStatistics;
}

StatisticHelp::~StatisticHelp()
{//INTERFUN;
}

bool StatisticHelp::PostZJDeviceConnectNum()
{//INTERFUN;
	bool ret = false;
	char* szTab = new char[6];
	wstring strMaxDeviceCount = MyFileOperation::GetIniFileValue(CONFIG(_strUserSettingIni), L"Run", L"MaxDevice");
	int nMaxDeviceCount = 0;
	if (strMaxDeviceCount.length() > 0)
		nMaxDeviceCount = _wtoi(strMaxDeviceCount.c_str());
	if (nMaxDeviceCount >= 7)
		sprintf_s( szTab, 6, "t%d", 7 );
	else if (nMaxDeviceCount > 0)
		sprintf_s( szTab, 6, "t%d", nMaxDeviceCount );
	/*CStaticsticsMessageWithTab msg(Statistic_ID_ConnectDeviceNum,szTab);
	msg.ret = ret;
	msg.SendAndGetReturn();*/
	if (_pStatistics->_pUfsHandle) ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_ConnectDeviceNum, NULL, szTab);
	//Statistic_StatFuncTab( Statistic_ID_ConnectDeviceNum, szTab, ret );
	SAFE_DELETE_ARRAY(szTab);

	return ret;
}

bool StatisticHelp::PostZJInstallPlanNum( vector<InstallsCaseData *> DataList )
{//INTERFUN;
	bool ret = false;
	int iosNum=0, andNUm=0;
	char* szTab = new char[6];

	for ( int i=0; i<(int)DataList.size(); i++ )
	{
		InstallsCaseData * pdata = DataList[i];
		
		if ( pdata->PlanType() != _INSTALL_PLAN_TYPE_USER )
			continue;
		
		if ( pdata->CaseType() == _INSTALLS_CASE_DATA_IOS )
			iosNum++;
		else
			andNUm++;
	}
	
	if ( iosNum < 3 )
		sprintf_s( szTab, 6, "i%d", 1 );	
	else if ( iosNum < 5 )
		sprintf_s( szTab, 6, "i%d", 2 );
	else if ( iosNum < 7 )
		sprintf_s( szTab, 6, "i%d", 3 );
	else if ( iosNum < 10 )
		sprintf_s( szTab, 6, "i%d", 4 );
	else
		sprintf_s( szTab, 6, "i%d", 5 );

	//Statistic_StatFuncTab( Statistic_ID_InstallPlanNum, szTab, ret );
	if (_pStatistics->_pUfsHandle) 
	{
		ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_InstallPlanNum, NULL, szTab);
	}
	memset(szTab, '\0', 6);

	if ( andNUm < 3 )
		sprintf_s( szTab, 6, "i%d", 1 );	
	else if ( andNUm < 5 )
		sprintf_s( szTab, 6, "i%d", 2 );
	else if ( andNUm < 7 )
		sprintf_s( szTab, 6, "i%d", 3 );
	else if ( andNUm < 10 )
		sprintf_s( szTab, 6, "i%d", 4 );
	else
		sprintf_s( szTab, 6, "i%d", 5 );
	
	//Statistic_StatFuncTab( Statistic_ID_InstallPlanNum, szTab, ret );
	if (_pStatistics->_pUfsHandle) 
	{
		ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_InstallPlanNum, NULL, szTab);
	}
	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::PostZJInstallDuration( int num, int allSize )
{//INTERFUN;

	int sum = 0; bool ret = false;
	char* szTab = new char[6];

	UIStringFormat::InstallNeedTime(num, allSize, sum);
	if ( sum <= 30 )
		sprintf_s( szTab, 6, "t%d", sum/10 +1 );
	else if ( sum <= 60 )
		sprintf_s( szTab, 6, "t%d", 4 );
	else if ( sum <= 90 )
		sprintf_s( szTab, 6, "t%d", 5 );
	else
		sprintf_s( szTab, 6, "t%d", 6 );

	/*CStaticsticsMessageWithTab msg(Statistic_ID_ExecuteDuration,szTab);
	msg.ret = ret;
	msg.SendAndGetReturn();*/
	//Statistic_StatFuncTab(Statistic_ID_ExecuteDuration, szTab, ret);
	if (_pStatistics->_pUfsHandle) ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_ExecuteDuration, NULL, szTab);
	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::PostZJAppNumOfAnPlan(int num)
{//INTERFUN;
	bool ret = false;
	char* szTab = new char[6];

	if ( num > 50 )
		sprintf_s( szTab, 6, "t%d", 6 );
	else
		sprintf_s( szTab, 6, "t%d", num/10 +1 );

	if (_pStatistics->_pUfsHandle) ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_AppNumInAnPlan, NULL, szTab);
	//Statistic_StatFuncTab(Statistic_ID_AppNumInAnPlan, szTab, ret);
	//CStaticsticsMessageWithTab msg(Statistic_ID_AppNumInAnPlan,szTab);
	//msg.ret = ret;
	//msg.SendAndGetReturn();
	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::PostZJStartTime()
{//INTERFUN;
	bool ret = false;
	time_t t = time(NULL);
	struct tm* times = gmtime(&t);
	char* szTab = new char[6];
	sprintf_s(szTab, 6, "t%d", times->tm_hour+1);

	//Statistic_StatFuncTab( Statistic_ID_ZJStartTime, szTab, ret);
	/*CStaticsticsMessageWithTab msg(Statistic_ID_ZJStartTime,szTab);
	msg.ret = ret;
	msg.SendAndGetReturn();*/
	if (_pStatistics->_pUfsHandle) ret = FunctionCount(_pStatistics->_pUfsHandle, Statistic_ID_ZJStartTime, NULL, szTab);

	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::PostInstallApp( int size, int duration )
{
	bool ret = false;
	char* szTab = new char[36];
	sprintf_s(szTab, 36, "S%dT%d", size, duration);

	//Statistic_StatFuncTab( "900009", szTab, ret);
	/*CStaticsticsMessageWithTab msg("900009",szTab);
	msg.ret = ret;
	msg.SendAndGetReturn();*/
	if (_pStatistics->_pUfsHandle) ret = FunctionCount(_pStatistics->_pUfsHandle,  "900009", NULL, szTab);

	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::PostInstallPlan( int size, int num )
{
	int duration = 0;
	bool ret = false;
	char* szTab = new char[42];

	UIStringFormat::InstallNeedTime(num, size, duration);

	sprintf_s(szTab, 42, "S%dC%dT%d", size, num, duration);

	//Statistic_StatFuncTab( "900010", szTab, ret);
	/*CStaticsticsMessageWithTab msg("900010",szTab);
	msg.ret = ret;
	msg.SendAndGetReturn();*/
	if (_pStatistics->_pUfsHandle)
	{
		ret = FunctionCount(_pStatistics->_pUfsHandle,  "900010", NULL, szTab);
	}
	SAFE_DELETE_ARRAY(szTab);
	return ret;
}

bool StatisticHelp::Handle( const PostDeviceConnectNumMsg* pMsg )
{
	return PostZJDeviceConnectNum();
}

bool StatisticHelp::Handle( const PostStartTimeMsg* pMsg )
{
	return PostZJStartTime();
}

bool StatisticHelp::Handle( const PostInstallAppMsg* pMsg )
{
	return PostInstallApp(pMsg->size,pMsg->duration);
}

bool StatisticHelp::Handle( const PostInstallPlanListMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	
	InstallsAppInfo* appInfo = (InstallsAppInfo*)pMsg->pAppInfo;
	return PostInstallPlanList(appInfo);
}

bool StatisticHelp::Handle( const PostZJAppNumOfAnPlanMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	return PostZJAppNumOfAnPlan(pMsg->num);
}

bool StatisticHelp::Handle( const PostZJInstallDurationMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	return PostZJInstallDuration(pMsg->num,pMsg->allSize);
}

bool StatisticHelp::Handle( const PostZJInstallPlanNumMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	vector<InstallsCaseData*> *pData = (vector<InstallsCaseData*> *)(pMsg->pInstallsCaseDataCollection);
	PostZJInstallPlanNum(*pData);
	return true;
}

bool StatisticHelp::Handle( const PostInstallPlanMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	return PostInstallPlan(pMsg->size,pMsg->num);
}

bool StatisticHelp::PostInstallPlanList( InstallsAppInfo* appInfo )
{
	bool ret = false;
	bool bIos = appInfo->m_strFileType == L"ipa" || appInfo->m_strFileType == L"pxl";	

	int appid = bIos ? ( appInfo->m_fid == 0 ? appInfo->m_itemID : appInfo->m_fid ) : appInfo->m_fid;
	wstring strTab = CStrOperation::IntToWString( appInfo->m_fid ) + L";" + appInfo->m_strPackageID;
	//Statistic_StatFuncTab( "900007", CCodeOperation::UnicodeToUTF_8(strTab).c_str(), ret);
	if (_pStatistics->_pUfsHandle)
	{
		ret = FunctionCount(_pStatistics->_pUfsHandle,  "900007", NULL,  CCodeOperation::UnicodeToUTF_8(strTab).c_str());
	}
	return ret;
}