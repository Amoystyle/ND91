#include "stdafx.h"
#ifdef MODULE_IPOD

#include <atlconv.h>
#include <time.h>
#include <algorithm> 
#include <functional>

#include "Module/iPod/Ios5iPodManager.h"
#include "Ios5ItemDB.h"
#include "Ios5Artist.h"
#include "Ios5albumDB.h"
#include "Ios5ComposerDB.h"
#include "Ios5AlbumArtistDB.h"
#include "Ios5Container.h"

#include "AlbumArtistDB.h"
#include "Module/iPod/VCiPodFileSync.h"
#include "Module/iPod/iPodDbBase.h"
#include "db_infoDB.h"

#include "xmltoPlist.h"
#include "Winver.h"

#include "Common/SqlAccess/BaseTable.h"
#include "Common/FileOperation.h"
#include "Common/DealTime.h"
#include "Common/plist/PlistOperation.h"
#include "Device/iPhoneInterfaceDef.h"
#include "Module/Common/Media/iPodMedia/iPodMedia.h"
#include "Module/Common/Media/iPodMedia/IiPodMediaParser.h"

#define	IS_SOFT_ITUNES					__T("SOFTWARE\\Apple Computer, Inc.\\iPod\\RegisteredApps\\4")
#define	IS_SOFT_APPLE_MOBILE_DEVICE		_T("SOFTWARE\\Apple Inc.\\Apple Application Support")
#define INSDIR  _T("InstallDir")
#define path _T("path")

#define  IOS_5_MUSIC_PATH L"/private/var/mobile/Media/iTunes_Control/Music/"
#pragma comment(lib,"Version.lib")
using namespace ND91Assistant;
ciPodManager5*  ciPodManager5::m_pInstance = NULL;	//实例初始化
wstring ciPodManager5::m_plistBuffer = L"";
int  ciPodManager5::m_Version=0;


	HANDLE _m_hDealEvent;

ciPodManager5::ciPodManager5(wstring sqlDbPath,int handle):_sqlDbPath(sqlDbPath)
{
		_m_hDealEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	bSyncProcessFinsh = false;
	bDBActive = false;
	if(G_DBver==0)
		G_DBver = iPodFileSync::GetDBVer();

	MessageWinHandle = handle;

	wstring sqldbfilename = sqlDbPath + L"\\iTunes\\MediaLibrary.sqlitedb";
	//刚用itunes同步完没有任何歌曲的iphone没有这些文件，手动拷贝备份的文件
	if(!CFileOperation::IsFileExist(sqldbfilename.c_str()))
	{
		wstring copydesc = sqlDbPath + L"\\iTunes\\";
		wstring copysrc = CFileOperation::GetCurrWorkingDir() + L"IOS\\";
		copysrc+=CStrOperation::IntToWString(G_DBver);
		copysrc+=L"\\iTunes_Control\\iTunes\\";
		CFileOperation::CopyDir(copysrc,copydesc);
	}

	piPodDataList = new cIPodData5(sqldbfilename);

// 	hDLL = LoadLibrary(L"MusicTrace.dll");
// 	if (hDLL != NULL)
// 	{
// 		Ios5SyncMusic = (SyncMusic)GetProcAddress(hDLL, "ND_IOS5_SyncMusic");
// 		Ios5SendPlistAddr = (SendPlistAddr)GetProcAddress(hDLL, "ND_SendPlistAddr");
// 	}

	TCHAR cBuffer[1024];
	DWORD dwRet = GetEnvironmentVariable(_T("PATH"), cBuffer, 1024);
	wstring allPath = cBuffer;

	wstring szPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLiTunesMobileDeviceReg, _T("InstallDir"));
	wstring dllPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
	wstring strItunes = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, path);
	wstring strItunesPath = CFileOperation::GetFileDir(strItunes);

	allPath += L";";
	allPath +=	strItunesPath+ L";" ;

	if ( (int)allPath.find(szPath) < 0 )
	{
		allPath += szPath + L";";
	}
	if ( (int)allPath.find(dllPath) < 0 )
	{
		allPath += dllPath + L";" ;
	}
	
	int i = SetEnvironmentVariable(_T("PATH"), allPath.c_str());



	piPodCDBDataList = new CIosiPodData();
	try
	{
		piPodCDBDataList->LoadCDBDataFromFile(sqlDbPath ,NULL);
	}
	catch(...)
	{
		THROW_ASSISTANT_EXCEPTION(CDB_FILE_FORMAT_ERROR);
	}
	
	if(bDBActive == false)
	{
		bDBActive = sql.Open(_sqlDbPath + L"\\iTunes\\MediaLibrary.sqlitedb");
	}
	

	wstring ProfileName = CFileOperation::GetCurrWorkingDir() + L"Option.ini";
	if(CFileOperation::IsFileExist(ProfileName.c_str()))
	{
		WORD MajorVersion =0; WORD MinorVersion =0; WORD BuildNumber =0; WORD RevisionNumber =0;
		GetAppVersion((wchar_t *)strItunes.c_str(),&MajorVersion,&MinorVersion,&BuildNumber,&RevisionNumber);
		wstring m_strVersion = CStrOperation::IntToWString(MajorVersion);
		m_strVersion+=L".";
		m_strVersion+=CStrOperation::IntToWString(MinorVersion);
		m_strVersion+=L".";
		m_strVersion+=CStrOperation::IntToWString(BuildNumber);
		m_strVersion+=L".";
		m_strVersion+=CStrOperation::IntToWString(RevisionNumber);
		
		wstring addrvalue= MyFileOperation::GetIniFileValue(ProfileName, L"ItunesPrvInterface",m_strVersion);
		USES_CONVERSION;
		iTunesPrvInterface = CStrOperation::Hex2Int(W2A(addrvalue.c_str()));
	}	
	else
	{
		iTunesPrvInterface =0;
	}
	
}


