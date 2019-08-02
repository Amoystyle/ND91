#include "stdafx.h"
#ifdef MODULE_IPOD

#include "AlbumArtistDB.h"

DAlbumArtistDB::DAlbumArtistDB()
{
		Pid = 0;
		Kind = 0;
		artwork_status = 1;
		Artwork_album_pid = 0;
		name_order = 0;
		name_order_section = 0;
		name_blank = 0;
}
wstring DAlbumArtistDB::GetTableName()
{
	if(G_DBver>=310)
		return L"album_artist";		
	else
		return L"artist";
}
DAlbumArtistDB* DAlbumArtistDB::FromValue(const vector<const char*>* pValues)
{
	DAlbumArtistDB* p = new DAlbumArtistDB();
	int i = 0;	
	p->Pid = _atoi64(pValues->at(i++));
	p->Kind = atoi(pValues->at(i++));
	p->artwork_status = atoi(pValues->at(i++));
	p->Artwork_album_pid = _atoi64(pValues->at(i++));
	p->Name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->name_order = atoi(pValues->at(i++));
	p->sort_name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	if(G_DBver>=310)
	{
		p->name_order_section = atoi(pValues->at(i++));
		p->name_blank = atoi(pValues->at(i++));
	}
	return p;
}

void DAlbumArtistDB::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"Pid");
	vector.push_back(L"Kind");
	vector.push_back(L"artwork_status");
	vector.push_back(L"Artwork_album_pid");
	vector.push_back(L"Name");
	vector.push_back(L"name_order");
	vector.push_back(L"sort_name");
	if(G_DBver>=310)
	{
		if(G_DBver >=400)
			vector.push_back(L"name_order_section");
		else
			vector.push_back(L"sort_name_section");	
		vector.push_back(L"name_blank");
	}
}
const vector<wstring>* const DAlbumArtistDB::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(Pid));
	pVector->push_back(Int32ToStr(Kind));
	pVector->push_back(Int32ToStr(artwork_status));
	pVector->push_back(Int64ToStr(Artwork_album_pid));
	pVector->push_back(ConvertStr(Name));
	pVector->push_back(Int32ToStr(name_order));
	pVector->push_back(ConvertStr(sort_name));
	if(G_DBver>=310)
	{
		pVector->push_back(Int32ToStr(name_order_section));
		pVector->push_back(Int32ToStr(name_blank));
	}
	return pVector;
}
wstring DAlbumArtistDB::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" Pid = "<<Pid;
	return woss.str();
}


#endif