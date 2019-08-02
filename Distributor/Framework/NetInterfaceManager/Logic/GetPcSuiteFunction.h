#pragma once

/////////获取功能大全////////////
class GetPcSuiteFunction
{
public:
	GetPcSuiteFunction(void);
	~GetPcSuiteFunction(void);

	//第一个参数获得android的配置，第二个获得ios的配置
	void GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);

private:
	wstring Request(wstring wsUrl=L"");

	//解析服务端返回的xml文件
	bool AnalyzeXml(wstring wsReturnData, vector<FunctionItem>& PcSuitFunction);

	//检查图片是否已下载，未下载则进行下载操作
	void CheckAndDownLoadIcon(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);
	
	//读取操作系统的名称   
	void GetSystemName();


private:
	wstring _wsOsname;
};
