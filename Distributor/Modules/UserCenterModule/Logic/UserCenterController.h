#pragma once

class MainView;
class UserCenterController
{
public:
	UserCenterController();
	~UserCenterController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

private:
	MainView* _pMainView;
};