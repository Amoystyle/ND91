#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosWallpaperOper.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureFolder.h"

#include "Module/Common/IosDB/IthmbDB.h"
#include "Module/System/SystemManager.h"
#include "Common/md5.h"
#include "Common/Path.h"
#include "Common/IosPngConver.h"

#define WALLPAPER_MIN_NUMBER L"300"

IosWallpaperOper::IosWallpaperOper( const DeviceData* pDevice)
: _pDevice(pDevice)
{
	_nVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);

	_pDirCmd	 = NULL; 
	_pFileStream = NULL;

}

IosWallpaperOper::~IosWallpaperOper()
{
	SAFE_DELETE(_pDirCmd);
	SAFE_DELETE(_pFileStream);
}

IIosFileSystem* IosWallpaperOper::GetIosFileIO()
{
	if ( !_pFileStream )
	{
		if (_pDevice->GetDevInfo()->_connectType == USB)
			_pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo());
		else
			_pFileStream = new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
	}
	return _pFileStream;
}

bool IosWallpaperOper::MakeThumbnail( wstring sourceFile, wstring destFile, int width, int height, bool bFill )
{
	Image* pImage = Image::FromFile(sourceFile.c_str());
	RETURN_FALSE_IF(! pImage);

	Image* pThumb = IthmbDB::MakeThumbnail(pImage, bFill, width, height);
	SAFE_DELETE(pImage);

	RETURN_FALSE_IF(! pThumb);
	DUMP_IMAGE(pThumb, destFile.c_str());
	SAFE_DELETE(pThumb);

	return true;
}

DirectoryOperationCommand* IosWallpaperOper::DirCommand()
{
	if ( ! _pDirCmd )
	{
		_pDirCmd = new DirectoryOperationCommand(_pDevice);
	}
	return _pDirCmd;
}

bool IosWallpaperOper::IosDownPictureFile( wstring iosPath, wstring localPath )
{
	RETURN_FALSE_IF ( Transfer_Success != GetIosFileIO()->Ios_DownFromPhone(iosPath,localPath) );

#ifdef WIN32
	//转换PNG图片为可用格式
	if (CStrOperation::endsWithW(CStrOperation::toLowerW(iosPath), L".png"))
		IosPngConver::ConverApplePngToNormal(localPath.c_str(), localPath.c_str());
#endif
	return true;
}

wstring IosWallpaperOper::GetWallpaperNumber(wstring deviceFolder)
{
    wstring suffix(PICPNG);

    DirData dir(_pDevice);
    dir._name = deviceFolder;
    deque<NodeData*> childs = dir.GetChildrens();
    RETURN_VALUE_IF( childs.empty(), WALLPAPER_MIN_NUMBER);

    // 找到所有的***.png格式的文件名，并将前面的数据保存在set中
    set<int> existFiles;
    const int minNumber = 300; // 用户上传的壁纸序号从300开始
    for (size_t i = 0; i < childs.size(); ++i)
    {
        NodeData* pNode = childs[i];

        // 只判断 这个***.png名字格式的文件
        if ( ! CStrOperation::endsWithW(pNode->_name, suffix) )
        {
            continue;
        }

        wstring numberString = pNode->_name.substr(0, pNode->_name.size() - suffix.size());
        int number = _wtoi(numberString.c_str());
        if ( number >= minNumber )
        {
            existFiles.insert(number);
        }
    }

    int i;
    for(i = minNumber; i < 9999; ++i)
    {
        if (existFiles.find(i) == existFiles.end() )
            break;
    }
    wchar_t buffer[6] = {0};
    swprintf_s(buffer, 6, L"%d", i);
    return buffer;
}

