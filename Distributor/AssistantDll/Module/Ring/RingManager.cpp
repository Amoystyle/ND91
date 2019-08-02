#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/RingManager.h"
#include "Module/Ring/LoadRingsCommand.h"
#include "Module/Ring/RingOperationCommand.h"
#include "Module/Ring/IosUserRingData.h"
#include "Module/Ring/IosSystemRingData.h"
#include "Module/Ring/AndroidRingData.h"

#include "Module/System/GetBaseSettingCommand.h"
#include "Module/System/GetProfilesCommand.h"
#include "Module/System/ProfileData.h"
#include "Common/Path.h"

RingManager::RingManager( const DeviceData* pDevice )
: IManager (pDevice)
{

}

RingManager::~RingManager()
{

}

CommandExecuteResult ND91Assistant::RingManager::Load()
{
    Release();
    LoadRingsCommand cmd(_pDevice);
    cmd.SetAudioType(SYSTEM_RING);
    cmd.Execute();
    cmd.SetAudioType(CUSTOM_RING);
    cmd.Execute();
    const deque<RingData*>* plist = cmd.GetDataList();

    if (_pDevice->GetDevInfo()->_deviceType == Android)
    {
        wstring sdcardPath=L"/sdcard/";
        RingDirData*  callring = new RingDirData(_pDevice, CALL_RING, sdcardPath + L"ringtones");
        RingDirData*  smsring = new RingDirData(_pDevice, SMS_RING, sdcardPath + L"notifications");
        RingDirData*  alarmring = new RingDirData(_pDevice,ALARM_RING, sdcardPath + L"alarms");

        for(size_t i=0; i< plist->size(); i++)
        {
            switch(plist->at(i)->_ringType)
            {
            case RingData::Ring:
                callring->Insert(plist->at(i));
                break;
            case RingData::Notif:
            case RingData::Mail:
            case RingData::Calendar:
            case RingData::Reminder:
            case RingData::Fetion:
                smsring->Insert(plist->at(i));
                break;
            case RingData::Alarm:
                alarmring->Insert(plist->at(i));
                break;
            case RingData::Music:
                break;
            default:
                {
                    if ((plist->at(i)->_ringType&RingData::Ring) == RingData::Ring)
                        callring->Insert(plist->at(i));
                    if ((plist->at(i)->_ringType&RingData::Notif) == RingData::Notif)
                        smsring->Insert(plist->at(i));
                    if ((plist->at(i)->_ringType&RingData::Alarm) == RingData::Alarm)
                        alarmring->Insert(plist->at(i));
                }
                break;
            }
        }
        _datas.push_back(callring);
        _datas.push_back(smsring);
        _datas.push_back(alarmring);
        // 获取当前铃声
        wstring curCallRingPath, curSmsRingPath, curAlarmRingPath;
        GetBaseSettingCommand basesettingCmd(_pDevice);
        if(_pDevice->GetDevInfo()->_platform != Platform_OMS)
        {
            curCallRingPath = basesettingCmd.GetSettingValueByName(ANDROID_RINGTONE);
            curSmsRingPath = basesettingCmd.GetSettingValueByName(ANDROID_NOTIFICATION_SOUND);
            curAlarmRingPath = basesettingCmd.GetSettingValueByName(ANDROID_ALARM_SOUND);
        }
        else
        {
            wstring curprofilename = basesettingCmd.GetSettingValueByName(ANDROID_CURRENT_PROFILE);
            //int  curprofileId  = _wtoi(curprofilename.c_str());
            int  curprofileId  = atoi(CCodeOperation::WstringToString(curprofilename).c_str());
            GetProfilesCommand profileCmd(_pDevice);
            profileCmd.Execute();
            deque<ProfileData*> profiles = profileCmd.GetProfiles();
            for(size_t i =0; i < profiles.size(); i++)
            {
                if(curprofileId == profiles[i]->_id)
                {
                    curCallRingPath = profiles[i]->_line1_ringtone;
                    curSmsRingPath  = profiles[i]->_message_alert;
                    curAlarmRingPath = profiles[i]->_alarm_alert;
                    break;
                }
            }
        }
        callring->SetCurrentRing(curCallRingPath);
        smsring->SetCurrentRing(curSmsRingPath);
        alarmring->SetCurrentRing(curAlarmRingPath);
    }
    else if (_pDevice->GetDevInfo()->_deviceType == Ios)
    {		
        wstring IosSyetemRingPath = _pDevice->GetIosRingPath()->GetIosUserRingPath();
        RingDirData*  systemRings = new RingDirData(_pDevice,CALL_RING, IosSyetemRingPath);

        for(size_t i=0; i< plist->size(); i++)
        {
            systemRings->Insert(plist->at(i));
        }

        RingOperationCommand opercmd(_pDevice);
        wstring ringid = opercmd.GetIosCurrentRing();
        systemRings->SetCurrentRing(ringid);

        _datas.push_back(systemRings);
    }
    return COMMAND_EXECUTE_SUCCESS;
}

void ND91Assistant::RingManager::Release()
{
    _datas.clear();
}

int ND91Assistant::RingManager::GetRingCount()
{
    int nCount = 0;
    if (_pDevice)
    {
        LoadRingsCommand cmd(_pDevice);	
        if (_pDevice->GetDevInfo()->_deviceType == Ios)
        {	
            nCount = cmd.GetRingCount();
        }
        else if (_pDevice->GetDevInfo()->_deviceType == Android)
        {
            cmd.SetAudioType(SYSTEM_RING);
            cmd.Execute();
            cmd.SetAudioType(CUSTOM_RING);
            cmd.Execute();
            nCount += cmd.GetDataList()->size();
        }
    }
    return nCount;
}

const vector<RingDirData*>* ND91Assistant::RingManager::GetDatas()
{
    return &_datas;
}

CommandExecuteResult ND91Assistant::RingManager::SetRing(int nID, RingData::MediaFileKind mediaType)
{
    RingData* ring = new AndroidRingData();
    ring->_Id = nID;
    ring->_IsSystemRing = false;
    ring->_ringType = mediaType;
    RingOperationCommand cmd(_pDevice);
    bool result = cmd.SettingRing(ring, mediaType);
    SAFE_DELETE(ring);
    return result ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}


#endif