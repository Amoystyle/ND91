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

	//��ȡ�����ļ�������ID
	bool Handle(GetDownloadFileReportTaskIDMsg* pMsg);

	//�����л㱨���ȵ��ļ�
	bool Handle(const DownloadFileReportMsg* pMsg);

	//�û������ȡ���ܴ�ȫ
	bool Handle(GetPcSuitFunctionMsg* pMsg);

	
private:
	static vector<FunctionItem> _Android_PcSuitFunction;
	static vector<FunctionItem> _IOS_PcSuitFunction;

};
