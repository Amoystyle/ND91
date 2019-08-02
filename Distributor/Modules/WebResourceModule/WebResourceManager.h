#pragma once

#include "Common/cJSON.h"
#include "NetResourceStruct.h"
#include <map>
#include <string>
using namespace std;

class BaseCSLock;
class CWebResourceManager : private BaseCSLock
{
public:
	CWebResourceManager();
	~CWebResourceManager(void);

	//s:返回服务端发的信息
	//p1:服务端地址，（例子：http://pc5.sj.91.com/controller.ashx?act=getnewclientmenu&datatype=json）
	//p2:返回的数据结构
	//R:是否成功
	bool Get(wstring strURL, NetEnumPhoneOS ePhoneOS, vector<NetModuleData>& stResult);
	bool Get();
	tstring GetNetNodeValue(NetNodeType type);
	wstring GetModuleDataId(NetEnumAppSort appSort);
	vector<NetModuleData> GetNetModuleDatas();
	static CWebResourceManager* GetInstance();
	static void ChangeNetNodeUrl(NetSite &site, ENUM_PLATFORM platform);
	static ENUM_PLATFORM GetPlatformFromUrl(string url);
	static string  ClearSearchKey(string &url);

private:
	// s:加载Web节点，若有更新则读取后写入数据库，若无更新则无操作，前台直接读取数据库即可
	// p: url地址
	bool LoadNewNodeMenu(wstring moduleValue, NetModuleData& stData);
	////以下接口为解析功能
	//NetResultData ParseGetResult(wstring wsReturnData);
	//NetSite		  ParseGetNetSite(cJSON* pSites,NetEnumPhoneOS platform);
	//NetNode		  ParseGetNetNode(cJSON* pNode);
	//time_t		  convert_string_to_time_t( const std::string & time_string );
	void InitNetModuleDataIds();

	void InitNetNodeValues();

	map<NetEnumAppSort,wstring> _mapNetModuleDataIds;
	map<NetNodeType,tstring> _mapNetNodeValues;
	vector<NetModuleData> _stDatas;
private:
	static CWebResourceManager* _pInstance;
};
