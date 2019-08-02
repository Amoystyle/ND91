#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/MusicSimpleCommand.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/AlbumData.h"
#include "Module/AndroidDefine.h"

MusicSimpleCommand::MusicSimpleCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_success		= false;		// 成功标志
	_size			= 0;			// 空间大小
	_pTrack			= NULL;
	_pAlbum			= NULL;
	_nAndroidChannelID = MEDIA;
}

MusicSimpleCommand::~MusicSimpleCommand(void)
{
}

void ND91Assistant::MusicSimpleCommand::Execute()
{
	_success = false;

	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{

		IIosFileSystem* pIosFs = (_pDevice->GetDevInfo()->_connectType == WIFI) ?
			(IIosFileSystem*)new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle) :
		(IIosFileSystem*)new AFCFileSysemBase(_pDevice->GetDevInfo());

// 		if (!pIosFs->Ios_Exists(_strTempRingList))
// 		{
// 			pIosFs->Ios_DownFromPhone(_strIosRingList, _strTempRingList);
// 		}

		switch(_type)
		{
		case ERemoveTrack:
			_success = RemoveMusic();	   
			break;
		case EGetSpace:
			_size = GetMusicAllSpace();
			break;
		case EQueryAlbum:
			_success = ChangeAlbumCover();
			break;
		default:
			break;
		}

		delete pIosFs;
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		DefaultExecute();
	}
}

std::string ND91Assistant::MusicSimpleCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufferWriter;
	bufferWriter.WriteInt(_nAndroidChannelID);
	bufferWriter.WriteChar((char)_type);

	switch(_type)
	{
		case ERemoveTrack:
			bufferWriter.WriteInt(_pTrack->Id());
			break;
		case EQueryAlbum:
			{
				bufferWriter.WriteString("art");
				bufferWriter.WriteStringAsUTF8(CStrOperation::trimRightW(_pAlbum->_albumName,L"\0"));
			}
			break;
		case RENAME_TRACK:
			bufferWriter.WriteInt(_pTrack->Id());
			bufferWriter.WriteStringAsUTF8(_newName);
			break;
		case SET_ALBUM_IMAGE:
			bufferWriter.WriteStringAsUTF8(_pAlbum->_albumName);
			bufferWriter.WriteStringAsUTF8(_ImagePath);
			break;
		case REMOVE_ALBUM_IMAGE:
			bufferWriter.WriteStringAsUTF8(_pAlbum->_albumName);
			break;
		case CHANGE_ALBUM_ARTIST:
			{
				bufferWriter.WriteInt(_pTrack->Id());
				bufferWriter.WriteStringAsUTF8(_newName.empty()?L"<unknown>":_newName);
				bufferWriter.WriteStringAsUTF8(_newArtistname);
			}
			 break;
		default:
			break;
	}

	return bufferWriter.GetBuffer();
}

void ND91Assistant::MusicSimpleCommand::SetAndroidReturnData( std::string data )
{
     BinaryBufferReader  reader(data.c_str(), data.length());
	 if(!parseAndroidPackageHead(&reader))
		 return;
	 switch(_type)
	 {
		  case EQueryAlbum:
			 reader.ReadStringFromUTF8(_pAlbum->_albumImagePath);
			 _success = true;
			 break;
		  case ERemoveTrack:
		  case RENAME_TRACK:
		  case SET_ALBUM_IMAGE:
		  case REMOVE_ALBUM_IMAGE:
		  case CHANGE_ALBUM_ARTIST:
			 {
				 string okay;
				 reader.ReadString(okay, 4);
				 if (okay=="OKAY")
					 _success = true;
			 }
			 break;
		 case EGetSpace:
			 {
				 __int64 size;
				 reader.ReadInt64(size);
				 _size = (long)size;
			 }
			 break;
	 }

}

bool ND91Assistant::MusicSimpleCommand::RemoveTrack( AudioData* pTrack )
{
	_type = ERemoveTrack;
	_pTrack = pTrack;
	Execute();
	return _success;
}

bool ND91Assistant::MusicSimpleCommand::GetAlbumImage( AlbumData* pAlbum )
{
	_type = EQueryAlbum;
	_pAlbum = pAlbum;
	Execute();
	return _success;
}

long ND91Assistant::MusicSimpleCommand::GetTotalSpace()
{
	_type = EGetSpace;
	_size = 0;
	Execute();
	return _size;
}

bool ND91Assistant::MusicSimpleCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

bool ND91Assistant::MusicSimpleCommand::RemoveMusic()
{
	return true;
}

int ND91Assistant::MusicSimpleCommand::GetMusicAllSpace()
{
	return 0;
}

bool ND91Assistant::MusicSimpleCommand::ChangeAlbumCover()
{
	return true;
}


bool ND91Assistant::MusicSimpleCommand::RenameTrack( AudioData* pTrack, wstring newName )
{
	_type = RENAME_TRACK;
	_pTrack = pTrack;
	_newName = newName;
	Execute();
	return _success;
}

bool ND91Assistant::MusicSimpleCommand::SetAlbumImage( AlbumData* pAlbum, const wstring remotepath )
{
	_type = SET_ALBUM_IMAGE;
	_pAlbum = pAlbum;
	_ImagePath = remotepath;
	Execute();
	if(_success)
		_pAlbum->_albumImagePath = remotepath;
	return _success;
}

bool ND91Assistant::MusicSimpleCommand::RemoveAlbumImage( AlbumData* pAlbum )
{
	_type = REMOVE_ALBUM_IMAGE;
	_pAlbum =pAlbum;
	Execute();
	if(_success)
		_pAlbum->_albumImagePath =L"";
	return _success;
}

bool ND91Assistant::MusicSimpleCommand::ChangeAlbumArtist( AudioData* pTrack, wstring newAlbumName, wstring newArtist )
{
	_type = CHANGE_ALBUM_ARTIST;
	_pTrack = pTrack;
	_newName = newAlbumName;
	_newArtistname = newArtist;
	Execute();
    return _success;
}



#endif