#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/AudioData.h"
#include "Module/Music/AndroidMusicManager.h"
#include "Module/Music/MusicSimpleCommand.h"
#include "Module/Music/MusicPlayCommand.h"

#include "Module/File/CopyFileCommand.h"
#include "Common/Path.h"

#define SURPORT_IMAGE_TYPE  L"jpg,jpeg,png,gif,bmp,wbmp"
#define AUDIO_ALBUM_THUMB_FOLDER  L"/sdcard/albumthumbs/"

AudioData::AudioData(const DeviceData* pDevice, AndroidMusicManager* pMusic)
{
	_pDevice    = pDevice	;
	_pMusic     = pMusic	;
	_id			= -1		;
	_remote		= L""		;	// 路径
	_title		= L""		;	// 标题
	_duration	= 0			;	// 时长
	_artist		= L""		;	// 艺术家
	_isMusic	= false		;	// 是否音乐
	_pOwnAlbum  = NULL		;
}

AudioData::~AudioData()
{

}

bool ND91Assistant::AudioData::ParseAndroidData( BinaryBufferReader& reader)
{
	reader.ReadInt32(_id);
	reader.ReadStringFromUTF8(_remote);
	reader.ReadStringFromUTF8(_title);
	reader.ReadInt32(_duration);
	reader.ReadStringFromUTF8(_artist);
	wstring  ablumname;
	reader.ReadStringFromUTF8(ablumname);
	bool type;
	reader.ReadBool(type);
	reader.ReadBool(_isMusic);
	reader.ReadBool(type);
	reader.ReadBool(type);
	int64_t  size;
	reader.ReadInt64(size);
	_size = (long)size;

	if(_isMusic)
	{
		_pOwnAlbum = _pMusic->InsertAlbum(ablumname);
		_pOwnAlbum->InsertTrack(this);
	}
	return true;
}

void ND91Assistant::AudioData::ModifyAlbumArtistName( wstring albumName, wstring artist )
{
	MusicSimpleCommand cmd(_pDevice);
	if(cmd.ChangeAlbumArtist(this, albumName, artist))
	{
		this->_artist = artist;
		// 改了专辑名
		if(albumName != this->_pOwnAlbum->_albumName)
		{
			wstring  oldAlbumImg = this->_pOwnAlbum->_albumImagePath;
			AlbumData* pOldAlbum = this->_pOwnAlbum;
			// 从旧专辑移除歌曲
			this->_pOwnAlbum->RemoveTrack(this->_id);
			// 插入新专辑
			this->_pOwnAlbum = _pMusic->InsertAlbum(albumName.empty()?L"<unknown>":albumName);
			this->_pOwnAlbum->InsertTrack(this);

			// 新专辑封面不存在，旧封面存在，则使用旧封面
			if(!this->_pOwnAlbum->_albumName.empty() && this->_pOwnAlbum->_albumImagePath.empty() && !oldAlbumImg.empty())
			{
				CopyFileCommand cmd(_pDevice);
				string hashstr = CStrOperation::CreateHashCode(CCodeOperation::UnicodeToUTF_8(this->_pOwnAlbum->_albumName));
				cmd.setFileNames(oldAlbumImg, AUDIO_ALBUM_THUMB_FOLDER+CCodeOperation::UTF_8ToUnicode(hashstr));
				cmd.Execute();
				if(cmd.IsCopyDoneWell())
				{
					this->_pOwnAlbum->SetAlbumImage(_pDevice, AUDIO_ALBUM_THUMB_FOLDER+CCodeOperation::UTF_8ToUnicode(hashstr));
				}
			}
			if(pOldAlbum && pOldAlbum->GetTracks()->empty())
			{
				_pMusic->RemoveAlbumFromlist(pOldAlbum);
			}
		}
	}
}

void ND91Assistant::AudioData::ModifyName( wstring name )
{
	MusicSimpleCommand cmd(_pDevice);
	if (!name.empty() && name != _title)
	{
		if(cmd.RenameTrack(this, name))
		{
			_title = name;
		}
	}
}

bool ND91Assistant::AudioData::AddToPlaylist( PlaylistData* playlist )
{
	return playlist->AddTrack(_id);
}

bool ND91Assistant::AudioData::Play()
{
   MusicPlayCommand cmd(_pDevice);
   return cmd.Play(_remote);
}

void ND91Assistant::AudioData::ModifyAlbumImage( wstring localPath )
{
	if(_pOwnAlbum->_albumName.empty())
		return;
	if(localPath.empty())
	{
		_pOwnAlbum->RemoveAlbumImage(_pDevice);
		return;
	}
    int pos = localPath.find_last_of(L".");
	wstring ext = L"";
	if (pos >= 0)
		ext = localPath.substr(pos+1);
	if(!CStrOperation::ContainsInW(SURPORT_IMAGE_TYPE,CStrOperation::toLowerW(ext),L","))
		return;

	UploadFileCommand  cmd(_pDevice);
	string hashstr = CStrOperation::CreateHashCode(CCodeOperation::UnicodeToUTF_8(this->_pOwnAlbum->_albumName));
	wstring remotePath = AUDIO_ALBUM_THUMB_FOLDER + CCodeOperation::UTF_8ToUnicode(hashstr);
	// overwrite
	cmd.setfilename(localPath, remotePath);
	cmd.Execute();

	if(cmd.GetUploadResult()==Transfer_Success)
	{
		if(_pOwnAlbum->SetAlbumImage(_pDevice, remotePath))
		{
			wstring destlocalPath = _pDevice->GetPath()->GetLocalAlbumPath() + CFileOperation::GetFileName(_pOwnAlbum->_albumImagePath);
            CFileOperation::CopyFile( localPath, destlocalPath, false );			
		}
	}
}


#endif