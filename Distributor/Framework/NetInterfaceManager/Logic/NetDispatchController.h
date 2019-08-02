#pragma once

#ifdef NETDISPATCHCONTROLLER_EXPORTS
#define NETDISPATCHCONTROLLER_API __declspec(dllexport)
#else
#define NETDISPATCHCONTROLLER_API __declspec(dllimport)
#endif

#include "GetPcSuiteFunction.h"

//����ͨ�ſ���
class NETDISPATCHCONTROLLER_API NetDispatchController
{
private:
	NetDispatchController(void);
	~NetDispatchController(void);

public:
	void Init();

	static NetDispatchController* GetInstance();

	//�ӷ���˻�ȡ��������������
	void GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);

	//��ñ������ص�����ID
	static UINT GetDownloadFileTaskID();
	
	//�����ļ������л㱨����
	static void DownloadFileReportProgress(string wsDownPath, wstring wsSavePath, UINT unTaskID);
private:
	static NetDispatchController* _pNetDispatchController;

	GetPcSuiteFunction _PcSuiteFunction;
};
