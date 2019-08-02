#include "stdafx.h"
#ifdef MODULE_IPOD

#include <atlconv.h>

#include "Module/iPod/IosiPodData.h"
#include "HashVerify.h"
#include "Module/iPod/VCiPodFileSync.h"
#include "Module/iPod/PlaylistDB.h"

#include "zlib.h"
#include "Module/Common/Media/iPodMedia/IiPodMediaParser.h"

#pragma warning( disable : 4244)
#define  CHUNK 16384

using namespace std;
#define SAVESTRUCT(n,a,p,b){n++; \
		a.insert(map<int, cdbBase*>::value_type(n,b)); \
		p->ItemKey.push_back(n);}

template<class T>
void DeleteVector(vector<T*> p)
{
	for(size_t i = 0; i < p.size(); ++i)
		SAFE_DELETE(p[i]);
	p.clear();
}

using namespace ND91Assistant;
CIosiPodData::CIosiPodData()
{	 
	_hashab.InitInstance();
}
CIosiPodData::~CIosiPodData()
{
		DeleteVector(_albumlist);
		DeleteVector(_alltracklist);
		DeleteVector(_artist);
		DeleteVector(_playlist1);
		DeleteVector(_playlist2);
		DeleteVector(_playlist3);
		for(map<int,cdbBase*>::iterator it =_AllItem.begin();it != _AllItem.end();it++)
		{
			cdbBase* p=it->second;
			if(p)
				p->release();
			SAFE_DELETE(p);		
		}
		_AllItem.clear();
}

void CIosiPodData::LoadCDBDataFromFile(wstring CDBfilePath, cIosPlayListDB* pIosPlayListDB)
{

	vector<cdbBase* > verCBDStruct;

	wstring cdbFileName = CDBfilePath + L"\\iTunes\\iTunesCDB";

	
		
	string fileData  = CFileOperation::ReadFile(cdbFileName, true);

	wstring TempFileName = cdbFileName + L"New";

	if(fileData.length() ==0 )
		return;

	CFileOperation::WriteBinFile(TempFileName,fileData.c_str(),fileData.length());

	BinaryBufferReader readerhead(fileData.c_str(), fileData.size());

	USES_CONVERSION;
	//先读最外层结构
	m_Mhbd.Read(&readerhead);
 	if(m_Mhbd.hashtype[0] == 3)
 	{
		string struuid = W2A(iPodFileSync::GetUUID().c_str());

		int length = struuid.length();
		char* buffer = new char[length / 2];
		for (int i = 0; i < length; i += 2)
		{
			
			int iVal = 0;                                                                              
			const char *pSrcTemp = struuid.c_str() + i;                                                     
			sscanf((char *)pSrcTemp, "%02x", &iVal);                                                   
			buffer[i / 2] = (unsigned char)iVal;           
				
		//	 =  Convert.ToByte(A_0.Substring(i, 2), 0x10);
		}
		_hashab.GetiTunesCDBFilePath(W2A(TempFileName.c_str())); 		
		_hashab.SetUuidAndHash(buffer,1);
 	
 	}
 	
	HashVerify::BackCheckSumiTunesCDB((void*)fileData.c_str(), fileData.size(),SrcParam1,SrcParam);

	char* pUnCompress = NULL;
	int nUnCompressBuffSize = 0;

	if(m_Mhbd._nIsCompress == 2 )
	{	
		//压缩包开始位置
		const char * p = readerhead.GetCurBuffer();
		__uncompress_itdb(p,m_Mhbd._nOffsetCompress - m_Mhbd._head._headSize + 1,&pUnCompress,&nUnCompressBuffSize);
	}
	__int16 hashtype;
	memcpy((char*)&hashtype,m_Mhbd.hashtype,2);
	m_HashType = hashtype;

	if(nUnCompressBuffSize == 0)//解压缩失败
	{
	//	THROW_ASSISTANT_EXCEPTION(CDB_FILE_FORMAT_ERROR);
		return;
	}

	ofstream ofs(L"d:\\iTunesUncompress", ios_base::out|ios_base::binary);
	ofs.write(pUnCompress, nUnCompressBuffSize);
	ofs.close();

	//读取解压后的缓冲区
	BinaryBufferReader readerBody(pUnCompress,nUnCompressBuffSize);

	for(int i=0;i<m_Mhbd._nItemNum;i++)	
	{
			CMhsd* MhsdItem = new CMhsd();
			MhsdItem->Read(&readerBody);	
			//将每个读取出来的数据保存到map表中
			int newkey = _AllItem.size();
			_AllItem.insert(map<int, cdbBase*>::value_type(newkey,MhsdItem));
			_MhsdList.insert(map<int,int>::value_type(newkey,MhsdItem->_type));//mhsd头
				
			switch(MhsdItem->_type)
			{
			case 1://所有歌曲
				ReadTracks(&readerBody,&_alltracklist);
				break;
			case 4: //专辑列表
				ReadAlbums(&readerBody);
				break;
			case 2:
				ReadPlayList(&readerBody,&_playlist1);
				break;
			case 3://列表名=设备名
				ReadPlayList(&readerBody,&_playlist2);
				break;
			case 5://播放列表 目前只实现读取歌曲ID列表跟列表ID
				ReadPlayList(&readerBody,&_playlist3);//音乐 影片 图书 电视节目 租借项目
				break;
			case 8://表演者列表
				ReadArtist(&readerBody);
				break;
			case 6:
				ReadTracks(&readerBody,&_alltracklist1);
				break;
			case 9:
			case 10:
			default://未知格式直接跳过
				CMhuk * pMhukIte = new CMhuk(MhsdItem->_nBodylen -  MhsdItem->_head._headSize);
				//readerBody.AddOffset(MhsdItem->_nBodylen - MhsdItem->_head._headSize);
				pMhukIte->Read(&readerBody);
				int newkey = _AllItem.size();
				_AllItem.insert(map<int, cdbBase*>::value_type(newkey,pMhukIte));
				_MhsdList.insert(map<int,int>::value_type(newkey,0));
				break;				
			}		
	}
	
	free(pUnCompress);

	wstring _TempFilePath = CDBfilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB )//&& pIosPlayListDB->CheckDBActive())
		pIosPlayListDB->GetAllPlayList(&_playlistDB);
}
void CIosiPodData::ReLoadPlayListDB(cIosPlayListDB* pIosPlayListDB)
{
	if(pIosPlayListDB)// && pIosPlayListDB->CheckDBActive())
		pIosPlayListDB->GetAllPlayList(&_playlistDB);
}
__int64 CIosiPodData::GetTrackDbidByid(int id)
{
	for(size_t i=0;i<_alltracklist.size();i++)
	{
		CIosCDBTrackData* pTrackData =_alltracklist.at(i);
		if(pTrackData->Id() == id)
			return pTrackData->Dbid();
	}
	return 0;
}
void CIosiPodData::ReadPlayList(BinaryBufferReader *pReader,vector<CIosCDBPlayList*>* _playlist)
{
	CMhlp *MhlpIte = new CMhlp();
	MhlpIte->Read(pReader);//读取列表	

	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,MhlpIte));
	_MhsdList.insert(map<int,int>::value_type(newkey,0));
	//循环读取每个播放列表信息			
	for(int i = 0;i<MhlpIte->_nMhodNum;i++ )
	{
		CMhyp *MhypIte = new CMhyp();
		MhypIte->Read(pReader);
		
		CIosCDBPlayList* DataPlayList = new CIosCDBPlayList();
		DataPlayList->ID(MhypIte->_playlistID);//保存列表信息-id
			
		//保存读出的结构
		SAVESTRUCT(newkey,_AllItem,DataPlayList,MhypIte);

		//循环读取每个歌曲信息子项
		for(int m=0;m<MhypIte->_nMhodNum;m++)
		{
			CMhod *MhodIte = new CMhod();
			MhodIte->Read(pReader);

			//保存读出的结构
			SAVESTRUCT(newkey,_AllItem,DataPlayList,MhodIte);

			if (MhodIte->_type == MHOD_ID_TITLE)
				DataPlayList->Listname(MhodIte->_pbody->GetInfo()); 
			
		}	

		//循环读取每个列表的歌曲信息
		for(int k=0;k<MhypIte->_nmhips;k++)
		{
			CMhip* MhipIte = new CMhip();//读取信息
			MhipIte->Read(pReader);	

			//保存读出的结构
			SAVESTRUCT(newkey,_AllItem,DataPlayList,MhipIte);

			//DataPlayList->TrackList()->push_back(MhipIte->_Trackid);//保存列表里面歌曲信息-id
			DataPlayList->PushTrackid(GetTrackDbidByid(MhipIte->_Trackid));
			//循环读取每个歌曲信息子项
			for(int j=0;j<MhipIte->_nMhodNum;j++)
			{
				CMhod *MhodIte = new CMhod();
				MhodIte->Read(pReader);

				//保存读出的结构
				SAVESTRUCT(newkey,_AllItem,DataPlayList,MhodIte);

				//if (MhodIte->_type == MHOD_ID_PLAYLIST)
				//	DataPlayList->Listname(MhodIte->_pbody->GetInfo()); //这里存放的是乐曲在列表的排序位置
			
			}	
		}
		_playlist->push_back(DataPlayList);
	}			
}

