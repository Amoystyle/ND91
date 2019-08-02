#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetBaseSettingCommand.h"
#include "Module/AndroidDefine.h"

GetBaseSettingCommand::GetBaseSettingCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = SETTING;
	_nAndroidCommandID = Query_BaseSetting;
}

GetBaseSettingCommand::~GetBaseSettingCommand(void)
{
}

void ND91Assistant::GetBaseSettingCommand::Execute()
{
	_baseSettingInfos.clear();
    DefaultExecute();
}

std::string ND91Assistant::GetBaseSettingCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(_nAndroidCommandID);
	bufwriter.WriteInt(1);
	return bufwriter.GetBuffer();
}

void ND91Assistant::GetBaseSettingCommand::SetAndroidReturnData( std::string data )
{
   BinaryBufferReader reader(data.c_str(), data.length());
   if(!parseAndroidPackageHead(&reader))
	   return;
   int queryCode;
   reader.ReadInt32(queryCode);
   if(queryCode != 1)
	   return;
   int count =0;
   reader.ReadInt32(count);
   for(int i =0; i < count; i++)
   {
	   string name;
	   reader.ReadString(name);
	   wstring settingvalue;
	   reader.ReadStringFromUTF8(settingvalue);
	   if(!name.empty())
	      _baseSettingInfos.insert(pair<string, wstring>(name, settingvalue));
   }
}

std::wstring ND91Assistant::GetBaseSettingCommand::GetSettingValueByName( string name )
{
	wstring settingvalue = L"";
	if(_baseSettingInfos.empty())
		Execute();
	map<string, wstring>::iterator it = _baseSettingInfos.find(name);
	if(it != _baseSettingInfos.end())
	   settingvalue = it->second;
	return settingvalue;
}


#endif