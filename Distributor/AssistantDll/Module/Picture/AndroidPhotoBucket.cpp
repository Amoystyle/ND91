#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "AndroidPhotoBucket.h"

const wstring CAMERA_GALLERY = L"sdcard/dcim/camera,sdcard/dcim/100media,sdcard/pictures/camera,sdcard/dcim/100andro";
const wstring WALLPAPER_GALLERY = L"sdcard/wallpaper";

AndroidPhotoBucket::AndroidPhotoBucket(void)
{
	_visible = true;
}

AndroidPhotoBucket::~AndroidPhotoBucket(void)
{
}

bool AndroidPhotoBucket::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadString(_id);
	reader.ReadStringFromUTF8(_name);
	reader.ReadStringFromUTF8(_path);
	_path = CStrOperation::trimRightW(_path, L"/");
	int visible;
	reader.ReadInt32(visible);
	_visible = visible==1?true:false;
	return true;
}


AndroidPhotoBucket::AndroidPhotoBucket( const AndroidPhotoBucket* pBucket )
{
	_id = pBucket->_id;
	_name = pBucket->_name;
	_visible = pBucket->_visible;
	_path = pBucket->_path;
}

PICTURE_TYPE AndroidPhotoBucket::GetBucketTypeByPath( wstring path )
{
	wstring tempPath = path;
    int pos = path.find(L"sdcard/");
	if(pos >= 0)
		tempPath = path.substr(pos);
	tempPath = CStrOperation::trimRightW(CStrOperation::toLowerW(tempPath),L"/");
	if ( tempPath == WALLPAPER_GALLERY)
		return PICTURE_TYPE_WALLPAPER;
	if ( CStrOperation::ContainsInW(CAMERA_GALLERY,tempPath,L","))
		return PICTURE_TYPE_PHOTO;
	return PICTURE_TYPE_NORMAL;
}

#endif