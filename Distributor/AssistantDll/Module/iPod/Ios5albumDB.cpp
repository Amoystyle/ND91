#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Ios5albumDB.h"


DAlbum5::DAlbum5()
{
		album_pid =0;
		album = L"";
		sort_album = L"";
		album_artist_pid =0;
		representative_item_pid =0;
		grouping_key ="";
		cloud_status=0;
		artwork_cache_id=0;
		user_rating=0;
		all_compilations =0;
		feed_url =L"";
		season_number=0;
 
}
wstring DAlbum5::GetTableName()
{
	return L"album";
}
DAlbum5* DAlbum5::FromValue(const vector<const char*>* pValues)
{
	DAlbum5* p = new DAlbum5();
	int i = 0;	
	p->album_pid = _atoi64(pValues->at(i++));
	p->album=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->sort_album=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->album_artist_pid = _atoi64(pValues->at(i++));
	p->representative_item_pid = _atoi64(pValues->at(i++));
	p->grouping_key=pValues->at(i++);
	p->cloud_status=atoi(pValues->at(i++));
	p->artwork_cache_id=_atoi64(pValues->at(i++));
	p->user_rating=atoi(pValues->at(i++));
	p->all_compilations=atoi(pValues->at(i++));
	p->feed_url=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->season_number=atoi(pValues->at(i++));
	return p;
}

void DAlbum5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"album_pid");
	vector.push_back(L"album");
	vector.push_back(L"sort_album");
	vector.push_back(L"album_artist_pid");
	vector.push_back(L"representative_item_pid");
	vector.push_back(L"grouping_key");
	vector.push_back(L"cloud_status");
	vector.push_back(L"artwork_cache_id");
	vector.push_back(L"user_rating");
	vector.push_back(L"all_compilations");
	vector.push_back(L"feed_url");
	vector.push_back(L"season_number");
}
const vector<wstring>* const DAlbum5::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(album_pid));
	pVector->push_back(ConvertStr(album));
	pVector->push_back(ConvertStr(sort_album));
	pVector->push_back(Int64ToStr(album_artist_pid));
	pVector->push_back(Int64ToStr(representative_item_pid));
	pVector->push_back(BinToStr(grouping_key));
	pVector->push_back(Int32ToStr(cloud_status));
	pVector->push_back(Int64ToStr(artwork_cache_id));
	pVector->push_back(Int32ToStr(user_rating));
	pVector->push_back(Int32ToStr(all_compilations));
	pVector->push_back(ConvertStr(feed_url));
	pVector->push_back(Int32ToStr(season_number));

	return pVector;
}
wstring DAlbum5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" album_pid = "<<album_pid;
	return woss.str();
}


#endif