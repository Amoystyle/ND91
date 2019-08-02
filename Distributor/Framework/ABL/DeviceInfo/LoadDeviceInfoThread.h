#pragma once
#include "Common/DeviceBaseThread.h"

class LoadDeviceInfoThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    LoadDeviceInfoThread( string threadID, int nRequestID, wstring deviceID );
	~LoadDeviceInfoThread();

	void SetDeviceInfo(AblDeviceInfo* pInfo);

	void SetIsRefresh(bool bRefresh);

private:
	virtual unsigned long Main();

    void LoadInfo();

	void GetBaseInfo();

	void GetDeviceState();
	void GetDeviceName();

	wstring GetIosDeviceRegion(wstring ios_deviceRegionInfo);
private:
	AblDeviceInfo* _pInfo;
	int _nRequestID;
	bool _bRefresh;
};