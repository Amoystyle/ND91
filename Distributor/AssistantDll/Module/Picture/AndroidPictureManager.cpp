#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/AndroidPictureManager.h"
#include "Module/Picture/PictureLibrary.h"
#include "Module/Picture/LoadPhotoBucketsCommand.h"
#include "Module/Picture/LoadPhotosCommand.h"
#include "Module/Picture/AndroidPhotoBucket.h"
#include "Module/Picture/LoadThumbnailsCommand.h"
#include "Module/Picture/AddPictureCommand.h"
#include "Module/Picture/PictureOperationCommand.h"
#include "Module/Picture/SetWallpaperCommand.h"
#include "Module/Picture/AddPhotoBucketCommand.h"

#include "Monitor/DefaultProgressObserver.h"
#include "Common/Path.h"

AndroidPictureManager::AndroidPictureManager(const DeviceData* pDevice)
: PictureManager(pDevice)
{

}

AndroidPictureManager::~AndroidPictureManager(void)
{

}

bool AndroidPictureManager::ExistBucket(string id)
{
	RETURN_TRUE_IF ( _pCamera && _pCamera->_id == id );

	RETURN_TRUE_IF ( _pWallpaper && _pWallpaper->_id == id );

	for (size_t i = 0; i < _PictureGroups.size(); i++)
	{
		RETURN_TRUE_IF(_PictureGroups[i]->_id == id)
	}

	return false;
}

bool AndroidPictureManager::Load()
{
    RELEASE_VECTOR(_PictureGroups);
    _pAllPictures->Clear();

	LoadAndroidBucket();
	LoadAndroidPicture();
    LoadAndroidThumbnails();

    return true;
}

void AndroidPictureManager::LoadAndroidBucket()
{
	LoadPhotoBucketsCommand cmd(_pDevice);
	cmd.Execute();
	const deque<AndroidPhotoBucket*> buckets= cmd.GetPhotoBuckets();

	for(size_t i = 0;i < buckets.size(); i++)
	{
        CreateAndroidBucketInMemory(buckets[i]);
    }
}

void AndroidPictureManager::LoadAndroidPicture()
{
	// ��������ͼƬ
	LoadPhotosCommand cmd(_pDevice);
	cmd.Execute();
	const deque<PictureData*> pictures = cmd.GetPictures();

	// ���ͼƬ
	for(size_t i = 0; i < pictures.size(); i++)
	{
		PictureData* pPicture = pictures[i];
        pPicture->SetType(AndroidPhotoBucket::GetBucketTypeByPath(pPicture->_remotePath));

        if ( ! ExistBucket(pPicture->GetCategoryId()))            // ��������
		{
            AndroidPhotoBucket bucket; 
            bucket._id = pPicture->GetCategoryId();
            bucket._path = CFileOperation::GetFileDir(pPicture->_remotePath);
			bucket._path = CStrOperation::trimRightW(bucket._path, L"/");
            bucket._visible = true;

            // ��Ŀ¼����Ϊ������
            wstring tmp = bucket._path;
            if ( *tmp.rbegin() == '/' )
                tmp.erase(tmp.size()-1);
            bucket._name = CFileOperation::GetFileName(tmp);

            CreateAndroidBucketInDevice(&bucket);
            CreateAndroidBucketInMemory(&bucket);
		}

        _pAllPictures->Add(pPicture);

        // ͼƬ�ŵ�����
        BaseGroup* pGroup = GetBaseGroup(pPicture->GetCategoryId());
        if ( pGroup )
            pGroup->InsertPicture(pPicture);
    }

    // ����Ƭ���������
	CreateCamera(L"sdcard/DCIM/Camera");
    if ( _pCamera->_id.empty() )
    {
        _pCamera->_id = CreateAndroidBucket(L"/sdcard/DCIM/Camera");
    }
    // �ޱ�ֽ���������
    CreateWallpaper(L"sdcard/wallpaper");
    if ( _pWallpaper->_id.empty() )
    {
        _pWallpaper->_id = CreateAndroidBucket(L"/sdcard/wallpaper");
    }
}

void AndroidPictureManager::LoadAndroidThumbnails()
{
	LoadThumbnailsCommand cmd(_pDevice);
	cmd.Execute();
	deque<ThumbnailData*> thumbnails = cmd.GetThumbNails();

    PictureData* pData;
	for(size_t i=0; i < thumbnails.size();i++)
	{
        pData = _pAllPictures->Get(thumbnails[i]->_imageId);
        if ( pData)
            pData->SetThumbnail(thumbnails[i]);
	}
}

string AndroidPictureManager::AddPhotoGroup( const wstring displayname,  wstring path)
{

	wstring grouppath = path;
	if(path.empty())
		grouppath = L"/sdcard/image/" + CStrOperation::trimLeftW(displayname,L" ");
	return CreateAndroidBucket(grouppath);

}

