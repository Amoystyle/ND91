#pragma once
#include "WebSoftInfo.h"
#include "Common/cJSON.h"


// �������ݽ�����
class WebSoftHelp 
{
public:
	WebSoftHelp(PlatFrom platType);
	~WebSoftHelp();
	
	// Summary  : ����24Сʱ�ȵ�Ӧ�÷�������
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftData * > & pVerDatas		����Ӧ�������б�
	bool Pare24HotSoftDatas(std::string str, vector<WebSoftData*>& pVerDatas);

	// Summary  : �������صķ�������
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftCategory * > & pVerCats		���ط��������б�
	bool PareCategoryData(std::string str, vector<WebSoftCategory*>& pVerCats);

	// Summary  : ��������,��ҳ�������ص�Ӧ������
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftData * > & pVerDatas		����Ӧ�������б�
	// Parameter: int& total								�÷���Ӧ������
	bool PareSoftDatas(std::string str, vector<WebSoftData*>& pVerDatas, int& total, bool isSearch = false);


	// Summary  : ��������Ӧ����ϸ����
	// Returns  : WebSoftData*
	// Parameter: std::string str
	WebSoftData* PareAppData(std::string str);

	// Summary  : ��������Ӧ��ͼƬ��˵��
	// Returns  : void
	// Parameter: std::string str
	// Parameter: vector<std::string> verPics			ͼƬurl�б�
	void GetDescPicture(std::string str, vector<std::string>& verPics );


private:
	
	// Summary  : ��������Ӧ����Ϣ
	// Returns  : WebSoftData*			��������
	// Parameter: void * pdata			Json����
	WebSoftData* PareData(cJSON* pdata, bool isSearch = false);

	wstring CjsonString(cJSON* pRoot,std::string key);

	int CjsonNumber(cJSON* pRoot,std::string key);
	
	PlatFrom			_platType;		// ƽ̨����

};