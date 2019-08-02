#include "stdafx.h"
#include "InstallsAppInfo.h"


InstallsAppInfo::InstallsAppInfo(UINT id)
{INTERFUN;
	m_id = id;												
	m_strName = L"";										
	m_strVersion = L"";									
	m_FileSize=0;									
	m_strFileType=L"";									
	m_tDownloadTime=0;									
	m_bUpdateable = INSTALLS_CASE_UPDATEABLE_UNKNOW;
	m_bLegalUpdateable = INSTALLS_CASE_LEGALUPDATEABLE_UNKNOW;
	m_Iconpath = L"";										
	m_bSelected = false;									
	m_status = INSTALLS_CASE_STATUS_UNKNOW;	
	m_Purchaser=L"";
	m_isLegalSoft=0;
	m_SDKVersion=L"";
	m_SupportDevice=L"";
	m_fid=0;
	m_itemID=0;
	m_strNewVersion=L"";
	m_strNewLegalVersion=L"";
	m_bRemoving=false;
	m_AutoDelete=false;
}

InstallsAppInfo::InstallsAppInfo(InstallsAppInfo* appinfo,int newID)
{INTERFUN;
	//构建新的app数据
	this->m_id=newID;
	this->m_bUpdateable = appinfo->m_bUpdateable;
	this->m_bLegalUpdateable = appinfo->m_bLegalUpdateable;
	this->m_FileSize = appinfo->m_FileSize;
	this->m_Iconpath = appinfo->m_Iconpath;
	this->m_status = INSTALLS_CASE_STATUS_UNKNOW;
	this->m_strDownLoadURL = appinfo->m_strDownLoadURL;
	this->m_strLegalDownLoadURL = appinfo->m_strLegalDownLoadURL;
	this->m_strFileType = appinfo->m_strFileType;
	this->m_strName = appinfo->m_strName;
	this->m_strPackageID = appinfo->m_strPackageID;
	this->m_strSavePath = appinfo->m_strSavePath;
	this->m_strVersion = appinfo->m_strVersion;
	this->m_tDownloadTime = appinfo->m_tDownloadTime;
	this->m_bSelected = false;
	this->m_Purchaser=appinfo->m_Purchaser;
	this->m_isLegalSoft=appinfo->m_isLegalSoft;
	this->m_SupportDevice=appinfo->m_SupportDevice;
	this->m_SDKVersion=appinfo->m_SDKVersion;
	this->m_fid=appinfo->m_fid;
	this->m_itemID=appinfo->m_itemID;
	this->m_bRemoving=appinfo->m_bRemoving;
	this->m_strNewVersion=appinfo->m_strNewVersion;
	this->m_strNewLegalVersion=appinfo->m_strNewLegalVersion;
	this->m_VerCode=appinfo->m_VerCode;
	this->m_AutoDelete=false;
}

InstallsAppInfo::InstallsAppInfo( InstallsAppInfo* appinfo, bool autoDelete)
{INTERFUN;
//构建新的app数据
	this->m_id=appinfo->m_id;
	this->m_bUpdateable = appinfo->m_bUpdateable;
	this->m_bLegalUpdateable = appinfo->m_bLegalUpdateable;
	this->m_FileSize = appinfo->m_FileSize;
	this->m_Iconpath = appinfo->m_Iconpath;
	this->m_status = appinfo->m_status;
	this->m_strDownLoadURL = appinfo->m_strDownLoadURL;
	this->m_strLegalDownLoadURL = appinfo->m_strLegalDownLoadURL;
	this->m_strFileType = appinfo->m_strFileType;
	this->m_strName = appinfo->m_strName;
	this->m_strPackageID = appinfo->m_strPackageID;
	this->m_strSavePath = appinfo->m_strSavePath;
	this->m_strVersion = appinfo->m_strVersion;
	this->m_tDownloadTime = appinfo->m_tDownloadTime;
	this->m_bSelected = appinfo->m_bSelected;
	this->m_Purchaser=appinfo->m_Purchaser;
	this->m_isLegalSoft=appinfo->m_isLegalSoft;
	this->m_SupportDevice=appinfo->m_SupportDevice;
	this->m_SDKVersion=appinfo->m_SDKVersion;
	this->m_fid=appinfo->m_fid;
	this->m_itemID=appinfo->m_itemID;
	this->m_bRemoving=appinfo->m_bRemoving;
	this->m_strNewVersion=appinfo->m_strNewVersion;
	this->m_strNewLegalVersion=appinfo->m_strNewLegalVersion;
	this->m_VerCode=appinfo->m_VerCode;
	this->m_AutoDelete = true;
}
