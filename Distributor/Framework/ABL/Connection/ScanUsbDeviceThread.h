#pragma once
#include "Base/BaseThread.h"

class ScanUsbDeviceThread 
	: public BaseThread
	, public BaseCSLock
{
public:
    // 构造函数 threadID线程唯一标识
    ScanUsbDeviceThread( string threadID);
	~ScanUsbDeviceThread();

	void ActiveScan();

private:
	virtual unsigned long Main();

	void Scan();

	bool _bActiveScan;
	bool _bScanning;
	int _nScanCount;
	UINT _nScanStep;
};