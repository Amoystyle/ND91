#pragma once

class NoticeMsgThread : public BaseThread
{
public:
	NoticeMsgThread(void);
	~NoticeMsgThread(void);

	void SetNoticeMsg(wstring msg, wstring icon = L"", int millisecond = 2000);
	unsigned long Main();

private:
	wstring _msg;
	wstring _icon;
	int _millisecond;
};
