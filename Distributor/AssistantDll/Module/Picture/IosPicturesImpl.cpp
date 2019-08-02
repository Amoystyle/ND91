#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPicturesImpl.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Picture/IosPictureSqlite4.h"
#include "Module/Picture/IosPictureSqlite5.h"

#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Common/Path.h"

#include "Monitor/ProgressObserver.h"
#include "Monitor/ProgressSubject.h"

#define WALLPAPER_MIN_NUMBER L"300"

IosPicturesImpl::IosPicturesImpl( const DeviceData* pDevice)
: _pDevice(pDevice)
{
	_nVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);

	_pWallpaper = new IosWallpaperOper(pDevice);
	_ios4SQL	= _nVersion>=400&&_nVersion<500 ? new IosPictureSqlite4(pDevice) : NULL;
	_ios5SQL	= _nVersion>=500 ? new IosPictureSqlite5(_pDevice) : NULL;

	_pIosDB		= NULL;	
	_pDownCmd	= NULL;

	_UploadPictureFiles	= new map<wstring, wstring>;
	_DeletePictureFiles = new vector<wstring>;

	ResetSyncState();	

}

IosPicturesImpl::~IosPicturesImpl()
{
	SAFE_DELETE(_ios4SQL);
	SAFE_DELETE(_ios5SQL);
	SAFE_DELETE(_pIosDB);
	SAFE_DELETE(_pDownCmd);
	SAFE_DELETE(_pWallpaper);
	SAFE_DELETE(_UploadPictureFiles);
	SAFE_DELETE(_DeletePictureFiles);
}

bool IosPicturesImpl::Init()
{
	RETURN_FALSE_IF( !DownloadInitFiles() );

	vector<int> thumbTypes = GetThumbNames();
	_pIosDB = new IosPhotoDB(_pDevice->GetIosPicturePath()->GetLocalPhotoDatabase(), thumbTypes);

	return true;
}

wstring IosPicturesImpl::MakeFilename( PICTURE_TYPE t, int fileNumber )
{
    wchar_t buffer[16];
    wstring format = t == PICTURE_TYPE_PHOTO ? L"IMG_%04d" : L"%d";

    swprintf_s(buffer, 16, format.c_str(), fileNumber);
    return buffer;
}

void IosPicturesImpl::LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot)
{
	vector<PictureData*> pictures;
	RETURN_IF ( !_ios4SQL->GetPhotoFromSqlite(&pictures) );


	// JPG为照片, PNG为截图
	for(size_t i =0; i < pictures.size(); ++i)
	{
		PictureData* pPicture = pictures[i];

		if ( _nVersion < 421 )  // 421以上缩略图是放在ithmb文件中的, 没有路径
		{
			wstring fileid = pPicture->_name.substr(0,pPicture->_name.length()-4);
			pPicture->_thumbRemotePath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + fileid + THMPIC;
			pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalAlbumPath() + fileid + THMPIC;
		}

		int pos = pPicture->_name.rfind('.');
		if ( string::npos == pos )
			continue;

		if ( CStrOperation::endsWithW(pPicture->_name, L".PNG") )
		{
			pPicture->SetType(PICTURE_TYPE_SCREENSHOT);
			pPicture->SetCategoryId(SCREENSHOTID);
			pScreenshot->InsertPicture(pPicture);
		}
		else
		{
			pPicture->SetType(PICTURE_TYPE_PHOTO);
			pPicture->SetCategoryId(CAMERAID);
			pCamera->InsertPicture(pPicture);
		}
	}

	RELEASE_VECTOR(pictures);
}

bool IosPicturesImpl::LoadPicturesByGroup4X( PictureGroup* pGroup, vector<PictureData*>& pVerDatas )
{
	vector<int>* pPicIds = _pIosDB->GetCategoryImages(atoi(pGroup->GetId().c_str()));

	RETURN_FALSE_IF ( !pPicIds || pPicIds->empty() )
	
	for (size_t i = 0;i < pPicIds->size(); ++i )
	{
		PictureData* pData = new PictureData;
		pData->SetId(pPicIds->at(i));
		pData->SetType(PICTURE_TYPE_NORMAL);
		pData->SetCategoryId(pGroup->GetId());

		wstring sourceFile = _pIosDB->GetImageFilePath(pPicIds->at(i), '/');
		pData->_name = CFileOperation::GetFileName(sourceFile);
		pData->_remotePath = _pDevice->GetIosPicturePath()->GetIosThumbsPath() + sourceFile;

		sourceFile = _pIosDB->GetImageFilePath(pPicIds->at(i), '\\');
		pData->_localPath  = _pDevice->GetIosPicturePath()->GetLocalThumbsPath() + sourceFile;
		pData->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath() + CFileOperation::GetGuidW() + PICJPG;

		pVerDatas.push_back(pData);
		CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(pData->_localPath));			
	}
	return true;
}

