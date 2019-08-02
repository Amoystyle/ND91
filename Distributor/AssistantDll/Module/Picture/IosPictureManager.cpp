#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPictureManager.h"
#include "Module/Picture/IosPicturesImpl.h"
#include "Module/Picture/IosPicturesImpl3.h"
#include "Module/Picture/IosPicturesImpl4.h"
#include "Module/Picture/IosPicturesImpl421.h"
#include "Module/Picture/IosPicturesImpl5.h"
#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Picture/PictureLibrary.h"
#include "Module/Picture/IosSyncATHost.h"

#include "Device/IosUsbSync.h"
#include "Module/System/SystemManager.h"
#include "Common/Path.h"
#include "Device/CFConverter.h"
#include "Common/plist/PlistOperation.h"
#include "Module/System/KillProcessCommand.h"

IosPictureManager::IosPictureManager(const DeviceData* pDevice)
: PictureManager(pDevice)
{
	_nDeviceVer = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);

    _pIosImpl		= NULL;
	_wallpaperType	= UnKnowType;
}

IosPictureManager::~IosPictureManager(void)
{
	Release();	
}

void IosPictureManager::Release() 
{

	SAFE_DELETE(_pIosImpl);

	PictureManager::Release();
}

bool IosPictureManager::IosInit()
{
    // 如果不存在目录，创建各种存放临时数据的目录
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocaliPhotoPath      ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalThumbsPath      ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalPicPath         ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalThumbnailPath   ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalMediaPath       ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalAlbumPath       ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocal100APPLEPath    ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalWallPaperPath   ()));
    RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalBackground      ()));

	// 初始化图片管理接口 
	_pIosImpl = CreateIosImplement();

	// 三个特殊文件夹
	CreateCamera(_pDevice->GetIosPicturePath()->GetIosAlbumPath());

	CreateWallpaper(_pDevice->GetIosPicturePath()->GetIosWallPaperPath());

	CreateScreenshot(_pDevice->GetIosPicturePath()->GetIosAlbumPath());

	RETURN_FALSE_IF ( ! _pIosImpl->Init() );

	return true;
}

IosPicturesImpl* IosPictureManager::CreateIosImplement() 
{
    if (_nDeviceVer < 400)
    {
        return new IosPicturesImpl3(_pDevice);
    }
    else if (_nDeviceVer < 421 && _nDeviceVer >= 400)
    {
        return new IosPicturesImpl4(_pDevice);
    }
    else if ( _nDeviceVer >= 421 && _nDeviceVer < 500)
    {
        return new IosPicturesImpl421(_pDevice);
    }
	else
	{
		return new IosPicturesImpl5(_pDevice);
	}
}

bool IosPictureManager::Load()
{
    RELEASE_VECTOR(_PictureGroups);
    _pAllPictures->Clear();

	RETURN_FALSE_IF( ! IosInit() );

	LoadIosBucket();
	LoadIosPicture();

    return true;
}

void IosPictureManager::LoadIosBucket()
{
	_pCamera->_id		= CAMERAID;
	_pScreenShot->_id	= SCREENSHOTID;
	_pWallpaper->_id	= WALLPAPERID;

    char id[5] = {0};
    map<int, wstring>* pMap = NULL;
	pMap = _pIosImpl->LoadGalleryBucket();

	RETURN_IF(!pMap);
    for( map<int, wstring>::iterator it = pMap->begin(); it != pMap->end(); ++it)
    {
        PictureGroup* pGroup = new PictureGroup(_pDevice, _pAllPictures);
        sprintf_s(id, 5, "%d", it->first);
        pGroup->_id = id;
        pGroup->_name = it->second;

        _PictureGroups.push_back(pGroup);
    }

    SAFE_DELETE(pMap);
}

