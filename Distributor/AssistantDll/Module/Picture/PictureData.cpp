#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PictureData.h"
#include "Module/Picture/ThumbnailData.h"

PictureData::PictureData()
{
    _id = 0;
    _type = PICTURE_TYPE_UNSET;

    _width          = 0;
    _height         = 0;
    _thumbWidth     = 0;		
    _thumbHeight    = 0;
    
    _createTime     = 0;
    _modifyTime     = 0;
    _size           = 0;
    _thumbnailIndex = -1;
}


PictureData::~PictureData()
{
}


bool ND91Assistant::PictureData::ParseAndroidData( BinaryBufferReader& reader, const DeviceData* pDevice)
{
	reader.ReadInt32(_id);
	reader.ReadStringFromUTF8(_remotePath);
	reader.ReadString(_bucketId);
	reader.ReadStringFromUTF8(_categoryName);
	
	wstring  sdcardpath = pDevice->GetDevInfo()->_sdcardpath;
	sdcardpath = CStrOperation::toLowerW(sdcardpath);
	sdcardpath = CStrOperation::trimLeftW(sdcardpath, L"/");
	wstring trmppath = CStrOperation::toLowerW(_remotePath);

	int pos = trmppath.find(sdcardpath);
	if(pos != 0 && pos != 1)
	{
		pos = trmppath.find(L"/sdcard/");
		if(pos >= 0)
		{
            // 转为实际的sd卡路径
			_remotePath = pDevice->GetDevInfo()->_sdcardpath + _remotePath.substr(pos+8);
		}
	}
	return true;


}

ND91Assistant::PictureData::PictureData(const PictureData* pic )
{
	_id				= pic->_id;
    _name           = pic->_name;
	_bucketId		= pic->_bucketId;
	_categoryName	= pic->_categoryName;
    _type           = pic->_type;

    _createTime     = pic->_createTime;
    _modifyTime     = pic->_modifyTime;
    _size           = pic->_size;
    _thumbnailIndex = pic->_thumbnailIndex;

    _width          = pic->_width;
    _height         = pic->_height;
    _thumbWidth     = pic->_thumbWidth;
    _thumbHeight    = pic->_thumbHeight;

    _localPath		= pic->_localPath;
    _remotePath		= pic->_remotePath;
    _thumbLocalPath = pic->_thumbLocalPath;
    _thumbRemotePath= pic->_thumbRemotePath;
}

ND91Assistant::PictureData::PictureData( wstring remoteFilePaths, PICTURE_TYPE type)
{
    _name = CFileOperation::GetFileName(remoteFilePaths);
    _modifyTime = _createTime = time(NULL);
    _type = type;
    _size = 0;
    _id = 0;

    _width  = IOS_WALLPAPER_WIDTH;
    _height = IOS_WALLPAPER_HEIGHT;
    _thumbWidth  = IOS_THUMB_WIDTH;
    _thumbHeight = IOS_THUMB_HEIGHT;
}

void ND91Assistant::PictureData::SetThumbnail( const ThumbnailData* pThumbnail )
{
    _thumbLocalPath     = pThumbnail->_localPath;
    _thumbRemotePath    = pThumbnail->_remotePath;
    _thumbWidth         = pThumbnail->_width;
    _thumbHeight        = pThumbnail->_height;
}

void ND91Assistant::PictureData::Create( wstring name, time_t createTime, time_t modifyTime,
										 int width, int height, int thumbnailIndex, int id, int size, 							 
										 wstring localPath, wstring remotePath, wstring thumbLocalPath, 
										 wstring thumbRemotePath, PICTURE_TYPE type)
{
	_name           = name;
	_createTime     = createTime;
	_modifyTime     = modifyTime;
	_width          = width;
	_height         = height;
	_thumbnailIndex = thumbnailIndex;

	_id				= id;
	_size           = size;
	_localPath		= localPath;
	_remotePath		= remotePath;
	_thumbLocalPath = thumbLocalPath;
	_thumbRemotePath= thumbRemotePath;
	_type			= type;	
}

#endif