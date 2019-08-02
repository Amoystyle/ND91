#pragma once

#include "PictureManager.h"

namespace ND91Assistant
{
    class DeviceData;
	class PictureData;

    class BaseGroup;
    class PictureFolder;
    class PictureGroup;
	class IosPictureSqlite5;

    // 图片管理
	class CLASSINDLL_CLASS_DECL IosPictureManager : public PictureManager
    {
    public:
        IosPictureManager(const DeviceData* pDevice);
        virtual ~IosPictureManager(void);

        // Summary  : 初始化加载
        // Returns	: 是否成功
        virtual bool Load();

		// Summary  : 释放内存
		void Release();
		
		// Summary  : 同步到手机
		// Returns	: 是否成功
		bool IosSync(ProgressSubject* pSubject); 
		bool IosSyncThroughATHost();

		// Summary  : 清除缓存目录
		// Returns	: void
        void ClearCache();

#pragma region 分类相关操作

        // Summary  : 添加图库分类
        // Parameter: const wstring displayname分类名
		virtual std::string AddPhotoGroup(const wstring displayname, wstring path = L"");

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

        // Summary  : 新增图片到分类(若是普通分类, 会自动添加到"所有图片"中)
        // Returns	: 是否成功
        // Parameter: localPath : 要添加的本地图片
        // Parameter: groupId   : 要添加到的分类
        //virtual bool AddPictureToGroup(wstring localPath, PICTURE_TYPE nType);

		// Summary  : 添加多张相册图片(仅支持ios4.0以上版本)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId);


		// Summary  : 将特殊分类和图库中已存在的图片, 添加到另一个图库分类(仅ios用)
		// Returns	: 是否成功: 若分类是全部或者分类id找不到则返回false
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: groupId  : 分类ID
		bool IosAddExistPicture(const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : 批量删除图片
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : 须删除的图片集合
		// Parameter: PictureFolder* pFolder : 删除图片的所属分类
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : 从图库分类中移除图片(仅ios用)
		//            若此分类是普通分类, 则"所有图片"中依然有此图片
		//            若此分类是所有图片, 则不支持返回false
		// Returns	: 是否成功: 若分类id是"全部图片"或者分类id找不到则返回false
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: groupId  : 分类ID
		bool IosRemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : 将图片移到另一个分类
		//            只能在普通分类间移动. 对特殊分类要使用添加和删除方法
		// Returns	: bool 是否成功
		// Parameter: const vector<const PictureData*>* vPictures : 图片指针集合
		// Parameter: pSourceGroup : 原分类
		// Parameter: pDestGroup   : 要移到的分类
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, 
									string sourceGroupId, 
									string destGroupId);

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

		// Summary  : 判断是否支持主屏设置壁纸
		// Returns  : bool
		bool IsSupportHomeScreen();

        // Summary  : 设为壁纸
        // Returns	: bool
        // Parameter: wstring localpath
        virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType);

		// 获取全部图片数量
		int GetAllPicturesCount();

		//Summary  : 为要上传的图片产生plist文件
		//Parameter: 图片信息pVecPicture，图片的uuid，图库的名称groupName
		void GeneratePlistFile(vector<PictureData*>* pVecPicture, vector<char*> uuids, wstring groupName);

		// Summary  : 读取手机上存储的图库的UUID
		// Returns	: char*图库的UUID
		char* GetLibraryUUIDFromDevice();

#pragma region 图片相关操作

#pragma region 私有方法
    private:

		// 同步修改后的相册图片数据到手机端 (4.0-5.0版本相册图片同步专用)
		bool IosSyncHelper();

        // 数据，路径初始化
        bool IosInit();

        // Summary  : 根据设备的系统版本, 创建不同的ios图片操作实现类
        // Returns  : ios图片操作实现类指针
        IosPicturesImpl* CreateIosImplement();

        // IOS加载特殊分类
        void LoadIosBucket();

        // IOS加载图库图片
        void LoadIosPicture();

        virtual int GetPhotoCount();
#pragma endregion

	protected:
		IosPicturesImpl*            _pIosImpl;      	// Ios图片操作实现类

    private:
        int							_nDeviceVer;		// 系统版本
		SetWallPaper_Type			_wallpaperType;     // 是否支持双屏壁纸
    };

}
