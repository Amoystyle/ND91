#pragma once


class InstallCaseTipWindow:public CWindowUI
{
public:
	InstallCaseTipWindow(void);
	~InstallCaseTipWindow(void);

	static InstallCaseTipWindow* GetInstance();

	void Show(HWND hParent, RECT rect, wstring name, wstring size, int cnt, wstring installtimestr);

	void Hide();

private:

	static  InstallCaseTipWindow* _instance;

};
