#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPicturesImpl4.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Picture/IosPictureSqlite4.h"

#include "Module/Common/IosDB/IthmbDB.h"
#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Common/Path.h"

#include "Monitor/ProgressObserver.h"
#include "Monitor/ProgressSubject.h"

IosPicturesImpl4::IosPicturesImpl4(const DeviceData* pDevice)
:IosPicturesImpl(pDevice)
{
}

bool IosPicturesImpl4::DownloadInitFiles()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), 
					_pDevice->GetIosPicturePath()->GetLocalSqlitePath()) != Transfer_Success );	
	
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosAuxSqlitePath(), 
					_pDevice->GetIosPicturePath()->GetLocalAuxSqlitePath()) != Transfer_Success );

	return DownloadGalleryFiles();

}

bool IosPicturesImpl4::DeleteSpecialPictures( const vector<const PictureData*>* pVecPicture )
{
	RETURN_FALSE_IF ( !pVecPicture || pVecPicture->empty() );

	if (pVecPicture->at(0)->GetType() == PICTURE_TYPE_WALLPAPER)
	{
		for ( size_t i=0; i<pVecPicture->size(); ++i )
			_pWallpaper->GetIosFileIO()->Ios_DeleteFile(pVecPicture->at(i)->_remotePath);		
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

		RETURN_FALSE_IF( !_ios4SQL->DeletePhotoFromSqlite(&VerPictureID) );
	}

	return true;
}


vector<PictureData*>* IosPicturesImpl4::AddSpecialPictures( vector<wstring>* vPicFiles, int fileNumber, int thumbIndex /*= -1*/, std::string gruopId )
{

	RETURN_NULL_IF ( !vPicFiles || vPicFiles->empty() );
	vector<PictureData*>* vPictures = new vector<PictureData*>;

	if ( gruopId == WALLPAPERID )
	{
		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			PictureData* pPicture = _pWallpaper->AddWallpaper(vPicFiles->at(i), MakeFilename(PICTURE_TYPE_WALLPAPER, fileNumber++), false);
			vPictures->push_back(pPicture);
		}
	}
	else
	{

		wstring remoteFolder		= _pDevice->GetIosPicturePath()->GetIosAlbumPath();
		wstring remoteThumbFolder	= _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath();
		wstring localFolder			= _pDevice->GetIosPicturePath()->GetLocal100APPLEPath();
		wstring localThumbFolder	= _pDevice->GetIosPicturePath()->GetLocalAlbumPath();

		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			Image* img = Gdiplus::Image::FromFile(vPicFiles->at(i).c_str());
			RETURN_NULL_IF ( NULL == img);

			wstring newFileName = MakeFilename(PICTURE_TYPE_PHOTO, fileNumber++);

			wstring picExt			= gruopId == CAMERAID ? PICJPG : CStrOperation::toUpperW(PICPNG);
			PICTURE_TYPE picType	= gruopId == CAMERAID ? PICTURE_TYPE_PHOTO : PICTURE_TYPE_SCREENSHOT;

			// 拷贝并上传原图
			std::wstring localFile = localFolder + newFileName + picExt;
			std::wstring remoteFile = remoteFolder + newFileName + picExt;
            CFileOperation::CopyFile(vPicFiles->at(i), localFile, false);

			// 放入上传文件集合容器中
			_UploadPictureFiles->insert(pair<wstring,wstring>(localFile, remoteFile));

			// 生成.THM缩略图
			wstring localThumbFile = localThumbFolder + newFileName + THMPIC;
			wstring remoteThumbFile = remoteThumbFolder + newFileName + THMPIC;
			RETURN_NULL_IF( ! _pWallpaper->MakeThumbnail(vPicFiles->at(i), localThumbFile, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT));

			// 生成并上传.BTH缩略图
			IthmbDB ithmbdb(F3001, L"");
			wstring localThumbBTH = localThumbFolder + newFileName + BTHPIC;
			wstring remoteThumbBTH = remoteThumbFolder + newFileName + BTHPIC;
			RETURN_NULL_IF( ! ithmbdb.MakeBthFile(localThumbFile, localThumbBTH, false));

			// 创建对象
			PictureData* pPicture = new PictureData(remoteFolder, picType);

			time_t now;
			time(&now);
			pPicture->Create( 
							newFileName + picExt,
							now - CDealTime::IosStartTime().GetTime(),
							now - CDealTime::IosStartTime().GetTime(),
							img->GetWidth(),
							img->GetHeight(),
							-1,
							-1,
							CFileOperation::GetFileSizeW(pPicture->_localPath),
							localFile,
							remoteFile,
							localThumbFile,
							remoteThumbFile,
							picType
							);

			vPictures->push_back(pPicture);
		}

		RETURN_FALSE_IF ( !_ios4SQL->InsertPhotoToSqlite(vPictures) );   // 写入数据库
		_bSqliteChange = _bPhotoDataIthmbChange = true;
	}
	
	return vPictures;
}

