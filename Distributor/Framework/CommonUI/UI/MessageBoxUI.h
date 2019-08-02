#pragma once

class COMMONUI_API MessageBoxUI:public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
	MessageBoxUI(void);
	~MessageBoxUI(void);
	int Show(HWND hParent,wstring stateIcon, wstring notifycontent, int textWidth=200, int nDelayTime=2000);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    int _nDelayTime;

};
