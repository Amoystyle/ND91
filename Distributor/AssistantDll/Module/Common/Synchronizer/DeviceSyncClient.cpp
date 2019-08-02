#include "stdafx.h"
#include "DeviceSyncClient.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceLink.h"
#include "Device/IosUsbConnection.h"

string DeviceSyncClient::EMPTY_PARAMETER_STRING  = "___EmptyParameterString___";
string DeviceSyncClient::RECORD_ENTITY_NAME_KEY  = "com.apple.syncservices.RecordEntityName";

DeviceSyncClient::DeviceSyncClient(IosUsbConnection* device) : ServiceClientBase(device)
{
    MSYNC_VERSION_INT1      = 100;
    MSYNC_VERSION_INT2      = 100;
    _isServiceRunning       = false;
    _dlink                  = new DeviceLink((ServiceClientBase*)this);
    _dataClass              = "";
    _syncDirection          = Unknown;
    _deviceAnchor           = "";
    _syncType               = Unknow;
    _serviceName            = "com.apple.mobilesync";
}

DeviceSyncClient::~DeviceSyncClient()
{
    SAFE_DELETE(_dlink);
}

bool DeviceSyncClient::IsReadyToReceiveChangesFromComputer()
{
    if (_syncDirection != DeviceToComputer)// Wrong sync direction,maybe something wrong
        return false;

    vector<string> msg;
    if (!_dlink->Receive(msg))
        return false;

    string responseType = msg.at(0);
    if (responseType.empty())
        return false;
    else if (responseType == "SDMessageCancelSession")
        return false;
    else if (responseType != "SDMessageDeviceReadyToReceiveChanges")
        return false;
    else
        if (!_dlink->SendPing("Preparing to get changes for device"))
            return false;

    _syncDirection = ComputerToDevice;

    return true;
}

bool DeviceSyncClient::StartSync(string dataClass, DeviceSyncAnchors anchors, int computerDataClassVersion,int* deviceDataClassVersion)
{
    _syncType = Unknow;
    *deviceDataClassVersion = 0;
    _dataClass = dataClass;

    bool result = false;
    if (!StartMobileSyncService())
        return false;

    vector<string> msgs;
    msgs.clear();
    msgs.push_back("SDMessageSyncDataClassWithDevice");
    msgs.push_back(_dataClass);
    //anchors._deviceAnchor.empty() == true ?  msgs.push_back("---") : msgs.push_back(anchors._deviceAnchor);
	msgs.push_back("---");
    msgs.push_back(anchors._computerAnchor);
    msgs.push_back(CStrOperation::Int2string(computerDataClassVersion));
    msgs.push_back(EMPTY_PARAMETER_STRING);

    if (!_dlink->Send(msgs))
        return false;

    vector<string> recvMsg;
    recvMsg.clear();
    if (!_dlink->Receive(recvMsg))
        return false;
    result = true;

    if (recvMsg.empty())// Can't Receive SDMessageSyncDataClassWithComputer Msg from device
        return false;
    else
    {
        string responseType = recvMsg.at(0);
        if (responseType == "SDMessageRefuseToSyncDataClassWithComputer") // string reason = recvMsg.at(2)
            return false;
        else if (responseType == "SDMessageCancelSession") // string reason = recvMsg.at(2)
            return false;

        string syncTypeNode = recvMsg.at(4);
        if (!syncTypeNode.empty())
        {
            if (syncTypeNode == "SDSyncTypeFast")
                _syncType = Fast;
            else if (syncTypeNode == "SDSyncTypeSlow")
                _syncType = Slow;
            else if (syncTypeNode == "SDSyncTypeReset")
                _syncType = Reset;
            else
            {
                _syncType = Unknow;
                result = false;
            }
        }

        if (*deviceDataClassVersion == 0)
        {
            string deviceDataClassVersionNode = recvMsg.at(5);
            if (!deviceDataClassVersionNode.empty())
                *deviceDataClassVersion = atoi(deviceDataClassVersionNode.c_str());
        }
        //2是deviceAnchor,3 is hostAnchor
        _deviceAnchor = recvMsg.at(3);
    }

    _syncDirection = DeviceToComputer;
    return result;
}

bool DeviceSyncClient::StartMobileSyncService()
{         
    if (_isServiceRunning)
        return false;

    _isServiceRunning = StartService();
    if (_isServiceRunning)
        return _dlink->VersionExchange(MSYNC_VERSION_INT1, MSYNC_VERSION_INT2);

    return false;
}

void DeviceSyncClient::StopSync()
{
    if (_dlink != NULL)
        _dlink->Disconnect("All done, thanks for the memories");

    CloseService();
    _isServiceRunning = false;

    _syncDirection = Unknown;
}

bool DeviceSyncClient::IsDataClassDisableSync(string dataClass, bool tryEnableIfDisabled)
{
    return _device->IsDataClassDisableTetheredSync(dataClass, tryEnableIfDisabled);
}

