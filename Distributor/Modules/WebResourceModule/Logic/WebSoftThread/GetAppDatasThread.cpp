#include "stdafx.h"
#include "GetAppDatasThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long GetAppDatasThread::Main()
{

	RETURN_FALSE_IF(_url.empty());
	std::string retData = CNetOperation::Get(_url);

	_VerDatas->clear();
	WebSoftHelp help(_platType);
	bool success = help.PareSoftDatas(retData, *_VerDatas, *_total);
	if (CheckExit())
		return 0;

	WebSoftMsg msg;
	msg.message = WM_WebLegalSoft_AppDatasHasLoad;
	msg.platType = _platType;
	msg.SendToUI();

	return 0;
}

GetAppDatasThread::GetAppDatasThread( const string id )
:BaseThread(id)
{

}

GetAppDatasThread::~GetAppDatasThread()
{

}
