#pragma once

#include "ScreenShotSubject.h"

#ifndef WIN32
	#include "Common/winPort.h"
	void* ScreenShotPlayerRun (void* lpParameter);
#endif

namespace ND91Assistant
{

// ��Ļ��ʾ
class ScreenShotPlayer:public ScreenShotSubject
{
public:
	ScreenShotPlayer(const DeviceData* pDevice);
	virtual ~ScreenShotPlayer(void);

	// ���������߳�
	virtual void StartThread();

	// ֹͣ�����߳�
	virtual void StopThread();

	// Summary  : �����������ȡ��ĻͼƬ
	// Returns	: bool 
	bool GetScreenShot();

private:

#ifdef WIN32
	//�߳�
	static DWORD WINAPI ScreenShotPlayerRun(LPVOID lpParameter);
#endif	

//private:
public:

	HANDLE	_hThread;					// �������߳̾��

	bool	_bThreadQuitSign;			// ��־�߳��Ƿ��˳�	

	int		_nSilenceTime;				// ���ʱ��
};

}