void IosPicturesImpl4::LoadSpecialPictures( PictureFolder* pCamare, PictureFolder* pWallpaper, PictureFolder* pScreenshot )
{
    // 取壁纸
    bool iPad = _pDevice->GetDevInfo()->_deviceProductType.find(L"iPad") != string::npos;
    _pWallpaper->LoadWallpaper4(pWallpaper);

    LoadPhotoAndScreenShot(pCamare, pScreenshot);
}

bool IosPicturesImpl4::UpdateSpecialPicture( PictureData* pic , wstring srcPath)
{ 
	time_t now;
	time(&now);

	Image* pImg = Gdiplus::Image::FromFile(srcPath.c_str());
	RETURN_FALSE_IF( ! pImg );	

	pic->_modifyTime = now;
	pic->_height = pImg->GetHeight();
	pic->_width = pImg->GetWidth();
	pic->_size = CFileOperation::GetFileSizeW(srcPath);

    // 更新本地缩略图文件
	_pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT, true);

	if (PICTURE_TYPE_PHOTO == pic->GetType() || PICTURE_TYPE_SCREENSHOT == pic->GetType() )
	{
        // 生成本地大图: 直接拷贝即可
        CFileOperation::CopyFile(srcPath, pic->_localPath, false);

        // 在本地生成BTH文件, 覆盖手机上的.BTH文件
        wstring localThumbBTH = pic->_thumbLocalPath.substr(0,pic->_thumbLocalPath.length()-4);
        localThumbBTH += BTHPIC;
        wstring remoteThumbBTH = pic->_thumbRemotePath.substr(0,pic->_thumbRemotePath.length()-4);
        remoteThumbBTH += BTHPIC;

		IthmbDB ithmbdb(F3001, L"");
		ithmbdb.MakeBthFile(pic->_thumbLocalPath, localThumbBTH, false);   

        // 更新sqlite数据库
		RETURN_FALSE_IF ( !_ios4SQL->UpdatePhotoToSqlite(pic) );

		_bSqliteChange = _bPhotoDataIthmbChange = true;
	} 
	else if ( PICTURE_TYPE_WALLPAPER == pic->GetType())
	{
        // 生成本地大图
		_pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT, true);
		_pWallpaper->MakeThumbnail(srcPath, pic->_localPath, IOS_WALLPAPER_WIDTH, IOS_WALLPAPER_HEIGHT, true);
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(pic->_thumbLocalPath, pic->_thumbRemotePath) != Transfer_Success );
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( pic->_localPath, pic->_remotePath ) != Transfer_Success );
	}
	else
		return false;

	// 放入上传文件集合容器中
	_UploadPictureFiles->insert(pair<wstring,wstring>(pic->_localPath, pic->_remotePath));

	return true;
}

bool IosPicturesImpl4::SetLockBackground( wstring wallpaper )
{
	wstring LocalLockBack		= _pDevice->GetIosPicturePath()->GetLocalLockBack();
	wstring IosLockBack			= _pDevice->GetIosPicturePath()->GetIosLockBack();
	wstring LocalLockThumbBack	= _pDevice->GetIosPicturePath()->GetLocalLockThumbBack();
	wstring IosLockThumbBack	= _pDevice->GetIosPicturePath()->GetIosLockThumbBack(); 

	RETURN_FALSE_IF ( ! SetBackground(wallpaper, LocalLockBack, IosLockBack, LocalLockThumbBack, IosLockThumbBack) );
	
	return true;
}

bool IosPicturesImpl4::SetHomeBackground( wstring wallpaper )
{
	wstring LocalHomeBack		= _pDevice->GetIosPicturePath()->GetLocalHomeBack();
	wstring IosHomeBack			= _pDevice->GetIosPicturePath()->GetIosHomeBack();
	wstring LocalHomeThumbBack	= _pDevice->GetIosPicturePath()->GetLocalHomeThumbBack();
	wstring IosHomeThumbBack	= _pDevice->GetIosPicturePath()->GetIosHomeThumbBack(); 

	bool isSuppor = _pWallpaper->IsSupportHomeScreen();
	if (isSuppor)
	{
		RETURN_FALSE_IF ( ! SetBackground(wallpaper, LocalHomeBack, IosHomeBack, LocalHomeThumbBack, IosHomeThumbBack) );
	}
	else
	{
		return false;
	}

	return true;
}