PictureData* IosWallpaperOper::AddWallpaper(std::wstring sourceFile, wstring newFilename, bool need2x)
{
	bool isPad = _pDevice->GetDevInfo()->IsPad();
	if ( isPad )
		need2x = false;

    wstring localFolder = _pDevice->GetIosPicturePath()->GetLocalWallPaperPath();
    wstring remoteFolder = _pDevice->GetIosPicturePath()->GetIosWallPaperPath();

    // 本地和远程文件名(无后缀)
	wstring	nameTag = isPad ? IPADTAG : L"";		// ipad的壁纸名称是xxx_iPad.png
    wstring remotePath = remoteFolder + newFilename + nameTag;
    wstring localPath =  localFolder + newFilename + nameTag;

    // 按固定尺寸生成多个缩略图并上传
	int width = isPad ? IPAD_WALLPAPER_WIDTH : IOS_WALLPAPER_WIDTH;
	int height= isPad ? IPAD_WALLPAPER_HEIGHT: IOS_WALLPAPER_HEIGHT;

    RETURN_NULL_IF( ! MakeThumbnail(sourceFile, localPath + PICPNG,		width,			height));
    RETURN_NULL_IF( ! MakeThumbnail(sourceFile, localPath + THUMBNAIL,IOS_THUMB_WIDTH,	IOS_THUMB_HEIGHT));
    RETURN_NULL_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + PICPNG,       remotePath + PICPNG) != Transfer_Success );
    RETURN_NULL_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + THUMBNAIL,    remotePath + THUMBNAIL) != Transfer_Success );

    if (need2x)
    {
        RETURN_NULL_IF( ! MakeThumbnail(sourceFile, localPath + PNG2X,      width*2,			height*2));
        RETURN_NULL_IF( ! MakeThumbnail(sourceFile, localPath + THUMBNAIL2X,IOS_THUMB_WIDTH*2,	IOS_THUMB_HEIGHT*2));
        RETURN_NULL_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + PNG2X,        remotePath + PNG2X) != Transfer_Success );
        RETURN_NULL_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + THUMBNAIL2X,  remotePath + THUMBNAIL2X) != Transfer_Success );
    }

    // 创建对象
    wstring fileSuffix = need2x ? PNG2X : PICPNG;
    wstring thumbSuffix = need2x ? THUMBNAIL2X : THUMBNAIL;

    PictureData* pPicture = new PictureData(remotePath + fileSuffix, PICTURE_TYPE_WALLPAPER);

    pPicture->_localPath	    = localPath + fileSuffix;
    pPicture->_remotePath	    = remoteFolder + newFilename + fileSuffix;
    pPicture->_thumbLocalPath   = localPath + thumbSuffix;
    pPicture->_thumbRemotePath  = remoteFolder + newFilename + thumbSuffix;

    pPicture->_size = CFileOperation::GetFileSizeW(pPicture->_localPath);

    return pPicture;
}

void IosWallpaperOper::LoadWallpaper4(PictureFolder* pWallpaper)
{
    ReadWallpaperFolder(pWallpaper, pWallpaper->GetPath());

    // iPod的壁纸有2个目录, 再读一次
    if ( _pDevice->GetDevInfo()->IsPod() )
    {
        ReadWallpaperFolder(pWallpaper, _pDevice->GetIosPicturePath()->GetIpodWallpaperPath());
    }
}

