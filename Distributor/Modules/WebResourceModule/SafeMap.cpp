#include "StdAfx.h"
#include "SafeMap.h"

CSafeMap::CSafeMap(void)
{
	this->_pMap = new map<string,wstring>();
}

CSafeMap::~CSafeMap(void)
{
	if (NULL != _pMap)
	{
		_pMap->clear();
		delete _pMap;
		_pMap = NULL;
	}
}


void CSafeMap::Insert( string key, wstring value )
{
	AutoLock;
	(*_pMap)[key] = value;
}

wstring CSafeMap::GetValue( string key )
{
	AutoLock;
	if (_pMap->find(key) != _pMap->end())
	{
		return (*_pMap)[key];
	}
	return L"";
}

void CSafeMap::Earse( string key )
{
	AutoLock;
	_pMap->erase(key);
}

void CSafeMap::Clear()
{
	AutoLock;
	_pMap->clear();
}

map<string,wstring>* CSafeMap::GetMapCopy()
{
	AutoLock;
	map<string,wstring>* pCopy = new map<string,wstring>();
	for (map<string,wstring>::iterator iter = _pMap->begin(); iter != _pMap->end(); ++iter)
	{
		(*pCopy)[iter->first] = iter->second;
	}
	return pCopy;

}


