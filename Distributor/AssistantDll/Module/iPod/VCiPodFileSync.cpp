#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Module/iPod/VCiPodFileSync.h"
#include "HashVerify.h"

#include "Core/DeviceInfo.h"
#include "Device/IosUsbSync.h"
#include "Module/File/UploadFileCommand.h"
#include "Module/File/DirectoryOperationCommand.h"
#include "Module/File/DownloadFileCommand.h"

#define  IOS_4_ITUNES_DB_FILE_PATH L"/iTunes_Control/iTunes/iTunes Library.itlp/"
#define  IOS_5_CDBFILE_PATH  L"/iTunes_Control/iTunes/iTunesCDB"
#define  IOS_4_MUSIC_PATH L"/iTunes_Control/Music/"
#define  IOS_4_ARTWORK_PATH  L"/iTunes_Control/Artwork/"
#define  IOS_4_CDBFILE_PATH  L"/iTunes_Control/iTunes/iTunesCDB"
#define  IOS_4_JAB_PATH L"/private/var/mobile/Media"
#define  IOS_2_JAB_PATH L"/private/var/root/Media"
DeviceData* iPodFileSync::_pDevice=NULL;
wstring iPodFileSync::_sqlDbPath=L"";
wstring iPodFileSync::DeviceiTunsPath=L"";
wstring iPodFileSync::_UUID=L"";
wstring iPodFileSync::GetDevicePath(wstring FileName)
{
	wstring stringret;
	if(_pDevice->GetDevInfo()->_ios_bJailBreaked)
	{
		stringret = DeviceiTunsPath+FileName;
	}
	else
		stringret = FileName;
	return stringret;
}
void iPodFileSync::UpLoadCDB()
{
	if(GetDBVer() >=430 ||  HashVerify::CheckCBDFile(_sqlDbPath) == S_OK)
	{		
		DirectoryOperationCommand cmd(_pDevice);		
		UploadFileCommand uploadfilecmd(_pDevice);
		wstring localfile1 = _sqlDbPath + L"\\iTunes\\iTunesCDB";
		wstring remotFile1 = GetDevicePath(IOS_5_CDBFILE_PATH);
		
		DirectoryOperationCommand diroperCmd(_pDevice);
		if(CFileOperation::IsFileExist(localfile1.c_str()))
			diroperCmd.DeleteFileLink(remotFile1);
		uploadfilecmd.Upload(localfile1,remotFile1);//暂时屏蔽上传CDB文件
	}

}
void iPodFileSync::InitFileSync(DeviceData* Device,wstring sqlDbPath)
{

	_pDevice = Device;
	_sqlDbPath = sqlDbPath;
	_UUID = _pDevice->GetDevInfo()->_ios_deviceIdentifier.c_str();
	if(iPodFileSync::GetDBVer() > 300)
		DeviceiTunsPath = IOS_4_JAB_PATH;
	else
		DeviceiTunsPath = IOS_2_JAB_PATH;
}
int iPodFileSync::GetDBVer()
{
	wstring replay;
	replay= CStrOperation::ReplaceW(_pDevice->GetDevInfo()->_deviceProductVersion,L".",L"");
	while (replay.length()<3)
	{
		replay+=L"0";
	}
	return _wtoi(replay.c_str());
}
void iPodFileSync::UpdateDevItunesDB()
{
	UploadFileCommand uploadfilecmd(_pDevice);
	//CBD文件
	wstring localfile1 = _sqlDbPath + L"\\iTunes\\iTunesCDB";
	wstring remotFile1=GetDevicePath(IOS_4_CDBFILE_PATH);
	
	//localtions校验文件
	wstring localfile2 =_sqlDbPath + L"\\iTunes\\iTunes Library.itlp\\Locations.itdb.cbk";
	wstring remotFile2=GetDevicePath(IOS_4_ITUNES_DB_FILE_PATH);
	remotFile2 += L"Locations.itdb.cbk";

	//数据库文件
	wstring localfile3 =_sqlDbPath + L"\\iTunes\\iTunes Library.itlp\\Locations.itdb";
	wstring remotFile3=GetDevicePath(IOS_4_ITUNES_DB_FILE_PATH);	
	remotFile3 += L"Locations.itdb";

	wstring localfile4 =_sqlDbPath + L"\\iTunes\\iTunes Library.itlp\\Library.itdb";
	wstring remotFile4=GetDevicePath(IOS_4_ITUNES_DB_FILE_PATH);
	remotFile4 += L"Library.itdb";
	//封面文件
	wstring localfile5 =_sqlDbPath +  L"\\Artwork\\ArtworkDBNew";
	wstring remotFile5=GetDevicePath(IOS_4_ARTWORK_PATH);
	remotFile5 += L"ArtworkDB";

	wstring localfile6 =_sqlDbPath +  L"\\Artwork\\F3006_1.ithmb";
	wstring remotFile6=GetDevicePath(IOS_4_ARTWORK_PATH);
	remotFile6 += L"F3006_1.ithmb";

	wstring localfile7 =_sqlDbPath +  L"\\Artwork\\F3005_1.ithmb";
	wstring remotFile7=GetDevicePath(IOS_4_ARTWORK_PATH);;
	remotFile7 += L"F3005_1.ithmb";

	wstring localfile8 =_sqlDbPath +  L"\\Artwork\\F3001_1.ithmb";
	wstring remotFile8=GetDevicePath(IOS_4_ARTWORK_PATH);
	remotFile8 += L"F3001_1.ithmb";

	wstring localfile9 =_sqlDbPath +  L"\\Artwork\\F3002_1.ithmb";
	wstring remotFile9=GetDevicePath(IOS_4_ARTWORK_PATH);
	remotFile9 += L"F3002_1.ithmb";

	wstring localfile10 =_sqlDbPath +  L"\\Artwork\\F3007_1.ithmb";
	wstring remotFile10=GetDevicePath(IOS_4_ARTWORK_PATH);
	remotFile10 += L"F3007_1.ithmb";

	bool ret;

	DirectoryOperationCommand diroperCmd(_pDevice);
	if(CFileOperation::IsFileExist(localfile1.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile1);
	if(CFileOperation::IsFileExist(localfile2.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile2);
	if(CFileOperation::IsFileExist(localfile3.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile3);
	if(CFileOperation::IsFileExist(localfile4.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile4);
	if(CFileOperation::IsFileExist(localfile5.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile5);
	if(CFileOperation::IsFileExist(localfile6.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile6);
	if(CFileOperation::IsFileExist(localfile7.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile7);
	if(CFileOperation::IsFileExist(localfile8.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile8);
	if(CFileOperation::IsFileExist(localfile9.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile9);
	if(CFileOperation::IsFileExist(localfile10.c_str()))
		ret = diroperCmd.DeleteFileLink(remotFile10);
	diroperCmd.MakeDir(CFileOperation::GetFileDir(remotFile5));

	uploadfilecmd.Upload(localfile1,remotFile1);
	uploadfilecmd.Upload(localfile2,remotFile2);
	uploadfilecmd.Upload(localfile3,remotFile3);
	uploadfilecmd.Upload(localfile4,remotFile4);
	uploadfilecmd.Upload(localfile5,remotFile5);
	uploadfilecmd.Upload(localfile6,remotFile6);
	uploadfilecmd.Upload(localfile7,remotFile7);
	uploadfilecmd.Upload(localfile8,remotFile8);
	uploadfilecmd.Upload(localfile9,remotFile9);
	uploadfilecmd.Upload(localfile10,remotFile10);
	wstring localfileCp =_sqlDbPath +  L"\\Artwork\\ArtworkDB";
    bool cpyret = CFileOperation::CopyFile( localfile5, localfileCp, false );
}


void iPodFileSync::UploadPlayListDB(wstring SrcFile)
{
	if(!CFileOperation::IsFileExist(SrcFile.c_str()))
		return;
	wstring remotFile=GetDevicePath(IOS_4_ITUNES_DB_FILE_PATH);
	remotFile += L"Library.itdb";
	IosUsbSync usbSync(_pDevice->GetDevInfo());			
	usbSync.StartSync();
	DirectoryOperationCommand diroperCmd(_pDevice);
	bool ret = diroperCmd.DeleteFileLink(remotFile);
	UploadFileCommand uploadfilecmd(_pDevice);
	uploadfilecmd.Upload(SrcFile,remotFile);
	usbSync.StopSync();
}
void iPodFileSync::DownLodMusic(wstring localfile,wstring remotfile)
{
	wstring remotFile1=GetDevicePath(IOS_4_MUSIC_PATH);
		remotFile1+=remotfile;

	DownloadFileCommand downloadcmd(_pDevice);
	downloadcmd.Download(localfile,remotFile1);		
}
void iPodFileSync::UpLoadFile(wstring localfile,wstring remotfile)
{
	
	wstring remotFile1=GetDevicePath(IOS_4_MUSIC_PATH);
		remotFile1+=remotfile;

	IosUsbSync usbSync(_pDevice->GetDevInfo());			
	usbSync.StartSync();	
	
	DirectoryOperationCommand diroperCmd(_pDevice);
	if(CFileOperation::IsFileExist(localfile.c_str()))
		diroperCmd.DeleteFileLink(remotFile1);
	diroperCmd.MakeDir(CFileOperation::GetFileDir(remotFile1));
	UploadFileCommand uploadfilecmd(_pDevice);
	uploadfilecmd.Upload( localfile ,remotFile1);
	UpdateDevItunesDB();	
	usbSync.StopSync();
}

void iPodFileSync::DeleteMusicFile(wstring filename)
{
	wstring remotfile=GetDevicePath(IOS_4_MUSIC_PATH);
		remotfile+=filename;
	//同步手机
	IosUsbSync usbSync(_pDevice->GetDevInfo());			
	usbSync.StartSync();	 
	DirectoryOperationCommand diroperCmd(_pDevice);
	diroperCmd.DeleteFileLink(remotfile);
	wstring pcMusicPath = _sqlDbPath + L"\\Music\\";
	pcMusicPath+=filename;
    CFileOperation::DeleteFile(pcMusicPath);
	UpdateDevItunesDB();	
	usbSync.StopSync();

}
int iPodFileSync::GetAFCHandle()
{
//	const DeviceInfo* devinfo =  _pDevice->GetDevInfo();
	if(!_pDevice)
		return -1;
	int ret = (int)_pDevice->GetDevInfo()->_ios_afcHandleApp;
	return ret;
}
wstring iPodFileSync::GetUUID()
{
	return _UUID;
}


#endif