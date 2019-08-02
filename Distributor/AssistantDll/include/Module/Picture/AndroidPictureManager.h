#pragma once

#include "PictureManager.h"

namespace ND91Assistant
{
    class DeviceData;
    class AndroidPhotoBucket;
	class PictureData;

    class BaseGroup;
    class PictureFolder;
    class PictureGroup;

    // 图片管理
    class CLASSINDLL_CLASS_DECL AndroidPictureManager : public PictureManager
    {
    public:
        AndroidPictureManager(const DeviceData* pDevice);
        virtual ~AndroidPictureManager(void);

        // Summary  : 初始化加载
        // Returns	: 是否成功
        virtual bool Load();

#pragma region 分类相关操作

        // Summary  : 添加图库分类
        // Parameter: const wstring displayname分类名
        virtual string AddPhotoGroup(const wstring displayname, wstring path = L"");

        // Summary  : 删除图库分类
        // Returns  : 是否成功
        // Parameter: PictureGroup * pGroup 分类指针
        virtual bool DeleteGroup(PictureGroup* pGroup);

		// Summary  : 修改图库分类名
		// Returns  : 是否成功
		// Parameter: const string nGroupid 分类ID
		// Parameter: const wstring name	分类名
		virtual bool RenameGroup(const string nGroupid, const wstring name);

#pragma endregion

#pragma region 图片相关操作

        
		// Summary  : 添加多张相册图片(仅支持ios4.0以上版本)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId);


		// Summary  : 批量删除图片
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : 须删除的图片集合
		// Parameter: PictureFolder* pFolder : 删除图片的所属分类
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : 将图片移到另一个分类
		//            只能在普通分类间移动. 对特殊分类要使用添加和删除方法
		// Returns	: bool 是否成功
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: pSourceGroup : 原分类
		// Parameter: pDestGroup   : 要移到的分类
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId);

        // Summary  : 编辑图片
		// Returns  : bool 是否成功
		// Parameter: const PictureData * pPicture
		virtual bool ChangeImage( PictureData* pPicture, wstring srcPath);

        // Summary  : 下载图片
        // Returns  : 是否成功
        // Parameter: pPicture 图片指针
        // Parameter: wstring 自定义图片保存的路径
        virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath = L"");

        // Summary  : 下载图片的缩略图
        // Returns  : 是否成功
        // Parameter: pPicture 图片指针
        virtual bool DownloadThumbnail(PictureData* pPicture);

		// Summary  : 设为壁纸
		// Returns	: bool
		// Parameter: wstring localpath
		virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType);

        virtual int GetPhotoCount();
#pragma region 图片相关操作

#pragma region 私有方法
    private:

		// Summary  : 根据分类id找分类是否存在
		bool ExistBucket(string groupId);

		// Summary  : 创建分类 (安卓用)
		// Return   : 分类ID, 空表示失败
		// Parameter: wstring path 设备上的路径
		string CreateAndroidBucket(wstring path);

        // 在设备上创建分类
        bool CreateAndroidBucketInDevice( AndroidPhotoBucket* Bucket );

        // 在内存中创建分类
        void CreateAndroidBucketInMemory(AndroidPhotoBucket* pBucket);

        // Summary  : 加载分类 (安卓用)
        // Returns	: void
        void LoadAndroidBucket();

        // Summary  : 加载图片 (安卓用)
        // Returns	: void
        void LoadAndroidPicture();

        // Summary  : 加载缩略图信息 (安卓用)
        void LoadAndroidThumbnails();

		// Summary  : 添加图片 (安卓用)
		// Parameter  groupId	分类ID, 空表示失败
		// Parameter: filePath	本地文件的路径
		const PictureData* AndroidAddPicture(wstring filePath, const string groupId);

#pragma endregion
        
    private:

    };

}
