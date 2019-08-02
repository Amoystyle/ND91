#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPicturesImpl5.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/IosWallpaperOper.h"

#include "IosPictureSqlite5.h"
#include "Module/Common/IosDB/IthmbDB.h"
#include "sqlite/CppSqlite3.h"
#include "Common/Path.h"

#include "Monitor/ProgressObserver.h"
#include "Monitor/ProgressSubject.h"

//写入RingList文件的固定格式部分
const std::string	Plist_Head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
								 <!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\"\
								 \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n\0";



IosPicturesImpl5::IosPicturesImpl5(const DeviceData* pDevice)
:IosPicturesImpl(pDevice)
{
	_ios5HasInit	= false; 
}

IosPicturesImpl5::~IosPicturesImpl5()
{
	_ios5HasInit = false; 
	RELEASE_VECTOR(_vPictures);
}

bool IosPicturesImpl5::Init()
{
	return DownloadInitFiles();
}

bool IosPicturesImpl5::DownloadInitFiles()
{
	if ( ! _ios5HasInit )
	{
		_ios5HasInit = Initialize();
	}
	RETURN_FALSE_IF ( !_ios5HasInit );
	
	return DownloadPhotoDataThumbs();	
}

bool IosPicturesImpl5::Initialize()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), 
		_pDevice->GetIosPicturePath()->GetLocalSqlitePath()) != Transfer_Success );	

	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(_pDevice->GetIosPicturePath()->GetIosShmSqlitePath()) )
		RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosShmSqlitePath(), 
							_pDevice->GetIosPicturePath()->GetLocalShmSqlitePath()) != Transfer_Success );	
	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(_pDevice->GetIosPicturePath()->GetIosWalSqlitePath()) )
		RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosWalSqlitePath(), 
							_pDevice->GetIosPicturePath()->GetLocalWalSqlitePath()) != Transfer_Success );

	return true;
}

bool IosPicturesImpl5::DeleteSpecialPictures( const vector<const PictureData*>* pVecPicture )
{
	RETURN_FALSE_IF ( !pVecPicture || pVecPicture->empty() );

	if ( PICTURE_TYPE_WALLPAPER == pVecPicture->at(0)->GetType() )
	{
		for (size_t i=0; i<pVecPicture->size(); ++i)
		{
			_pWallpaper->DeleteWallpaper421( pVecPicture->at(i) );
		}
	}
	else 
	{
		vector<int> VerPictureID;

		for (size_t i=0; i<pVecPicture->size(); ++i)
		{
			VerPictureID.push_back( pVecPicture->at(i)->GetId() );

			// 放入删除文件集合容器中
			_DeletePictureFiles->push_back(pVecPicture->at(i)->_remotePath);
		}

		// 删除数据库记录
		RETURN_FALSE_IF ( ! _ios5SQL->DeleteIos5Pictures(&VerPictureID) );
		_bSqliteChange = true;
	}

	return true;
}

