#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
	class MediaData
	{ 
	public:
		MediaData();
		~MediaData(void);

	public:
		long			_fileSize;			// 文件大小
		long			_duration;			// 媒体的播放时长
		int				_bitrate;
		bool			_isVideo;			// 是否是视频文件

        wstring         _title;             // 标题
        wstring         _artist;            // 表演者
        wstring         _album;             // 专辑名
        wstring         _comment;           // 注释
        UINT            _nSampleRate;		//采样速率（单位Hz）

	};

}
