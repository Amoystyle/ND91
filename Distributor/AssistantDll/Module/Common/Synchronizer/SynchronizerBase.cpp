#include "stdafx.h"
#include "SynchronizerBase.h"

#include "Core/DeviceData.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"
#include "Device/IosPublicFunc.h"

#include "Module/Common/Synchronizer/DeviceLink.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

SynchronizerBase::SynchronizerBase(const DeviceData* pDevice)
{
    _shortDataClass     = "";
    _dataClass          = "";
    _dataClassAnchor    = "";
    _dataClassVersion   = 0;
    //_useCache           = false;
    //_isCacheExpired     = true;
    _mapEntities.clear();
    _sendInfo.clear();
    _actions.clear();

    _pDevice            = pDevice;
    _device             = new IosUsbConnection((DeviceInfo*)pDevice->GetDevInfo());
    _syncClient         = new DeviceSyncClient(_device);
}

SynchronizerBase::~SynchronizerBase(void)
{
    ReleaseSendInfo();
    SAFE_DELETE(_syncClient);
    SAFE_DELETE(_device);
}

bool SynchronizerBase::SaveChanges()
{
    bool result = false;
    try
    {
        result = LoadAllData(true, false);

        if (result)
            result = _syncClient->IsReadyToReceiveChangesFromComputer();

        if (result)
            DoSaveChanges();
    }
    catch (...)
    {
        //_isCacheExpired = true;
    }

    result = _syncClient->FinishSync();
    _syncClient->StopSync();

    return result;
}

bool SynchronizerBase::RemoveAllData()
{
    bool result = false;
    try
    {
        result = PrepareSync(true);
        result = _syncClient->ClearAllRecordsOnDevice();
    }
    catch (...)
    {
    }

    result = _syncClient->FinishSync();
    _syncClient->StopSync();
    //_isCacheExpired = true;

    return result;
}

bool SynchronizerBase::LoadAllData(bool useFastSync, bool parseData)
{
    if (!PrepareSync(useFastSync))
    {
        LOG->WriteDebug(L"PrepareSync failure");
        return false;
    }

    switch (_syncClient->GetDeviceSyncType())
    {
    case DeviceSyncClient::Fast:
        {
            if (useFastSync)
            {
                if (!_syncClient->GetChangesFromDevice())
                    return false;
            }
            else
            {
                if (!_syncClient->GetAllRecordsFromDevice())
                    return false;
            }
        }
        break;
    case DeviceSyncClient::Reset:
        {
            if (!_syncClient->GetAllRecordsFromDevice())
                return false;
        }
        break;
    case DeviceSyncClient::Slow:
    case DeviceSyncClient::Unknow:
        {
            if (!_syncClient->GetAllRecordsFromDevice())
                return false;
        }
        break;
    default:
        break;
    }

    bool isHaveRecord = false;
    if (parseData)
        ClearCachedData();

    do 
    {
        ReleaseEntities();
        if (!_syncClient->ReceiveChangesEntitiesFromDevice(_mapEntities, isHaveRecord)) // failed to read changesEntities from device
        {
            LOG->WriteDebug(L"ReceiveChangesEntitiesFromDevice failure");
            return false;
        }

        if (!_syncClient->AcknowledgeChangesFromDevice()) // failed to send AcknowledgeChanges to device
        {
            LOG->WriteDebug(L"AcknowledgeChangesFromDevice failure");
            return false;
        }

        if (parseData)
            ParsePlistEntities();

    } while (isHaveRecord);

    return true;
}


bool SynchronizerBase::PrepareSync(bool useFastSync)
{
    if (_syncClient->GetDeviceSyncDirection() != DeviceSyncClient::Unknown)
        _syncClient->StopSync();

    DeviceSyncAnchors anchors;
    if (useFastSync && !_syncClient->GetDeviceAnchor().empty())
        anchors._deviceAnchor = _syncClient->GetDeviceAnchor();
    else
        anchors._deviceAnchor = _dataClassAnchor;

    time_t time;
    ::time(&time);
    CDealTime nowTime(time+8*3600);
    anchors._computerAnchor = nowTime.TimeFormat() + " +0800";

    int deviceDataClassVersion = 0;
    return _syncClient->StartSync(_dataClass,anchors,_dataClassVersion,&deviceDataClassVersion);
}

bool SynchronizerBase::IsDisableSync(bool tryEnableIfDisabled)
{
    bool isDisableSync = _syncClient->IsDataClassDisableSync(_shortDataClass, tryEnableIfDisabled);
    if (isDisableSync)
        return isDisableSync;

    try
    {
        //由于书签这样的怪异处理,因此,即便disable 是false,也要尝试实际真的是否可以同步
        PrepareSync(true);
    }
    catch (...)
    {
        isDisableSync = true;
    }

    _syncClient->FinishSync();
    _syncClient->StopSync();

    return isDisableSync;
}

bool SynchronizerBase::IsEnableiCloud()
{
    if (_shortDataClass == "Notes")
        if (IosPublicFunc::GetVerNum(_device->GetDeviceInfo()->_deviceProductVersion) < 400)
            return false;

    return _syncClient->IsDataClassEnableiCloud(_shortDataClass);
}

bool SynchronizerBase::LoadAllData()
{
//     if (_useCache && !_isCacheExpired)
//         return true;

    bool res = false;
    try
    {
        res = LoadAllData(false, true);
        //_isCacheExpired = !res;
    }
    catch (...)
    {
        LOG->WriteDebug(L"SynchronizerBase::LoadAllData() failure");
    }

    res = _syncClient->IsReadyToReceiveChangesFromComputer();
    res = _syncClient->FinishSync();
    _syncClient->StopSync();

    return res;
}

void SynchronizerBase::ReleaseEntities()
{
    for (map< string, map<string,void*> >::iterator iter = _mapEntities.begin(); iter != _mapEntities.end(); iter++)
        CCFConverter::ClearMapRecursive(iter->second);
    _mapEntities.clear();
}

void SynchronizerBase::ReleaseSendInfo()
{
    CCFConverter::ClearVecRecursive(_sendInfo);
}

/*
map<string,string> SynchronizerBase::BuildActions(vector<string> linkEntityNamesKey, bool linkAllRecordsOfPulledEntityTypeSentKey)
{
    for (size_t i = 0; i < linkEntityNamesKey.size(); i++)
        _actions.insert(pair<string, string>("SyncDeviceLinkEntityNamesKey",linkEntityNamesKey.at(i)));

    string temp = "";
    linkAllRecordsOfPulledEntityTypeSentKey == true ? temp = "true" : temp = "false";
    _actions.insert(pair<string, string>("SyncDeviceLinkAllRecordsOfPulledEntityTypeSentKey",temp));
    return _actions;
}
*/