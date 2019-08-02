#pragma once

#include "Module/Picture/BaseGroup.h"
#include "Module/Picture/PhotoBucketData.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/ThumbnailData.h"

namespace ND91Assistant
{
    // 设置壁纸动作的类型
	enum SetWallPaper_Type
	{
        UnKnowType      = 0,	// 未知
		LockBackground  = 1,	// 锁屏屏幕
		HomeBackground  = 2,	// 主屏幕(4.0 新增)
		LockAndHome     = 3,	// 同时设置主屏幕和锁屏屏幕
	};

    class DeviceData;
    class AndroidPhotoBucket;
	class PictureData;

    class PictureFolder;
    class PictureGroup;
	class PictureLibrary;
	class BaseGroup;

    // 图片管理
    class CLASSINDLL_CLASS_DECL PictureManager
    {
    public:
        PictureManager(const DeviceData* pDevice);
        virtual ~PictureManager(void);

        // Summary  : 初始化加载
        // Returns	: 是否成功
        virtual bool Load() = 0;

		// 释放
		void Release();

#pragma region 分类相关操作

        // 获取图库分类
        void GetPictureGroups(vector<PictureGroup*>* pGroups){
            pGroups->insert(pGroups->begin(), _PictureGroups.begin(), _PictureGroups.end());
        }

		// 获取照片
		PictureFolder* GetCamera(){return _pCamera;}

		// 获取屏幕截图
		PictureFolder* GetScreenShot(){return _pScreenShot;}

		// 获取壁纸
		PictureFolder* GetWallpaper(){return _pWallpaper;}

        // Summary  : 添加图库分类
        // Parameter: const wstring displayname分类名
		virtual std::string AddPhotoGroup(const wstring displayname, wstring path = L"") = 0;

        // Summary  : 删除图库分类
        // Returns  : 是否成功
        // Parameter: PictureGroup * pGroup 分类指针
        virtual bool DeleteGroup(PictureGroup* pGroup) = 0;

		// Summary  : 修改图库分类名
		// Returns  : 是否成功
		// Parameter: const string nGroupid 分类ID
		// Parameter: const wstring name	分类名
		virtual bool RenameGroup(const string nGroupid, const wstring name) = 0;

#pragma endregion

#pragma region 图片相关操作

        // 获取全部图片
        void GetAllPictures(vector<PictureData*>* pDeque) const;

		// Summary  : 新增图片到分类(若是普通分类, 会自动添加到"所有图片"中)
		// Returns	: 是否成功
		// Parameter: localPath : 要添加的本地图片
		// Parameter: nType		: 要添加到的分类
		bool AddPictureToGroup(wstring localPath, PICTURE_TYPE nType);
        
		// Summary  : 添加多张相册图片(仅支持ios4.0以上版本)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId) = 0;


		// Summary  : 批量删除图片
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : 须删除的图片集合
		// Parameter: PictureFolder* pFolder : 删除图片的所属分类
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId) = 0;

        // Summary  : 编辑图片
		// Returns  : bool 是否成功
		// Parameter: const PictureData * pPicture
		virtual bool ChangeImage( PictureData* pPicture, wstring srcPath) = 0;

		// Summary  : 将图片移到另一个分类
		//            只能在普通分类间移动. 对特殊分类要使用添加和删除方法
		// Returns	: bool 是否成功
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: pSourceGroup : 原分类
		// Parameter: pDestGroup   : 要移到的分类
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, 
									string sourceGroupId, 
									string destGroupId) = 0;

        // Summary  : 下载图片
        // Returns  : 是否成功
        // Parameter: pPicture 图片指针
        // Parameter: wstring 自定义图片保存的路径
        virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath = L"") = 0;

        // Summary  : 下载图片的缩略图
        // Returns  : 是否成功
        // Parameter: pPicture 图片指针
        virtual bool DownloadThumbnail(PictureData* pPicture) = 0;

		// Summary  : 设为壁纸
		// Returns	: bool
		// Parameter: wstring localpath
		virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType) = 0;

		// Summary  : 根据分类id获得分类指针
		// Returns  : BaseGroup*	图片指针
		// Parameter: string id		分类id
        BaseGroup* GetBaseGroup( string id );

        // 若未越狱则不包括壁纸数量
        virtual int GetPhotoCount();

		// 各个分类中是否包含此图片
		bool ContainsInGroup(int pictureId);

#pragma region 图片相关操作

#pragma region 私有方法
    protected:

        PictureGroup* FindGroup( string id );

        // 创建照片分类
        PictureFolder* CreateCamera(wstring path);

        // 创建壁纸分类
        PictureFolder* CreateWallpaper(wstring path);

        // 创建截图分类(IOS用)
        PictureFolder* CreateScreenshot( wstring path );

		// Summary  : 创建缩略图
		// Returns  : 
		// Parameter: std::wstring picPath 源图文件路径
		// Parameter: int & width  预定缩略图宽度, 若原图宽高均小于预定宽高则被赋值为原图宽度
		// Parameter: int & height 预定缩略图高度, 若原图宽高均小于预定宽高则被赋值为原图高度
		// Parameter: std::wstring thmPath 缩略图文件路径
		bool MakeThumbnail(std::wstring picPath, int& width, int& height, std::wstring thmPath, bool isIos);

#pragma endregion

    protected:

        // 图库的普通分类用PictureGroup存放图片ID, 图片则存放在在_AllPictures中
        vector<PictureGroup*>		_PictureGroups;		// 图库分类
        PictureLibrary*	            _pAllPictures;		// 图库图片

        // 照片/壁纸/截图这三种特殊分类, 使用PictureFolder存放PictureData指针
        PictureFolder*              _pCamera;	  		// 照片
        PictureFolder*              _pWallpaper;  		// 壁纸
        PictureFolder*              _pScreenShot; 		// 截图

        const DeviceData*			_pDevice;			

    };

}
