#include "stdafx.h"

#ifdef MODULE_SYSTEM
#include "SetDeviceNameCommand.h"
#include "Core/DeviceData.h"
#include "Device/IosUsbConnection.h"
#include "Module/AndroidDefine.h"
#include "Module/System/SystemManager.h"

SetDeviceNameCommand::SetDeviceNameCommand(const DeviceData* pDevice, std::wstring strNewName)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    
    wstring deviceName = _pDevice->GetSystemManager()->GetDeviceName();
    deviceName = CStrOperation::ReplaceW(deviceName, L" ", L"");
    if (deviceName.empty())
        _nAndroidCommandID = UPDATE_DEVICENAME;
    else
        _nAndroidCommandID = ADD_DEVICENAME;

	_strNewName = strNewName;
}

SetDeviceNameCommand::~SetDeviceNameCommand(void)
{
}

// Ö´ÐÐÃüÁî
void ND91Assistant::SetDeviceNameCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
        DefaultExecute();
		break;
	case Ios:
		{
			if (_pDevice->GetDevInfo()->_connectType == USB)
			{
				IosUsbConnection iosUsb((DeviceInfo*)_pDevice->GetDevInfo());
				_success = iosUsb.SetDeviceName(_strNewName);
			}			
		}
		break;

	default:
		break;
	}

    if (_success)
        _pDevice->GetMutableDevInfo()->_deviceName = _strNewName;
}

std::string ND91Assistant::SetDeviceNameCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteStringAsUTF8(_pDevice->GetDevInfo()->_strSerialNumber);
    writer.WriteStringAsUTF8(_strNewName);
    return writer.GetBuffer(); 
}

void ND91Assistant::SetDeviceNameCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    RETURN_IF(!parseAndroidPackageHead_Top16(&reader));

    int flag = 0;
    reader.ReadInt32(flag);
    if (flag != 1)
        _success = false;
}

#endif