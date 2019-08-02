#include "stdafx.h"
#include "AppDataUpgrade.h"
App_CC_BEGIN
AppDataUpgrade::AppDataUpgrade(const AppData* appdata)
:AppData(appdata)
{
	m_PiracyUrl=L"";
	m_LegalUrl=L"";
	m_PiracyNewVersion=L"";
	m_LegalNewVersion=L"";
	m_FileSize=0;
	m_fid=0;
	m_itunesid=0;
	m_bIsUpgrading=false;
};
AppDataUpgrade::~AppDataUpgrade()
{

}
App_CC_END