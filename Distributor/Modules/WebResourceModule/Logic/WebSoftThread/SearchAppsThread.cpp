#include "stdafx.h"
#include "SearchAppsThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long SearchAppsThread::Main()
{
	RETURN_FALSE_IF(_url.empty());
	std::string retData = CNetOperation::Get(_url);
	_VerDatas->clear();

	WebSoftHelp help(_platType);
	bool success = help.PareSoftDatas(retData, *_VerDatas, *_total, true);
	if (CheckExit())
		return 0;

	WebSoftMsg msg;
	msg.message = WM_WebLegalSoft_SearchAppsHasLoad;
	msg.platType = _platType;
	msg.SendToUI();

	return 0;
}

SearchAppsThread::SearchAppsThread( const string id )
:BaseThread(id)
{

}

SearchAppsThread::~SearchAppsThread()
{

}
