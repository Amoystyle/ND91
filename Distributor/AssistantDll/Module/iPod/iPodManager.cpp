#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Module/iPod/iPodManager.h"
#include "Module/iPod/VCiPodFileSync.h"
#include "HashVerify.h"

#include "Common/FileOperation.h"
#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Module/Common/Media/iPodMedia/iPodMedia.h"
#include "Module/Common/Media/iPodMedia/IiPodMediaParser.h"

using namespace ND91Assistant;
ciPodManager*  ciPodManager::m_pInstance = NULL;	//实例初始化
int G_DBver=0;
ciPodManager::ciPodManager(wstring cdbFilePath)
:iPodDbBase(cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\"),_cdbFilePath(cdbFilePath)
{
	if(G_DBver==0)
		G_DBver = iPodFileSync::GetDBVer();

	wstring parafile = cdbFilePath + L"\\param.bin";
    CFileOperation::DeleteFile(parafile);

  pIosPlayListDB  = new cIosPlayListDB(cdbFilePath + L"\\iTunes");
  piPodCDBDataList = new CIosiPodData();

//			int cret = HashVerify::CheckCBDFile(cdbFilePath + L"\\iTunes\\iTunesCDB");
  try
  {
	piPodCDBDataList->LoadCDBDataFromFile(cdbFilePath,pIosPlayListDB);
  }
  catch(...)
  {
	  THROW_ASSISTANT_EXCEPTION(CDB_FILE_FORMAT_ERROR);
  }
	m_Hashtype = piPodCDBDataList->m_HashType;

	GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);	
	
}

ciPodManager::~ciPodManager()
{
	GdiplusShutdown(_gdiplusToken);
	SAFE_DELETE(piPodCDBDataList);
	SAFE_DELETE(pIosPlayListDB);
}

//销毁单键实例
void   ciPodManager::DestroyInstance()
{
	if (NULL != m_pInstance)
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}

}

