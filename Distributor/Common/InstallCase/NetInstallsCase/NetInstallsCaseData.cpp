#include "stdafx.h"
#include "NetInstallsCaseData.h"


NetInstallsCaseData::NetInstallsCaseData(UINT id)
	:InstallsCaseData(id)
{INTERFUN;
	PlanType(_INSTALL_PLAN_TYPE_NET);
	NetInstallStatus = _NET_INSTALL_PLAN_STATUS_INIT;
	_progressPer = 0;
	NetAppItems = new InstallsAppList();
	_wsPackegeIconpath = L"";
	_bIsCpa = false;
};
NetInstallsCaseData::~NetInstallsCaseData()
{INTERFUN;
	SAFE_DELETE(NetAppItems);
}

bool NetInstallsCaseData::AddNetAppToList(InstallsAppInfo * appinfo)
{INTERFUN;

	NetAppItems->m_list.push_back(appinfo);
	return true;
}

UINT NetInstallsCaseData::GetAppNum()
{INTERFUN;
	return NetAppItems->size() + ListApp()->size();
}

unsigned long NetInstallsCaseData::GetSumFileSize()
{INTERFUN;
	unsigned long totalsize=0;

	for(InstallsAppList::iterator it = ListApp()->begin();it != ListApp()->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		totalsize+= appinfo->m_FileSize;
	}
	for(InstallsAppList::iterator it = NetAppItems->begin();it != NetAppItems->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		totalsize+= appinfo->m_FileSize;
	}

	return totalsize;
}