void IosPictureManager::LoadIosPicture()
{
    _pCamera->Clear();
    _pWallpaper->Clear();
    _pScreenShot->Clear();
    _pIosImpl->LoadSpecialPictures(_pCamera, _pWallpaper, _pScreenShot);

	vector<PictureData*> pVerPictures;
	for ( size_t i=0; i < _PictureGroups.size(); ++i)
	{
		pVerPictures.clear();
		PictureGroup* pGroup = _PictureGroups[i];
		_pIosImpl->LoadPicturesByGroup(pGroup, pVerPictures);

		if ( !pVerPictures.empty() )
		{
			for (size_t j = 0;j < pVerPictures.size(); ++j)
			{
				PictureData* pData = pVerPictures[j];
				pGroup->InsertPicture(pData->GetId());
				_pAllPictures->Add(pData);
			}
		}		
	}
	
}

string IosPictureManager::AddPhotoGroup( const wstring displayname,  wstring path)
{
	int id = _pIosImpl->AddGalleryGroup(displayname, path);
	RETURN_VALUE_IF( -1 == id, "");

	char buffer[5] = {0};
	sprintf_s(buffer, 5, "%d", id);

	PictureGroup* pGroup = new PictureGroup(_pDevice, _pAllPictures);
	pGroup->_id		=	buffer; 
	pGroup->_name	=	displayname;
	_PictureGroups.push_back(pGroup);

	return buffer;
}

bool IosPictureManager::RenameGroup(const string nGroupid, const wstring name)
{
	BaseGroup* pBaseGroup = GetBaseGroup(nGroupid);
	RETURN_FALSE_IF( ! pBaseGroup );

	RETURN_FALSE_IF( ! _pIosImpl->RenameGalleryGroup(nGroupid, name) );
	pBaseGroup->SetName( name );

	return true;
}

bool IosPictureManager::DeleteGroup(PictureGroup* pGroup)
{
    // 从分类集合中移除此分类
    bool bNotFound = true;
    vector<PictureGroup*>::iterator itGroup;
    for ( itGroup =_PictureGroups.begin(); itGroup != _PictureGroups.end(); itGroup++)
    {
        if ( (*itGroup)->_id == pGroup->_id )
        {
            // 删除分类
            _PictureGroups.erase(itGroup);
            bNotFound = false;
            break;
        }
    }
    RETURN_FALSE_IF( bNotFound );    // 分类没找到

	vector<PictureData*> pVerPicDatas;
	pGroup->GetPictures(&pVerPicDatas);
	// 从图片数据库中移除此分类
	RETURN_FALSE_IF( !_pIosImpl->DeleteGalleryGroup( atoi(pGroup->_id.c_str()), &pVerPicDatas) );

	// 从设备或数据库中去掉此分类及其图片
	const list<int>* pList = pGroup->GetPictures();
	list<int>::const_iterator it = pList->begin();
	for(; it != pList->end(); ++it )
	{
		if ( ! ContainsInGroup( *it ) ) // ios仅当其他分类都不包含此图片才删除
		{
			_pAllPictures->Del(*it);
		}
	}

    SAFE_DELETE(pGroup);    // 释放分类对象
    return true;
}

bool IosPictureManager::MovePicture( const vector<const PictureData*>* vPictures, 
										 string sourceGroupId, 
										 string destGroupId)
{

	RETURN_FALSE_IF ( ! vPictures || vPictures->empty() );
	RETURN_TRUE_IF ( sourceGroupId == destGroupId );

	BaseGroup* pSourceGroup = GetBaseGroup(sourceGroupId);
	RETURN_FALSE_IF ( ! pSourceGroup );

	BaseGroup* pDestGroup = GetBaseGroup(destGroupId);
	RETURN_FALSE_IF ( ! pDestGroup );

	// 如果是图库分类移动到图库分类
	if ( dynamic_cast<PictureGroup*>(pSourceGroup) && dynamic_cast<PictureGroup*>(pDestGroup) )
	{
		RETURN_FALSE_IF( !_pIosImpl->MovePictures(vPictures, sourceGroupId, destGroupId) );

		for (size_t i =0; i < vPictures->size(); ++i)
		{
			pSourceGroup->RemovePictureFromList(vPictures->at(i));
			pDestGroup->InsertPicture(vPictures->at(i));
		}	
	}
	else 
	{
		vector<wstring> paths;
		for (size_t i =0; i < vPictures->size(); ++i)
		{
			const PictureData* pdata = vPictures->at(i);
			if ( ! CFileOperation::IsFileExist( pdata->_localPath.c_str() ) )
			{
				DownloadPicture(pdata);
			}
			paths.push_back(pdata->_localPath);
		}
		RETURN_FALSE_IF ( ! AddPictureToGroup(&paths, destGroupId.c_str()) );
		RETURN_FALSE_IF ( ! DeletePicture(vPictures, sourceGroupId.c_str()) );
	}

	return true;
}

