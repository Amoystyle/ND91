#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetMacAddressCommand.h"
#include "Module/AndroidDefine.h"

GetMacAddressCommand::GetMacAddressCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_MACADDRESS;
}

GetMacAddressCommand::~GetMacAddressCommand()
{
}

std::string ND91Assistant::GetMacAddressCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetMacAddressCommand::SetAndroidReturnData(std::string data)
{
    LOG->WriteDebug(L"GetMacAddressCommand::SendAndroidDatagram binary:");
    LOG_BINARY(Log::LOG_DEBUG, data);

    LOG_DEBUG(L"Parse MacAddress");
    BinaryBufferReader reader(data.c_str(), data.length());
    RETURN_IF(!parseAndroidPackageHead_Top16(&reader));
    reader.ReadStringFromUTF8(_macAddress);
    LOG_DEBUG(L"MacAddress is:"+_macAddress);
}

void ND91Assistant::GetMacAddressCommand::Execute()
{
    RETURN_IF(_pDevice == NULL 
        || _pDevice->GetDevInfo() == NULL 
        || _pDevice->GetDevInfo()->_deviceType == Ios);
	DefaultExecute();
}

#endif