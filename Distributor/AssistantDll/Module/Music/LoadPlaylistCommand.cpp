#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/LoadPlaylistCommand.h"
#include "Module/Music/PlaylistData.h"
#include "Module/Music/AndroidMusicManager.h"
#include "Module/AndroidDefine.h"

#define PLAY_LIST_COLUMN "_id,name"

LoadPlaylistCommand::LoadPlaylistCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic)
:TableQueryCommand(pDevice)
{
	_pMusic            = pmusic;
	_nAndroidChannelID = MEDIA;
	SetParam("playlist",PLAY_LIST_COLUMN, "", "_id");
}

LoadPlaylistCommand::~LoadPlaylistCommand(void)
{
	for(size_t i=0; i< _playlists.size();i++)
	{
		SAFE_DELETE(_playlists[i]);
	}
	_playlists.clear();
}

void ND91Assistant::LoadPlaylistCommand::Execute()
{
	DefaultExecute();

}

void ND91Assistant::LoadPlaylistCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	int count = 0;
	reader.ReadInt32(count);
	for(int i=0; i< count; i++)
	{
		PlaylistData* playlist= new PlaylistData(_pDevice);
		playlist->ParseAndroidData(reader);
		_pMusic->InsertPlaylist(playlist);
	}
}


#endif