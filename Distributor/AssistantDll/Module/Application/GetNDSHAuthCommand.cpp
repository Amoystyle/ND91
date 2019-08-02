#include "pch_module.h"

#ifdef MODULE_SYSTEM

#include "Module/Application/GetNdShAuthCommand.h"
#include "Module/Application/AppData.h"
#include "Module/AndroidDefine.h"

GetNdShAuthCommand::GetNdShAuthCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_nAndroidChannelID = SHELL;
	_nAndroidCommandID = GET_SU_AUTH;
	_bSucess = false;
}

GetNdShAuthCommand::~GetNdShAuthCommand()
{
}

std::string ND91Assistant::GetNdShAuthCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteInt(_nAndroidCommandID);
	return writer.GetBuffer(); 
}

void ND91Assistant::GetNdShAuthCommand::SetAndroidReturnData(std::string data)
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	reader.ReadBool(_bSucess);
}

void ND91Assistant::GetNdShAuthCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;

	DefaultExecute();	
}

#endif