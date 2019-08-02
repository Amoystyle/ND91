#pragma once
#include "Module/Common/Synchronizer/ServiceClientBase.h"

namespace ND91Assistant
{

	class MobileImageMounterClient:public ServiceClientBase
	{
	public:
		MobileImageMounterClient(IosUsbConnection* device);
		virtual ~MobileImageMounterClient(void);

		bool StartMobileImageMouterService();

		bool MountImage(string imagePathOnDevice, string imageSignature);

	private:

		bool  _isServiceRunning;
	};

}