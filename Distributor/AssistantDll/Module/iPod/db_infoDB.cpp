#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Db_InfoDB.h"

using namespace ND91Assistant;


DDbInfo::DDbInfo()
{
	db_pid = 0;
	primary_container_pid = 0;
	media_folder_url = L"";
	audio_language = 0;
	subtitle_language = 0;
	genius_cuid = 0;
	bib = "";
	rib = "";
}
wstring DDbInfo::GetTableName()
{
	return L"db_info";
}
DDbInfo* DDbInfo::FromValue(const vector<const char*>* pValues)
{
	DDbInfo* p = new DDbInfo();
	int i = 0;	
	p->db_pid = _atoi64(pValues->at(i++));
	p->primary_container_pid = _atoi64(pValues->at(i++));
	p->media_folder_url = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->audio_language = atoi(pValues->at(i++));
	p->subtitle_language = atoi(pValues->at(i++));
	p->genius_cuid = atoi(pValues->at(i++));
	p->bib = pValues->at(i++);
	p->rib =pValues->at(i++);
	return p;
}

void DDbInfo::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"db_pid");
	vector.push_back(L"primary_container_pid");
	vector.push_back(L"media_folder_url");
	vector.push_back(L"audio_language");
	vector.push_back(L"subtitle_language");
	vector.push_back(L"genius_cuid");
	vector.push_back(L"bib");
	vector.push_back(L"rib");

}
const vector<wstring>* const DDbInfo::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(db_pid));
	pVector->push_back(Int64ToStr(primary_container_pid));
	pVector->push_back(ConvertStr(media_folder_url));
	pVector->push_back(Int32ToStr(audio_language));
	pVector->push_back(Int32ToStr(subtitle_language));
	pVector->push_back(Int32ToStr(genius_cuid));
	pVector->push_back(BinToStr(bib));
	pVector->push_back(BinToStr(rib));
	return pVector;
}
wstring DDbInfo::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" db_pid = "<<db_pid;
	return woss.str();
}


#endif