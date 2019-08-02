#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/PlaylistData.h"
#include "Module/Music/PlaylistOperationCommand.h"
#include "Module/Music/LoadPlayItemsCommand.h"

PlaylistData::PlaylistData(const DeviceData* pDevice)
{
	_pDevice= pDevice;
	_id		= -1;
	_name	= L"";
	_hasLoad = false;
}

PlaylistData::~PlaylistData(void)
{
}

bool ND91Assistant::PlaylistData::IsSystem()
{
    if(CStrOperation::ContainsInW(SYSTEM_PlAYLIST_NAMES,CStrOperation::toLowerW(_name),L","))
	{
		return true;
	}
	return false;
}

bool ND91Assistant::PlaylistData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadInt32(_id);
	reader.ReadStringFromUTF8(_name);
	return true;
}

bool ND91Assistant::PlaylistData::Rename( wstring name )
{
	RETURN_FALSE_IF(name.empty());
	if (CStrOperation::ContainsInW(SYSTEM_PlAYLIST_NAMES,CStrOperation::toLowerW(name),L","))
		return false;

	PlaylistOperationCommand cmd(_pDevice);
	bool success = cmd.RenamePlaylist(this, name);
	if (success)
		_name = name;
     return success;
}

bool ND91Assistant::PlaylistData::AddTrack( int trackId )
{
	PlaylistOperationCommand cmd(_pDevice);
	return cmd.AddTrackToPlaylist(this, trackId);
}

bool ND91Assistant::PlaylistData::RemoveItem( PlayItemData* item )
{
	PlaylistOperationCommand cmd(_pDevice);
	bool Success = cmd.RemoveFromPlaylist(item);
	if (Success)
	{
		for (deque<PlayItemData>::iterator it = _PlayItems.begin(); it!=_PlayItems.end();++it)
		{
			if(item->_id == (*it)._id)
			{
				_PlayItems.erase(it);
				break;
			}
		}
	}
	return Success;

}

bool ND91Assistant::PlaylistData::MoveItem( PlayItemData* item, int newIndex )
{
	PlaylistOperationCommand cmd(_pDevice);
	return cmd.MovePlaylistItem(item, newIndex);
}

bool ND91Assistant::PlaylistData::RemoveAllItems()
{
	for (size_t i = 0; i< _PlayItems.size(); i++)
	{
		if (RemoveItem(&(_PlayItems[i])))
		{
			i--;
		}
	}
	if(_PlayItems.empty())
		return true;
	return false;
}

bool ND91Assistant::PlaylistData::Load()
{
	_PlayItems.clear();
	LoadPlayItemsCommand cmd(_pDevice);
	cmd.SetPlaylist(this);
	cmd.Execute();
	_hasLoad = true;
	return true;
}

ND91Assistant::PlaylistData::PlaylistData( PlaylistData* playlist )
{
	_pDevice = playlist->_pDevice;
	_id = playlist->_id;
	_name =playlist->_name;
	for(size_t i=0;i<playlist->_PlayItems.size();i++)
	{
		_PlayItems.push_back((playlist->_PlayItems)[i]);
	}
}

void ND91Assistant::PlaylistData::RemoveItemByTrackId( int trackId )
{
	for (deque<PlayItemData>::iterator it = _PlayItems.begin(); it!=_PlayItems.end();++it)
	{
		if((*it)._trackId == trackId)
		{
			// 是否要调移除命令，还是直接删除即可？
			RemoveItem(&(*it));
		}
	}
}

void ND91Assistant::PlaylistData::AddPlayListItem( PlayItemData item )
{
	_PlayItems.push_back(item);
}

deque<PlayItemData>* ND91Assistant::PlaylistData::GetPlaylistItems()
{
	if (!_hasLoad) 
		this->Load();
	return &_PlayItems;
}


#endif