ciPodManager5::~ciPodManager5()
{
	if(bDBActive)
	{
		sql.Close();
		bDBActive=false;
	}
	SAFE_DELETE(piPodCDBDataList);
	SAFE_DELETE(piPodDataList);
	CloseHandle(_m_hDealEvent);
	//if (hDLL)
	//{
	//	FreeLibrary(hDLL);
	//}
}
BOOL ciPodManager5::GetAppVersion( wchar_t *LibName, WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber )
{
	DWORD dwHandle, dwLen;
	UINT BufLen;
	LPTSTR lpData;
	VS_FIXEDFILEINFO *pFileInfo;
	dwLen = GetFileVersionInfoSize( LibName, &dwHandle );
	if (!dwLen)   return FALSE; 
	lpData = (LPTSTR) malloc (dwLen);
	if (!lpData)   return FALSE; 
	if( !GetFileVersionInfo( LibName, dwHandle, dwLen, lpData ) )
	{ 
		free (lpData);
		return FALSE; 
	} 
	if( VerQueryValue( lpData, _T("\\"), (LPVOID *) &pFileInfo, (PUINT)&BufLen ) ) 
	{ 
		*MajorVersion = HIWORD(pFileInfo->dwFileVersionMS);
		*MinorVersion = LOWORD(pFileInfo->dwFileVersionMS); 
		*BuildNumber = HIWORD(pFileInfo->dwFileVersionLS);
		*RevisionNumber = LOWORD(pFileInfo->dwFileVersionLS); 
		free (lpData); 
		return TRUE; 
	}
	free (lpData);
	return FALSE;
}
// void ciPodManager5::SyncMessage(notification_callback_info * back_info)
// {
// 	int ret = back_info->msg;
// 	string msg = back_info->str;
// }

bool ciPodManager5::CheckNewName(string filename,BaseTable<DBaseLocaltion5>* pItem)
{
	USES_CONVERSION;
	wstring sql = L"path = '" ;
		sql+= A2W(filename.c_str());
		sql+= L"'";

	pItem->Select(sql);
	int ret =pItem->GetRowCount();

	if( ret == 0)
		return false;
	else
		return true;
}
bool ciPodManager5::FindTrackByMp3Info(IiPodMediaParser * mediainfo)
{
	const CIosiPodData* iPodata = GetPiPodCDBData();	
	for (size_t i = 0; i < iPodata->GetTrackCount(); i++)
	{
		CIosCDBTrackData* pTrack = iPodata->GetTrackData(i);
		if(pTrack->Title() == mediainfo->PropertyGetMusicName()
			&& pTrack->Timer() == mediainfo->GetPlayTime()
			&& _wtol(pTrack->FileSizeStr().c_str()) == mediainfo->GetFileSize())
			return true;
	}
	return false;
}

