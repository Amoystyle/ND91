#pragma once
#include "WebSoftInfo.h"
#include "Common/cJSON.h"


// 返回数据解析类
class WebSoftHelp 
{
public:
	WebSoftHelp(PlatFrom platType);
	~WebSoftHelp();
	
	// Summary  : 解析24小时热点应用返回数据
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftData * > & pVerDatas		返回应用数据列表
	bool Pare24HotSoftDatas(std::string str, vector<WebSoftData*>& pVerDatas);

	// Summary  : 解析返回的分类数据
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftCategory * > & pVerCats		返回分类数据列表
	bool PareCategoryData(std::string str, vector<WebSoftCategory*>& pVerCats);

	// Summary  : 解析查找,分页搜索返回的应用数据
	// Returns  : bool
	// Parameter: std::string str
	// Parameter: vector<WebSoftData * > & pVerDatas		返回应用数据列表
	// Parameter: int& total								该分类应用总数
	bool PareSoftDatas(std::string str, vector<WebSoftData*>& pVerDatas, int& total, bool isSearch = false);


	// Summary  : 解析单个应用详细数据
	// Returns  : WebSoftData*
	// Parameter: std::string str
	WebSoftData* PareAppData(std::string str);

	// Summary  : 解析单个应用图片等说明
	// Returns  : void
	// Parameter: std::string str
	// Parameter: vector<std::string> verPics			图片url列表
	void GetDescPicture(std::string str, vector<std::string>& verPics );


private:
	
	// Summary  : 解析单个应用信息
	// Returns  : WebSoftData*			返回数据
	// Parameter: void * pdata			Json数据
	WebSoftData* PareData(cJSON* pdata, bool isSearch = false);

	wstring CjsonString(cJSON* pRoot,std::string key);

	int CjsonNumber(cJSON* pRoot,std::string key);
	
	PlatFrom			_platType;		// 平台类型

};