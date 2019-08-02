#include "stdafx.h"

#include "IosPicturePath.h"
#include "Device/IosPublicFunc.h"
#include "Common/Path.h"
#include "Core/DeviceInfo.h"

// #define TEMPPATH		L"Picture\\"
// #define TEMPIPHOTO		L"Picture\\iPhoto\\"
// #define TEMPMEDIA		L"Picture\\Media\\"
// #define TEMPBACKGROUNDS	L"Picture\\Backgrounds\\"

#define TEMPPATH		L"Picture" + WSTRING_PATH_SEPARATER
#define TEMPIPHOTO		L"Picture" + WSTRING_PATH_SEPARATER + L"iPhoto" + WSTRING_PATH_SEPARATER
#define TEMPMEDIA		L"Picture" + WSTRING_PATH_SEPARATER + L"Media" + WSTRING_PATH_SEPARATER
#define TEMPBACKGROUNDS	L"Picture" + WSTRING_PATH_SEPARATER + L"Backgrounds" + WSTRING_PATH_SEPARATER

#define IOSDCIM			L"/DCIM/"
#define IOSPHOTOS		L"/Photos/"
#define IOSPHOTODATA	L"/PhotoData/"
#define IOSLIBRARY		wstring(L"/private/var/mobile/Library/")

IosPicturePath::IosPicturePath(const wstring workFolder,
                               const DeviceInfo* pInfo)
: _pInfo(pInfo)
, _workFolder(workFolder)
{
	_nVersion = IosPublicFunc::GetVerNum(_pInfo->_deviceProductVersion);
}

wstring IosPicturePath::GetIosRingWallerSet() const
{
    return L"/private/var/mobile/Library/Preferences/RingWallerSetting.ini";
}

wstring IosPicturePath::Home() const
{	
    return _workFolder + TEMPPATH;
}

wstring IosPicturePath::GetLocaliPhotoPath() const
{	
	return _workFolder + TEMPIPHOTO;
}

