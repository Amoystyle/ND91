#pragma once

class DeviceBaseThread
	: public BaseThread
{
public:
	DeviceBaseThread(string threadID, wstring deviceID);
	~DeviceBaseThread();

	virtual bool CheckExit();

private:
	bool _deviceExit;

};