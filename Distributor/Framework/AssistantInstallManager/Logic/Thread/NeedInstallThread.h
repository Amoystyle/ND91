#pragma once
class NeedInstallThread : public BaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    NeedInstallThread( string threadID, wstring deviceID );
	~NeedInstallThread();

	virtual bool CheckExit();

private:
	virtual unsigned long Main();
	
	bool _deviceExit;

};