#pragma once

// ��ѯʱ������1000ms
#define IOS_DEFAULT_PROCESS_FIND_INTERVAL   4000


namespace ND91Assistant
{
	// ios Խ�����
    class IosjailBreakMonitorThread
    {

    public:
        IosjailBreakMonitorThread();
        virtual ~IosjailBreakMonitorThread();

        // �����߳�
        virtual int StartThread();

        // ֹͣ�߳�
        virtual void StopThread();


		//�߳�
		static DWORD WINAPI IosjailBreakMonitorThreadRun(LPVOID lpParameter);


		static DWORD WINAPI TestDeviceIsjailBreakThread( PVOID lpParam );


		//�����豸�Ƿ�Խ���߳��˳��ж�
		bool IsTestThreadExit( int nWaitTime );

		void StartTestDevice(){_bTestDeviceRun = true;}
		void StopTestDevice(){_bTestDeviceRun = false;}

		HANDLE	_hTestDeviceThread;			//�豸Խ������߳̾��
	private:
		bool _bTestDeviceRun;
		HANDLE _hExitEvent;                 // �˳��¼�

		HANDLE	_hThread;					// �������߳̾��

		bool	_bThreadQuitSign;			// ��־�߳��Ƿ��˳�	

		int		_nSilenceTime;				// ���ʱ��
    };
}
