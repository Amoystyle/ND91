#pragma once

class NetInterfaceManager
	: public BaseGetData<GetDownloadFileReportTaskIDMsg>
	, public BaseHandler<DownloadFileReportMsg>
	, public BaseGetData<GetPcSuitFunctionMsg>
{
public:
	NetInterfaceManager(void);
	~NetInterfaceManager(void);

	void Init();

	static void GetPCSuitFunc();

	//获取下载文件的任务ID
	bool Handle(GetDownloadFileReportTaskIDMsg* pMsg);

	//下载有汇报进度的文件
	bool Handle(const DownloadFileReportMsg* pMsg);

	//用户请求获取功能大全
	bool Handle(GetPcSuitFunctionMsg* pMsg);

	
private:
	static vector<FunctionItem> _Android_PcSuitFunction;
	static vector<FunctionItem> _IOS_PcSuitFunction;

};
