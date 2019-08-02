#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetPropertyCommand.h"
#include "Device/AndroidPropertyData.h"
#include "Module/AndroidDefine.h"

GetPropertyCommand::GetPropertyCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SHELL;
    _nAndroidCommandID = ExecShell;
    _success = false;
}

GetPropertyCommand::~GetPropertyCommand()
{
}

std::string ND91Assistant::GetPropertyCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    writer.WriteStringAsUTF8(ANDROID_SHELL_GETPROPERTY);
    return writer.GetBuffer();
}

void ND91Assistant::GetPropertyCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
        return;

    reader.ReadBool(_success);
    string strBody;
    reader.ReadStringFromUTF8(strBody);
    AndroidPropertyData parsePropertyData(strBody.c_str(), strBody.length());

    _DeviceInfo._strSerialNumberForShow   = parsePropertyData.GetPropertyValue("ro.serialno");
	if(_DeviceInfo._strSerialNumber.empty())
		_DeviceInfo._strSerialNumber = _DeviceInfo._strSerialNumberForShow;

    _DeviceInfo._deviceIMEI             = parsePropertyData.GetPropertyValue("ro.gsm.imei");
    _DeviceInfo._manufacturer           = parsePropertyData.GetPropertyValue("ro.product.manufacturer");
    _DeviceInfo._deviceHWModel          = parsePropertyData.GetPropertyValue("ro.product.model");
    _DeviceInfo._deviceProductVersion   = parsePropertyData.GetPropertyValue("ro.build.version.release");
	_DeviceInfo._deviceCoreVersion		= parsePropertyData.GetPropertyValue("ro.build.display.id");
	_DeviceInfo._deviceBaseVersion		= parsePropertyData.GetPropertyValue("gsm.version.baseband");
	_DeviceInfo._deviceInsdeVersion		= parsePropertyData.GetPropertyValue("ro.build.description");
	if(_DeviceInfo._deviceProductVersion.empty())
		_DeviceInfo._deviceProductVersion = L"2.0";
    _DeviceInfo._language               = parsePropertyData.GetPropertyValue("ro.product.locale.region");
}

void ND91Assistant::GetPropertyCommand::Execute()
{
    DefaultExecute();
}


#endif