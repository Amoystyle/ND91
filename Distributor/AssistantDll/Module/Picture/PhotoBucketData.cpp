#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PhotoBucketData.h"

const wstring CAMERA_GALLERY = L"sdcard/dcim/camera,sdcard/dcim/100media,sdcard/pictures/camera";
const wstring WALLPAPER_GALLERY = L"sdcard/wallpaper";

PhotoBucketData::PhotoBucketData(void)
{
	_Id ="";
	_DisplayName=L"";
	_path=L"";
	_visible = true;
}

PhotoBucketData::~PhotoBucketData(void)
{
}

bool ND91Assistant::PhotoBucketData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadString(_Id);
	reader.ReadStringFromUTF8(_DisplayName);
	reader.ReadStringFromUTF8(_path);
	int visible;
	reader.ReadInt32(visible);
	_visible = visible==1?true:false;
	return true;
}


ND91Assistant::PhotoBucketData::PhotoBucketData( const PhotoBucketData* pBucket )
{
	_Id = pBucket->_Id;
	_DisplayName = pBucket->_DisplayName;
	_visible = pBucket->_visible;
	_path = pBucket->_path;
}

ND91Assistant::BucketType ND91Assistant::PhotoBucketData::GetBucketTypeByPath( wstring path )
{
	wstring tempPath = path;
    int pos = path.find(L"sdcard/");
	if(pos >= 0)
		tempPath = path.substr(pos);
	tempPath = CStrOperation::trimRightW(CStrOperation::toLowerW(tempPath),L"/");
	if ( tempPath == WALLPAPER_GALLERY)
		return BucketType_Wallpaper;
	if ( CStrOperation::ContainsInW(CAMERA_GALLERY,tempPath,L","))
		return BucketType_Camera;
	return BucketType_Normal;
}

#endif
