#include "stdafx.h"
#include "ChangeInstallPositionThread.h"

ChangeInstallPositionThread::ChangeInstallPositionThread( string threadID,  bool bInstallOnPhone)
						: BaseThread(threadID)
{
    _autoDelete = true;
	_bInstallOnPhone = bInstallOnPhone;
}

ChangeInstallPositionThread::~ChangeInstallPositionThread()
{
}

unsigned long ChangeInstallPositionThread::Main()
{
	wstring strInstall = CConfig::GetIniFileValue(CONFIG(_strOptionIni), L"Android", L"installonphone");
	bool bTmp = strInstall == L"1";
	if(bTmp == _bInstallOnPhone)
		return 0;

	CConfig::SetIniFileValue(CONFIG(_strOptionIni), L"Android",  L"installonphone", _bInstallOnPhone? L"1" : L"0");

	RETURN_VALUE_IF(CheckExit(), 0);

	ResponseChangeInstallPositionMsg msg;
	msg.bInstallOnPhone = _bInstallOnPhone;
	msg.PostToUI();

	return 0;
}