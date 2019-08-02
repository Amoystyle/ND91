#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{
    class PictureData;
    class PictureFolder;
	class PictureGroup;
	class AndroidPhotoBucket;
	class IosWallpaperOper;
    class UploadFileCommand;
	class DownloadFileCommand;
	class DirectoryOperationCommand;
	class IosPictureSqlite4;
	class IosPictureSqlite5;
	class IosPhotoDB;

    // interface: 照片/壁纸/截图/图库操作的不同版本实现
	class IosPicturesImpl
	{
	public:
        IosPicturesImpl(const DeviceData* pDevice);
        virtual ~IosPicturesImpl();

#pragma region 照片/壁纸/截图 相关操作

		// Summary  : 图片管理模块初始化函数
		// Returns  : void
		virtual bool Init();

        // Summary  : 加载(照片/壁纸/截图)图片
        // Parameter: PictureFolder * pCamera     用来存放照片
        // Parameter: PictureFolder * pWallpaper  用来存放壁纸
        // Parameter: PictureFolder * pScreenshot 用来存放截图
        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot) = 0;

		// Summary  : 新增多张(照片/壁纸/截图)图片 
		// Returns  : 新图片指针集合
		// Parameter: vector<wstring> sourceFiles  图片文件路径集合 
		// Parameter: int fileNumber		文件名的下一个数字(用于生成文件名)
		// Parameter: int thumbIndex		缩略图编号(仅对4.2.1以上版本有用)
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
						int fileNumber, int thumbIndex = -1, std::string gruopId = "") = 0;

		// Summary  : 删除多张(照片/壁纸/截图)图片
		// Returns  : 是否成功
		// Parameter: vector<const PictureData * > vPictures  被删除图片集合   
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* vPictures) = 0;

        // Summary  : 修改(照片/壁纸/截图)图片
        // Returns  : 是否成功
        // Parameter: PictureData * pic 图片指针
        // Parameter: std::wstring srcPath 图片文件路径
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath) = 0;

		// Summary  : 设置待机锁屏壁纸
		// Returns  : bool				
		// Parameter: wstring wallpaper   被选中壁纸的本地路径
		virtual bool SetLockBackground( wstring wallpaper ) = 0;

		// Summary  : 设置主屏幕壁纸
		// Returns  : bool
		virtual bool SetHomeBackground( wstring wallpaper ) = 0;

		// 获得各模块是否需要同步
		bool GetPhotosIthmbState()		{ return _bPhotosIthmbChange;		}
		bool GetPhotoDataBaseState()	{ return _bPhotoDataBaseChange;		}
		bool GetSqliteState()			{ return _bSqliteChange;			}
		bool GetPhotoDataIthmbState()	{ return _bPhotoDataIthmbChange;	}

		// 获得需要同步的图片
		map<wstring, wstring>* GetUploadPictureFiles(void) { return _UploadPictureFiles; }
		// 获得需要图片的uuid
		vector<char*> GetPictureUuids() {return _pictUuids;}
		
		// 各模块同步状态的初始化
		void ResetSyncState()
		{
			_bPhotosIthmbChange		= false;		
			_bPhotoDataBaseChange	= false;
			_bSqliteChange			= false;			
			_bPhotoDataIthmbChange	= false;
			_pictUuids.clear();
		}

#pragma endregion


#pragma region 图库部分相关操作

		// ----- 图库分类管理 -----

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
		virtual bool DeleteGalleryGroup(int groupId, vector<PictureData*>* VerPictDatas);



		// ----- 图库图片管理 -----
		
		// 加载所有图库图片
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas) = 0;

		// Summary  : 添加多张图库图片
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : 本地图片集合
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId, 
														  int nextNumber=0, int index=0, const string firstGroupId="" ) = 0;

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
		virtual bool DeleteGalleryPictures( const vector<const PictureData*>* vPictures );

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
		virtual bool DownloadThumbnail(PictureData* pPicture) = 0;
		
		// 下载原图文件
		virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath);


		// ----- 图片管理相关辅助方法 -----

		// 得到该版本下所有ithumb文件id
		virtual vector<int> GetThumbType() = 0;

		// Summary  : 下载图库管理所需文件
		// Returns  : bool
		virtual bool DownloadInitFiles() = 0;

		// 获取图片总数量
		int GetAllPicturesCount();

		// 获取相册截屏, 图库图片总数量
		virtual int GetSpecialPhotos();

