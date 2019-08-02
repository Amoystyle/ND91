#include "stdafx.h"
#include "NoticeMsgThread.h"

USING_NSTALLCASE_Space

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
// 	wstring icon = L"#InstallCase_Image_icon_TipSuccess";
// 	if (_icon!=L"")
// 	{
// 		icon = _icon;
// 	}
	CommonUIFunc::NoticeDialog(_msg, ICON_SUCCESS, _millisecond);

	return 0;
}