bool AndroidPictureManager::RenameGroup(const string nGroupid, const wstring name)
{
	BaseGroup* pBaseGroup = GetBaseGroup(nGroupid);
	RETURN_FALSE_IF( ! pBaseGroup );
	
	PictureOperationCommand cmd(_pDevice);
	RETURN_FALSE_IF ( ! cmd.RenamePhotoBucket(nGroupid, pBaseGroup->GetPath(), pBaseGroup->GetName(), name) );

	pBaseGroup->SetName(name);

	return true;
}

bool AndroidPictureManager::DeleteGroup(PictureGroup* pGroup)
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

    // ���豸�����ݿ���ȥ���˷��༰��ͼƬ
    const list<int>* pList = pGroup->GetPictures();
    list<int>::const_iterator it = pList->begin();

    PictureOperationCommand cmd(_pDevice);    // ���豸���Ƴ��˷���
	RETURN_FALSE_IF( ! cmd.RemovePhotoBucket(pGroup->_id, pGroup->_path, pGroup->_name));

    for(; it != pList->end(); ++it )
    {
        _pAllPictures->Del(*it); // ��׿��һ��ͼƬ������һ��������, ��ȫ��ɾ��
    }

    SAFE_DELETE(pGroup);    // �ͷŷ������
    return true;
}


bool AndroidPictureManager::MovePicture( const vector<const PictureData*>* vPictures, 
											string sourceGroupId, 
											string destGroupId )
{
	RETURN_FALSE_IF ( !vPictures || vPictures->empty() );
	RETURN_TRUE_IF ( sourceGroupId == destGroupId );

	// ��ӵ��·���
	BaseGroup* pGroup = GetBaseGroup(destGroupId);//֮ǰ�˴�����id���ԣ����´����˾ɷ���������������id�ɷ��������·���
	RETURN_FALSE_IF ( ! pGroup );
	PICTURE_TYPE destGroupType = pGroup->GetType();
	PICTURE_TYPE srcGroupType = GetBaseGroup(sourceGroupId)->GetType();

	for ( size_t i=0; i<vPictures->size(); ++i )
	{
		PictureData* pdata = (PictureData*)vPictures->at(i);
		if(destGroupType == PICTURE_TYPE_NORMAL && srcGroupType == destGroupType)
		{
			RETURN_FALSE_IF( ! PictureOperationCommand(_pDevice).MovePicToCategory
				(pdata->GetId(), destGroupId, pGroup->GetName()));
			pdata->SetCategoryId(destGroupId);
			pGroup->InsertPicture(vPictures->at(i));
		}
		else
		{// ���ļ��ѹ�ȥ
			wstring destpath = pGroup->GetPath();
			if ( *destpath.rbegin() != '/' )
				destpath.push_back('/');
			destpath += CFileOperation::GetFileName(pdata->_remotePath);
			DirectoryOperationCommand cmd(_pDevice);
			if(cmd.Move(pdata->_remotePath, destpath))
				pdata->_remotePath = destpath;

			AddPictureCommand addcmd(_pDevice);
			addcmd.SetParam(destpath, pdata->_localPath, destGroupId, pGroup->GetName());
			addcmd.Execute();
			const PictureData* newpic = addcmd.GetRetPicture();
			if(newpic)
			{
				pGroup->InsertPicture(newpic);
				if(destGroupType == PICTURE_TYPE_NORMAL)
					_pAllPictures->Add(newpic);
			}

			PictureOperationCommand(_pDevice).DeletePicture( pdata->GetId());
		}

		// ��ԭ����ɾ��
		pGroup = GetBaseGroup(sourceGroupId);

		RETURN_FALSE_IF ( ! pGroup );
		pGroup->RemovePictureFromList(pdata);
		if(pGroup->GetType() == PICTURE_TYPE_NORMAL && pGroup->GetType() != destGroupType )
		{
			_pAllPictures->Del(pdata->GetId());
		}
	}

	return true;
}


bool AndroidPictureManager::DeletePicture( const vector<const PictureData*>* vPictures, const string groupId )
{
	RETURN_FALSE_IF ( !vPictures || vPictures->empty() );

	BaseGroup* pGroup = GetBaseGroup(groupId);
	RETURN_FALSE_IF ( ! pGroup );

	for ( size_t i=0; i<vPictures->size(); ++i )
	{
		// ɾ�������ļ�
        CFileOperation::DeleteFile(vPictures->at(i)->_localPath);
        CFileOperation::DeleteFile(vPictures->at(i)->_thumbLocalPath);

		//TODO::��Ҫ�ж��Ƿ������������,���²��ڴ˷���Ҳ�ᱻɾ��
		RETURN_FALSE_IF(vPictures->at(i)->GetCategoryId() != pGroup->GetId());
		RETURN_FALSE_IF( ! PictureOperationCommand(_pDevice).DeletePicture( vPictures->at(i)->GetId()) );
		pGroup->RemovePictureFromList(vPictures->at(i));
		if ( pGroup->GetType() == PICTURE_TYPE_NORMAL )
			_pAllPictures->Del(vPictures->at(i));
	}

	return true;

}

