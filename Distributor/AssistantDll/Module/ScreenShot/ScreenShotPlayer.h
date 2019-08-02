#pragma once

#include "ScreenShotSubject.h"

#ifndef WIN32
	#include "Common/winPort.h"
	void* ScreenShotPlayerRun (void* lpParameter);
#endif

namespace ND91Assistant
{

// 屏幕演示
class ScreenShotPlayer:public ScreenShotSubject
{
public:
	ScreenShotPlayer(const DeviceData* pDevice);
	virtual ~ScreenShotPlayer(void);

	// 启动监听线程
	virtual void StartThread();

	// 停止监听线程
	virtual void StopThread();

	// Summary  : 调截屏命令，获取屏幕图片
	// Returns	: bool 
	bool GetScreenShot();

private:

#ifdef WIN32
	//线程
	static DWORD WINAPI ScreenShotPlayerRun(LPVOID lpParameter);
#endif	

//private:
public:

	HANDLE	_hThread;					// 开启的线程句柄

	bool	_bThreadQuitSign;			// 标志线程是否退出	

	int		_nSilenceTime;				// 间隔时间
};

}
