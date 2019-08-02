#pragma once
#include "CBase_ACK.h"

enum  _INSTALLS_CASE_APP_UPDATEABLE_FALG
{
	INSTALLS_CASE_UPDATEABLE_UNKNOW,
	INSTALLS_CASE_UPDATEABLE_YES,
	INSTALLS_CASE_UPDATEABLE_NO
};

enum  _INSTALLS_CASE_APP_LEGALUPDATEABLE_FALG
{
	INSTALLS_CASE_LEGALUPDATEABLE_UNKNOW,
	INSTALLS_CASE_LEGALUPDATEABLE_YES,
	INSTALLS_CASE_LEGALUPDATEABLE_NO
};

enum _INSTALLS_CASE_APP_STATUS
{
	INSTALLS_CASE_STATUS_UNKNOW,
	INSTALLS_CASE_STATUS_NO_DOWNLOAD,
	INSTALLS_CASE_STATUS_DOWNLOADING,
	_INSTALLS_CASE_STATUS_DOWNLOAD_FAIL,
	INSTALLS_CASE_STATUS_HAS_DOWNLAOD,
	_INSTALLS_CASE_STATUS_INSTALLING,
	INSTALLS_CASE_STATUS_INSTALL_FAIL,
	INSTALLS_CASE_STATUS_INSTALL_SUCESS
};
//待安装应用程序信息类
class InstallsAppInfo
{
public:
	InstallsAppInfo(UINT id);
	InstallsAppInfo(InstallsAppInfo* appinfo, bool autodelete);
	InstallsAppInfo(InstallsAppInfo* appinfo,int newID);
	~InstallsAppInfo(){};
public:
	UINT m_id;												//id
	wstring m_strName;										//应用程序名称
	wstring m_strVersion;									//版本号
	unsigned long m_FileSize;								//文件大小
	wstring m_strFileType;									//文件类型 :ipa,pxl,apk...
	time_t m_tDownloadTime;									//下载时间
	_INSTALLS_CASE_APP_UPDATEABLE_FALG m_bUpdateable;		//是否可以更新标志 0
	_INSTALLS_CASE_APP_LEGALUPDATEABLE_FALG m_bLegalUpdateable;	//是否可以正版更新标志 0
	wstring m_Iconpath;										//图标
	bool m_bSelected;										//是否被选中
	_INSTALLS_CASE_APP_STATUS m_status;						//状态
	wstring m_strSavePath;									//文件保存路径			
	wstring m_strPackageID;									//安装包标识名
	wstring m_strDownLoadURL;								//下载地址
	wstring m_strLegalDownLoadURL;							//正版下载地址（正版更新用到）
	wstring m_VerCode;
	wstring m_Purchaser;									//购买者
	int m_isLegalSoft;										//是否正版 1为正版
	wstring m_SupportDevice;								//支持的设备
	wstring m_SDKVersion;
	int m_fid;												//软件id,服务器用
	int m_itemID;
	wstring m_strNewVersion;								//可升级到的版本号
	wstring m_strNewLegalVersion;							//可升级到的正版版本号
	bool m_bRemoving;
	bool m_AutoDelete;
};
//文件列表
class InstallsAppList : public CBase_ACK<InstallsAppInfo>
{
public:
	InstallsAppList(){};
	~InstallsAppList(){};
};