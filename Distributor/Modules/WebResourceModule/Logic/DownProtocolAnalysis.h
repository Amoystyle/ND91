#ifndef DownProtocolAnalysis_h__
#define DownProtocolAnalysis_h__

typedef struct AnalysisData 
{
	wstring module;			//��Դ����
	wstring action;			//��������
	wstring identify;		//Ӧ�����Ʊ�ʶ��
	wstring name;			//Ӧ�ó������
	wstring vername;		//Ӧ�ð汾
	wstring vercode;		//Ӧ�ð汾��
	wstring icon;			//ͼ��·��
	wstring info;			//Ӧ������
	wstring src;			//��Դ��Դ
	wstring url;			//��Դ���ص�ַ
	wstring resid;			//fid
	bool    islegal;			//�Ƿ�����
	AnalysisData()
	{
		module = L"";
		action = L"";
		identify = L"";
		name = L"";
		vername = L"";
		vercode = L"";
		icon = L"";
		info = L"";
		src = L"";
		url = L"";
		resid = L"";
		islegal = false;
	}
}AnalysisData;


class DownProtocolAnalysis 
{
public:
	DownProtocolAnalysis();
	~DownProtocolAnalysis();

public:
	//����URL����
	//url��������Ҫ�ֽ��url
	//key�����������ڵĲ�����
	//����ֵ�����key��Ϊ������url���Ƿ���ڶ�Ӧֵ���������򷵻�false ���򷵻�true1
	static bool AnalysisUrl(AnalysisData &data,wstring url,wstring key=_T(""));

	static bool WebSoftAnalysisUrl(AnalysisData &data,wstring url);

private:
	//url�ӽ���
	static tstring UrlEncode(tstring text);	
	static tstring UrlDecode(wstring decode);

	//�����Ӧ�ַ�����=�������ж��ַ���ȷ��
	static wstring CheckValue(wstring buf,wstring key);

	static char hex2char(const char st[]) ;

	//������Ѷ���ؽ���
	static void Parse(map<string,string> &mapParams, wstring url);

	//��ȡ������Դ�ؼ�������Ӧ��ֵ
	static wstring GetValue(map<string,string> &mapData, string key);
};


#endif 