#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    class PictureFolder;
    class PictureData;
    class NodeData;
	class DeviceData;
	class AndroidPhotoBucket;
	class IosPicturesImpl;

    // 照片/壁纸/截图操作的IOS 3 版本实现类
	class IosPicturesImpl3 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl3(const DeviceData* pDevice);
        virtual ~IosPicturesImpl3(void) {}

        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);

		// Summary  : 新增多张图片 
		// Returns  : 新图片指针集合
		// Parameter: vector<wstring> sourceFiles  图片文件路径集合 
		// Parameter: int fileNumber		文件名的下一个数字(用于生成文件名)
		// Parameter: int thumbIndex		缩略图编号(仅对4.2.1以上版本有用)
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : 删除多张图片
		// Returns  : 是否成功
		// Parameter: const vector<const PictureData * > vPictures   被删除图片集合  
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* pVecPicture);

        // Summary  : 修改图片
        // Returns  : 是否成功
        // Parameter: PictureData * pic 图片指针
        // Parameter: std::wstring srcPath 图片文件路径
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath);

		// Summary  : 设置待机锁定屏幕壁纸
		// Returns  : bool 
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

		// 下载该版本所需初始化文件
		virtual bool DownloadInitFiles();


		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : 添加多张图库图片
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );

		// 下载缩略图文件
		virtual bool DownloadThumbnail(PictureData* pPicture);

		// 下载原图文件
		virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath);


		// 得到该版本下所有ithumb文件id
		virtual vector<int> GetThumbType();

		// 获取相册截屏, 图库图片总数量
		int GetSpecialPhotos();


#pragma endregion

	private:

        // Summary  : 根据目录内容, 生成图片, 存放到PictureFolder中
        // Parameter: deque<NodeData * > * nodes 目录节点
        // Parameter: PictureFolder * pFolder    容器
        void Load( deque<NodeData*>* nodes, PictureFolder* pFolder);

	};

}
