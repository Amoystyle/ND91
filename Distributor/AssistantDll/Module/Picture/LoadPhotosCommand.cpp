#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/LoadPhotosCommand.h"
#include "Module/Picture/PictureData.h"
#include "Module/AndroidDefine.h"

#define LOAD_PHOTO_COLOMN "_id,_data,bucket_id,bucket_display_name"

LoadPhotosCommand::LoadPhotosCommand(const DeviceData* pDevice)
:TableQueryCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	SetParam("images", LOAD_PHOTO_COLOMN, "", "_id desc");
	_getcount = false;
	_picCount = 0;
}

LoadPhotosCommand::~LoadPhotosCommand(void)
{
	for (size_t i=0; i < _Pictures.size(); i++)
	{
		SAFE_DELETE(_Pictures[i]);
	}
	_Pictures.clear();
}

void ND91Assistant::LoadPhotosCommand::Execute()
{
	_picCount = 0;
	DefaultExecute();

}

void ND91Assistant::LoadPhotosCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	_picCount = 0;
	reader.ReadInt32(_picCount);
	if(_getcount)
		return;
	for(int i=0; i< _picCount; i++)
	{
	     PictureData* pic = new PictureData();
		 pic->ParseAndroidData(reader,_pDevice);
		 _Pictures.push_back(pic);
	}
}

int ND91Assistant::LoadPhotosCommand::GetPictureCounts()
{
	_getcount = true;
	Execute();
	return _picCount;
}

#endif