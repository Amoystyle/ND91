#include "stdafx.h"
#include "ExtItemViewMembershipDB.h"

using namespace ND91Assistant;


DExtViewMembership::DExtViewMembership()
{
	item_pid = 0;
	movie_mbr = 0;
	movie_rental_mbr = 0;
}
wstring DExtViewMembership::GetTableName()
{
	return L"Ext_Item_View_Membership";
}
DExtViewMembership* DExtViewMembership::FromValue(const vector<const char*>* pValues)
{
	DExtViewMembership* p = new DExtViewMembership();
	int i = 0;	
	p->item_pid = _atoi64(pValues->at(i++));
	p->movie_mbr = atoi(pValues->at(i++));
	p->movie_rental_mbr = atoi(pValues->at(i++));
	return p;
}

void DExtViewMembership::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"item_pid");
	vector.push_back(L"movie_mbr");
	vector.push_back(L"movie_rental_mbr");

}
const vector<wstring>* const DExtViewMembership::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(item_pid));
	pVector->push_back(Int32ToStr(movie_mbr));
	pVector->push_back(Int32ToStr(movie_rental_mbr));
	return pVector;
}
wstring DExtViewMembership::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" item_pid = "<<item_pid;
	return woss.str();
}