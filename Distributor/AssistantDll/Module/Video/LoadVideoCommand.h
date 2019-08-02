#pragma once
#include "Module/TableQueryCommand.h"
#include <deque>

namespace ND91Assistant
{
class VideoData;
// 加载视频命令
class LoadVideoCommand :public TableQueryCommand
{
public:
	LoadVideoCommand(const DeviceData* pDevice);
	virtual ~LoadVideoCommand(void);

	// 执行命令
	virtual void Execute();

	const deque<VideoData*> GetVideoList(){return _videoList;}

protected:
	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

private:

	deque<VideoData*>   _videoList;
};

}
