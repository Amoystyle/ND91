#pragma once

class WebUIReceiver
{
public:
	WebUIReceiver(void);
	~WebUIReceiver(void);

	virtual void SetReceive(INotifyUI* pINotify = NULL);

protected:
	void Send(UINT message, CControlUI* pSender = NULL,  WPARAM wParam = NULL, LPARAM lParam = NULL);

private:
	INotifyUI* _pReceiveUI;
};


class WebSoftMessageReceiver
{
public:
	WebSoftMessageReceiver();
	~WebSoftMessageReceiver(void);

public:
	static void DoMessage(UINT message, WPARAM wParam, LPARAM lParam);
	static CRITICAL_SECTION	_pLock;

protected:
	virtual void MessageProc(UINT message, WPARAM wParam, LPARAM lParam){}

private:
	static vector<WebSoftMessageReceiver*> Receivers;
};
