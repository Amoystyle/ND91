#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// ��ȡ��ͬ�豸���ػ���װ��
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforDeamon : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforDeamon(DeviceData* pDevice);
		~CGetPackageInfoforDeamon(void);

		bool GetPackageInfoFromWeb();

	};
}


