#pragma once

/////////��ȡ���ܴ�ȫ////////////
class GetPcSuiteFunction
{
public:
	GetPcSuiteFunction(void);
	~GetPcSuiteFunction(void);

	//��һ���������android�����ã��ڶ������ios������
	void GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);

private:
	wstring Request(wstring wsUrl=L"");

	//��������˷��ص�xml�ļ�
	bool AnalyzeXml(wstring wsReturnData, vector<FunctionItem>& PcSuitFunction);

	//���ͼƬ�Ƿ������أ�δ������������ز���
	void CheckAndDownLoadIcon(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc);
	
	//��ȡ����ϵͳ������   
	void GetSystemName();


private:
	wstring _wsOsname;
};
