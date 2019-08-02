#pragma once

#include <iostream>
#include "AssistantDll.h"

using namespace std;

namespace ND91Assistant
{
	typedef enum eIosIconType
	{
		eIosIconType_Normal,
		eIosIconType_Folder
	};

	// 普通图标信息存储类
	class CLASSINDLL_CLASS_DECL IosIcon
	{
	public:
		IosIcon(void);
		~IosIcon(void);

		// 获取图标的类型
		virtual eIosIconType GetIconType(){return eIosIconType_Normal;}

	public:
		string _strDisPlayName; //显示名称
		time_t _iconModeDate; // 时间
		string _strBundleIdentifier; // 唯一标志值
		string _strDisplayIndentifier; // 显示的唯一标志值
		wstring _strIconImgFile; //图标的图片名，本地
	};
}