bool IosPictureManager::DeletePicture( const vector<const PictureData*>* vPictures, const string groupId )
{
	RETURN_FALSE_IF ( !vPictures || vPictures->empty() );

	BaseGroup* pGroup = GetBaseGroup(groupId);
	RETURN_FALSE_IF ( ! pGroup );

	// 设备操作
	if ( pGroup->GetType() == PICTURE_TYPE_NORMAL )
	{
		RETURN_FALSE_IF ( !_pIosImpl->DeleteGalleryPictures(vPictures) );
	}
	else
	{
		RETURN_FALSE_IF ( !_pIosImpl->DeleteSpecialPictures(vPictures) );
	}

	for (size_t i=0; i<vPictures->size(); ++i)
	{
		const PictureData* pData = vPictures->at(i);
		RETURN_FALSE_IF (NULL == pData);

		// 删除本地文件
        CFileOperation::DeleteFile(pData->_localPath);
        CFileOperation::DeleteFile(pData->_thumbLocalPath);

		//特殊分类有释放Pictures
		pGroup->RemovePictureFromList(pData);

		if ( pGroup->GetType() == PICTURE_TYPE_NORMAL )
		{
			// ios的图片可能存在于多个组中, 要从所有组里删除
			for(size_t j = 0; j < _PictureGroups.size(); ++j)
			{
				_PictureGroups[j]->RemovePictureFromList(pData);
			}
			_pAllPictures->Del(pData);
		}
	}

	return true;

}

bool IosPictureManager::IosSync(ProgressSubject* pSubject)
{
			KillProcessCommand killCameraCmd(_pDevice, L"Camera");
			killCameraCmd.Execute();
			KillProcessCommand killMobileSlideShowCmd(_pDevice, L"MobileSlideShow");
			killMobileSlideShowCmd.Execute();
	// 开始同步
	IosUsbSync usbSync(_pDevice->GetDevInfo());	
	usbSync.StartSync();

	if ( _pIosImpl->GetSqliteState() )
		RETURN_FALSE_IF ( !_pIosImpl->PhotoSqilteSync() );
	pSubject->AddTaskProgress(2);
	pSubject->SendProgress();

	if ( _pIosImpl->GetPhotoDataBaseState() )
		RETURN_FALSE_IF ( !_pIosImpl->PhotodatabaseSync() );
	pSubject->AddTaskProgress(1);
	pSubject->SendProgress();

	wstring deviceversion=_pDevice->GetDevInfo()->_deviceProductVersion;

	// 同步此次需要上传的所有原图文件
	RETURN_FALSE_IF ( !_pIosImpl->UploadPictureFiles(pSubject,deviceversion<L"4.0"?15:20) );
	// 同步此次需要删除的所有原图文件
	RETURN_FALSE_IF ( !_pIosImpl->DeletePictreFiles() );
	_pIosImpl->ReleaseCase();

	if ( _pIosImpl->GetPhotoDataIthmbState() )
	{
		RETURN_FALSE_IF ( !_pIosImpl->PhotoDataIthmbSync(pSubject,deviceversion<L"5.0"?7:17) );
	}
	if ( _pIosImpl->GetPhotosIthmbState() )
		RETURN_FALSE_IF ( !_pIosImpl->PhotosIthmbSync(pSubject,deviceversion<L"4.0"?22:10) );

	// 结束同步
	usbSync.StopSync();
	RETURN_FALSE_IF ( !IosSyncHelper() );

	_pIosImpl->ResetSyncState();	

	return true;
}

