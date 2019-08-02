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
	vector<WebSoftData*>	*_VerDatas;	// ���������б�
	string					_url;	

	int						*_total;		// �÷��������Ӧ����
};