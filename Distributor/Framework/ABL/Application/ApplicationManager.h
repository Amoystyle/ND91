#pragma once

class ApplicationManager
	: public BaseCSLock
	, public BaseGetData<RequestLoadAppMsg>
	, public BaseHandler<RequestInstallAppMsg>
{
public:
	ApplicationManager();

	~ApplicationManager();

	void Init();

	virtual bool Handle( RequestLoadAppMsg* pMsg );

	virtual bool Handle( const RequestInstallAppMsg* msg );

};