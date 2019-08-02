#include "stdafx.h"
#include "IosjailBreakMonitorThread.h"
#include "Core/DeviceManager.h"
#include "Core/DeviceData.h"
#include "Core/DeviceInfo.h"
#include "Device/IosUsbConnection.h"

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{	
	IosjailBreakMonitorThread* pThread = (IosjailBreakMonitorThread*)lParam;
	TCHAR szTitle[200];
//	TCHAR szClass[200];
	GetWindowText(hwnd, szTitle, sizeof(szTitle) / sizeof(TCHAR)); // ��ȡ��������
	//GetClassName(hWnd, szClass, sizeof(szClass) / sizeof(TCHAR)); // ������
	wstring winTitleName(szTitle);
	if(winTitleName.find(L"Absinthe")!=wstring::npos)
	{
		pThread->StartTestDevice();
	}
	
	return TRUE;
}

IosjailBreakMonitorThread::IosjailBreakMonitorThread()
{
	 _bThreadQuitSign = true;
	 _hThread = NULL;
	 _nSilenceTime = 0;
	 _bTestDeviceRun=false;
	 _hExitEvent = CreateEvent(NULL,true, false, NULL);

}

IosjailBreakMonitorThread::~IosjailBreakMonitorThread()
{ 
	 
     StopThread();
}

DWORD WINAPI IosjailBreakMonitorThread::IosjailBreakMonitorThreadRun(LPVOID lpParameter)
{
	//��鵱ǰ�Ƿ���Խ�������������У��Ȳ����ţ� ����ط����ǻ����
	/*
	IosjailBreakMonitorThread* pThread = (IosjailBreakMonitorThread*)lpParameter;	
	
	while (! pThread->_bThreadQuitSign)
	{
		   Sleep(IOS_DEFAULT_PROCESS_FIND_INTERVAL);  // ��ѯʱ����
		
			//���Խ�������Ƿ������� 
		   try
		   {
			   EnumWindows(lpEnumFunc, (LPARAM)lpParameter);  
		   }
		   catch (...)
		   {
		   }  
			Sleep(1000);
			//ö�������ͣ�̣߳���
			pThread->StopTestDevice();
	}

    pThread->_bThreadQuitSign = true;
	*/
	return 0L;
}


int IosjailBreakMonitorThread::StartThread()
{
    if ( ! _bThreadQuitSign )
    {
        return true;
    }

   
	_bThreadQuitSign = false;

	// ���������̣߳����ֽ���
	DWORD	nThreadId;

	_hThread = CreateThread(NULL, 0, IosjailBreakMonitorThread::IosjailBreakMonitorThreadRun, (LPVOID)this, 0, &nThreadId);

	if (_hThread)
		_hTestDeviceThread = CreateThread(NULL, 0, IosjailBreakMonitorThread::TestDeviceIsjailBreakThread, (LPVOID)this, 0, &nThreadId);
	else
		return false;

	if (_hThread)
		return true;

	return false;
}
DWORD WINAPI IosjailBreakMonitorThread::TestDeviceIsjailBreakThread( PVOID lpParam )
{
	IosjailBreakMonitorThread* MonitorThread = (IosjailBreakMonitorThread*)lpParam;
 
	if (MonitorThread->IsTestThreadExit(100)) goto THREAD_END;

	while ( !MonitorThread->IsTestThreadExit(300) )
	{        
		Sleep(1000);
		if (!MonitorThread->_bTestDeviceRun)
		{
			continue;
		}  
		try
		{
			vector<wstring> DeviceList = DEVICE_MANAGER->GetAllDevice();
			for(size_t i=0;i<DeviceList.size();i++)
			{
				DeviceData* dev = DEVICE_MANAGER->FindDevice(DeviceList.at(i));
				if(!dev)
					continue;
				if(dev->GetDevInfo()->_deviceType == Ios && dev->GetDevInfo()->_ios_bJailBreaked == false
					&&dev->GetDevInfo()->_connectType == USB )
				{
					IosUsbConnection usbconnect((DeviceInfo*)dev->GetDevInfo());
					if(usbconnect.IsJailBreak()==0)
					{
						//Խ����					
						DEVICE_MANAGER->OnIosDevJailBreak(dev->GetDevInfo()->_ios_deviceIdentifier);
					}
				}
			}
		}
		catch (...)
		{
		}
	}
THREAD_END:
  return 0L;   
}

bool IosjailBreakMonitorThread::IsTestThreadExit( int nWaitTime )
{    
	return WaitForSingleObject(_hExitEvent, nWaitTime) != WAIT_TIMEOUT;
}



void IosjailBreakMonitorThread::StopThread()
{	
    if (_hThread &&  ! _bThreadQuitSign )
    {
	    _bThreadQuitSign = true;
	    WaitForSingleObject(_hThread,1000); 
    }
	SetEvent(_hExitEvent);
	WaitForSingleObject(_hTestDeviceThread, 5000);

}



