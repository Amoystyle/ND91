#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/LoadTracksCommand.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/AndroidMusicManager.h"
#include "Module/AndroidDefine.h"

#define LOAD_AUDIO_COLUMNS "_id,_data,title,duration,artist,album,is_ringtone,is_music,is_alarm,is_notification,_size"

LoadTracksCommand::LoadTracksCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic)
:TableQueryCommand(pDevice)
{
	_pMusic				= pmusic;
	_nAndroidChannelID	= MEDIA;
	SetParam("cusaudio", LOAD_AUDIO_COLUMNS, "", "title");
	_trackCount = 0;
	_getCount = false;
}

LoadTracksCommand::~LoadTracksCommand(void)
{
	for(size_t i=0; i< _audios.size();i++)
	{
		SAFE_DELETE(_audios[i]);
	}
	_audios.clear();
}

void ND91Assistant::LoadTracksCommand::Execute()
{
    DefaultExecute();
}

void ND91Assistant::LoadTracksCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	_trackCount = 0;
	reader.ReadInt32(_trackCount);
	if(_getCount)
		return;
	for(int i=0; i< _trackCount; i++)
	{
        AudioData* paudio = new AudioData(_pDevice, _pMusic);
		paudio->ParseAndroidData(reader);
		if(paudio->IsMusic())
			_pMusic->InsertTrack(paudio);
		else
		{
			SAFE_DELETE(paudio);
		}
	}

}

int ND91Assistant::LoadTracksCommand::GetTrackCounts()
{
	_trackCount = 0;
	_getCount = true;
	Execute();
	return _trackCount;
}



#endif