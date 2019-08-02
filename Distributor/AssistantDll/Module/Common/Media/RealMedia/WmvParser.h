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

	class WmvParser : public MediaBase
	{ 
	public:
		WmvParser( const wstring path );
		~WmvParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		// 读取媒体文件播放时长
		long Read_ASF_File_Properties_Object(BinaryBufferReader* binaryBuf);        

		// 获取文件格式		
		bool Read_ASF_Codec_List_Object(BinaryBufferReader* binaryBuf);									

		// 获取媒体文件的其他元素(暂时没做)
		void ReadASF_Extended_Content_Description_Object();							

		// 判断文件是否合格
		bool Verification();								   						

		int					_pos;			// BinaryBuffer操作流位置
		int32_t				_header_num;	// 文件指定头部字节数
		int64_t				_bufsize;		// 固定缓冲字节数
	};
}