vector<PictureData*>* IosPicturesImpl::AddPictureToGroup4X( vector<wstring>* localPaths, const string groupId,
														    const string firstGroupId )
{	
	vector<PictureData*>* vPictures = new vector<PictureData*>;
	for ( size_t i=0;i< localPaths->size();++i )
	{
		// 向photo database中添加图片, 并得到图片id
		Bitmap* pImage = Gdiplus::Bitmap::FromFile(localPaths->at(i).c_str());
		RETURN_FALSE_IF( ! pImage );

		int pictureId = _pIosDB->AddImage(pImage);
		RETURN_FALSE_IF( _pIosDB->AddImageToCategory(pictureId, atoi(groupId.c_str())));

		// 写完Photodatabase后才能获得路径,ios4以上版本才有原文件
		_pIosDB->Write(_pDevice->GetIosPicturePath()->GetLocalPhotoDatabase());
		wstring sourceFile = _pIosDB->GetImageFilePath(pictureId, '\\');

		PictureData* pData = new PictureData;
		pData->Create(
					  CFileOperation::GetFileName(sourceFile),
					  0,0,0,0,-1,pictureId,-1,
					  _pDevice->GetIosPicturePath()->GetIosThumbsPath() + sourceFile,
					  _pDevice->GetIosPicturePath()->GetLocalThumbsPath() + sourceFile,
					  _pDevice->GetIosPicturePath()->GetLocalThumbnailPath() + CFileOperation::GetGuidW() + PICJPG,
					  L"",
					  PICTURE_TYPE_NORMAL
					  );	
		pData->SetCategoryId(groupId);
		vPictures->push_back(pData);

		if ( groupId != firstGroupId &&
			( !_pDevice->GetDevInfo()->IsPad() || !_pDevice->GetDevInfo()->IsPod()) )
		{
			_pIosDB->AddImageToCategory(pData->GetId(), atoi(firstGroupId.c_str()) );
		}
	}
	_bPhotoDataBaseChange = _bPhotosIthmbChange = true;
	return vPictures;
}

bool IosPicturesImpl::UploadFiles4X(ProgressSubject* pSubject, int percent)
{
	// 获取4.x图库上传图片的ID
	list<int> imageIds = _pIosDB->GetUploadFiles();
	int total=imageIds.size();
	int curcount=0;
	for(list<int>::iterator it = imageIds.begin(); it != imageIds.end(); ++it)
	{
		if(pSubject)
		{
			IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver()));
		}
		wstring sourcePath	= _pDevice->GetIosPicturePath()->GetLocalThumbsPath() + _pIosDB->GetImageFilePath(*it, '\\');
		wstring destPath	= _pDevice->GetIosPicturePath()->GetIosThumbsPath() + _pIosDB->GetImageFilePath(*it, '/');

		// 目录可能存在, 也可能不存在
		FileHelper::CreateDir(_pDevice, CFileOperation::GetFileDir(destPath));
		// 上传原图文件
		RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( sourcePath, destPath ) != Transfer_Success );
		curcount++;
		if(pSubject&&(percent*curcount/total>=1))
		{
			pSubject->AddTaskProgress(percent*curcount/total);
			pSubject->SendProgress();
			curcount=0;
		}
	}
	return true;
}

bool IosPicturesImpl::DeleteFiles4X()
{	
	// 获取4.x图库删除图片的文件名
	vector<wstring> picFiles = _pIosDB->GetDeleteFiles();
	for (size_t i=0;i<picFiles.size();++i)
	{
		wstring destPath = _pDevice->GetIosPicturePath()->GetIosThumbsPath() + picFiles[i];
		RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile(destPath) );
	}
	return true;
}

bool IosPicturesImpl::PhotodatabaseSync()
{
	_pIosDB->Write( _pDevice->GetIosPicturePath()->GetLocalPhotoDatabase() );

	wstring sourcePath	= _pDevice->GetIosPicturePath()->GetLocalPhotoDatabase();
	wstring destPath	= _pDevice->GetIosPicturePath()->GetIosPhotoDatabasePath();
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(sourcePath, destPath) != Transfer_Success );

	return true;
}

