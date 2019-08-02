#include "stdafx.h"
#include "InstallsCaseData.h"


InstallsCaseData::InstallsCaseData(UINT id)
{INTERFUN;
	m_id = id;
	m_ListApp = new InstallsAppList();
	m_InstallCount=0;
	m_FolderPath=L"";
	mPlanType = _INSTALL_PLAN_TYPE_SYS;
	PackId = 0;
};
InstallsCaseData::~InstallsCaseData()
{INTERFUN;
	m_ListApp->Empty();
	SAFE_DELETE(m_ListApp);
}
InstallsAppInfo * InstallsCaseData::GetAppByID(UINT id)
{INTERFUN;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if(appinfo->m_id == id)
			return appinfo;
	}
	return NULL;
}
InstallsAppInfo * InstallsCaseData::GetAppByAppInfo(InstallsAppInfo * oldappinfo)
{INTERFUN;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if(appinfo->m_strPackageID == oldappinfo->m_strPackageID && appinfo->m_strVersion == oldappinfo->m_strVersion)
			return appinfo;
	}
	return NULL;
}

InstallsAppInfo * InstallsCaseData::GetAppBySavePath(wstring path)
{INTERFUN;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if(appinfo->m_strSavePath == path)
			return appinfo;
	}
	return NULL;
}

InstallsAppInfo * InstallsCaseData::GetAppByName(wstring name)
{INTERFUN;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if(appinfo->m_strName == name)
			return appinfo;
	}
	return NULL;
}

UINT InstallsCaseData::GetAppNum()
{INTERFUN;
	return m_ListApp->size();
}
bool InstallsCaseData::AddAppToList(InstallsAppInfo * appinfo)
{INTERFUN;
	if(_INSTALLS_CASE_DATA_IOS==this->CaseType() && 1==appinfo->m_isLegalSoft && 1!=this->IsLegalPlan() && _INSTALL_PLAN_TYPE_USER==this->PlanType())
	{
		this->IsLegalPlan(1);
	}
	m_ListApp->m_list.push_back(appinfo);
	return true;
}

bool InstallsCaseData::RemoveAppByid(UINT id)
{INTERFUN;
	if(m_ListApp->size()==0)
		return true;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();)
	{
		InstallsAppInfo *appinfo = *it;
		if(appinfo->m_id == id)
		{
			//appinfo->m_bRemoving = true;
			//delete *it;
			if(_INSTALLS_CASE_DATA_IOS==this->CaseType() && 1==appinfo->m_isLegalSoft && 1==this->IsLegalPlan() && _INSTALL_PLAN_TYPE_USER==this->PlanType())
			{
				bool isLegalPlan = false;
				for(InstallsAppList::iterator it1 = m_ListApp->begin();it1 != m_ListApp->end(); it1++)
				{
					InstallsAppInfo *tappinfo = *it1;
					if(tappinfo->m_id == appinfo->m_id)
						continue;
					if(1==tappinfo->m_isLegalSoft)
					{
						isLegalPlan = true;
						break;
					}
				}
				if(!isLegalPlan)
				{
					this->IsLegalPlan(0);
				}
			}
			it = m_ListApp->erase(it);
			return true;
		}
		else
		{
			it++;
		}
	}
	return false;
}

bool InstallsCaseData::RemoveAppByPath( wstring path )
{
	INTERFUN;
	if(m_ListApp->size()==0)
		return true;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();)
	{
		InstallsAppInfo *appinfo = *it;
		if(CStrOperation::CompareUnCase(appinfo->m_strSavePath, path)==0)
		{			
			//delete *it;
			it = m_ListApp->erase(it);
			if(_INSTALLS_CASE_DATA_IOS==this->CaseType() && 1==appinfo->m_isLegalSoft && 1==this->IsLegalPlan() && _INSTALL_PLAN_TYPE_USER==this->PlanType())
			{
				bool isLegalPlan = false;
				for(InstallsAppList::iterator it1 = m_ListApp->begin();it1 != m_ListApp->end(); it1++)
				{
					InstallsAppInfo *tappinfo = *it1;
					if(1==tappinfo->m_isLegalSoft)
					{
						isLegalPlan = true;
						break;
					}
				}
				if(!isLegalPlan)
				{
					this->IsLegalPlan(0);
				}
			}
			return true;
		}
		else
		{
			it++;
		}
	}
	return false;
}

bool InstallsCaseData::setSelect(UINT id)
{INTERFUN;
	InstallsAppInfo *appinfo = GetAppByID(id);
	appinfo->m_bSelected = true;
	return true;
}
bool InstallsCaseData::setUnSelect(UINT id)
{INTERFUN;
	InstallsAppInfo *appinfo = GetAppByID(id);
	appinfo->m_bSelected = false;
	return true;
}
unsigned long InstallsCaseData::GetSumFileSize()
{INTERFUN;
	unsigned long totalsize=0;
	for(InstallsAppList::iterator it = m_ListApp->begin();it != m_ListApp->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		totalsize+= appinfo->m_FileSize;
	}
	return totalsize;
}