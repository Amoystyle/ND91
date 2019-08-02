#pragma once

#include <string>
#include <fstream>
#include "MediaBase.h"
#include "Common/BinaryBufferReader.h"

using namespace std;

namespace ND91Assistant
{
	// avi文件头结构体
	struct AviMainHeader
	{
		int fcc;					// 必须为‘avih’
		int cb;						// 本数据结构的大小，不包括最初的8个字节（fcc和cb两个域）
		int dwMicroSecPerFrame;		// 视频帧间隔时间（以毫秒为单位）
		int dwMaxBytesPerSec;		// 这个AVI文件的最大数据率
		int dwPaddingGranularity;	// 数据填充的粒度
		int dwFlags;				// AVI文件的全局标记，比如是否含有索引块等
		int dwTotalFrames;			// 总帧数
		int dwInitialFrames;		// 为交互格式指定初始帧数（非交互格式应该指定为0）
		int dwStreams;				// 本文件包含的流的个数
		int dwSuggestedBufferSize;	// 建议读取本文件的缓存大小（应能容纳最大的块）
		int dwWidth;				// 视频图像的宽（以像素为单位）
		int dwHeight;				// 视频图像的高（以像素为单位）
		int dwReserved1;   			// 保留1
		int dwReserved2;   			// 保留2
		int dwReserved3;   			// 保留3
		int dwReserved4;   			// 保留4
	};

	// avi文件解析类
    class MediaData;
	class AviParser : public MediaBase
	{ 
	public:
		AviParser( const wstring path );
		~AviParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		// Summary  : 解析文件的数据头
		// Returns  : unsigned char*
		// Parameter: wstring path
		std::string ParserHeader(BinaryBufferReader* binarybuf, int headSize);

		AviMainHeader	_aviHeader;		// 文件头数据封装的结构体		

		int				_pos;			// 全局BinaryBufferReader指针读数据位置

	};
}
