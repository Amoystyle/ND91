#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "ScreenShotPlayer.h"
#include "ScreenshotCommand.h"

ScreenShotPlayer::ScreenShotPlayer(const DeviceData* pDevice)
:ScreenShotSubject(pDevice)
{
	_hThread =NULL;
	_bThreadQuitSign = false;
}

ScreenShotPlayer::~ScreenShotPlayer(void)
{
	if (! _bThreadQuitSign)
		StopThread();
}

void ND91Assistant::ScreenShotPlayer::StartThread()
{
	_bThreadQuitSign = false;

	// 启动监听线程，保持接收
	DWORD	nThreadId;
#ifdef WIN32	
	_hThread = CreateThread(NULL, 0, ScreenShotPlayer::ScreenShotPlayerRun, (LPVOID)this, 0, &nThreadId);
#else
	pthread_create(&_hThread, NULL, ScreenShotPlayerRun, (LPVOID)this);
#endif
}

void ND91Assistant::ScreenShotPlayer::StopThread()
{
	if (_hThread && !_bThreadQuitSign)
	{
		_bThreadQuitSign = true;
		//WaitForSingleObject(_hThread,INFINITE); 
		WaitForSingleObject(_hThread, 1000); 
	}
	_hThread= NULL;

	ResetScreenImageShow();//推出全屏时, 显示不缩放，不旋转
}

#ifdef WIN32
	DWORD WINAPI ND91Assistant::ScreenShotPlayer::ScreenShotPlayerRun( LPVOID lpParameter )
#else
	void* ScreenShotPlayerRun(void* lpParameter)
#endif		
{
	ScreenShotPlayer* pThread = (ScreenShotPlayer*)lpParameter;

	while (!pThread->_bThreadQuitSign)
	{
		Sleep(50);  // 轮询时间间隔
		pThread->_nSilenceTime += 50;

		pThread->GetScreenShot();
	}

	pThread->_bThreadQuitSign = true;
	return 0L;
}

bool ND91Assistant::ScreenShotPlayer::GetScreenShot()
{
	ScreenshotCommand cmd(_pDevice,L"");
	cmd.Execute();
	SetScreenImageBinaryData(cmd.GetScreenImageBinaryData());

	return !cmd.GetScreenImageBinaryData().empty();
}


#endif