void IosWallpaperOper::ReadWallpaperFolder(PictureFolder* pWallpaper, wstring folder)
{
    bool iPad	= _pDevice->GetDevInfo()->IsPad();
    bool has2x	= _pDevice->GetDevInfo()->Is2x();
	bool ios5	= _nVersion > 510 ? true : false;

    wstring localPath = _pDevice->GetIosPicturePath()->GetLocalWallPaperPath();

    int  width = iPad ? IPAD_WALLPAPER_WIDTH : IOS_WALLPAPER_WIDTH;
    int height = iPad ? IPAD_WALLPAPER_HEIGHT: IOS_WALLPAPER_HEIGHT;
    if ( has2x )
    {
        width  *= 2;
        height *= 2;
    }

    DirData wallpaperFolder(_pDevice);
    wallpaperFolder._name = folder;

	std::set<int> numList;
	std::set<int>::iterator it;
    deque<NodeData*> wallpapers = wallpaperFolder.GetChildrens();
    for( size_t i = 0; i < wallpapers.size(); ++i )
    {	
        NodeData* pNode = wallpapers[i];

		if ( pNode->_type != NodeData::FILE || 
			! iswdigit( *pNode->_name.begin() ) ||
			! CStrOperation::endsWithW(pNode->_name, L".png")
			)
			continue;

		int nValue = _wtoi(pNode->_name.c_str());
		it = numList.find(nValue);

		// 如果发现容器已存在该图片，则跳过.
		if ( it != numList.end() )
			continue;

		numList.insert(nValue);
		
		wstring nameWithoutExt;
		int pos = pNode->_name.find(L"iPod");
		if ( folder == _pDevice->GetIosPicturePath()->GetIpodWallpaperPath() && pos > 0 )
		{
			nameWithoutExt = pNode->_name.substr(0, pos+4);
		}
		else if ( iPad )
		{
			int tag = pNode->_name.find(L"iPad");
			nameWithoutExt = pNode->_name.substr(0, tag+4);
		}
		else
		{
			wchar_t picName[12] = {0};
			swprintf_s(picName, 12, L"%d", nValue);
			nameWithoutExt = picName;
		}

        wstring name,thumbName;
		if ( (int)pNode->_name.find(L"@2x~ip") > 0 )
		{
			name		= iPad ? nameWithoutExt + IPADPNG2X : nameWithoutExt + NEWPNG2X;
			thumbName	= iPad ? nameWithoutExt + IPADTHUMBNAIL2X : nameWithoutExt + NEWTHUMBNAIL2X;	
		}
		else if ( (int)pNode->_name.find(L"_iPad~ipad") > 0 ||
				  (int)pNode->_name.find(L"thumbnail~ipad") > 0	)
		{
			name		= nameWithoutExt + L"~ipad.png";
			thumbName	= nameWithoutExt + L".thumbnail~ipad";
		}
		else
		{
			name = has2x ? nameWithoutExt + PNG2X : nameWithoutExt + PICPNG;
			thumbName = has2x ? nameWithoutExt + THUMBNAIL2X : nameWithoutExt + THUMBNAIL;	
		}

        PictureData pic;
        pic.SetType(PICTURE_TYPE_WALLPAPER);

        pic._name = name;
        pic._size = 0; // 大小在下载原图时赋值
        pic._createTime = pNode->_createTime;
        pic._modifyTime = pNode->_modifyTime;

        pic._remotePath = folder + name;
        pic._localPath = localPath + name;
        pic._thumbRemotePath = folder + thumbName;
        pic._thumbLocalPath = localPath + thumbName;

        pic._width = width;
        pic._height = height;
        pic._thumbWidth = IOS_THUMB2_WIDTH;
        pic._thumbHeight = IOS_THUMB2_WIDTH;
		pic.SetCategoryId(WALLPAPERID);

        pWallpaper->InsertPicture(&pic);
    }
	numList.clear();
}

bool IosWallpaperOper::UpdateWallpaper421( PictureData* pWallpaper, wstring sourcePath )
{
	bool isIPad = _pDevice->GetDevInfo()->IsPad();

	wstring localFolder = CFileOperation::GetFileDir(pWallpaper->_localPath);
	wstring remoteFolder = CFileOperation::GetFileDir(pWallpaper->_remotePath);
	wstring picName = CFileOperation::GetFileNameNoExt(pWallpaper->_localPath);
	int pos = picName.find(L"@2x");
	if ( pos > 0)
	{
		picName = picName.substr(0, pos);
	}

	// 本地和远程文件名(无后缀)
	wstring	nameTag = isIPad ? IPADTAG : L"";		// ipad的壁纸名称是xxx_iPad.png
	wstring localPath =  localFolder + picName + nameTag;
	wstring remotePath = remoteFolder + picName + nameTag;
	

	// 按固定尺寸生成多个缩略图并上传
	int width = isIPad ? IPAD_WALLPAPER_WIDTH : IOS_WALLPAPER_WIDTH;
	int height= isIPad ? IPAD_WALLPAPER_HEIGHT: IOS_WALLPAPER_HEIGHT;

	RETURN_FALSE_IF( ! MakeThumbnail(sourcePath, localPath + PICPNG,		width,			height));
	RETURN_FALSE_IF( ! MakeThumbnail(sourcePath, localPath + THUMBNAIL, IOS_THUMB_WIDTH,	IOS_THUMB_HEIGHT));
	RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + PICPNG,      remotePath + PICPNG) != Transfer_Success );
	RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + THUMBNAIL,   remotePath + THUMBNAIL) != Transfer_Success );

	if ( _pDevice->GetDevInfo()->Is2x() ) // 如果是高清屏幕, 还要处理2个文件
	{
		RETURN_FALSE_IF( ! MakeThumbnail(sourcePath, localPath + PNG2X,      width*2,			height*2));
		RETURN_FALSE_IF( ! MakeThumbnail(sourcePath, localPath + THUMBNAIL2X,IOS_THUMB_WIDTH*2,	IOS_THUMB_HEIGHT*2));
		RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + PNG2X,        remotePath + PNG2X) != Transfer_Success );
		RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(localPath + THUMBNAIL2X,  remotePath + THUMBNAIL2X) != Transfer_Success );
	}
	
	return true;
}