void AndroidPictureManager::CreateAndroidBucketInMemory(AndroidPhotoBucket* pBucket)
{
    switch (AndroidPhotoBucket::GetBucketTypeByPath(pBucket->_path))
    {
    case PICTURE_TYPE_NORMAL:
        {
			if(!FindGroup(pBucket->_id))//һ��Ҫ��ͬid������ɾ���ֳ�����
			{
				PictureGroup* pGroup = new PictureGroup(_pDevice, _pAllPictures);
				pGroup->SetBucketData(pBucket);
				_PictureGroups.push_back(pGroup);
			}
        }
        break;

    case PICTURE_TYPE_PHOTO:
        CreateCamera(pBucket->_path)->SetBucketData(pBucket);
        break;

    case PICTURE_TYPE_WALLPAPER:
        CreateWallpaper(pBucket->_path)->SetBucketData(pBucket);
        break;

    default:
        break;
    }
}

string AndroidPictureManager::CreateAndroidBucket(const wstring path)
{
    AndroidPhotoBucket bucket; 
	bucket._path = CStrOperation::trimRightW(path, L"/");
	bucket._id = CStrOperation::CreateHashCode(CCodeOperation::UnicodeToUTF_8(bucket._path));
    bucket._visible = true;
    bucket._name = CFileOperation::GetFileName(bucket._path);

	RETURN_VALUE_IF(GetBaseGroup(bucket._id), bucket._id);

    RETURN_VALUE_IF( !CreateAndroidBucketInDevice(&bucket), "");

    CreateAndroidBucketInMemory(&bucket);
    return bucket._id;
}

bool AndroidPictureManager::CreateAndroidBucketInDevice( AndroidPhotoBucket* Bucket )
{
    RETURN_FALSE_IF ( ! Bucket || Bucket->_path.empty() );

    AddPhotoBucketCommand addbucketCmd(_pDevice);
    addbucketCmd.SetParam(Bucket);
    addbucketCmd.Execute();
    return addbucketCmd.Success();
}

const PictureData* AndroidPictureManager::AndroidAddPicture( wstring filePath, const string groupId )
{
	RETURN_NULL_IF( !MyFileOperation::IsSupportImageFile(filePath) );

	BaseGroup* pBaseGroup = GetBaseGroup(groupId);
	RETURN_FALSE_IF( ! pBaseGroup );

	// �����豸���ļ�·��
	wstring remotepath = pBaseGroup->GetPath();
	if ( *remotepath.rbegin() != '/' )
		remotepath.push_back('/');
	remotepath += CFileOperation::GetFileName(filePath);

	// �ϴ��ļ�
	DefaultProgressObserver observer(RENAME_ALL);  // �ļ��Ѵ�����������
	UploadFileCommand cmd(_pDevice);
	cmd.Attach(&observer);
	cmd.Upload(filePath, remotepath);
	remotepath = cmd.GetDestFilePath(); // ȡ�����������ļ���(��δ�������򲻱�)

	// ���ͼƬ��ͼ��
	AddPictureCommand  addPicCmd(_pDevice);
	addPicCmd.SetParam(remotepath, filePath, pBaseGroup->GetId(), pBaseGroup->GetName());
	addPicCmd.Execute();
	return addPicCmd.GetRetPicture();	
}


bool AndroidPictureManager::AddPictureToGroup( vector<wstring>* localPaths, const string groupId )
{
	RETURN_FALSE_IF( !localPaths || localPaths->empty() );

	BaseGroup* pBaseGroup = GetBaseGroup(groupId);
	RETURN_FALSE_IF( ! pBaseGroup );

	for ( size_t i = 0; i < localPaths->size(); ++i )
	{
        RETURN_FALSE_IF( !CFileOperation::IsFileExist(localPaths->at(i).c_str()) );

		const PictureData* pData = AndroidAddPicture(localPaths->at(i), groupId);
        RETURN_FALSE_IF( !pData );

		_pAllPictures->Add(pData);
		pBaseGroup->InsertPicture(pData);
		SAFE_DELETE(pData);
	}

	return true;
}


