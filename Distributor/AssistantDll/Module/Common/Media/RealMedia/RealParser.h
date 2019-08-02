#pragma once

#include <string>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "RealData.h"
#include "Common/BinaryBufferReader.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	class RealParser : public MediaBase
	{ 
	public:
		RealParser(const wstring path);
		~RealParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

  private:
		
		// 封装与real文件头对比的数据集合
		void GetInitData();

		// 解析文件取得时长
		unsigned int Decode( FIELDDEF* field, BinaryBufferReader* binaryBuf );

		unsigned int ReadUInt32BE( BinaryBufferReader* binaryBuf );

		unsigned short ReadUInt16BE( BinaryBufferReader* binaryBuf );

		// 根据类型得到数据长度
		int GetSize( std::string type );

	private:

		vector<RealData*>		_recordef;		// real文件数据的存储

	};
}
