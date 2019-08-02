///////////////////////////////////////////////////////////
//  AndroidMonitorThread.h
//  Implementation of the Class AndroidMonitorThread
//  Created on:      01-����-2011 9:42:47
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_)
#define EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_

#include "MonitorThread.h"
#include "Common/BaseSocket.h"
#include "Monitor/DeamonMessageObserver.h"

#define DEMON_PACKAGE_HEAD_SIZE 16

// Ĭ���ػ����������ʱ��(ms)����������Ϊ�豸�Ͽ�
// ��ʱ������Option�����ã�option���Ҳ�����ʹ�ô�Ĭ��ֵ
#define DEFAULT_SILENCE_LIMIT   2000 //TODO: ��Ϊ40000 �ȶ�Ϊ3000���������

// ��ѯʱ������30ms
#define DEFAULT_POLL_INTERVAL   30

#ifndef WIN32
	void* AndroidMonitorThreadRun (void* lpParameter);
#endif

namespace ND91Assistant
{
	// Android �����̣߳��ػ����������¶���֪ͨ�����Ż�ִ
    class AndroidMonitorThread : public MonitorThread 
    {
    public:
        AndroidMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~AndroidMonitorThread();

        // �����߳�
        // ����ֵ: Tcp ���ӱ��ض˿� 
        virtual int StartThread();

        // ֹͣ�߳�
        virtual void StopThread();

		bool  isDeamonVaid() const { return (_pBaseSocket && _pBaseSocket->GetSockLastError()==0 && _bDeamonSockConnect==true); }

	private:

        // ���ͻ��Ѱ����ػ�
        // ���ر������Ӷ˿ڣ�����0��ʾʧ��
        void ActivateDeamon(int nLongConnectSign);

		// �����ػ������������ݰ�,���ݰ����ݷ���_pDemonData
		// ����true��ʾ�ɹ���false��ʾʧ��
		bool getPackage();

		void OnConnectionOK();

		void OnConnectionRetry();
#ifdef WIN32
		//�߳�
		static DWORD WINAPI run(LPVOID lpParameter);
#endif		

    //private:

        CRITICAL_SECTION _csLock;        // �����ٽ�ֵ
        CBaseSocket *_pBaseSocket;          // socketʵ��

		bool _bDeamonSockConnect;
		
		
	};
}

#endif // !defined(EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_)
