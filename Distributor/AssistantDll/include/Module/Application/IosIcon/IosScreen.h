#pragma once

#pragma warning(disable : 4251)

#include "IosFolderIcon.h"

namespace ND91Assistant
{
	//屏幕信息存储类
	class CLASSINDLL_CLASS_DECL IosScreen
	{
	public:
		IosScreen(void);
		~IosScreen(void);

	public:
		IosIcon* _screenInfo[5][5]; // 每个屏幕中的图标信息，具体行列边界值根据IosIconSet类中的_nScreenLineCount及_nScreenRowCount决定
	};
}
