#pragma once

#ifdef NETDISPATCHCONTROLLER_EXPORTS
#define NETDISPATCHCONTROLLER_API __declspec(dllexport)
#else
#define NETDISPATCHCONTROLLER_API __declspec(dllimport)
#endif

#include "GetPcSuiteFunction.h"

//网络通信控制
class NETDISPATCHCONTROLLER_API NetDispatchController
{
private:
	NetDispatchController(void);
	~NetDispatchController(void);

public:
	void Init();

	static NetDispatchController* GetInstance();

	//从服务端获取第三方功能配置
	void GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);

	//获得本次下载的任务ID
	static UINT GetDownloadFileTaskID();
	
	//下载文件，具有汇报功能
	static void DownloadFileReportProgress(string wsDownPath, wstring wsSavePath, UINT unTaskID);
private:
	static NetDispatchController* _pNetDispatchController;

	GetPcSuiteFunction _PcSuiteFunction;
};
