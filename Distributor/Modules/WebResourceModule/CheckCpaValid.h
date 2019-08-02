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
	//检查本地cpa文件夹是否需要向服务端发送检验信息
	void CheckLocal(const map<string, wstring>& map_CpaInfo);
	//解析返回的CPA jason信息
	vector<string> ParseCpaJason(string& wsInfo);
	//删除本地无效的cpa软件
	void DeleteInvalidCpa(const map<string, wstring>& map_CpaInfo, vector<string>& vec_InvalidCpaInfo);

	bool ParseServerReturn(string& strData, string& strReturn);
};
