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

	//s:���ط���˷�����Ϣ
	//p1:����˵�ַ�������ӣ�http://pc5.sj.91.com/controller.ashx?act=getnewclientmenu&datatype=json��
	//p2:���ص����ݽṹ
	//R:�Ƿ�ɹ�
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
	// s:����Web�ڵ㣬���и������ȡ��д�����ݿ⣬���޸������޲�����ǰֱ̨�Ӷ�ȡ���ݿ⼴��
	// p: url��ַ
	bool LoadNewNodeMenu(wstring moduleValue, NetModuleData& stData);
	////���½ӿ�Ϊ��������
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
