#pragma once
#ifdef DEPENDENCEFUNCCENTER_EXPORTS
#define DEPENDENCEFUNCCENTER_API __declspec(dllexport)
#else
#define DEPENDENCEFUNCCENTER_API __declspec(dllimport)
#endif


class DependenceFuncCenterController;
class FeedbackController;
class SysSettingController;
class VersionController;
class ClearTempCenter;
class CloseMutex;
class DEPENDENCEFUNCCENTER_API DependenceFuncCenter 
{
public:
	DependenceFuncCenter(void);
	~DependenceFuncCenter();

	virtual tstring Id();

	static DependenceFuncCenter* GetInstance();

	void Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar);

private:
	static DependenceFuncCenter* _pInstance;
	DependenceFuncCenterController *_pDependenceFuncCenterController;
	FeedbackController *_pFeedbackController;
	SysSettingController* _pSysSettingController;
	VersionController* _pVersionController;
	ClearTempCenter* _pClearTempCenter;
	CloseMutex* _pCloseMutex;
};