void CIosiPodData::ReadAlbums(BinaryBufferReader *pReader)
{
	CMhla* MhlaIte = new CMhla();
	MhlaIte->Read(pReader);//读取列表
	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,MhlaIte));
	_MhsdList.insert(map<int,int>::value_type(newkey,0));
	for(int i = 0;i<MhlaIte->_nAlbumsNum;i++ )
	{

		CMhia* MhiaIte = new CMhia();//读取信息
		MhiaIte->Read(pReader);

		CIosCDBAlbumData * AlbumData = new CIosCDBAlbumData();
		AlbumData->Id(MhiaIte->_nAlbumid);
		AlbumData->DBID(MhiaIte->_nAlbumDBid);

		SAVESTRUCT(newkey,_AllItem,AlbumData,MhiaIte);

		for(int j=0;j<MhiaIte->_nTrackNum;j++)
		{
			CMhod* MhodIte = new CMhod();
			MhodIte->Read(pReader);

			SAVESTRUCT(newkey,_AllItem,AlbumData,MhodIte);

			if (MhodIte->_type == MHOD_ID_ALBUM_ALBUM)
				AlbumData->AlbumName(MhodIte->_pbody->GetInfo());
			if (MhodIte->_type == MHOD_ID_ALBUM_ARTIST)
				AlbumData->AlbumArtis(MhodIte->_pbody->GetInfo());	
			if (MhodIte->_type == MHOD_ID_ALBUM_SORT_ARTIST)
				AlbumData->AlbumSort(MhodIte->_pbody->GetInfo());	
		}		
		_albumlist.push_back(AlbumData);
	}
}
void CIosiPodData::ReadTracks(BinaryBufferReader *pReader,vector<CIosCDBTrackData*>* alltracklist)
{
	USES_CONVERSION;
	CMhlt* MhltIte = new CMhlt();
	MhltIte->Read(pReader);
	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,MhltIte));
	_MhsdList.insert(map<int,int>::value_type(newkey,0));

	for(int i=0;i<MhltIte->_nTracksNum;i++)
	{
		CIosCDBTrackData* pBTrackData = new CIosCDBTrackData();
		CMhit* MhitIte=new CMhit();
		MhitIte->Read(pReader);

		SAVESTRUCT(newkey,_AllItem,pBTrackData,MhitIte);

		pBTrackData->Albumid(MhitIte->_alumbID);
		pBTrackData->Artist(MhitIte->_artistid);
		pBTrackData->Id(MhitIte->_ntrackid);
		pBTrackData->Lryic(MhitIte->_haslyric==0?false:true);
		pBTrackData->Timer(MhitIte->_longer);					
		pBTrackData->Type(A2W(MhitIte->_nFileType));
		pBTrackData->HasArtWork(MhitIte->_hasartwork==0?false:true);
		pBTrackData->Isvisable(MhitIte->_nIsHide==0?false:true);
		pBTrackData->Dbid(MhitIte->_ndbID);
		pBTrackData->Dbid2(MhitIte->_dbid2);
		pBTrackData->IsVideo(MhitIte->_isMoving=='1'?true:false);
		wchar_t buffer[16];
        CStrOperation::MyItows(MhitIte->_issueyear, buffer, 16);
		pBTrackData->YearStr(buffer);
        CStrOperation::MyItows(MhitIte->_size, buffer, 16);
		pBTrackData->FileSizeStr(buffer);
		for(int j=0;j<MhitIte->_nInfoNum;j++)
		{
			CMhod* MhodIte = new CMhod();
			MhodIte->Read(pReader);

			SAVESTRUCT(newkey,_AllItem,pBTrackData,MhodIte);

			if (MhodIte->_type == MHOD_ID_TITLE)
				pBTrackData->Title(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_PATH)
				pBTrackData->Devicepath(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_ALBUM)
				pBTrackData->AlbumStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_ARTIST)
				pBTrackData->ArtistStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_GENRE)
				pBTrackData->GenreStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_FILETYPE)
				pBTrackData->FiletypeStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_EQSETTING)
				pBTrackData->EQSetting(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_COMMENT)
				pBTrackData->CommentStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_CATEGORY)
				pBTrackData->CategoryStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_COMPOSER)
				pBTrackData->ComposerStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_GROUPING)
				pBTrackData->GroupingStr(MhodIte->_pbody->GetInfo());
			else if (MhodIte->_type == MHOD_ID_DESCRIPTION)
				pBTrackData->DescriptionStr(MhodIte->_pbody->GetInfo());
		}
		alltracklist->push_back(pBTrackData);
	}
}
void CIosiPodData::ReadArtist(BinaryBufferReader *pReader)
{
	CMhli* MhliIte = new CMhli();
	MhliIte->Read(pReader);//读取列表
	
	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,MhliIte));
	_MhsdList.insert(map<int,int>::value_type(newkey,0));

	for(int i = 0;i<MhliIte->_nArtistsNum;i++ )
	{					
		CMhii* MhiiIte = new CMhii();//读取信息
		MhiiIte->Read(pReader);	

		CIosCDBTrackArtist* TrackArtist = new CIosCDBTrackArtist();
		TrackArtist->Id(MhiiIte->_nArtistid);
		TrackArtist->Dbid(MhiiIte->_nArtistDBid);

		SAVESTRUCT(newkey,_AllItem,TrackArtist,MhiiIte);

		for(int j=0;j<MhiiIte->_nTrackNum;j++)
		{
			CMhod* MhodIte = new CMhod();
			MhodIte->Read(pReader);		

			SAVESTRUCT(newkey,_AllItem,TrackArtist,MhodIte);

			if (MhodIte->_type == MHOD_ID_ALBUM_ARTIST_MHII)
				TrackArtist->Artistname(MhodIte->_pbody->GetInfo()); //这里信息还未知无法处理,以后分析清楚后可以添加进来
			//wostringstream strlog;
			//strlog <<"CMhit=" << MhodIte->_type << "|body=:" << MhodIte->_pbody->GetInfo();
			//LOG->WriteDebug(strlog.str());
		}	
		_artist.push_back(TrackArtist);
	}
}


