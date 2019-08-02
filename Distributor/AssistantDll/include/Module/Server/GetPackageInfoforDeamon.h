#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// 获取不同设备的守护安装包
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforDeamon : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforDeamon(DeviceData* pDevice);
		~CGetPackageInfoforDeamon(void);

		bool GetPackageInfoFromWeb();

	};
}


