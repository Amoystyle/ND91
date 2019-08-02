#include "stdafx.h"
#ifdef MODULE_IPOD

#include "ItemToContainerDB.h"

using namespace ND91Assistant;


DItemToContainer::DItemToContainer()
{
	item_pid = 0;
	container_pid = 0;
	physical_order = 0;
	shuffle_order = 0;
}
wstring DItemToContainer::GetTableName()
{
	return L"item_to_container";
}
DItemToContainer* DItemToContainer::FromValue(const vector<const char*>* pValues)
{
	DItemToContainer* p = new DItemToContainer();
	int i = 0;	
	p->item_pid = _atoi64(pValues->at(i++));
	p->container_pid = _atoi64(pValues->at(i++));
	p->physical_order = atoi(pValues->at(i++));
	p->shuffle_order = atoi(pValues->at(i++));
	return p;
}

void DItemToContainer::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"item_pid");
	vector.push_back(L"container_pid");
	vector.push_back(L"physical_order");
	vector.push_back(L"shuffle_order");

}
const vector<wstring>* const DItemToContainer::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(item_pid));
	pVector->push_back(Int64ToStr(container_pid));
	pVector->push_back(Int32ToStr(physical_order));
	pVector->push_back(Int32ToStr(shuffle_order));
	return pVector;
}
wstring DItemToContainer::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" item_pid = "<<item_pid<<" and container_pid = "<<container_pid;
	return woss.str();
}


#endif