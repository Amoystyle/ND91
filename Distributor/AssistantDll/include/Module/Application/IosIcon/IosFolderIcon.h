#pragma once

#pragma warning(disable : 4251)

#include "IosIcon.h"
#include <vector>

namespace ND91Assistant
{
	// 文件夹图标信息存储类
	class CLASSINDLL_CLASS_DECL IosFolderIcon :public IosIcon
	{
	public:
		IosFolderIcon(void);
		~IosFolderIcon(void);

		// 获取图标的类型
		virtual eIosIconType GetIconType(){return eIosIconType_Folder;}

	public:
		vector<IosIcon*> _vtIconList; //文件夹里的普通图标信息
		string _strListType; // 文件夹的类型
	};
}