vector<PictureData*>* IosPicturesImpl5::AddSpecialPictures( vector<wstring>* vPicFiles, int fileNumber, int thumbIndex /*= -1*/, string gruopId )
{
	RETURN_NULL_IF ( !vPicFiles || vPicFiles->empty() );
	vector<PictureData*>* vPictures = new vector<PictureData*>;

	if ( gruopId == WALLPAPERID )
	{
		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			PictureData* pPicture = _pWallpaper->AddWallpaper(vPicFiles->at(i), MakeFilename(PICTURE_TYPE_WALLPAPER, fileNumber++), true);
			vPictures->push_back(pPicture);
		}
	}
	else
	{
		int gID = atoi(gruopId.c_str());
		wstring remoteFolder = gID == 0 ? 
			_pDevice->GetIosPicturePath()->GetIosAlbumPath() : _pDevice->GetIosPicturePath()->GetIos5GalleryPath();
		wstring localFolder	 = gID == 0  ? 
			_pDevice->GetIosPicturePath()->GetLocal100APPLEPath() : _pDevice->GetIosPicturePath()->GetLocalPicPath();
		wstring localthumbFolder = gID == 0  ? 
			_pDevice->GetIosPicturePath()->GetLocalAlbumPath() : _pDevice->GetIosPicturePath()->GetLocalThumbnailPath();

		PICTURE_TYPE type;
		if ( gID == 0 )
		{
			if ( gruopId == CAMERAID )
				type = PICTURE_TYPE_PHOTO;
			else
				type = PICTURE_TYPE_SCREENSHOT;
		}
		else
			type = PICTURE_TYPE_NORMAL;

		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			int thumbid	= thumbIndex;
			wstring newFileName = MakeFilename(PICTURE_TYPE_PHOTO, fileNumber++);
			wstring picExt = CStrOperation::toLowerW(vPicFiles->at(i));
			picExt = CStrOperation::endsWithW(picExt, PICPNG) ? L".PNG" : PICJPG;

			// 拷贝并上传原图
			wstring localFile = localFolder + newFileName + picExt;
			wstring remoteFile = remoteFolder + newFileName + picExt;
            CFileOperation::CopyFile(vPicFiles->at(i), localFile, false);

			// 放入上传文件集合容器中
			_UploadPictureFiles->insert(pair<wstring,wstring>(localFile, remoteFile));

			// 生成缩略图
			Image* img = Gdiplus::Image::FromFile(vPicFiles->at(i).c_str());
			RETURN_NULL_IF ( NULL == img);
			WriteThumbDB(img, thumbid);

			// 创建对象
			PictureData* pPicture = new PictureData(remoteFolder, type);

			time_t now;
			time(&now);
			pPicture->Create( 
							newFileName + picExt,
							now - CDealTime::IosStartTime().GetTime(),
							now - CDealTime::IosStartTime().GetTime(),
							img->GetWidth(),
							img->GetHeight(),
							thumbid,
							-1,
							CFileOperation::GetFileSizeW(vPicFiles->at(i)),
							localFile,
							remoteFile,
							localthumbFolder + newFileName + THMPIC,
							L"",
							type
							);

			vPictures->push_back(pPicture);
			thumbIndex++;
		}
		// 写入数据库
		RETURN_NULL_IF ( ! _ios5SQL->InsertIos5PicturesToDB(vPictures, gruopId) );
		//获得新增图片的uuid
		_pictUuids = _ios5SQL->GetPictureUuids();
		_ios5SQL->ClearTempPictureUuids();

		_bSqliteChange = _bPhotoDataIthmbChange = true;

		// 上传缩略图配置文件
		UpLoadthumbConfig();
	}

	return vPictures;
}

void IosPicturesImpl5::LoadSpecialPictures(PictureFolder* pCamera,
                                        PictureFolder* pWallpaper,
                                        PictureFolder* pScreenshot)
{
    _pWallpaper->LoadWallpaper4(pWallpaper);
	LoadPhotoAndScreenShot( pCamera, pScreenshot );
}

bool IosPicturesImpl5::UpdateSpecialPicture( PictureData* pic , wstring srcPath)
{
	Image* pImg = Gdiplus::Image::FromFile(srcPath.c_str());
	RETURN_FALSE_IF( ! pImg );	

	pic->_modifyTime = time(NULL);
	pic->_height = pImg->GetHeight();
	pic->_width = pImg->GetWidth();
	pic->_size = CFileOperation::GetFileSizeW(srcPath);

	if ( PICTURE_TYPE_WALLPAPER == pic->GetType() )
	{
		RETURN_FALSE_IF( ! _pWallpaper->UpdateWallpaper421(pic, srcPath) );
	}
	else 
	{
		// 修改数据库
		RETURN_FALSE_IF ( ! _ios5SQL->UpdateIos5Pictures(pic) );
        RETURN_FALSE_IF( ! CFileOperation::CopyFile(srcPath, pic->_localPath, false));
		RETURN_FALSE_IF( ! _pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB2_WIDTH, IOS_THUMB2_HEIGHT));
		WriteThumbDB(pImg, pic->_thumbnailIndex);

		// 放入上传文件集合容器中
		_UploadPictureFiles->insert(pair<wstring,wstring>(pic->_localPath, pic->_remotePath));

		_bSqliteChange = _bPhotoDataIthmbChange = true;
	}
	
	return true; 
}

