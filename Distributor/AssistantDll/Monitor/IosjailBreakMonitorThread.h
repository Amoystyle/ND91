#pragma once

// 轮询时间间隔：1000ms
#define IOS_DEFAULT_PROCESS_FIND_INTERVAL   4000


namespace ND91Assistant
{
	// ios 越狱监控
    class IosjailBreakMonitorThread
    {

    public:
        IosjailBreakMonitorThread();
        virtual ~IosjailBreakMonitorThread();

        // 启动线程
        virtual int StartThread();

        // 停止线程
        virtual void StopThread();


		//线程
		static DWORD WINAPI IosjailBreakMonitorThreadRun(LPVOID lpParameter);


		static DWORD WINAPI TestDeviceIsjailBreakThread( PVOID lpParam );


		//测试设备是否越狱线程退出判断
		bool IsTestThreadExit( int nWaitTime );

		void StartTestDevice(){_bTestDeviceRun = true;}
		void StopTestDevice(){_bTestDeviceRun = false;}

		HANDLE	_hTestDeviceThread;			//设备越狱检测线程句柄
	private:
		bool _bTestDeviceRun;
		HANDLE _hExitEvent;                 // 退出事件

		HANDLE	_hThread;					// 开启的线程句柄

		bool	_bThreadQuitSign;			// 标志线程是否退出	

		int		_nSilenceTime;				// 间隔时间
    };
}
