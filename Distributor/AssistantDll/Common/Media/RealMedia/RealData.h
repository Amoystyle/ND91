#pragma once

#include <string>
#include <vector>

using namespace std;

namespace ND91Assistant
{
	// real文件特有标记间数据结构体
	struct FIELDDEF
	{
		std::string name;
		std::string type;
		int			size;
		std::string szieField;	
		std::string endian;		
	};

	// real文件标记类
	class RealData
	{ 
	public:
		RealData();
		~RealData(void);

	public:

		std::string			_name;			// 标记名
		vector<FIELDDEF*>	_fieldDef;		// 数据集
	};

}