void CIosiPodData::SaveCDBDataToFile(wstring CDBfilePath)
{
	if(_AllItem.size()==0)
					return;
	BinaryBufferWriter bodyWriter;

	if(m_Mhbd._nItemNum > 0)
	{				
		cdbBase* p;//生成body内容	
		for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
		{			
			int itype = it->second;
			p = _AllItem[it->first];//mhsd头保存位置
			p->Write(&bodyWriter);
			it++;//写列表头Mhl*
			p = _AllItem[it->first];
			p->Write(&bodyWriter);
			switch(itype)
			{
			case 1://所有歌曲
				for(size_t i=0;i<_alltracklist.size();i++)
				{
					for(size_t j=0;j<_alltracklist[i]->ItemKey.size();j++)
					{
						p=_AllItem[_alltracklist[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}
				break;
			case 4: //专辑列表
				for(size_t i=0;i<_albumlist.size();i++)
				{
					for(size_t j=0;j<_albumlist[i]->ItemKey.size();j++)
					{
						p=_AllItem[_albumlist[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}
				break;
			case 2:
				for(size_t i=0;i<_playlist1.size();i++)
				{
					for(size_t j=0;j<_playlist1[i]->ItemKey.size();j++)
					{
						p=_AllItem[_playlist1[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}
				break;
			case 3:
				for(size_t i=0;i<_playlist2.size();i++)
				{
					for(size_t j=0;j<_playlist2[i]->ItemKey.size();j++)
					{
						p=_AllItem[_playlist2[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}
				break;
			case 5://播放列表 目前只实现读取歌曲ID列表跟列表ID
				for(size_t i=0;i<_playlist3.size();i++)
				{
					for(size_t j=0;j<_playlist3[i]->ItemKey.size();j++)
					{
						p=_AllItem[_playlist3[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}
				break;
			case 8://表演者列表						
				for(size_t i=0;i<_artist.size();i++)
				{
					for(size_t j=0;j<_artist[i]->ItemKey.size();j++)
					{
						p=_AllItem[_artist[i]->ItemKey[j]];
						p->Write(&bodyWriter);
					}
				}	
				break;
			case 6:
			case 9:
			case 10:
			default://未知格式内容包含着头里面了
				break;
			}
		}
		
		int bodySize;
		const char* body = bodyWriter.GetBuffer(bodySize);

		ofstream ofstemp("d:\\iTunesPackUnCompress", ios_base::out|ios_base::binary);
		ofstemp.write(body, bodySize);
		ofstemp.close();

	
		BinaryBufferWriter writer;

		if(m_Mhbd._nIsCompress==2)
		{
			//压缩
			int compressBodySize=0;
			char * compressBody = NULL;
			__compress_itdb(body,bodySize,&compressBody,&compressBodySize);		
			m_Mhbd._nOffsetCompress  = compressBodySize + m_Mhbd._head._headSize;
			// 写入到buffer		
			m_Mhbd.Write(&writer);
			writer.WriteBuffer(compressBody, compressBodySize);
			SAFE_DELETE(compressBody);
		}
		else
		{
			m_Mhbd._nOffsetCompress  = bodySize + m_Mhbd._head._headSize;
			m_Mhbd.Write(&writer);
			writer.WriteBuffer(body, bodySize);
		}
		

		int fileSize;
		const char* file = writer.GetBuffer(fileSize);

		if(m_Mhbd.hashtype[0]<3)
			HashVerify::MakeCheckSumiTunesCDB( (void*)file, fileSize,SrcParam1,SrcParam);

		// 写到文件
		ofstream ofs(CDBfilePath.c_str(), ios_base::out|ios_base::binary);
		ofs.write(file, fileSize);
		ofs.close();

		USES_CONVERSION;
		if(m_Mhbd.hashtype[0]==3)
		{
			_hashab.ReSetiTunesCDB(NULL,W2A(CDBfilePath.c_str()));


			if(iPodFileSync::GetDBVer()<500)
			{
				wstring cdbPath = CFileOperation::GetFileDir(CDBfilePath);
				wstring LocalDB = cdbPath + L"iTunes Library.itlp\\Locations.itdb";				
				wstring LocalCBK = LocalDB + L".cbk";
				int nIndex = cdbPath.find_last_of(L"\\iTunes\\");
				wstring conpath = cdbPath.substr(0, nIndex - 6);
				if(HashVerify::checkSqlDb(conpath,3) == S_OK)
					_hashab.CaclLocationCDK(NULL,W2A(LocalCBK.c_str()));
			}
		}
		
	}
}

//是否采用压缩模式
int CIosiPodData::zlib_inflate( unsigned char * outbuf, const unsigned char *zdata, unsigned int compressed_size, unsigned int * uncompressed_size )
{
	int ret;
	unsigned int inpos = 0;
	unsigned int outpos = 0;
	unsigned have;
	z_stream strm;
	unsigned char out[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	*uncompressed_size = 0;

	/* decompress until deflate stream ends or end of file */
	do 
	{
		strm.avail_in = CHUNK;

		if( inpos+strm.avail_in > compressed_size )
		{
			strm.avail_in = compressed_size - inpos;
		}

		strm.next_in = (unsigned char*)zdata+inpos;
		inpos+=strm.avail_in;

		/* run inflate() on input until output buffer not full */
		do
		{
			strm.avail_out = CHUNK;
			if( outbuf )
			{
				strm.next_out = (Bytef*)(outbuf + outpos);
			}
			else
			{
				strm.next_out = out;
			}

			ret = inflate( &strm, Z_NO_FLUSH );            
			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNK - strm.avail_out;
			*uncompressed_size += have;

			if (outbuf)
			{
				outpos += have;
			}
		}while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void  CIosiPodData::__uncompress_itdb( const char * pSrc, int iSrcSize, char ** ppDest, int * piDestSize )
{
	char * p = NULL;
	int iSize;

	if( zlib_inflate( NULL, (unsigned char*)pSrc, iSrcSize, (unsigned int*)&iSize ) == 0 )
	{
		p = (char*)malloc(iSize);
		if( p )
		{
			if( zlib_inflate( (unsigned char*)p, (unsigned char*)pSrc, iSrcSize, (unsigned int*)&iSize ) == 0 )
			{
				*ppDest = p;
				*piDestSize = iSize;
				return;
			}
		}
	}
	if( p )
		free( p );

}

void CIosiPodData::__compress_itdb(const char* srcbody,int size,char** ppDes,int * retsize)
{
	ULONG compressed_len = compressBound (size);
	char *p =(char*)malloc(compressed_len);
	int status = compress2((Bytef*)p,&compressed_len,(const Bytef*)srcbody,size,1);
	if(status == Z_OK)
	{
		*retsize = compressed_len;
		*ppDes = p;
	}
	retsize = 0;		
}

CIosCDBTrackData* CIosiPodData::InsertTrack(wstring filename,IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb)
{	
	USES_CONVERSION;		
		//专辑表演者ID
		int ArtistID=0;
		if(pMP3Data->PropertyGetMusicPerformer().length() > 0)
			ArtistID = FindArtistID(pMP3Data->PropertyGetMusicPerformer(),pstudb);
		else
			ArtistID = FindArtistID(L"Unknow Album",pstudb);
		//专辑的ID
		int AlbumID = 0;
		if(pMP3Data->PropertyGetAlbumName().length() > 0)
			AlbumID = FindAlbumID(pMP3Data->PropertyGetAlbumName(),pstudb);
		else
			AlbumID = FindAlbumID(L"Unknow Artist",pstudb);

		int ComposerId=0;
		if(pMP3Data->PropertyGetComposer().length()>0)
			ComposerId = FindComposerID(pMP3Data->PropertyGetComposer(),pstudb);
		else
			AlbumID = FindAlbumID(L"Unknow Composer",pstudb);

		//播放列表增加一个对象
		CIosCDBTrackData * pTrack = new CIosCDBTrackData();
		pTrack->Title(pMP3Data->PropertyGetMusicName());		
		if(pTrack->Title().length()==0)
		{
			pTrack->Title(CFileOperation::GetFileName(filename));
		}
		pTrack->Dbid(pstudb->TrackID);//新增ID号
		pTrack->Dbid2(0);//新增ID号
		pTrack->Id(Create32BitID());//新增ID号
		//pTrack->Devicepath()//用不到
		pTrack->HasArtWork(pMP3Data->GetSelectWorks());
		pTrack->Isvisable(true);
		pTrack->Lryic(pMP3Data->PropertyGetLyrics().length() > 0?true:false);
		pTrack->Pcpath(filename);
		pTrack->Timer(pMP3Data->GetPlayTime());
		wstring filtype = filename.substr(filename.length()-3);		
		pTrack->Type(filtype + L" ");
		pTrack->Albumid(AlbumID);
		pTrack->Artist(ArtistID);	
		pTrack->Composer(ComposerId);
		pTrack->Devicepath(pstudb->DevFilePath);
		pTrack->FiletypeStr(pMP3Data->GetFormat());
		pTrack->AlbumStr(pMP3Data->PropertyGetAlbumName());
		pTrack->ArtistStr(pMP3Data->PropertyGetMusicPerformer());
		pTrack->AlbumArtistStr(pMP3Data->PropertyGetAlbumPerformer());
		if(pTrack->AlbumArtistStr().length()==0)
				pTrack->AlbumArtistStr(pMP3Data->PropertyGetMusicPerformer());
		wchar_t buffer[16];
        CStrOperation::MyItows(pMP3Data->GetFileSize(), buffer, 16);
		pTrack->FileSizeStr(buffer);
		pTrack->YearStr(pMP3Data->PropertyGetYear());
		wstring fileExt = CFileOperation::GetFileExt(pMP3Data->GetLocalFileName());
		fileExt = CStrOperation::toUpperW(fileExt);
		if(fileExt == L"MP3" || fileExt == L"M4a")
						pTrack->IsVideo(false);
		else
						pTrack->IsVideo(true);
		//增加一个用于保存到CDB文件的Struct对象
		CMhit * pmhit = new CMhit();
		//memset(pmhit,0,sizeof(pmhit));
		memcpy(pmhit->_head._name,"mhit",4);
		pmhit->_head._headSize = 584;
		pmhit->_nBodylen = pmhit->_head._headSize;	//长度需要计算							
		pmhit->_ntrackid = pTrack->Id();						//album id，是iTunes内部ID 
		pmhit->_nIsHide=1;
		//memcpy(pmhit->_nFileType , CCodeOperation::UnicodeToUTF_8(pTrack->Type()).c_str(),4);					//track的文件类型标示
		
		fileExt+=L" ";
		char temp[5];
		memcpy(temp,W2A(fileExt.c_str()),4);	
		temp[4]=0;
		char Filetype[5];
		memset(Filetype,0,sizeof(Filetype));
		for(int i=0;i<4;i++)//" 3PM"
		{
			Filetype[3-i]=	temp[i];
		}
		memcpy(pmhit->_nFileType,Filetype,4);
		pmhit->_type1=0;
		pmhit->_type2=1;
		pmhit->_compilation=0;
		pmhit->_rating=pMP3Data->GetiBitSpeed();
		time_t now_time;
		time(&now_time);
		CDealTime date_time(now_time);
		pmhit->_timeMac= date_time.GetTime();//-CDealTime::IosStartTime().GetTime();										//MAC时间戳
		pmhit->_size = pMP3Data->GetFileSize();					//大小
		if(pmhit->_size == 0)
			pmhit->_size = CFileOperation::GetFileSize(W2A(filename.c_str()));
		pmhit->_longer = pMP3Data->GetPlayTime();				//歌曲时长 单位
		if(pmhit->_longer==0)
			pmhit->_longer = pMP3Data->GetMediaDuration();
		pmhit->_nIdInAlbuum=0;									//专辑中的编号		 需要从数据库中提取
		pmhit->_nAlbumtrackNum=0;								//所属专辑的歌曲总数 需要从数据库中提取
		pmhit->_issueyear = atoi(W2A(pMP3Data->PropertyGetYear().c_str()));			//发行年份
		pmhit->_bytespeed = pMP3Data->GetiBitSpeed();			//比特率
		pmhit->_nSamplingrate = pMP3Data->GetSampleRate();		//采样率
		pmhit->_volume = pMP3Data->GetVolumeAdjustment();		//音量		
		pmhit->_begintime=0;
		pmhit->_endtime=0;
		pmhit->_teststring=0;
		pmhit->_playcount=0;
		pmhit->_playcount2=0;
		pmhit->_lastplaytime=0;
		pmhit->_cdid = atoi(W2A(pMP3Data->PropertyGetTrackNumber().c_str()));		//cd编号
		pmhit->_cdNum = atoi(W2A(pMP3Data->PropertyGetCDNumber().c_str()));		//CD的个数		
		pmhit->_nDRMID=0;
		pmhit->_nAddTime= date_time.GetTime();//-CDealTime::IosStartTime().GetTime();
		pmhit->_nBoolmarkTime=0;
		pmhit->_ndbID = pTrack->Dbid();	
		pmhit->_isSelect=0;
		pmhit->_playerSpeed=0;
		memcpy(pmhit->_bpm ,pMP3Data->PropertyGetBPM().c_str(),2);				//每分钟节拍数
		//memset(pmhit->_artworkcount,0,sizeof(pmhit->_artworkcount));			//插图数目
		__int16 artworkcount =  pstudb->ArtWorkID==0?1:2;
		memcpy(pmhit->_artworkcount,&artworkcount,sizeof(__int16));
		pmhit->artworksize = 0;													//插图大小
		pmhit->_samplerate=pMP3Data->GetSampleRate();
		pmhit->_releasetime=0;
		memset(pmhit->_detailflag,0,sizeof(pmhit->_detailflag));				//详述标志
		pmhit->_skipcount=0;
		pmhit->_lastskipped=0;
		pmhit->_hasartwork = pstudb->ArtWorkID==0?2:1;
		pmhit->_skipwhenshuffing=2;
		pmhit->_rememberplaybackposition=0;
		pmhit->flag4 = 0;
		pmhit->_dbid2 = pTrack->Dbid();
		pmhit->_haslyric = pMP3Data->PropertyGetLyrics().length() > 0?1:0;	//歌词标志	
		pmhit->_isMoving=pTrack->IsVideo()?1:0;
		pmhit->_cannotplay=0x0d;
		pmhit->_pregap=528;//typenum
		pmhit->_usecount=9204552;//typenum
		pmhit->_postgap=600;//typenum
		if(!pTrack->IsVideo())
			pmhit->_mediaType = 0x00000001;
		else
			pmhit->_mediaType = 0x00000002;
		pmhit->_seasonnr=0;
		pmhit->_episodenr=0;
		for(int i=0;i<7;i++)
		{
			pmhit->_unknowArray[i]=0;
		}
		pmhit->gaplessdata=0;
		memset(pmhit->_gaplessTrackFlag,1,1);	
		memset(pmhit->_gaplessTrackFlag+1,0,1);	

		memset(pmhit->_gaplessAlbumFlag,0,sizeof(pmhit->_gaplessAlbumFlag));
		memset(pmhit->_keep,0,sizeof(pmhit->_keep));
		pmhit->_alumbID = AlbumID;
		pmhit->_ID = -1310291365236195271;//unknow
		pmhit->_size2 = pmhit->_size;										//track大小 
		memset(pmhit->_keep1,0,sizeof(pmhit->_keep1));
		pmhit->_mhiiLink=pstudb->ArtWorkID;
		pmhit->_composerId=ComposerId;										//跟数据库关联，作曲者ID
		memset(pmhit->_keep2,0,sizeof(pmhit->_keep2));
		pmhit->_artistid = ArtistID;
		memset(pmhit->_unknow,0xffff,sizeof(pmhit->_unknow));
		memset(pmhit->_unknow2,0,sizeof(pmhit->_unknow2));
		memset(pmhit->_unknow3,12,1);
		memset(pmhit->_unknow3+1,0,1);
		memset(pmhit->_unknow4,0,sizeof(pmhit->_unknow4));
		pmhit->_unknow5=0;
		memset(pmhit->_unknow6,0,sizeof(pmhit->_unknow6));
		memset(pmhit->_unknow7,0,sizeof(pmhit->_unknow7));
		memset(pmhit->_unknow8,3,1);
		memset(pmhit->_unknow8+1,0,2);
		memset(pmhit->_unknow8+3,2,1);
		pmhit->_unknow9=0;
		pmhit->_unknow10=0;
		memset(pmhit->_unknow11,0x80,6);
		memset(pmhit->_unknow11+6,0,2);
		memset(pmhit->_unknow12,0,sizeof(pmhit->_unknow12));
		memset(pmhit->_unknow13,0,sizeof(pmhit->_unknow13));
		memset(pmhit->_unknow14,0,sizeof(pmhit->_unknow14));
		pmhit->_unknow15=0;
		pmhit->_unknow16=32;
		pmhit->_unknow17=0;
		
		//保存内容
		int newkey = _AllItem.size();		
		int nInfoNum = 0;
		wstring TrackTitle = pTrack->Title();
		CStrOperation::ReplaceW(TrackTitle,L".",L"");
		CMhod* mhodTitle = AddMhod(MHOD_ID_TITLE,TrackTitle);
		nInfoNum++;
 		pmhit->_nBodylen += mhodTitle->_nBodylen;		

		CMhod* mhodFiletypeStr = NULL;
		if(!pTrack->IsVideo())
			mhodFiletypeStr = AddMhod(MHOD_ID_FILETYPE,L"MPEG Audio File");//pTrack->FiletypeStr());
		else
			mhodFiletypeStr = AddMhod(MHOD_ID_FILETYPE,L"MPEG4 Video File");
		nInfoNum++;
		pmhit->_nBodylen += mhodFiletypeStr->_nBodylen;

		wstring devpath = L":iTunes_Control:Music:" + pTrack->Devicepath();
		CStrOperation::ReplaceW(devpath,L"/",L":");
 		CMhod* mhodDevicepath = AddMhod(MHOD_ID_PATH,devpath);
 		nInfoNum++;
  		pmhit->_nBodylen += mhodDevicepath->_nBodylen;
//  		CMhod* mhodAlbumStr = AddMhod(MHOD_ID_ALBUM,pTrack->AlbumStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodAlbumStr->_nBodylen;
//  		CMhod* mhodArtistStr = AddMhod( MHOD_ID_ARTIST,pTrack->ArtistStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodArtistStr->_nBodylen;
//  		CMhod* mhodGenreStr = AddMhod(MHOD_ID_GENRE,pTrack->GenreStr());
//  		nInfoNum++;
// 		pmhit->_nBodylen += mhodGenreStr->_nBodylen;
//  		CMhod* mhodEQSetting = AddMhod(MHOD_ID_EQSETTING,pTrack->EQSetting());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodEQSetting->_nBodylen;
//  		CMhod* mhodCommentStr = AddMhod(MHOD_ID_COMMENT,pTrack->CommentStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodCommentStr->_nBodylen;
//  		CMhod* mhodCategoryStr = AddMhod(MHOD_ID_CATEGORY,pTrack->CategoryStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodCategoryStr->_nBodylen;
//  		CMhod* mhodComposerStr = AddMhod(MHOD_ID_COMPOSER,pTrack->ComposerStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodComposerStr->_nBodylen;
//  		CMhod* mhodGroupingStr = AddMhod(MHOD_ID_GROUPING,pTrack->GroupingStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodGroupingStr->_nBodylen;
//  		CMhod* mhodDescriptionStr = AddMhod(MHOD_ID_DESCRIPTION,pTrack->DescriptionStr());
//  		nInfoNum++;
//  		pmhit->_nBodylen += mhodDescriptionStr->_nBodylen;

		CMhod* MhodNameSort=NULL;
		if(pMP3Data->PropertyGetNameSort().length() >0)
		{		
				MhodNameSort=AddMhod(MHOD_ID_SORT_TITLE,pMP3Data->PropertyGetNameSort());	
				nInfoNum++;
				pmhit->_nBodylen += MhodNameSort->_nBodylen;	
		}
		CMhod* MhodAlbumSort=NULL;
		if(pMP3Data->PropertyGetAlbumSort().length()>0)
		{
				MhodAlbumSort=AddMhod(MHOD_ID_SORT_ALBUM,pMP3Data->PropertyGetAlbumSort());
				nInfoNum++;
				pmhit->_nBodylen += MhodAlbumSort->_nBodylen;
		}
		CMhod* MhodPerformerSort=NULL;
		if(pMP3Data->PropertyGetPerformerSort().length()>0)
		{
				MhodPerformerSort=AddMhod(MHOD_ID_SORT_ALBUMARTIST,pMP3Data->PropertyGetPerformerSort());
				nInfoNum++;
				pmhit->_nBodylen += MhodPerformerSort->_nBodylen;
		}
		CMhod* MhodAlbumPerformerSort=NULL;
		if(pMP3Data->PropertyGetAlbumPerformerSort().length()>0)
		{
				MhodAlbumPerformerSort=AddMhod(MHOD_ID_ALBUM_SORT_ARTIST,pMP3Data->PropertyGetAlbumPerformerSort());
				nInfoNum++;
				pmhit->_nBodylen += MhodAlbumPerformerSort->_nBodylen;
		}
		CMhod* MhodtComposerSor=NULL;
		if(pMP3Data->PropertyGetComposerSort().length()>0)
		{
				MhodtComposerSor=AddMhod(MHOD_ID_SORT_COMPOSER,pMP3Data->PropertyGetComposerSort());
				nInfoNum++;
				pmhit->_nBodylen += MhodtComposerSor->_nBodylen;
		}		
		
		pmhit->_nInfoNum=nInfoNum;	

 		_AllItem.insert(map<int, cdbBase*>::value_type(newkey,pmhit));
 		pTrack->ItemKey.push_back(newkey);
 
 		SAVESTRUCT(newkey,_AllItem,pTrack,mhodTitle);
		SAVESTRUCT(newkey,_AllItem,pTrack,mhodFiletypeStr);
 		SAVESTRUCT(newkey,_AllItem,pTrack,mhodDevicepath);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodAlbumStr);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodArtistStr);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodGenreStr); 		
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodEQSetting);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodCommentStr);		
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodCategoryStr);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodComposerStr);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodGroupingStr);
//   		SAVESTRUCT(newkey,_AllItem,pTrack,mhodDescriptionStr);
 		if(MhodNameSort)
 			SAVESTRUCT(newkey,_AllItem,pTrack,MhodNameSort);
 		if(MhodAlbumSort)
 			SAVESTRUCT(newkey,_AllItem,pTrack,MhodAlbumSort);
 		if(MhodPerformerSort)
 			SAVESTRUCT(newkey,_AllItem,pTrack,MhodPerformerSort);
 		if(MhodAlbumPerformerSort)
 			SAVESTRUCT(newkey,_AllItem,pTrack,MhodAlbumPerformerSort);
 		if(MhodtComposerSor)
 			SAVESTRUCT(newkey,_AllItem,pTrack,MhodtComposerSor); 		
 
 		//增加了新内容MHSD头的数据包长度字段要重新计算
 		for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
 		{
 			if(it->second == 1)
 			{
 				CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
 				pMhsd->_nBodylen += pmhit->_nBodylen; //长度变了
 				it++;
 				CMhlt * pMhlt = (CMhlt*)_AllItem[it->first];
 				pMhlt->_nTracksNum++; //列表里面歌曲增加一行
 				break;
 			}
 		}		
 		
 		_alltracklist.push_back(pTrack);
		return pTrack;

}
void CIosiPodData::AddPlayList(wstring playListName,__int64 pid)
{
	CIosCDBPlayList* PlayList = new CIosCDBPlayList();
	PlayList->ID(pid);
	PlayList->Listname(playListName);

	CMhyp* Addmhyp = new CMhyp();
	//memset(Addmhyp,0,sizeof(Addmhyp));
	memcpy(Addmhyp->_head._name,"mhyp",4);
	Addmhyp->_head._headSize = 184;
	//Addmhyp->_mhodcount =3;
	Addmhyp->_nmhips=0;
	Addmhyp->_playlistID = pid;
	Addmhyp->_type = 0;
	Addmhyp->_nMhodNum = 1;
	time_t now_time;
	time(&now_time);
	CDealTime date_time(now_time);
	Addmhyp->_timeMAC = date_time.GetTime()-CDealTime::IosStartTime().GetTime();	
	Addmhyp->_nBodylen =184;

	int newkey = _AllItem.size();	
	CMhod* mhodTitle = AddMhod(MHOD_ID_TITLE,playListName);
	
	Addmhyp->_nBodylen += mhodTitle->_nBodylen;

	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,Addmhyp));
	PlayList->ItemKey.push_back(newkey);

	SAVESTRUCT(newkey,_AllItem,PlayList,mhodTitle);

	//增加了新内容MHSD头的数据包长度字段要重新计算
	for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
	{
		if(it->second == 3)
		{
			CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
			pMhsd->_nBodylen += Addmhyp->_nBodylen; //长度变了
			it++;
			CMhlp * pMhlp = (CMhlp*)_AllItem[it->first];
			pMhlp->_nMhodNum++; //列表里面歌曲增加一行
			break;
		}
	}		
	_playlist2.push_back(PlayList);
	_playlistDB.push_back(PlayList);
}
void CIosiPodData::RemoveTrackFromPlayList(__int64 PlayListID,__int64 trackID)
{
	for(vector<CIosCDBPlayList*>::iterator pPlayListitem = _playlist2.begin();pPlayListitem!=_playlist2.end();pPlayListitem++)
	{
		if(PlayListID == (*pPlayListitem)->ID())
		{			
			//第0个肯定是mhyp
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[0]);
			CMhyp* Mhypite = (CMhyp*)iterAllItem->second;
			int MhypModNum = Mhypite->_nMhodNum;
			int seachPos =MhypModNum+1;//跳过yp的mod字段
			int bodylen=0;
			int FinSort=-1;
			for(int mips = 0;mips< Mhypite->_nmhips;mips++)
			{
				iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[seachPos]);//寻找mhip字段
				CMhip* pMhip = (CMhip*)iterAllItem->second;
				if(pMhip->_Trackid == trackID)//找到
				{
					bodylen = pMhip->_nBodylen;
					for(int MhipModNum=0;MhipModNum<pMhip->_nMhodNum;pMhip++)//移除mhip的mod
					{
						map<int,cdbBase*>::iterator iterMhipMhod = _AllItem.find((*pPlayListitem)->ItemKey[seachPos+MhipModNum+1]);
						SAFE_DELETE(iterMhipMhod->second);
						_AllItem.erase(iterMhipMhod);
						(*pPlayListitem)->ItemKey.erase((*pPlayListitem)->ItemKey.begin()+seachPos+MhipModNum+1);
					}
					delete pMhip;
					_AllItem.erase(iterAllItem);//移除mhip
					(*pPlayListitem)->ItemKey.erase((*pPlayListitem)->ItemKey.begin()+seachPos);
					FinSort = mips;
					break;					
				}
				seachPos += pMhip->_nMhodNum+1;//1即是mhip				

			}
			if(FinSort>-1)
			{			
				Mhypite->_nBodylen-= bodylen;
				Mhypite->_nmhips--;
				//(*pPlayListitem)->ItemKey.erase((vector<int>::iterator)(*pPlayListitem)->ItemKey.at(FinSort));
				(*pPlayListitem)->EraseTrackid(FinSort);
			}
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 3)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen -= bodylen;
					it++;
					//CMhlp *pMhlp = (CMhlp*)_AllItem[it->first];
					//pMhlp->_nMhodNum--;
					break;
				}
			}
			//SAFE_DELETE(*pPlayListitem); 			
			//_playlist2.erase(pPlayListitem);				
			break;
		}		
	}	
}

void CIosiPodData::AddTrackToPlayList(__int64 PlayListID,__int64 trackID)
{
	for(vector<CIosCDBPlayList*>::iterator pPlayListitem = _playlist2.begin();pPlayListitem!=_playlist2.end();pPlayListitem++)
	{
		if(PlayListID == (*pPlayListitem)->ID())
		{			
			//第0个肯定是mhyp
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[0]);
			CMhyp* Mhypite = (CMhyp*)iterAllItem->second;
						
			CMhip* pMhip= new CMhip();
			//memset(pMhip,0,sizeof(pMhip));
			memcpy(pMhip->_head._name,"mhip",4);
			pMhip->_head._headSize = 76;
			pMhip->_nMhodNum=0;
			ZeroMemory(pMhip->_unknow,8);
			time_t now_time;
			time(&now_time);
			CDealTime date_time(now_time);
			pMhip->_timeMAC= date_time.GetTime()-CDealTime::IosStartTime().GetTime();	
			pMhip->_nBodylen=pMhip->_head._headSize;
			pMhip->_Trackid=GetTrackIdByPid(trackID);
			pMhip->_Podcastgroupflag=0;
			pMhip->_Podcastgroupref=0;
			pMhip->_Podcastgroupid=pMhip->_Trackid+10086;//not same to Trackid 
			__int64 id = trackID+10086;
			memcpy(pMhip->_unknow1,(char*)&id,8);

		//	map<int,cdbBase*>::iterator iterAllItem1 = _AllItem.find((*pPlayListitem)->ItemKey[5]);
		//	CMhod* mhod = (CMhod*)iterAllItem1->second;
		//	pMhip->_nBodylen+=mhod->_nBodylen;

			int newkey = _AllItem.size();	
			_AllItem.insert(map<int, cdbBase*>::value_type(newkey,pMhip));
			(*pPlayListitem)->ItemKey.push_back(newkey);
			(*pPlayListitem)->PushTrackid(trackID);

		//	SAVESTRUCT(newkey,_AllItem,(*pPlayListitem),mhod);

				Mhypite->_nBodylen+= pMhip->_nBodylen;
				Mhypite->_nmhips++;
			
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 3)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen += pMhip->_nBodylen;			
					break;
				}
			}	
			break;
		}		
	}	
}
void CIosiPodData::ModifyPlayList(__int64 PlayListID,wstring NewName)
{
	for(vector<CIosCDBPlayList*>::iterator pPlayListitem = _playlist2.begin();pPlayListitem!=_playlist2.end();pPlayListitem++)
	{
		if(PlayListID == (*pPlayListitem)->ID())
		{			
			//第0个肯定是mhyp
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[0]);
			CMhyp* Mhypite = (CMhyp*)iterAllItem->second;
			//数据包大小的变化
			int size_change=0;

			for(int posTrack= 1; posTrack<Mhypite->_nMhodNum;posTrack++)
			{
				iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[posTrack]);
				CMhod* pMhod = (CMhod*)iterAllItem->second;
				int change=0;
				switch(pMhod->_type)
				{
				case MHOD_ID_TITLE://修改标题
					change = ChangeMhodBody(pMhod,NewName);
					break;				
				}
				size_change += change;					
			}		
			Mhypite->_nBodylen+=size_change;
			(*pPlayListitem)->Listname(NewName);
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 3)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen += size_change;			
					break;
				}
			}	
			break;
		}		
	}	

}
CIosCDBTrackData* CIosiPodData::FindTrackDataByID64(__int64 pid)
{
	vector<CIosCDBTrackData*>::iterator iter;
	iter = find_if(_alltracklist.begin(),_alltracklist.end(),TrackIDEqu(pid));
	return *iter;
}
int CIosiPodData::GetTrackIdByPid(__int64 PID)
{
// 	for(size_t i=0;i<_alltracklist.size();i++)
// 	{
// 		CIosCDBTrackData* pTrackData =_alltracklist.at(i);
// 		if(pTrackData->Dbid() == PID)
// 			return pTrackData->Id();
// 	}
	vector<CIosCDBTrackData*>::iterator iter;
	iter = find_if(_alltracklist.begin(),_alltracklist.end(),TrackIDEqu(PID));
	if(*iter!=NULL)
		return (*iter)->Id();
	return 0;
}
void CIosiPodData::RemovePlayList(__int64 PlayListID)
{
	for(vector<CIosCDBPlayList*>::iterator pPlayListitem = _playlist2.begin();pPlayListitem!=_playlist2.end();pPlayListitem++)
	{

		if(PlayListID == (*pPlayListitem)->ID())
		{
			//第0个肯定是mhyp
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[0]);
			CMhyp* Mhypite = (CMhyp*)iterAllItem->second;
			int bodylen = Mhypite->_nBodylen;
			SAFE_DELETE(Mhypite);
			_AllItem.erase(iterAllItem);
			for(size_t posTrack= 1; posTrack<(*pPlayListitem)->ItemKey.size();posTrack++)
			{
				iterAllItem = _AllItem.find((*pPlayListitem)->ItemKey[posTrack]);
				SAFE_DELETE(iterAllItem->second);
				_AllItem.erase(iterAllItem);
			}				
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 3)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen -= bodylen;
					it++;
					CMhlp *pMhlp = (CMhlp*)_AllItem[it->first];
					pMhlp->_nMhodNum--;
					break;
				}
			}
			SAFE_DELETE(*pPlayListitem); 			
			_playlist2.erase(pPlayListitem);				
			break;
		}		
	}	
}
int CIosiPodData::FindAlbumID(wstring AlbumName,STU_CREATE_DBID * pstudb)
{
	for(size_t i = 0; i < _albumlist.size(); ++i)
	{
		if(_albumlist[i]->AlbumName() == AlbumName)
		{
			return _albumlist[i]->Id();
		}
	}
	return CreateNewAlbum(AlbumName,pstudb);
}
int CIosiPodData::FindComposerID(wstring ComposerName,STU_CREATE_DBID * pstudb)
{
	return 0;//作曲者列表还没有信息可以操作
}
int CIosiPodData::FindArtistID(wstring ArtistName,STU_CREATE_DBID * pstudb)
{
	for(size_t i = 0; i < _artist.size(); ++i)
	{
		if(_artist[i]->Artistname() == ArtistName)
		{
			return _artist[i]->Id();
		}
	}

	return CreateNewArtist(ArtistName,pstudb);
}

