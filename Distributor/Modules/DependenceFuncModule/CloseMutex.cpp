#include "stdafx.h"
#include "CloseMutex.h"
#include "Core/DeviceManager.h"
#include<algorithm>

void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
/*这里不要加INTERFUN;*/{
	//检测是否创建了关闭信号量
	HANDLE hMutex;
	hMutex	= ::OpenMutex(0x1F0001, 0, MUTEX_CLOSEMAINAPP);
	if (hMutex)
	{
		DEVICE_MANAGER->Release();
		::CloseHandle(hMutex);
		KillTimer(hwnd,idEvent);
		exit(1);	//程序退出，暂时调用exit  如需修改稍后修改
	}
}



CloseMutex::CloseMutex()
{
	
}

CloseMutex::~CloseMutex()
{

}

void CloseMutex::Init()
{
	::SetTimer(NULL,0,3000,&TimerProc);
}
