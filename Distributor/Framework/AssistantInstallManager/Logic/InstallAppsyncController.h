#pragma once
#include "Common/UserCenter/UserCenterMsg.h"



class InstallAppsyncController
	: public BaseHandler<RequestInstallAppSyncMsg>
	, public BaseHandler<RequestInstallDeamonOverMsg>
	, public BaseGetData<RequestNeedInstallAppSyncMsg>
	
{
public:
	InstallAppsyncController();
	~InstallAppsyncController();

	virtual bool Handle( const RequestInstallAppSyncMsg* pMsg );

	bool Handle( RequestNeedInstallAppSyncMsg* pMsg );


	virtual bool Handle(const RequestInstallDeamonOverMsg* pMsg );

private:
	map<wstring,HANDLE> m_event;


};