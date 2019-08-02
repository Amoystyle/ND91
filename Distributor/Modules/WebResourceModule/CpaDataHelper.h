#pragma once
using namespace std;
class CSafeMap;
class CCpaThread;

#include "CWebResourceMacros.h"
class CCpaDataHelper : 
	public BaseHandler<LocalCpaAddOrRemoveMsg>,
	public BaseGetData<CpaWebIsCancelMsg>
{
	friend class CCpaThread;
public:
	static CCpaDataHelper* GetInstance();
	void Init();
	virtual ~CCpaDataHelper(void);

	//android cpa是否初始化完成
	PROPERTYREALONLY(bool, _bAndroidInitFinished,AndroidInitFinished)
	//ios cpa是否初始化完成
	PROPERTYREALONLY(bool, _bIOSInitFinished,IOSInitFinished)

	vector<wstring> GetMultiItemPath(const vector<wstring>& vec_Hash);
	wstring GetItemPath(string hashValue);
	wstring GetAndroidItemPath(string hashValue);
	wstring GetIOSItemPath(string hashValue);
	map<string,wstring>* GetAllDataCopy();
private:
	CCpaDataHelper(void);
	static CCpaDataHelper* _instance;
	CSafeMap* _pAndroidCpaData;
	CSafeMap* _pIOSCpaData;
	void InitAndroidCpaHashValue();
	void InitIOSCpaHashValue();
	void InitInternal();

	virtual bool Handle( const LocalCpaAddOrRemoveMsg* pMsg );

	virtual bool Handle( CpaWebIsCancelMsg* pMsg );

	//size_t readBinFile(wstring strFilePath, char** ppData);

};
