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
	vector<WebSoftData*>	*_VerDatas;	// ���������б�
	string					_url;		
	int						*_total;		// �÷��������Ӧ����
};