wstring IosPicturePath::GetLocalPicPath() const
{	
	//return _workFolder + TEMPIPHOTO + L"Cache\\";
    return _workFolder + TEMPIPHOTO + L"Cache" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocalThumbnailPath() const
{
	//return _workFolder + TEMPIPHOTO + L"ThumbnailCache\\";
    return _workFolder + TEMPIPHOTO + L"ThumbnailCache" + WSTRING_PATH_SEPARATER;
}
wstring IosPicturePath::GetLocalThumbsPath() const
{	
	//return _workFolder + TEMPIPHOTO + L"Thumbs\\";
    return _workFolder + TEMPIPHOTO + L"Thumbs" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocalPhotoDatabase() const
{	
	return _workFolder + TEMPIPHOTO + L"Photo Database";
}

wstring IosPicturePath::GetLocalMediaPath() const
{	
	return _workFolder + TEMPMEDIA;
}

wstring IosPicturePath::GetLocalAlbumPath() const
{	
	//return _workFolder + TEMPMEDIA  + L"Album\\";
    return _workFolder + TEMPMEDIA  + L"Album" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocal100APPLEPath() const
{		
	//return _workFolder + TEMPMEDIA  + L"Album\\100APPLE\\";
    return _workFolder + TEMPMEDIA  + L"Album" + WSTRING_PATH_SEPARATER + L"100APPLE" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocalWallPaperPath() const
{
	//return _workFolder + TEMPPATH + L"WallPapers\\";
    return _workFolder + TEMPPATH + L"WallPapers" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocalSqlitePath() const
{
	return _workFolder + TEMPMEDIA + L"Photos.sqlite";
}

wstring IosPicturePath::GetLocalAuxSqlitePath() const
{
	return _workFolder + TEMPMEDIA + L"PhotosAux.sqlite";
}

wstring IosPicturePath::GetLocalShmSqlitePath() const
{
	return _workFolder + TEMPMEDIA + L"Photos.sqlite-shm";
}

wstring IosPicturePath::GetLocalWalSqlitePath() const
{
	return _workFolder + TEMPMEDIA + L"Photos.sqlite-wal";
}

wstring IosPicturePath::GetLocalN82APPlist() const
{
	return _workFolder + TEMPPATH + L"N82AP.plist";
}

wstring IosPicturePath::GetLocalN88APPlist() const
{
	return _workFolder + TEMPPATH + L"N88AP.plist";
}

wstring IosPicturePath::GetLocalBackground() const
{
	return _workFolder + TEMPBACKGROUNDS;
}

wstring IosPicturePath::GetLocalLockBack() const
{
	return _workFolder + TEMPBACKGROUNDS + L"LockBackground.jpg";
}

wstring IosPicturePath::GetLocalHomeBack() const
{
	return _workFolder + TEMPBACKGROUNDS + L"HomeBackground.jpg";
}

wstring IosPicturePath::GetLocalLockThumbBack() const
{
	return _workFolder + TEMPBACKGROUNDS + L"LockBackgroundThumbnail.jpg";
}

wstring IosPicturePath::GetLocalHomeThumbBack() const
{
	return _workFolder + TEMPBACKGROUNDS + L"HomeBackgroundThumbnail.jpg";
}

wstring IosPicturePath::GetLocalLockCpbitmapPath() const
{
	return _workFolder + TEMPBACKGROUNDS + L"LockBackground.cpbitmap";
}

wstring IosPicturePath::GetLocalHomeCpbitmapPath() const
{
	return _workFolder + TEMPBACKGROUNDS + L"HomeBackground.cpbitmap";
}

wstring IosPicturePath::GetLocalTempCpbitmapPath() const
{
	return _workFolder + TEMPPATH + L"Temp.cpbitmap";
}

wstring IosPicturePath::GetLocalRingWallerSet() const
{
	return _workFolder + L"RingWallerSetting.ini";
}

wstring IosPicturePath::GetLocalAlbumThumbsPath() const
{
	//return GetLocalMediaPath() + L"Thumbnails\\";
    return GetLocalMediaPath() + L"Thumbnails" + WSTRING_PATH_SEPARATER;
}

wstring IosPicturePath::GetLocal32IthmbPath() const
{
	return GetLocalAlbumThumbsPath() + L"32x32.ithmb";
}

wstring IosPicturePath::GetLocal79IthmbPath() const
{
	return GetLocalAlbumThumbsPath() + L"79x79.ithmb";
}

wstring IosPicturePath::GetLocal120IthmbPath() const
{
	return GetLocalAlbumThumbsPath() + L"120x120.ithmb";
}

wstring IosPicturePath::GetLocal158IthmbPath() const
{
	return GetLocalAlbumThumbsPath() + L"158x158.ithmb";
}

wstring IosPicturePath::GetLocal166IthmbPath() const
{
	return GetLocalAlbumThumbsPath() + L"166x166.ithmb";
}

wstring IosPicturePath::GetLocalSpringboardPlist() const
{
	return _workFolder + TEMPPATH + L"SpringBoard.plist";
}

wstring IosPicturePath::GetLocalIthmbConfigPath() const
{
	return GetLocalAlbumThumbsPath() + L"thumbnailConfiguration";
}

//-----------------------------------------   获取图片模块手机端路径	-----------------------------------------------------


wstring IosPicturePath::GetIosAlbumPath() const
{	
    return	GetIosMediaPath() +	IOSDCIM +	L"100APPLE/";
}

wstring IosPicturePath::GetIosAlbumThumbPath() const
{	
	if ( _nVersion < 400 )
		return GetIosMediaPath() +	IOSDCIM +	L"100APPLE/.MISC/";
	else if (_nVersion >= 400 && _nVersion < 500 )
		return GetIosMediaPath() +	IOSPHOTODATA + L"100APPLE/";
	else
		return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/";
}

wstring IosPicturePath::GetIosPhotosPath() const
{	
	return	GetIosMediaPath() +	IOSPHOTOS;
}

wstring IosPicturePath::GetIosThumbsPath() const
{	
	return	GetIosMediaPath() +	IOSPHOTOS	+	L"Thumbs/";
}

wstring IosPicturePath::GetIosPhotoDatabasePath() const
{		
	return	GetIosMediaPath() +	IOSPHOTOS	+	L"Photo Database";
}

wstring IosPicturePath::GetIosWallPaperPath() const
{	
	wstring devicetype = _pInfo->_deviceProductType;

	if (_nVersion<400)
	{
		return	L"/Library/Wallpaper/";
	} 
	else
	{
		if (_pInfo->IsPad())
		{
			//4.0固件开始iPad变更壁纸相关路径
			return L"/Library/Wallpaper/iPad/";
		}
		//4.0固件开始iPhone变更壁纸相关路径
		return L"/Library/Wallpaper/iPhone/";
	}
	//!!!iTouch 4.2.1即有iPhone,也有iPod两个文件夹(不同团队做的?)
}


wstring IosPicturePath::GetIosSqlitePath() const
{	
	return	GetIosMediaPath() +	IOSPHOTODATA	+	L"Photos.sqlite";
}

wstring IosPicturePath::GetIosShmSqlitePath() const
{
	return	GetIosMediaPath() +	IOSPHOTODATA	+	L"Photos.sqlite-shm";
}

wstring IosPicturePath::GetIosWalSqlitePath() const
{
	return	GetIosMediaPath() +	IOSPHOTODATA	+	L"Photos.sqlite-wal";
}

wstring IosPicturePath::GetIosAuxSqlitePath() const
{	
	return	GetIosMediaPath() +	IOSPHOTODATA	+	L"PhotosAux.sqlite";
}

wstring IosPicturePath::GetIosSqliteAsidePath() const
{
	return	GetIosMediaPath() +	IOSPHOTODATA	+	L"Photos.sqlite.aside";
}

wstring IosPicturePath::GetIosN82APPlist() const
{
	return L"/System/Library/CoreServices/SpringBoard.app/N82AP.plist";
}

wstring IosPicturePath::GetIosN88APPlist() const
{
	return L"/System/Library/CoreServices/SpringBoard.app/N88AP.plist";
}

wstring IosPicturePath::GetIosLockBack() const
{
	if (_nVersion<400)
	{
		return IOSLIBRARY	+ L"LockBackground.jpg";
	} 
	else if (_nVersion<421)
	{
		//4.0固件开始变更壁纸相关路径
		return IOSLIBRARY	+	L"SpringBoard/LockBackground.jpg";
	}
	else
		return IOSLIBRARY	+	L"SpringBoard/LockBackground.cpbitmap";
		//4.2.1固件开始变更壁纸相关路径
}

wstring IosPicturePath::GetIosHomeBack() const
{
	if (_nVersion<400)
	{
		//iPad 3.2.1主屏壁纸
		return IOSLIBRARY	+	L"HomeBackground.jpg";
	} 
	else if (_nVersion<421)
	{
		//4.0固件开始变更主屏壁纸相关路径
		return IOSLIBRARY	+	L"SpringBoard/HomeBackground.jpg";
	}
	else
		return IOSLIBRARY	+	L"SpringBoard/HomeBackground.cpbitmap";
		//4.2.1固件开始变更主屏壁纸相关路径
}

wstring IosPicturePath::GetIosLockThumbBack() const
{
	return IOSLIBRARY	+	L"SpringBoard/LockBackgroundThumbnail.jpg";
}

wstring IosPicturePath::GetIosHomeThumbBack() const
{
	return IOSLIBRARY	+	L"SpringBoard/HomeBackgroundThumbnail.jpg";
}

wstring IosPicturePath::GetIosLockPortraitPath() const
{
	return IOSLIBRARY	+	L"SpringBoard/LockBackgroundPortrait.jpg";
}

wstring IosPicturePath::GetIosHomePortraitPath() const
{
	return IOSLIBRARY	+	L"SpringBoard/HomeBackgroundPortrait.jpg";
}

wstring IosPicturePath::GetIosLockCpbitmapPath() const
{
	return IOSLIBRARY	+	L"SpringBoard/LockBackground.cpbitmap";
}

wstring IosPicturePath::GetIosHomeCpbitmapPath() const
{
	return IOSLIBRARY	+	L"SpringBoard/HomeBackground.cpbitmap";
}

wstring IosPicturePath::GetIosMediaPath() const
{
    return _pInfo->_ios_bJailBreaked ? L"/private/var/mobile/Media" : L"";
}

wstring IosPicturePath::GetIosAlbumThumbsPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/";
}

wstring IosPicturePath::GetIos32IthmbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/32x32.ithmb";
}

wstring IosPicturePath::GetIos79IthmbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/79x79.ithmb";
}

wstring IosPicturePath::GetIos120IthmbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/120x120.ithmb";
}

wstring IosPicturePath::GetIos158IthmbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/158x158.ithmb";
}

