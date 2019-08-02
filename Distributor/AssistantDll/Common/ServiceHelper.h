#pragma once

#include <Windows.h>
#include <winsvc.h>

#include <string>

class ServiceHelper
{
public:
	ServiceHelper(std::wstring name);
	
    // ��������
	bool Start();

    // ֹͣ����
    void Stop();

	// ������ѯ������������֮�˻�������ȴ�ʱ�䣩
	// interval��������ѯ�ļ��ʱ�䣬��λ����
	// repeat���ظ����ٴ�
	void SetPollParameter(int interval, int repeat);
	
private:
    // ��Ϊ��ֹ��������Ϊ�Զ�����
    bool SetStartType(SC_HANDLE hService);

    // ��ѯ����״̬��������ָ��״̬����true�����򷵻�false
    bool PollServiceStatus(SC_HANDLE hService, SC_HANDLE hHandle, DWORD waiting_status);

private:
	std::wstring _name;	// service name
	int _interval;		// sleep time between 2 query, unit: microsecond
	int _repeat;		// repeat times of query
};