CMhod* CIosiPodData::AddMhod(int type,wstring body)
{
	CMhod* pMhod = new CMhod();
	if(type == MHOD_ID_SPLPREF || type == MHOD_ID_SPLRULES  || type == MHOD_ID_PLAYLIST || type == 102)
	{
		return NULL;//_pbody= new CMhNodyUnknow(_type,) //文档不完整无法完成此类信息添加
	}
	else 
	{
		int bodylen = 0;
		CMhodBody * MhodBody = NULL;
		switch(type)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 200:
		case 201:
		case 202:
		case 203:
		case 204:
		case 300:
		case 301:
		case 302:
		case 303:
				MhodBody = new CMhodBody(type);
				MhodBody->_Code =1;
				MhodBody->_size = body.length()*2;//多字节要双倍
				MhodBody->_unknow=1;
				MhodBody->_unknow1=0;
				MhodBody->_value = body;
				pMhod->_pbody = MhodBody;
				bodylen = 40 + MhodBody->_size;
				break;
		case 15:
		case 16:

		case 17:

		case 50:

		case 51:

		case 52:

		case 100:

		case 102:

				//todo:继续添加其他类型
		default:
			bodylen = 0;
			return NULL;//文档不完整无法完成此类信息添加
		}
		
		memcpy(pMhod->_head._name,"mhod",4);
		pMhod->_head._headSize = 24;
		pMhod->_type = type;		
		pMhod->_nBodylen = bodylen;
	}
	return pMhod;
}

