#include "stdafx.h"
#ifdef MODULE_IPOD

#include "containerDB.h"

using namespace ND91Assistant;

DContainerDB::DContainerDB()
{

	Pid = 0;
	distinguished_kind = 0;
	date_created = 0;
	date_modified = 0;
	name = L"";
	name_order = 0;
	parent_pid = 0;
	media_kinds = 0;
	workout_template_id = 0;
	is_hidden = 0;
	smart_is_folder = 0;
	smart_is_dynamic = 0;
	smart_is_filtered = 0;
	smart_is_genius = 0;
	smart_enabled_only = 0;
	smart_is_limited = 0;
	smart_limit_kind = 0;
	smart_limit_order = 0;
	smart_limit_value = 0;
	smart_reverse_limit_order = 0;
	smart_criteria = L"";
	Description = L"";
}
wstring DContainerDB::GetTableName()
{
	return L"container";
}
DContainerDB* DContainerDB::FromValue(const vector<const char*>* pValues)
{
	DContainerDB* p = new DContainerDB();
	int i = 0;	
	p->Pid = _atoi64(pValues->at(i++));
	p->distinguished_kind = atoi(pValues->at(i++));
	p->date_created = atoi(pValues->at(i++));
	p->date_modified = atoi(pValues->at(i++));
	p->name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->name_order = atoi(pValues->at(i++));
	p->parent_pid = atoi(pValues->at(i++));
	p->media_kinds = atoi(pValues->at(i++));
	p->workout_template_id = atoi(pValues->at(i++));
	p->is_hidden = atoi(pValues->at(i++));
	p->smart_is_folder = atoi(pValues->at(i++));
	p->smart_is_dynamic = atoi(pValues->at(i++));
	p->smart_is_filtered = atoi(pValues->at(i++));
	p->smart_is_genius = atoi(pValues->at(i++));
	p->smart_enabled_only = atoi(pValues->at(i++));
	p->smart_is_limited = atoi(pValues->at(i++));
	p->smart_limit_kind = atoi(pValues->at(i++));
	p->smart_limit_order = atoi(pValues->at(i++));
	p->smart_limit_value = atoi(pValues->at(i++));
	p->smart_reverse_limit_order = atoi(pValues->at(i++));
	p->smart_criteria = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->Description = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	return p;
}
void DContainerDB::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"Pid");
	vector.push_back(L"distinguished_kind");
	vector.push_back(L"date_created");
	vector.push_back(L"date_modified");
	vector.push_back(L"name");
	vector.push_back(L"name_order");
	vector.push_back(L"parent_pid");
	vector.push_back(L"media_kinds");
	vector.push_back(L"workout_template_id");
	vector.push_back(L"is_hidden");
	vector.push_back(L"smart_is_folder");
	vector.push_back(L"smart_is_dynamic");
	vector.push_back(L"smart_is_filtered");
	vector.push_back(L"smart_is_genius");
	vector.push_back(L"smart_enabled_only");
	vector.push_back(L"smart_is_limited");
	vector.push_back(L"smart_limit_kind");
	vector.push_back(L"smart_limit_order");
	vector.push_back(L"smart_limit_value");
	vector.push_back(L"smart_reverse_limit_order");
	vector.push_back(L"smart_criteria");
	vector.push_back(L"Description");

}

const vector<wstring>* const DContainerDB::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(Pid));
	pVector->push_back(Int32ToStr(distinguished_kind));
	pVector->push_back(Int32ToStr(date_created));
	pVector->push_back(Int32ToStr(date_modified));
	pVector->push_back(ConvertStr(name));
	pVector->push_back(Int32ToStr(name_order));
	pVector->push_back(Int32ToStr(parent_pid));
	pVector->push_back(Int32ToStr(media_kinds));
	pVector->push_back(Int32ToStr(workout_template_id));
	pVector->push_back(Int32ToStr(is_hidden));
	pVector->push_back(Int32ToStr(smart_is_folder));
	pVector->push_back(Int32ToStr(smart_is_dynamic));
	pVector->push_back(Int32ToStr(smart_is_filtered));
	pVector->push_back(Int32ToStr(smart_is_genius));
	pVector->push_back(Int32ToStr(smart_enabled_only));
	pVector->push_back(Int32ToStr(smart_is_limited));
	pVector->push_back(Int32ToStr(smart_limit_kind));
	pVector->push_back(Int32ToStr(smart_limit_order));
	pVector->push_back(Int32ToStr(smart_limit_value));
	pVector->push_back(Int32ToStr(smart_reverse_limit_order));
	pVector->push_back(ConvertStr(smart_criteria));
	pVector->push_back(ConvertStr(Description));

return pVector;
}
wstring DContainerDB::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" Pid = "<<Pid;
	return woss.str();
}



#endif