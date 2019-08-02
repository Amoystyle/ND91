#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetAppDatasThread:public BaseThread
{
public:
	GetAppDatasThread(const string id);
	~GetAppDatasThread();

	unsigned long Main();

	PlatFrom				_platType;
	vector<WebSoftData*>	*_VerDatas;	// 返回数据列表
	string					_url;	

	int						*_total;		// 该分类包含总应用数
};