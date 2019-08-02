#include "stdafx.h"
#include "DeviceLink.h"

#include "Module/Common/Synchronizer/ServiceClientBase.h"
#include "Device/CFConverter.h"

DeviceLink::DeviceLink(ServiceClientBase* client)
{
    _client = client;
}

DeviceLink::~DeviceLink()
{
}

bool DeviceLink::VersionExchange(int versionMajor, int versionMinor)
{
    vector<string> replyInfo;
    replyInfo.clear();
    _client->ReadMessageFromService(replyInfo);
    if (replyInfo.empty()) // Can't receive initial message from device
        return false;

    string message = GetMessageFromDLMSG(replyInfo);
    if (message != "DLMessageVersionExchange")// Did not receive DLMessageVersionExchange from device!
        return false;

    if (replyInfo.size() < 3)// DLMessageVersionExchange has unexpected format!
        return false;

    int vmajor = atoi(replyInfo.at(1).c_str());
    int vminor = atoi(replyInfo.at(2).c_str());
    bool useDLErrorAcceptingDeviceLinkGreater = false;

    if (vmajor > versionMajor)// Debug.WriteLine(string.Format("Version mismatch: device=({0},{1})>expected=({2},{3})", vmajor, vminor, versionMajor, versionMinor));
    {
        //return false;

        //ios6 beta3 vmajor值是300
        //ios 6 这里把版本号设置为最新的比如备份还原300,
        //则ios 设备会要求获取系统剩余空间大小.DLMessageGetFreeDiskSpace
        //versionMajor = vmajor;
    }
    else if ((vmajor == versionMajor) && (vminor > versionMinor))//Debug.WriteLine(string.Format("WARNING: Version mismatch: device=({0},{1})>expected=({2},{3})", vmajor, vminor, versionMajor, versionMinor));
    {    
        //return false;

        versionMinor = vminor;
        useDLErrorAcceptingDeviceLinkGreater = true;
    }

    //版本一致,发送版本信息给设备作为反馈

    //当版本不一致的时候,可以发送DLErrorAcceptingDeviceLinkGreater 这个值给设备.
    //以下是iTunes 跟踪得到的值
    //<array>
    //    <string>DLMessageVersionExchange</string>
    //    <string>DLErrorAcceptingDeviceLinkGreater</string>
    //    <integer>100</integer>
    //</array>

    vector<string> sendInfo;
    sendInfo.clear();
    sendInfo.push_back("DLMessageVersionExchange");
    useDLErrorAcceptingDeviceLinkGreater ? sendInfo.push_back("DLErrorAcceptingDeviceLinkGreater") : sendInfo.push_back("DLVersionsOk");
    sendInfo.push_back(CStrOperation::Int2string(versionMajor));
    
    if (!_client->SendMessageToService(sendInfo)) // Error when sending DLVersionOK
        return false;

    _client->ReadMessageFromService(replyInfo);
    if (replyInfo.empty())// Error when receiving DLMessageDeviceReady!
        return false;

    message = GetMessageFromDLMSG(replyInfo);
    if (message != "DLMessageDeviceReady")// Did not receive DLMessageDeviceReady from device!
        return false;

    return true;
}

bool DeviceLink::Send(vector<string> sendInfo)
{
    return _client->SendMessageToService(sendInfo);
}

bool DeviceLink::Send(vector<void*> sendInfo)
{
    return _client->SendMessageToService(sendInfo);
}

bool DeviceLink::Receive(vector<string>& replyInfo)
{
    return _client->ReadMessageFromService(replyInfo);
}

bool DeviceLink::Receive( map<string, string>& mapInfos )
{
	return _client->ReadMessageFromService(mapInfos);
}

bool DeviceLink::Receive(vector<string>& replyInfo,map< string, map<string,void*> >& mapEntities)
{
    return _client->ReadMessageFromService(replyInfo,mapEntities);
}

bool ND91Assistant::DeviceLink::Receive( vector<string>& replyInfo,map<string, string>& mapIds, map<string,string>& mapEntities )
{
	return _client->ReadMessageFromService(replyInfo, mapIds, mapEntities);
}

bool DeviceLink::Receive(vector<string>& replyInfo,map<string, string>& mapIds)
{
    return _client->ReadMessageFromService(replyInfo,mapIds);
}

bool DeviceLink::SendPing(string message)
{
    vector<string> sendInfo;
    sendInfo.clear();
    sendInfo.push_back("DLMessagePing");
    sendInfo.push_back(message);

    return _client->SendMessageToService(sendInfo);
}

bool DeviceLink::Disconnect(string message)
{
    vector<string> sendInfo;
    sendInfo.clear();
    sendInfo.push_back("DLMessageDisconnect");
    message.empty() == true ? sendInfo.push_back("___EmptyParameterString___") : sendInfo.push_back(message);

    return _client->SendMessageToService(sendInfo);
}

string DeviceLink::GetMessageFromDLMSG(vector<string> replyInfo)
{
    if (replyInfo.empty())
        return false;

    string cmd = replyInfo.at(0);
    if (cmd.length() < 9 || !CStrOperation::startsWith(cmd,"DL"))
        return "";
    else
        return cmd;
}

bool ND91Assistant::DeviceLink::Send( map<string, string> sendInfo )
{
	vector<void*> info;
	string msg= "DLMessageProcessMessage";
	info.push_back(CCFConverter::StringtoCFString(msg));
	info.push_back(CCFConverter::DictionarytoCFDictionary(sendInfo));
	return _client->SendMessageToService(info);
}

bool ND91Assistant::DeviceLink::Send( map<string, void*> sendInfo )
{
	return _client->SendMessageToService(sendInfo);
}

bool ND91Assistant::DeviceLink::Receive( wstring filepath)
{
	vector<string> reply;
	_client->ReadMessageFromService(reply, filepath);

	if(_client->ReadMessageFromService(reply, filepath)&&(int)reply.size()>0 && reply[0]=="DLMessageProcessMessage")
	{
		return true;
	}

	return false;
}

