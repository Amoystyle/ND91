#pragma once
#include "UI/MainView.h"
#include "UI/LeftView.h"
class InstallCaseController : public IExit
{
public:
	InstallCaseController();
	~InstallCaseController();

	bool Init(CContainerUI* pLeft, CContainerUI* pContainer);

	void Show();

	void Hide();

	void Exit(); 
private:
	InstallCaseUI* _pInstallCaseUI;// mainviewÎÄ¼þ
	LeftView*      _pLeftView;
};