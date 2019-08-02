#pragma once

#ifdef WIN32
	#include "ITunesMobileDevice.h"
	#include "AirTrafficHost.h"
#else
	#include "MobileDevice.h"
#endif
#include "IConnection.h"
#include <string>
#include <sstream>
using namespace std;

namespace ND91Assistant
{
    void OnAMDConnectionNotify(struct am_device_notification_callback_info *info);
    void OnDfuDeviceConnected(void * device);
    void OnDfuDeviceDisconnected(void * device);
    void OnrecoveryConnected(void * device);
    void OnrecoveryDisconnected(void * device);	

	typedef enum iTunesVersionType
	{
        // <summary>
        // 10.5��ʼ��iTunes�ٷ�֧���豸wifi����,�豸�̼���Ҫ10.5
        // <remarks>ע�����ö�ٳ�������iTunesWifi�Ƿ��Զ������ж�,�޸���ע��</remarks>
        // </summary>
        iTunes105,
		iTunes92,
		iTunes91,
		iTunes82,
		iTunesOld
	};

	//
	// ios�豸�Ļ�����ʼ��������iTunes�������ûص�����
	// 
	class CITunesService
	{
	public:
		//��ʼ��IOS����
		static bool InitIosEnviroment();
		static bool RegisterIosDeviceCallback();
		static void UnRegisterIosDeviceCallback();

		//��ȡITUNES�汾
		static iTunesVersionType GetiTunesVersionType(){return _itunesType;}

		static void InitStateInfo(); //��ʼ��״̬��Ϣ

		static void ReConnPasswordLockDevice(void* pConnHandle);

	private:		
		// �ص�����������״̬��̬��ÿ̨�豸����һ�λص����β嶼��
		friend void OnAMDConnectionNotify(struct am_device_notification_callback_info *info);

		// �ص�������Dfu״̬
		friend void OnDfuDeviceConnected(void * device);
		friend void OnDfuDeviceDisconnected(void * device);

		// �ص�������recovery״̬
		friend void OnrecoveryConnected(void * device);
		friend void OnrecoveryDisconnected(void * device);	

		// ��� iTunesMobileDevice.dll �ļ��汾��
		static void CheckiTunesMobileDeviceVersion(wstring path);

	private:
		// IOS������ʼ���Ƿ����
		static bool _bIosEvnInited;

		// iTunesMobileDevice.dll �ļ��汾��
		static iTunesVersionType _itunesType;

		// �ص������Ƿ�������
		static bool _bCallbackInited;

		// �����豸���ӻص������е��ã�
		static void Connect(ND91Assistant::IConnection* pConn, void * device);

		// �Ͽ����ӣ��ӻص������е��ã�
		static void Disconnect(ND91Assistant::IConnection* pConn, void * device);

        // �����豸���ӻص������е��ã�
        static void DFUConnect(ND91Assistant::IConnection* pConn, void * device);

        // �Ͽ����ӣ��ӻص������е��ã�
        static void DFUDisConnect(ND91Assistant::IConnection* pConn, void * device);

        // �����豸���ӻص������е��ã�
        static void RecoveryConnect(ND91Assistant::IConnection* pConn, void * device);

        // �Ͽ����ӣ��ӻص������е��ã�
        static void RecoveryDisConnect(ND91Assistant::IConnection* pConn, void * device);

		static void* _subscribe_info_ptr;

		static bool _bDevConnAlready; //ע��ص����Ƿ����豸���ӹ�
	};
}