//获得自身的单键实例
ciPodManager*	ciPodManager::GetInstance()
{
	return m_pInstance;
}
void ciPodManager::Init(wstring cdbFilePath,int VersionNUM = 421)
{
	G_DBver = VersionNUM;
	if (NULL == m_pInstance)
	{
		m_pInstance = new ciPodManager(cdbFilePath);
	}	
}
BOOL ciPodManager::GetAlbumCover(__int64 TrackID,wstring TrackTitle)
{
	std::vector<int> thumbTypes;
	thumbTypes.push_back(3006);
	int ArtWorkID = GetArtworkid(TrackID);
	wstring artworkdb = _cdbFilePath + L"\\Artwork\\ArtworkDB";
	if(CFileOperation::IsFileExist(artworkdb.c_str()))
	{
		IosPhotoDB pdb(artworkdb, thumbTypes,_cdbFilePath + L"\\Artwork\\");
		Bitmap* ArtWorkBmp = pdb.GetImage(ArtWorkID,F3006);
		if(ArtWorkBmp)
		{		
			CLSID jpegClsid;
			wstring artworkFileName = _cdbFilePath + L"\\cache\\";

			if(!CFileOperation::IsDirExist(artworkFileName.c_str()))
						CFileOperation::MakeLocalDir(artworkFileName);
			artworkFileName+=TrackTitle;
			artworkFileName+=L"_";
			wchar_t buffer[32];
			_i64tow_s(TrackID, buffer, 31, 10);
			artworkFileName+=buffer;
			artworkFileName+=L".jpg";
			GetEncoderClsid(_T("image/jpeg"), &jpegClsid);//选择编码
			ArtWorkBmp->Save(artworkFileName.c_str(),&jpegClsid);
			return TRUE;
		}
	}
	return FALSE;
}
bool ciPodManager::FindTrackByMp3Info(IiPodMediaParser * mediainfo)
{
	const CIosiPodData* iPodata = GetPiPodCDBData();	
	for (size_t i = 0; i < iPodata->GetTrackCount(); i++)
	{
		CIosCDBTrackData* pTrack = iPodata->GetTrackData(i);
		if(pTrack->Title() == mediainfo->PropertyGetMusicName())
		{			
			if( pTrack->Timer() == mediainfo->GetPlayTime())
			{
				if( _wtol(pTrack->FileSizeStr().c_str()) == mediainfo->GetFileSize())
				{
					return true;			
				}
			}
		}
	}
	return false;
}
CIosCDBTrackData* ciPodManager::InsertNewTrack(wstring FileName)
{
	ciPodMedia Media(FileName);
	IiPodMediaParser* objMP3Data = Media.GetInstance();
	RETURN_NULL_IF(!objMP3Data);
	if(FindTrackByMp3Info(objMP3Data))
		return NULL;
 	string pArtWork = objMP3Data->PropertyGetArtWork();

	int ArtworkID = 0;
	STU_CREATE_DBID NewDBID;
	NewDBID.ArtWorkID = 0;
	GetAndCreateAllDBID(objMP3Data,&NewDBID);	 
	 if(NewDBID.ArtWorkID == 0 && pArtWork.size()>0)
	 {
		wstring TempBmpFile = _cdbFilePath + L"\\TempArtworkFile.jpg";
		CFileOperation::WriteBinFile(TempBmpFile,pArtWork.c_str(),pArtWork.size());

		std::vector<int> thumbTypes;
		thumbTypes.push_back(3001);
		thumbTypes.push_back(3002);
		thumbTypes.push_back(3005);
		thumbTypes.push_back(3006);
		thumbTypes.push_back(3007);
		
		wstring artworkdb = _cdbFilePath + L"\\Artwork\\ArtworkDB";
		if(CFileOperation::IsFileExist(artworkdb.c_str()))
		{
			IosPhotoDB pdb(artworkdb, thumbTypes,_cdbFilePath + L"\\Artwork\\");
			pdb.SetNeedMHAF(true);
			Bitmap* ArtWorkBmp = new Bitmap(TempBmpFile.c_str());
			ArtworkID = pdb.AddImage(ArtWorkBmp);

			pdb.Write(_cdbFilePath + L"\\Artwork\\ArtworkDBNew");	
		}
		NewDBID.ArtWorkID = ArtworkID;
	 }

	//创建所有需要用到的数据库新ID
	
	 NewDBID.DevFilePath = GetNewDevFileName();
	//CBD文件增加内容
	CIosCDBTrackData* ptrackdata = piPodCDBDataList->InsertTrack(FileName,objMP3Data,&NewDBID);
	//数据库插入数据
	InsertSqlDBNewTrack(&NewDBID,objMP3Data);	
	//ptrackdata->Devicepath(remotFile);//设备上的路径
	
	//添加到总列表
	if(pIosPlayListDB)
	{
		__int64 PlayListDBID = pIosPlayListDB->addTrackToTotalPlayListDB(NewDBID.TrackID);
		piPodCDBDataList->AddTrackToPlayList(PlayListDBID,ptrackdata->Dbid());
	}
	//保存CBD文件
	piPodCDBDataList->SaveCDBDataToFile(_cdbFilePath + L"\\iTunes\\iTunesCDB");
	//生成校验码跟校验文件
	if(HashVerify::CheckCBDFile(_cdbFilePath) == S_OK )
	{
		if(iPodFileSync::GetDBVer() >= 430 ||
			HashVerify::checkSqlDb(_cdbFilePath,m_Hashtype) == S_OK)
		{		
			iPodFileSync::UpLoadFile(FileName,NewDBID.DevFilePath);
		}
	}
	return ptrackdata;
}


