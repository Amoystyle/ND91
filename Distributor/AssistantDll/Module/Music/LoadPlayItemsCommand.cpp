#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/LoadPlayItemsCommand.h"
#include "Module/Music/PlaylistData.h"
#include "Module/AndroidDefine.h"

LoadPlayItemsCommand::LoadPlayItemsCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_pPlaylsit = NULL;
	_nAndroidChannelID = MEDIA;
	_nAndroidCommandID = TABLE_QUERY;
}

LoadPlayItemsCommand::~LoadPlayItemsCommand(void)
{
}

void ND91Assistant::LoadPlayItemsCommand::Execute()
{
	RETURN_IF(!_pPlaylsit);
	DefaultExecute();
}

std::string ND91Assistant::LoadPlayItemsCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  writer;
	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString("playlistitem");
	writer.WriteInt(_pPlaylsit->Id());
	return writer.GetBuffer();
}

void ND91Assistant::LoadPlayItemsCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	int count = 0;
	reader.ReadInt32(count);
	for(int i=0; i< count; i++)
	{
		PlayItemData item;
		item.ParseAndroidData(reader);
		item._owner = _pPlaylsit;
		_pPlaylsit->AddPlayListItem(item);
	}
}

bool ND91Assistant::LoadPlayItemsCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}

void ND91Assistant::LoadPlayItemsCommand::SetPlaylist( PlaylistData* pPlaylist )
{
	_pPlaylsit = pPlaylist;
}


#endif