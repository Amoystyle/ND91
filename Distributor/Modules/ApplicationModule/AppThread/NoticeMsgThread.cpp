#include "stdafx.h"
#include "NoticeMsgThread.h"

NoticeMsgThread::NoticeMsgThread(void)
{
}

NoticeMsgThread::~NoticeMsgThread(void)
{
}

void NoticeMsgThread::SetNoticeMsg(wstring msg, wstring icon, int millisecond)
{
	_msg = msg;
	_icon = icon;
	_millisecond = millisecond;
}

unsigned long NoticeMsgThread::Main()
{
// 	NotifyIcon icon = ICON_SUCCESS;
// 	if (_icon!=L"")
// 	{
// 		icon = _icon;
// 	}
	CommonUIFunc::NoticeDialog(_msg, ICON_SUCCESS, _millisecond);

	return 0;
}