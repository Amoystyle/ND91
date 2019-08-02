#include "stdafx.h"
#include "abl.h"

#include "Connection/ConnectionManager.h"
#include "Application/ApplicationManager.h"
#include "DeviceInfo/DeviceInfoManager.h"
#include "FuncCenter/NetInterfaceManager.h"
#include "Statistics/Statistics.h"
#include "Statistics/StatisticHelp.h"
#include "Statistics/TaskHelpModel.h"
Cabl* Cabl::_pInstance = NULL;

Cabl::Cabl()
{
	_pConnectionManager = new ConnectionManager();
	_pDeviceInfoManager = new DeviceInfoManager();
	_pApplicationManager = new ApplicationManager();
	_statistics = new CStatistics;
	_pStatisticHelp = new StatisticHelp(_statistics);
	_pNetInterfaceManager = new NetInterfaceManager;
	_pTaskHelpModel		= TaskHelpModel::GetInstance();
	return;
}

bool Cabl::Init()
{
	bool bRet = false;
	try
	{
		_pConnectionManager->Init();		
		_pApplicationManager->Init();
		_pNetInterfaceManager->Init();
		bRet = true;
	}
	catch (AssistantException* e)
	{
		
	}
	catch (...)
	{
	}
	return bRet;
}

Cabl* Cabl::GetInstance()
{
	if (!_pInstance)
		_pInstance = new Cabl();
	return _pInstance;
}

Cabl::~Cabl( void )
{
	SAFE_DELETE(this->_pConnectionManager);
	SAFE_DELETE(this->_pDeviceInfoManager);
	SAFE_DELETE(this->_pApplicationManager);
	SAFE_DELETE(this->_statistics);
	SAFE_DELETE(this->_pNetInterfaceManager);
	SAFE_DELETE(this->_pTaskHelpModel);
}
