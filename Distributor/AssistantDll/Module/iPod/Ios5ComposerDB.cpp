#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Ios5ComposerDB.h"


DComposer5::DComposer5()
{
	composer_pid=0;
	sort_composer=L"";
	composer=L"";
	grouping_key="";
	cloud_status=0;
	representative_item_pid=0;

}
wstring DComposer5::GetTableName()
{
	return L"composer";
}
DComposer5* DComposer5::FromValue(const vector<const char*>* pValues)
{
	DComposer5* p = new DComposer5();
	int i = 0;	
	p->composer_pid = _atoi64(pValues->at(i++));
	p->composer=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->sort_composer=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->grouping_key=pValues->at(i++);
	p->cloud_status=atoi(pValues->at(i++));
	p->representative_item_pid = _atoi64(pValues->at(i++));
	return p;
}

void DComposer5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"composer_pid");
	vector.push_back(L"composer");
	vector.push_back(L"sort_composer");
	vector.push_back(L"grouping_key");
	vector.push_back(L"cloud_status");
	vector.push_back(L"representative_item_pid");
}
const vector<wstring>* const DComposer5::GetValues()
{
	vector<wstring>* pVector = GetVector();	
	pVector->push_back(Int64ToStr(composer_pid));
	pVector->push_back(ConvertStr(composer));
	pVector->push_back(ConvertStr(sort_composer));
	pVector->push_back(BinToStr(grouping_key));
	pVector->push_back(Int32ToStr(cloud_status));
	pVector->push_back(Int64ToStr(representative_item_pid));
	return pVector;
}
wstring DComposer5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" composer_pid = "<<composer_pid;
	return woss.str();
}



#endif