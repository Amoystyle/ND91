#pragma once

#include "IIosIconDataOperation.h"
#include <map>

using namespace std;

namespace ND91Assistant
{
	//4.X固件的图标数据操作类
	class IosIconDataOperation4 : public IIosIconDataOperation
	{
	public:
		IosIconDataOperation4(IosIconSet* pIconSet);
		~IosIconDataOperation4(void);

		// 打包图标数据
		virtual void* PackIconCFData();
		// 解析图标数据
		virtual void PraseIconCFData(void* pCFData);

	private:
		void* PackSingleIconToCFData(IosIcon* pIcon);
		void* PackFolderIconToCFData(IosIcon* pIcon);
	};
}
