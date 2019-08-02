#pragma once

#include "sqlite/CppSqlite3.h"

namespace ND91Assistant
{
	// 照片/壁纸/截图sqlite操作 (IOS 4.x实现)
	class PictureData;
	class IosPictureSqlite4
	{
	public:
		IosPictureSqlite4(const DeviceData* pDevice);
		virtual ~IosPictureSqlite4(void);

		// Summary  : 从photo.sqlite数据库中读取照片截图数据
		// Returns  : 是否成功
		// Parameter: vector<PictureData * > * pVecPicture 用于存放数据的输出容器
		bool GetPhotoFromSqlite(vector<PictureData*>* pVecPicture);

		// Summary  : 插入一张照片到photo.sqlite数据库
		// Returns  : 是否成功
		// Parameter: PictureData * pdata
		bool InsertPhotoToSqlite(vector<PictureData*>* pVecPicture);

		// Summary  : 从photo.sqlite数据库删除一张照片
		// Returns  : 是否成功
		// Parameter: int pictureid
		bool DeletePhotoFromSqlite(vector<int>* pVecPicId);

		// Summary  : 从photo.sqlite数据库修改一张照片数据
		// Returns  : 是否成功
		// Parameter: PictureData * pdata
		bool UpdatePhotoToSqlite(PictureData* pdata);

		// Summary  : 关闭数据库
		// Returns  : bool
		bool ClosePhotoDB();


	private:

		// Summary  : 打开数据库
		// Returns  : bool
		bool OpenPhotoDB();

		// Summary  : 开启数据库事务
		// Returns  : bool
		void PhotoDBTransaction();

		// Summary  : 事务提交
		// Returns  : bool
		void PhotoDBCommit();

		// Summary  : 异常回滚
		// Returns  : bool
		void PhotoDBRollback();

		// Summary  : 往数据库photo.sqlite表PhotoExtras插入图片数据
		// Returns  : bool
		// Parameter: int id		图片id
		// Parameter: wstring name	图片名称
		// Parameter: int size		图片大小(421以上需要)	
		bool InsertToPhotoExtras(int id, wstring name, int size);

		// Summary  : 打印出数据库操作异常日志
		// Returns  : void
		// Parameter: wstring strMsg
		// Parameter: int nErrorCode
		// Parameter: std::string strErrorMsg
		void WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg);

		const DeviceData*		_pDevice;
		int						_nVersion;			// 固件系统版本号

		CppSQLite3DB*			_SQLite;			// 操作photo.sqlite句柄	
		CppSQLite3DB*			_AuxSQLite;			// 操作photoAux.sqlite句柄

		

	};

}
