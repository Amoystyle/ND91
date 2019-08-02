#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPicturesImpl3.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Common/IosDB/IosPhotoDB.h"
#include "Common/Path.h"

IosPicturesImpl3::IosPicturesImpl3(const DeviceData* pDevice)
:IosPicturesImpl(pDevice)
{
}

vector<PictureData*>* IosPicturesImpl3::AddSpecialPictures( vector<wstring>* vPicFiles, int fileNumber, int thumbIndex /*= -1*/, std::string gruopId )
{
	RETURN_NULL_IF ( !vPicFiles || vPicFiles->empty() );
	vector<PictureData*>* vPictures = new vector<PictureData*>;

	if ( gruopId == WALLPAPERID )
	{
		for (size_t i=0; i<vPicFiles->size(); ++i)
		{
			PictureData* pData = _pWallpaper->AddWallpaper(vPicFiles->at(i), MakeFilename(PICTURE_TYPE_WALLPAPER, fileNumber++), false);
			vPictures->push_back(pData);
		}
	}
	else
	{
		wstring remoteFolder = _pDevice->GetIosPicturePath()->GetIosAlbumPath();
		wstring remoteThumbFolder = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath();
		wstring localFolder = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath();

		for ( size_t i=0; i<vPicFiles->size(); ++i )
		{
			// 生成设备端保存的图片文件名(在已有最大文件名上加一)
			wstring newFileName = MakeFilename(PICTURE_TYPE_PHOTO, fileNumber++);

			wstring picExt			= gruopId == CAMERAID ? PICJPG : CStrOperation::toUpperW(PICPNG);
			PICTURE_TYPE picType	= gruopId == CAMERAID ? PICTURE_TYPE_PHOTO : PICTURE_TYPE_SCREENSHOT;

			// 拷贝并上传原图
			std::wstring localFile = localFolder + newFileName + picExt;
			std::wstring remoteFile = remoteFolder + newFileName + picExt;
            CFileOperation::CopyFile(vPicFiles->at(i), localFile, false);

			// 放入上传文件集合容器中
			_UploadPictureFiles->insert(pair<wstring,wstring>(localFile, remoteFile));

			// 生成并上传.THM缩略图
			wstring localThumbFile = localFolder + newFileName + THMPIC;
			wstring remoteThumbFile = remoteThumbFolder + newFileName + THMPIC;
			RETURN_NULL_IF( ! _pWallpaper->MakeThumbnail(vPicFiles->at(i), localThumbFile, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT));
			
			// 创建对象
			PictureData* pPicture = new PictureData(remoteFolder, picType);
			pPicture->_size = CFileOperation::GetFileSizeW(pPicture->_localPath);

			pPicture->_localPath	    = localFile;
			pPicture->_remotePath       = remoteFile;
			pPicture->_thumbLocalPath   = localThumbFile;
			pPicture->_thumbRemotePath  = remoteThumbFile;
			pPicture->_name             = newFileName + picExt;

			vPictures->push_back(pPicture);
		}
	}
	
	return vPictures;
}

bool IosPicturesImpl3::DeleteSpecialPictures( const vector<const PictureData*>* pVecPicture )
{
	RETURN_FALSE_IF ( pVecPicture->empty() );

	if (pVecPicture->at(0)->GetType() == PICTURE_TYPE_WALLPAPER)
	{
		for ( size_t i=0; i<pVecPicture->size(); ++i )
			_pWallpaper->GetIosFileIO()->Ios_DeleteFile(pVecPicture->at(i)->_remotePath);		
	}
	else
	{
		// 删除图片的原图路径放入集合容器中
		for ( size_t i=0; i<pVecPicture->size(); ++i )
			_DeletePictureFiles->push_back(pVecPicture->at(i)->_remotePath); 	
	}
	
	return true;
}

void IosPicturesImpl3::Load(deque<NodeData*>* nodes, PictureFolder* pFolder)
{
    for( size_t i = 0; i < nodes->size(); ++i )
    {
        NodeData* pNode = (*nodes)[i];

        if ( pNode->_type != NodeData::FILE )
            continue;

        wstring nodeName = pNode->_name;

        if (pFolder->GetType() == PICTURE_TYPE_WALLPAPER )
        {
            if ( CStrOperation::endsWithW(nodeName, L"thumbnail.png") )
                continue;
        }
        else if (pFolder->GetType() == PICTURE_TYPE_SCREENSHOT )
        {
            if ( ! CStrOperation::endsWithW(nodeName, L".PNG") )
                continue;
        }
        else if (pFolder->GetType() == PICTURE_TYPE_PHOTO )
        {
            if ( ! CStrOperation::endsWithW(nodeName, L".JPG") )
                continue;
        }
        else
            continue;

        pFolder->InsertPicture(pNode, pFolder->GetType());
    }
}

