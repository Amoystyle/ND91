#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Ios5Container.h"


DContainer5::DContainer5()
{
	container_pid =0;
	distinguished_kind =0;
	date_created =0;
	date_modified =0;
	name=L"";
	name_order =0;
	parent_pid =0;
	media_kinds =0;
	workout_template_id =0;
	is_hidden =0;
	description =0;
	smart_is_folder =0;
	smart_is_dynamic =0;
	smart_is_filtered =0;
	smart_is_genius =0;
	smart_enabled_only =0;
	smart_is_limited =0;
	smart_limit_kind =0;
	smart_limit_order =0;
	smart_limit_value =0;
	smart_reverse_limit_order =0;
	smart_criteria="";
	play_order =0;
	is_reversed =0;
	album_field_order =0;
	repeat_mode =0;
	shuffle_items =0;
	has_been_shuffled =0;
	filepath =L"";
	is_saveable =0;
	is_src_remote =0;
	is_ignored_syncing =0;
	container_type =0;
	is_container_type_active_target =0;
	orig_date_modified =0;
	store_cloud_id =0;
}
wstring DContainer5::GetTableName()
{
	return L"container";
}
DContainer5* DContainer5::FromValue(const vector<const char*>* pValues)
{
	DContainer5* p = new DContainer5();
	int i = 0;		
	p->container_pid =_atoi64(pValues->at(i++));
	p->distinguished_kind =atoi(pValues->at(i++));
	p->date_created =atoi(pValues->at(i++));
	p->date_modified =atoi(pValues->at(i++));
	p->name=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->name_order =_atoi64(pValues->at(i++));
	p->parent_pid =atoi(pValues->at(i++));
	p->media_kinds =atoi(pValues->at(i++));
	p->workout_template_id =atoi(pValues->at(i++));
	p->is_hidden =atoi(pValues->at(i++));
	p->description =atoi(pValues->at(i++));
	p->smart_is_folder =atoi(pValues->at(i++));
	p->smart_is_dynamic =atoi(pValues->at(i++));
	p->smart_is_filtered =atoi(pValues->at(i++));
	p->smart_is_genius =atoi(pValues->at(i++));
	p->smart_enabled_only =atoi(pValues->at(i++));
	p->smart_is_limited =atoi(pValues->at(i++));
	p->smart_limit_kind =atoi(pValues->at(i++));
	p->smart_limit_order =atoi(pValues->at(i++));
	p->smart_limit_value =atoi(pValues->at(i++));
	p->smart_reverse_limit_order =atoi(pValues->at(i++));
	p->smart_criteria=pValues->at(i++);
	p->play_order =atoi(pValues->at(i++));
	p->is_reversed =atoi(pValues->at(i++));
	p->album_field_order =atoi(pValues->at(i++));
	p->repeat_mode =atoi(pValues->at(i++));
	p->shuffle_items =atoi(pValues->at(i++));
	p->has_been_shuffled =atoi(pValues->at(i++));
	p->filepath =CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->is_saveable =atoi(pValues->at(i++));
	p->is_src_remote =atoi(pValues->at(i++));
	p->is_ignored_syncing =atoi(pValues->at(i++));
	p->container_type =atoi(pValues->at(i++));
	p->is_container_type_active_target =atoi(pValues->at(i++));
	p->orig_date_modified =atoi(pValues->at(i++));
	p->store_cloud_id =atoi(pValues->at(i++));

	return p;
}

void DContainer5::GetColumns(vector<wstring>& vector)
{

	vector.push_back(L"container_pid");
	vector.push_back(L"distinguished_kind");
	vector.push_back(L"date_created");
	vector.push_back(L"date_modified");
	vector.push_back(L"name");
	vector.push_back(L"name_order");
	vector.push_back(L"parent_pid");
	vector.push_back(L"media_kinds");
	vector.push_back(L"workout_template_id");
	vector.push_back(L"is_hidden");
	vector.push_back(L"description");
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
	vector.push_back(L"play_order");
	vector.push_back(L"is_reversed");
	vector.push_back(L"album_field_order");
	vector.push_back(L"repeat_mode");
	vector.push_back(L"shuffle_items");
	vector.push_back(L"has_been_shuffled");
	vector.push_back(L"filepath");
	vector.push_back(L"is_saveable");
	vector.push_back(L"is_src_remote");
	vector.push_back(L"is_ignored_syncing");
	vector.push_back(L"container_type");
	vector.push_back(L"is_container_type_active_target");
	vector.push_back(L"orig_date_modified");
	vector.push_back(L"store_cloud_id");

}
const vector<wstring>* const DContainer5::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(container_pid));
	pVector->push_back(Int32ToStr(distinguished_kind));
	pVector->push_back(Int32ToStr(date_created));
	pVector->push_back(Int32ToStr(date_modified));
	pVector->push_back(ConvertStr(name));
	pVector->push_back(Int64ToStr(name_order));
	pVector->push_back(Int32ToStr(parent_pid));
	pVector->push_back(Int32ToStr(media_kinds));
	pVector->push_back(Int32ToStr(workout_template_id));
	pVector->push_back(Int32ToStr(is_hidden));
	pVector->push_back(Int32ToStr(description));
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
	pVector->push_back(BinToStr(smart_criteria));
	pVector->push_back(Int32ToStr(play_order));
	pVector->push_back(Int32ToStr(is_reversed));
	pVector->push_back(Int32ToStr(album_field_order));
	pVector->push_back(Int32ToStr(repeat_mode));
	pVector->push_back(Int32ToStr(shuffle_items));
	pVector->push_back(Int32ToStr(has_been_shuffled));
	pVector->push_back(ConvertStr(filepath));
	pVector->push_back(Int32ToStr(is_saveable));
	pVector->push_back(Int32ToStr(is_src_remote));
	pVector->push_back(Int32ToStr(is_ignored_syncing));
	pVector->push_back(Int32ToStr(container_type));
	pVector->push_back(Int32ToStr(is_container_type_active_target));
	pVector->push_back(Int32ToStr(orig_date_modified));
	pVector->push_back(Int32ToStr(store_cloud_id));
	return pVector;
}
wstring DContainer5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" container_pid = "<<container_pid;
	return woss.str();
}


#endif