void ciPodManager::DeleteTrack(CIosCDBTrackData *pTrackData)
{
	if(!pTrackData)
				return;
	int ArtWorkID = 0;
	int &nArtWorkID = ArtWorkID;

	wstring filename = DeleteSqlDbtrack(pTrackData,nArtWorkID);
	//wstring remoteFile =  IOS_4_MUSIC_PATH +filename;
	piPodCDBDataList->RemoveTrack(pTrackData);
	piPodCDBDataList->SaveCDBDataToFile(_cdbFilePath + L"\\iTunes\\iTunesCDB");
	wstring CoverDbPath = _cdbFilePath + L"\\Artwork\\ArtworkDB";
	if(pTrackData->HasArtWork() && CFileOperation::IsFileExist(CoverDbPath.c_str()))
	//修改封面
	{	
		std::vector<int> thumbTypes;
		thumbTypes.push_back(3006);
		thumbTypes.push_back(3005);
		IosPhotoDB pdb(CoverDbPath, thumbTypes,_cdbFilePath + L"\\Artwork\\");
		pdb.DelImage(ArtWorkID,false);
		pdb.Write(_cdbFilePath + L"\\Artwork\\ArtworkDBNew");
	}

	//生成校验码跟校验文件
	if(HashVerify::CheckCBDFile(_cdbFilePath) == S_OK )
	{
		if(iPodFileSync::GetDBVer() >= 430 ||
			HashVerify::checkSqlDb(_cdbFilePath,m_Hashtype) == S_OK)
		{		
		iPodFileSync::DeleteMusicFile(filename);
		}
	}

}
void ciPodManager::ModifyTrack(CIosCDBTrackData *pTrackData ,wstring ArtWorkFilePath)
{
	if(!pTrackData)
		return;
	//如果修改的专辑，表演者等并产生新专辑，表演者等，需要新建数据库ID
	STU_CREATE_DBID NewDBID;

	NewDBID.AlbumArtist = pTrackData->AlbumArtistStr();
	NewDBID.TrackID = pTrackData->Dbid();
	NewDBID.AlbumArtistID = pTrackData->AlbumArtistDBId();
	NewDBID.AlbumID = pTrackData->AlbumDBId();
	NewDBID.ArtistID = pTrackData->ArtistDBId();
	NewDBID.ComposerID = pTrackData->ComposerDBId();

	

	string Artwork;
	if(ArtWorkFilePath.length()>0)
		Artwork = CFileOperation::ReadFile( ArtWorkFilePath, true);

	//修改数据库并获得文件的存储位置.
	wstring RandFileName = UpdateSqlDbTrack(pTrackData,&NewDBID,ArtWorkFilePath,_cdbFilePath);
	//wstring remotFile = IOS_4_MUSIC_PATH + RandFileName;
	wstring remotFile =  RandFileName;
	wstring LocalFile = _cdbFilePath + L"\\Music\\";
	LocalFile += RandFileName;

	if( CFileOperation::IsFileExist(LocalFile.c_str())== false)
	{	//如果mp3文件不存在，下载.
		iPodFileSync::DownLodMusic(LocalFile,remotFile);
	}
	//修改MP3文件
	ciPodMedia Media(LocalFile);
	IiPodMediaParser* objMP3Data = Media.GetInstance();
	RETURN_IF(!objMP3Data);
	if(objMP3Data->IsValid())
	{
		objMP3Data->PropertySetMusicPerformer(pTrackData->ArtistStr());
		objMP3Data->PropertySetAlbumName(pTrackData->AlbumStr());
		objMP3Data->PropertySetAlbumPerformer(NewDBID.AlbumArtist);
	//	objMP3Data.PropertySetNotes(pTrackData->CommentStr());
		objMP3Data->PropertySetMusicName(pTrackData->Title());
		objMP3Data->PropertySetComposer(pTrackData->ComposerStr());
	//	objMP3Data.PropertySetClassify(pTrackData->CategoryStr());
	//	objMP3Data.PropertySetStyle(pTrackData->GenreStr());
		if(Artwork.length()>0)
			objMP3Data->PropertySetArtWork(Artwork);
	}
	

	//修改CBD文件
	piPodCDBDataList->ModifyTrack(pTrackData,&NewDBID);

	piPodCDBDataList->SaveCDBDataToFile(_cdbFilePath + L"\\iTunes\\iTunesCDB");

	//生成校验码跟校验文件
	if(HashVerify::CheckCBDFile(_cdbFilePath) == S_OK )
	{
		if(iPodFileSync::GetDBVer() >= 430 ||
			HashVerify::checkSqlDb(_cdbFilePath,m_Hashtype) == S_OK)
		{			
		//上传音乐文件
		iPodFileSync::UpLoadFile(LocalFile,remotFile);
		}
	}
}
void ciPodManager::TestPackCDBFile()
{ 
	piPodCDBDataList->SaveCDBDataToFile(_cdbFilePath + L"\\iTunes\\iTunesCDB");

	//生成校验码跟校验文件
	if(HashVerify::CheckCBDFile(_cdbFilePath) == S_OK )
	{
		if(iPodFileSync::GetDBVer() >= 430 ||
			HashVerify::checkSqlDb(_cdbFilePath,m_Hashtype) == S_OK)
		{			
		//上传文件
		iPodFileSync::UpLoadCDB();
		iPodFileSync::UpdateDevItunesDB();
		}
	}
}

