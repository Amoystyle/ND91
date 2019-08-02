#pragma once

namespace ND91Assistant
{
//  加载图片信息
class PictureData;
class LoadPhotosCommand :public TableQueryCommand
{
public:
	LoadPhotosCommand(const DeviceData* pDevice);
	virtual ~LoadPhotosCommand(void);

	// 执行命令
	virtual void Execute();

	// Summary  : 获得全部图片信息
	// Returns	: const deque<PictureData*>
	const deque<PictureData*> GetPictures(){return _Pictures;}

	int GetPictureCounts();


protected:
	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

private:
	deque<PictureData*>  _Pictures;    // 图片信息列表

	bool  _getcount;

	int  _picCount;

};

}
