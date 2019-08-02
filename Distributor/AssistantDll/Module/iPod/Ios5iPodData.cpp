#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Module/iPod/Ios5iPodData.h"
#include "Ios5Container.h"

#include "common/sqlaccess/SqliteAccess.h"
#include "common/sqlaccess/BaseTable.h"

template<class T>
void DeleteVector(vector<T*> p)
{
	for(size_t i = 0; i < p.size(); ++i)
		SAFE_DELETE(p[i]);
	p.clear();
}

cIPodData5::cIPodData5(wstring DBFilePath):_DBFilePath(DBFilePath)
{
	LoadData();
}
cIPodData5::~cIPodData5()
{
	DeleteVector(_alltracklist);
	DeleteVector(_playlist);
}

void cIPodData5::LoadData()
{
	string sqlstr = "select \
		item.item_pid,item.item_artist_pid,item.album_pid, \
		item.album_artist_pid,item.composer_pid,item_extra.title, \
		album.album,album_artist.album_artist, \
		composer.composer,base_location.path, \
		item_artist.item_artist,item_extra.location from item \
		inner join item_extra on item.item_pid =item_extra.item_pid \
		inner join album_artist on item.album_artist_pid = album_artist.album_artist_pid \
		inner join album on item.album_pid=album.album_pid \
		inner join composer on item.composer_pid= composer.composer_pid \
		inner join base_location on item.base_location_id =base_location.base_location_id \
		inner join item_artist on item.item_artist_pid =  item_artist.item_artist_pid";

	CppSQLite3DB SQLdb;
	SQLdb.Open(CCodeOperation::UnicodeToUTF_8(_DBFilePath).c_str());

	CppSQLite3Query query = SQLdb.ExecQuery( sqlstr.c_str() );

	DeleteVector(_alltracklist);
	while(!query.Eof())
	{
		CIosCDBTrackData * pTrackData = new CIosCDBTrackData();
		pTrackData->Dbid(_atoi64(query.GetStringField(0)));
		pTrackData->ArtistDBId(_atoi64(query.GetStringField(1)));
		pTrackData->AlbumDBId(_atoi64(query.GetStringField(2)));
		pTrackData->AlbumArtistDBId(_atoi64(query.GetStringField(3)));
		pTrackData->ComposerDBId(_atoi64(query.GetStringField(4)));
		pTrackData->Title(CCodeOperation::UTF_8ToUnicode(query.GetStringField(5)));
		pTrackData->AlbumStr(CCodeOperation::UTF_8ToUnicode(query.GetStringField(6)));
		pTrackData->AlbumArtistStr(CCodeOperation::UTF_8ToUnicode(query.GetStringField(7)));
		pTrackData->ComposerStr(CCodeOperation::UTF_8ToUnicode(query.GetStringField(8)));
		wstring devpath = CCodeOperation::UTF_8ToUnicode(query.GetStringField(9));
		if(!CStrOperation::endsWithW(devpath,L"mp3"))
		{
			devpath+= L":";
			devpath+= CCodeOperation::UTF_8ToUnicode(query.GetStringField(11));
		}
		pTrackData->Devicepath(devpath);
		pTrackData->ArtistStr(CCodeOperation::UTF_8ToUnicode(query.GetStringField(10)));
		_alltracklist.push_back(pTrackData);
		query.NextRow();
	}
	

	

	SQLdb.Close();

	DeleteVector(_playlist);
	GetAllPlayList5(&_playlist);
	
}

void cIPodData5::GetAllPlayList5(vector<CIosCDBPlayList*>* pVecPlayList)
{
	SqliteAccess sql;
	sql.Open(_DBFilePath);
	pVecPlayList->clear();
	BaseTable<DContainer5> Container(&sql);	
	Container.SelectAll();
	for(int i=0;i<Container.GetRowCount();i++)
	{
		DContainer5* pContainer = Container.GetDataRow(i);
		CIosCDBPlayList* item = new CIosCDBPlayList();
		item->ID(pContainer->container_pid);
		item->Listname(pContainer->name);
		item->Plistpath(pContainer->filepath);
		pVecPlayList->push_back(item);
	}
	Container.Clear();

}



#endif