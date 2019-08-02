#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    // 相册/壁纸/截图/图库 IOS 5.0以上图片操作的实现
	class IosPicturesImpl;
	class PictureData;
	class IosPicturesImpl5 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl5(const DeviceData* pDevice);
        virtual ~IosPicturesImpl5();

		bool Init();	

        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);
	
		// Summary  : 新增多张图片
		// Returns  : vector<PictureData*>*			新图片指针集合
		// Parameter: vector<wstring> vPicFiles     图片文件路径集合
		// Parameter: int fileNumber
		// Parameter: int thumbIndex
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : 删除多张图片(仅支持ios4.0.0以上版本)
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
		// Parameter: bool isHome			是否是设置主屏壁纸
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

		// ----- 图库分类管理 -----

		// 下载该版本所需初始化文件
		virtual bool DownloadInitFiles();

		// IOS加载所有图库分类
		virtual map<int, wstring>* LoadGalleryBucket();

		// 添加图库分类
		virtual int AddGalleryGroup( const wstring displayname,  wstring path);

		// Summary  : 修改图库分类名
		// Returns  : 是否成功
		// Parameter: const string nGroupid 分类ID
		// Parameter: const wstring name	分类名
		virtual bool RenameGalleryGroup(const string nGroupid, const wstring name);

		// Summary  : 删除图库分类
		// Returns  : 是否成功
		// Parameter: PictureGroup * pGroup 分类指针
		virtual bool DeleteGalleryGroup( int groupId, vector<PictureData*>* VerPictDatas);


		// ----- 图库图片管理 -----

		// 加载所有图库图片
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : 添加多张图库图片
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );

		// Summary  : 将一分类多张图片移到另一个分类
		//            只能在普通分类间移动. 对特殊分类要使用添加和删除方法
		// Returns	: bool 是否成功
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: pSourceGroup : 原分类
		// Parameter: pDestGroup   : 要移到的分类
		virtual bool MovePictures( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId);

		// Summary  : 批量删除图库图片
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : 须删除的图片集合
		// Parameter: PictureFolder* pFolder : 删除图片的所属分类
		virtual bool DeleteGalleryPictures( const vector<const PictureData*>* vPictures);

		// Summary  : 从图库分类中移除图片
		//            若此分类是普通分类, 则"所有图片"中依然有此图片
		//            若此分类是所有图片, 则不支持返回false
		// Returns	: 是否成功: 若分类id是"全部图片"或者分类id找不到则返回false
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: groupId  : 分类ID
		virtual bool RemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId);

		// Summary  : 将分类已存在的图片, 添加到另一个分类
		// Returns	: 是否成功: 若分类是全部或者分类id找不到则返回false
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: groupId  : 分类ID
		virtual bool IosAddExistPictures(const vector<const PictureData*>* vPictures, const string groupId);

		// Summary  : 编辑图库图片
		// Returns  : bool 是否成功
		// Parameter: const PictureData * pPicture
		virtual bool UpdateGalleryPicture( PictureData* pPicture, wstring srcPath );

		// 下载缩略图文件
		virtual bool DownloadThumbnail(PictureData* pPicture);


		// ----- 图片管理相关辅助方法 -----

		// 得到该版本下所有ithumb文件名
		virtual vector<int> GetThumbType();

		// 获取相册截屏, 图库图片总数量
		int GetSpecialPhotos();


#pragma endregion

	private:
		
		// Summary  : 加载相册和截屏图片
		// Returns  : void
		// Parameter: PictureFolder * pCamera		相册分类指针
		// Parameter: PictureFolder * pScreenshot	截屏分类指针
		void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

		// Summary  : 往.ithmb文件添加缩略图
		// Returns  : void
		// Parameter: Image * img			原图指针
		// Parameter: int idnex				缩略图位置
		void WriteThumbDB(Image* img, int idnex);

		// Summary  : 上传缩略图配置信息文件
		// Returns  : void
		void UpLoadthumbConfig();

		// Summary  : 判断是否是截屏
		// Returns  : bool
		bool IsScreenShoot(int width, int heigth);

		// Summary  : ios5初始化函数
		// Returns  : bool
		bool Initialize();

	private:

		bool					_ios5HasInit;		// ios5是否已经下载初始化数据库文件
		vector<PictureData*>    _vPictures;			// 所有图库图片集合	 
	};
}