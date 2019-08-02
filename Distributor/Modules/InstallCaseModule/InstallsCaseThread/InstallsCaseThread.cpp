#include "StdAfx.h"
#include "InstallsCaseThread.h"


InstallCaseModelEx* InstallsCaseThread::pInstallCaseModelEx = NULL;

InstallsCaseThread::InstallsCaseThread(void)
{
}

InstallsCaseThread::~InstallsCaseThread(void)
{
}

void InstallsCaseThread::SetInstallCaseModelEx(InstallCaseModelEx* pAddress)
{
	pInstallCaseModelEx = pAddress;
}
void InstallsCaseThread::SetThreadInfo(int nAction, int nParam, bool bParam, wstring strParam, void* pParam, void* pSpecParam)
{
	_InstallsCaseMsg._nAction = nAction;
	_InstallsCaseMsg._nParam = nParam;
	_InstallsCaseMsg._bParam = bParam;
	_InstallsCaseMsg._strParam = strParam;
	_InstallsCaseMsg._pParam = pParam;
	_InstallsCaseMsg._pSpecParam = pSpecParam;
}

unsigned long InstallsCaseThread::Main()
{
	if (NULL == pInstallCaseModelEx)
	{
		return 0;
	}
Sleep(50);
	if(_InstallsCaseMsg._nAction != Action_LoadWebCase && _InstallsCaseMsg._nAction != Action_CheckIOSUpgrate \
		&& _InstallsCaseMsg._nAction != Action_CheckAndroidUpgrate && _InstallsCaseMsg._nAction != Action_SearchApp)
		pInstallCaseModelEx->_bOperating = true;
	if(_InstallsCaseMsg._nAction==Action_RefreshCase)
		pInstallCaseModelEx->_bRefreshing = true;
	if( _InstallsCaseMsg._nAction==Action_SearchApp )
		pInstallCaseModelEx->_bSearching = true;

	try
	{
		switch (_InstallsCaseMsg._nAction)
		{
		case Action_LoadCase:
			pInstallCaseModelEx->LoadDataInternal();
			break;
		case Action_LoadWebCase:
			pInstallCaseModelEx->LoadWebCaseInternal(_InstallsCaseMsg._bParam);
			break;
		case Action_CheckIOSUpgrate:
			pInstallCaseModelEx->CheckIOSUpgrateInternal();
			break;
		case Action_CheckAndroidUpgrate:
			pInstallCaseModelEx->CheckAndroidUpgrateInternal();
			break;
		case Action_RefreshCase:
			pInstallCaseModelEx->RefreshCaseInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._bParam);
			break;
		case Action_LoadAppList:
			pInstallCaseModelEx->LoadAppListInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._strParam);
			break;
		case Action_SearchApp:
			pInstallCaseModelEx->SearchAppInternal(_InstallsCaseMsg._pParam);
			break;
		case Action_EditCase:
			pInstallCaseModelEx->EditCaseInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._strParam, _InstallsCaseMsg._pParam);
			break;
		case Action_RemoveCase:
			pInstallCaseModelEx->RemoveCaseInternal(_InstallsCaseMsg._pParam);
			break;
		case Action_AddAppToCase:
			pInstallCaseModelEx->AddAppToCaseInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._strParam, _InstallsCaseMsg._bParam, _InstallsCaseMsg._pParam, _InstallsCaseMsg._pSpecParam);
			break;
		case Action_RemoveAppFromCase:
			pInstallCaseModelEx->RemoveAppFromCaseInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._strParam, _InstallsCaseMsg._bParam, _InstallsCaseMsg._pParam);
			break;
		case Action_UpgrateApp:
			pInstallCaseModelEx->UpgrateAppInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._pParam);
			break;
		case Action_UpgrateAppToLegal:
			pInstallCaseModelEx->UpgradeAppToLegalInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._pParam);
			break;
		case Action_InstallApp:
			pInstallCaseModelEx->InstallAppInternal(_InstallsCaseMsg._nParam, _InstallsCaseMsg._strParam, _InstallsCaseMsg._pParam);
			break;
		case Action_AddFilesToLib:
			pInstallCaseModelEx->AddFilesToLibInternal(_InstallsCaseMsg._pParam, _InstallsCaseMsg._pSpecParam);
			break;
		case Action_CheckIOSLegalUpgrade:
			pInstallCaseModelEx->CheckIOSLegalUpgradeInternal();
			break;
		default:
			wstring s = L"";
			break;
		}
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("线程InstallsCaseThread出错"));
	}
	catch (...)
	{
		LOG->WriteDebug(L"InstallCaseModelEx->Action: 线程InstallsCaseThread出错");		
	}

	if(_InstallsCaseMsg._nAction != Action_LoadWebCase && _InstallsCaseMsg._nAction != Action_CheckIOSUpgrate \
		&& _InstallsCaseMsg._nAction != Action_CheckAndroidUpgrate && _InstallsCaseMsg._nAction != Action_SearchApp)
		pInstallCaseModelEx->_bOperating = false;
	if(_InstallsCaseMsg._nAction==Action_RefreshCase)
		pInstallCaseModelEx->_bRefreshing = false;
	if( _InstallsCaseMsg._nAction==Action_SearchApp )
		pInstallCaseModelEx->_bSearching = false;

	return 0;
}