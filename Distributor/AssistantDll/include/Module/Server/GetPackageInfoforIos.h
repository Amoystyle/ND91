#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// 获取IOS设备的手机版助手安装包
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforIos : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforIos(DeviceData* pDevice);
		~CGetPackageInfoforIos(void);

		bool GetPackageInfoforIos(wstring verType);

	};
}