void IosPicturesImpl3::LoadSpecialPictures(PictureFolder* pCamera, PictureFolder* pWallpaper, PictureFolder* pScreenshot)
{
    DirData cameraFolder(_pDevice);   // 照片和截图是同一个目录, 照片是JPG, 截图是PNG
    cameraFolder._name = pCamera->GetPath();

    deque<NodeData*> photos = cameraFolder.GetChildrens();
    if ( ! photos.empty() )
    {
        Load(&photos, pCamera);
        Load(&photos, pScreenshot);
    }

    DirData wallpaperFolder(_pDevice);   // 壁纸是PNG
    wallpaperFolder._name = pWallpaper->GetPath();

    deque<NodeData*> wallpapers = wallpaperFolder.GetChildrens();
    if ( ! wallpapers.empty() )
    {
        Load(&wallpapers, pWallpaper);
    }
}

bool IosPicturesImpl3::UpdateSpecialPicture( PictureData* pic , wstring srcPath)
{
	time_t now;
	time(&now);
	wstring BTHlocalPath, BTHremotePath;

	Image* pImg = Gdiplus::Image::FromFile(srcPath.c_str());
	RETURN_FALSE_IF(pImg==NULL);	

	pic->_modifyTime = now;
	pic->_height = pImg->GetHeight();
	pic->_width = pImg->GetWidth();
	pic->_size = CFileOperation::GetFileSizeW(srcPath);

    // 生成本地大图并上传
	if ( PICTURE_TYPE_PHOTO == pic->GetType() || PICTURE_TYPE_SCREENSHOT == pic->GetType())
	{
        CFileOperation::CopyFile(srcPath, pic->_localPath, false);
	}
	else if ( PICTURE_TYPE_WALLPAPER == pic->GetType() )
	{
		_pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT, true);
		_pWallpaper->MakeThumbnail(srcPath, pic->_localPath, IOS_WALLPAPER_WIDTH, IOS_WALLPAPER_HEIGHT, true);
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(pic->_thumbLocalPath, pic->_thumbRemotePath) != Transfer_Success );
		RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( pic->_localPath, pic->_remotePath ) != Transfer_Success );
	}
	else
		return false;

    // 生成本地缩略图并上传
	_pWallpaper->MakeThumbnail(srcPath, pic->_thumbLocalPath, IOS_THUMB_WIDTH, IOS_THUMB_HEIGHT, true);

	// 放入上传文件集合容器中
	_UploadPictureFiles->insert(pair<wstring,wstring>(pic->_localPath, pic->_remotePath));

	return true;
}

bool IosPicturesImpl3::SetLockBackground( wstring wallpaper )
{
	wstring LocalLockBack	= _pDevice->GetIosPicturePath()->GetLocalLockBack();
	wstring IosLockBack		= _pDevice->GetIosPicturePath()->GetIosLockBack();

	Image* pImg = Gdiplus::Image::FromFile(wallpaper.c_str());
	RETURN_FALSE_IF(pImg==NULL);

	if (pImg->GetWidth()!=320&&pImg->GetHeight()!=480)
		_pWallpaper->MakeThumbnail(wallpaper, LocalLockBack, 320, 480, true);
	else
        CFileOperation::CopyFile(wallpaper, LocalLockBack, false);

	RETURN_FALSE_IF( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone(LocalLockBack, IosLockBack) != Transfer_Success );
	
	return true;
}

bool IosPicturesImpl3::SetHomeBackground( wstring wallpaper )
{
	return false;
}

bool IosPicturesImpl3::UploadPictureFiles(ProgressSubject* pSubject, int percent)
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
			wstring localPath = it->first.substr(0, it->first.length()-4);
			wstring remotePath = it->second.substr(0, it->second.length()-4);
			wstring picTitle = remotePath.substr(remotePath.length()-8);

			// BTH缩略图文件路径生成
			wstring remoteBTHpath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picTitle + BTHPIC;
			wstring localBTHpath = localPath + BTHPIC;

			// 缩略图的上传
			wstring localthumbpath = localPath + THMPIC;
			wstring remotethumbpath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picTitle + THMPIC;
			RETURN_FALSE_IF ( _pWallpaper->GetIosFileIO()->Ios_CopyToPhone( localthumbpath, remotethumbpath ) != Transfer_Success);

			// 此处为3.x的编辑图片时，手机端可能存在BTH缩略图
			if ( _pWallpaper->GetIosFileIO()->Ios_Exists(remoteBTHpath) )
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
	return true;
}

bool IosPicturesImpl3::DeletePictreFiles()
{
	if ( _DeletePictureFiles && !_DeletePictureFiles->empty() )
	{
		for ( size_t i=0; i<_DeletePictureFiles->size(); i++ )
		{
			wstring remotePath = _DeletePictureFiles->at(i);
			wstring picPath = remotePath.substr(0, remotePath.length()-4);
			wstring picTitle = picPath.substr(picPath.length()-8);

			wstring remotethumbpath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picTitle + THMPIC;
			RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( remotethumbpath ) );

			// .BTH文件的同步
			wstring remoteBTHpath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picTitle + BTHPIC;
			if ( _pWallpaper->GetIosFileIO()->Ios_Exists(remoteBTHpath) )
				RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( remoteBTHpath ) );

			RETURN_FALSE_IF ( ! _pWallpaper->GetIosFileIO()->Ios_DeleteFile( remotePath ) );
		}
	}
	return true;
}