CIosCDBTrackData* ciPodManager5::InsertNewTrack(wstring FileName)
{
	//生成不重复ID
	BaseTable<DItem5> tItem(&sql);		
	int ret = 1;	
	__int64 trackid;
	do{
		trackid=iPodDbBase::Create64BitID();
		tItem.Select(trackid,L"item_pid");		
		ret =tItem.GetRowCount();
		tItem.Clear();
	}while( ret != 0);

	//生成设备上保存的文件名
	BaseTable<DBaseLocaltion5> pBaseLocaltion5(&sql);	
	string	DeviceFileName= "iTunes_Control/Music/";
	string mp3name = iPodDbBase::gen_file_name();
	DeviceFileName+=mp3name; //遇到同名文件要要换文件名
	while(CheckNewName(DeviceFileName,&pBaseLocaltion5))
	{		
		DeviceFileName = "iTunes_Control/Music/";
		 mp3name = iPodDbBase::gen_file_name();
		DeviceFileName+=mp3name;
	}
	//mp3音乐属性
	ciPodMedia Media(FileName);
	IiPodMediaParser* objMP3Data = Media.GetInstance();
	RETURN_NULL_IF(!objMP3Data);
	//已经有这首歌曲了
	if(FindTrackByMp3Info(objMP3Data))
		return NULL;

	//获得或生成所有属性pid
	STU_CREATE_DBID NewDBID;
	NewDBID.TrackID = trackid;
	NewDBID.AlbumArtistID=GetAlbumArtistPid(objMP3Data->PropertyGetAlbumPerformer(),trackid);
	NewDBID.AlbumID =  GetAlbumPid(objMP3Data->PropertyGetAlbumName(),trackid,NewDBID.AlbumArtistID);
	NewDBID.ArtistID = GetArtistPid(objMP3Data->PropertyGetMusicPerformer(),trackid);
	NewDBID.ComposerID=GetComposerPid(objMP3Data->PropertyGetComposer(),trackid);
	USES_CONVERSION;
	NewDBID.DevFilePath = A2W(mp3name.c_str());
	////数据库增加一条记录
	//DItem5 item = new DItem5();
	//item->album_pid
	//DItemextra5 itemext = new DItemextra5();
	//DBaseLocaltion5 = new DBaseLocaltion5();
	
	//CDB文件处理
	CIosCDBTrackData* pTrackData = piPodCDBDataList->InsertTrack(FileName,objMP3Data,&NewDBID);
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
	//调用dll同步plist文本操作
	iPodFileSync::UpLoadCDB();
	sMusicData arrMusicData[1];
	ZeroMemory(arrMusicData,sizeof(arrMusicData));
	_i64toa_s(trackid,	arrMusicData[0].music_name,31,10);
	//需上传的音乐数据
	string localpath = W2A(FileName.c_str());
	memcpy(arrMusicData[0].music_local_path, localpath.c_str(),localpath.length());
	memcpy(arrMusicData[0].music_store_path, DeviceFileName.c_str(),DeviceFileName.length());	
	string pArtWork = objMP3Data->PropertyGetArtWork();
	if(pArtWork.size()>0)
	{
		wstring TempBmpFile = _sqlDbPath + L"\\TempArtworkFile.jpg";
		CFileOperation::WriteBinFile(TempBmpFile,pArtWork.c_str(),pArtWork.size());
		memcpy(arrMusicData[0].artwork_local_path,W2A(TempBmpFile.c_str()),TempBmpFile.size());
	}
	m_plistBuffer = CreatePlistAddMidea(objMP3Data,&NewDBID);	
	
	SyncOper(MUSIC_ADD,arrMusicData);
	return pTrackData;
}
void ciPodManager5::UpLoadCDB()
{
	iPodFileSync::UpLoadCDB();
}
string ciPodManager5::CreatePlistWithXML(wstring xmlstr)
{

	string xmldata = CCodeOperation::UnicodeToUTF_8(xmlstr);
	void* pError = NULL;	
	string plistdata = PlistOperation::ConvertPlist(xmldata,TRUE);
	return plistdata;
}

void ciPodManager5::DeleteTrack(CIosCDBTrackData *pTrackData)
{
	RETURN_IF(!pTrackData);
	piPodCDBDataList->RemoveTrack(pTrackData);
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
	iPodFileSync::UpLoadCDB();
	m_plistBuffer = CreatePlistDelMidea(pTrackData);
	
	SyncOper(MUSIC_DEL,NULL);
}

void ciPodManager5::SyncOper(int OperateMode,sMusicData arrMusicData[])
{
	sConnDevice  *PsConnDevice = new sConnDevice; 
	memset(PsConnDevice,0,sizeof(sConnDevice));
	
	PsConnDevice->hWnd=0x100;// MessageWinHandle; 
	//if(devinfo->_ios_bJailBreaked)
	//	PsConnDevice->AFcConnection = (int)devinfo->_ios_afcHandle;
	//else
	PsConnDevice->AFcConnection = iPodFileSync::GetAFCHandle(); 
	PsConnDevice->OperateMode	= OperateMode; 	
	PsConnDevice->ReleaseFlag	= 0;		
	PsConnDevice->ItemCount	= 1;	//音乐数目
	PsConnDevice->iPodDev=0;
	PsConnDevice->CigAddr = iTunesPrvInterface;
	wstring struuid=iPodFileSync::GetUUID();
	USES_CONVERSION;
	PsConnDevice->uuid			= W2A(struuid.c_str());

	int syncret = ND_IOS5_SyncMusic(PsConnDevice,arrMusicData,&syncMusicCallBack,NULL);
	if(syncret != 0)
	{
		//ASSISTANT_EXCETPION(L"LOAD mobilesync dll fail!");
	}
	//等待回调返回
	::WaitForSingleObject(_m_hDealEvent, 10000);
	::ResetEvent(_m_hDealEvent);
	SAFE_DELETE(PsConnDevice);
}
void ciPodManager5::syncMusicCallBack(notification_callback_info * para)
{
	CreateThread(NULL,0,ciPodManager5::SYNCMusicThread,para,0, NULL);
}
DWORD WINAPI ciPodManager5::SYNCMusicThread(LPVOID lpParameter)
{
	notification_callback_info * para = (notification_callback_info *)lpParameter;
	if(para->iStatus == Msg_Status_Require_plist)
		ciPodManager5:: SendPlistAdd(para->pszMessage);
	else if(para->iStatus == Msg_Status_Copy_Music_finish)
	{
		//::SetEvent(_m_hDealEvent);
	}
	else if(para->iStatus == Msg_Status_Sync_finish
		||para->iStatus ==Msg_Status_Sync_error
		||para->iStatus ==Msg_Status_Sync_timeout
		||para->iStatus ==Msg_Status_Get_Plist_timeout)
	{
		::SetEvent(_m_hDealEvent);
	}

	return 0;
}
void ciPodManager5:: SendPlistAdd(char* revision)
{
	m_plistBuffer+=CreatePlistTail(revision);
	string plistbuff=CreatePlistWithXML(m_plistBuffer);
	wstring plistsavepath =  CFileOperation::GetCurrWorkingDir() + L"temp.plist";
	CFileOperation::WriteBinFile(plistsavepath,plistbuff.c_str(),plistbuff.size());
	if(plistbuff.size() == 0)
	{
		printf("error on convxml");
	}
	sPlistInfo *pPlistInfo = new sPlistInfo;
	pPlistInfo->plist_size	= plistbuff.length();
	pPlistInfo->plist_buffer = (int *)plistbuff.c_str();
	int sendret = ND_SendPlistAddr(pPlistInfo);

	SAFE_DELETE_ARRAY(pPlistInfo);
}

