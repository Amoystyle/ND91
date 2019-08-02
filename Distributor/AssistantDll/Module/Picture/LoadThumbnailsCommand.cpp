#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/LoadThumbnailsCommand.h"
#include "Module/Picture/ThumbnailData.h"
#include "Module/AndroidDefine.h"

#define ThumbnailColum "_id,_data,image_id,kind,width,height"

LoadThumbnailsCommand::LoadThumbnailsCommand(const DeviceData* pDevice)
:TableQueryCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	SetParam("thumbnails", ThumbnailColum, "","");
}

LoadThumbnailsCommand::~LoadThumbnailsCommand(void)
{
	for(size_t i=0; i< _thumbnails.size();i++)
	{
		SAFE_DELETE(_thumbnails[i]);
	}
	_thumbnails.clear();
}

void ND91Assistant::LoadThumbnailsCommand::Execute()
{

	DefaultExecute();

}

void ND91Assistant::LoadThumbnailsCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	int count = 0;
	reader.ReadInt32(count);
	for(int i=0; i< count; i++)
	{
		ThumbnailData* nail = new ThumbnailData();
		nail->ParseAndroidData(reader);
		_thumbnails.push_back(nail);
	}
}

#endif