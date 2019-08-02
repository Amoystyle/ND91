#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPicturesImpl421.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Picture/IosPictureSqlite4.h"

#include "Module/Common/IosDB/IthmbDB.h"
#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Common/Path.h"

#include "Monitor/ProgressObserver.h"
#include "Monitor/ProgressSubject.h"


IosPicturesImpl421::IosPicturesImpl421(const DeviceData* pDevice)
:IosPicturesImpl(pDevice)
{
	_isIPad = _pDevice->GetDevInfo()->IsPad();
}

bool IosPicturesImpl421::DownloadInitFiles()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), 
		_pDevice->GetIosPicturePath()->GetLocalSqlitePath()) != Transfer_Success );	

	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_DownFromPhone(_pDevice->GetIosPicturePath()->GetIosAuxSqlitePath(), 
		_pDevice->GetIosPicturePath()->GetLocalAuxSqlitePath()) != Transfer_Success );

	RETURN_FALSE_IF ( ! DownloadGalleryFiles() );

	return DownloadPhotoDataThumbs();
}

bool IosPicturesImpl421::DeleteSpecialPictures( const vector<const PictureData*>* pVecPicture )
{
	RETURN_FALSE_IF ( !pVecPicture || pVecPicture->empty() );

	if (PICTURE_TYPE_PHOTO == pVecPicture->at(0)->GetType() || 
		PICTURE_TYPE_SCREENSHOT == pVecPicture->at(0)->GetType())
	{
		vector<int> VerPictureID;
		for (size_t i=0; i<pVecPicture->size(); ++i)
		{
			VerPictureID.push_back( pVecPicture->at(i)->GetId() );

			// 放入删除文件集合容器中
			_DeletePictureFiles->push_back(pVecPicture->at(i)->_remotePath);
		}
		
		RETURN_FALSE_IF ( !_ios4SQL->DeletePhotoFromSqlite(&VerPictureID) );
		_bSqliteChange = false;
	}
	else if (PICTURE_TYPE_WALLPAPER == pVecPicture->at(0)->GetType())
	{
		for (size_t i=0; i<pVecPicture->size(); ++i)
		{
			_pWallpaper->DeleteWallpaper421( pVecPicture->at(i) );
		}
	}
	else
		return false;

	return true;
}


vector<PictureData*>* IosPicturesImpl421::AddSpecialPictures( vector<wstring>* vPicFiles, int fileNumber, int thumbIndex /*= -1*/, std::string gruopId )
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
		wstring remoteFolder = _pDevice->GetIosPicturePath()->GetIosAlbumPath();
		wstring localFolder = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath();

		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			int thumbid	= thumbIndex;
			wstring newFileName = MakeFilename(PICTURE_TYPE_PHOTO, fileNumber++);

			wstring picExt			= gruopId == CAMERAID ? PICJPG : CStrOperation::toUpperW(PICPNG);
			PICTURE_TYPE picType	= gruopId == CAMERAID ? PICTURE_TYPE_PHOTO : PICTURE_TYPE_SCREENSHOT;

			// 拷贝并上传原图
			wstring localFile = localFolder + newFileName + picExt;
			wstring remoteFile = remoteFolder + newFileName + picExt;
            CFileOperation::CopyFile(vPicFiles->at(i), localFile, false);

			// 放入上传文件集合容器中
			_UploadPictureFiles->insert(pair<wstring,wstring>(localFile, remoteFile));

			// 生成并上传缩略图
			Image* img = Gdiplus::Image::FromFile(vPicFiles->at(i).c_str());
			RETURN_NULL_IF ( NULL == img);
			WriteThumbDB(img, thumbid);

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
							thumbid,
							-1,
							CFileOperation::GetFileSizeW(pPicture->_localPath),
							localFile,
							remoteFile
							);

			vPictures->push_back(pPicture);
			thumbIndex++;
		}

		RETURN_FALSE_IF ( ! _ios4SQL->InsertPhotoToSqlite(vPictures) );   // 写入数据库
		_bSqliteChange = _bPhotoDataIthmbChange = false;
	}

	return vPictures;
}

void IosPicturesImpl421::LoadSpecialPictures(PictureFolder* pCamera,
                                        PictureFolder* pWallpaper,
                                        PictureFolder* pScreenshot)
{
    _pWallpaper->LoadWallpaper4(pWallpaper);

    LoadPhotoAndScreenShot(pCamera, pScreenshot);
}

