#include "stdafx.h"
#include "GetHotAppsThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long GetHotAppsThread::Main()
{
	RETURN_FALSE_IF(_url.empty());
	std::string retData = CNetOperation::Get(_url);

	_VerDatas->clear();
	WebSoftHelp help(_platType);
	bool success = help.Pare24HotSoftDatas(retData, *_VerDatas);
	if (CheckExit())
		return 0;


	WebSoftMsg msg;
	msg.message = WM_WebLegalSoft_24HotHasLoad;
	msg.platType = _platType;
	msg.SendToUI();
	return 0;
}

GetHotAppsThread::GetHotAppsThread( const string id )
:BaseThread(id)
{

}

GetHotAppsThread::~GetHotAppsThread()
{

}