__int32 CIosiPodData::Create32BitID()
{
	__int32 r =(rand()<<17)|(rand()<<3)|(rand());
	return r;
}

void CIosiPodData::ModifyTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID * pstudb)
{
	if(!ptrackData)
		return;

	for(vector<CIosCDBTrackData*>::iterator pTrackitem = _alltracklist.begin();pTrackitem!=_alltracklist.end();pTrackitem++)
	{
		if(ptrackData->Id() == (*pTrackitem)->Id())
		{
			//第0个肯定是mhit
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pTrackitem)->ItemKey[0]);
			CMhit* Mhitite = (CMhit*)iterAllItem->second;
			//数据包大小的变化
			int size_change=0;			
		    Mhitite->_alumbID = FindAlbumID(ptrackData->AlbumStr(),pstudb);//包头替换掉修改专辑id
			if(Mhitite->_alumbID != ptrackData->Albumid())//传进来的TrackData包含的各种ID还是旧的值，在这里替换掉
			{
				//DeleteAlbum(ptrackData->Albumid());
				ptrackData->Albumid(Mhitite->_alumbID);
			}
			Mhitite->_artistid = FindArtistID(ptrackData->ArtistStr(),pstudb);//替换表演者id
			if(Mhitite->_artistid != ptrackData->Artist())
			{
				//DeleteArtist(ptrackData->Artist());
				ptrackData->Artist(Mhitite->_artistid);				
			}
			//Mhitite->_composerId = FindComposerID(ptrackData->ComposerStr(),pstudb);
			//if(Mhitite->_composerId != ptrackData->Composer())
			//{
			//	ptrackData->Composer(Mhitite->_composerId);
			//}
			Mhitite->_hasartwork = ptrackData->HasArtWork()?1:0;		
			Mhitite->_haslyric = ptrackData->Lryic()?1:0;
			Mhitite->_nIsHide = ptrackData->Isvisable()?1:0;
			
			for(size_t posTrack= 1; posTrack<(*pTrackitem)->ItemKey.size();posTrack++)
			{
				iterAllItem = _AllItem.find((*pTrackitem)->ItemKey[posTrack]);
				CMhod* pMhod = (CMhod*)iterAllItem->second;
				int change=0;
				switch(pMhod->_type)
				{
				case MHOD_ID_TITLE://修改标题
					change = ChangeMhodBody(pMhod,ptrackData->Title());
					break;
				case MHOD_ID_ALBUM:
					change = ChangeMhodBody(pMhod,ptrackData->AlbumStr());
					break;
				case MHOD_ID_ARTIST:
					change = ChangeMhodBody(pMhod,ptrackData->ArtistStr());
					break;
				case MHOD_ID_COMPOSER:
					change = ChangeMhodBody(pMhod,ptrackData->ComposerStr());
					break;
				case MHOD_ID_GENRE:
					change = ChangeMhodBody(pMhod,ptrackData->GenreStr());
					break;
				case MHOD_ID_COMMENT:
					change = ChangeMhodBody(pMhod,ptrackData->CommentStr());
					break;
				}
				size_change += change;					
			}				
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 1)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen += size_change;					
					break;
				}
			}
			Mhitite->_nBodylen += size_change;			
			break;
		}		
	}	

}
//修改Mhody内容
int CIosiPodData::ChangeMhodBody(CMhod* pMhod,wstring Newvalue)
{
	int change = 0;
	CMhodBody* pMhodBody = (CMhodBody*)pMhod->_pbody;
	if(pMhodBody->_value != Newvalue)
	{
		pMhodBody->_value = Newvalue;
		change = Newvalue.length()*2 - pMhodBody->_size;					
		pMhodBody->_size  = Newvalue.length()*2;
		pMhod->_nBodylen += change;
	}
	return change;
}
void CIosiPodData::RemoveTrack(CIosCDBTrackData * ptrackData)
{
	if(!ptrackData)
		return;
	int countArt = 0;	//艺术家歌曲数
	int countAlbum = 0; //专辑歌曲数
	int BodySizeChange=0;//总的数据大小改变数
	
	//确定所有歌曲里面要删除的歌曲对应的专辑跟表演者有几条相同记录
	for(size_t i = 0;i<_alltracklist.size();i++)
	{
		CIosCDBTrackData* pTrack = _alltracklist[i];
		if(pTrack->Artist() == ptrackData->Artist() )
		{
			countArt++;			
		}
		if(pTrack->Albumid() == ptrackData->Albumid())
		{
			countAlbum++;			
		}		
	}

	//如果只有一个歌曲有这个艺术家记录的话需要删除此艺术家
	if(countArt == 1)
	{
		DeleteArtist(ptrackData->Artist());
	}
	//清理专辑
	if(countAlbum == 1)
	{
		DeleteAlbum(ptrackData->Albumid());
	}
	//清理歌曲列表数据
	DeleteTrack(ptrackData->Id());
}
void CIosiPodData::DeleteAlbum(int Albumid)
{
	//检查专辑列表确定是否需要删除
	for(vector<CIosCDBAlbumData*>::iterator pAlbum = _albumlist.begin();pAlbum!=_albumlist.end();pAlbum++)
	{		

		if(Albumid == (*pAlbum)->Id())
		{
			//第0个肯定是mhia
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pAlbum)->ItemKey[0]);
			CMhia* MhiaIte = (CMhia*)iterAllItem->second;
			int bodylen = MhiaIte->_nBodylen;
			SAFE_DELETE(MhiaIte);
			_AllItem.erase(iterAllItem);
			for(size_t posAlbum= 1; posAlbum<(*pAlbum)->ItemKey.size();posAlbum++)
			{
				iterAllItem = _AllItem.find((*pAlbum)->ItemKey[posAlbum]);
				SAFE_DELETE(iterAllItem->second);
				_AllItem.erase(iterAllItem);
			}				
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 4)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen -= bodylen;
					it++;
					CMhla *pMhla = (CMhla*)_AllItem[it->first];
					pMhla->_nAlbumsNum--;
					break;
				}
			}
			SAFE_DELETE((*pAlbum));
			_albumlist.erase(pAlbum);				
			break;
		}		
	}
}

