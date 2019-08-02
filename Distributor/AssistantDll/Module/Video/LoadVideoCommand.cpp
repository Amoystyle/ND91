#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/LoadVideoCommand.h"
#include "Module/Video/VideoData.h"
#include "Module/AndroidDefine.h"

#define VIDEO_TABLE "_id,_data,title,duration,artist,album"

LoadVideoCommand::LoadVideoCommand(const DeviceData* pDevice)
:TableQueryCommand(pDevice)
{
	_nAndroidChannelID = MEDIA;
	SetParam("video",VIDEO_TABLE,"","");
}

LoadVideoCommand::~LoadVideoCommand(void)
{
	for (deque<VideoData*>::iterator it = _videoList.begin(); it!= _videoList.end(); it++)
	{
		SAFE_DELETE((*it));
	}
	_videoList.clear();
}

void ND91Assistant::LoadVideoCommand::Execute()
{
	DefaultExecute();
}

void ND91Assistant::LoadVideoCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;

	int count =0;
	reader.ReadInt32(count);
	for (int i =0; i < count; i++)
	{
		VideoData* video = new VideoData();
		video->ParseAndroidData(reader);
		_videoList.push_back(video);
	}

}


#endif