bool IosPictureManager::IosSyncThroughATHost()
{
	//开始同步
	const DeviceInfo* tempDevInfo = _pDevice->GetDevInfo();
	IosUsbSync usbSync(tempDevInfo);	
	usbSync.StartSync();

	map<wstring, wstring>* tempUpLoadPictureFiles = _pIosImpl->GetUploadPictureFiles();
	if ( !tempUpLoadPictureFiles || tempUpLoadPictureFiles->empty() )
	{
		return true;  // No pictures added.
	}

	vector<char*> tempPictUuids = _pIosImpl->GetPictureUuids();
	if (tempPictUuids.empty())
	{
		LOG_DEBUG(L"Missing picture uuids!");
	}

	sConnDeviceInfo  *psConnDevice = new sConnDeviceInfo;
	psConnDevice->OperateMode = PHOTO_ADD;		
	psConnDevice->ItemCount = tempUpLoadPictureFiles->size();	
	psConnDevice->DevInfo = _pDevice->GetDevInfo();

	StartSyncPhoto(psConnDevice, tempUpLoadPictureFiles, tempPictUuids);

	SAFE_DELETE_ARRAY(psConnDevice);

	// 结束同步
	usbSync.StopSync();
	RETURN_FALSE_IF ( !IosSyncHelper() );

	_pIosImpl->ResetSyncState();

	return true;
}


bool IosPictureManager::IosSyncHelper()
{
	DirectoryOperationCommand dirCmd(_pDevice);

	// --- 421版本的缓存文件不删除导致相册数据不刷新---
	if (dirCmd.Exist(_pDevice->GetIosPicturePath()->GetIos421CacheFile()))
	{
		dirCmd.DeleteFileLink(_pDevice->GetIosPicturePath()->GetIos421CacheFile());
	}
	if (dirCmd.Exist(_pDevice->GetIosPicturePath()->GetIpad421CachePath()))
	{
		dirCmd.DeleteFileLink(_pDevice->GetIosPicturePath()->GetIpad421CachePath());
	}

	// 删除iTunes的图片目录记录,这样iTunes在同步图库的时候就会提示已经在其他计算机上同步过
	// 从而避免iTunes在同步的时候把图库破坏。
	if (dirCmd.Exist(_pDevice->GetIosPicturePath()->GetIosPhotoPrefsPath()))
		dirCmd.DeleteFileLink(_pDevice->GetIosPicturePath()->GetIosPhotoPrefsPath());

	if ( _pIosImpl->GetSqliteState() || _pIosImpl->GetPhotoDataIthmbState() )
	{
		DeviceData* pdata = (DeviceData*)_pDevice;
		
		if ( IosPublicFunc::GetVerNum(pdata->GetDevInfo()->_deviceProductVersion) >= 600  )
		{	
			RETURN_FALSE_IF(!pdata->GetSystemManager()->RebootDev());
		}
		else
		{
			RETURN_FALSE_IF(!pdata->GetSystemManager()->LogoffDev());
		}
	}
	return true;
}

bool IosPictureManager::IsSupportHomeScreen()
{
	bool success = false;
    if ( UnKnowType == _wallpaperType )
    {
        _wallpaperType = _pIosImpl->_pWallpaper->IsSupportHomeScreen() ? LockAndHome : LockBackground;
    }

	if ( LockAndHome == _wallpaperType )
		success = true;

	_wallpaperType = UnKnowType;
	return success;
    
}

int IosPictureManager::GetAllPicturesCount()
{
    return CreateIosImplement()->GetAllPicturesCount();
}

