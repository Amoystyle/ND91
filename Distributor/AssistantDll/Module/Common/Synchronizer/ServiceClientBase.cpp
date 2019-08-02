#include "stdafx.h"
#include "ServiceClientBase.h"

#include "Device/IosUsbConnection.h"

ServiceClientBase::ServiceClientBase(IosUsbConnection* device)
{
    _serviceName            = "";
    _device                 = device;
    _serviceSock            = NULL;
    _securitySocketCookie   = NULL;
}

ServiceClientBase::~ServiceClientBase()
{
}

bool ServiceClientBase::ReadMessageFromService(vector<string>& replyInfo)
{
//     if (_device->GetDeviceInfo()->_connectType == WIFI)
//         return _device->ReadSSLMessageFromService(_securitySocketCookie);
//     else
        return _device->ReadMssageFromService( _serviceSock, replyInfo );
}


bool ServiceClientBase::ReadMessageFromService(vector<string>& replyInfo,map< string, map<string,void*> >& mapEntities)
{
    //     if (_device->GetDeviceInfo()->_connectType == WIFI)
    //         return _device->ReadSSLMessageFromService(_securitySocketCookie);
    //     else
    return _device->ReadMssageFromService( _serviceSock, replyInfo, mapEntities );
}

bool ServiceClientBase::ReadMessageFromService( vector<string>& replyInfo,map<string, string>& mapIds, map<string,string>& mapEntities )
{
	return _device->ReadMssageFromService( _serviceSock, replyInfo, mapIds, mapEntities );
}

bool ServiceClientBase::ReadMessageFromService(vector<string>& replyInfo,map<string, string>& mapIds)
{
    //     if (_device->GetDeviceInfo()->_connectType == WIFI)
    //         return _device->ReadSSLMessageFromService(_securitySocketCookie);
    //     else
    return _device->ReadMssageFromService( _serviceSock, replyInfo, mapIds );
}

bool ServiceClientBase::SendMessageToService(vector<string> sendInfo)
{
    return _device->SendMessageToService( _serviceSock, sendInfo);
}

bool ServiceClientBase::SendMessageToService(vector<void*> sendInfo)
{
    return _device->SendMessageToService( _serviceSock, sendInfo);
}

bool ServiceClientBase::StartService()
{
    if (!_device->GetDeviceInfo()->_ios_bAFCConn)
	{
		#ifdef _DEBUG
        throw "iDevice Didn't connected with Computer";
		#endif
	}

    int ret = _device->StartIOSService((char*)_serviceName.c_str(), &_serviceSock);
    if (ret != 0)
    {
        _serviceSock = NULL;
		#ifdef _DEBUG
        throw "Can't start Service on device";
		#endif 
    }
    return true;
}

void ServiceClientBase::CloseService()
{
    _device->StopIOSService(&_serviceSock);
}

bool ServiceClientBase::SendMessageToService( map<string, string> sendInfo )
{
	return _device->SendMessageToService(_serviceSock,sendInfo);
}

bool ServiceClientBase::SendMessageToService( map<string, void*> sendInfo )
{
	return _device->SendMessageToService(_serviceSock, sendInfo);
}
bool ServiceClientBase::ReadMessageFromService( map<string, string> &mapMsg )
{
	return _device->ReadMssageFromService(_serviceSock, mapMsg);
}


bool ServiceClientBase::ReadMessageFromService( vector<string>& replyInfo, wstring filepath)
{
	return _device->ReadMssageFromService(_serviceSock,replyInfo, filepath);
}
