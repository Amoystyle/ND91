#pragma once

// #ifdef WIN32
// 	#include "Device/ITunesMobileDevice.h"
// #else
// 	#include "Device/MobileDevice.h"
// #endif
#include "Device/IosUsbConnection.h"
#include "Core/Deviceinfo.h"
#include <string>
using namespace std;

typedef ULONGLONG afc_file_ref;
namespace ND91Assistant
{
	class IosUsbSync
	{ 
	public:
		IosUsbSync( const DeviceInfo* pDevInfo );
		~IosUsbSync(void);

		// Summary  : 向设备发送开始同步命令
		// Returns  : bool
		bool StartSync();

		// Summary  : 向设备发送结束同步命令
		// Returns  : void
		void StopSync();

	private:

		//  发送消息命令
		bool SendNotification(std::string notification);

		//	开始通知
		bool StartNotification();

		//	结束通知
		bool CloseNotification();

		const DeviceInfo*		_pDevInfo;

		bool					_isRuning;			// 判断消息是否已经在执行

		void*					_socketHandle;		// socket通信句柄

		afc_file_ref			_ref;				// 


	};

}
