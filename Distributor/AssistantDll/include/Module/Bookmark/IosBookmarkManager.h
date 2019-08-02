 #pragma once

#include "BookmarkManager.h"

namespace ND91Assistant
{
	// Ios书签文件夹类
	class DeviceData;
	class BookmarkData;
	class BookMarksSynchronizer;
	class CLASSINDLL_CLASS_DECL IosBookmarkManager : public BookmarkManager
	{
		friend class IosBookMarkDB;
		friend class IosBookMarkPlistHelper;
	public:
		IosBookmarkManager(const DeviceData* pDevice);
		virtual ~IosBookmarkManager(void);

//         IosBookmarkManager(map<string,void*> mapEntities,const DeviceData* pDevice);

		virtual BookmarkFolder* LoadBookMarks();

		// Summary  : 添加书签
		// Returns	: bool
		// Parameter: wstring tittle 标题
		// Parameter: wstring url    网址, Ios可空，表示添加文件夹
		// Parameter: wstring des	 描述
		virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url=L"", wstring des=L"");

		// Summary  : 添加书签导入用
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder);

		// Summary  : 删除书签
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool DeleteBookMark(BookmarkData* pBookMark);

		// Summary  : 更新书签
		// Returns	: bool
		// Parameter: BookmarkData * pBookMark
		virtual bool UpdateBookMark(BookmarkData* pBookMark);

		// Summary  : 同步到手机，仅根节点操作
		// Returns	: bool
		virtual bool  SyncToPhone();

		// Summary  : 上移下移
		// Returns	: bool  
		// Parameter: BookmarkData * pChildBookMark 子节点
		// Parameter: bool Up 上下
		bool MoveBookMarkNode(BookmarkData* pChildBookMark, bool Up);


	private:
		enum DBOperateType //数据库操作类型
		{
			EDelete,  // 删除
			EUpdate,  // 更新
			EAdd	  // 添加
		};
		// Summary  : 获得新增孩子节点的顺序Id
		// Returns	: int
		int  GetChildOrderIndex();

		// Summary  : 320以上版本数据操作
		// Returns	: void
		// Parameter: DBOperateType type
		// Parameter: BookmarkData * pBookMark
		bool DBOperation(DBOperateType type, BookmarkData* pBookMark);

		BookmarkData* FindNode(BookmarkData* pBookMark);


	private:

		int                     _version; //版本号

		BookmarksSynchronizer* _pBookmarkSync;
	};

}