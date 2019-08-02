#include "stdafx.h"
#include "DeviceContext.h"
#include "DeviceModuleType.h"

DeviceContext::DeviceContext(wstring deviceID, void* phone, CContainerUI* pContainer)
{
	this->_pContainer = pContainer;
	this->_currentType = DeviceModuleType_ConnecGuide;
	this->_deviceID = deviceID;
	this->_nPhone = phone;

	this->Init();
}

DeviceContext::~DeviceContext()
{
	for (map<DeviceModuleType, DeviceModule*>::iterator it = _mapModules.begin(); it != _mapModules.end(); it++)
	{
		DeviceModule* pModule = it->second;
		if(pModule != NULL)
			pModule->Release();
	}
	_mapModules.clear();
}

void DeviceContext::UpdateDeviceID( wstring newKey )
{
	this->_deviceID = newKey;
}

void DeviceContext::ShowModule( DeviceModuleType type )
{
	DeviceModule* pModule = _mapModules[_currentType];
	if(pModule != NULL)
		pModule->Hide();

	pModule = _mapModules[type];

	if(pModule == NULL)
	{
		CStaticsticsMessage statMsg("");
		switch(type)
		{
			case DeviceModuleType_ConnecGuide:
				{
					pModule = CreateConnectGuideModule(this->_deviceID, this->_nPhone);
				}
				break;
			case DeviceModuleType_DeviceInfo:
				{
					pModule = CreateDeviceInfoModule(this->_deviceID, this->_nPhone);
				}
				break;
			case DeviceModuleType_Application:
				{
					statMsg._id = "11001";
					pModule = CreateApplicationModule(this->_deviceID, this->_nPhone);
				}
				break;
			case DeviceModuleType_FileManager:
				{
					statMsg._id = "12001";
					pModule = CreateFileManagerModule(this->_deviceID, this->_nPhone);
				}
				break;
			case DeviceModuleType_BackupAndRestore:
				{
					statMsg._id = "13001";
					pModule = CreateBackUpAndRestoreModule(this->_deviceID, this->_nPhone);
				}
				break;
			default:
				break;
		}
		if(!statMsg._id.empty())
			statMsg.Send();

		pModule->Init(this->_pContainer);

	}

	this->_currentType = type;

	if(pModule != NULL)
	{
		_mapModules[this->_currentType] = pModule;
		pModule->Show();
	}
}

void DeviceContext::HideModule()
{
	DeviceModule* pModule = _mapModules[this->_currentType];

	if(pModule != NULL)
	{
		pModule->Hide();
	}
}

void* DeviceContext::GetPhone()
{
	return _nPhone;
}

wstring DeviceContext::GetDeviceID()
{
	return _deviceID;
}

void DeviceContext::Init()
{
	DeviceModule* pModule = _mapModules[DeviceModuleType_ConnecGuide];
	if(pModule != NULL)
		return;

	pModule = CreateConnectGuideModule(this->_deviceID, this->_nPhone);
	_mapModules[DeviceModuleType_ConnecGuide] = pModule;
	pModule->Init(this->_pContainer);
}

void DeviceContext::RemoveDeviceModule()
{
	for (map<DeviceModuleType, DeviceModule*>::iterator it = _mapModules.begin(); it != _mapModules.end();)
	{
		if(it->first == DeviceModuleType_ConnecGuide)
		{
			it++;
			continue;
		}

		DeviceModule* pModule = it->second;
		if(pModule != NULL)
			pModule->Release();
		it = _mapModules.erase(it);
	}
}

void DeviceContext::InitModule( DeviceModuleType type )
{

	DeviceModule* pModule = _mapModules[type];
	if(pModule != NULL)
		return;

	switch(type)
	{
	case DeviceModuleType_Application:
		{
			pModule = CreateApplicationModule(this->_deviceID, this->_nPhone);
		}
	}

	if(pModule)
	{
		pModule->Init(this->_pContainer);
		_mapModules[type] = pModule;
	}
}