#pragma endregion


#pragma region 同步处理操作

		// Summary  : 设置图库图片管理指针(仅提供给4.x版本增加和删除图片是用)
		// Returns  : void
		// Parameter: IosPhotoDB * pIosDB
		void SetIosDB(IosPhotoDB* pIosDB) { _pIosDB = pIosDB; }	

		// Summary  : 释放2个容器
		// Returns  : void
		void ReleaseCase();

		// Summary  : 上传图片（包括相机、截屏、图库、相关缩略图）
		// Returns  : bool
		virtual bool UploadPictureFiles(ProgressSubject* pSubject=NULL, int percent=100) = 0;

		// Summary  : 删除图片（包括相机、截屏、图片、相关缩略图）
		// Returns  : bool
		virtual bool DeletePictreFiles() = 0;

		// Summary  : 上传PhotoData下sqlite文件
		// Returns  : bool
		virtual bool PhotoSqilteSync() = 0;

		// Summary  : 上传photodatabase文件
		// Returns  : bool
		bool PhotodatabaseSync();

		// Summary  : 上传Photos下ithumb文件(图库缩略图文件)
		// Returns  : bool
		bool PhotosIthmbSync(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : 上传PhotoData下ithumb文件(相册缩略图文件)
		// Returns  : bool
		bool PhotoDataIthmbSync(ProgressSubject* pSubject=NULL, int percent=100);

	protected:

		// Summary  : 通过IosPhotoDB获得421版本需上传的图库图片集合
		// Returns  : void
		bool UploadFiles4X(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : 通过IosPhotoDB获得421版本需删除的图库图片集合
		// Returns  : void
		bool DeleteFiles4X();

		// Summary  : 公用sqlite的上传
		// Returns  : bool
		bool SqilteDBSync();

#pragma endregion

    protected:

        // 取照片和截图
        void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

		// 获取4.x所有图库图片
		bool LoadPicturesByGroup4X(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);
		
		// 添加4.x图库图片
		vector<PictureData*>* AddPictureToGroup4X( vector<wstring>* localPaths, const string groupId, const string firstGroupId );

        // Summary  : 生成不带扩展名的文件名
        wstring MakeFilename(PICTURE_TYPE t, int fileNumber);

		// 获得缩略图文件名
		vector<int> GetThumbNames();

		// Summary  : 获取指定目录下的.ithumb文件
		// Returns  : vector<NodeData*>
		// Parameter: DirData * pFolder
		map<wstring, wstring> GetPhotoAllThumbs(bool isPhotoData );

		// 下载3.0-4.3.5版本的图库相关文件
		bool DownloadGalleryFiles();

		// 下载421以上相册截屏缩略图文件
		bool DownloadPhotoDataThumbs();

		// 下载3.0-435版本图库图片缩略图
		bool DownloadGalleryThumbnails(PictureData* pPicture);

		// 下载421以上特殊分类缩略图
		bool DownloadSpecialThumbnails(PictureData* pPicture);

		// 获取4.x相册图片总数
		int GetCameraCount4x();

		// 获得3.x-435图库图片总数
		int GetGalleryPicCount();

		// 获得壁纸总数
		int GetWallpaperPicCount();

		// 根据不同路径得到该目录下的壁纸数目
		int GetPicCountByPath( wstring wallPaperPath );

	public:
		IosWallpaperOper*			_pWallpaper;				// 壁纸相关操作类
		vector<char*>               _pictUuids;                 // 需要同步的图片的uuid
    protected:

        const DeviceData*			_pDevice;
	
		IosPictureSqlite4*			_ios4SQL;					// ios4.x数据库操作接口
		IosPictureSqlite5*			_ios5SQL;					// ios5数据库操作接口
		IosPhotoDB*					_pIosDB;					// 只为获取4.x版本上传，删除图库图片的暂时做法

		map<wstring, wstring>*		_UploadPictureFiles;
		vector<wstring>*			_DeletePictureFiles;

		bool						_bPhotosIthmbChange;		// 图库缩略图文件所有被修改
		bool						_bPhotoDataBaseChange;		// 图库photodatabase被修改
		bool						_bSqliteChange;				// 数据库sqlite文件被修改
		bool						_bPhotoDataIthmbChange;		// 相册缩略图文件是否被修改

		DownloadFileCommand*        _pDownCmd;

    private:

		int							_nVersion;					// 固件系统版本号
						


	};
}
