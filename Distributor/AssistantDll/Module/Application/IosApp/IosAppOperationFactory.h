#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//IOS��APP��������������APP���ʹ�����Ӧ�Ĳ�������
	class IosAppOperationFactory
	{
	public:
		IosAppOperationFactory(void);
		~IosAppOperationFactory(void);

		// Summary  : ����APP���ʹ�����Ӧ�Ĳ�������
		// Returns  : IIosAppOperation*
		// Parameter: const DeviceInfo * pDevInfo 
		// Parameter: APP_TYPE appType				: ��������
		// Parameter: ProgressSubject & subject		�����ȷ�����subject
		static IIosAppOperation* GetAppOperation(DeviceData* pDevice,
            APP_TYPE appType, ProgressSubject& subject);
	};
}