bool IosPicturesImpl5::SetLockBackground( wstring wallpaper )
{
	return _pWallpaper->SetBackground421( wallpaper, false );
}

bool IosPicturesImpl5::SetHomeBackground( wstring wallpaper )
{
	return _pWallpaper->SetBackground421( wallpaper, true );
}

void IosPicturesImpl5::LoadPhotoAndScreenShot( PictureFolder* pCamera, PictureFolder* pScreenshot )
{
	vector<PictureData*> pictures;

	RETURN_IF ( ! _ios5SQL->LoadIos5Pictures(&pictures, false) );

	// JPG为照片, PNG为截图
	for(size_t i =0; i < pictures.size(); ++i)
	{
		PictureData* pPicture = pictures[i];

		int pos = pPicture->_name.rfind('.');
		if ( string::npos == pos )
			continue;

		if ( CStrOperation::endsWithW( pPicture->_name, L".PNG" ) &&
			 IsScreenShoot(pPicture->_width, pPicture->_height) )
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

void IosPicturesImpl5::WriteThumbDB( Image* img, int idnex )
{
	RETURN_IF ( ! img );

	if ( idnex < 500 )
		IthmbDB(X166, _pDevice->GetIosPicturePath()->GetLocal166IthmbPath()).WriteImageIndex(img, idnex);
	if ( idnex < 1000 )
		IthmbDB(X158, _pDevice->GetIosPicturePath()->GetLocal158IthmbPath()).WriteImageIndex(img, idnex);
	if ( idnex < 2000 ) 
		IthmbDB(X120, _pDevice->GetIosPicturePath()->GetLocal120IthmbPath()).WriteImageIndex(img, idnex);

	IthmbDB(X32,  _pDevice->GetIosPicturePath()->GetLocal32IthmbPath ()).WriteImageIndex(img, idnex);
	IthmbDB(X79,  _pDevice->GetIosPicturePath()->GetLocal79IthmbPath ()).WriteImageIndex(img, idnex);
		
}

void IosPicturesImpl5::UpLoadthumbConfig()
{
	if ( ! _pWallpaper->DirCommand()->Exist( _pDevice->GetIosPicturePath()->GetIosIthmbConfigPath() ) )
	{
		wstring localpath = _pDevice->GetIosPicturePath()->GetLocalIthmbConfigPath();

		std::string strValue = (int)_pDevice->GetDevInfo()->_deviceProductType.find(L"iPad2") == 0 ? "1095" : "1096";

		xml_document<char> doc;
		xml_node<>* root = doc.allocate_node( node_element, "dict", NULL);  
		doc.append_node(root);
		root->append_node(doc.allocate_node(node_element, "key", "PLThumbnailManagerThumbnailFormatKey"));
		root->append_node(doc.allocate_node(node_element, "integer", strValue.c_str()));
		root->append_node(doc.allocate_node(node_element, "key", "PLThumbnailManagerVersionKey"));
		root->append_node(doc.allocate_node(node_element, "integer", "12"));

		std::string text;
		text += Plist_Head +  "<plist version=\"1.0\">\n\0";
		rapidxml::print(std::back_inserter(text), doc, 0);
		text += "</plist>";
		
		CFileOperation::WriteBinFile(localpath, text.c_str(), text.length());  // 写入.plist文件
		IosPlistConverter::GetPlistFromXml(localpath, localpath);			   // xml转plist

		_pWallpaper->GetIosFileIO()->Ios_CopyToPhone( localpath, _pDevice->GetIosPicturePath()->GetIosIthmbConfigPath() );
	}
	
	// 删除DICM下的缓存文件
	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(_pDevice->GetIosPicturePath()->GetIosDcimCacheFile()) )
		_pWallpaper->GetIosFileIO()->Ios_DeleteFile( _pDevice->GetIosPicturePath()->GetIosDcimCacheFile() );	

}

bool IosPicturesImpl5::IsScreenShoot( int width, int heigth )
{
	int nDeviceWidth = 0, nDeviceHeight = 0;
	if ( _pDevice->GetDevInfo()->Is2x() )
	{
		nDeviceWidth = 640;
		nDeviceHeight = 960;
	}
	else if ( _pDevice->GetDevInfo()->IsPad())
	{
		nDeviceWidth = 768;
		nDeviceHeight = 1024;
	}
	else
	{
		nDeviceWidth = 320;
		nDeviceHeight = 480;
	}

	return (width == nDeviceWidth && heigth == nDeviceHeight) || (width == nDeviceHeight && heigth == nDeviceWidth);
}

bool IosPicturesImpl5::UploadPictureFiles(ProgressSubject* pSubject, int percent)
{
	if ( _UploadPictureFiles && !_UploadPictureFiles->empty() )
	{
		int total=_UploadPictureFiles->size();
		int curcount=0;
		map<wstring,wstring>::iterator it = _UploadPictureFiles->begin();
		for (; it!=_UploadPictureFiles->end();it++ )
		{   
			if(pSubject)
			{
				IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver()));
			}
			// 原图的上传
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( it->first, it->second ) != Transfer_Success );
			curcount++;
			if(pSubject&&(percent*curcount/total>=1))
			{
				pSubject->AddTaskProgress(percent*curcount/total);
				pSubject->SendProgress();
				curcount=0;
			}
		}
	}
	return true;
}

