#pragma once

#include "sqlite/CppSqlite3.h"

namespace ND91Assistant
{
    // 照片/壁纸/截图sqlite操作 IOS 5.0以上实现
	class PictureGroup;
	class PictureData;

	class IosPictureSqlite5
	{
	public:
		IosPictureSqlite5( const DeviceData* pDevice );
        virtual ~IosPictureSqlite5();

		// Summary  : 初始化IOS5数据库
		// Returns  : bool
		bool OpenIos5DB();

		// Summary  : 加载ios5所有图库分类
		// Returns  : map<int, wstring>
		map<int, wstring>* LoadIos5Category();

		// Summary  : 添加ios5图库分类
		// Returns  : 返回分类ID
		int AddIos5Category(const wstring name);

		// Summary  : 重命名ios5图库分类
		// Returns  : bool
		bool RenameIos5Category(const int id, const wstring name);

		// Summary  : 删除ios5图库分类
		// Parameter: const int id		分类ID	
		// Returns  : bool
		bool DeleteIos5Category(const int id, vector<int>* vDelPicId);

		// Summary  : 加载相册,截屏图片
		// Returns  : bool
		// Parameter: vector<PictureData * > * pVecPicture   图片集合
		// Parameter: bool isGallery						 是否是加载图库图片	
		bool LoadIos5Pictures( vector<PictureData*>* pVecPicture, bool isGallery );

		// Summary  : 插入相册图片数据
		// Returns  : bool
		// Parameter: PictureData * pdata			图片指针
		bool InsertIos5PicturesToDB(vector<PictureData*>* pVecPicture, std::string gruopId);

		// Summary  : 删除一条相册,截屏图片数据
		// Returns  : bool
		// Parameter: int id            图片id
		bool DeleteIos5Pictures(vector<int>* pVecPicId);

		// Summary  : 修改一条相册,截屏图片数据
		// Returns  : bool
		// Parameter: PictureData * pdata        图片指针
		bool UpdateIos5Pictures(const PictureData* pdata);

		// Summary  : 添加已存在的图库图片数据
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			图片指针集合
		// Parameter: std::string gruopId						分类id
		bool Ios5AddExistPictureToGroup(vector<int>* pVecPictureid, std::string gruopId);

		// Summary  : 从图库分类中移除图片数据
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			图片指针集合
		// Parameter: std::string gruopId						分类id
		bool Ios5RemoveFromGroup(vector<int>* pVecPictureid, std::string gruopId);

		// Summary  : 从图库分类中移动图片数据
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			图片指针集合
		// Parameter: std::string sourceGroupId					原分类id
		// Parameter: std::string DestGroupId					目标分类id
		bool Ios5MovePictures(vector<int>* pVecPictureid, std::string sourceGroupId, std::string destGroupId);

		// Summary  : 根据分类获得该分类下的所有图片
		// Returns  : vector<PictureData*>*
		// Parameter: PictureGroup* pGroup
		bool GetIosCategoryImages(PictureGroup* pGroup, vector<PictureData*>* pVerPictures, vector<PictureData*>& pVerDatas);

		// Summary : 获得图片的UUID
		// Returns : 返回图片的UUID
		vector<char*> GetPictureUuids();

		// Summary : 将图片GUID类型的UUID转换成char*
		char* ConvertUuidToPChar(GUID uuidIn);

		// Summary ：清除临时存放的图片的UUID
		void ClearTempPictureUuids();		

	public:
		vector<char*> _picUuids;  // 图片的UUID

	private:

		// Summary  : ios5的Sqlite数据库触发器操作
		// Returns  : bool
		bool InitDBTrigger();

        // 获取数据库中的一些字段
        bool InitParamsByDB();
		
		// Summary  : 打印出数据库操作异常日志
		// Returns  : void
		// Parameter: wstring strMsg
		// Parameter: int nErrorCode
		// Parameter: std::string strErrorMsg
		void WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg);

		bool DropDBTrigger();
	private:
		
		const DeviceData*		_pDevice;

		CppSQLite3DB*			_SQLdb;				// 接口句柄	
     
        // 获取数据库中的一些字段，以便对相应的数据库操作
        int _iAlbumENT;
        int _iGenericAssetENT;
        int _iGenericAlbumENT;
        int _iAssetENT;
        int _iWallpaperAssetENT;
        int _iAdditionalAssetAttributesEnt;
        int _iAlbumListENT;
        bool _bInitParams; 

	};
}