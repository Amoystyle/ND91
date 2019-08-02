#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/AlbumData.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/MusicSimpleCommand.h"
#include "Common/Path.h"

AlbumData::AlbumData()
{
	_albumName		= L"";       // 专辑名
	_albumImagePath = L"";		 // 专辑封面
	_hasLoad        = false;
}

AlbumData::~AlbumData(void)
{
}

wstring ND91Assistant::AlbumData::GetAblumImage(const DeviceData* pDevice)
{
	if(!pDevice)
		return _albumImagelocalPath;
	wstring localPath = pDevice->GetPath()->GetLocalAlbumPath()+CFileOperation::GetFileName(_albumImagePath);
    if (!_albumImagePath.empty() && _hasLoad && !localPath.empty() && CFileOperation::IsFileExist(localPath.c_str()) )
	{
		_albumImagelocalPath = localPath;
		return localPath;
	}

	MusicSimpleCommand cmd(pDevice);
	cmd.GetAlbumImage(this);
	if(!_albumImagePath.empty())
	{
		localPath = pDevice->GetPath()->GetLocalAlbumPath()+CFileOperation::GetFileName(_albumImagePath);

		// 专辑封面下载存储到本地
		if(!CFileOperation::IsFileExist(localPath.c_str()))
		{
			DownloadFileCommand(pDevice).Download(localPath, _albumImagePath);
		}
		_hasLoad = true;
		if(CFileOperation::IsFileExist(localPath.c_str()))
		{
			_albumImagelocalPath = localPath;
			return localPath;
		}
	}
	return L"";
}

bool ND91Assistant::AlbumData::SetAlbumImage( const DeviceData* pDevice, const wstring remotepath )
{
	MusicSimpleCommand cmd(pDevice);
	return cmd.SetAlbumImage(this, remotepath);
}

bool ND91Assistant::AlbumData::RemoveAlbumImage( const DeviceData* pDevice )
{
	MusicSimpleCommand cmd(pDevice);
	bool success = cmd.RemoveAlbumImage(this);
	if(success)
	{
        wstring localFile = GetAlbumImageLocalSavePath(pDevice->GetPath()->GetLocalAlbumPath());
        CFileOperation::DeleteFile(localFile);
		_albumImagePath = L"";
		_albumImagelocalPath = L"";
	}
	return success;
}

void ND91Assistant::AlbumData::RemoveTrack( int trackId )
{
	for (deque<AudioData*>::iterator it = _audios.begin(); it != _audios.end(); ++it)
	{
		if((*it)->Id() == trackId)
		{
			//SAFE_DELETE((*it));
			_audios.erase(it);
			break;
		}
	}
}

void ND91Assistant::AlbumData::InsertTrack( AudioData* track )
{
	_audios.push_back(track);
}

std::wstring AlbumData::GetAlbumImageLocalSavePath(wstring localFolder)
{
	RETURN_VALUE_IF (_albumImagePath.empty(), L"")

	return localFolder + CFileOperation::GetFileName(_albumImagePath);
}


#endif