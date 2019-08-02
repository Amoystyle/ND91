#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Ios5Artist.h"

using namespace ND91Assistant;


DArtist5::DArtist5()
{
	item_artist_pid=0;
	sort_item_artist=L"";
	item_artist=L"";
	grouping_key="";
	cloud_status=0;
	representative_item_pid=0;
}
wstring DArtist5::GetTableName()
{
	return L"item_artist";
}
DArtist5* DArtist5::FromValue(const vector<const char*>* pValues)
{
	DArtist5* p = new DArtist5();
	int i = 0;	
	p->item_artist_pid = _atoi64(pValues->at(i++));
	p->item_artist=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->sort_item_artist=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->grouping_key=pValues->at(i++);
	p->cloud_status=atoi(pValues->at(i++));
	p->representative_item_pid = _atoi64(pValues->at(i++));
	return p;
}

void DArtist5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"item_artist_pid");
	vector.push_back(L"item_artist");
	vector.push_back(L"sort_item_artist");
	vector.push_back(L"grouping_key");
	vector.push_back(L"cloud_status");
	vector.push_back(L"representative_item_pid");
}
const vector<wstring>* const DArtist5::GetValues()
{
	vector<wstring>* pVector = GetVector();	
	pVector->push_back(Int64ToStr(item_artist_pid));
	pVector->push_back(ConvertStr(item_artist));
	pVector->push_back(ConvertStr(sort_item_artist));
	pVector->push_back(BinToStr(grouping_key));
	pVector->push_back(Int32ToStr(cloud_status));
	pVector->push_back(Int64ToStr(representative_item_pid));
	return pVector;
}
wstring DArtist5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" item_artist_pid = "<<item_artist_pid;
	return woss.str();
}


#endif