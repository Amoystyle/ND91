#pragma once
#include <map>
#include <vector>
using namespace std;
class BaseCSLock;
class CSafeMap : private BaseCSLock
{
public:
	CSafeMap(void);
	~CSafeMap(void);
	void Insert(string key, wstring value);
	wstring GetValue(string key);
	void Earse(string key);
	void Clear();
	//ֻ�Ǹ����������Լ�ɾ
	map<string,wstring>* GetMapCopy();
private:
	map<string,wstring>* _pMap;

};