bool IosPicturesImpl3::PhotoSqilteSync()
{
	return SqilteDBSync();
}

bool IosPicturesImpl3::DownloadThumbnail( PictureData* pPicture )
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
	}

	return true;
}

bool IosPicturesImpl3::DownloadPicture( const PictureData* pPicture, wstring customPath )
{
	wstring path;

	if (customPath.empty())
		path = pPicture->_localPath;
	else
		path = customPath;

	if ( pPicture->GetType() == PICTURE_TYPE_NORMAL)
	{
		Bitmap* bmp = _pIosDB->GetImage(pPicture->GetId(), F3008);
		RETURN_FALSE_IF (NULL == bmp);
		DUMP_IMAGE(dynamic_cast<Image*>(bmp), path.c_str());
	}
	else
	{
		if ( _pWallpaper->GetIosFileIO()->Ios_Exists(pPicture->_remotePath) )
			RETURN_FALSE_IF( ! _pWallpaper->IosDownPictureFile(pPicture->_remotePath, path) );	
	}
	
	return true;
}

bool IosPicturesImpl3::DownloadInitFiles()
{
	return DownloadGalleryFiles();
}

vector<int> IosPicturesImpl3::GetThumbType()
{
	vector<int> vec;
	vec.push_back(3004);
	vec.push_back(3011);
	vec.push_back(3008);
	vec.push_back(3009);
	return vec;
}

bool IosPicturesImpl3::LoadPicturesByGroup( PictureGroup* pGroup, vector<PictureData*>& pVerDatas )
{
	vector<int>* pPicIds = _pIosDB->GetCategoryImages(atoi(pGroup->GetId().c_str()));

	RETURN_FALSE_IF ( !pPicIds || pPicIds->empty() )
	
	for (size_t i = 0;i < pPicIds->size(); ++i )
	{
		PictureData* pData = new PictureData;
		pData->SetId(pPicIds->at(i));
		pData->SetType(PICTURE_TYPE_NORMAL);
		pData->SetCategoryId(pGroup->GetId());

		// ios4以下版本从ithmb文件中取图片并随机生成文件名
		pData->_localPath = _pDevice->GetIosPicturePath()->GetLocalPicPath() + CFileOperation::GetGuidW() + PICJPG;
		pData->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath() + CFileOperation::GetGuidW() + PICJPG;
		pVerDatas.push_back(pData);
	}
	return true;
}

vector<PictureData*>* IosPicturesImpl3::AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														   int nextNumber, int index, const string firstGroupId)
{
	vector<PictureData*>* vPictures = new vector<PictureData*>;
	for ( size_t i=0;i< localPaths->size();++i )
	{
		// 向photo database中添加图片, 并得到图片id
		Bitmap* pImage = Gdiplus::Bitmap::FromFile(localPaths->at(i).c_str());
		RETURN_FALSE_IF( ! pImage );

		int pictureId = _pIosDB->AddImage(pImage);
		RETURN_FALSE_IF( _pIosDB->AddImageToCategory(pictureId, atoi(groupId.c_str())));
		
		PictureData* pData = new PictureData;
		pData->SetId(pictureId);
		pData->SetType(PICTURE_TYPE_NORMAL);
		pData->SetCategoryId(groupId);

		// ios4以下版本随机生成文件名	
		pData->_localPath = _pDevice->GetIosPicturePath()->GetLocalPicPath() + CFileOperation::GetGuidW() + PICJPG;
		pData->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath() + CFileOperation::GetGuidW() + PICJPG;

		int width = 480, height = 640;
		RETURN_FALSE_IF( !_pWallpaper->MakeThumbnail(localPaths->at(i), pData->_localPath, width, height, true) );   // 生成大图
							
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

int IosPicturesImpl3::GetSpecialPhotos()
{
	int nCount = 0;

	// 遍历手机目录底下的文件
	DirData cameraFolder(_pDevice);   // 照片和截图是同一个目录, 照片是JPG, 截图是PNG
	cameraFolder._name = _pDevice->GetIosPicturePath()->GetIosAlbumPath();

	deque<NodeData*> photos = cameraFolder.GetChildrens();
	if ( ! photos.empty() )
	{
		for( size_t i = 0; i < photos.size(); ++i )
		{
			NodeData* pNode = photos[i];
			if ( pNode->_type != NodeData::FILE )
				continue;

			wstring nodeName = pNode->_name;
			if ( ! CStrOperation::endsWithW(nodeName, L".PNG") &&
				! CStrOperation::endsWithW(nodeName, L".JPG") &&
				(int)nodeName.find(IMGTAG) < 0 )
				continue;

			++nCount;				
		}
	}

	return nCount + GetGalleryPicCount();

}

#endif