#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/ThumbnailData.h"

ThumbnailData::ThumbnailData(void)
{
	_remotePath = L"";
	_width		= 0  ;
	_height		= 0  ;
	_localPath	= L"";
	
}

ThumbnailData::~ThumbnailData(void)
{
}

bool ND91Assistant::ThumbnailData::ParseAndroidData( BinaryBufferReader& reader )
{
	int id;
	reader.ReadInt32(id);
	reader.ReadStringFromUTF8(_remotePath);
	reader.ReadInt32(_imageId);
	int kind;
	reader.ReadInt32(kind);
	reader.ReadInt32(_width);
	reader.ReadInt32(_height);
	return true;
}

ND91Assistant::ThumbnailData::ThumbnailData( const ThumbnailData* pnail )
{
	_remotePath = pnail->_remotePath;
	_width		= pnail->_width  ;
	_height		= pnail->_height  ;
	_localPath	= pnail->_localPath;
	_imageId    = pnail->_imageId;
}

#endif