bool IosWallpaperOper::DeleteWallpaper421( const PictureData* pWallpaper )
{
	RETURN_FALSE_IF( ! DirCommand()->DeleteFileLink(pWallpaper->_remotePath));
	RETURN_FALSE_IF( ! DirCommand()->DeleteFileLink(pWallpaper->_thumbRemotePath));

	if ( _pDevice->GetDevInfo()->Is2x() || _pDevice->GetDevInfo()->IsPod() )
	{
		wstring picPath = CFileOperation::GetFileDir(pWallpaper->_remotePath);
		wstring picName = CFileOperation::GetFileNameNoExt(pWallpaper->_remotePath);
		int pos = picName.find(L"@2x");
		picName = picName.substr(0,pos);

		RETURN_FALSE_IF( ! DirCommand()->DeleteFileLink(picPath + picName + PICPNG));
		RETURN_FALSE_IF( ! DirCommand()->DeleteFileLink(picPath + picName + THUMBNAIL));
	}
	
	return true;
}

bool IosWallpaperOper::SetBackground421( wstring wallpaper, bool isHome)
{
	bool isIPad = _pDevice->GetDevInfo()->IsPad();

	wstring LocalBack, LocalCpbitmap, IosCpbitmap, LocalThumbBack, IosThumbBack;

	if (isHome)
	{
		LocalBack		= _pDevice->GetIosPicturePath()->GetLocalLockBack();
		LocalCpbitmap	= _pDevice->GetIosPicturePath()->GetLocalLockCpbitmapPath();
		IosCpbitmap		= _pDevice->GetIosPicturePath()->GetIosLockCpbitmapPath();
		LocalThumbBack	= _pDevice->GetIosPicturePath()->GetLocalLockThumbBack();
		IosThumbBack	= _pDevice->GetIosPicturePath()->GetIosLockThumbBack(); 
	}
	else
	{
		LocalBack		= _pDevice->GetIosPicturePath()->GetLocalHomeBack();
		LocalCpbitmap	= _pDevice->GetIosPicturePath()->GetLocalHomeCpbitmapPath();
		IosCpbitmap		= _pDevice->GetIosPicturePath()->GetIosHomeCpbitmapPath();
		LocalThumbBack	= _pDevice->GetIosPicturePath()->GetLocalHomeThumbBack();
		IosThumbBack	= _pDevice->GetIosPicturePath()->GetIosHomeThumbBack(); 
	}

	int weigth	  = 640, heigth	   = 960;
	int thmWeigth = 206, thmHeigth = 308;

	if (isIPad)
	{
		Image* pImg = Gdiplus::Image::FromFile(wallpaper.c_str());
		RETURN_FALSE_IF(pImg==NULL);
		if (pImg->GetWidth()==1024&&pImg->GetHeight()==1024)
		{
			weigth	  = 1024; 
			heigth    = 1024;
		}
		thmWeigth = 102;
		thmHeigth = 154;
	}
	MakeThumbnail(wallpaper, LocalThumbBack, thmWeigth, thmHeigth, true);
	MakeThumbnail(wallpaper, LocalBack, weigth, heigth, true);	
	ConvertPngToCPBitmap(LocalBack);

	RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(LocalCpbitmap, IosCpbitmap) != Transfer_Success );
	RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(LocalThumbBack, IosThumbBack) != Transfer_Success );

	return true;
}

bool IosWallpaperOper::ConvertPngToCPBitmap(wstring localJpg)
{
	Bitmap* img = Gdiplus::Bitmap::FromFile(localJpg.c_str());
	wstring localcpbitmap = localJpg.substr(0,localJpg.length()-4) + L".cpbitmap";

	fstream fcpbitmap;
	fcpbitmap.open(CCodeOperation::UnicodeToGB2312(localcpbitmap).c_str(), ios::binary|ios::out);

	BitmapData* pBmpData = new BitmapData();
	Rect r(0, 0, img->GetWidth(), img->GetHeight());
	img->LockBits(&r, ImageLockModeRead, PixelFormat32bppARGB, pBmpData);
	for( size_t i = 0 ; i < pBmpData->Height; ++i)
	{
		fcpbitmap.write((char*)pBmpData->Scan0 + pBmpData->Stride * i, pBmpData->Width * 4);
	}
	img->UnlockBits(pBmpData);
	SAFE_DELETE(pBmpData);

	char zero[4] = { 0x00, 0x00, 0x00, 0x00 };
	int width = img->GetWidth();
	int height = img->GetHeight();
	unsigned char padding[8] = { 0x01, 0x00, 0x00, 0x00, 0x91, 0x32, 0xA4, 0xCB };
	fcpbitmap.write(zero, 4);
	fcpbitmap.write((char*)&width, 4);
	fcpbitmap.write((char*)&height, 4);
	fcpbitmap.write(zero, 4);
	fcpbitmap.write((char*)padding, 8);
	fcpbitmap.close();

	return true;
}

