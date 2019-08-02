#include "stdafx.h"
#ifdef MODULE_IPOD

#include "AlbumDB.h"

extern int G_DBver;
DAlbumDB::DAlbumDB()
{
		Pid = 0;
		Kind = 0;
		artwork_status = 1;
		artwork_item_pid = 0;
		artist_pid = 0;
		user_rating = 0;
		name_order = 0;
		all_compilations = 0;
		season_number = 0;
		name_order_section = 0;
		artist_order_section = 0;
		name_blank = 0;
}
wstring DAlbumDB::GetTableName()
{
	return L"album";
}
DAlbumDB* DAlbumDB::FromValue(const vector<const char*>* pValues)
{
	DAlbumDB* p = new DAlbumDB();
	int i = 0;	
	p->Pid = _atoi64(pValues->at(i++));
	p->Kind = atoi(pValues->at(i++));
	p->artwork_status = atoi(pValues->at(i++));
	p->artwork_item_pid = _atoi64(pValues->at(i++));
	p->artist_pid = _atoi64(pValues->at(i++));
	p->user_rating = atoi(pValues->at(i++));
	p->Name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->name_order = atoi(pValues->at(i++));
	p->all_compilations = atoi(pValues->at(i++));
	p->feed_url = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->season_number = atoi(pValues->at(i++));
	if(G_DBver>=310)
	{
		p->name_order_section = atoi(pValues->at(i++));
		p->sort_name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
		p->Artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
		p->sort_artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
		p->artist_order_section = atoi(pValues->at(i++));
		p->name_blank = atoi(pValues->at(i++));
	}
	return p;
}

void DAlbumDB::GetColumns(vector<wstring>& vector)
{
	 vector.push_back(L"Pid");
	 vector.push_back(L"Kind");
	 vector.push_back(L"artwork_status");
	 vector.push_back(L"artwork_item_pid");
	 vector.push_back(L"artist_pid");
	 vector.push_back(L"user_rating");
	 vector.push_back(L"Name");
	 vector.push_back(L"name_order");
	 vector.push_back(L"all_compilations");
	 vector.push_back(L"feed_url");
	 vector.push_back(L"season_number");
	 if(G_DBver>=310)
	 {
		 if(G_DBver >=400)
			 vector.push_back(L"name_order_section");
		 else
			vector.push_back(L"sort_name_section");
		 vector.push_back(L"sort_name");
		 vector.push_back(L"Artist");
		 vector.push_back(L"sort_artist");
		 if(G_DBver >=400)
			vector.push_back(L"artist_order_section");
		 else
			 vector.push_back(L"sort_artist_section");
		 vector.push_back(L"name_blank");
	 }
}
const vector<wstring>* const DAlbumDB::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(Pid));
	pVector->push_back(Int32ToStr(	Kind ));
	pVector->push_back(Int32ToStr(	artwork_status ));
	pVector->push_back(Int64ToStr(	artwork_item_pid ));
	pVector->push_back(Int64ToStr(	artist_pid ));
	pVector->push_back(Int32ToStr(	user_rating ));
	pVector->push_back(ConvertStr(	Name ));
	pVector->push_back(Int32ToStr(	name_order ));
	pVector->push_back(Int32ToStr(	all_compilations ));
	pVector->push_back(ConvertStr(	feed_url ));
	pVector->push_back(Int32ToStr(	season_number ));
	if(G_DBver>=310)
	{
		pVector->push_back(Int32ToStr(	name_order_section ));
		pVector->push_back(ConvertStr(	sort_name ));
		pVector->push_back(ConvertStr(	Artist ));
		pVector->push_back(ConvertStr( sort_artist ));
		pVector->push_back(Int32ToStr(	artist_order_section ));
		pVector->push_back(Int32ToStr(	name_blank ));
	}
	return pVector;
}
wstring DAlbumDB::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" Pid = "<<Pid;
	return woss.str();
}


#endif