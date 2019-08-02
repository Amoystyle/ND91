#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class SearchAppsThread:public BaseThread
{
public:
	SearchAppsThread(const string id);
	~SearchAppsThread();

	unsigned long Main();

	PlatFrom			_platType;
	string				_url;
	vector<WebSoftData*>		*_VerDatas;	// 返回数据列表
	int							*_total;		// 该分类包含总应用数
};