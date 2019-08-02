#include "stdafx.h"
#include "DependenceFuncCenter.h"
#include "DependenceFuncCenterController.h"
#include "FeedbackController.h"
#include "SysSettingController.h"
#include "VersionController.h"
#include "ClearTempCenter.h"
#include "CloseMutex.h"

DependenceFuncCenter* DependenceFuncCenter::_pInstance = NULL;


DependenceFuncCenter::DependenceFuncCenter( void )
{
	_pDependenceFuncCenterController = new DependenceFuncCenterController();
	//ÆÁ±Î·´À¡°´Å¥
	//_pFeedbackController = new FeedbackController();
	_pSysSettingController = new SysSettingController;
	_pVersionController = new VersionController;
	_pClearTempCenter = new ClearTempCenter;
	_pCloseMutex = new CloseMutex;

}

DependenceFuncCenter::~DependenceFuncCenter()
{

}

DependenceFuncCenter* DependenceFuncCenter::GetInstance()
{
	if(_pInstance == NULL)
	{
		_pInstance = new DependenceFuncCenter();
	}
	return _pInstance;
}

void DependenceFuncCenter::Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar)
{
	_pDependenceFuncCenterController->Init(pTitleToolBar, pStatusBar);

	_pFeedbackController->Init(pTitleToolBar, pStatusBar);

	_pSysSettingController->Init(pTitleToolBar, pStatusBar);

	_pVersionController->Init(pTitleToolBar, pStatusBar);

	_pClearTempCenter->Init();

	_pCloseMutex->Init();
}

tstring DependenceFuncCenter::Id()
{
	return L"DependenceFuncCenter";
}
