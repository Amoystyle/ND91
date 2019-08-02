#include "stdafx.h"
#include "GetSoftCategorysThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long GetSoftCategorysThread::Main()
{
	RETURN_FALSE_IF(_url.empty());
	
	std::string retData = CNetOperation::Get(_url);

	WebSoftHelp help(_platType);
	bool success = help.PareCategoryData(retData, *_VerCats);
	if (CheckExit())
		return 0;

	WebSoftMsg msg;
	msg.message = WM_WebLegalSoft_CategorysHasLoad;
	msg.platType = _platType;
	msg.SendToUI();
	return 0;
}

GetSoftCategorysThread::GetSoftCategorysThread( const string id )
:BaseThread(id)
{

}

GetSoftCategorysThread::~GetSoftCategorysThread()
{

}
