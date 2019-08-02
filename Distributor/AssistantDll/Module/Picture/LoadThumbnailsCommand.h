#pragma once

namespace ND91Assistant
{
	// 加载缩略图信息
	class ThumbnailData;
	class LoadThumbnailsCommand :public TableQueryCommand
	{
	public:
		LoadThumbnailsCommand(const DeviceData* pDevice);
		virtual ~LoadThumbnailsCommand(void);

		// 执行命令
		virtual void Execute();

		// Summary  : 获的缩略图信息类表
		// Returns	: deque<ThumbnailData*>
		deque<ThumbnailData*> GetThumbNails(){return _thumbnails;}

        const deque<ThumbnailData*>* GetData()
        {
            return &_thumbnails;
        }

	protected:
		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);
	private:

		deque<ThumbnailData*>   _thumbnails;  // 缩略图信息列表

	};

}
