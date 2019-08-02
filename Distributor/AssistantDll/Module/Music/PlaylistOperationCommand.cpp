#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/PlaylistOperationCommand.h"
#include "Module/Music/PlaylistData.h"
#include "Module/AndroidDefine.h"

PlaylistOperationCommand::PlaylistOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID  = MEDIA;
	_pReturnPlaylist	= NULL;
	_pSourcePlayItem    = NULL;
	_pSourcePlaylist    = NULL;
}

PlaylistOperationCommand::~PlaylistOperationCommand(void)
{
	SAFE_DELETE(_pReturnPlaylist);
}

void ND91Assistant::PlaylistOperationCommand::Execute()
{
	SAFE_DELETE(_pReturnPlaylist);
	_success = false;
	DefaultExecute();
}

std::string ND91Assistant::PlaylistOperationCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufferWriter;
	bufferWriter.WriteInt(_nAndroidChannelID);
	bufferWriter.WriteChar(_cmdType);
	switch(_cmdType)
	{
		case ADD_PLAYLSIT:
			bufferWriter.WriteStringAsUTF8(_Name);
			break;
		case RENAME_PLAYLIST:
			bufferWriter.WriteInt(_pSourcePlaylist->Id());
			bufferWriter.WriteStringAsUTF8(_Name);
			break;
		case REMOVE_PLAYLIST:
			bufferWriter.WriteInt(_pSourcePlaylist->Id());
			break;
		case ADD_TRACK_TO_PLAYLIST:
			bufferWriter.WriteInt(_pSourcePlaylist->Id());
			bufferWriter.WriteInt(_trackId);
			bufferWriter.WriteInt(_pSourcePlaylist->_PlayItems.size());
			break;
		case MOVE_PLAYLIST_ITEM:
			bufferWriter.WriteInt(_pSourcePlayItem->_owner->Id());
			bufferWriter.WriteInt(_pSourcePlayItem->_id);
			bufferWriter.WriteInt(_pSourcePlayItem->_trackId);
			bufferWriter.WriteInt(_newIndex);
			break;
		case REMOVE_FROM_PLAYLIST:
			bufferWriter.WriteInt(_pSourcePlayItem->_owner->Id());
			bufferWriter.WriteInt(_pSourcePlayItem->_id);
			break;
		default:
			break;
	}

	return bufferWriter.GetBuffer();
}

void ND91Assistant::PlaylistOperationCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	if(reader.GetOffset()>=(int)data.length())
		return;
	switch(_cmdType)
	{
	case ADD_PLAYLSIT:
		{
			_pReturnPlaylist = new PlaylistData(_pDevice);
			_pReturnPlaylist->ParseAndroidData(reader);
		}
		break;
	case RENAME_PLAYLIST:
	case REMOVE_PLAYLIST:
	case REMOVE_FROM_PLAYLIST:
		{
			char* pOk = new char[5];
			reader.ReadBuffer(pOk, 4);
			if (!strncmp(pOk,"OKAY",4))
				_success = true;
			SAFE_DELETE_ARRAY(pOk);
		}
		break;
	case ADD_TRACK_TO_PLAYLIST:
		{
			PlayItemData item;
			item.ParseAndroidData(reader);
			item._owner = _pSourcePlaylist;
			_pSourcePlaylist->AddPlayListItem(item);
		}
		break;
	case MOVE_PLAYLIST_ITEM:
		{
			reader.ReadInt32(_pSourcePlayItem->_id);
			_pSourcePlayItem->_index = _newIndex;
		}
		break;
	default:
		break;
	}

}

bool ND91Assistant::PlaylistOperationCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

PlaylistData* ND91Assistant::PlaylistOperationCommand::AddPlaylist( wstring name )
{
	_cmdType = ADD_PLAYLSIT;
	_Name	 = name;
	Execute();
	return _pReturnPlaylist;
}

bool ND91Assistant::PlaylistOperationCommand::RemovePlaylist( PlaylistData* playlist )
{
	_cmdType = REMOVE_PLAYLIST;
	_pSourcePlaylist = playlist;
	Execute();
	return _success;
}

bool ND91Assistant::PlaylistOperationCommand::RenamePlaylist( PlaylistData* playlist, wstring name )
{
	_cmdType		 = RENAME_PLAYLIST;
	_pSourcePlaylist = playlist;
	_Name            = name;
	Execute();
	return _success;
}

bool ND91Assistant::PlaylistOperationCommand::AddTrackToPlaylist( PlaylistData* playlist, int trackId)
{
	_cmdType		 = ADD_TRACK_TO_PLAYLIST;
	_pSourcePlaylist = playlist;
	_trackId		 = trackId;
	Execute();
	return _success;
}

bool ND91Assistant::PlaylistOperationCommand::RemoveFromPlaylist( PlayItemData* item )
{
	_cmdType         = REMOVE_FROM_PLAYLIST;
	_pSourcePlayItem = item;
	Execute();
	return _success;
}

bool ND91Assistant::PlaylistOperationCommand::MovePlaylistItem( PlayItemData* item, int newIndex )
{
	_cmdType		 = MOVE_PLAYLIST_ITEM;
	_pSourcePlayItem = item;
	_newIndex        = newIndex;
	Execute();
	return _success;
}



#endif