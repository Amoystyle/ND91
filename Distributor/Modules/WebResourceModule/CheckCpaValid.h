#pragma once
#include "Logic/WebResourceInnerMsg.h"
class CCheckLocalCpaValid
	:BaseGetData<CheckCpaInstalledMsg>,
	BaseHandler<DeleteLocalCpa>
{
public:
	CCheckLocalCpaValid(void);
	~CCheckLocalCpaValid(void);

public:
	virtual bool Handle(CheckCpaInstalledMsg* pMsg);

	virtual bool Handle( const DeleteLocalCpa* pMsg );

private:
	//��鱾��cpa�ļ����Ƿ���Ҫ�����˷��ͼ�����Ϣ
	void CheckLocal(const map<string, wstring>& map_CpaInfo);
	//�������ص�CPA jason��Ϣ
	vector<string> ParseCpaJason(string& wsInfo);
	//ɾ��������Ч��cpa���
	void DeleteInvalidCpa(const map<string, wstring>& map_CpaInfo, vector<string>& vec_InvalidCpaInfo);

	bool ParseServerReturn(string& strData, string& strReturn);
};
