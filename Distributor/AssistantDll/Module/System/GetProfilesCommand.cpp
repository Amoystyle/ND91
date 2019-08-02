#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetProfilesCommand.h"
#include "ProfileData.h"
#include "Module/AndroidDefine.h"

GetProfilesCommand::GetProfilesCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = SETTING;
	_nAndroidCommandID = Query_Profiles;
}

GetProfilesCommand::~GetProfilesCommand(void)
{
	for(size_t i = 0; i< _profilelist.size(); i++)
	{
		SAFE_DELETE(_profilelist[i]);
	}
	_profilelist.clear();
}

void ND91Assistant::GetProfilesCommand::Execute()
{
	DefaultExecute();
}

std::string ND91Assistant::GetProfilesCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(_nAndroidCommandID);
	bufwriter.WriteInt(1);
	return bufwriter.GetBuffer();
}

void ND91Assistant::GetProfilesCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	int querycode;
	reader.ReadInt32(querycode);
	if(querycode != 1)
		return;
	int count = 0;
	reader.ReadInt32(count);
	for(int i = 0; i < count; i++)
	{
		ProfileData* profile = new ProfileData();
		profile->ParseAndroidData(reader);
		_profilelist.push_back(profile);
	}

}


#endif