bool IosPictureManager::IosRemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId)
{
	// 不能从iPhone的"所有图片" 中移除
	RETURN_FALSE_IF( _pDevice->GetDevInfo()->IsPhone() && groupId == ALL_IMAGE_GROUP_ID );

	PictureGroup* pGroup = FindGroup(groupId);
	RETURN_FALSE_IF( !pGroup );

	RETURN_FALSE_IF( !_pIosImpl->RemoveFromGroup(vPictures, groupId) );

	for (size_t i =0; i < vPictures->size(); ++i)
	{
		pGroup->RemovePictureFromList(vPictures->at(i));
	}
	return true;
}

bool IosPictureManager::IosAddExistPicture(const vector<const PictureData*>* vPictures, const string groupId)
{
	if ( PICTURE_TYPE_NORMAL != vPictures->at(0)->GetType() || atoi(groupId.c_str()) == 0)
	{
		vector<wstring> paths;
		for (size_t i =0; i < vPictures->size(); ++i)
		{
			const PictureData* pdata = vPictures->at(i);
			if ( ! CFileOperation::IsFileExist( pdata->_localPath.c_str() ) )
			{
				if ( DownloadPicture(pdata) )
					paths.push_back(pdata->_localPath);		
			}
			else
				paths.push_back(pdata->_localPath);
		}
		return AddPictureToGroup(&paths, groupId);
	}
	else
	{
		RETURN_FALSE_IF( groupId == ALL_IMAGE_GROUP_ID );

		PictureGroup* pGroup = FindGroup(groupId);
		RETURN_FALSE_IF( ! pGroup );
		RETURN_FALSE_IF( ALL_IMAGE_GROUP_NAME == pGroup->GetName() );
		
		RETURN_FALSE_IF ( ! _pIosImpl->IosAddExistPictures(vPictures, groupId) );

		for (size_t i =0; i < vPictures->size(); ++i)
		{
			pGroup->InsertPicture(vPictures->at(i)->GetId());
		}
	}

	return true;
}

bool IosPictureManager::DownloadThumbnail(PictureData* pPicture)
{
	RETURN_FALSE_IF ( !pPicture );
	return _pIosImpl->DownloadThumbnail(pPicture);     
}


bool IosPictureManager::DownloadPicture(const PictureData* pPicture, wstring customPath)
{
   return _pIosImpl->DownloadPicture(pPicture, customPath);
}

bool IosPictureManager::SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType)
{
    // 若本地图片不存在则下载
    if ( !CFileOperation::IsFileExist(pPicture->_localPath.c_str()) )
    {
        RETURN_FALSE_IF( ! DownloadPicture(pPicture) );
    }

	bool result = false;

	switch(setType)
	{
	case LockBackground:
		result = _pIosImpl->SetLockBackground(pPicture->_localPath);
		_pIosImpl->_pWallpaper->SetUniqueHomeScreenValue(true);
		break;
	case HomeBackground:
		result = _pIosImpl->SetHomeBackground(pPicture->_localPath);
		_pIosImpl->_pWallpaper->SetUniqueHomeScreenValue(true);
		break;
	case LockAndHome:
		result = _pIosImpl->SetLockBackground(pPicture->_localPath);
		result = _pIosImpl->SetHomeBackground(pPicture->_localPath);
		_pIosImpl->_pWallpaper->SetUniqueHomeScreenValue(false);
		break;
	default:
		break;
	}
	if (result)
	{
		result = _pIosImpl->_pWallpaper->UploadWallpaperInI(pPicture->_localPath);
	}	

    return result;
}

bool IosPictureManager::ChangeImage( PictureData* pPicture, wstring srcPath )
{
	RETURN_FALSE_IF ( !pPicture || srcPath.empty() );

	if ( PICTURE_TYPE_PHOTO == pPicture->GetType() || 
		PICTURE_TYPE_SCREENSHOT == pPicture->GetType() ||
		PICTURE_TYPE_WALLPAPER == pPicture->GetType() )
	{
		return _pIosImpl->UpdateSpecialPicture(pPicture, srcPath);
	}
	else if ( PICTURE_TYPE_NORMAL == pPicture->GetType())
	{
		return _pIosImpl->UpdateGalleryPicture(pPicture, srcPath);
	}
	else
		return false;

}

