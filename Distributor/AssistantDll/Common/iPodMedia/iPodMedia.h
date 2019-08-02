#pragma once

#include <string>
#include <fstream>
#include "iPodMp3Paeser.h"

namespace ND91Assistant
{
	//ipod支持的媒体类型适配类
	//根据文件自动适配一种类型的媒体格式
	class ciPodMedia 
	{ 
	public:
		ciPodMedia(wstring path);
		~ciPodMedia();
		IiPodMediaParser* GetInstance(){return _MediaData;}
	private:
		//媒体数据类型指针
		IiPodMediaParser* _MediaData;
	};
}