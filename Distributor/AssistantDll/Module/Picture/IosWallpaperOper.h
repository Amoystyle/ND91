#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{
    class PictureData;
    class PictureFolder;
	class DirectoryOperationCommand;

    // interface: 照片/壁纸/截图操作的不同版本实现
	class IosWallpaperOper
	{
	public:
        IosWallpaperOper(const DeviceData* pDevice);
        virtual ~IosWallpaperOper();

#pragma region 壁纸相关操作

		// Summary  : 判断是否支持设置主屏幕壁纸
		// Returns  : bool
		bool IsSupportHomeScreen();

		// Summary  : 设置壁纸后上传配置文件RingWallpaper.ini
		// Returns  : bool
		bool UploadWallpaperInI(wstring localPath);

		// Summary  : 设置锁主屏标记键值
		// Returns  : bool
		// Parameter: bool isSame		锁屏和主屏是否是同一张图片 
		bool SetUniqueHomeScreenValue(bool isSame);

        // ios壁纸文件名是按数字排列的, 本方法取得下一个数字
        wstring GetWallpaperNumber(wstring deviceFolder);

		// 主要用来设置文件权限
        inline DirectoryOperationCommand* DirCommand();

        // Summary  : 生成缩略图
        // Returns  : 是否成功
        // Parameter: sourceFile    原文件名
        // Parameter: destFile      新文件名
        // Parameter: width         新文件宽
        // Parameter: height        新文件高
        // Parameter: bFill         是否应该充满(即截取原图中间部分生成缩略图)
        bool MakeThumbnail(wstring sourceFile, wstring destFile, int width, int height, bool bFill = true);

        // Summary  : 增加壁纸
        // Returns  : 图片对象, NULL表示失败
        // Parameter: localFile    本地图片文件
        // Parameter: remoteFolder 新的文件名(不含扩展名后缀)
        // Parameter: need2x       是否需要2倍大图(ios4以上需要)
        PictureData* AddWallpaper(std::wstring localFile, wstring newFilename, bool need2x);

        // Summary  : 读取壁纸数据(4以上)
        // Parameter: pWallpaper    壁纸容器类
        void LoadWallpaper4(PictureFolder* pWallpaper);

		// Summary  : 修改壁纸(421以上)
		// Parameter: pWallpaper    图片指针
		bool UpdateWallpaper421(PictureData* pWallpaper, wstring sourcePath);

		// Summary  : 删除壁纸(421以上)
		// Parameter: pWallpaper    图片指针
		bool DeleteWallpaper421( const PictureData* pWallpaper );

		// Summary  : 设置壁纸(421以上)
		// Returns  : bool
		// Parameter: wstring wallpaper		 选中壁纸本地路径
		// Parameter: bool isHome			是否是设置主屏壁纸
		bool SetBackground421( wstring wallpaper, bool isHome );

		// ----  设置壁纸  ----

		// Summary  : 高版本IOS手机生成.cpbitmap壁纸文件
		// Returns  : bool
		bool ConvertPngToCPBitmap(wstring cpBitmap);

		// Summary  : 判断Plist文件中的标记
		// Returns  : bool
		// Parameter: strPlist
		bool IsHasHomeScreen(wstring strPlist);

        // 取照片和截图
        void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

        // Summary  : 生成不带扩展名的文件名
        wstring MakeFilename(PICTURE_TYPE t, int fileNumber);    

		// 获取IOS文件操作指针
		IIosFileSystem* GetIosFileIO();

		// ios图片的下载处理
		bool IosDownPictureFile(wstring iosPath, wstring localPath);

#pragma endregion

    private:
        // Summary  : 通过路径获取壁纸集合
        // Returns  : void
        // Parameter: PictureFolder * pWallpaper
        // Parameter: wstring folder
        void ReadWallpaperFolder(PictureFolder* pWallpaper, wstring folder);

    private:

		const DeviceData*			_pDevice;

        DirectoryOperationCommand*	_pDirCmd;				// 文件操作命令
		IIosFileSystem*				_pFileStream;

		int							_nVersion;				// 固件系统版本号
						


	};
}
