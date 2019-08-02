#include "stdafx.h"

#ifdef MODULE_IPOD

#include <atlconv.h>

#include "Module/iPod/iPodDbBase.h"
#include "containerDb.h"
#include "ItemToContainerDB.h"
#include "AvformatInfoDB.h"
#include "ExtItemViewMembershipDB.h"
#include "AlbumArtistDB.h"
#include "AlbumDB.h"
#include "ArtistDB.h"
#include "ComposerDB.h"

#include "Common/SqlAccess/SqliteAccess.h"
#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Module/iPod//Location.h"
#include "Module/iPod//Item.h"
#include "Module/Common/Media/iPodMedia/IiPodMediaParser.h"
#include "Module/Common/Media/Utils/iTunesMP3Lib_Type.h"


#pragma warning(disable : 4309)
extern int G_DBver;
using namespace ND91Assistant;
iPodDbBase::iPodDbBase(wstring sqlDbPath)
:_sqlDbPath(sqlDbPath)
{

}
iPodDbBase::~iPodDbBase()
{

}


wstring iPodDbBase::UpdateSqlDbTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID,wstring ArtWorkFilePath,wstring ArtWorkDBPath)
{
	wstring DeviceFileName = L"";
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
		return DeviceFileName;

	CreateiPhoneKeyFunction(sql.GetDB());

	ptrackData->AlbumDBId(find_Album_PID(ptrackData->Id()));
	ptrackData->ComposerDBId(find_composer_PID(&sql,ptrackData->Dbid()));
	ptrackData->ArtistDBId(find_artist_PID(ptrackData->Id()));
	//ר��������ID��	
	pDBID->AlbumArtistID = UpdateAlbumArtistDB(&sql,ptrackData->Dbid(),pDBID->AlbumArtist);
	pDBID->ComposerID = UpdateComposerDB(&sql,ptrackData);
	pDBID->ArtistID = UpdateArtistDB(&sql,ptrackData);
	pDBID->AlbumID = UpdateAlbumDB(&sql,ptrackData,pDBID);
	//���¸�����Ϣ
	BaseTable<DItem> tItem(&sql);
	tItem.Select(ptrackData->Dbid(),L"pid");
	if(tItem.GetRowCount() > 0)
	{
		DItem* pitem = tItem.GetDataRow(0);
		pDBID->TrackID = pitem->pid;
		pDBID->ArtWorkID = pitem->artwork_cache_id;

		//�޸ķ���.
		if(ArtWorkFilePath.length()>0)
		{	
			Bitmap* bmpArtWork = new Bitmap(ArtWorkFilePath.c_str());
			std::vector<int> thumbTypes;
			thumbTypes.push_back(3001);
			thumbTypes.push_back(3002);
			thumbTypes.push_back(3005);
			thumbTypes.push_back(3006);
			thumbTypes.push_back(3007);
			thumbTypes.push_back(3101);
			thumbTypes.push_back(3102);
			thumbTypes.push_back(3105);
			thumbTypes.push_back(3106);
			thumbTypes.push_back(3107);
			IosPhotoDB pdb(ArtWorkDBPath + L"\\Artwork\\ArtworkDB", thumbTypes,ArtWorkDBPath + L"\\Artwork\\");
			if(pDBID->ArtWorkID == 0)
			{
				pDBID->ArtWorkID = pdb.AddImage(bmpArtWork);
				pitem->artwork_cache_id = pDBID->ArtWorkID;
			}
			else
				pdb.UpdateImage(pDBID->ArtWorkID, bmpArtWork);
			pdb.Write(ArtWorkDBPath + L"\\Artwork\\ArtworkDBNew");
		}

		pitem->Title = ptrackData->Title();
		pitem->Artist = ptrackData->ArtistStr();
		pitem->artist_pid = pDBID->ArtistID;
		pitem->Album = ptrackData->AlbumStr();
		pitem->album_pid = pDBID->AlbumID;
		pitem->album_artist = pDBID->AlbumArtist;
		pitem->album_artist_pid = pDBID->AlbumArtistID;
		pitem->Composer = ptrackData->ComposerStr();
		pitem->composer_pid = pDBID->ComposerID;
		pitem->album_artist_blank =  pDBID->AlbumArtist.length()>0?1:0;
		pitem->album_blank = ptrackData->AlbumStr().length()>0?1:0;
		pitem->artist_blank = ptrackData->ArtistStr().length()>0?1:0;
		pitem->composer_blank = ptrackData->ComposerStr().length()>0?1:0;
		pitem->sort_album = ptrackData->AlbumStr();
		pitem->sort_album_artist = pDBID->AlbumArtist;
		pitem->sort_composer = ptrackData->ComposerStr();
		pitem->sort_artist = ptrackData->ArtistStr();
		pitem->sort_title = ptrackData->Title();
		pitem->comment	= ptrackData->CommentStr();
		if(pDBID->ArtWorkID >0)
			pitem->artwork_status =1;
		time_t now;
		time(&now);
		pitem->date_modified = now;
		//pitem->artwork_cache_id

		//pitem->genre_id
		//pitem->relative_volume		
		//pitem->total_time_ms
		tItem.Update(pitem);
		tItem.Clear();
	}
	else
	{
		return DeviceFileName;
	}	
	sql.Close();

	


	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return DeviceFileName;	
	BaseTable<DLocation> location(&sql);	
	//������豸�ϵ��ļ�·��
	location.Select(ptrackData->Dbid(),L"item_pid");	
	if(location.GetRowCount() == 1)
	{
		DLocation* pLocationSecect = location.GetDataRow(0);
		DeviceFileName = pLocationSecect->location;
	}
	location.Clear();	
	sql.Close();
	return DeviceFileName;
}
__int64 iPodDbBase::UpdateAlbumArtistDB(SqliteAccess* pSql,__int64 oldAlbumid,wstring AlbumArtistName)
{
	__int64 AlbumArtistId=0;
	__int64 oldAlbumArtistID = find_Album_artist_PID(pSql,oldAlbumid);
	if(AlbumArtistName.length() > 0 )
	{
		BaseTable<DAlbumArtistDB> TDAlbumArtist(pSql);	
		//���ݱ�����IDȡ�þɵı�������
		TDAlbumArtist.Select(oldAlbumArtistID,L"pid");
		//�ɵı�����
		if(TDAlbumArtist.GetRowCount()>0)
		{
			DAlbumArtistDB* pTabAlbumArtist = TDAlbumArtist.GetDataRow(0);
			wstring OldAlbumArtistName = pTabAlbumArtist->Name;			
			TDAlbumArtist.Clear();
			//�Ƿ�����������޸�
			if(OldAlbumArtistName != AlbumArtistName)
			{	//���±�����
				AlbumArtistId = GetAlbumArtistIdByName(pSql,AlbumArtistName);
				//����ǰ��ʹ�ù��ľɱ��������ݿ��¼
				BaseTable<DAlbumDB> tAlbum(pSql);
				if(tAlbum.Count(oldAlbumArtistID,L"artist_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
				{					
					TDAlbumArtist.Delete(oldAlbumArtistID,L"pid");
				}
			}
			else//���ȷ��û���޸ķ��ؾ�ID
				AlbumArtistId = oldAlbumArtistID;
		}
		else//û���ҵ��ɱ�����?
		{	//���±�����
			AlbumArtistId = GetAlbumArtistIdByName(pSql,AlbumArtistName);
		}
	}
	else
	{	
		//û�б�����,������ǰ�ľɱ��������ݿ��¼
		if(oldAlbumArtistID !=0 )
		{		
			BaseTable<DAlbumDB> tAlbum(pSql);
			if(tAlbum.Count(oldAlbumArtistID,L"artist_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
			{		
				BaseTable<DAlbumArtistDB> TDAlbumArtist(pSql);	
				TDAlbumArtist.Delete(oldAlbumArtistID,L"pid");
			}
		}
	}
	return AlbumArtistId;

}
__int64 iPodDbBase::UpdateAlbumDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID)
{
	__int64 AlbumID=0;
	//�Ƿ���ר��
	if(ptrackData->AlbumStr().length()>0)
	{	
		BaseTable<DAlbumDB> TDAlbum(pSql);	
		//����ר��IDȡ�þɵ�ר����
		TDAlbum.Select(ptrackData->AlbumDBId(),L"pid");
		//�ɵ�ר����
		if(TDAlbum.GetRowCount()>0)
		{
			DAlbumDB* pTabAlbum = TDAlbum.GetDataRow(0);
			wstring OldAlbumName = pTabAlbum->Name;
			__int64 OldAlbumID = ptrackData->AlbumDBId();
			TDAlbum.Clear();
			//�Ƿ�ר�������޸�
			if(OldAlbumName != ptrackData->AlbumStr())
			{	//����ר��
				AlbumID = GetAlbumIDByName(pSql,ptrackData->AlbumStr(),pDBID);
				//����ǰ��ʹ�ù��ľ�ר�����ݿ��¼
				BaseTable<DItem> tItem(pSql);
				if(tItem.Count(OldAlbumID,L"album_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
				{					
					//	TDAlbum.Delete(OldArtistID,L"pid");
					DeleteAlbumArtistDbRow(OldAlbumID,pSql);
				}
			}
			else//���ȷ��û���޸ķ��ؾ�ID
				AlbumID = OldAlbumID;
		}
		else//û���ҵ��ɱ�����?
		{	//���±�����
			AlbumID = GetAlbumIDByName(pSql,ptrackData->AlbumStr(),pDBID);
		}
	}
	else
	{	
		//û�б�����,������ǰ�ľɱ��������ݿ��¼
		if(ptrackData->AlbumDBId() !=0 )
		{		
			BaseTable<DItem> tItem(pSql);
			if(tItem.Count(ptrackData->AlbumDBId(),L"album_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
			{					
				//TDAlbum.Delete(ptrackData->Albumid(),L"pid");
				DeleteAlbumArtistDbRow(ptrackData->AlbumDBId(),pSql);
			}
		}
	}
	return AlbumID;
}
__int64 iPodDbBase::UpdateArtistDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData)
{
	__int64 ArtistID=0;
	//�Ƿ��б�����
	if(ptrackData->ArtistStr().length()>0)
	{	
		BaseTable<DArtistDB> TDArtist(pSql);	
		//���ݱ�����IDȡ�þɵı�������
		TDArtist.Select(ptrackData->ArtistDBId(),L"pid");
		//�ɵı�����
		if(TDArtist.GetRowCount()>0)
		{
			DArtistDB* pTabArtist = TDArtist.GetDataRow(0);
			wstring OldArtistName = pTabArtist->Name;
			__int64 OldArtistID = ptrackData->ArtistDBId();
			TDArtist.Clear();
			//�Ƿ�����������޸�
			if(OldArtistName != ptrackData->ArtistStr())
			{	//���±�����
				ArtistID = GetArtistIdByName(pSql,ptrackData->ArtistStr());
				//����ǰ��ʹ�ù��ľɱ��������ݿ��¼
				BaseTable<DItem> tItem(pSql);
				if(tItem.Count(OldArtistID,L"artist_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
				{					
					TDArtist.Delete(OldArtistID,L"pid");
				}
			}
			else//���ȷ��û���޸ķ��ؾ�ID
				ArtistID = OldArtistID;
		}
		else//û���ҵ��ɱ�����?
		{	//���±�����
			ArtistID = GetArtistIdByName(pSql,ptrackData->ArtistStr());
		}
	}
	else
	{	
		//û�б�����,������ǰ�ľɱ��������ݿ��¼
		if(ptrackData->ArtistDBId() !=0 )
		{		
			BaseTable<DItem> tItem(pSql);
			if(tItem.Count(ptrackData->ArtistDBId(),L"artist_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
			{		
				BaseTable<DArtistDB> TDArtist(pSql);
				TDArtist.Delete(ptrackData->ArtistDBId(),L"pid");
			}
		}
	}
	return ArtistID;
}
__int64 iPodDbBase::UpdateComposerDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData)
{
	__int64 ComposerID=0;
	//�Ƿ���������
	if(ptrackData->ComposerStr().length()>0)
	{	
		BaseTable<DComposer> TDComposer(pSql);	
		//����������IDȡ�þɵ���������
		TDComposer.Select(ptrackData->ComposerDBId(),L"pid");
		//�ɵ�������
		if(TDComposer.GetRowCount()>0)
		{
			DComposer* pTabComposer = TDComposer.GetDataRow(0);
			wstring OldCompSerName = pTabComposer->Name;
			__int64 OldCompserID = ptrackData->ComposerDBId();
			TDComposer.Clear();
			//�Ƿ������������޸�
			if(OldCompSerName != ptrackData->ComposerStr())
			{	//����������
				ComposerID = GetComposerIdByName(pSql,ptrackData->ComposerStr());
				//����ǰ��ʹ�ù��ľ����������ݿ��¼
				BaseTable<DItem> tItem(pSql);
				if(tItem.Count(OldCompserID,L"composer_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
				{					
					TDComposer.Delete(OldCompserID,L"pid");
				}
			}
			else//���ȷ��û���޸ķ��ؾ�ID
				ComposerID = OldCompserID;
		}
		else//û���ҵ���������?
		{	//����������
			ComposerID = GetComposerIdByName(pSql,ptrackData->ComposerStr());
		}
	}
	else
	{	
		//û��������,������ǰ�ľ����������ݿ��¼
		if(ptrackData->ComposerDBId() !=0 )
		{		
			BaseTable<DItem> tItem(pSql);
			if(tItem.Count(ptrackData->ComposerDBId(),L"composer_pid") == 1)//ֻ�в��ظ�������²��Ƴ�
			{			
				BaseTable<DComposer> TDComposer(pSql);	
				TDComposer.Delete(ptrackData->ComposerDBId(),L"pid");
			}
		}
	}
	return ComposerID;
}
wstring iPodDbBase::LocationDBDeltrack(__int64 pid)
{
	wstring DeviceFileName = L"";
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return DeviceFileName;	
	BaseTable<DLocation> location(&sql);	
	//������豸�ϵ��ļ�·��
	location.Select(pid,L"item_pid");	
	if(location.GetRowCount() == 1)
	{
		DLocation* pLocationSecect = location.GetDataRow(0);
		DeviceFileName = pLocationSecect->location;
		location.Clear();
		//�Ƴ�localtion����ؼ�¼
		location.Delete(pid,L"item_pid");
	}

	location.Clear();	
	sql.Close();
	return DeviceFileName;

}
wstring iPodDbBase::DeleteSqlDbtrack(CIosCDBTrackData * ptrackData,int& ArtWorkID)
{
	wstring DeviceFileName = LocationDBDeltrack(ptrackData->Dbid());	
	if(DeviceFileName.length() == 0 )
		return DeviceFileName;
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
		return DeviceFileName;
	//����item��
	BaseTable<DItem> tItem(&sql);
	tItem.Select(ptrackData->Dbid(),L"pid");
	if(tItem.GetRowCount() > 0)
	{	
		//���3����ص�pid�������������
		DItem* pItemSelect = tItem.GetDataRow(0);
		__int64 composerid = pItemSelect->composer_pid; 
		__int64 albumid = pItemSelect->album_pid;
		__int64 artistid = pItemSelect->artist_pid;
		__int64 trackid = ptrackData->Dbid();
		ArtWorkID = pItemSelect->artwork_cache_id;
		tItem.Clear();
		//�����ظ���������	
		if(tItem.Count(composerid,L"composer_pid") == 1)//ֻ�в��ظ�������²��Ƴ�,�������������
		{
			//�Ƴ������߱�
			BaseTable<DComposer> TDComposer(&sql);	
			TDComposer.Delete(composerid,L"pid");
		}
		//�����ظ��ı�������
		if(tItem.Count(artistid,L"artist_pid") == 1)//ֻ�в��ظ�������²��Ƴ�,�������������
		{
			//�Ƴ������߱�
			BaseTable<DArtistDB> TDArtist(&sql);	
			TDArtist.Delete(artistid,L"pid");
		}
		//�����ظ���ר��	
		if(tItem.Count(albumid,L"album_pid") == 1)//ֻ�в��ظ�������²��Ƴ�,�������������
		{		
			DeleteAlbumArtistDbRow(albumid,&sql);
		}
		//�Ƴ�item���¼		
		tItem.Delete(trackid,L"pid");

		if(G_DBver<310)
		{
			BaseTable<DAvformatInfo> tAvformatInfo(&sql);
			tAvformatInfo.Delete(trackid,L"item_pid");

			BaseTable<DExtViewMembership> tExtViewMembership(&sql);
			tExtViewMembership.Delete(trackid,L"item_pid");

		}
	}	
	sql.Close();
	return DeviceFileName;
}
void iPodDbBase::DeleteAlbumArtistDbRow(__int64 pid,SqliteAccess * sql)
{
	//�Ƴ�ר��������
	BaseTable<DAlbumDB> TDAlbum(sql);				
	//ר�������߱���
	TDAlbum.Select(pid,L"pid");			
	if(TDAlbum.GetRowCount() == 1)
	{
		//����ר�������߱�
		DAlbumDB * pAlbumSelect = TDAlbum.GetDataRow(0);
		__int64 AlbumArtistID = pAlbumSelect->artist_pid;
		TDAlbum.Clear();		
		if(TDAlbum.Count(AlbumArtistID,L"artist_pid") == 1)
		{
			//�Ƴ�ר��������
			BaseTable<DAlbumArtistDB> TAlbumArtist(sql);	
			TAlbumArtist.Delete(AlbumArtistID,L"pid");
		}
	}
	//�Ƴ�ר��
	TDAlbum.Clear();
	TDAlbum.Delete(pid,L"pid");
}


static void sqlite_func_iphone_sort_section(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	const unsigned char *sval;
	int res = 26;

	switch (sqlite3_value_type(argv[0])) {
		case SQLITE_BLOB:
		case SQLITE_TEXT:
			sval = sqlite3_value_text(argv[0]);
			if (sval && (sval[0] == 0x30) && (sval[1] >= 0x2D) && (sval[1] <= 0x5F)) {
				res = (sval[1] - 0x2D) / 2;
			}
			break;
		default:
			break;
	}
	sqlite3_result_int(context, res);
}

static void sort_key_get_buffer_boundaries(const char* sval, int *length, int *word_offset)
{
	int word_count = 0;
	int i = 0;
	int l = 0;
	int o = 0;
	char *sval_uppercase = NULL;
	if (sval && strlen(sval)) {
		sval_uppercase = strupr((char*)sval);
		while (sval_uppercase[i]) {			
			if (isalnum(sval_uppercase[i])) {
				l++;
			} else {
				switch (sval_uppercase[i]) {
					case ' ':
						word_count++;
						l++;
						break;
					case ':':
					case '-':
					case ',':
					case '.':
					case '\'':
					default:
						l += 2;
						break;
				}
			}
			i++;
		}
		//free(sval_uppercase);

		word_count++;
		/* magic + transformed string + length + word weights + null */
		o = 1 + l + 3;
		l = o + (word_count*2) + 1;
	} else {
		l = 4;
	}

	*length = l;
	*word_offset = o;
}
static void sqlite_func_ML3Section(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	if(argv[0] == NULL)
	{
		sqlite3_result_null(context);
		return;
	}
	const unsigned char* arg0 = sqlite3_value_text(argv[0]);
	char chr = arg0[0];
	if(chr>32 && islower(chr))chr-=32;//
	if (chr >= 65) //65 is uppercase 'a'
		chr -= 64;
	sqlite3_result_int(context, (int)chr);

}
static int sqlite_func_ML3SortCollation(void* a1,int a2,const void* a3,int a4,const void* a5)
{
	return 0;
}

static void sqlite_func_iphone_sort_key(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	const char *sval;
	char *sval_uppercase = NULL;

	char *buffer = NULL;

	int word_count = 0;
	int word_offset = 0;
	int word_length = 0;
	int i = 0;
	int buffer_index = 0;
	int buffer_size = 0;

	switch (sqlite3_value_type(argv[0])) {
		case SQLITE_TEXT:
			sval = (const char*)sqlite3_value_text(argv[0]);
			sort_key_get_buffer_boundaries(sval, &buffer_size, &word_offset);
			buffer = (char*)malloc(buffer_size);
			memset(buffer, '\0', buffer_size);
			buffer[buffer_index] = 0x31;
			if (sval) {
				if (buffer_size > 4) {
					buffer[buffer_index++] = 0x30;
					/* transform text value */
					/* uppercase the text */
					sval_uppercase = strupr((char*)sval);
					while (sval_uppercase[i]) {
						word_length++;
						if (isalnum(sval_uppercase[i])) {
							/* transform regular character */
							buffer[buffer_index++] = sval_uppercase[i] - (0x55 - sval_uppercase[i]);
						} else {
							/* transform special chars (punctuation,special) */
							switch (sval_uppercase[i]) {
		case ' ':
			buffer[buffer_index++] = 0x06;
			word_length--;

			/* since we reached word end, calculate word weight */
			buffer[word_offset + word_count*2] = 0x8f;
			buffer[word_offset + word_count*2 + 1] = (char)(0x86 - word_length);
			word_count++;
			word_length = 0;
			break;
		case ':':
			buffer[buffer_index++] = 0x07;
			buffer[buffer_index++] = 0xd8;
			break;
		case '-':
			buffer[buffer_index++] = 0x07;
			buffer[buffer_index++] = 0x90;
			break;
		case ',':
			buffer[buffer_index++] = 0x07;
			buffer[buffer_index++] = 0xb2;
			break;
		case '.':
			buffer[buffer_index++] = 0x08;
			buffer[buffer_index++] = 0x51;
			break;
		case '\'':
			buffer[buffer_index++] = 0x07;
			buffer[buffer_index++] = 0x31;
			break;
		default:
			/* FIXME: We just simulate "-" for any other char, needs proper conversion */
			buffer[buffer_index++] = 0x07;
			buffer[buffer_index++] = 0x90;
			break;
							}
						}
						i++;
					}
					//free(sval_uppercase);

					/* calculate word weight for last word */
					buffer[word_offset + word_count*2] = 0x8f;
					buffer[word_offset + word_count*2 + 1] = 3 + word_length;
					word_count++;
					word_length = 0;

					/* write length of input string */
					buffer[word_offset - 3] = 0x01;
					buffer[word_offset - 2] = i + 4; /* length of input string + 4 */
					buffer[word_offset - 1] = 0x01;
				} else {
					buffer[0] = 0x31;
					buffer[1] = 0x01;
					buffer[2] = 0x01;
				}
			}
			sqlite3_result_blob(context, buffer, buffer_size, free);
			break;
		case SQLITE_NULL:
			buffer = (char*)malloc(4);
			memcpy(buffer, "\x31\x01\x01\x00", 4);
			sqlite3_result_blob(context, buffer, 4, free);
			break;
		default:
			sqlite3_result_null(context);
			break;
	}
}

void iPodDbBase::CreateiPhoneKeyFunction(CppSQLite3DB* db)
{
	RETURN_IF ( ! db->IsOpen() );
	if(G_DBver<400)
	{
		sqlite3_create_function(db->GetSqlite3DB(), "icu_data_for_string", 1, SQLITE_ANY, NULL, &sqlite_func_iphone_sort_key, NULL, NULL);
		sqlite3_create_function(db->GetSqlite3DB(), "icu_section_data_for_string", 1, SQLITE_ANY, NULL, &sqlite_func_iphone_sort_section, NULL, NULL);
		sqlite3_create_function(db->GetSqlite3DB(), "icu_data_for_string", 1, SQLITE_ANY, NULL, &sqlite_func_iphone_sort_key, NULL, NULL);
		sqlite3_create_function(db->GetSqlite3DB(), "ML3Section", 1, SQLITE_UTF16_ALIGNED, NULL, &sqlite_func_ML3Section, NULL, NULL);
		sqlite3_create_collation(db->GetSqlite3DB(), "ML3SortCollation", SQLITE_UTF8, NULL, &sqlite_func_ML3SortCollation);
		sqlite3_create_collation(db->GetSqlite3DB(), "ML3SearchCollation", SQLITE_UTF8, NULL, &sqlite_func_ML3SortCollation);
	}
	else
	{
		sqlite3_create_function(db->GetSqlite3DB(), "iPhoneSortKey", 1, SQLITE_ANY, NULL, &sqlite_func_iphone_sort_key, NULL, NULL);
		sqlite3_create_function(db->GetSqlite3DB(), "iPhoneSortSection", 1, SQLITE_ANY, NULL, &sqlite_func_iphone_sort_section, NULL, NULL);
	}
}

void iPodDbBase::GetAndCreateAllDBID(IiPodMediaParser* pMP3data,STU_CREATE_DBID * pstudb)
{
	__int64 trackid,artistid,albumid,composerid,albumartistid;
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
	{
		pstudb->AlbumArtistID=0;
		pstudb->AlbumID=0;
		pstudb->ArtistID=0;
		pstudb->ComposerID=0;
		pstudb->TrackID=0;
		return;
	}
	CreateiPhoneKeyFunction(sql.GetDB());
	//���Ӹ�track�Ĳ��ظ����ݿ�ID
	BaseTable<DItem> tItem(&sql);
	//if(G_DBver>=400 && G_DBver<430)
	//{			
 		int ret = 1;	
 		do{
 			trackid=Create64BitID();
 			tItem.Select(trackid,L"Pid");		
 			ret =tItem.GetRowCount();
 			tItem.Clear();
 		}while( ret != 0);	
	//}
	//else
	//	trackid = tItem.MaxPid(L"pid")+1;
	pstudb->AlbumArtist = pMP3data->PropertyGetAlbumPerformer();
	if(pstudb->AlbumArtist.length() == 0)
		pstudb->AlbumArtist = pMP3data->PropertyGetMusicPerformer();

	//�Ȳ����Ƿ��Ѿ�����������ҵ����֡�
	artistid = GetArtistIdByName(&sql,pMP3data->PropertyGetMusicPerformer());
	//ר��������ID��
	albumartistid = GetAlbumArtistIdByName(&sql,pstudb->AlbumArtist);
	//�����ҵ�ID��
	composerid = GetComposerIdByName(&sql,pMP3data->PropertyGetComposer());

	pstudb->AlbumArtistID = albumartistid;
	pstudb->ArtistID = artistid;
	pstudb->ComposerID = composerid;
	pstudb->TrackID = trackid;

	//ר��ID��
	albumid = GetAlbumIDByName(&sql,pMP3data->PropertyGetAlbumName(),pstudb);
	pstudb->AlbumID = albumid;

	sql.Close();
	

}
__int64 iPodDbBase::GetArtistIdByName(SqliteAccess* pSql,wstring ArtistName)
{
	__int64 artistid;
	//�Ȳ����Ƿ��Ѿ�����������ҵ����֡�
	BaseTable<DArtistDB> pArtistDB(pSql);	
	pArtistDB.Select(L"name='" + ArtistName + L"'");
	int ret = pArtistDB.GetRowCount();	
	if(ret == 0)
	{//û����Ҫ�½�
		DArtistDB* ArtistDB =  new DArtistDB();
		ArtistDB->Name = ArtistName;
		ArtistDB->Kind = 0;
		ArtistDB->artwork_status =0;
		ArtistDB->Artwork_album_pid =0;
		ArtistDB->name_order = 0;
		ArtistDB->sort_name = ArtistName;
		ArtistDB->name_order_section=0;
		ArtistDB->name_blank=ArtistDB->Name.length()>0?1:0;
		pArtistDB.Clear();
		do{//�������ظ����ݿ�ID
			artistid=Create64BitID();
			pArtistDB.Select(artistid,L"Pid");		
			ret =pArtistDB.GetRowCount();
		}while( ret != 0);

		ArtistDB->Pid = artistid;		
		pArtistDB.Insert(ArtistDB);
		SAFE_DELETE(ArtistDB);
	}
	else
	{
		DArtistDB* ArtistDB = pArtistDB.GetDataRow(0);
		artistid=ArtistDB->Pid;
	}
	pArtistDB.Clear();
	return artistid;
}
__int64 iPodDbBase::GetAlbumArtistIdByName(SqliteAccess* pSql,wstring AlbumArtistName)
{
	__int64 albumartistid;
	//�Ȳ����Ƿ��Ѿ������ר�������ߵ����֡�
	BaseTable<DAlbumArtistDB> pAlbumArtistDB(pSql);	
	pAlbumArtistDB.Select(L"name='" + AlbumArtistName + L"'");
	int ret = pAlbumArtistDB.GetRowCount();	
	if(ret == 0)
	{//û����Ҫ�½�
		DAlbumArtistDB* AlbumArtistDB =  new DAlbumArtistDB();
		AlbumArtistDB->Name = AlbumArtistName;
		if(AlbumArtistDB->Name.length() == 0)
			AlbumArtistDB->Name = AlbumArtistName;
		AlbumArtistDB->Kind = 2;
		AlbumArtistDB->artwork_status =1;
		AlbumArtistDB->Artwork_album_pid =0;
		AlbumArtistDB->name_order = 0;
		AlbumArtistDB->sort_name = AlbumArtistDB->Name;
		AlbumArtistDB->name_order_section=0;
		AlbumArtistDB->name_blank=AlbumArtistDB->Name.length()>0?1:0;
		pAlbumArtistDB.Clear();
		do{//�������ظ����ݿ�ID
			albumartistid=Create64BitID();
			pAlbumArtistDB.Select(albumartistid,L"Pid");		
			ret =pAlbumArtistDB.GetRowCount();
		}while( ret != 0);

		AlbumArtistDB->Pid = albumartistid;
		pAlbumArtistDB.Insert(AlbumArtistDB);
		SAFE_DELETE(AlbumArtistDB);
	}
	else
	{
		DAlbumArtistDB* AlbumArtistDB = pAlbumArtistDB.GetDataRow(0);
		albumartistid=AlbumArtistDB->Pid;
	}
	pAlbumArtistDB.Clear();
	return albumartistid;
}
__int64 iPodDbBase::GetAlbumIDByName(SqliteAccess* pSql,wstring AlbumName,STU_CREATE_DBID * pstudb)
{
	__int64 albumid=0;
	//�Ȳ����Ƿ��Ѿ������ר�������֡�
	BaseTable<DAlbumDB> palbum(pSql);	
	palbum.Select(L"name='" + AlbumName + L"'");
	int ret = palbum.GetRowCount();

	if(ret == 0)
	{//û����Ҫ�½�
		DAlbumDB* Album =  new DAlbumDB();
		Album->Name = AlbumName;
		Album->name_order = 0;
		Album->sort_name = AlbumName;
		Album->name_order_section=0;
		Album->artist_pid = pstudb->AlbumArtistID;
		Album->Artist = pstudb->AlbumArtist;
		Album->sort_artist = Album->Artist;
		Album->all_compilations =0;
		Album->artwork_status = 0;
		Album->Kind =2;
		Album->artwork_item_pid = pstudb->TrackID;
		Album->user_rating = 0;
		Album->artist_order_section = 0;
		Album->name_blank=Album->Name.length()>0?1:0;
		palbum.Clear();
		do{//�������ظ����ݿ�ID
			albumid=Create64BitID();
			palbum.Select(albumid,L"Pid");		
			ret =palbum.GetRowCount();
		}while( ret != 0);

		Album->Pid = albumid;
		palbum.Insert(Album);
	}
	else
	{
		DAlbumDB* Album = palbum.GetDataRow(0);
		albumid=Album->Pid;
	}
	palbum.Clear();
	return albumid;
}
__int64 iPodDbBase::GetComposerIdByName(SqliteAccess* pSql,wstring ComperName)
{
	__int64 ComposerId=0;
	//�Ȳ����Ƿ��Ѿ�����������ҵ����֡�
	BaseTable<DComposer> pComposer(pSql);	
	pComposer.Select(L"name='" + ComperName + L"'");
	int ret = pComposer.GetRowCount();	
	if(ret == 0)
	{//û����Ҫ�½�
		DComposer* Composer =  new DComposer();
		Composer->Name = ComperName;
		Composer->name_order = 0;
		Composer->sort_name = ComperName;
		Composer->name_order_section=0;
		Composer->name_blank=Composer->Name.length()>0?1:0;
		pComposer.Clear();
		do{//�������ظ����ݿ�ID
			ComposerId=Create64BitID();
			pComposer.Select(ComposerId,L"Pid");		
			ret =pComposer.GetRowCount();
		}while( ret != 0);
		Composer->Pid = ComposerId;
		int insertRet = pComposer.Insert(Composer);
		if(insertRet==0)//����ʧ�ܱ�����0
			ComposerId=0;
	}
	else
	{
		DComposer* Compose = pComposer.GetDataRow(0);
		ComposerId=Compose->Pid;
	}
	pComposer.Clear();
	return ComposerId;
}
__int64 iPodDbBase::Create64BitID()
{
	srand( GetTickCount() );
	return (((ND_UINT64)(rand()&0xffff))<<48)+(((ND_UINT64)(rand()&0xffff))<<32)+(((ND_UINT64)(rand()&0xffff))<<16)+((ND_UINT64)(rand()&0xffff));
}

bool iPodDbBase::CheckNewName(wstring filename,BaseTable<DLocation>* plocaltion)
{
	USES_CONVERSION;
	wstring sql = L"Location = '" + filename + L"'";

	plocaltion->Select(sql);
	int ret =plocaltion->GetRowCount();

	if( ret == 0)
		return false;
	else
		return true;
}
wstring iPodDbBase::GetNewDevFileName()
{
		USES_CONVERSION;
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return L"";
	BaseTable<DLocation> location(&sql);
	wstring DeviceFileName= A2W(gen_file_name().c_str());
	while(CheckNewName(DeviceFileName,&location))
	{		
		DeviceFileName = A2W(gen_file_name().c_str());
	}
	sql.Close();
	return DeviceFileName;
}

void iPodDbBase::InsertSqlDBNewTrack(STU_CREATE_DBID * pstudb,IiPodMediaParser * pMP3Data)
{
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return;

	//wstring DeviceFileName;

	BaseTable<DLocation> location(&sql);
	USES_CONVERSION;
	//����Localtion��
	DLocation* pLocation = new DLocation();
	pLocation->item_pid = pstudb->TrackID;
	pLocation->sub_id = 0;// pTrackData->Dbid2();
	pLocation->base_location_id = 1;
	pLocation->location_type = 0x46494C45;//����ȫ����"FILE"
	//DeviceFileName= A2W(gen_file_name().c_str()); //����ͬ���ļ�ҪҪ���ļ���
	////while(CheckNewName(DeviceFileName,&location))
	//{		
	//	DeviceFileName = A2W(gen_file_name().c_str());
	//}
	pLocation->location = pstudb->DevFilePath;//DeviceFileName;
	// char FileType[4];
	// memcpy(FileType,pMP3Data->GetLocalFileName().substr(pMP3Data->GetLocalFileName().length()-4,3).c_str(),3);
	// FileType[3] = ' ';
	//memcpy((char*)&pLocation->extension ,pTrackData->Type().c_str(),4);//1297101600 -"MP3 "
	wstring fileExt = CFileOperation::GetFileExt(pMP3Data->GetLocalFileName());
	fileExt = CStrOperation::toUpperW(fileExt);
	fileExt+=L" ";
	memcpy((char*)&pLocation->extension ,W2A(fileExt.c_str()),4);
	//pLocation->extension = 1297101600;
	time_t now_time;
	time(&now_time);	
	pLocation->date_created = now_time;
	pLocation->file_size =  pMP3Data->GetFileSize();

	location.Insert(pLocation);
	SAFE_DELETE(pLocation);
	sql.Close();

	//����item��
	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
		return ;
	CreateiPhoneKeyFunction(sql.GetDB());
	BaseTable<DItem> tItem(&sql);
	DItem* pItem = new DItem();
	pItem->pid = pstudb->TrackID;
	pItem->media_kind = 1;
	pItem->is_song = 1;
	struct _finddata_t fileInfo;
	_findfirst( W2A(pMP3Data->GetLocalFileName().c_str()), &fileInfo );
	pItem->date_modified = fileInfo.time_write;	
	pItem->Year = atoi(W2A(pMP3Data->PropertyGetYear().c_str()));
	pItem->total_time_ms =  pMP3Data->GetPlayTime();
	pItem->track_number = atoi(W2A(pMP3Data->PropertyGetTrackNumber().c_str()));
	pItem->disc_number=atoi(W2A(pMP3Data->PropertyGetCDNumber().c_str()));
	pItem->Bpm=atoi(W2A(pMP3Data->PropertyGetBPM().c_str()));
	pItem->genre_id  = 0;//atoi(W2A(pMP3Data->PropertyGetStyle().c_str()));
	pItem->Title = pMP3Data->PropertyGetMusicName();
	if(pItem->Title.length() == 0) //���û�б��⣬Ҫ���ļ�����Ϊ����
		pItem->Title = CFileOperation::GetFileName(pMP3Data->GetLocalFileName());
	pItem->Artist = pMP3Data->PropertyGetMusicPerformer();
	pItem->Album = pMP3Data->PropertyGetAlbumName();
	pItem->album_artist = pMP3Data->PropertyGetAlbumPerformer();
	if(pItem->album_artist.length() == 0 )
		pItem->album_artist = pMP3Data->PropertyGetMusicPerformer();
	pItem->Composer = pMP3Data->PropertyGetComposer();
	pItem->sort_title =  pMP3Data->PropertyGetMusicName();//pMP3Data->PropertyGetNameSort();
	pItem->sort_artist = pItem->Artist; //pMP3Data->PropertyGetPerformerSort();
	pItem->sort_album =  pItem->Album;//pMP3Data->PropertyGetAlbumSort();
	pItem->sort_album_artist =  pItem->album_artist;//pMP3Data->PropertyGetAlbumPerformerSort();
	pItem->sort_composer =  pItem->Composer;//pMP3Data->PropertyGetComposerSort();
	pItem->comment = pMP3Data->PropertyGetNotes(); 
	pItem->Grouping =  pMP3Data->PropertyGetClassify();
	pItem->Description =  pMP3Data->PropertyGetDescription();  
	pItem->description_long = L"";
	//pItem->collection_description = L"";
	pItem->Copyright=L"";
	pItem->album_artist_pid = pstudb->AlbumArtistID;
	pItem->in_songs_collection=1;
	pItem->album_pid = pstudb->AlbumID;
	pItem->artist_pid = pstudb->ArtistID;
	pItem->composer_pid = pstudb->ComposerID;
	pItem->artwork_cache_id = pstudb->ArtWorkID;
	pItem->is_movie = pMP3Data->IsVideo()?1:0;
	if(pstudb->ArtWorkID > 0)
		pItem->artwork_status =1;
	tItem.Insert(pItem);
	SAFE_DELETE(pItem);
// 	if(G_DBver<310)
// 	{
// 		BaseTable<DAvformatInfo> tAvformatInfo(&sql);
// 		DAvformatInfo* pAvformatInfo = new DAvformatInfo();
// 		pAvformatInfo->item_pid=pstudb->TrackID;
// 		pAvformatInfo->sub_id;
// 		pAvformatInfo->audio_format=301;
// 		pAvformatInfo->bit_rate=pMP3Data->GetiBitSpeed();
// 		pAvformatInfo->sample_rate=pMP3Data->GetSampleRate();
// 		pAvformatInfo->duration = pMP3Data->GetPlayTime();	
// 		pAvformatInfo->gapless_heuristic_info=0;	
// 		pAvformatInfo->gapless_encoding_delay=0;	
// 		pAvformatInfo->gapless_encoding_drain=0;	
// 		pAvformatInfo->gapless_last_frame_resynch=0;	
// 		pAvformatInfo->analysis_inhibit_flags=0;	
// 		pAvformatInfo->audio_fingerprint=0;	
// 		pAvformatInfo->volume_normalization_energy=pMP3Data->GetVolumeAdjustment();
// 		tAvformatInfo.Insert(pAvformatInfo);
// 		delete pAvformatInfo;
// 
// 
// 		BaseTable<DExtViewMembership> tExtViewMembership(&sql);
// 		DExtViewMembership* pExtViewMembership = new DExtViewMembership();
// 		pExtViewMembership->item_pid=pstudb->TrackID;
// 		pExtViewMembership->movie_mbr=0;
// 		pExtViewMembership->movie_rental_mbr=0;
// 		tExtViewMembership.Insert(pExtViewMembership);
// 		delete pExtViewMembership;
// 	}
	sql.Close();
	//return DeviceFileName;
}


__int64 iPodDbBase::find_Album_artist_PID(SqliteAccess *sql,__int64  trackpid)
{
	__int64 ret=0;
	BaseTable<DItem> tItem(sql);
	tItem.Select(trackpid,L"pid");
	if(tItem.GetRowCount() > 0)
	{
		DItem* pitem = tItem.GetDataRow(0);
		ret =  pitem->album_artist_pid;
		tItem.Clear();
	}
	return ret;
};
__int64 iPodDbBase::find_composer_PID(SqliteAccess *sql,__int64  trackpid)
{
	__int64 ret=0;
	BaseTable<DItem> tItem(sql);
	tItem.Select(trackpid,L"pid");
	if(tItem.GetRowCount() > 0)
	{
		DItem* pitem = tItem.GetDataRow(0);
		ret =  pitem->composer_pid;
		tItem.Clear();
	}

	return ret;
};
string iPodDbBase::gen_file_name()
{
	char szBuf[256];
	int iV32;
	char sz[5];

	srand( GetTickCount() );
	iV32 = rand() % 100;
	do 
	{
		iV32 = rand() % 100;
	} while (iV32 > 48);
	sprintf_s( szBuf, 255, "F%02d", iV32 );
	iV32 = rand() % 26;
	sz[0] = 'A'+iV32;
	iV32 = rand() % 26;
	sz[1] = 'A'+iV32;
	iV32 = rand() % 26;
	sz[2] = 'A'+iV32;
	iV32 = rand() % 26;
	sz[3] = 'A'+iV32;
	sz[4] = 0;
	sprintf_s( szBuf, 255, "%s/%s.mp3", szBuf, sz );
	string ret(szBuf);
	return ret;//_strdup( szBuf );
}

wstring iPodDbBase::getInfoFromSqlDB(int infotype)
{
	return L"";
}




wstring iPodDbBase::GetTrackPath(__int64 pid)
{
	wstring DeviceFileName = L"";
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return DeviceFileName;	
	BaseTable<DLocation> location(&sql);	
	//������豸�ϵ��ļ�·��
	location.Select(pid,L"item_pid");	
	if(location.GetRowCount() == 1)
	{
		DLocation* pLocationSecect = location.GetDataRow(0);
		DeviceFileName = pLocationSecect->location;			
	}
	location.Clear();	
	sql.Close();
	return DeviceFileName;

}

void iPodDbBase::TestDb3()
{
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Locations.itdb"))
		return;	
	BaseTable<DLocation> location(&sql);	
	//������豸�ϵ��ļ�·��
	location.SelectAll();
	location.Clear();
	sql.Close();

	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
		return;	

	/*BaseTable<DAlbumArtistDB> AlbumArtistDB(&sql);
	AlbumArtistDB.SelectAll();
	AlbumArtistDB.Clear();*/

// 	BaseTable<DAlbumDB> AlbumDB(&sql);
// 	AlbumDB.SelectAll();
// 	AlbumDB.Clear();

// 	BaseTable<DArtistDB> ArtistDB(&sql);
// 	ArtistDB.SelectAll();
// 	ArtistDB.Clear();

// 	BaseTable<DComposer> Composer(&sql);
// 	Composer.SelectAll();
// 	Composer.Clear();

	BaseTable<DContainerDB> ContainerDB(&sql);
	ContainerDB.SelectAll();
	ContainerDB.Clear();

// 	BaseTable<DItem> Item(&sql);
// 	Item.SelectAll();
// 	Item.Clear();

	BaseTable<DItemToContainer> ItemToContainer(&sql);
	ItemToContainer.SelectAll();
	ItemToContainer.Clear();

	sql.Close();

}

int iPodDbBase::GetArtworkid(__int64 pid)
{
	SqliteAccess sql;
	if(!sql.Open(_sqlDbPath + L"Library.itdb"))
		return 0;

	int ret=0;
	BaseTable<DItem> tItem(&sql);
	tItem.Select(pid,L"pid");
	if(tItem.GetRowCount() > 0)
	{
		DItem* pitem = tItem.GetDataRow(0);
		ret =  pitem->artwork_cache_id;
		tItem.Clear();
	}
	sql.Close();
	return ret;
}


#endif