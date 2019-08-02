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
    // ���������Ŀ¼���������ִ����ʱ���ݵ�Ŀ¼
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocaliPhotoPath      ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalThumbsPath      ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalPicPath         ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalThumbnailPath   ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalMediaPath       ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalAlbumPath       ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocal100APPLEPath    ()));
	RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalWallPaperPath   ()));
    RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosPicturePath()->GetLocalBackground      ()));

	// ��ʼ��ͼƬ����ӿ� 
	_pIosImpl = CreateIosImplement();

	// ���������ļ���
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
    // �ӷ��༯�����Ƴ��˷���
    bool bNotFound = true;
    vector<PictureGroup*>::iterator itGroup;
    for ( itGroup =_PictureGroups.begin(); itGroup != _PictureGroups.end(); itGroup++)
    {
        if ( (*itGroup)->_id == pGroup->_id )
        {
            // ɾ������
            _PictureGroups.erase(itGroup);
            bNotFound = false;
            break;
        }
    }
    RETURN_FALSE_IF( bNotFound );    // ����û�ҵ�

	vector<PictureData*> pVerPicDatas;
	pGroup->GetPictures(&pVerPicDatas);
	// ��ͼƬ���ݿ����Ƴ��˷���
	RETURN_FALSE_IF( !_pIosImpl->DeleteGalleryGroup( atoi(pGroup->_id.c_str()), &pVerPicDatas) );

	// ���豸�����ݿ���ȥ���˷��༰��ͼƬ
	const list<int>* pList = pGroup->GetPictures();
	list<int>::const_iterator it = pList->begin();
	for(; it != pList->end(); ++it )
	{
		if ( ! ContainsInGroup( *it ) ) // ios�����������඼��������ͼƬ��ɾ��
		{
			_pAllPictures->Del(*it);
		}
	}

    SAFE_DELETE(pGroup);    // �ͷŷ������
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

	// �����ͼ������ƶ���ͼ�����
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

	// �豸����
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

		// ɾ�������ļ�
        CFileOperation::DeleteFile(pData->_localPath);
        CFileOperation::DeleteFile(pData->_thumbLocalPath);

		//����������ͷ�Pictures
		pGroup->RemovePictureFromList(pData);

		if ( pGroup->GetType() == PICTURE_TYPE_NORMAL )
		{
			// ios��ͼƬ���ܴ����ڶ������, Ҫ����������ɾ��
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
	// ��ʼͬ��
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

	// ͬ���˴���Ҫ�ϴ�������ԭͼ�ļ�
	RETURN_FALSE_IF ( !_pIosImpl->UploadPictureFiles(pSubject,deviceversion<L"4.0"?15:20) );
	// ͬ���˴���Ҫɾ��������ԭͼ�ļ�
	RETURN_FALSE_IF ( !_pIosImpl->DeletePictreFiles() );
	_pIosImpl->ReleaseCase();

	if ( _pIosImpl->GetPhotoDataIthmbState() )
	{
		RETURN_FALSE_IF ( !_pIosImpl->PhotoDataIthmbSync(pSubject,deviceversion<L"5.0"?7:17) );
	}
	if ( _pIosImpl->GetPhotosIthmbState() )
		RETURN_FALSE_IF ( !_pIosImpl->PhotosIthmbSync(pSubject,deviceversion<L"4.0"?22:10) );

	// ����ͬ��
	usbSync.StopSync();
	RETURN_FALSE_IF ( !IosSyncHelper() );

	_pIosImpl->ResetSyncState();	

	return true;
}

bool IosPictureManager::IosSyncThroughATHost()
{
	//��ʼͬ��
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

	// ����ͬ��
	usbSync.StopSync();
	RETURN_FALSE_IF ( !IosSyncHelper() );

	_pIosImpl->ResetSyncState();

	return true;
}


bool IosPictureManager::IosSyncHelper()
{
	DirectoryOperationCommand dirCmd(_pDevice);

	// --- 421�汾�Ļ����ļ���ɾ������������ݲ�ˢ��---
	if (dirCmd.Exist(_pDevice->GetIosPicturePath()->GetIos421CacheFile()))
	{
		dirCmd.DeleteFileLink(_pDevice->GetIosPicturePath()->GetIos421CacheFile());
	}
	if (dirCmd.Exist(_pDevice->GetIosPicturePath()->GetIpad421CachePath()))
	{
		dirCmd.DeleteFileLink(_pDevice->GetIosPicturePath()->GetIpad421CachePath());
	}

	// ɾ��iTunes��ͼƬĿ¼��¼,����iTunes��ͬ��ͼ���ʱ��ͻ���ʾ�Ѿ��������������ͬ����
	// �Ӷ�����iTunes��ͬ����ʱ���ͼ���ƻ���
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
	// ���ܴ�iPhone��"����ͼƬ" ���Ƴ�
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
    // ������ͼƬ������������
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
				// ios: ���粻����ӵ�������ͼƬ������, ������ͼƬ��������ҲҪ����
				// ע��: �˴������һ��������ǡ�����ͼƬ������, δ�����б仯�ٵ���
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
		// ��ᴦ��
		if (_pCamera->GetPictures()->empty() )
		{
			// ���������Ƭû��ʱ, �����ಾĿ¼������
			FileHelper::CreateDir(_pDevice, _pDevice->GetIosPicturePath()->GetIosAlbumPath());
			FileHelper::CreateDir(_pDevice, _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath());
		}

		int i, j, m, n;
		_pCamera->GetMaxNumber(i, m);
		_pScreenShot->GetMaxNumber(j, n);
		nextNumber = max(i, j);
		index = max(m, n) ;
		vPictures = _pIosImpl->AddSpecialPictures(localPaths, nextNumber+1, index+1, groupId);
		//firstGroupName = L"�����";
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