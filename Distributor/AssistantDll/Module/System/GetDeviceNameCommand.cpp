#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetDeviceNameCommand.h"
#include "Module/AndroidDefine.h"

GetDeviceNameCommand::GetDeviceNameCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = QUERY_DEVICENAME;
}

GetDeviceNameCommand::~GetDeviceNameCommand()
{
}

std::string ND91Assistant::GetDeviceNameCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteStringAsUTF8(_pDevice->GetDevInfo()->_strSerialNumber);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetDeviceNameCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    RETURN_IF(!parseAndroidPackageHead_Top16(&reader));

    int flag = 0;
    reader.ReadInt32(flag);
    RETURN_IF(flag == 0);

    int nSize = 0;
    string strUTF8;
    reader.ReadInt32(nSize);
    reader.ReadString(strUTF8, nSize);
    _strDeviceName = CCodeOperation::UTF_8ToUnicode(strUTF8);
    _pDevice->GetMutableDevInfo()->_deviceName = _strDeviceName;
}

void ND91Assistant::GetDeviceNameCommand::Execute()
{
    RETURN_IF(_pDevice == NULL 
        || _pDevice->GetDevInfo() == NULL 
        || _pDevice->GetDevInfo()->_deviceType == Ios);
	DefaultExecute();
}

#endif