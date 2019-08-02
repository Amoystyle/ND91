#include "stdafx.h"
#include "CloseMutex.h"
#include "Core/DeviceManager.h"
#include<algorithm>

void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
/*���ﲻҪ��INTERFUN;*/{
	//����Ƿ񴴽��˹ر��ź���
	HANDLE hMutex;
	hMutex	= ::OpenMutex(0x1F0001, 0, MUTEX_CLOSEMAINAPP);
	if (hMutex)
	{
		DEVICE_MANAGER->Release();
		::CloseHandle(hMutex);
		KillTimer(hwnd,idEvent);
		exit(1);	//�����˳�����ʱ����exit  �����޸��Ժ��޸�
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
