#pragma once
class NeedInstallThread : public BaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    NeedInstallThread( string threadID, wstring deviceID );
	~NeedInstallThread();

	virtual bool CheckExit();

private:
	virtual unsigned long Main();
	
	bool _deviceExit;

};