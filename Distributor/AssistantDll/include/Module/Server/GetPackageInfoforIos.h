#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// ��ȡIOS�豸���ֻ������ְ�װ��
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforIos : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforIos(DeviceData* pDevice);
		~CGetPackageInfoforIos(void);

		bool GetPackageInfoforIos(wstring verType);

	};
}


