#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PictureManager.h"
#include "Module/Picture/PictureLibrary.h"

PictureManager::PictureManager(const DeviceData* pDevice)
: _pDevice(pDevice)
{
    _pCamera		= NULL;
    _pWallpaper		= NULL;
    _pScreenShot	= NULL;
	_pAllPictures	= new PictureLibrary();
}

PictureManager::~PictureManager(void)
{
	Release();
}

void PictureManager::Release() 
{
	RELEASE_VECTOR(_PictureGroups);

	SAFE_DELETE(_pCamera);
	SAFE_DELETE(_pWallpaper);
	SAFE_DELETE(_pScreenShot);

	SAFE_DELETE(_pAllPictures);
}

bool PictureManager::ContainsInGroup( int pictureId )
{
	// _PictureGroups��һ��Ϊ����ͼ��ͼƬ����, ���Դӵڶ�����ʼ����
    for (size_t i=1; i<_PictureGroups.size();i++)
    {
        RETURN_TRUE_IF( _PictureGroups[i]->Contains(pictureId) );
    }
    return false;
}


PictureGroup* PictureManager::FindGroup( string id )
{
    for (size_t i = 0; i < _PictureGroups.size(); i++)
    {
        RETURN_VALUE_IF(_PictureGroups[i]->_id == id, _PictureGroups[i])
    }

    return NULL;
}

void PictureManager::GetAllPictures( vector<PictureData*>* pDeque ) const
{
	_pAllPictures->GetAllPictures(pDeque);
}

PictureFolder* PictureManager::CreateWallpaper( wstring path )
{
    if ( ! _pWallpaper )
        _pWallpaper = new PictureFolder(_pDevice, path, PICTURE_TYPE_WALLPAPER);
    return _pWallpaper;
}

PictureFolder* PictureManager::CreateCamera( wstring path )
{
    if ( ! _pCamera )
        _pCamera = new PictureFolder(_pDevice, path, PICTURE_TYPE_PHOTO);
    return _pCamera;
}

PictureFolder* PictureManager::CreateScreenshot( wstring path )
{
    if ( ! _pScreenShot )
        _pScreenShot = new PictureFolder(_pDevice, path, PICTURE_TYPE_SCREENSHOT);
    return _pScreenShot;
}

BaseGroup* PictureManager::GetBaseGroup( string id )
{
    RETURN_VALUE_IF( _pCamera && _pCamera->GetId() == id, _pCamera);
    RETURN_VALUE_IF( _pWallpaper && _pWallpaper->GetId() == id, _pWallpaper);
    RETURN_VALUE_IF( _pScreenShot && _pScreenShot->GetId() == id, _pScreenShot);

    for (size_t i = 0; i < _PictureGroups.size(); i++)
    {
        RETURN_VALUE_IF(_PictureGroups[i]->_id == id, _PictureGroups[i]);
    }

    return NULL;
}

bool PictureManager::AddPictureToGroup( wstring localPath, PICTURE_TYPE nType )
{
	PictureFolder* pFolder;
	if ( ( nType == PICTURE_TYPE_SCREENSHOT && _pDevice->GetDevInfo()->_connectType == Android )
		 || nType == PICTURE_TYPE_PHOTO )			// ��ƽ̨���ݻ�ԭͼƬ�޸�(ios�н�����android�޽�����ָ��)
		pFolder = _pCamera;
	else if (nType == PICTURE_TYPE_WALLPAPER)
		pFolder = _pWallpaper;
	else if (nType == PICTURE_TYPE_SCREENSHOT)
		pFolder = _pScreenShot;
	else
		return false;

	vector<wstring> vPicPaths;
	vPicPaths.push_back(localPath);

	return AddPictureToGroup(&vPicPaths, pFolder->GetId());
}

bool PictureManager::MakeThumbnail(std::wstring picPath, int& width, int& height, std::wstring thmPcPath, bool isIos)
{
	RETURN_FALSE_IF (picPath.empty()||thmPcPath.empty());

	Image* pImage = Gdiplus::Image::FromFile(picPath.c_str());
	RETURN_FALSE_IF ( !pImage );

	double ratio = min( double(pImage->GetWidth ())/width, double(pImage->GetHeight())/height);

	//�½�һ��bmpͼƬ,
	Bitmap* bitmap = new Bitmap(width, height, PixelFormat16bppRGB565);
	Graphics g(bitmap);

	g.Clear(Color::Transparent);
	g.SetCompositingQuality(CompositingQualityHighQuality);
	g.SetInterpolationMode(InterpolationModeHighQuality);   // ���ò�ֵ��ʽ
	g.SetSmoothingMode(SmoothingModeHighQuality);

	if ( isIos )
	{
		width = int(pImage->GetWidth() * ratio);
		height= int(pImage->GetHeight()* ratio);

		g.DrawImage(pImage, Rect(0, 0, width, height), 0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel);
		DUMP_IMAGE(bitmap, thmPcPath.c_str());
	}
	else
	{
		if((int)pImage->GetWidth() > width || (int)pImage->GetHeight() > height)
		{
			int x = (int)(pImage->GetWidth()-width*ratio)/2;
			int y = (int)(pImage->GetHeight()-height*ratio)/2;

			//��ָ��λ�ò��Ұ�ָ����С����ԭͼƬ��ָ������
			g.DrawImage(pImage, Rect(0, 0, width, height),
				x, y, int(width*ratio), int(height*ratio), UnitPixel);

			DUMP_IMAGE(bitmap, thmPcPath.c_str());
		}
		else
		{
            CFileOperation::CopyFile(picPath, thmPcPath, false);
			width	= pImage->GetWidth();
			height  = pImage->GetHeight();
		}
	}

	SAFE_DELETE(bitmap);
	delete pImage;
	return true;
}

int ND91Assistant::PictureManager::GetPhotoCount()
{
    return 0;
}




#endif
