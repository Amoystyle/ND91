#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
    class DeviceData;

	// ��ȡAndroid�豸���ֻ������ְ�װ��
	class CLASSINDLL_CLASS_DECL CGetPackageInfoforAndroid : public CGetWebInfoBase
	{
	public:
		CGetPackageInfoforAndroid(DeviceData* pDevice);
		~CGetPackageInfoforAndroid(void);

		bool GetPackageInfoforAndroid();

	};
}


