#pragma once
#include "UI/DevicePanelCaselistUI.h"

class RecentInstallCaseController
{
public:
	RecentInstallCaseController(wstring deviceID = L"");
	~RecentInstallCaseController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

private:
	DevicePanelCaselistUI* _pDevicePanelCaselistUI;
};