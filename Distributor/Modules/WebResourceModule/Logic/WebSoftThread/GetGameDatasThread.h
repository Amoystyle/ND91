#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetGameDatasThread:public BaseThread
{
public:
	GetGameDatasThread(const string id);
	~GetGameDatasThread();

	unsigned long Main();

	PlatFrom				_platType;
	vector<WebSoftData*>	*_VerDatas;	// 返回数据列表
	string					_url;		
	int						*_total;		// 该分类包含总应用数
};