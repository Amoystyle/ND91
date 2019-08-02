#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "LoadPhotoBucketsCommand.h"
#include "AndroidPhotoBucket.h"
#include "Module/AndroidDefine.h"

#define PhotoBucketColum "bucket_id,display_name,path,visible"

LoadPhotoBucketsCommand::LoadPhotoBucketsCommand(const DeviceData* pDevice)
:TableQueryCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	SetParam("bucket", PhotoBucketColum, "", "");
}

LoadPhotoBucketsCommand::~LoadPhotoBucketsCommand(void)
{
	for (size_t i=0; i < _PhotoBuckets.size(); i++)
	{
		SAFE_DELETE(_PhotoBuckets[i]);
	}
	_PhotoBuckets.clear();
}

void ND91Assistant::LoadPhotoBucketsCommand::Execute()
{
	DefaultExecute();
}

void ND91Assistant::LoadPhotoBucketsCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(),data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
    int count = 0;
	reader.ReadInt32(count);
	for (int i = 0; i < count; i++)
	{
        AndroidPhotoBucket* bucket = new AndroidPhotoBucket();
		bucket->ParseAndroidData(reader);
		_PhotoBuckets.push_back(bucket);
	}

}

#endif