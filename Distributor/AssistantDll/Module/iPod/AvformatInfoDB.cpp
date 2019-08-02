#include "stdafx.h"
#ifdef MODULE_IPOD

#include "AvformatInfoDB.h"

using namespace ND91Assistant;


DAvformatInfo::DAvformatInfo()
{
	item_pid = 0;
	sub_id = 0;
	audio_format = 0;	
	bit_rate = 0;	
	sample_rate = 0;
	duration = 0;	
	gapless_heuristic_info = 0;	
	gapless_encoding_delay = 0;	
	gapless_encoding_drain = 0;	
	gapless_last_frame_resynch = 0;	
	analysis_inhibit_flags = 0;	
	audio_fingerprint = 0;	
	volume_normalization_energy = 0;
}
wstring DAvformatInfo::GetTableName()
{
	return L"Ext_Item_View_Membership";
}
DAvformatInfo* DAvformatInfo::FromValue(const vector<const char*>* pValues)
{
	DAvformatInfo* p = new DAvformatInfo();
	int i = 0;	
	p->item_pid = _atoi64(pValues->at(i++));
	p->sub_id = atoi(pValues->at(i++));
	p->audio_format = atoi(pValues->at(i++));
	p->bit_rate = atoi(pValues->at(i++));
	p->sample_rate = atoi(pValues->at(i++));
	p->duration = atoi(pValues->at(i++));
	p->gapless_heuristic_info = atoi(pValues->at(i++));
	p->gapless_encoding_delay = atoi(pValues->at(i++));
	p->gapless_encoding_drain = atoi(pValues->at(i++));
	p->gapless_last_frame_resynch = atoi(pValues->at(i++));
	p->analysis_inhibit_flags = atoi(pValues->at(i++));
	p->audio_fingerprint = atoi(pValues->at(i++));
	p->volume_normalization_energy = atoi(pValues->at(i++));
	return p;
}

void DAvformatInfo::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"item_pid");
	vector.push_back(L"sub_id");
	vector.push_back(L"audio_format");
	vector.push_back(L"bit_rate");
	vector.push_back(L"sample_rate");
	vector.push_back(L"duration");
	vector.push_back(L"gapless_heuristic_info");
	vector.push_back(L"gapless_encoding_delay");
	vector.push_back(L"gapless_encoding_drain");
	vector.push_back(L"gapless_last_frame_resynch");
	vector.push_back(L"analysis_inhibit_flags");
	vector.push_back(L"audio_fingerprint");
	vector.push_back(L"volume_normalization_energy");

}
const vector<wstring>* const DAvformatInfo::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(item_pid));
	pVector->push_back(Int32ToStr(sub_id));
	pVector->push_back(Int32ToStr(audio_format));
	pVector->push_back(Int32ToStr(bit_rate));
	pVector->push_back(Int32ToStr(sample_rate));
	pVector->push_back(Int32ToStr(duration));
	pVector->push_back(Int32ToStr(gapless_heuristic_info));
	pVector->push_back(Int32ToStr(gapless_encoding_delay));
	pVector->push_back(Int32ToStr(gapless_encoding_drain));
	pVector->push_back(Int32ToStr(gapless_last_frame_resynch));
	pVector->push_back(Int32ToStr(analysis_inhibit_flags));
	pVector->push_back(Int32ToStr(audio_fingerprint));
	pVector->push_back(Int32ToStr(volume_normalization_energy));
	return pVector;
}
wstring DAvformatInfo::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" item_pid = "<<item_pid;
		return woss.str();
}


#endif