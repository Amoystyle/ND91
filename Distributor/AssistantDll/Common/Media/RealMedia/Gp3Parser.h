#pragma once

#include <string>
#include <fstream>
#include "MediaBase.h"
using namespace std;

namespace ND91Assistant
{
	class Gp3Parser : public MediaBase
	{ 
	public:
		Gp3Parser(const wstring path);
		~Gp3Parser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		// 获取3gp文件的各种元素数据
		void ParserMoov(fstream *fs, int moovSize);
		
		//	读取标记间的数据转成int类型
		int GetAtomSize(fstream *fs);

		//	读取3gp文件特有标记
		std::string GetAtomType(fstream *fs);

	};
}
