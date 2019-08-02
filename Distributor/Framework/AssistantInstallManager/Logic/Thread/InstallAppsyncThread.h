#pragma once




class InstallAppsyncThread:public BaseThread

	
{
public:
	InstallAppsyncThread(const string id);
	~InstallAppsyncThread();

	unsigned long Main();


	HANDLE m_event_lock;
	wstring m_deviceid;

};