void IosPictureManager::ClearCache()
{
    CFileOperation::DeleteDirectory(_pDevice->GetIosPicturePath()->Home());
}


bool IosPictureManager::AddPictureToGroup( vector<wstring>* localPaths, const string groupId )
{
	vector<PictureData*>* vPictures = NULL;
	RETURN_FALSE_IF( !localPaths || localPaths->empty() );

	BaseGroup* pBaseGroup = GetBaseGroup(groupId);
	RETURN_FALSE_IF( ! pBaseGroup );

	int nextNumber, index;
	//wstring firstGroupName = L"";
	if ( pBaseGroup->GetType() == PICTURE_TYPE_WALLPAPER )
	{
		_pWallpaper->GetMaxNumber(nextNumber, index);
		vPictures = _pIosImpl->AddSpecialPictures(localPaths, nextNumber+1, index+1, groupId);
		//firstGroupName = pBaseGroup->GetName();
	}
	else if ( pBaseGroup->GetType() == PICTURE_TYPE_NORMAL )
	{
		_pAllPictures->GetMaxFileData(nextNumber, index);
		vPictures = _pIosImpl->AddPictureToGroup(localPaths, groupId, nextNumber+1, index+1, _PictureGroups[0]->_id);
		//firstGroupName = pBaseGroup->GetName();

		if (vPictures && !vPictures->empty())
		{
			for (size_t i=0; i<vPictures->size(); ++i)
			{
				// ios: 假如不是添加到【所有图片】分类, 【所有图片】分类中也要增加
				// 注意: 此处假设第一个分类就是【所有图片】分类, 未来如有变化再调整
				if ( _PictureGroups[0]->GetId() != pBaseGroup->GetId() )
				{
					_PictureGroups[0]->InsertPicture(vPictures->at(i));
// 					BaseGroup* pBaseGroupAll = GetBaseGroup(_PictureGroups[0]->_id);
// 					RETURN_FALSE_IF( ! pBaseGroupAll );
				}
				_pAllPictures->Add(vPictures->at(i));
			}		
		}
	}
	else
	{		
		// 相册处理
		if (_pCamera->GetPictures()->empty() )
		{
			// 照相机的照片没有时, 可能相簿目录不存在
			FileHelper::CreateDir(_pDevice, _pDevice->GetIosPicturePath()->GetIosAlbumPath());
			FileHelper::CreateDir(_pDevice, _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath());
		}

		int i, j, m, n;
		_pCamera->GetMaxNumber(i, m);
		_pScreenShot->GetMaxNumber(j, n);
		nextNumber = max(i, j);
		index = max(m, n) ;
		vPictures = _pIosImpl->AddSpecialPictures(localPaths, nextNumber+1, index+1, groupId);
		//firstGroupName = L"照相机";
	}
	RETURN_FALSE_IF ( !vPictures || vPictures->empty());
	
	for (size_t i=0; i<vPictures->size(); ++i)
	{
		const PictureData* pData = vPictures->at(i);
		RETURN_FALSE_IF (NULL == pData);
		pBaseGroup->InsertPicture(pData);
	}

	//GeneratePlistFile(vPictures,_pIosImpl->GetPictureUuids(),firstGroupName);

	RELEASE_VECTOR((*vPictures));
	SAFE_DELETE(vPictures);

	return true;
}

char* IosPictureManager::GetLibraryUUIDFromDevice()
{
	wstring libPlistRemotePath = L"/private/var/mobile/Media/Photos/Sync/CurrentLibraryUUID.plist";
	wstring libPlistLocalPath = L"d:\\latestCodes\\CurrentLibraryUUID.plist"; // To be updated

	 _pIosImpl->_pWallpaper->GetIosFileIO()->Ios_DownFromPhone(libPlistLocalPath, libPlistRemotePath);

	string libraryUuid;
	PlistOperation::XmlPlistGetKey(libPlistLocalPath, "libraryUUID", libraryUuid);

	int nLen = libraryUuid.length();
	char* destUuid = new char[nLen+1];
	memset(destUuid, 0, nLen+1);
	memcpy(destUuid, libraryUuid.c_str(), nLen+1);

	return destUuid;
}