void CIosiPodData::DeleteTrack(int trackId)
{
	for(vector<CIosCDBTrackData*>::iterator pTrackitem = _alltracklist.begin();pTrackitem!=_alltracklist.end();pTrackitem++)
	{

		if(trackId == (*pTrackitem)->Id())
		{
			//第0个肯定是mhia
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pTrackitem)->ItemKey[0]);
			CMhit* Mhitite = (CMhit*)iterAllItem->second;
			int bodylen = Mhitite->_nBodylen;
			SAFE_DELETE(Mhitite);
			_AllItem.erase(iterAllItem);
			for(size_t posTrack= 1; posTrack<(*pTrackitem)->ItemKey.size();posTrack++)
			{
				iterAllItem = _AllItem.find((*pTrackitem)->ItemKey[posTrack]);
				SAFE_DELETE(iterAllItem->second);
				_AllItem.erase(iterAllItem);

			}				
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 1)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen -= bodylen;
					it++;
					CMhlt *pMhlt = (CMhlt*)_AllItem[it->first];
					pMhlt->_nTracksNum--;
					break;
				}
			}
			SAFE_DELETE(*pTrackitem); 			
			_alltracklist.erase(pTrackitem);				
			break;
		}		
	}	
}
void CIosiPodData::DeleteArtist(int  ArtistID)
{
	for(vector<CIosCDBTrackArtist*>::iterator pArtist = _artist.begin();pArtist!=_artist.end();pArtist++)
	{
		if(ArtistID == (*pArtist)->Id())
		{
			//第0个肯定是mhii
			map<int,cdbBase*>::iterator iterAllItem = _AllItem.find((*pArtist)->ItemKey[0]);
			if(iterAllItem == _AllItem.end())
				break;
			CMhii* MhiiIte = (CMhii*)iterAllItem->second;
			int bodylen = MhiiIte->_nBodylen;
			SAFE_DELETE(MhiiIte);
			_AllItem.erase(iterAllItem);
			//移除相关的数据
			for(size_t posArtist= 1; posArtist<(*pArtist)->ItemKey.size();posArtist++)
			{
				iterAllItem = _AllItem.find((*pArtist)->ItemKey[posArtist]);
				if(iterAllItem == _AllItem.end())
					break;
				SAFE_DELETE(iterAllItem->second);
				_AllItem.erase(iterAllItem);
			}				
			//MHSD头的数据包长度,个数要重新计算
			for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
			{
				if(it->second == 8)
				{
					CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
					pMhsd->_nBodylen -= bodylen;
					it++;
					CMhli *pMhli = (CMhli*)_AllItem[it->first];
					pMhli->_nArtistsNum--;
					break;
				}
			}	

			SAFE_DELETE((*pArtist));
			_artist.erase(pArtist);
			break;
		}
	}
}

