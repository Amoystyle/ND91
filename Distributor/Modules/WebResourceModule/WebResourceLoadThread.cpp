#include "StdAfx.h"
#include "WebResourceLoadThread.h"
#include "WebResourceManager.h"
#include "Logic/WebResourceInnerMsg.h"
#include "Base/BaseCSLock.h"

CWebResourceLoadThread::CWebResourceLoadThread(const string threadId)
:BaseThread(threadId)
{
}

CWebResourceLoadThread::~CWebResourceLoadThread(void)
{
}

unsigned long CWebResourceLoadThread::Main()
{
	AutoLock;

	CWebResourceManager::GetInstance()->Get();
	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_LeftTreeLoaded;
	msg.lParam = this->_platform;
	msg.wParam = this->_appSort;
	msg.SendToUI();

	return 0;
}

NetEnumAppSort CWebResourceLoadThread::getAppSort()
{
	return _appSort;
}


void CWebResourceLoadThread::setAppSort(NetEnumAppSort var)
{
	_appSort = var;
}

ENUM_PLATFORM CWebResourceLoadThread::getPlatform()
{
	return _platform;
}

void CWebResourceLoadThread::setPlatform(ENUM_PLATFORM var)
{
	_platform = var;
}