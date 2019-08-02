#include "stdafx.h"
#include "ScanUsbDeviceThread.h"

ScanUsbDeviceThread::ScanUsbDeviceThread( string threadID )
						: BaseThread(threadID)
{
	_bActiveScan = false;
	_bScanning = false;
	_nScanCount = 0;
	_nScanStep = 0;
}

ScanUsbDeviceThread::~ScanUsbDeviceThread()
{
}

unsigned long ScanUsbDeviceThread::Main()
{
	while(!CheckExit())
	{
		bool bTimeOut = (GetTickCount() - _nScanStep) > 15000;
		if(!_bActiveScan && !bTimeOut)
		{
			Sleep(300);
			continue;
		}

		if(bTimeOut)
		{
			LOG->WriteDebug(L"ScanUsbDeviceThread: Timeout");
		}

		if(!_bScanning)
		{
			_bScanning = true;
			this->_bActiveScan = false;
			_nScanCount++;

			this->Scan();

			_nScanStep = GetTickCount();

			_nScanCount = 0;

			_bScanning = false;
		}
	}

	LOG->WriteDebug(L"ScanUsbDeviceThread::Scan Exit");

	return 0;
}

void ScanUsbDeviceThread::ActiveScan()
{
	++_nScanCount;

	this->_bActiveScan = true;
}

void ScanUsbDeviceThread::Scan()
{
	try
	{
		if(_nScanCount > 0)
		{
			DEVICE_MANAGER->ScanUsbDevice();

			LOG->WriteDebug(L"ScanUsbDeviceThread::Scan  Step2");
		}

		Sleep(1500);

		if(_nScanCount > 0)
		{
			_nScanCount--;
			this->Scan();
		}
	}
	catch (AssistantException* e)
	{
		LOG->WriteDebug(L"ScanUsbDeviceThread::Scan  Error_AssistantException");		
	}
	catch (...)
	{
		LOG->WriteDebug(L"ScanUsbDeviceThread::Scan  Error_...");
	}
}
