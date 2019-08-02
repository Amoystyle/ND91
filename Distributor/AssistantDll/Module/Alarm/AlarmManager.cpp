#include "pch_module.h"

#ifdef MODULE_ALARM

#include "Module/Alarm/AlarmManager.h"
#include "Module/Alarm/AlarmOperationCommand.h"
#include "Module/Alarm/IosAlarmPlistHelper.h"

#include "Core/International.h"
#include "Module/System/LogOffCommand.h"
#include "Module/Ring/LoadRingsCommand.h"
#include "Common/Path.h"

#define  DEFAULT_URI  L"content://settings/system/ringtone"  
AlarmManager::AlarmManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;
	_version = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
}

AlarmManager::~AlarmManager(void)
{
	for(size_t i = 0; i < _alarmList.size(); i++)
	{
		SAFE_DELETE(_alarmList[i]);
	}
	_alarmList.clear();
}

CommandExecuteResult ND91Assistant::AlarmManager::LoadAlarms()
{
	if(_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		if (!LoadIosAlarms())
            return COMMAND_EXECUTE_FAIL;
	}
	else
	{
		AlarmOperationCommand alarmCmd(_pDevice);
		_alarmList.clear();
		alarmCmd.LoadAlarms(&_alarmList);
        if (alarmCmd.Result() != COMMAND_EXECUTE_SUCCESS)
            return alarmCmd.Result();
	}

	LoadAlarmRings();

	for (deque<AlarmData*>::iterator it = _alarmList.begin(); it!=_alarmList.end(); it++)
	{
		int index = GetAlarmRing((*it));
		(*it)->_ringTittle = _alarmRinglist[index]._ringTittle;
	}
	return COMMAND_EXECUTE_SUCCESS;
}

AlarmData* ND91Assistant::AlarmManager::AddAlarm( AlarmData* pNewAlarm )
{
	if(_pDevice->GetDevInfo()->_deviceType == Android)
	{
		AlarmOperationCommand alarmCmd(_pDevice);
		RETURN_NULL_IF(!alarmCmd.AddAlarm(pNewAlarm));
	}
	AlarmData* newAlarm = pNewAlarm->Clone();
	if(!IsSupportedAlarmOpen())
		newAlarm->_enabled = false;
	int index = GetAlarmRing(pNewAlarm);
	newAlarm->_ringTittle = _alarmRinglist[index]._ringTittle;
	_alarmList.push_back(newAlarm);
	return newAlarm;
}

bool ND91Assistant::AlarmManager::DeleteAlarm( AlarmData* pAlarm )
{
	DEVICE_OS_TYPE devicetype = _pDevice->GetDevInfo()->_deviceType;
	bool result = false;
	if(devicetype == Android)
	{
		AlarmOperationCommand alarmCmd(_pDevice);
		RETURN_FALSE_IF(!alarmCmd.DeleteAlarm(pAlarm));
	}
	for (deque<AlarmData*>::iterator it = _alarmList.begin(); it != _alarmList.end(); it++)
	{
		if( (devicetype == Android && (*it)->Id() == pAlarm->Id()) 
			|| (devicetype == Ios && (*it)->IosGuid() == pAlarm->IosGuid()))
		{
			SAFE_DELETE((*it));
			_alarmList.erase(it);
			return true;
		}
	}
	return false;
}

bool ND91Assistant::AlarmManager::UpdateAlarm( AlarmData* pAlarm )
{
	if(_pDevice->GetDevInfo()->_deviceType == Android)
	{
		AlarmOperationCommand alarmCmd(_pDevice);
		RETURN_FALSE_IF(!alarmCmd.UpdateAlarm(pAlarm));
	}
	int index = GetAlarmRing(pAlarm);
	pAlarm->_ringTittle = _alarmRinglist[index]._ringTittle;
	return true;
}

const deque<wstring> ND91Assistant::AlarmManager::GetAlarmRings()
{
	deque<wstring> tittleList;
	for (deque<AlarmRing>::iterator it = _alarmRinglist.begin(); it != _alarmRinglist.end(); it++)
	{
		tittleList.push_back((*it)._ringTittle);
	}
	return tittleList;
}


