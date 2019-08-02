#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{
    class PictureData;
    class PictureFolder;
	class DirectoryOperationCommand;

    // interface: ��Ƭ/��ֽ/��ͼ�����Ĳ�ͬ�汾ʵ��
	class IosWallpaperOper
	{
	public:
        IosWallpaperOper(const DeviceData* pDevice);
        virtual ~IosWallpaperOper();

#pragma region ��ֽ��ز���

		// Summary  : �ж��Ƿ�֧����������Ļ��ֽ
		// Returns  : bool
		bool IsSupportHomeScreen();

		// Summary  : ���ñ�ֽ���ϴ������ļ�RingWallpaper.ini
		// Returns  : bool
		bool UploadWallpaperInI(wstring localPath);

		// Summary  : ������������Ǽ�ֵ
		// Returns  : bool
		// Parameter: bool isSame		�����������Ƿ���ͬһ��ͼƬ 
		bool SetUniqueHomeScreenValue(bool isSame);

        // ios��ֽ�ļ����ǰ��������е�, ������ȡ����һ������
        wstring GetWallpaperNumber(wstring deviceFolder);

		// ��Ҫ���������ļ�Ȩ��
        inline DirectoryOperationCommand* DirCommand();

        // Summary  : ��������ͼ
        // Returns  : �Ƿ�ɹ�
        // Parameter: sourceFile    ԭ�ļ���
        // Parameter: destFile      ���ļ���
        // Parameter: width         ���ļ���
        // Parameter: height        ���ļ���
        // Parameter: bFill         �Ƿ�Ӧ�ó���(����ȡԭͼ�м䲿����������ͼ)
        bool MakeThumbnail(wstring sourceFile, wstring destFile, int width, int height, bool bFill = true);

        // Summary  : ���ӱ�ֽ
        // Returns  : ͼƬ����, NULL��ʾʧ��
        // Parameter: localFile    ����ͼƬ�ļ�
        // Parameter: remoteFolder �µ��ļ���(������չ����׺)
        // Parameter: need2x       �Ƿ���Ҫ2����ͼ(ios4������Ҫ)
        PictureData* AddWallpaper(std::wstring localFile, wstring newFilename, bool need2x);

        // Summary  : ��ȡ��ֽ����(4����)
        // Parameter: pWallpaper    ��ֽ������
        void LoadWallpaper4(PictureFolder* pWallpaper);

		// Summary  : �޸ı�ֽ(421����)
		// Parameter: pWallpaper    ͼƬָ��
		bool UpdateWallpaper421(PictureData* pWallpaper, wstring sourcePath);

		// Summary  : ɾ����ֽ(421����)
		// Parameter: pWallpaper    ͼƬָ��
		bool DeleteWallpaper421( const PictureData* pWallpaper );

		// Summary  : ���ñ�ֽ(421����)
		// Returns  : bool
		// Parameter: wstring wallpaper		 ѡ�б�ֽ����·��
		// Parameter: bool isHome			�Ƿ�������������ֽ
		bool SetBackground421( wstring wallpaper, bool isHome );

		// ----  ���ñ�ֽ  ----

		// Summary  : �߰汾IOS�ֻ�����.cpbitmap��ֽ�ļ�
		// Returns  : bool
		bool ConvertPngToCPBitmap(wstring cpBitmap);

		// Summary  : �ж�Plist�ļ��еı��
		// Returns  : bool
		// Parameter: strPlist
		bool IsHasHomeScreen(wstring strPlist);

        // ȡ��Ƭ�ͽ�ͼ
        void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

        // Summary  : ���ɲ�����չ�����ļ���
        wstring MakeFilename(PICTURE_TYPE t, int fileNumber);    

		// ��ȡIOS�ļ�����ָ��
		IIosFileSystem* GetIosFileIO();

		// iosͼƬ�����ش���
		bool IosDownPictureFile(wstring iosPath, wstring localPath);

#pragma endregion

    private:
        // Summary  : ͨ��·����ȡ��ֽ����
        // Returns  : void
        // Parameter: PictureFolder * pWallpaper
        // Parameter: wstring folder
        void ReadWallpaperFolder(PictureFolder* pWallpaper, wstring folder);

    private:

		const DeviceData*			_pDevice;

        DirectoryOperationCommand*	_pDirCmd;				// �ļ���������
		IIosFileSystem*				_pFileStream;

		int							_nVersion;				// �̼�ϵͳ�汾��
						


	};
}