bool IosPicturesImpl::SqilteDBSync()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(_pDevice->GetIosPicturePath()->GetLocalSqlitePath(),
		_pDevice->GetIosPicturePath()->GetIosSqlitePath()) != Transfer_Success );
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(_pDevice->GetIosPicturePath()->GetLocalSqlitePath(),
		_pDevice->GetIosPicturePath()->GetIosSqliteAsidePath()) != Transfer_Success );

	RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->ChangeMode(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), "644", MOBILEUSER) );
	RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->ChangeMode(_pDevice->GetIosPicturePath()->GetIosSqliteAsidePath(), "644", MOBILEUSER) );	

	return true;
}

bool IosPicturesImpl::PhotoDataIthmbSync(ProgressSubject* pSubject, int percent)
{
	// (相册缩略图文件)
	map<wstring, wstring> mPicFiles = GetPhotoAllThumbs(true);

	int totalsize=0;
	for (map<wstring,wstring>::iterator itr=mPicFiles.begin();itr!=mPicFiles.end();itr++)
	{
		totalsize+=CFileOperation::GetFileSizeW(itr->first);
	}
	map<wstring, wstring>::iterator it = mPicFiles.begin();
	for(;it!=mPicFiles.end();++it)
	{
		if(pSubject)
		{
			IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver()));
		}
		int filesize=CFileOperation::GetFileSizeW(it->first) ;
		if (filesize > 0 )
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(it->first, it->second) != Transfer_Success );
		if (pSubject&&totalsize>0&&filesize>0)
		{
			pSubject->AddTaskProgress(percent*filesize/totalsize);
			pSubject->SendProgress();
		}

		if (_nVersion >= 421)
			RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->ChangeMode(it->second, "644", MOBILEUSER) );

	}
	return true;
}

bool IosPicturesImpl::PhotosIthmbSync(ProgressSubject* pSubject, int percent)
{
	// (图库缩略图文件)
	vector<wstring> thumbFiles = _pIosDB->GetThumbFileName();
	int totalsize=0;
	for (size_t i=0;i<thumbFiles.size();i++)
	{
		wstring sourcePath = _pDevice->GetIosPicturePath()->GetLocalThumbsPath() + thumbFiles[i];
		totalsize+=CFileOperation::GetFileSizeW(sourcePath);
	}
	for(size_t i = 0; i < thumbFiles.size(); ++i)
	{
		if(pSubject)
		{
			IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver()));
		}
		wstring sourcePath = _pDevice->GetIosPicturePath()->GetLocalThumbsPath() + thumbFiles[i];
		wstring destPath = _pDevice->GetIosPicturePath()->GetIosThumbsPath() + thumbFiles[i];
		int filesize=CFileOperation::GetFileSizeW(sourcePath);
		if ( filesize > 0 )
		{
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(sourcePath, destPath) != Transfer_Success );
			if (pSubject&&totalsize>0)
			{
				pSubject->AddTaskProgress(percent*filesize/totalsize);
				pSubject->SendProgress();
			}
		}

	}
	return true;
}

map<wstring, wstring> IosPicturesImpl::GetPhotoAllThumbs(bool isPhotoData )
{
	// 下载PhotoData/Thumbnails目录下的所有.ithmb文件
	DirData root(_pDevice);
	wstring iosDir = isPhotoData ? _pDevice->GetIosPicturePath()->GetIosAlbumThumbsPath() : 
									  _pDevice->GetIosPicturePath()->GetIosThumbsPath();	
	wstring localDir   = isPhotoData ? _pDevice->GetIosPicturePath()->GetLocalAlbumThumbsPath() :
								      _pDevice->GetIosPicturePath()->GetLocalThumbsPath(); 

	DirData* pFolder = dynamic_cast<DirData*>(FileHelper::GetNode(&root, iosDir));

	if ( pFolder )
	{
		deque<NodeData*> pChilds = pFolder->GetChildrens();
		map<wstring, wstring> mPicFiles;
		for(size_t i = 0; i < pChilds.size(); ++i )
		{
			NodeData* pNode = pChilds[i];
			if ( pNode->_type == NodeData::FILE &&
				CStrOperation::endsWithW(pNode->_name, L".ithmb"))
			{
				wstring iosPath = iosDir + pNode->_name;
				wstring localPath = localDir + pNode->_name;

				mPicFiles.insert(pair<wstring,wstring>(localPath, iosPath));
			}
		}
		return mPicFiles;
	}
	else
		return map<wstring, wstring>();
}

