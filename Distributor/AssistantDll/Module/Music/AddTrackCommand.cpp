#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/AddTrackCommand.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/AndroidMusicManager.h"

#include "Module/Common/Media/RealMedia/MP3Data.h"
#include "Module/AndroidDefine.h"

AddTrackCommand::AddTrackCommand(const DeviceData* pDevice, AndroidMusicManager* pMusic)
:BaseCommand(pDevice)
{
	_pMusic            = pMusic;
	_nAndroidChannelID = MEDIA;
	_nAndroidCommandID = ADD_RING;
	_pReturnAudio = NULL;
}

AddTrackCommand::~AddTrackCommand(void)
{
}

void ND91Assistant::AddTrackCommand::SetParam( wstring localpath, wstring remotepath )
{
	_LocalPath  = localpath;
	_RemotePath = remotepath;
}

void ND91Assistant::AddTrackCommand::Execute()
{
	_success = false;
	_pReturnAudio = NULL;
	DefaultExecute();
}

std::string ND91Assistant::AddTrackCommand::GetAndroidBodyData()
{
	//TODO:待获取本地歌曲属性
	//MP3Manager mp3(_LocalPath);
	const MP3Data* mp3Info = new MP3Data(_LocalPath);// mp3.GetMp3DataInfo();
	wstring tittle = mp3Info->PropertyGetMusicName();
	if(tittle.empty())
		tittle = CFileOperation::GetFileNameNoExt(_RemotePath);
	wstring artist = mp3Info->PropertyGetMusicPerformer();

	wstring album = mp3Info->PropertyGetAlbumName();

    // 如有封面需上传
	string imagefilename = CStrOperation::CreateHashCode(album.empty()?"<unknown>":CCodeOperation::UnicodeToUTF_8(album));
	wstring localalbumpath = mp3Info->PropertyGetFrontCover();//_pDevice->GetPath()->GetLocalAlbumPath()+CCodeOperation::UTF_8ToUnicode(imagefilename);
	wstring remotealbumpath = L"";
	if(CFileOperation::IsFileExist(localalbumpath.c_str()))
	{
		wstring albumPath = L"/sdcard/albumthumbs/";
		if (_pDevice->GetDevInfo()->_platform == Platform_OMS)
			albumPath = L"/sdcard/.albumthumbs/";
		remotealbumpath = albumPath+CCodeOperation::UTF_8ToUnicode(imagefilename);
		UploadFileCommand UpfileCmd(_pDevice);
		if(!UpfileCmd.Upload(localalbumpath, remotealbumpath))
			remotealbumpath = L"";
	}
	int duration = mp3Info->GetPlayTime();
	wstring stryear = mp3Info->PropertyGetYear();

	BinaryBufferWriter  writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_RemotePath));//手机端路径
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(tittle));		//tittle
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(artist));		//Artist
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(album));		//Album
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(remotealbumpath));//专辑封面
	writer.WriteInt64(CFileOperation::GetFileSizeW(_LocalPath));    // filesize
	writer.WriteString("audio/mp3");								// mime
	writer.WriteInt(duration);										//duration时长
	writer.WriteChar(0);											//TrackNumber
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(stryear));	//年份
	writer.WriteInt(128);											//类型

	SAFE_DELETE(mp3Info);
	return writer.GetBuffer();
}

void ND91Assistant::AddTrackCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());

	if(!parseAndroidPackageHead(&reader))
		return;

	if (reader.GetOffset() >= (int)data.length())
		return;

	AudioData* pAudio= new AudioData(_pDevice, _pMusic);
	pAudio->ParseAndroidData(reader);
	_pMusic->InsertTrack(pAudio);
	_pReturnAudio = pAudio;
	_success = true;
}

bool ND91Assistant::AddTrackCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != MEDIA )
		return false;
	return true;
}



#endif