bool IosPicturesImpl5::DeletePictreFiles()
{
	if ( _DeletePictureFiles && !_DeletePictureFiles->empty() )
	{
		for ( size_t i=0; i<_DeletePictureFiles->size(); i++ )
		{
			RETURN_FALSE_IF ( !_pWallpaper->GetIosFileIO()->Ios_DeleteFile( _DeletePictureFiles->at(i) ) );
		}
	}
	return true;
}

bool IosPicturesImpl5::PhotoSqilteSync()
{			
	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(_pDevice->GetIosPicturePath()->GetIosShmSqlitePath()) )
		RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( _pDevice->GetIosPicturePath()->GetIosShmSqlitePath() ) );
	if ( _pWallpaper->GetIosFileIO()->Ios_Exists(_pDevice->GetIosPicturePath()->GetIosWalSqlitePath()) )
		RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( _pDevice->GetIosPicturePath()->GetIosWalSqlitePath() ) );
	
	return SqilteDBSync();
}

map<int, wstring>* IosPicturesImpl5::LoadGalleryBucket()
{
	return _ios5SQL->LoadIos5Category();
}

int IosPicturesImpl5::AddGalleryGroup( const wstring displayname, wstring path )
{
	_bSqliteChange = true;
	return _ios5SQL->AddIos5Category(displayname) ;
}

bool IosPicturesImpl5::RenameGalleryGroup( const string nGroupid, const wstring name )
{
	RETURN_FALSE_IF ( !_ios5SQL->RenameIos5Category(atoi(nGroupid.c_str()), name) );
	_bSqliteChange = true;
	return true;
}

bool IosPicturesImpl5::DeleteGalleryGroup( int groupId, vector<PictureData*>* VerPictDatas)
{
	vector<int> verPicIds;
	for (size_t i=0; i<VerPictDatas->size(); ++i)
	{
		verPicIds.push_back(VerPictDatas->at(i)->GetId());
		_DeletePictureFiles->push_back(VerPictDatas->at(i)->_remotePath);
	}
	
	RETURN_FALSE_IF ( !_ios5SQL->DeleteIos5Category(groupId, &verPicIds) );
	_bSqliteChange = true;
	return true;
}