bool IosWallpaperOper::UploadWallpaperInI(wstring localPath)
{
	string filemd5 = MD5(ifstream(localPath.c_str())).toString();

	wstring RemoteWallIni;
	wstring RingWallIni = _pDevice->GetIosPicturePath()->GetLocalRingWallerSet();
	if (CFileOperation::IsFileExist(RingWallIni.c_str()))
		::DeleteFileW(RingWallIni.c_str());
	
	if (CFileOperation::IsDirExist(RingWallIni.c_str()))
		::DeleteFileW(RingWallIni.c_str());
	

	if ( GetIosFileIO()->Ios_Exists( _pDevice->GetIosPicturePath()->GetIosRingWallerSet() ) )
	{
		RemoteWallIni = _pDevice->GetIosPicturePath()->GetIosRingWallerSet();
	} 
	else if ( GetIosFileIO()->Ios_Exists( _pDevice->GetIosPicturePath()->GetIosRingWallerSet() + L".old" ) )
	{
		RemoteWallIni = _pDevice->GetIosPicturePath()->GetIosRingWallerSet() + L".old";
	}

	if ( Transfer_Success == GetIosFileIO()->Ios_DownFromPhone(RemoteWallIni,RingWallIni))
	{
		char szDaytime[20] = {0};
		string strDayTime;
		time_t t = time(NULL);

		CDealTime dt(t);
		sprintf(szDaytime,"%d-%d-%d %d:%d:%d",
			dt.GetYear(),
			dt.GetMonth(),
			dt.GetDay(),
			8+dt.GetHour(),
			dt.GetMinute(),
			dt.GetSecond());
		strDayTime = szDaytime;

		//::WritePrivateProfileStringW(L"Info",L"WallerHash", CCodeOperation::GB2312ToUnicode(filemd5).c_str(), RingWallIni.c_str());
		//::WritePrivateProfileStringW(L"Info",L"WallerSetTime", strDayTime.c_str(), RingWallIni.c_str());
        write_profile_string("Info", "WallerHash", filemd5.c_str(), CCodeOperation::WstringToString(RingWallIni).c_str());
        write_profile_string("Info", "WallerSetTime", strDayTime.c_str(), CCodeOperation::WstringToString(RingWallIni).c_str());
	}

    RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(RingWallIni, _pDevice->GetIosPicturePath()->GetIosRingWallerSet()) != Transfer_Success );

	DeviceData* pdata = (DeviceData*)_pDevice;
	RETURN_FALSE_IF(!pdata->GetSystemManager()->LogoffDev());

	return true;
}

bool IosWallpaperOper::IsSupportHomeScreen()
{
	bool relust = false;

	wstring LocalN88APplist	= _pDevice->GetIosPicturePath()->GetLocalN88APPlist();
	wstring IosN88APplist	= _pDevice->GetIosPicturePath()->GetIosN88APPlist();
	wstring LocalN82APplist	= _pDevice->GetIosPicturePath()->GetLocalN82APPlist();
	wstring IosN82APplist	= _pDevice->GetIosPicturePath()->GetIosN82APPlist();

	wstring deviceType = _pDevice->GetDevInfo()->_deviceProductType;
	deviceType = deviceType.substr(0,4);

	if (deviceType == L"iPad" || _pDevice->GetDevInfo()->_deviceProductType == L"iPod4,1")
		relust = true;
	else if (_nVersion < 400)
		relust = false;
	else
	{
		if (_pDevice->GetDevInfo()->_deviceProductType == L"iPhone3,1")
		{
			//iPhone4 直接支持
			relust = true;
		}
		else if (_pDevice->GetDevInfo()->_deviceProductType == L"iPhone2,1")
		{
			if (CFileOperation::IsFileExist(LocalN88APplist.c_str()))
				::DeleteFileW(LocalN88APplist.c_str());

			//iPhone3GS 需要判断N88AP.Plist
			if ( Transfer_Success == GetIosFileIO()->Ios_DownFromPhone(IosN88APplist, LocalN88APplist))
			{
				IosPlistConverter::GetXmlFromPlist(LocalN88APplist, LocalN88APplist);
				relust = IsHasHomeScreen(LocalN88APplist);
			}
		}
		else
		{
			if (CFileOperation::IsFileExist(LocalN82APplist.c_str()))
				::DeleteFileW(LocalN82APplist.c_str());

			//iPhone 3g 需要判断N82AP.Plist
			if ( Transfer_Success == GetIosFileIO()->Ios_DownFromPhone(IosN82APplist, LocalN82APplist))
			{
				IosPlistConverter::GetXmlFromPlist(LocalN82APplist, LocalN82APplist);
				relust = IsHasHomeScreen(LocalN82APplist);
			}
		}
	}
	return relust;
}

