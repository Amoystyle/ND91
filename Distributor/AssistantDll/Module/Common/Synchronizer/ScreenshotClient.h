#pragma once
#include "Module/Common/Synchronizer/ServiceClientBase.h"

namespace ND91Assistant
{
    class DeviceData;
    class DeviceLink;
    class IosUsbConnection;

	class ScreenshotClient:public ServiceClientBase
	{
		public:
			ScreenshotClient(const DeviceData* pDeviceData,IosUsbConnection* device);
			virtual ~ScreenshotClient(void);

			bool CheckEnvironment();
				
			bool StartScreenshotService();

			wstring TakeScreenshotWithoutConvertTiff2PNG();

			bool InstallPrerequisite(wstring prerequisitePackagePath);


	private:

		DeviceLink*   _dlink;

		bool          _isServiceRunning;

		const DeviceData* _pDeviceData;
	};

}
