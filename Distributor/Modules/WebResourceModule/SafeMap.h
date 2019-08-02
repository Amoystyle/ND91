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
	//只是副本，用完自己删
	map<string,wstring>* GetMapCopy();
private:
	map<string,wstring>* _pMap;

};
