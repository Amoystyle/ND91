#pragma once
#include "Module/BaseData.h"
#include <string>

using namespace std;

namespace ND91Assistant
{

	class CLASSINDLL_CLASS_DECL RecordingData : public BaseData
	{
	public:
		RecordingData(void);
		virtual ~RecordingData(void);
		
		int			_pk;					// DB里面的键值
		int			_labelPreset;           // 系统预置的标签值
		double		_duration;				// 录音时长	
		wstring		_path;					// 录音存放的路径
		wstring		_customLabel;			// 用户自定义标签(需要系统预置标签值为7)
		time_t		_createDate;            // 录音创建的时间

		wstring		_labelPresetText;		// 系统预置标签值对应的文本
		wstring		_name;
		int			_size;

	};

}