void ND91Assistant::AlarmManager::LoadAlarmRings()
{
	_alarmRinglist.clear();
#ifdef MODULE_RING
	AlarmRing defaultRing;
	defaultRing._ringUri = _pDevice->GetDevInfo()->_deviceType==Ios?L"<default>":DEFAULT_URI;
	defaultRing._ringTittle = International::GetString(DEFAULT_RING_TITTLE);
	defaultRing._IsSystem = true;
	_alarmRinglist.push_back(defaultRing);

	DEVICE_OS_TYPE devtype= _pDevice->GetDevInfo()->_deviceType;

	LoadRingsCommand loadRingCmd(_pDevice);
	if(devtype != Ios)
	{
		loadRingCmd.SetAudioType(SYSTEM_RING);
		loadRingCmd.Execute();
		loadRingCmd.SetAudioType(CUSTOM_RING);
		loadRingCmd.Execute();
	}
	else
		loadRingCmd.LoadAlarmRings();

	const deque<RingData*>*  rings = loadRingCmd.GetDataList();
	for(size_t i = 0; i < rings->size(); i++)
	{
		RingData* ringdata = rings->at(i);
		if(devtype == Android && !(ringdata->_ringType & RingData::Alarm))
			continue;

		AlarmRing  alarmring;
		alarmring._ringTittle = ringdata->_title;
		alarmring._ringPath = ringdata->GetFullPath();
		alarmring._IsSystem = ringdata->_IsSystemRing;
		if(devtype == Android)
		{
			alarmring._ringUri = ringdata->_IsSystemRing?L"content://media/internal/audio/media/":L"content://media/external/audio/media/";
			alarmring._ringUri += CStrOperation::IntToWString(ringdata->_Id);
		}
		else
		{
			alarmring._ringUri = CCodeOperation::UTF_8ToUnicode(ringdata->_Guid);

			wstring tittle = International::GetIosRingNameByGuid(alarmring._ringUri);
			if(!tittle.empty())
				alarmring._ringTittle = tittle;

		}
		_alarmRinglist.push_back(alarmring);
	}
#endif
}

int ND91Assistant::AlarmManager::GetAlarmRing( AlarmData* pAlarm )
{
	wstring guid = pAlarm->_ringuri;
	if (guid.find(L"itunes:") == 0)
		guid = guid.substr(7);
	for (size_t i = 0; i< _alarmRinglist.size(); i++)
	{
		if(_alarmRinglist[i]._ringUri == guid)
		{
			return i;
		}
	}
	return 0;
}

void ND91Assistant::AlarmManager::SetAlarmRing( AlarmData* pAlarm, int ringIndex )
{
	RETURN_IF(!pAlarm);
	if (ringIndex>=0 && ringIndex < (int)_alarmRinglist.size())
		pAlarm->_ringuri = _alarmRinglist[ringIndex]._ringUri;
	else if(pAlarm->_ringuri.empty())
	{
		pAlarm->_ringuri = _alarmRinglist[0]._ringUri;
	}
	else
		return;
	if(_pDevice->GetDevInfo()->_deviceType == Ios && _version > 320)
	{
		wstring ringuri = CStrOperation::toLowerW(pAlarm->_ringuri);
		if((int)ringuri.find(L"system:")<0 
			&& (int)ringuri.find(L"itunes:")<0 
			&& (int)ringuri.find(L"default")<0)
		{
			pAlarm->_ringuri = L"itunes:"+pAlarm->_ringuri;
		}
	}
}

bool ND91Assistant::AlarmManager::LoadIosAlarms()
{
	wstring remotepath = _pDevice->GetPath()->GetIosClockPlistPath();
	wstring localPath = _pDevice->GetPath()->GetWorkFolder()+CFileOperation::GetFileName(remotepath);
	if(CFileOperation::IsFileExist(localPath.c_str()))
        CFileOperation::DeleteFile(localPath);

	DownloadFileCommand cmd(_pDevice);
	if(!cmd.Download(localPath, remotepath))
    {
		return false;
    }
	if (!IosPlistConverter::GetXmlFromPlist(localPath, localPath))
        return false;

	IosAlarmPlistHelper::ParseAlarms(localPath, _alarmList, _version > 320);
    return true;
}

bool ND91Assistant::AlarmManager::SyncToPhone()
{
	wstring remotepath = _pDevice->GetPath()->GetIosClockPlistPath();
	wstring localPath = _pDevice->GetPath()->GetWorkFolder()+CFileOperation::GetFileName(remotepath);
	IosAlarmPlistHelper::WriteAlarmPlist(localPath, _alarmList, _version > 320);
	DirectoryOperationCommand dircmd(_pDevice);
	dircmd.DeleteFileLink(remotepath);
	UploadFileCommand cmd(_pDevice);
	if(cmd.Upload(localPath, remotepath))
	{
		LogOffCommand restart(_pDevice);
		restart.Execute();
		return true;
	}
	return false;
}

bool ND91Assistant::AlarmManager::IsSupportedAlarmOpen()
{
	if(_pDevice->GetDevInfo()->_deviceType == Ios && _version > 320)
		return false;
	return true;
}

#endif