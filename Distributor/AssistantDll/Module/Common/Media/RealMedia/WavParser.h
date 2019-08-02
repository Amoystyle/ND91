#pragma once

#include <string>
#include <fstream>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "Common/BinaryBufferReader.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	// wav头数据结构体
	struct wavRiffHeader
	{
		int RiffID;		// 'R','I','F','F'
		int fileSize;
		int fmt;			// 'W','A','V','E'
	};
	
	// wav文件参数
	struct wavFormat
	{
		int		FmtID;
		int		BlockSize;
		short	FormatTag;
		short	Channels;
		int		SamplesPerSec;
		int		AvgBytesPerSec;
		short	lockAlign;
		short	BitsPerSample;
	};

	class WavParser : public MediaBase
	{ 
	public:
		WavParser(const wstring path);
		~WavParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		wavRiffHeader			_riffHeader;
		wavFormat				_format;

		int						_pos;			// 全局数据读取位置

	};
}
