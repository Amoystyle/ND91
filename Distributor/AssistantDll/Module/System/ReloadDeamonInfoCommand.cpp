#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "ReloadDeamonInfoCommand.h"
#include "Module/AndroidDefine.h"

ReloadDeamonInfoCommand::ReloadDeamonInfoCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_nAndroidChannelID = DEAMONMANAGER;
	_nAndroidCommandID = GET_DEMON_INFO;
    _isWifiOn          = false;
}


ReloadDeamonInfoCommand::~ReloadDeamonInfoCommand()
{
}

std::string ND91Assistant::ReloadDeamonInfoCommand::GetAndroidBodyData()
{
    BinaryBufferWriter write;
    write.Clear();
    write.WriteInt(_nAndroidChannelID);
    write.WriteInt(_nAndroidCommandID);
    return write.GetBuffer();
}

void ND91Assistant::ReloadDeamonInfoCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    //这里有点不符合包头规范，少了业务ID值,所以不要使用 parseAndroidPackageHead
    reader.ReadBuffer(_pReturnHead, 4);
    reader.ReadShort(_nReturnFlag);
    reader.ReadShort(_nReturnLongConnectSign);
    reader.ReadInt32(_nReturnBodySize);
    reader.ReadInt32(_nReturnChannelID);
    _nReturnCommonType = GET_DEMON_INFO; //就这个值没有返回

    _executesuccess = true;
    int count = 0;
    reader.ReadInt32(count);
    if (count == 0)
        _success = false;

    map<string, string> returnDataMap;
    map<string, string>::const_iterator iter;
    for (int i = 0; i < count; i++)
    {
        string key;
        string value;
        reader.ReadString(key);
        reader.ReadString(value);
        iter = returnDataMap.find(key);
        if (iter == returnDataMap.end())
            returnDataMap[key] = value;
    }

    for (iter = returnDataMap.begin(); iter != returnDataMap.end(); iter++)
    {
        if (iter->first == "version")
            _deamonVersion = iter->second;
        else if (iter->first == "password")
            _pwd = iter->second;
        else if (iter->first == "is_wifi_on")
            _isWifiOn = iter->second == "1" ? true : false;
        else if (iter->first == "imei")
            _imei = iter->second;
        else if (iter->first == "mac")
            _mac = iter->second;
        else if (iter->first == "model")
            _model = iter->second;
    }
}

void ND91Assistant::ReloadDeamonInfoCommand::Execute()
{
    RETURN_IF(_pDevice->GetDevInfo()->_deviceType == Ios);
    DefaultExecute();
}

#endif