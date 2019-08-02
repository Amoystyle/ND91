#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetIsWriteCommand.h"
#include "Module/AndroidDefine.h"

GetIsWriteCommand::GetIsWriteCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_ISWRITE;
	
	_bWrite = false;
}

GetIsWriteCommand::~GetIsWriteCommand()
{
}

std::string ND91Assistant::GetIsWriteCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetIsWriteCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead_Top16(&reader))
        return;
    reader.ReadBool(_bWrite);
}

void ND91Assistant::GetIsWriteCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;

	DefaultExecute();

}

#endif