void ciPodManager::CreatePlayList(wstring PlayListName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB)
	{
		__int64 pid = pIosPlayListDB->InsertSqlDbPlayList(PlayListName);

		piPodCDBDataList->AddPlayList(PlayListName,pid);
		piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
		iPodFileSync::UpLoadCDB();
		iPodFileSync::UploadPlayListDB(_TempFilePath);
	}
}

void ciPodManager::DeletePlayList(wstring PlayListName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB)
		pIosPlayListDB->DeleteSqlDbPlayList(PlayListName);

	iPodFileSync::UploadPlayListDB(_TempFilePath);
}
void ciPodManager::DeletePlayList(__int64 PlayListId)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB)
		pIosPlayListDB->DeleteSqlDbPlayList(PlayListId);

	iPodFileSync::UploadPlayListDB(_TempFilePath);
}
void ciPodManager::ModifyPlayList(wstring PlayListOldName,wstring PlayListNewName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB)
		pIosPlayListDB->UpdateSqlDbPlayList(PlayListOldName,PlayListNewName);

	iPodFileSync::UploadPlayListDB(_TempFilePath);
}
void ciPodManager::ModifyPlayList(__int64 PlayListId,wstring PlayListNewName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	if(pIosPlayListDB)
		pIosPlayListDB->UpdateSqlDbPlayList(PlayListId,PlayListNewName);

	iPodFileSync::UploadPlayListDB(_TempFilePath);
}
//
void ciPodManager::AddToPlayList(__int64 trackid,wstring PlayListName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	//wstring uploadfile =  cIosPlayListXml::addTrackToPlist(trackid,PlayListName,_TempFilePath);
	if(pIosPlayListDB)
		pIosPlayListDB->addTrackToPlayListDB(trackid,PlayListName);	
	ReloadPlayListDB();
	iPodFileSync::UploadPlayListDB(_TempFilePath);
}

void ciPodManager::RemoveFromPlayList(__int64 trackid,wstring PlayListName)
{
	wstring _TempFilePath = _cdbFilePath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	//wstring uploadfile =  cIosPlayListXml::addTrackToPlist(trackid,PlayListName,_TempFilePath);
	if(pIosPlayListDB)
		pIosPlayListDB->RemoveTrackFromPlayListDB(trackid,PlayListName);
	ReloadPlayListDB();
	iPodFileSync::UploadPlayListDB(_TempFilePath);
}

__int64 ciPodManager::find_Album_PID(int  trackid)
{
	for(size_t i = 0; i<piPodCDBDataList->_albumlist.size();i++)
	{
		if(piPodCDBDataList->_albumlist[i]->Id() == trackid)
			return piPodCDBDataList->_albumlist[i]->DBID();
	}
	return 0;
};

__int64 ciPodManager::find_artist_PID(int  trackid)
{
	for(size_t i = 0; i<piPodCDBDataList->_artist.size();i++)
	{
		if(piPodCDBDataList->_artist[i]->Id() == trackid)
			return piPodCDBDataList->_artist[i]->Dbid();
	}
	return 0;
};

wstring ciPodManager::GetDevFilePathByID(__int64 PlayListId)
{
	return GetTrackPath(PlayListId);
}	

int ciPodManager::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num= 0;
	UINT size= 0;
	ImageCodecInfo* pImageCodecInfo= NULL;
	GetImageEncodersSize(&num, &size);
	if(size== 0)
	{
		return -1;
	}
	pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo== NULL)
	{
		return -1;
	}
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j=0; j< num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
		{
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}
void ciPodManager::ReloadPlayListDB()
{
	piPodCDBDataList->ReLoadPlayListDB(pIosPlayListDB);
}


#endif