wstring IosPicturePath::GetIos166IthmbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA + L"Thumbnails/166x166.ithmb";
}

wstring IosPicturePath::GetIpodWallpaperPath() const
{
    return L"/Library/Wallpaper/iPod/";
}

wstring IosPicturePath::GetIos421CacheFile() const
{
	return	GetIosMediaPath() +	IOSPHOTODATA +  L"com.apple.photos.caches_metadata.plist";
}

wstring IosPicturePath::GetIpad421CachePath() const
{
	return	GetIosMediaPath() +	IOSPHOTODATA +  L"Caches/";
}

wstring IosPicturePath::GetIosPhotoPrefsPath() const
{
	return GetIosMediaPath() + L"iTunes_Control/iTunes/PhotosFolderPrefs";
}

wstring IosPicturePath::GetIosSpringboardPlist() const
{
	if ( _nVersion < 400 && !_pInfo->IsPad())
	{
		return L"/System/Library/LaunchDaemons/com.apple.SpringBoard.plist";
	}
	else
	{
		return IOSLIBRARY + L"Preferences/com.apple.springboard.plist";
	}	
}

wstring IosPicturePath::GetIos5VideoThumbPath() const
{
	return GetIosMediaPath() +	IOSPHOTODATA +  L"Metadata/DCIM/100APPLE/";
}

wstring IosPicturePath::GetIosIthmbConfigPath() const
{
	return GetIosAlbumThumbsPath() + L"thumbnailConfiguration";
}

wstring IosPicturePath::GetIosDcimCacheFile() const
{
	return GetIosMediaPath() + L"/DCIM/Caches/StackedImages";
}

wstring IosPicturePath::GetIos5GalleryPath() const
{
	return GetIosMediaPath() + L"/PhotoData/Sync/100SYNCD/";
}