int CIosiPodData::CreateNewAlbum(wstring AlbumName,STU_CREATE_DBID * pstudb)
{
	CIosCDBAlbumData * pAlbumData = new CIosCDBAlbumData();
	CMhia* pmhia = new CMhia();
	//memset(pmhia,0,sizeof(pmhia));
	memcpy(pmhia->_head._name,"mhia",4);
	pmhia->_head._headSize = 88;
	pmhia->_nAlbumid = Create32BitID();	
	pmhia->_nAlbumDBid = pstudb->AlbumID;
	pmhia->_unknow=0;
	memset(pmhia->_unknow1,0,sizeof(pmhia->_unknow1));
	memset(pmhia->_unknow2,0,sizeof(pmhia->_unknow2));
	pmhia->_nTrackNum =3;
	CMhod* phmod = AddMhod(MHOD_ID_ALBUM_ALBUM,AlbumName);
	pmhia->_nBodylen =  pmhia->_head._headSize + phmod->_nBodylen;//包大小=头大小+mhod大小

	CMhod* mhodArtist = AddMhod(MHOD_ID_ALBUM_ARTIST,pstudb->AlbumArtist);
	pmhia->_nBodylen += mhodArtist->_nBodylen;

	CMhod* mhodTitleSort = AddMhod(MHOD_ID_ALBUM_SORT_ARTIST,AlbumName);
	pmhia->_nBodylen += mhodTitleSort->_nBodylen;	

	//提供给外部数据
	pAlbumData->Id(pmhia->_nAlbumid);
	pAlbumData->DBID(pmhia->_nAlbumDBid);
	pAlbumData->AlbumName(AlbumName);
	pAlbumData->AlbumArtis(pstudb->AlbumArtist);
	//保存数据
	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,pmhia));
	pAlbumData->ItemKey.push_back(newkey);
	SAVESTRUCT(newkey,_AllItem,pAlbumData,phmod);

	SAVESTRUCT(newkey,_AllItem,pAlbumData,mhodArtist);

	SAVESTRUCT(newkey,_AllItem,pAlbumData,mhodTitleSort);	

	_albumlist.push_back(pAlbumData);//保存列表
	//增加了新内容MHSD头的数据包长度字段要重新计算
	for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
	{
		if(it->second == 4)
		{
			CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
			pMhsd->_nBodylen += pmhia->_nBodylen;
			it++;
			CMhla *pMhla = (CMhla*)_AllItem[it->first];
			pMhla->_nAlbumsNum++;
			break;
		}
	}
	return pmhia->_nAlbumid;
}