bool IosPicturesImpl5::LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas)
{
	if (_vPictures.empty())
	{
		RETURN_NULL_IF ( ! _ios5SQL->LoadIos5Pictures(&_vPictures, true) );
	}
	return _ios5SQL->GetIosCategoryImages(pGroup, &_vPictures, pVerDatas);
}

vector<PictureData*>* IosPicturesImpl5::AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														   int nextNumber, int index, const string firstGroupId)
{
	// 图库的原图存放目录不存在则创建
	FileHelper::CreateDir(_pDevice, _pDevice->GetIosPicturePath()->GetIos5GalleryPath());
	// 图库图片处理

	return AddSpecialPictures(localPaths, nextNumber+1, index+1, groupId);
}

bool IosPicturesImpl5::MovePictures( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId )
{
	vector<int> vPicturesId;
	for (size_t i =0; i < vPictures->size(); ++i)
	{
		vPicturesId.push_back(vPictures->at(i)->GetId());
	}
		
	// 修改数据库记录
	RETURN_FALSE_IF  ( !_ios5SQL->Ios5MovePictures(&vPicturesId, sourceGroupId, destGroupId) );
	_bSqliteChange = true;
	return true;
}

bool IosPicturesImpl5::RemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId )
{
	vector<int> vPicturesId;
	for (size_t i =0; i < vPictures->size(); ++i)
		vPicturesId.push_back(vPictures->at(i)->GetId());
	
	// 修改数据库记录
	RETURN_FALSE_IF ( !_ios5SQL->Ios5RemoveFromGroup(&vPicturesId, groupId) );
	_bSqliteChange = true;
	return true;
}

bool IosPicturesImpl5::IosAddExistPictures( const vector<const PictureData*>* vPictures, const string groupId )
{
	vector<int> vPicturesId;
	for (size_t i =0; i < vPictures->size(); ++i)
	{
		vPicturesId.push_back(vPictures->at(i)->GetId());
	}

	// 修改数据库记录
	RETURN_FALSE_IF ( !_ios5SQL->Ios5AddExistPictureToGroup(&vPicturesId, groupId) );
	_bSqliteChange = true;
	return true;
}

bool IosPicturesImpl5::DeleteGalleryPictures( const vector<const PictureData*>* vPictures)
{
	return DeleteSpecialPictures(vPictures);
}

bool IosPicturesImpl5::UpdateGalleryPicture( PictureData* pPicture, wstring srcPath )
{
	return UpdateSpecialPicture(pPicture, srcPath);
}

bool IosPicturesImpl5::DownloadThumbnail( PictureData* pPicture )
{
	if ( ! pPicture->_thumbRemotePath.empty() )
	{
		RETURN_FALSE_IF ( !_pWallpaper->IosDownPictureFile(pPicture->_thumbRemotePath, pPicture->_thumbLocalPath) );
	}

	return DownloadSpecialThumbnails( pPicture );
}


vector<int> IosPicturesImpl5::GetThumbType()
{
	vector<int> vec;
	vec.push_back(3009);
	vec.push_back(3113);
	vec.push_back(3111);
	vec.push_back(4005);
	vec.push_back(4010);
	vec.push_back(4035);
	return vec;
}


int IosPicturesImpl5::GetSpecialPhotos()
{
	int nCount = 0;
	if ( ! _ios5HasInit )
	{
		_ios5HasInit = Initialize();
	}
	RETURN_VALUE_IF ( !_ios5HasInit, 0);

	CppSQLite3DB SQLdb;
	SQLdb.Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str());

	CppSQLite3Query query = SQLdb.ExecQuery( "SELECT COUNT(*) From ZGENERICASSET,ZADDITIONALASSETATTRIBUTES \
											 WHERE ZGENERICASSET.[Z_PK]=ZADDITIONALASSETATTRIBUTES.[ZASSET] AND (ZSAVEDASSETTYPE=3 OR ZSAVEDASSETTYPE=256);" );

	if(!query.Eof())
	{
		nCount = query.GetIntField(0);
	}

	return nCount;
}

#endif