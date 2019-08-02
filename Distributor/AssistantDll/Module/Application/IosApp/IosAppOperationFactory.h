#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//IOS的APP操作工厂，根据APP类型创建对应的操作对象
	class IosAppOperationFactory
	{
	public:
		IosAppOperationFactory(void);
		~IosAppOperationFactory(void);

		// Summary  : 根据APP类型创建对应的操作对象
		// Returns  : IIosAppOperation*
		// Parameter: const DeviceInfo * pDevInfo 
		// Parameter: APP_TYPE appType				: 程序类型
		// Parameter: ProgressSubject & subject		：进度反馈的subject
		static IIosAppOperation* GetAppOperation(DeviceData* pDevice,
            APP_TYPE appType, ProgressSubject& subject);
	};
}