void IosPictureManager::GeneratePlistFile(vector<PictureData*>* pVecPicture, vector<char*> uuids, wstring groupName)
{
	unsigned int sizeofUuid = uuids.size();
	if ( pVecPicture->size() != sizeofUuid )
	{
		LOG_DEBUG(L"The number of pictures and uuids should be the same!")
			return;
	}
	// Construct a complex dictionary object
	void* dictKeyCallBacks = CCFConverter::NDkCFTypeDictionaryKeyCallBacks();
	void* dictValueCallBacks = CCFConverter::NDkCFTypeDictionaryValueCallBacks();
	void* typeArrayCallBacks = CCFConverter::NDkCFTypeArrayCallBacks();

	// Library part
	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
	void* pKey = CCFConverter::ND__CFStringMakeConstantString("libraryKind");
	void* pValue = CCFConverter::ND__CFStringMakeConstantString("Folder");
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);
	pKey = CCFConverter::ND__CFStringMakeConstantString("libraryUUID");
	char* libraryUuid = GetLibraryUUIDFromDevice();
	pValue = CCFConverter::StringtoCFString(libraryUuid);
	pValue = CCFConverter::ND__CFStringMakeConstantString(libraryUuid);
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pValue);

	//Picture dict
	void* pCFArray = CCFConverter::NDCFArrayCreateMutable(NULL, 0, typeArrayCallBacks);
	void* pKeyCaption = CCFConverter::ND__CFStringMakeConstantString("caption");
	void* pValueCaption  = CCFConverter::ND__CFStringMakeConstantString("");
	void* pKeyIsVideo = CCFConverter::ND__CFStringMakeConstantString("isVideo");
	void* pValueIsVideo  = CCFConverter::NDkCFBooleanFalse();
	void* pKeyItemType = CCFConverter::ND__CFStringMakeConstantString("itemType");
	void* pValueItemType = CCFConverter::ND__CFStringMakeConstantString("Asset");
	void* pCFValueInfinity = CCFConverter::NDkCFNumberPositiveInfinity();
	void* pKeyLatitude = CCFConverter::ND__CFStringMakeConstantString("latitude");
	void* pValueLatitude  = CCFConverter::NDCFNumberCreate(NULL, kCFNumberFloat32Type, pCFValueInfinity);
	void* pKeyLongitude = CCFConverter::ND__CFStringMakeConstantString("longitude");
	void* pValueLongitude  = CCFConverter::NDCFNumberCreate(NULL, kCFNumberFloat32Type, pCFValueInfinity);
	void* pKeyModifDate = CCFConverter::ND__CFStringMakeConstantString("modificationDate");
	void* pKeyUuid = CCFConverter::ND__CFStringMakeConstantString("uuid");
	void* pCFArrayUuid = CCFConverter::NDCFArrayCreateMutable(NULL, 0, typeArrayCallBacks);
	void* pCFDictI;
	void* pValueUuidI;
	for(unsigned int i = 0; i < sizeofUuid; ++i) // Add the information of each picture
	{
		pCFDictI = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyCaption, pValueCaption);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyIsVideo, pValueIsVideo);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyItemType, pValueItemType);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyLatitude, pValueLatitude);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyLongitude, pValueLongitude);
		time_t modifyTime = pVecPicture->at(i)->_modifyTime;
		void* pValueModifDate  = CCFConverter::NDCFNumberCreate(NULL, kCFNumberSInt32Type, &modifyTime);
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyModifDate, pValueModifDate);
		pValueUuidI = CCFConverter::ND__CFStringMakeConstantString(uuids.at(i));
		CCFConverter::NDCFDictionarySetValue(pCFDictI, pKeyUuid, pValueUuidI);
		CCFConverter::NDCFArrayAppendValue(pCFArray, pCFDictI);

		CCFConverter::NDCFArrayAppendValue(pCFArrayUuid, pValueUuidI);  // picture uuids in album dictionary
	}

	// Add album dictionary
	void* pCFDictAlbum = CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, dictKeyCallBacks, dictValueCallBacks);
	void* pKeyAlbumName = CCFConverter::ND__CFStringMakeConstantString("albumName");
	string AlbumName = ConverWstringtoString(groupName);
	void* pValueAlbumName = CCFConverter::StringtoCFString(AlbumName);
	CCFConverter::NDCFDictionarySetValue(pCFDictAlbum, pKeyAlbumName, pValueAlbumName);
	void* pKeyAssetUuid = CCFConverter::ND__CFStringMakeConstantString("assetUUID");
	CCFConverter::NDCFDictionarySetValue(pCFDictAlbum, pKeyAssetUuid, pCFArrayUuid);
	void* pValueItemTypeAlbum = CCFConverter::ND__CFStringMakeConstantString("Album");
	CCFConverter::NDCFDictionarySetValue(pCFDictAlbum, pKeyItemType, pValueItemTypeAlbum);
	int subclass = 1; // To be updated
	void* pKeySubclass = CCFConverter::ND__CFStringMakeConstantString("subclass");
	void* pValueSubclass  = CCFConverter::NDCFNumberCreate(NULL, kCFNumberSInt32Type, &subclass);
	CCFConverter::NDCFDictionarySetValue(pCFDictAlbum, pKeySubclass, pValueSubclass);
	void* pValueUuidAlbum  = CCFConverter::ND__CFStringMakeConstantString("00000000-0000-0000-0000-000000000063");
	CCFConverter::NDCFDictionarySetValue(pCFDictAlbum, pKeyUuid, pValueUuidAlbum);
	// Add album dictionary to array
	CCFConverter::NDCFArrayAppendValue(pCFArray, pCFDictAlbum);

	pKey = CCFConverter::ND__CFStringMakeConstantString("updates");
	CCFConverter::NDCFDictionarySetValue(pCFDict, pKey, pCFArray);

	PlistOperation::GenerateTempPlist(pCFDict);

	CCFConverter::NDCFRelease(pCFDict);
	CCFConverter::NDCFRelease(pKey);
	CCFConverter::NDCFRelease(pValue);
	CCFConverter::NDCFRelease(pCFArray);
	CCFConverter::NDCFRelease(pKeyCaption);
	CCFConverter::NDCFRelease(pValueCaption);
	CCFConverter::NDCFRelease(pKeyIsVideo);
	CCFConverter::NDCFRelease(pValueIsVideo);
	CCFConverter::NDCFRelease(pKeyItemType);
	CCFConverter::NDCFRelease(pValueItemType);
	CCFConverter::NDCFRelease(pKeyLatitude);
	CCFConverter::NDCFRelease(pValueLatitude);
	CCFConverter::NDCFRelease(pKeyLongitude);
	CCFConverter::NDCFRelease(pValueLongitude);
	CCFConverter::NDCFRelease(pKeyModifDate);
	CCFConverter::NDCFRelease(pKeyUuid);
	CCFConverter::NDCFRelease(pCFArrayUuid);
	CCFConverter::NDCFRelease(pCFDictI);
	CCFConverter::NDCFRelease(pValueUuidI);
	CCFConverter::NDCFRelease(pCFDictAlbum);
	CCFConverter::NDCFRelease(pKeyAlbumName);
	CCFConverter::NDCFRelease(pValueAlbumName);
	CCFConverter::NDCFRelease(pKeyAssetUuid);
	CCFConverter::NDCFRelease(pValueItemTypeAlbum);
	CCFConverter::NDCFRelease(pKeySubclass);
	CCFConverter::NDCFRelease(pValueSubclass);
	CCFConverter::NDCFRelease(pValueUuidAlbum);
}

int ND91Assistant::IosPictureManager::GetPhotoCount()
{
    return GetAllPicturesCount();
}

#endif