#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/AndroidPhotoBucket.h"
#include "Module/Picture/PictureData.h"
#include "Common/Path.h"

PictureFolder::PictureFolder(const DeviceData* pDevice,
                             std::wstring picPath,
                             PICTURE_TYPE type)
: BaseGroup(pDevice)
{
    _path       = picPath;
    _type       = type;
}

void PictureFolder::SetBucketData( AndroidPhotoBucket* pContainer )
{
    _id = pContainer->_id;
    _name = pContainer->_name;
    _path = pContainer->_path;
    _visible = pContainer->_visible;
}

PictureFolder::~PictureFolder(void)
{
    Clear();
}

void ND91Assistant::PictureFolder::InsertPicture(NodeData* pNode, PICTURE_TYPE type)
{
	PictureData* pPicture = new PictureData(pNode->GetFullPath(), type);
    
    pPicture->_size		  = pNode->_size;
    pPicture->_modifyTime = pNode->_modifyTime;
    pPicture->_createTime = pNode->_createTime;
	pPicture->_remotePath = pNode->GetFullPath();

	wstring tempPath = CFileOperation::GetFileNameNoExt(pPicture->_remotePath);

	if ( type != PICTURE_TYPE_WALLPAPER )
	{
		if (type == PICTURE_TYPE_PHOTO)
		{
			pPicture->SetCategoryId(CAMERAID);
			pPicture->_localPath   = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + tempPath + PICJPG;
		}
		else if (type == PICTURE_TYPE_SCREENSHOT )
		{
			pPicture->SetCategoryId(SCREENSHOTID);
			pPicture->_localPath   = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + tempPath + PICPNG;
		}
		
		pPicture->_thumbRemotePath = _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + tempPath + THMPIC;
		pPicture->_thumbLocalPath  = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + tempPath + THMPIC;
	}
	else
	{
		pPicture->_localPath	   = _pDevice->GetIosPicturePath()->GetLocalWallPaperPath() + tempPath + PICPNG;
		pPicture->_thumbRemotePath = _pDevice->GetIosPicturePath()->GetIosWallPaperPath()   + tempPath + THUMBNAIL;
		pPicture->_thumbLocalPath  = _pDevice->GetIosPicturePath()->GetLocalWallPaperPath() + tempPath + THUMBNAIL;
		pPicture->SetCategoryId(WALLPAPERID);
	}
    _pictures.push_back(pPicture);
}

void PictureFolder::InsertPicture(const PictureData* pic )
{
    if (_pDevice->GetDevInfo()->_deviceType == Android)
        InsertPictureAndroid(pic);
     else
        _pictures.push_back(new PictureData(pic));
}

void PictureFolder::InsertPictureAndroid(const PictureData* pic )
{
    bool has = false;
    for (size_t i = 0; i < _pictures.size(); i++)
    {
        PictureData* picture = _pictures.at(i);
        if (picture->GetId() == pic->GetId())
            has = true;
    }

    if (!has)
        _pictures.push_back(new PictureData(pic));
}

void ND91Assistant::PictureFolder::RemovePictureFromList(const PictureData* pic)
{
	for (vector<PictureData*>::iterator it = _pictures.begin(); it!=_pictures.end(); it++)
	{
		bool found = _pDevice->GetDevInfo()->_deviceType==Android?
					(*it)->GetId() == pic->GetId():(*it)->_remotePath == pic->_remotePath;
		if(found)
		{
			SAFE_DELETE((*it));
			_pictures.erase(it);
			return;
		}
	}
}


void ND91Assistant::PictureFolder::Clear()
{
    RELEASE_VECTOR(_pictures);
}

const vector<PictureData*>* ND91Assistant::PictureFolder::GetPictures()
{
    return &_pictures;
}

void PictureFolder::GetMaxNumber(int& number, int& index)
{
    const PictureData* pPicture = NULL;

    // 用户自定义壁纸, 我们默认从300开始
    number = _type == PICTURE_TYPE_WALLPAPER ? 300 : 0;
    index = 0;
    for (size_t i = 0; i < _pictures.size(); i++)
    {
        pPicture = _pictures[i];
        if ( ! pPicture ) continue;

        wstring name = pPicture->_name;
        if ( _type == PICTURE_TYPE_PHOTO || _type == PICTURE_TYPE_SCREENSHOT )
        {
            name = name.substr(4);  // 去掉照片前面的"img_"
        }

        int n = _wtoi(name.c_str());
        if ( n > number )
            number = n;

        if ( pPicture->_thumbnailIndex > index)
            index = pPicture->_thumbnailIndex;
    }
}

#endif