void IosPicturesImpl::ReleaseCase()
{
	_UploadPictureFiles->clear();
	_DeletePictureFiles->clear();
}

map<int, wstring>* IosPicturesImpl::LoadGalleryBucket()
{
	RETURN_NULL_IF( ! _pIosDB );
	
	return  _pIosDB->GetAllCategory();
}

int IosPicturesImpl::AddGalleryGroup( const wstring displayname, wstring path )
{
	_bPhotoDataBaseChange = true;

	return _pIosDB->AddCategory(displayname);	
}

bool IosPicturesImpl::RenameGalleryGroup( const string nGroupid, const wstring name )
{
	_pIosDB->RenameCategory(atoi(nGroupid.c_str()), name);
	_bPhotoDataBaseChange = true;
	return true;
}

bool IosPicturesImpl::DeleteGalleryGroup( int groupId, vector<PictureData*>* VerPictDatas )
{
	_pIosDB->DelCategory(groupId);
	for ( size_t i=0;i<VerPictDatas->size();i++ )
		_pIosDB->DelImage(VerPictDatas->at(i)->GetId(), true);

	_bPhotoDataBaseChange = true;
	return true;
}


bool IosPicturesImpl::DeleteGalleryPictures( const vector<const PictureData*>* vPictures)
{
	for (size_t i=0; i<vPictures->size(); ++i)
	{
		RETURN_FALSE_IF ( ! _pIosDB->DelImage(vPictures->at(i)->GetId(), true) );
	}

	_bPhotoDataBaseChange = true;
	return true;
}

bool IosPicturesImpl::RemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId )
{
	for (size_t i =0; i < vPictures->size(); ++i)
	{
		RETURN_FALSE_IF( ! _pIosDB->RemoveFromCategory(vPictures->at(i)->GetId(), atoi(groupId.c_str())) );
	}
	_bPhotoDataBaseChange = true;
	return true;
}

bool IosPicturesImpl::IosAddExistPictures( const vector<const PictureData*>* vPictures, const string groupId )
{
	for (size_t i =0; i < vPictures->size(); ++i)
	{
		_pIosDB->AddImageToCategory(vPictures->at(i)->GetId(), atoi(groupId.c_str()));
	}

	_bPhotoDataBaseChange = true;
	return true;
}

bool IosPicturesImpl::MovePictures( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId )
{
	for (size_t i =0; i < vPictures->size(); ++i)
	{
		_pIosDB->MoveImageBetweenCategory(vPictures->at(i)->GetId(),
			atoi(sourceGroupId.c_str()),
			atoi(destGroupId.c_str()));
	}

	_bPhotoDataBaseChange = true;
	return true;
}

bool IosPicturesImpl::UpdateGalleryPicture( PictureData* pPicture, wstring srcPath )
{
	Bitmap* pImage = Gdiplus::Bitmap::FromFile(srcPath.c_str());
	RETURN_FALSE_IF( ! pImage );

	_pIosDB->UpdateImage(pPicture->GetId(), pImage);

	const wchar_t* thumbName = pPicture->_thumbLocalPath.c_str();
    CFileOperation::DeleteFile(thumbName);

	Image* pThumb = IthmbDB::MakeThumbnail(pImage, true, pPicture->_thumbWidth, pPicture->_thumbHeight);
	DUMP_IMAGE(pThumb, thumbName);
	SAFE_DELETE(pThumb);

	_bPhotoDataBaseChange = _bPhotosIthmbChange = true;
	return true;
}


