#pragma once

#include <string>

using namespace std;
	

namespace ND91Assistant
{
	class IMediaParser
	{ 
	public:

		// 获取媒体文件播放时长
		virtual long GetMediaDuration() = 0;


	};
}
