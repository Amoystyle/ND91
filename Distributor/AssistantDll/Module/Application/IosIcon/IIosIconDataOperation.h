#pragma once

#include "Module/Application/IosIcon/IosIconSet.h"
#include <map>

namespace ND91Assistant
{
	// 图标操作基类
	class IIosIconDataOperation
	{
	public:
		IIosIconDataOperation(IosIconSet* pIconSet);
		~IIosIconDataOperation(void);

		// 打包图标数据
		virtual void* PackIconCFData() = 0;
		// 解析图标数据
		virtual void PraseIconCFData(void* pCFData) = 0;

	protected:
		void* PackNormalIconToCFData(IosIcon* pIcon);
		void AddIconInfo(void* pIconData, int nScreenIndex, int nLineIndex, int nRowIndex);

	protected:
		IosIconSet* _pIconSet;

	private:
		IosIcon* GetIcon(void* pIconData);
		IosIcon* GetNormalIcon(map<string, void*> mapInfo);
		IosFolderIcon* GetFolderIcon(map<string, void*> mapInfo);
	};
}