bool IosPicturesImpl::DownloadGalleryFiles()
{
	map<wstring, wstring> mPicFiles = GetPhotoAllThumbs(false);
	map<wstring, wstring>::iterator it = mPicFiles.begin();

	for(;it!=mPicFiles.end();++it)
		RETURN_FALSE_IF ( !_pWallpaper->IosDownPictureFile(it->second, it->first) );

	if ( _pWallpaper->DirCommand()->Exist(_pDevice->GetIosPicturePath()->GetIosPhotoDatabasePath()))
	{
		RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosPhotoDatabasePath(), 
						_pDevice->GetIosPicturePath()->GetLocalPhotoDatabase()) != Transfer_Success );
	}
	else
	{
		if ( !_pWallpaper->DirCommand()->Exist(_pDevice->GetIosPicturePath()->GetIosPhotosPath()) )
			RETURN_FALSE_IF ( _pWallpaper->DirCommand()->MakeDir(_pDevice->GetIosPicturePath()->GetIosPhotosPath()) );

		wstring pdbFile = CFileOperation::GetCurrWorkingDir() + _T("Photo Database");
		::CopyFileW(pdbFile.c_str(), _pDevice->GetIosPicturePath()->GetLocalPhotoDatabase().c_str(), true);
	}

	return true;
}

bool IosPicturesImpl::DownloadPhotoDataThumbs()
{
	// (相册缩略图文件)
	map<wstring, wstring> mPicFiles = GetPhotoAllThumbs(true);
	map<wstring, wstring>::iterator it = mPicFiles.begin();

	for(;it!=mPicFiles.end();++it)
		RETURN_FALSE_IF ( !_pWallpaper->IosDownPictureFile(it->second, it->first) );
	
	return true;
} 

bool IosPicturesImpl::DownloadPicture( const PictureData* pPicture, wstring customPath )
{
	wstring path;

	if (customPath.empty())
		path = pPicture->_localPath;
	else
		path = customPath;

	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(pPicture->_remotePath) )
		RETURN_FALSE_IF ( !_pWallpaper->IosDownPictureFile(pPicture->_remotePath, path) );	
	
	return true;
}

bool IosPicturesImpl::DownloadGalleryThumbnails(PictureData* pPicture)
{
	Image* pImage = _pIosDB->GetImage(pPicture->GetId(), F3011);
	if ( ! pImage )
	{
		pImage = _pIosDB->GetImage(pPicture->GetId(), F3111);
		if ( ! pImage )
		{
			pImage = _pIosDB->GetImage(pPicture->GetId(), F3143);
			if ( ! pImage )
			{
				pImage = _pIosDB->GetImage(pPicture->GetId(), F3041);
				RETURN_FALSE_IF(NULL == pImage);	
			}
		}
	}

	DUMP_IMAGE(pImage, pPicture->_thumbLocalPath.c_str());
	pPicture->_thumbWidth = pImage->GetWidth();
	pPicture->_thumbHeight= pImage->GetHeight();

	return true;
}

bool IosPicturesImpl::DownloadSpecialThumbnails(PictureData* pPicture)
{
	RETURN_FALSE_IF ( -1 == pPicture->_thumbnailIndex);

	IthmbDB* pThumb;
	wstring thumbFile = _pDevice->GetIosPicturePath()->GetLocal158IthmbPath();
	if ( CFileOperation::IsFileExist(thumbFile.c_str()))
	{
		pThumb = new IthmbDB(X158, thumbFile);
	}
	else
	{
		thumbFile = _pDevice->GetIosPicturePath()->GetLocal79IthmbPath();
		RETURN_FALSE_IF( ! CFileOperation::IsFileExist(thumbFile.c_str()));
		pThumb = new IthmbDB(X79, thumbFile);
	}

	Image* pImage = pThumb->ReadImage(pPicture->_thumbnailIndex);
	SAFE_DELETE(pThumb);
	RETURN_FALSE_IF ( ! pImage );

	if ( pPicture->_thumbLocalPath.empty() )  // 没有名字就取个名字
	{
		pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalAlbumPath();
		pPicture->_thumbLocalPath += CFileOperation::GetFileNameNoExt(pPicture->_localPath) + THMPIC;
	}

	DUMP_IMAGE(pImage, pPicture->_thumbLocalPath.c_str());
	pPicture->_thumbWidth = pImage->GetWidth();
	pPicture->_thumbHeight= pImage->GetHeight();

	return true;
}


