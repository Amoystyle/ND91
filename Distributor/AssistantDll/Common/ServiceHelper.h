#pragma once

#include <Windows.h>
#include <winsvc.h>

#include <string>

class ServiceHelper
{
public:
	ServiceHelper(std::wstring name);
	
    // 启动服务
	bool Start();

    // 停止服务
    void Stop();

	// 设置轮询参数（二参数之乘积，即最长等待时间）
	// interval：两次轮询的间隔时间，单位毫秒
	// repeat：重复多少次
	void SetPollParameter(int interval, int repeat);
	
private:
    // 若为禁止启动，改为自动启动
    bool SetStartType(SC_HANDLE hService);

    // 轮询服务状态，若符合指定状态返回true，否则返回false
    bool PollServiceStatus(SC_HANDLE hService, SC_HANDLE hHandle, DWORD waiting_status);

private:
	std::wstring _name;	// service name
	int _interval;		// sleep time between 2 query, unit: microsecond
	int _repeat;		// repeat times of query
};
