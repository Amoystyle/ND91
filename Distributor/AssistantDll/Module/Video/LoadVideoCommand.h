#pragma once
#include "Module/TableQueryCommand.h"
#include <deque>

namespace ND91Assistant
{
class VideoData;
// ������Ƶ����
class LoadVideoCommand :public TableQueryCommand
{
public:
	LoadVideoCommand(const DeviceData* pDevice);
	virtual ~LoadVideoCommand(void);

	// ִ������
	virtual void Execute();

	const deque<VideoData*> GetVideoList(){return _videoList;}

protected:
	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

private:

	deque<VideoData*>   _videoList;
};

}
