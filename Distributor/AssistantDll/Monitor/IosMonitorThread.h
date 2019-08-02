#pragma once

#include "MonitorThread.h"
#include "Device/IosDaemon.h"
#include "Device/AFCFileSysemBase.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/IosWifiConnection.h"

// Ĭ���ػ����������ʱ��(ms)����������Ϊ�豸�Ͽ�
// ��ʱ������Option�����ã�option���Ҳ�����ʹ�ô�Ĭ��ֵ
#define IOS_DEFAULT_CHECK_DEAMON   20000

// ��ѯʱ������100ms
#define IOS_DEFAULT_POLL_INTERVAL   100

#ifndef WIN32
	void* IosMonitorThreadRun (void* lpParameter);
#endif

namespace ND91Assistant
{
	// ios �����̣߳��ػ���������WIFI����״̬
    class IosMonitorThread : public MonitorThread
    {

    public:
        IosMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~IosMonitorThread();

        // ���������߳�
        // ����ֵ: 0: ʧ�� 1: �ɹ� 
        virtual int StartThread();

        // ֹͣ�����߳�
        virtual void StopThread();

	//private:
#ifdef WIN32		
		//�߳�
		static DWORD WINAPI IosMonitorThreadRun(LPVOID lpParameter);
#endif		

        // ��ʼ��
        // ���أ�true�ɹ� false ʧ��
        bool Init();

        // �ͷų�Ա����ָ��
        void Release();

    //private:
        IosDaemon*          _daemon;        // �ػ�������
        IIosFileSystem*     _pFileStream;   // �ļ���д��
        IosWifiConnection*  _pWiFiConn;     // wifi������

    };
}
