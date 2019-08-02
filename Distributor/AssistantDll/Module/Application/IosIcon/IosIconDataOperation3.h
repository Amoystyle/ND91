#pragma once

#include "IIosIconDataOperation.h"

namespace ND91Assistant
{
	//3.X固件的图标数据操作类
	class IosIconDataOperation3 : public IIosIconDataOperation
	{
	public:
		// 构造函数
		// IosIconSet* pIconSet ：IosIconSet类指针
		IosIconDataOperation3(IosIconSet* pIconSet);
		~IosIconDataOperation3(void);

		// 打包图标数据
		virtual void* PackIconCFData();
		// 解析图标数据
		virtual void PraseIconCFData(void* pCFData);

	private:
		void* PackSingleIconToCFData(IosIcon* pIcon);

	};
}
