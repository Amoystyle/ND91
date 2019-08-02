#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// 获取Android设备的手机版助手安装包
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforAndroid : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforAndroid(DeviceData* pDevice);
		~CGetPackageInfoforAndroid(void);

		bool GetPackageInfoforAndroid();

	};
}


