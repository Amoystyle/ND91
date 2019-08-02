#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetResolutionCommand.h"
#include "Core/DeviceData.h"
#include "Module/AndroidDefine.h"

GetResolutionCommand::GetResolutionCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = SYSINFO;
    _nAndroidCommandID = GET_RESOLUTION;
	
	_strSize = L"";
}

GetResolutionCommand::~GetResolutionCommand()
{
}

std::string ND91Assistant::GetResolutionCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);
    return writer.GetBuffer(); 
}

void ND91Assistant::GetResolutionCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead_Top16(&reader))
        return;
	__int32 temp=0,width=0,height=0;
	reader.ReadInt32(temp);
	reader.ReadInt32(width);
	reader.ReadInt32(height);
	WCHAR szResolut[16] = {0};
	swprintf(szResolut, L"%d*%d", width, height);
	_strSize = szResolut;
}

void ND91Assistant::GetResolutionCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return;

	DefaultExecute();

}

#endif