#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "GetIsRootCommand.h"
#include "Module/AndroidDefine.h"

GetIsRootCommand::GetIsRootCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_ISROOT;
	
	_isRoot = false;
}

GetIsRootCommand::~GetIsRootCommand()
{
}

std::string ND91Assistant::GetIsRootCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetIsRootCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead_Top16(&reader))
        return;
    reader.ReadBool(_isRoot);
}

void ND91Assistant::GetIsRootCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;

	DefaultExecute();

}

#endif