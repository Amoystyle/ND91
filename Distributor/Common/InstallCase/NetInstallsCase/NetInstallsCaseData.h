#pragma once

#include "../InstallsCase/InstallsCaseData.h"

/// <summary>
/// ����װ������״̬
/// </summary>
enum _NET_INSTALL_PLAN_STATUS
{
	/// <summary>
	/// ����
	/// </summary>
	_NET_INSTALL_PLAN_STATUS_INIT = 0,
	/// <summary>
	/// ������
	/// </summary>
	_NET_INSTALL_PLAN_STATUS_DOWNLOADING = 1,
	/// <summary>
	/// �������
	/// </summary>
	_NET_INSTALL_PLAN_STATUS_DOWNLOADED = 2
};

//����
class NetInstallsCaseData : public InstallsCaseData
{
public:
	NetInstallsCaseData(UINT id);
	~NetInstallsCaseData();


	wstring _wsPackegeIconpath;
	_NET_INSTALL_PLAN_STATUS NetInstallStatus; 

	InstallsAppList* NetAppItems;


	bool AddNetAppToList(InstallsAppInfo * appinfo);
	virtual UINT GetAppNum();
	virtual unsigned long GetSumFileSize();

	int GetDownProgress(){return _progressPer;}
	void SetbCpa(bool bIsCpa=false){_bIsCpa=bIsCpa;}
	bool IsbCpa(){return _bIsCpa;}
	void SetWebCpaUrl(string strUrl){_strUrl = strUrl;}
	string GetWebCpaUrl(){return _strUrl;}
private:

	int    _progressPer;
	bool   _bIsCpa;
	string _strUrl; 
};
