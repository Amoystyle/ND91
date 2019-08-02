#pragma once

#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "Common/BinaryBufferReader.h"
#include "AssistantDll.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
	// ogg文件头类型
	enum OggHeaderType
	{
		FreshPaquetInContinuedLogicalBitstream = 0,
		ContinuedPaquetInContinuedLogicalBitstream = 1,
		FreshPaquetInNewLogicalBitstream = 2,
		FreshPaquetInLastLogicalBitstream = 4,
		ContinuedPaquetInLastLogicalBitstream = 5,
		StartAndEndLogicalBitstream = 6
	};

	class OggParser : public MediaBase
	{ 
	public:
		OggParser(const wstring path);
		~OggParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		// 遍历文件读取ogg文件各元素数据
		bool ReadData( bool onlyLast );

		bool IsLastInLogicalBitstream();


	private:

		OggHeaderType	_oggType;					 						

		/*__int64*/int64_t			_abGranulePostion;			// ogg特定标识长度
		bool			_multiplexedStreamDetected;	// 读取某标识是否成功

		int				_samples;					// 采样率
		int				_sampleRate;				// 输出采样率

		unsigned char*	_segMents;					// ogg文件头数据

		int				_pos;						// binarybuf读取全局位置
		int				_firstBufLen;				// 数据头长度(每次读取的不一样)


	};
}
