#pragma once

class MainView;
class ModuleController
	: public BaseCSLock
	, BaseHandler<ModuleManagerInnerMsg>
{
public:
	ModuleController();
	~ModuleController();

	bool Init(CContainerUI* pTitleToolBar, CContainerUI* pMain);

	void Show();

	void Hide();

	void LoadResource();

	void LoadModules();

private:
	virtual bool Handle( const ModuleManagerInnerMsg* pMsg );

	void SelectModule( SourceModule* pModule );
private:
	CContainerUI* _pParent;
	CContainerUI* _pTitleToolBar;
	MainView* _pMainView;
	SourceModule* _pSelectModule;
};