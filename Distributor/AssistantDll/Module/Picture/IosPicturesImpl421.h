#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    // 照片/壁纸/截图操作的IOS 4.2.1以上实现
	class IosPicturesImpl;
	class PictureData;
	class IosPicturesImpl421 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl421(const DeviceData* pDevice);
        virtual ~IosPicturesImpl421(void) {}


        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);

		// Summary  : 新增多张图片
		// Returns  : vector<PictureData*>*			新图片指针集合
		// Parameter: vector<wstring> vPicFiles     图片文件路径集合
		// Parameter: int fileNumber
		// Parameter: int thumbIndex
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : 删除多张图片
		// Returns  : 是否成功
		// Parameter: const vector<const PictureData * > pVecPicture   被删除图片集合  
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* pVecPicture);

        // Summary  : 修改图片
        // Returns  : 是否成功
        // Parameter: PictureData * pic 图片指针
        // Parameter: std::wstring srcPath 图片文件路径
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath);

		// Summary  : 设置待机锁定屏幕壁纸
		// Returns  : bool				
		// Parameter: wstring wallpaper   被选中壁纸的本地路径
		virtual bool SetLockBackground( wstring wallpaper );

		// Summary  : 设置主屏幕壁纸
		// Returns  : bool
		virtual bool SetHomeBackground( wstring wallpaper );

		// Summary  : 上传图片（包括相机、截屏、图库、相关缩略图）
		// Returns  : bool
		virtual bool UploadPictureFiles(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : 删除图片（包括相机、截屏、图片、相关缩略图）
		// Returns  : bool
		virtual bool DeletePictreFiles();

		// Summary  : 上传PhotoData下sqlite文件
		// Returns  : bool
		virtual bool PhotoSqilteSync();


#pragma region 图库部分相关操作

		// ----- 图库图片管理 -----

		// 下载该版本所需初始化文件
		virtual bool DownloadInitFiles();

		// 加载所有图库图片
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : 添加多张图库图片
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );


		// 下载缩略图文件
		virtual bool DownloadThumbnail(PictureData* pPicture);


		// ----- 图片管理相关辅助方法 -----

		// 得到该版本下所有ithumb文件名
		virtual vector<int> GetThumbType();


#pragma endregion

	private:
		
		// Summary  : 往32x32.ithmb等文件写入数据
		// Returns  : 
		// Parameter: Image * img
		// Parameter: int idnex
		void WriteThumbDB(Image* img, int idnex);

		// 判断设备是否是IPad
		bool		_isIPad;							

	};
}