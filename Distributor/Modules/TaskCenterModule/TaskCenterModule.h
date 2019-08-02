#pragma once

#ifdef TASKCENTERMODULE_EXPORTS
#define TASKCENTERMODULE_API __declspec(dllexport)
#else
#define TASKCENTERMODULE_API __declspec(dllimport)
#endif

TASKCENTERMODULE_API SourceModule* CreateTaskCenterModule();

class TaskCenterController;
class TaskCenterModule
	: public SourceModule
{

public:
	TaskCenterModule(void);
	~TaskCenterModule();

	static TaskCenterModule* GetInstance();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init( CContainerUI* pLeft, CContainerUI* pMain );

private:
	static TaskCenterModule* _pInstance;
	TaskCenterController* _pController;
};