vector<int> IosPicturesImpl::GetThumbNames()
{
	vector<int> vec;

    if ( _pDevice->GetDevInfo()->IsPad() )
    {
        vec.push_back(3004);
        vec.push_back(3009);
        vec.push_back(3011);
        vec.push_back(4010);
        vec.push_back(4001);
        vec.push_back(4002);
        vec.push_back(4005);
    }
    else if ( _pDevice->GetDevInfo()->IsPod() )
    {
        vec.push_back(3009);
        vec.push_back(3039);
        vec.push_back(3111);
        vec.push_back(3113);
        vec.push_back(3141);
        vec.push_back(3143);
        vec.push_back(4010);
    }
	else
		vec = GetThumbType();

	// 取文件名
	vector<wstring> thumbFiles;
	CFileOperation::FindAllFileInDir(_pDevice->GetIosPicturePath()->GetLocalThumbsPath(), thumbFiles);
	for(vector<wstring>::iterator it = thumbFiles.begin(); it != thumbFiles.end(); ++it)
	{
		wstring filename = *it;
		if ( filename.substr(filename.size() - 8) != L"_1.ithmb" )
			continue;

		int thumbId = _wtoi(filename.substr(filename.size() - 12, 4).c_str());

		if ( thumbId <=0 || thumbId >= 10000)
			continue;

		if ( find(vec.begin(), vec.end(), thumbId) == vec.end() )
		{
			vec.push_back(thumbId);
		}
	}

	return vec;	
}

int IosPicturesImpl::GetAllPicturesCount()
{
	return GetSpecialPhotos() + GetWallpaperPicCount();
}

int IosPicturesImpl::GetSpecialPhotos()
{
	return GetCameraCount4x() + GetGalleryPicCount(); 
}

int IosPicturesImpl::GetCameraCount4x()
{
	int nCount = 0;
	// 如果不存在目录，创建存放临时数据的目录
	RETURN_VALUE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalMediaPath()), 0 );
	
	wstring path =_pDevice->GetIosPicturePath()->GetLocalSqlitePath();
	if (!CFileOperation::IsFileExist(path.c_str()))
	   RETURN_VALUE_IF( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), path)!=Transfer_Success, 0);

	CppSQLite3DB SQLdb;
	SQLdb.Open(CCodeOperation::UnicodeToUTF_8(path).c_str());

	CppSQLite3Query query = SQLdb.ExecQuery( "SELECT COUNT(*) FROM photo" );

	if(!query.Eof())
	{
		nCount = query.GetIntField(0);
	}
	return nCount;
}

int IosPicturesImpl::GetGalleryPicCount()
{
	int nCount = 0;

	wstring path = _pDevice->GetIosPicturePath()->GetLocalPhotoDatabase();
	RETURN_VALUE_IF ( _pWallpaper->DirCommand()->Exist(_pDevice->GetIosPicturePath()->GetIosPhotoDatabasePath()) != Transfer_Success, 0 );

	// 如果不存在目录，创建各种存放临时数据的目录
	RETURN_VALUE_IF ( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocaliPhotoPath()), 0 );

	if (!CFileOperation::IsFileExist(path.c_str()))
		_pWallpaper->GetIosFileIO()->Ios_DownFromPhone( _pDevice->GetIosPicturePath()->GetIosPhotoDatabasePath(), path);

	vector<int> thumbTypes;
	IosPhotoDB pIosDB(path, thumbTypes);
	nCount = pIosDB.size();

	return nCount;
}

int IosPicturesImpl::GetWallpaperPicCount()
{
	int nCount = 0;

	RETURN_VALUE_IF ( !_pDevice->GetDevInfo()->_ios_bJailBreaked, 0 );

	nCount = GetPicCountByPath( _pDevice->GetIosPicturePath()->GetIosWallPaperPath() );

	if (_pDevice->GetDevInfo()->IsPod())
	{
		nCount += GetPicCountByPath( _pDevice->GetIosPicturePath()->GetIpodWallpaperPath() );
	}
	return nCount;
}

int IosPicturesImpl::GetPicCountByPath( wstring wallPaperPath )
{
	int nCount = 0;
	std::set<int> tagList;

	DirData wallpaperFolder(_pDevice);
	wallpaperFolder._name = wallPaperPath;

	deque<NodeData*> wallpapers = wallpaperFolder.GetChildrens();
	for( size_t i = 0; i < wallpapers.size(); ++i )
	{
		NodeData* pNode = wallpapers[i];

		if ( pNode->_type != NodeData::FILE   || 
			! iswdigit(*pNode->_name.begin()) ||
			! CStrOperation::endsWithW(pNode->_name, L".png")
			)
			continue;

		int tag = atoi(CCodeOperation::UnicodeToGB2312(pNode->_name).c_str());
		if (tag >= 0)
		{
			tagList.insert(tag);
		}
	}
	nCount = tagList.size();
	tagList.clear();

	return nCount;
}

#endif