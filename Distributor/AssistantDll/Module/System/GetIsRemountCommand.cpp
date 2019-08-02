#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetIsRemountCommand.h"
#include "Module/AndroidDefine.h"

GetIsRemountCommand::GetIsRemountCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_ISREMOUNT;
	
	_bRemount = false;
}

GetIsRemountCommand::~GetIsRemountCommand()
{
}

std::string ND91Assistant::GetIsRemountCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetIsRemountCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead_Top16(&reader))
        return;
    reader.ReadBool(_bRemount);
}

void ND91Assistant::GetIsRemountCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;

	DefaultExecute();

}

#endif