bool AndroidPictureManager::DownloadThumbnail(PictureData* pPicture)
{
	if( ! pPicture->_thumbRemotePath.empty() )
	{
		if( pPicture->_thumbLocalPath.empty() )       // ���ر�������ͼ���ļ���(ȫ·��)
		{
			pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath();
			CFileOperation::MakeLocalDir(pPicture->_thumbLocalPath);
			pPicture->_thumbLocalPath += CFileOperation::GetFileName(pPicture->_thumbRemotePath);
		}

		RETURN_TRUE_IF( CFileOperation::IsFileExist(pPicture->_thumbLocalPath.c_str()));
		RETURN_TRUE_IF( DownloadFileCommand(_pDevice).Download(pPicture->_thumbLocalPath, pPicture->_thumbRemotePath));
	}

	// ����޷���������ͼ, ������ԭͼ������һ������ͼ
	RETURN_FALSE_IF( ! DownloadPicture(pPicture) );
	RETURN_FALSE_IF( ! CFileOperation::IsFileExist(pPicture->_localPath.c_str()));

	pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath();
	CFileOperation::MakeLocalDir(pPicture->_thumbLocalPath);

	wostringstream woss;
	woss << /*_time32(NULL)*/clock() << PICJPG;
	pPicture->_thumbLocalPath += woss.str();

	int width = 256;
	int height= 256;
	if(MakeThumbnail(pPicture->_localPath, width, height, pPicture->_thumbLocalPath, false))
	{
		pPicture->_thumbHeight	= height;
		pPicture->_thumbWidth		= width;
	}

	return true;
}


bool AndroidPictureManager::DownloadPicture(const PictureData* pPicture, wstring customPath)
{
    wstring path;

    if (customPath.empty())
        path = pPicture->_localPath;
    else
        path = customPath;

	if ( path.empty() )
	{
		path = _pDevice->GetAndroidPath()->GetPicturePath();
		path.erase(path.size() - 1);
		path += CStrOperation::ReplaceW(pPicture->_remotePath, L"/",L"\\");
	}

	if (! CFileOperation::IsFileExist(path.c_str()) )
	{
		if ( ! DownloadFileCommand(_pDevice).Download(path, pPicture->_remotePath) )
		{
			Sleep(100);     // Android�ػ���ʱ��Ӧ����, �˴��ṩһ�����Ի���
			RETURN_FALSE_IF( ! DownloadFileCommand(_pDevice).Download(path, pPicture->_remotePath));
		}
	}
	pPicture->_localPath = path;
	return true;

}

bool AndroidPictureManager::SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType)
{
    // ������ͼƬ������������
    if ( !CFileOperation::IsFileExist(pPicture->_localPath.c_str()) )
    {
        RETURN_FALSE_IF( ! DownloadPicture(pPicture) );
    }
 	//TODO:�Ŵ���
 	return SetWallpaperCommand(_pDevice).SetLocalImageAsWallPaper(pPicture->_localPath);
}

bool AndroidPictureManager::ChangeImage( PictureData* pPicture, wstring srcPath )
{
    RETURN_FALSE_IF( ! pPicture );

	// ��ȡͼ��
	Image* pImage = Image::FromFile(srcPath.c_str());
	RETURN_FALSE_IF( ! pImage );

	// ȡ��ͼ����
	pPicture->_width = pImage->GetWidth();
	pPicture->_height = pImage->GetHeight();
	SAFE_DELETE(pImage);

	// ���Ǳ����ļ�
    RETURN_FALSE_IF(!CFileOperation::CopyFile(srcPath, pPicture->_localPath, false));

	// �ϴ����ֻ�
	UploadFileCommand cmd(_pDevice);
	OverwriteObserver observer; // �����ļ�
	cmd.Attach(&observer);
	RETURN_FALSE_IF( ! cmd.Upload(pPicture->_localPath, pPicture->_remotePath) );

	if(!pPicture->_thumbLocalPath.empty())
		MakeThumbnail(pPicture->_localPath, pPicture->_thumbWidth, pPicture->_thumbHeight, pPicture->_thumbLocalPath, false);

	// ���������ͼ, ����
	if( ! pPicture->_thumbRemotePath.empty() )
	{
		if ( pPicture->_thumbLocalPath.empty())
		{
			pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalThumbnailPath();
			CFileOperation::MakeLocalDir(pPicture->_thumbLocalPath);
			pPicture->_thumbLocalPath += CFileOperation::GetFileName(pPicture->_thumbRemotePath);
			RETURN_FALSE_IF( ! MakeThumbnail(pPicture->_localPath, pPicture->_thumbWidth, 
							pPicture->_thumbHeight, pPicture->_thumbLocalPath, false));
		}

		RETURN_FALSE_IF( ! cmd.Upload(pPicture->_thumbLocalPath, pPicture->_thumbRemotePath) );
	}

	return true;

}

int ND91Assistant::AndroidPictureManager::GetPhotoCount()
{
    return LoadPhotosCommand(_pDevice).GetPictureCounts();
}

#endif

