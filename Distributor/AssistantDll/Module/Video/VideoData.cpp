#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/VideoData.h"
#include "Module/Video/VideoOperationCommand.h"

VideoData::VideoData()
{
	InitParams();
}

VideoData::~VideoData()
{
	_pOwnAlbum  = NULL;
}

void ND91Assistant::VideoData::InitParams()
{
	_duration   = 0;
	_pOwnAlbum  = NULL;

	_id		=  0;

	_size      = 0;

	_duration	=  0;

	_remotepath =  L"";

	_artist		=  L"";

	_album		=  L"";

	_title		=  L"";

	_localpath	=  L"";
}

bool ND91Assistant::VideoData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadInt32(_id);
	reader.ReadStringFromUTF8(_remotepath);
	reader.ReadStringFromUTF8(_title);
	reader.ReadInt32(_duration);
	reader.ReadStringFromUTF8(_artist);
	reader.ReadStringFromUTF8(_album);
	return true;
}

ND91Assistant::VideoData::VideoData(wstring strLocalFileName, wstring strPhoneFileName, PlayAlbumData* pAlbumData)
{
	InitParams();

	RETURN_IF(!CFileOperation::IsFileExist(strLocalFileName.c_str()));

	_name       = CFileOperation::GetFileName(strPhoneFileName);      
	_size       = CFileOperation::GetFileSizeW(strLocalFileName);		
	_Parent     = NULL;
	_remotepath = strPhoneFileName;
	_title = CFileOperation::GetFileNameNoExt(_name);
	_pOwnAlbum = pAlbumData;
}

ND91Assistant::VideoData::VideoData(FileData* pFileData, PlayAlbumData* pAlbumData)
{
	InitParams();

	RETURN_IF(!pFileData);

	_name       = pFileData->_name      ;      
	_mode       = pFileData->_mode      ;      
	_owner      = pFileData->_owner     ;     
	_group      = pFileData->_group     ;     
	_size       = pFileData->_size      ;		
	_modifyTime = pFileData->_modifyTime;
	_createTime = pFileData->_createTime;
	_Parent     = NULL;
	_remotepath = pFileData->GetFullPath();
	_title = CFileOperation::GetFileNameNoExt(_name);
	_pOwnAlbum = pAlbumData;
}

ND91Assistant::VideoData::VideoData( VideoData* pVideo )
{
	 InitParams();
	 RETURN_IF(!pVideo);

	 _id		=  pVideo->_id;

     _size      = pVideo->_size;

	_duration	=  pVideo->_duration;

	_remotepath =  pVideo->_remotepath;

	_artist		=  pVideo->_artist;

	_album		=  pVideo->_album;

	_title		=  pVideo->_title;

	_localpath	=  pVideo->_localpath;

	_pOwnAlbum  =  pVideo->_pOwnAlbum;
}

std::wstring ND91Assistant::VideoData::GetFullPath() const
{
	return _remotepath;
}

bool ND91Assistant::VideoData::LoadVideoFileInfo( const DeviceData* pDevice )
{
	ListDirectoryCommand cmd(pDevice, this);
	cmd.Execute();
	return cmd.DoneSuccess();
}

bool ND91Assistant::VideoData::Rename( const DeviceData* pDevice , wstring newName)
{
	VideoOperationCommand cmd(pDevice);
	if(cmd.RenameVideo(this, newName))
	{
		_title = newName;
		return true;
	}
	return false;
}

bool ND91Assistant::VideoData::DownLoadVideoFile( const DeviceData* pDevice, wstring localSavePath )
{
	if( DownloadFileCommand(pDevice).Download(localSavePath, _remotepath) )
	{
		_localpath = localSavePath;
	   return true;
	}
	return false;
}


#endif