void ciPodManager5::CreatePlayList(wstring PlayListName)
{
	__int64 pid;
	m_plistBuffer = CreatePlistCreateplayList(PlayListName,pid);
	piPodCDBDataList->AddPlayList(PlayListName,pid);
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
	iPodFileSync::UpLoadCDB();

	SyncOper(PLAYLIST_ADD,NULL);
}

void ciPodManager5::DeletePlayList(wstring PlayListName)
{
	__int64 pid;
	m_plistBuffer = CreatePlistDeleteplayList(PlayListName,pid);
	piPodCDBDataList->RemovePlayList(pid);
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
	iPodFileSync::UpLoadCDB();
	SyncOper(PLAYLIST_DEL,NULL);
}

void ciPodManager5::ModifyPlayList(wstring PlayListName,wstring NewFileName)
{
	__int64 pid = FindPidByName(PlayListName);

	std::vector<__int64> TrackToAdd;

	for(size_t i=0;i<piPodDataList->GetPlayListCount();i++)
	{
		CIosCDBPlayList* Playlist = piPodDataList->GetPlayListData(i);
		if(Playlist->ID() == pid)
		{
			for(size_t t=0;t< Playlist->TrackList()->size();t++)
				TrackToAdd.push_back(Playlist->GetTrackPid(t));
			break;
		}
	}	
	piPodCDBDataList->ModifyPlayList(pid,NewFileName);
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");
	m_plistBuffer = CreatePlistModifyPlayList(pid,NewFileName,TrackToAdd);
	SyncOper(PLAYLIST_UPDATE,NULL);
}
void ciPodManager5::AddToPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName )
{
	__int64 pid = FindPidByName(PlayListName);

	std::vector<__int64> TrackToAdd;
	
	for(size_t i=0;i<piPodDataList->GetPlayListCount();i++)
	{
		CIosCDBPlayList* Playlist = piPodDataList->GetPlayListData(i);
		if(Playlist->ID() == pid)
		{
			for(size_t t=0;t< Playlist->TrackList()->size();t++)
					TrackToAdd.push_back(Playlist->GetTrackPid(t));
			break;
		}
	}

	for(size_t j=0;j<trackid.size();j++)
	{
		TrackToAdd.push_back(trackid.at(j));
		piPodCDBDataList->AddTrackToPlayList(pid,trackid.at(j));
	}
	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");

	if(NewName == L"")
		m_plistBuffer = CreatePlistModifyPlayList(pid,PlayListName,TrackToAdd);
	else
		m_plistBuffer = CreatePlistModifyPlayList(pid,NewName,TrackToAdd);
	SyncOper(PLAYLIST_UPDATE,NULL);
}

void ciPodManager5::DelFromPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName )
{
	__int64 pid = FindPidByName(PlayListName);

	std::vector<__int64> TrackToAdd;

	for(size_t i=0;i<piPodCDBDataList->GetPlayList2Count();i++)
	{
		CIosCDBPlayList* Playlist = piPodCDBDataList->GetPlayList2Data(i);
		if(Playlist->ID() == pid)
		{
			for(size_t t=0;t< Playlist->TrackList()->size();t++)
			{
				vector<__int64>::iterator result =std::find(trackid.begin(),trackid.end(),Playlist->GetTrackPid(t));
				if(result == trackid.end())
				{
					TrackToAdd.push_back(Playlist->GetTrackPid(t));					
				}
				else
					piPodCDBDataList->RemoveTrackFromPlayList(pid,trackid.at(t));
			}
			break;
		}
	}

	piPodCDBDataList->SaveCDBDataToFile(_sqlDbPath + L"\\iTunes\\iTunesCDB");

	if(NewName == L"")
		m_plistBuffer = CreatePlistModifyPlayList(pid,PlayListName,TrackToAdd);
	else
		m_plistBuffer = CreatePlistModifyPlayList(pid,NewName,TrackToAdd);
	SyncOper(PLAYLIST_UPDATE,NULL);
}



ciPodManager5* ciPodManager5::GetInstance()
{
	return m_pInstance;
}
void   ciPodManager5::DestroyInstance()
{
	if (NULL != m_pInstance)
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void ciPodManager5::Init(wstring sqlDbPath,int VersionNUM,int handle)
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new ciPodManager5(sqlDbPath,handle);
	}
	m_Version = VersionNUM;
}


wstring ciPodManager5::CreatePlistAddMidea(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb)
{	
	wstring Xmlplist = CreatePlistHead();
	Xmlplist+=CreatePlistAddTrackBody(pMP3Data,pstudb);
//	Xmlplist+=CreatePlistTail();
	return Xmlplist;
}