bool DeviceSyncClient::IsDataClassEnableiCloud(string dataClass)
{
    return _device->IsDataClassEnableiCloud(dataClass);
}

bool DeviceSyncClient::FinishSync()
{
    vector<string> msgs;
    msgs.clear();
    msgs.push_back("SDMessageFinishSessionOnDevice");
    msgs.push_back(_dataClass);

    if (!_dlink->Send(msgs))
        return false;

    vector<string> recvMsg;
    if (!_dlink->Receive(recvMsg))
        return false;

    bool result = false;
    string responseType = recvMsg.at(0);
    if (responseType.empty())
        result = false;
    else if (responseType == "SDMessageCancelSession") // string reason = recvMsg.at(2)
        result = false;
    else if (responseType == "SDMessageDeviceFinishedSession")
        result = true;
    else // "Receive Unknow Protocol Msg,Maybe the process step is wrong"
        result = false;

    if(result)
        _syncDirection = Unknown;

    return result;
}

bool DeviceSyncClient::GetChangesFromDevice()
{
    return GetRecordsFromDevice("SDMessageGetChangesFromDevice");
}

bool DeviceSyncClient::GetAllRecordsFromDevice()
{
    return GetRecordsFromDevice("SDMessageGetAllRecordsFromDevice");
}

bool DeviceSyncClient::GetRecordsFromDevice(string operation)
{
    if (operation.empty())
        return false;

    vector<string> msgs;
    msgs.clear();
    msgs.push_back(operation);
    msgs.push_back(_dataClass);

    return _dlink->Send(msgs);
}

bool DeviceSyncClient::ReceiveChangesEntitiesFromDevice(map< string, map<string,void*> >& mapEntities,bool& isHaveRecord)
{
    vector<string> msg;
    msg.clear();
    if (!_dlink->Receive(msg,mapEntities))
        return false;

    string responseType = msg.at(0);
    if (responseType.empty())
        return false;
    else if (responseType == "SDMessageCancelSession") // string reason = recvMsg.at(2)
        return false;
    else
    {
        if (msg.size() > 3 && msg.at(3) == "true")// is last record
            isHaveRecord = true;
        else
            isHaveRecord = false;
    }

    return true;
}

bool DeviceSyncClient::AcknowledgeChangesFromDevice()
{
    vector<string> msgs;
    msgs.clear();

    msgs.push_back("SDMessageAcknowledgeChangesFromDevice");
    msgs.push_back(_dataClass);

    return _dlink->Send(msgs);
}

bool DeviceSyncClient::SendProcessChangesToDevice(vector<void*> sendInfo)
{
    return _dlink->Send(sendInfo);
}

bool DeviceSyncClient::SendProcessChangesToDevice(map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo)
{
    CreateProcessChangesMessage(_dataClass, entities, isHaveRecord, actions,sendInfo);
    return _dlink->Send(sendInfo);
}

void DeviceSyncClient::CreateProcessChangesMessage(string dataClass,map<string, void*> entities,bool isHaveRecord,map< string, vector<void*> > actions,vector<void*>& sendInfo)
{
    // 开始写数组内容
    sendInfo.push_back(CCFConverter::StringtoCFString("SDMessageProcessChanges"));
    sendInfo.push_back(CCFConverter::StringtoCFString(dataClass));

    // 实体内容
    sendInfo.push_back(CCFConverter::DictionarytoCFDictionary(entities));

    // 是否还有数据
    sendInfo.push_back(CCFConverter::BoolToCFBool(!isHaveRecord));

    // sync actions
    sendInfo.push_back(CCFConverter::ConvertStringVectorMap(actions));
}

bool DeviceSyncClient::GetRemapIdentifiersFromDevice(map<string, string>& mapIds)
{
    vector<string> msg;
    if (!_dlink->Receive(msg,mapIds))
        return false;

    string responseType = msg.at(0);
    if (responseType.empty())
        return false;
    else if (responseType == "SDMessageCancelSession")// string reason = recvMsg.at(2)
        return false;
    else if (responseType != "SDMessageRemapRecordIdentifiers")
        return false;
    else
    {
        if (msg.at(2) != "Entities")
            return false;
    }

    return true;
}

bool DeviceSyncClient::ClearAllRecordsOnDevice()
{
    vector<string> msgs;
    msgs.clear();
    msgs.push_back("SDMessageClearAllRecordsOnDevice");
    msgs.push_back(_dataClass);
    msgs.push_back(EMPTY_PARAMETER_STRING);

    if (!_dlink->Send(msgs))
        return false;

    vector<string> recvMsg;
    recvMsg.clear();
    if (!_dlink->Receive(recvMsg))
        return false;

    string responseType = recvMsg.at(0);
    if (responseType.empty())
        return false;
    else if (responseType == "SDMessageCancelSession")// string reason = recvMsg.at(2)
        return false;
    else if (responseType != "SDMessageDeviceWillClearAllRecords")// string reason = recvMsg.at(2)
        return false;

    return true;
}