bool IosPicturesImpl421::UpdateSpecialPicture( PictureData* pic , wstring srcPath)
{

	Image* pImg = Gdiplus::Image::FromFile(srcPath.c_str());
	RETURN_FALSE_IF( ! pImg );	

	pic->_modifyTime = time(NULL);
	pic->_height = pImg->GetHeight();
	pic->_width = pImg->GetWidth();
	pic->_size = CFileOperation::GetFileSizeW(srcPath);

	if (PICTURE_TYPE_PHOTO == pic->GetType() || PICTURE_TYPE_SCREENSHOT == pic->GetType() )
	{
        RETURN_FALSE_IF( ! CFileOperation::CopyFile(srcPath, pic->_localPath, false));
        RETURN_FALSE_IF( ! _pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB2_WIDTH, IOS_THUMB2_HEIGHT));
		WriteThumbDB(pImg, pic->_thumbnailIndex);

		// 放入上传文件集合容器中
		_UploadPictureFiles->insert(pair<wstring,wstring>(pic->_localPath, pic->_remotePath));

		// 更新sqlite数据库
		RETURN_FALSE_IF ( ! _ios4SQL->UpdatePhotoToSqlite(pic) );
		_bSqliteChange = _bPhotoDataIthmbChange = false;
	}
	else if ( PICTURE_TYPE_WALLPAPER == pic->GetType())
	{
		RETURN_FALSE_IF( ! _pWallpaper->UpdateWallpaper421(pic, srcPath) );
	}
	else
		return false;

	return true; 
}

bool IosPicturesImpl421::SetLockBackground( wstring wallpaper )
{

	RETURN_FALSE_IF ( ! _pWallpaper->SetBackground421( wallpaper, false ) ) ;	

	return true;
}

bool IosPicturesImpl421::SetHomeBackground( wstring wallpaper )
{

	RETURN_FALSE_IF ( ! _pWallpaper->SetBackground421( wallpaper, true ) ) ;	
	
	return true;
}

void IosPicturesImpl421::WriteThumbDB( Image* img, int idnex )
{
	RETURN_IF ( ! img );

	IthmbDB(X120, _pDevice->GetIosPicturePath()->GetLocal120IthmbPath()).WriteImageIndex(img, idnex);

	if (_isIPad)
	{
		IthmbDB(X32,  _pDevice->GetIosPicturePath()->GetLocal32IthmbPath ()).WriteImageIndex(img, idnex);
		IthmbDB(X79,  _pDevice->GetIosPicturePath()->GetLocal79IthmbPath ()).WriteImageIndex(img, idnex);
		IthmbDB(X166, _pDevice->GetIosPicturePath()->GetLocal166IthmbPath()).WriteImageIndex(img, idnex);
	}
	else
	{
		IthmbDB(X158, _pDevice->GetIosPicturePath()->GetLocal158IthmbPath()).WriteImageIndex(img, idnex);
	}
}

bool IosPicturesImpl421::UploadPictureFiles(ProgressSubject* pSubject, int percent)
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

bool IosPicturesImpl421::DeletePictreFiles()
{
	if ( _DeletePictureFiles && !_DeletePictureFiles->empty() )
	{
		for ( size_t i=0; i<_DeletePictureFiles->size(); i++ )
		{
			RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( _DeletePictureFiles->at(i) ) );
		}
	}

	//	IOS4.x版本的图库图片的特殊处理
	RETURN_FALSE_IF ( ! DeleteFiles4X() );
	return true;
}

bool IosPicturesImpl421::PhotoSqilteSync()
{
	RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(_pDevice->GetIosPicturePath()->GetLocalAuxSqlitePath(),
		_pDevice->GetIosPicturePath()->GetIosAuxSqlitePath()) != Transfer_Success );	
	RETURN_FALSE_IF ( ! _pWallpaper->DirCommand()->ChangeMode(_pDevice->GetIosPicturePath()->GetIosSqlitePath(), "644", MOBILEUSER) );

	return SqilteDBSync();
}


bool IosPicturesImpl421::LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas)
{
	return LoadPicturesByGroup4X(pGroup, pVerDatas);
}

vector<PictureData*>* IosPicturesImpl421::AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
															 int nextNumber, int index, const string firstGroupId)
{
	return AddPictureToGroup4X(localPaths, groupId, firstGroupId);
}

bool IosPicturesImpl421::DownloadThumbnail( PictureData* pPicture )
{
	if ( pPicture->GetType() == PICTURE_TYPE_NORMAL )
	{
		RETURN_FALSE_IF ( ! DownloadGalleryThumbnails(pPicture) );
	}
	else
	{
		if ( ! pPicture->_thumbRemotePath.empty() )
		{
			RETURN_FALSE_IF ( !_pWallpaper->IosDownPictureFile(pPicture->_thumbRemotePath, pPicture->_thumbLocalPath) );
		}
		
		RETURN_FALSE_IF ( ! DownloadSpecialThumbnails( pPicture ) );
	}
	return true;
}


vector<int> IosPicturesImpl421::GetThumbType()
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

#endif