wstring ciPodManager5::CreatePlistHead()
{
	
	DDbInfo* pDbinfo = NULL;
	__int64 primary_container_pid=0;
	__int64 pid=0;
	BaseTable<DDbInfo> tDbInfo(&sql);	
	tDbInfo.SelectAll();
	if(tDbInfo.GetRowCount()>0)
	{
		pDbinfo = tDbInfo.GetDataRow(0);
		primary_container_pid = pDbinfo->primary_container_pid;
		pid = pDbinfo->db_pid;
	}
	
	wstring Xmlplist = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
	Xmlplist+=	L"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n";
	Xmlplist+=L"<plist version=\"1.0\">\r\n";
	Xmlplist+=L"<dict>\r\n";
	Xmlplist+=L"	<key>operations</key>\r\n";
	Xmlplist+=L"	<array>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>db_info</key>\r\n";
	Xmlplist+=L"<dict>\r\n";
	Xmlplist+=L"<key>audio_language</key>\r\n";
	Xmlplist+=L"	<integer>-1</integer>\r\n";
	Xmlplist+=L"	<key>primary_container_pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=Int64ToStr(primary_container_pid);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>subtitle_language</key>\r\n";
	Xmlplist+=L"	<integer>-1</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>operation</key>\r\n";
	Xmlplist+=L"	<string>update_db_info</string>\r\n";
	Xmlplist+=L"	<key>pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=Int64ToStr(pid);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	tDbInfo.Clear();
	return Xmlplist;
}
wstring ciPodManager5::CreatePlistTail(char* revision)
{
	USES_CONVERSION;
	wstring Xmlplist =L"</array>\r\n";
	Xmlplist+=L"	<key>revision</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=A2W(revision);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>timestamp</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+=getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	</plist>\r\n";
	return Xmlplist;
}
wstring ciPodManager5::getNowTimeStr()
{
	time_t now_time;
	time(&now_time);
	struct tm when;
	gmtime_s(&when, &now_time);

	char buffer[32];
	sprintf_s(buffer, 31, "%4d-%02d-%02dT%02d:%02d:%02dZ",
		1900+when.tm_year, when.tm_mon + 1, when.tm_mday,
		when.tm_hour, when.tm_min, when.tm_sec);
	USES_CONVERSION;
	return A2W(buffer);
}
wstring ciPodManager5::IntToStr( int i )
{
	wchar_t buffer[16];
	CStrOperation::MyItows(i, buffer, 16);
	return buffer;
}

wstring ciPodManager5::Int64ToStr( __int64 i )
{
	TCHAR buffer[32];
	_ui64tow_s(i, buffer, 31, 10);
	return buffer;
}


wstring ciPodManager5::CreatePlistAddTrackBody(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb)
{
	
USES_CONVERSION;
	wstring Xmlplist =L"<dict>\r\n";
	Xmlplist+=L"<key>avformat_info</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>audio_format</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=	pMP3Data->IsVideo()?L"501":L"301";
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>bit_rate</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=IntToStr(pMP3Data->GetiBitSpeed());
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>sample_rate</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=L"0";//IntToStr(pMP3Data->GetSampleRate());
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>item</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>album</key>\r\n";
	Xmlplist+=L"	<string>";
	wstring albumname = pMP3Data->PropertyGetAlbumName();
	if(albumname.length() == 0)
		albumname = L"Unknow Album";
	wstring artistname = pMP3Data->PropertyGetMusicPerformer();
	if(artistname.length() == 0)
		artistname = L"Unknow artist";
	Xmlplist+=albumname.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>album_artist</key>\r\n";
	Xmlplist+=L"	<string>";
	wstring albumartist = pMP3Data->PropertyGetAlbumPerformer().c_str();
	if(albumartist.length() == 0)
		albumartist = artistname;
	Xmlplist+=albumartist.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>album_pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+= L"0";//Int64ToStr(pstudb->AlbumID);// GetAlbumPid(pMP3Data->PropertyGetAlbumName());
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>artist</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=artistname.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>artist_pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=L"0";//Int64ToStr(pstudb->ArtistID);//GetArtistPid(pMP3Data->PropertyGetMusicPerformer());
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>artwork_cache_id</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>comment</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=pMP3Data->PropertyGetDescription().c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>composer</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=pMP3Data->PropertyGetComposer().c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>date_created</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+=getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	<key>description</key>\r\n";
	Xmlplist+=L"	<string></string>\r\n";
	Xmlplist+=L"	<key>disc_count</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>disc_number</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>genre</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=pMP3Data->PropertyGetClassify().c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>grouping</key>\r\n";
	Xmlplist+=L"	<string></string>\r\n";	
	if(pMP3Data->IsVideo())
	{
		Xmlplist+=L"<key>is_movie</key>\r\n";
		Xmlplist+=L"<true/>\r\n";
	}
	else
	{
		Xmlplist+=L"	<key>is_song</key>\r\n";
		Xmlplist+=L"	<true/>\r\n";
	}
	Xmlplist+=L"	<key>sort_album</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=albumname.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>sort_album_artist</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=albumartist.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>sort_artist</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=artistname.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>sort_composer</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=pMP3Data->PropertyGetComposer().c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>sort_name</key>\r\n";
	Xmlplist+=L"	<string>";
	wstring title = pMP3Data->PropertyGetMusicName();
	if(title.length()==0)
		title = CFileOperation::GetFileNameNoExt(pMP3Data->GetLocalFileName());
	Xmlplist+=title.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>title</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=title.c_str();
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	<key>total_time_ms</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=IntToStr(pMP3Data->GetPlayTime());
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	<key>track_count</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>track_number</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>year</key>\r\n";
	Xmlplist+=L"	<integer>";
	if(pMP3Data->PropertyGetYear().length() == 0)
		Xmlplist+=L"0";
	else
		Xmlplist+=pMP3Data->PropertyGetYear().substr(0,4);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>item_stats</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>has_been_played</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	<key>play_count_recent</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>play_count_user</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>skip_count_recent</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	<key>skip_count_user</key>\r\n";
	Xmlplist+=L"	<integer>0</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>location</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>kind</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=	pMP3Data->IsVideo()?L"MPEG Video":L"MPEG Audio";
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>operation</key>\r\n";
	Xmlplist+=L"	<string>insert_track</string>\r\n";
	Xmlplist+=L"	<key>pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=Int64ToStr(pstudb->TrackID);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	return Xmlplist;// CStrOperation::ReplaceW(Xmlplist,L"?",L"");
}


