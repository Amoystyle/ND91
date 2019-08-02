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

		// Summary  : ���豸���Ϳ�ʼͬ������
		// Returns  : bool
		bool StartSync();

		// Summary  : ���豸���ͽ���ͬ������
		// Returns  : void
		void StopSync();

	private:

		//  ������Ϣ����
		bool SendNotification(std::string notification);

		//	��ʼ֪ͨ
		bool StartNotification();

		//	����֪ͨ
		bool CloseNotification();

		const DeviceInfo*		_pDevInfo;

		bool					_isRuning;			// �ж���Ϣ�Ƿ��Ѿ���ִ��

		void*					_socketHandle;		// socketͨ�ž��

		afc_file_ref			_ref;				// 


	};

}
