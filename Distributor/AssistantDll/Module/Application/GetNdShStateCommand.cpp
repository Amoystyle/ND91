#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "Module/Application/GetNdShStateCommand.h"
#include "Module/Application/AppData.h"
#include "Module/AndroidDefine.h"

GetNdShStateCommand::GetNdShStateCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_nAndroidChannelID = SHELL;
	_nAndroidCommandID = GET_NDSH_STATE;

	_bHasNdsh = false;
	_bSuAuth=false;

}

GetNdShStateCommand::~GetNdShStateCommand()
{
}

std::string ND91Assistant::GetNdShStateCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteInt(_nAndroidCommandID);
	return writer.GetBuffer(); 
}

void ND91Assistant::GetNdShStateCommand::SetAndroidReturnData(std::string data)
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	reader.ReadBool(_bHasNdsh);
	reader.ReadBool(_bSuAuth);
}

void ND91Assistant::GetNdShStateCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;
	
		DefaultExecute();	
}

#endif