__int64 ciPodManager5::GetArtistPid(wstring ArtistName,__int64 TrackPid)
{
	__int64 ret = 0;
	BaseTable<DArtist5> tArtist5(&sql);
	wstring condition = L" item_artist= '";
	condition+=ArtistName;
	condition+=L"'";
	tArtist5.Select(condition);
	if(tArtist5.GetRowCount() > 0)
	{
		DArtist5* pDArtist5r = tArtist5.GetDataRow(0);
		ret = pDArtist5r->item_artist_pid;
		tArtist5.Clear();	
	}
	else
	{
		int rowcount = 1;	
		do{
			ret=iPodDbBase::Create64BitID();
			tArtist5.Select(ret,L"item_artist_pid");		
			rowcount =tArtist5.GetRowCount();
			tArtist5.Clear();
		}while( rowcount != 0);

		DArtist5* pArtistins = new DArtist5();
		pArtistins->item_artist =ArtistName;
		pArtistins->sort_item_artist = ArtistName;

		pArtistins->item_artist_pid = ret;
		pArtistins->cloud_status = 0;
		pArtistins->representative_item_pid = TrackPid;
		tArtist5.Insert(pArtistins);
		SAFE_DELETE(pArtistins);
	}
	return ret;
}

__int64 ciPodManager5::GetAlbumPid(wstring AlbumName,__int64 TrackPid,__int64 AlbumArtistPid)
{
	__int64 ret = 0;

	BaseTable<DAlbum5> tAlbum5(&sql);
	wstring condition = L" album= '";
	condition+=AlbumName;
	condition+=L"'";
	tAlbum5.Select(condition);
	if(tAlbum5.GetRowCount() > 0)
	{
		DAlbum5* pDAlbum5 = tAlbum5.GetDataRow(0);
		ret = pDAlbum5->album_pid;
		tAlbum5.Clear();	
	}
	else
	{
		int rowcount = 1;	
		do{
			ret=iPodDbBase::Create64BitID();
			tAlbum5.Select(ret,L"album_pid");		
			rowcount =tAlbum5.GetRowCount();
			tAlbum5.Clear();
		}while( rowcount != 0);

		DAlbum5* pAlbumAins = new DAlbum5();
		pAlbumAins->album =AlbumName;
		pAlbumAins->sort_album = AlbumName;
		pAlbumAins->album_artist_pid = AlbumArtistPid;
		pAlbumAins->album_pid = ret;
		pAlbumAins->cloud_status = 0;
		pAlbumAins->grouping_key = "";
		pAlbumAins->representative_item_pid = TrackPid;
		tAlbum5.Insert(pAlbumAins);
		SAFE_DELETE(pAlbumAins);
	}
	return ret;
}

__int64 ciPodManager5::GetComposerPid(wstring ComposerName,__int64 TrackPid)
{
	__int64 ret = 0;

	BaseTable<DComposer5> tComposer(&sql);
	wstring condition = L" composer= '";
	condition+=ComposerName;
	condition+=L"'";
	tComposer.Select(condition);
	if(tComposer.GetRowCount() > 0)
	{
		DComposer5* pComposer = tComposer.GetDataRow(0);
		ret = pComposer->composer_pid;
		tComposer.Clear();
	}
	else
	{
		int rowcount = 1;	
		do{
			ret=iPodDbBase::Create64BitID();
			tComposer.Select(ret,L"composer_pid");		
			rowcount =tComposer.GetRowCount();
			tComposer.Clear();
		}while( rowcount != 0);

		DComposer5* pComposerins = new DComposer5();
		pComposerins->composer =ComposerName;
		pComposerins->sort_composer = ComposerName;
		pComposerins->composer_pid = ret;
		pComposerins->cloud_status = 0;
		pComposerins->representative_item_pid = TrackPid;
		tComposer.Insert(pComposerins);
		SAFE_DELETE(pComposerins);
	}
	return ret;
}

