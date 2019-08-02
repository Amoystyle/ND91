#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Ios5AlbumArtistDB.h"

using namespace ND91Assistant;


DAlbumArtist5::DAlbumArtist5()
{
	album_artist_pid=0;
	album_artist=L"";
	sort_album_artist=L"";
	grouping_key="";
	cloud_status=0;
	artwork_cache_id=0;
	representative_item_pid=0;

}
wstring DAlbumArtist5::GetTableName()
{
	return L"album_artist";
}
DAlbumArtist5* DAlbumArtist5::FromValue(const vector<const char*>* pValues)
{
	DAlbumArtist5* p = new DAlbumArtist5();
	int i = 0;	
	p->album_artist_pid = _atoi64(pValues->at(i++));
	p->album_artist=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->sort_album_artist=CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->grouping_key=pValues->at(i++);
	p->cloud_status=atoi(pValues->at(i++));
	p->artwork_cache_id = _atoi64(pValues->at(i++));
	p->representative_item_pid = _atoi64(pValues->at(i++));
	return p;
}

void DAlbumArtist5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"album_artist_pid");
	vector.push_back(L"album_artist");
	vector.push_back(L"sort_album_artist");
	vector.push_back(L"grouping_key");
	vector.push_back(L"cloud_status");
	vector.push_back(L"artwork_cache_id");
	vector.push_back(L"representative_item_pid");
}
const vector<wstring>* const DAlbumArtist5::GetValues()
{
	vector<wstring>* pVector = GetVector();	
	pVector->push_back(Int64ToStr(album_artist_pid));
	pVector->push_back(ConvertStr(album_artist));
	pVector->push_back(ConvertStr(sort_album_artist));
	pVector->push_back(BinToStr(grouping_key));
	pVector->push_back(Int32ToStr(cloud_status));
	pVector->push_back(Int64ToStr(artwork_cache_id));
	pVector->push_back(Int64ToStr(representative_item_pid));
	return pVector;
}
wstring DAlbumArtist5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" album_artist_pid = "<<album_artist_pid;
	return woss.str();
}



#endif