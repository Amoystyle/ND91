#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    class DeviceInfo;

    // ͼƬ�����Ŀ¼
	class IosPicturePath
	{ 
	public:
        IosPicturePath(const wstring workFolder, const DeviceInfo* pInfo);

		wstring GetIosMediaPath() const;

        wstring GetIosRingWallerSet() const;      // ���ػ���������ֽ�����ļ�		(/var/mobile/Library/Preferences/RingWallerSetting.ini)

        // ����ͼ����Ŀ¼ (%TEMP%\\Picture\\) 
        wstring Home() const;		            

#pragma region ͼƬ����ģ�鱾����ʱĿ¼

		wstring GetLocaliPhotoPath      () const; // ����ͼ��ͼƬ��ʱĿ¼				(%TEMP%\\Picture\\iPhoto\\) 
		wstring GetLocalPicPath         () const; // ����ͼ��ͼƬԭͼ��ʱ����Ŀ¼	(%TEMP%\\Picture\\iPhoto\\Cache\\)
		wstring GetLocalThumbnailPath   () const; // ����ͼ��ͼƬ����ͼ��ʱ����Ŀ¼	(%TEMP%\\Picture\\iPhoto\\ThumbnailCache\\)
		wstring GetLocalThumbsPath      () const; // ����ͼ��ͼƬ���ݿ���ʱĿ¼		(%TEMP%\\Picture\\iPhoto\\Thumbs\\)	
		wstring GetLocalPhotoDatabase   () const; // ����Photo Database���ݿ�·��		(%TEMP%\\Picture\\iPhoto\\Photo Database)
		wstring GetLocalMediaPath       () const; // ������Ƭ��ֽ����������Ŀ¼	(%TEMP%\\Picture\\Media\\)
		wstring GetLocalAlbumPath       () const; // ������Ƭ����ͼƬ���Ŀ¼		(%TEMP%\\Picture\\Media\\Album\\)
		wstring GetLocal100APPLEPath    () const; // �������ͼƬ����Ŀ¼				(%TEMP%\\Picture\\Media\\Album\\100APPLE)
		wstring GetLocalWallPaperPath   () const; // ���ر�ֽ����·��					(%TEMP%\\Picture\\ImagePreview)
		wstring GetLocalSqlitePath      () const; // ���ػ������ͼƬ��sqlite���ݿ�	(%TEMP%\\Picture\\Photos.sqlite)
		wstring GetLocalAuxSqlitePath   () const; // ���ؼ�¼ͼƬGPS��sqlite���ݿ�	(%TEMP%\\Picture\\PhotosAux.sqlite)
		wstring GetLocalShmSqlitePath   () const; // ���ػ����״̬�µ�sqlite�ļ�   	(%TEMP%\\Picture\\Photos.sqlite-shm)
		wstring GetLocalWalSqlitePath   () const; // ���ػ����״̬�µ�sqlite�ļ�   	(%TEMP%\\Picture\\Photos.sqlite-wal)
		wstring GetLocalN82APPlist      () const; // ���ػ����ֽ���õ�N82AP.plist	(%TEMP%\\Picture\\N82AP.plist)
		wstring GetLocalN88APPlist      () const; // ���ػ����ֽ���õ�N88AP.plist	(%TEMP%\\Picture\\N88AP.plist)
		wstring GetLocalBackground      () const; // ���ػ����趨�ı�ֽ��ʱĿ¼		(%TEMP%\\Picture\\Backgrounds\\)
		wstring GetLocalLockBack        () const; // ���ػ������������Ļ��ֽ		(%TEMP%\\Picture\\Backgrounds\\LockBackground.jpg)
		wstring GetLocalHomeBack        () const; // ���ػ�������Ļ��ֽ			   	(%TEMP%\\Picture\\Backgrounds\\HomeBackground.jpg)
		wstring GetLocalLockThumbBack   () const; // ���ػ��������Ļ��ֽ����ͼ		(%TEMP%\\Picture\\Backgrounds\\LockBackgroundThumbnail.jpg)
		wstring GetLocalHomeThumbBack   () const; // ���ػ�������Ļ��ֽ����ͼ		(%TEMP%\\Picture\\Backgrounds\\HomeBackgroundThumbnail.jpg)
		wstring GetLocalLockCpbitmapPath() const; // ���ػ���421���ϰ汾������ֽ		(%TEMP%\\Picture\\Backgrounds\\LockBackground.cpbitmap)
		wstring GetLocalHomeCpbitmapPath() const; // ���ػ���421���ϰ汾������ֽ		(%TEMP%\\Picture\\Backgrounds\\HomeBackground.cpbitmap)
		wstring GetLocalTempCpbitmapPath() const; // ���ػ���421���ϰ�Temp��ֽ		(%TEMP%\\Picture\\Temp.cpbitmap)
		wstring GetLocalRingWallerSet   () const; // ���ػ���������ֽ�����ļ�		(Local Settings\\Temp\\RingWallerSetting.ini)
		wstring GetLocalAlbumThumbsPath () const; // ������Ƭ����ͼƬ���Ŀ¼		(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal32IthmbPath		() const; // ���� 32x32.ithmb�ļ�����Ŀ¼	    (%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal79IthmbPath		() const; // ���� 79x79.ithmb�ļ�����Ŀ¼	    (%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal120IthmbPath    () const; // ���� 120x120.ithmb�ļ�����Ŀ¼	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal158IthmbPath    () const; // ���� 158x158.ithmb�ļ�����Ŀ¼	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocal166IthmbPath    () const; // ���� 166x166.ithmb�ļ�����Ŀ¼	(%TEMP%\\Picture\\Media\\PhotoData\\)
		wstring GetLocalSpringboardPlist() const; // ios�Ƿ��Ѿ����������			(%TEMP%\\Picture\\com.apple.SpringBoard.plist)
		wstring GetLocalIthmbConfigPath () const; // ����ios5����ͼ�����ļ�	    	(%TEMP%\\Picture\\iPhoto\\Thumbs\\thumbnailConfiguration)

#pragma endregion

#pragma region  ͼƬ����ģ�鱾�ֻ�Ŀ¼ 

		wstring GetIosAlbumPath         () const; // �ֻ���ᣬ��ͼ���Ŀ¼				(/private/var/mobile/Media/DCIM/100APPLE)
		wstring GetIosAlbumThumbPath    () const; // �ֻ���ᣬ��ͼ����ͼ���Ŀ¼		(/private/var/mobile/Media/DCIM/100APPLE/.MISC)
		wstring GetIos5VideoThumbPath   () const; // Ios5��Ƶ��������ͼ���Ŀ¼			(/private/var/mobile/Media/PhotoData/Metadata/DCIM/100APPLE)
		wstring GetIosPhotosPath        () const; // �ֻ�ͼ�����ݿ��Ÿ�Ŀ¼			(/private/var/mobile/Media/Photos)
		wstring GetIosThumbsPath        () const; // �ֻ�ͼ��ͼƬ���ݿ���Ŀ¼			(/private/var/mobile/Media/Photos/Thumbs)
		wstring GetIosPhotoDatabasePath () const; // �ֻ�ͼ��ͼƬ���ݿ���Ŀ¼			(/private/var/mobile/Media/Photos/Photo Database)
		wstring GetIosWallPaperPath     () const; // �ֻ���ֽ·��					    (/Library/Wallpaper)
		wstring GetIosSqlitePath        () const; // �ֻ�����ͼƬ��sqlite���ݿ�			(/private/var/mobile/Media/PhotoData/Photos.sqlite)
		wstring GetIosAuxSqlitePath     () const; // �ֻ���¼ͼƬGPS��sqlite ���ݿ�   	(/private/var/mobile/Media/PhotoData/PhotosAux.sqlite)
		wstring GetIosSqliteAsidePath   () const; // �ֻ�����ͼƬ��sqlite.aside���ݿ� 	(/private/var/mobile/Media/PhotoData/Photos.sqlite.aside)
		wstring GetIosShmSqlitePath     () const; // �ֻ���Ƭ��״̬�µ�sqlite�ļ�   	(/private/var/mobile/Media/PhotoData/Photos.sqlite-shm)
		wstring GetIosWalSqlitePath     () const; // �ֻ���Ƭ��״̬�µ�sqlite�ļ�   	(/private/var/mobile/Media/PhotoData/Photos.sqlite-wal)
		wstring GetIosN82APPlist        () const; // �ֻ������ֽ���õ�N82AP.plist    	(/System/Library/CoreServices/SpringBoard.app/N82AP.plist)
		wstring GetIosN88APPlist        () const; // �ֻ������ֽ���õ�N88AP.plist    	(/System/Library/CoreServices/SpringBoard.app/N88AP.plist)
		wstring GetIosLockBack          () const; // �ֻ�����������Ļ��ֽ            	(/private/var/mobile/Library/SpringBoard/LockBackground.jpg)
		wstring GetIosHomeBack          () const; // �ֻ�����Ļ��ֽ                  	(/private/var/mobile/Library/SpringBoard/HomeBackground.jpg)
		wstring GetIosLockThumbBack     () const; // �ֻ�������Ļ��ֽ����ͼ          	(/private/var/mobile/Library/SpringBoard/LockBackgroundThumbnail.jpg)
		wstring GetIosHomeThumbBack     () const; // �ֻ�����Ļ��ֽ����ͼ			    (/private/var/mobile/Library/SpringBoard/HomeBackgroundThumbnail.jpg)
		wstring GetIosLockPortraitPath  () const; // �ֻ��Զ������ļ�				    (/private/var/mobile/Library/SpringBoard/LockBackgroundPortrait.jpg)
		wstring GetIosHomePortraitPath  () const; // �ֻ��Զ������ļ�				    (/private/var/mobile/Library/SpringBoard/HomeBackgroundPortrait.jpg)
		wstring GetIosLockCpbitmapPath  () const; // �ֻ�421���ϰ汾������ֽ		    (/private/var/mobile/Library/SpringBoard/LockBackground.cpbitmap)
		wstring GetIosHomeCpbitmapPath  () const; // �ֻ�421���ϰ汾������ֽ		    (/private/var/mobile/Library/SpringBoard/HomeBackground.cpbitmap)
		wstring GetIosAlbumThumbsPath   () const; // �ֻ���Ŀ¼							(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos32IthmbPath		() const; // �ֻ���32x32.ithmb�ļ�	    		(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos79IthmbPath		() const; // �ֻ���79x79.ithmb�ļ�	    		(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos120IthmbPath    	() const; // �ֻ���120x120.ithmb�ļ�	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos158IthmbPath    	() const; // �ֻ���158x158.ithmb�ļ�	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
		wstring GetIos166IthmbPath    	() const; // �ֻ���166x166.ithmb�ļ�	    	(/private/var/mobile/Media/PhotoData/Thumbnails)
        wstring GetIpodWallpaperPath    () const; // iPod��ֽĿ¼						(/Library/Wallpaper/iPod/)
		wstring GetIos421CacheFile		() const; // IOS4.2.1��Ჿ�ֻ����ļ�			(/private/var/mobile/Media/PhotoData/com.apple.photos.caches_metadata.plist)
		wstring GetIpad421CachePath		() const; // iPad4.2.1����Ŀ¼					(/private/var/mobile/Media/PhotoData/Caches/)
		wstring GetIosPhotoPrefsPath	() const; // iTunes��ͼƬĿ¼��¼				(/private/var/root/Media/iTunes_Control/iTunes/PhotosFolderPrefs)
		wstring GetIosSpringboardPlist	() const; // ios�Ƿ�֧�����������				(/System/Library/LaunchDaemons/com.apple.SpringBoard.plist)
		wstring GetIosIthmbConfigPath   () const; // ios5�ֻ�������ͼ�����ļ�	    	(/private/var/mobile/Media/PhotoData/Thumbnails/thumbnailConfiguration)
		wstring GetIosDcimCacheFile		() const; // ios5�ֻ��˻����ļ�                 (/private/var/mobile/Media/DCIM/Caches/StackedImages)
		wstring GetIos5GalleryPath		() const; // ios5�ֻ���ͼ��ԭͼ���·��			(/private/var/mobile/Media/PhotoData/Sync/100SYNCD)

#pragma endregion

    private:

        const wstring		_workFolder;	// ��ʱ����Ŀ¼
        const DeviceInfo*	_pInfo;			// �豸��Ϣ
		int					_nVersion;		// ϵͳ�汾��
    };
}