__int64 ciPodManager5::GetAlbumArtistPid(wstring AlbumArtistName,__int64 TrackPid)
{
	__int64 ret = 0;

	BaseTable<DAlbumArtist5> tAlbumArtist(&sql);
	wstring condition = L" album_artist= '";
	condition+=AlbumArtistName;
	condition+=L"'";
	tAlbumArtist.Select(condition);
	if(tAlbumArtist.GetRowCount() > 0)
	{
		DAlbumArtist5* pAlbumArtist = tAlbumArtist.GetDataRow(0);
		ret = pAlbumArtist->album_artist_pid;
		tAlbumArtist.Clear();
	}
	else
	{
		int rowcount = 1;	
		do{
			ret=iPodDbBase::Create64BitID();
			tAlbumArtist.Select(ret,L"album_artist_pid");
			rowcount =tAlbumArtist.GetRowCount();
			tAlbumArtist.Clear();
		}while( rowcount != 0);

		DAlbumArtist5* pAlbumArtistins = new DAlbumArtist5();
		pAlbumArtistins->album_artist =AlbumArtistName;
		pAlbumArtistins->sort_album_artist = AlbumArtistName;
		pAlbumArtistins->album_artist_pid = ret;
		pAlbumArtistins->cloud_status = 0;
		pAlbumArtistins->grouping_key = "";
		pAlbumArtistins->representative_item_pid = TrackPid;
		tAlbumArtist.Insert(pAlbumArtistins);
		SAFE_DELETE(pAlbumArtistins);
	}
	return ret;
}


wstring ciPodManager5::CreatePlistDelMidea(CIosCDBTrackData *pTrackData)
{
	wstring Xmlplist = CreatePlistHead();
	Xmlplist+=CreatePlistDelTrackBody(pTrackData);
	//Xmlplist+=CreatePlistTail();
	return Xmlplist;
}

wstring ciPodManager5::CreatePlistCreateplayList(wstring playListName, __int64& pid)
{
	USES_CONVERSION;
	wstring Xmlplist = CreatePlistHead();
	pid = CreateNewPlayListPid();
	Xmlplist+=CreatePlistCreatePlayListBody(playListName.c_str(),pid);
	//Xmlplist+=CreatePlistTail();
	return Xmlplist;
}

wstring ciPodManager5::CreatePlistDeleteplayList(wstring playListName,__int64& pid)
{
	USES_CONVERSION;
	wstring Xmlplist = CreatePlistHead();
	pid = FindPidByName(playListName);
	Xmlplist+=CreatePlistDelPlayListBody(playListName.c_str(),pid);
	//Xmlplist+=CreatePlistTail();
	return Xmlplist;
}


wstring ciPodManager5::CreatePlistModifyPlayList(__int64 TrackPID,wstring playListName,vector<__int64> trackid)
{
	USES_CONVERSION;
	wstring Xmlplist = CreatePlistHead();
	Xmlplist+=CreatePlistModifyPlayListBody(playListName.c_str(),TrackPID,trackid);
	//Xmlplist+=CreatePlistTail();
	return Xmlplist;
}

wstring ciPodManager5::CreatePlistDelTrackBody(CIosCDBTrackData *pTrackData)
{
	wstring Xmlplist =L"<dict>\r\n";
//		Xmlplist +=L"<dict>\r\n";
		Xmlplist +=L"<key>operation</key>\r\n";
		Xmlplist +=L"<string>delete_track</string>\r\n";
		Xmlplist +=L"<key>pid</key>\r\n";
		Xmlplist +=L"<integer>";
		Xmlplist +=Int64ToStr(pTrackData->Dbid());
		Xmlplist +=L"</integer>\r\n";
		Xmlplist +=L"</dict>\r\n";
		if(IsAlbumEmpty(pTrackData->AlbumDBId()))
		{		
			Xmlplist +=L"<dict>\r\n";
			Xmlplist +=L"<key>operation</key>\r\n";
			Xmlplist +=L"<string>delete_album</string>\r\n";
			Xmlplist +=L"<key>pid</key>\r\n";
			Xmlplist +=L"<integer>";
			Xmlplist +=	Int64ToStr(pTrackData->AlbumDBId());
			Xmlplist +=L"</integer>\r\n";
			Xmlplist +=L"</dict>\r\n";
			Xmlplist +=L"<dict>\r\n";
		}
		Xmlplist +=L"<key>operation</key>\r\n";
		Xmlplist +=L"<string>delete_artist</string>\r\n";
		Xmlplist +=L"<key>pid</key>\r\n";
		Xmlplist +=L"<integer>";
		Xmlplist +=Int64ToStr(pTrackData->ArtistDBId());
		Xmlplist +=L"</integer>\r\n";
		Xmlplist +=L"</dict>\r\n";
	
		__int64 playlistPid = 0;
		for(size_t i =0;i<piPodCDBDataList->GetPlayList2Count();i++ )
		{	
			CIosCDBPlayList* pPlayList = piPodCDBDataList->GetPlayList2Data(i);
			const vector<__int64>* VecItemlist = pPlayList->TrackList();
			for(size_t j=0;j<VecItemlist->size();j++)
			{
				if(pTrackData->Dbid() == (__int64)VecItemlist->at(j) && VecItemlist->size() == 1)
				{
					Xmlplist +=L"<dict>\r\n";
					Xmlplist +=L"<key>container</key>\r\n";
					Xmlplist +=L"<dict>\r\n";
					Xmlplist +=L"<key>date_modified</key>\r\n";
					Xmlplist +=L"<date>";
					Xmlplist +=getNowTimeStr();
					Xmlplist +=L"</date>\r\n";
					Xmlplist +=L"</dict>\r\n";
					Xmlplist +=L"<key>operation</key>\r\n";
					Xmlplist +=L"<string>update_playlist</string>\r\n";
					Xmlplist +=L"<key>pid</key>\r\n";
					Xmlplist +=L"<integer>";
					Xmlplist +=Int64ToStr(playlistPid);
					Xmlplist +=L"</integer>\r\n";
					Xmlplist +=L"</dict>\r\n";
				}
			}

		}
	return Xmlplist;
}
bool ciPodManager5::IsAlbumEmpty(__int64 pid)
{
	bool ret = false;

	BaseTable<DAlbum5> tAlbum5(&sql);

	tAlbum5.Select(pid,L"album_pid");
	if(tAlbum5.GetRowCount() > 1)
	{
		ret = true;
	}
	return ret;
}
wstring ciPodManager5::CreatePlistCreatePlayListBody(wstring name,__int64 pid)
{

	wstring Xmlplist =L"<dict>\r\n";
	Xmlplist+=L"	<key>container</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>date_created</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+=	getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	<key>date_modified</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+= getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	<key>name</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=name;
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>container_ui</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>has_been_shuffled</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	<key>is_reversed</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	<key>play_order</key>\r\n";
	Xmlplist+=L"	<integer>1</integer>\r\n";
	Xmlplist+=L"	<key>shuffle_items</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>operation</key>\r\n";
	Xmlplist+=L"	<string>insert_playlist</string>\r\n";
	Xmlplist+=L"	<key>pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=Int64ToStr(pid);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	return Xmlplist;
}