bool IosWallpaperOper::IsHasHomeScreen(wstring strPlist)
{
	std::string strVal,strname;
	bool bfind = false;

	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(strPlist.c_str()) );

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(strPlist).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	rapidxml::xml_node<char> * root = doc.first_node();						// xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();					// 根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->first_node("dict");			// 子节点最后一个节点

	for (rapidxml::xml_node<char> * listnode = chnode->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		if (bfind)
		{
			break;										// 找到目标，跳出循环，strname为所需值
		}
		if ( strname != "key" )
		{
			continue;									// 非key键直接跳过
		}
		else
		{
			strVal = listnode->value();
			if(strVal == "homescreen-wallpaper")		// 若键名为key，value为homescreen-wallpaper则找到目标
			{
				bfind = true;
				continue;
			}
		}
	}

	return "true" == strname;
}

bool IosWallpaperOper::SetUniqueHomeScreenValue(bool isSame)
{
	bool have = false, needSync = false;

	wstring localPlist = _pDevice->GetIosPicturePath()->GetLocalSpringboardPlist();
	wstring remotePlist = _pDevice->GetIosPicturePath()->GetIosSpringboardPlist();

	std::string strValue = isSame ? "true" : "false";

	if ( CFileOperation::IsFileExist(localPlist.c_str()) )
        CFileOperation::DeleteFile(localPlist);
	
	if ( Transfer_Success == GetIosFileIO()->Ios_DownFromPhone( remotePlist, localPlist ) )
	{
		std::string strname, strVal; 
		
		if ( MyFileOperation::IsIosPlist(localPlist) )
		{
			IosPlistConverter::GetXmlFromPlist(localPlist, localPlist);          //.Plist转XML
		}
		file<> fdoc( CCodeOperation::UnicodeToUTF_8(localPlist).c_str() );
		xml_document<char> doc;
		doc.parse<0>(fdoc.data());

		rapidxml::xml_node<char> * root = doc.first_node();						// xml文件的根节点
		rapidxml::xml_node<char> * scnode = root->first_node();					// 根节点的子节点
		for (rapidxml::xml_node<char> * listnode = scnode->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
		{
			strname = listnode->name();
			strVal = listnode->value();
			if ( "key" == strname && "SBUseUniqueHomeScreenWallpaper" == strVal )
			{
				have = true;
				continue;
			}
			if ( have && strname == strValue)
			{
				break;
			}
			if ( have && strValue != strname )
			{
				rapidxml::xml_node<>* string = doc.allocate_node(node_element, strValue.c_str());
				scnode->insert_node(listnode,string);             //修改键值,先插再删
				scnode->remove_node(listnode);
				needSync = true;
				break;	
			}
		}
		if (!have)
		{
			scnode->append_node(doc.allocate_node(node_element,"key","SBUseUniqueHomeScreenWallpaper"));
			scnode->append_node(doc.allocate_node(node_element, strValue.c_str()));
			needSync = true;
		}

		if (needSync)
		{
			std::string text;  
			rapidxml::print(std::back_inserter(text), doc, 0);
			text = XML_PLIST_Base + text; 
			CFileOperation::WriteBinFile(localPlist, text.c_str(), text.length());  //写入.plist文件

			if ( MyFileOperation::IsIosPlist(localPlist) )
			{
				IosPlistConverter::GetPlistFromXml(localPlist, localPlist);          //.Plist转XML
			}
			RETURN_FALSE_IF( GetIosFileIO()->Ios_CopyToPhone(localPlist, remotePlist) != Transfer_Success );
		}

	}
	return true;
}

#endif