bool IosPicturesImpl4::SetBackground( wstring wallpaper, wstring LocalBack, wstring IosBack, wstring LocalThumbBack, wstring IosThumbBack )
{
	RETURN_FALSE_IF( wallpaper.empty() );
	Image* pImg = Gdiplus::Image::FromFile(wallpaper.c_str());
	RETURN_FALSE_IF(pImg==NULL);

	if (pImg->GetWidth()!=320&&pImg->GetHeight()!=480)
		_pWallpaper->MakeThumbnail(wallpaper, LocalBack, 320, 480, true);
	else
        CFileOperation::CopyFile(wallpaper, LocalBack, false);

	_pWallpaper->MakeThumbnail(wallpaper, LocalThumbBack, 102, 154, true);

	if (_pWallpaper->GetIosFileIO()->Ios_DeleteFile(_pDevice->GetIosPicturePath()->GetIosLockPortraitPath()))
	{
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(LocalBack, IosBack) != Transfer_Success );
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(LocalThumbBack, IosThumbBack) != Transfer_Success );
	}

	return true; 
}

bool IosPicturesImpl4::UploadPictureFiles(ProgressSubject* pSubject, int percent)
{
	if ( _UploadPictureFiles && !_UploadPictureFiles->empty() )
	{
		int total=_UploadPictureFiles->size()+_pIosDB->GetUploadFiles().size();
		int curcount=0;
		map<wstring,wstring>::iterator it = _UploadPictureFiles->begin();
		for (; it!=_UploadPictureFiles->end();it++ )
		{   
			if(pSubject)
			{
				IF_CANCEL_RETURN_FALSE(dynamic_cast<ProgressObserver*>(pSubject->GetFirstObserver()));
			}
			wstring localPath = it->first.substr(0, it->first.length()-4);
			localPath = CStrOperation::ReplaceW(localPath, L"Album\\100APPLE", L"Album");

			wstring remotePath = it->second.substr(0, it->second.length()-4);
			remotePath  = CStrOperation::ReplaceW(remotePath, L"DCIM", L"PhotoData" );

			// 缩略图文件路径生成
			wstring localthumbpath = localPath + THMPIC;
			wstring remotethumbpath = remotePath + THMPIC;
			// BTH缩略图文件路径生成
			wstring remoteBTHpath = remotePath + BTHPIC;
			wstring localBTHpath = localPath + BTHPIC;
			// 缩略图的上传
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( localthumbpath, remotethumbpath ) != Transfer_Success );
			// .BTH文件的上传
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( localBTHpath, remoteBTHpath ) != Transfer_Success );
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

	//	IOS4.x版本的图库图片的特殊处理
	RETURN_FALSE_IF ( ! UploadFiles4X() );

	return true;
}

bool IosPicturesImpl4::DeletePictreFiles()
{
	if ( _DeletePictureFiles && !_DeletePictureFiles->empty() )
	{
		for ( size_t i=0; i<_DeletePictureFiles->size(); i++ )
		{
			wstring remotePath = _DeletePictureFiles->at(i);
			wstring picPath = remotePath.substr(0, remotePath.length()-4);

			wstring remotethumbpath = picPath + THMPIC;
			RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->DeleteFileLink( remotethumbpath ) );
			// .BTH文件的同步
			wstring remoteBTHpath = picPath + BTHPIC;
			RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->DeleteFileLink( remoteBTHpath ) );

			RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->DeleteFileLink( remotePath ) );
		}
	}

	//	IOS4.x版本的图库图片的特殊处理
	RETURN_FALSE_IF ( ! DeleteFiles4X() );
	return true;
}

bool IosPicturesImpl4::PhotoSqilteSync()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(_pDevice->GetIosPicturePath()->GetLocalAuxSqlitePath(),
		_pDevice->GetIosPicturePath()->GetIosAuxSqlitePath()) != Transfer_Success );	
	RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->ChangeMode(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), "644", MOBILEUSER) );

	return SqilteDBSync();
}


bool IosPicturesImpl4::LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas)
{
	return LoadPicturesByGroup4X(pGroup, pVerDatas);
}

vector<PictureData*>* IosPicturesImpl4::AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														   int nextNumber, int index, const string firstGroupId)
{
	return AddPictureToGroup4X(localPaths, groupId, firstGroupId);	
}


bool IosPicturesImpl4::DownloadThumbnail( PictureData* pPicture )
{
	if ( pPicture->GetType() == PICTURE_TYPE_NORMAL )
	{
		DownloadGalleryThumbnails(pPicture);
	}
	else
	{
		if ( ! pPicture->_thumbRemotePath.empty() )
		{
			_pWallpaper->IosDownPictureFile(pPicture->_thumbRemotePath, pPicture->_thumbLocalPath);
		}
	}

	return true;
}


vector<int> IosPicturesImpl4::GetThumbType()
{
	vector<int> vec;
	vec.push_back(3004);
	vec.push_back(3011);
	vec.push_back(3008);
	vec.push_back(3009);
	vec.push_back(4006);
	vec.push_back(4010);
	return vec;
}

#endif