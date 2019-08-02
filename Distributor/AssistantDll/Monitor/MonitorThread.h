#pragma once

#include "DeamonMessageSubject.h"
#include "Monitor/DeviceSubject.h"
#include "Core/DeviceInfo.h"
#include "DeamonMessageSubject.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include "Common/winPort.h"
#endif

namespace ND91Assistant
{
    // ����̣߳�ִ��������⣬�����ػ�������Ϣ���¶���֪ͨ����׿�Ķ��Ż�ִ��
    class MonitorThread : public DeamonMessageSubject
    {
    public:
        MonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~MonitorThread();

        // ���������߳�
        // ����ֵ: Tcp ���ӱ��ض˿� 
        virtual int StartThread() = 0;

        // ֹͣ�����߳�
        virtual void StopThread() = 0;

    //protected:

        //�豸���ӶϿ�
        void onDisConnectDevice();

        //�ػ�ֹͣ����
        void onDaemonDisable();

    //protected:
        const DeviceInfo*   _pDeviceInfo;   // �豸��Ϣ

        const std::wstring   _id;            // �豸ID

        HANDLE	_hThread;					// �������߳̾��

        bool	_bThreadQuitSign;			// ��־�߳��Ƿ��˳�	

        int		_nSilenceTime;				// ���ʱ��
    };
}
