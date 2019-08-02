#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    class DeviceInfo;

    // 图片管理的目录
	class IosPicturePath
	{ 
	public:
        IosPicturePath(const wstring workFolder, const DeviceInfo* pInfo);

		wstring GetIosMediaPath() const;

        wstring GetIosRingWallerSet() const;      // 本地缓存铃声壁纸配置文件		(/var/mobile/Library/Preferences/RingWallerSetting.ini)

        // 本地图库总目录 (%TEMP%\\Picture\\) 
        wstring Home() const;		            

#pragma region 图片管理模块本地临时目录

		wstring GetLocaliPhotoPath      () const; // 本地图库图片临时目录				(%TEMP%\\Picture\\iPhoto\\) 
		wstring GetLocalPicPath         () const; // 本地图库图片原图临时缓存目录	(%TEMP%\\Picture\\iPhoto\\Cache\\)
		wstring GetLocalThumbnailPath   () const; // 本地图库图片缩略图临时缓存目录	(%TEMP%\\Picture\\iPhoto\\ThumbnailCache\\)
		wstring GetLocalThumbsPath      () const; // 本地图库图片数据库临时目录		(%TEMP%\\Picture\\iPhoto\\Thumbs\\)	
		wstring GetLocalPhotoDatabase   () const; // 本地Photo Database数据库路径		(%TEMP%\\Picture\\iPhoto\\Photo Database)
		wstring GetLocalMediaPath       () const; // 本地照片壁纸等特殊分类根目录	(%TEMP%\\Picture\\Media\\)
		wstring GetLocalAlbumPath       () const; // 本地照片缓存图片存放目录		(%TEMP%\\Picture\\Media\\Album\\)
		wstring GetLocal100APPLEPath    () const; // 本地相册图片缓存目录				(%TEMP%\\Picture\\Media\\Album\\100APPLE)
		wstring GetLocalWallPaperPath   () const; // 本地壁纸缓存路径					(%TEMP%\\Picture\\ImagePreview)
		wstring GetLocalSqlitePath      () const; // 本地缓存管理图片的sqlite数据库	(%TEMP%\\Picture\\Photos.sqlite)
		wstring GetLocalAuxSqlitePath   () const; // 本地记录图片GPS的sqlite数据库	(%TEMP%\\Picture\\PhotosAux.sqlite)
		wstring GetLocalShmSqlitePath   () const; // 本地缓存打开状态下的sqlite文件   	(%TEMP%\\Picture\\Photos.sqlite-shm)
		wstring GetLocalWalSqlitePath   () const; // 本地缓存打开状态下的sqlite文件   	(%TEMP%\\Picture\\Photos.sqlite-wal)
		wstring GetLocalN82APPlist      () const; // 本地缓存壁纸设置的N82AP.plist	(%TEMP%\\Picture\\N82AP.plist)
		wstring GetLocalN88APPlist      () const; // 本地缓存壁纸设置的N88AP.plist	(%TEMP%\\Picture\\N88AP.plist)
		wstring GetLocalBackground      () const; // 本地缓存设定的壁纸临时目录		(%TEMP%\\Picture\\Backgrounds\\)
		wstring GetLocalLockBack        () const; // 本地缓存待机锁定屏幕壁纸		(%TEMP%\\Picture\\Backgrounds\\LockBackground.jpg)
		wstring GetLocalHomeBack        () const; // 本地缓存主屏幕壁纸			   	(%TEMP%\\Picture\\Backgrounds\\HomeBackground.jpg)
		wstring GetLocalLockThumbBack   () const; // 本地缓存待机屏幕壁纸缩略图		(%TEMP%\\Picture\\Backgrounds\\LockBackgroundThumbnail.jpg)
		wstring GetLocalHomeThumbBack   () const; // 本地缓存主屏幕壁纸缩略图		(%TEMP%\\Picture\\Backgrounds\\HomeBackgroundThumbnail.jpg)
		wstring GetLocalLockCpbitmapPath() const; // 本地缓存421以上版本锁屏壁纸		(%TEMP%\\Picture\\Backgrounds\\LockBackground.cpbitmap)
		wstring GetLocalHomeCpbitmapPath() const; // 本地缓存421以上版本主屏壁纸		(%TEMP%\\Picture\\Backgrounds\\HomeBackground.cpbitmap)
		wstring GetLocalTempCpbitmapPath() const; // 本地缓存421以上版Temp壁纸		(%TEMP%\\Picture\\Temp.cpbitmap)
		wstring GetLocalRingWallerSet   () const; // 本地缓存铃声壁纸配置文件		(Local Settings\\Temp\\RingWallerSetting.ini)
		wstring GetLocalAlbumThumbsPath () const; // 本地照片缓存图片存放目录		(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal32IthmbPath		() const; // 本地 32x32.ithmb文件缓存目录	    (%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal79IthmbPath		() const; // 本地 79x79.ithmb文件缓存目录	    (%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal120IthmbPath    () const; // 本地 120x120.ithmb文件缓存目录	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal158IthmbPath    () const; // 本地 158x158.ithmb文件缓存目录	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal166IthmbPath    () const; // 本地 166x166.ithmb文件缓存目录	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocalSpringboardPlist() const; // ios是否已经锁主屏标记			(%TEMP%\\Picture\\com.apple.SpringBoard.plist)
		wstring GetLocalIthmbConfigPath () const; // 本地ios5缩略图配置文件	    	(%TEMP%\\Picture\\iPhoto\\Thumbs\\thumbnailConfiguration)

#pragma endregion

#pragma region  图片管理模块本手机目录 

		wstring GetIosAlbumPath         () const; // 手机相册，截图存放目录				(/private/var/mobile/Media/DCIM/100APPLE)
		wstring GetIosAlbumThumbPath    () const; // 手机相册，截图缩略图存放目录		(/private/var/mobile/Media/DCIM/100APPLE/.MISC)
		wstring GetIos5VideoThumbPath   () const; // Ios5视频封面缩略图存放目录			(/private/var/mobile/Media/PhotoData/Metadata/DCIM/100APPLE)
		wstring GetIosPhotosPath        () const; // 手机图库数据库存放根目录			(/private/var/mobile/Media/Photos)
		wstring GetIosThumbsPath        () const; // 手机图库图片数据库存放目录			(/private/var/mobile/Media/Photos/Thumbs)
		wstring GetIosPhotoDatabasePath () const; // 手机图库图片数据库存放目录			(/private/var/mobile/Media/Photos/Photo Database)
		wstring GetIosWallPaperPath     () const; // 手机壁纸路径					    (/Library/Wallpaper)
		wstring GetIosSqlitePath        () const; // 手机管理图片的sqlite数据库			(/private/var/mobile/Media/PhotoData/Photos.sqlite)
		wstring GetIosAuxSqlitePath     () const; // 手机记录图片GPS的sqlite 数据库   	(/private/var/mobile/Media/PhotoData/PhotosAux.sqlite)
		wstring GetIosSqliteAsidePath   () const; // 手机管理图片的sqlite.aside数据库 	(/private/var/mobile/Media/PhotoData/Photos.sqlite.aside)
		wstring GetIosShmSqlitePath     () const; // 手机照片打开状态下的sqlite文件   	(/private/var/mobile/Media/PhotoData/Photos.sqlite-shm)
		wstring GetIosWalSqlitePath     () const; // 手机照片打开状态下的sqlite文件   	(/private/var/mobile/Media/PhotoData/Photos.sqlite-wal)
		wstring GetIosN82APPlist        () const; // 手机管理壁纸设置的N82AP.plist    	(/System/Library/CoreServices/SpringBoard.app/N82AP.plist)
		wstring GetIosN88APPlist        () const; // 手机管理壁纸设置的N88AP.plist    	(/System/Library/CoreServices/SpringBoard.app/N88AP.plist)
		wstring GetIosLockBack          () const; // 手机待机锁定屏幕壁纸            	(/private/var/mobile/Library/SpringBoard/LockBackground.jpg)
		wstring GetIosHomeBack          () const; // 手机主屏幕壁纸                  	(/private/var/mobile/Library/SpringBoard/HomeBackground.jpg)
		wstring GetIosLockThumbBack     () const; // 手机待机屏幕壁纸缩略图          	(/private/var/mobile/Library/SpringBoard/LockBackgroundThumbnail.jpg)
		wstring GetIosHomeThumbBack     () const; // 手机主屏幕壁纸缩略图			    (/private/var/mobile/Library/SpringBoard/HomeBackgroundThumbnail.jpg)
		wstring GetIosLockPortraitPath  () const; // 手机自动生成文件				    (/private/var/mobile/Library/SpringBoard/LockBackgroundPortrait.jpg)
		wstring GetIosHomePortraitPath  () const; // 手机自动生成文件				    (/private/var/mobile/Library/SpringBoard/HomeBackgroundPortrait.jpg)
		wstring GetIosLockCpbitmapPath  () const; // 手机421以上版本锁屏壁纸		    (/private/var/mobile/Library/SpringBoard/LockBackground.cpbitmap)
		wstring GetIosHomeCpbitmapPath  () const; // 手机421以上版本主屏壁纸		    (/private/var/mobile/Library/SpringBoard/HomeBackground.cpbitmap)
		wstring GetIosAlbumThumbsPath   () const; // 手机端目录							(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos32IthmbPath		() const; // 手机端32x32.ithmb文件	    		(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos79IthmbPath		() const; // 手机端79x79.ithmb文件	    		(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos120IthmbPath    	() const; // 手机端120x120.ithmb文件	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos158IthmbPath    	() const; // 手机端158x158.ithmb文件	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos166IthmbPath    	() const; // 手机端166x166.ithmb文件	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
        wstring GetIpodWallpaperPath    () const; // iPod壁纸目录						(/Library/Wallpaper/iPod/)
		wstring GetIos421CacheFile		() const; // IOS4.2.1相册部分缓存文件			(/private/var/mobile/Media/PhotoData/com.apple.photos.caches_metadata.plist)
		wstring GetIpad421CachePath		() const; // iPad4.2.1缓存目录					(/private/var/mobile/Media/PhotoData/Caches/)
		wstring GetIosPhotoPrefsPath	() const; // iTunes的图片目录记录				(/private/var/root/Media/iTunes_Control/iTunes/PhotosFolderPrefs)
		wstring GetIosSpringboardPlist	() const; // ios是否支持锁主屏标记				(/System/Library/LaunchDaemons/com.apple.SpringBoard.plist)
		wstring GetIosIthmbConfigPath   () const; // ios5手机端缩略图配置文件	    	(/private/var/mobile/Media/PhotoData/Thumbnails/thumbnailConfiguration)
		wstring GetIosDcimCacheFile		() const; // ios5手机端缓存文件                 (/private/var/mobile/Media/DCIM/Caches/StackedImages)
		wstring GetIos5GalleryPath		() const; // ios5手机端图库原图存放路径			(/private/var/mobile/Media/PhotoData/Sync/100SYNCD)

#pragma endregion

    private:

        const wstring		_workFolder;	// 临时工作目录
        const DeviceInfo*	_pInfo;			// 设备信息
		int					_nVersion;		// 系统版本号
    };
}