__int64  ciPodManager5::CreateNewPlayListPid()
{
		BaseTable<DContainer5> tContainer(&sql);		
		int ret = 1;	
		__int64 Containerid;
		do{
			Containerid=iPodDbBase::Create64BitID();
			tContainer.Select(Containerid,L"container_pid");		
			ret =tContainer.GetRowCount();
		}while( ret != 0);
	return Containerid;
}

wstring ciPodManager5::CreatePlistDelPlayListBody(wstring name,__int64 pid)
{
	wstring Xmlplist =L"<dict>\r\n";
		Xmlplist+=L"<key>operation</key>\r\n";
		Xmlplist+=L"<string>delete_playlist</string>\r\n";
		Xmlplist+=L"<key>pid</key>\r\n";
		Xmlplist+=L"<integer>";
		Xmlplist+=Int64ToStr(pid);
		Xmlplist+=L"</integer>\r\n";
		Xmlplist+=L"</dict>\r\n";
	return Xmlplist;
}

__int64 ciPodManager5::FindPidByName(wstring playListName)
{
	__int64 ret = 0;

	BaseTable<DContainer5> tContainer(&sql);
	wstring condition = L" name= '";
	condition+=playListName;
	condition+=L"'";
	tContainer.Select(condition);
	if(tContainer.GetRowCount() > 0)
	{
		DContainer5* pContainer = tContainer.GetDataRow(0);
		ret = pContainer->container_pid;
	}
	tContainer.Clear();	
	
  return ret;
	
}

wstring ciPodManager5::CreatePlistModifyPlayListBody(wstring name,__int64 pid,vector<__int64> trackPid)
{
	wstring Xmlplist=L"<dict>\r\n";
	Xmlplist+=L"	<key>container</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>date_created</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+=getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	<key>date_modified</key>\r\n";
	Xmlplist+=L"	<date>";
	Xmlplist+=getNowTimeStr();
	Xmlplist+=L"</date>\r\n";
	Xmlplist+=L"	<key>name</key>\r\n";
	Xmlplist+=L"	<string>";
	Xmlplist+=name;
	Xmlplist+=L"</string>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>container_ui</key>\r\n";
	Xmlplist+=L"	<dict>\r\n";
	Xmlplist+=L"	<key>album_field_order</key>\r\n";
	Xmlplist+=L"	<integer>1</integer>\r\n";
	Xmlplist+=L"	<key>has_been_shuffled</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	<key>is_reversed</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	<key>play_order</key>\r\n";
	Xmlplist+=L"	<integer>1</integer>\r\n";
	Xmlplist+=L"	<key>shuffle_items</key>\r\n";
	Xmlplist+=L"	<false/>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	Xmlplist+=L"	<key>item_to_container</key>\r\n";
	Xmlplist+=L"	<array>\r\n";
	for(size_t i=0;i<trackPid.size();i++)
	{
		Xmlplist+=L"	<integer>";
		Xmlplist+=Int64ToStr(trackPid[i]);
		Xmlplist+=L"</integer>\r\n";
	}
	Xmlplist+=L"	</array>\r\n";
	Xmlplist+=L"	<key>operation</key>\r\n";
	Xmlplist+=L"	<string>update_playlist</string>\r\n";
	Xmlplist+=L"	<key>pid</key>\r\n";
	Xmlplist+=L"	<integer>";
	Xmlplist+=Int64ToStr(pid);
	Xmlplist+=L"</integer>\r\n";
	Xmlplist+=L"	</dict>\r\n";
	return Xmlplist;
}


#endif