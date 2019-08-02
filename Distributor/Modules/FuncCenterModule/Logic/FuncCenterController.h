#pragma once

class DownAndExecuteFuncThread;
class MainView;
class FuncCenterController 
{
public:
	FuncCenterController(wstring deviceID, void* phone);
	~FuncCenterController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

	void Release();

	wstring GetDeviceID();

private:
	wstring _deviceID;
	void* _nPhone;

	MainView* _pMainView;
};