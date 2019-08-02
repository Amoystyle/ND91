#pragma once

#include <string>
#include <fstream>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	// aac文件解析类
	class AacParser : public MediaBase
	{ 
	public:
		AacParser( const wstring path );
		~AacParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:
		// Summary  : 解析文件的数据头
		// Returns  : unsigned char*
		// Parameter: wstring path
		unsigned char* ParserHeader();

		// aac的ADTS格式解析
		void ADTS_Parser(fstream* fs);

		// aac的ADIF格式解析
		void ADIF_Parser(unsigned char* buf);

		bool		_isADTS;	// 是否是ADTS格式
		bool		_isADIF;	// 是否是ADIF格式	

	};
}