int CIosiPodData::CreateNewArtist(wstring ArtistName,STU_CREATE_DBID * pstudb)
{
	CIosCDBTrackArtist * pArtistata = new CIosCDBTrackArtist();
	
	//文件结构数据
	CMhii* pmhii = new CMhii();
	//memset(pmhii,0,sizeof(pmhii));
	memcpy(pmhii->_head._name,"mhii",4);
	pmhii->_head._headSize = 80;
	pmhii->_nArtistid = Create32BitID();	
	pmhii->_nArtistDBid = pstudb->ArtistID;
	pmhii->_unknow=0;
	pmhii->_nTrackNum =1;
	CMhod* phmod = AddMhod(MHOD_ID_ALBUM_ARTIST_MHII,ArtistName);
	pmhii->_nBodylen =  pmhii->_head._headSize + phmod->_nBodylen;//包大小=头大小+mhod大小
	
	//提供给外部数据
	pArtistata->Artistname(ArtistName);
	pArtistata->Id(pmhii->_nArtistid);
	pArtistata->Dbid(pmhii->_nArtistDBid);
	
	//保存数据
	int newkey = _AllItem.size();
	_AllItem.insert(map<int, cdbBase*>::value_type(newkey,pmhii));
	pArtistata->ItemKey.push_back(newkey);
	SAVESTRUCT(newkey,_AllItem,pArtistata,phmod);
	_artist.push_back(pArtistata);//保存列表
	//增加了新内容MHSD头的数据包长度字段要重新计算
	for(map<int,int>::iterator it =_MhsdList.begin();it != _MhsdList.end();it++)
	{
		if(it->second == 8)
		{
			CMhsd * pMhsd =	(CMhsd*)_AllItem[it->first];
			pMhsd->_nBodylen += pmhii->_nBodylen;
			it++;
			CMhli *pMhli = (CMhli*)_AllItem[it->first];
			pMhli->_nArtistsNum++;
			break;
		}
	}
	
	return pmhii->_nArtistid;
}

void CIosiPodData::UpdateDeviceMusicPath()
{  
}

CMhsd* CIosiPodData::CreateMhsdHead(int type,int bodylen)
{
	CMhsd* pmhsd = new CMhsd();
	memcpy(pmhsd->_head._name,"mhsd",4);
	pmhsd->_head._headSize = 96;
	pmhsd->_nBodylen = bodylen;
	pmhsd->_type = type;
	return pmhsd;
}




#endif