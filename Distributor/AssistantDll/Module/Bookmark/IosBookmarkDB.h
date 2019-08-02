#pragma once

#include "Module/Common/IosDB/IosDBBase.h"

namespace ND91Assistant
{
	class BookmarkData;
	class BookmarkFolder;
	class DeviceData;

	// Ios书签数据库操作
	class IosBookmarkDB:public IosDBBase
	{
	public:
		IosBookmarkDB(IosDBPath iosDBPath);
		~IosBookmarkDB(void);

		// Summary  : 获得所有书签
		// Returns	: BookmarkFolder* 返回根节点
		// Parameter: const DeviceData * pDevice
		BookmarkFolder* GetAllBookMark(const DeviceData* pDevice);
		
		// Summary  : 删除书签
		// Returns	: void
		// Parameter: BookmarkData * pNode
		void DeleteNode(BookmarkData* pNode);

		// Summary  : 删除孩子节点
		// Returns	: void
		// Parameter: BookmarkFolder * parent父节点
		void DeleteChildren(BookmarkFolder* parent);

		// Summary  : 更新书签
		// Returns	: bool
		// Parameter: BookmarkData * pNode
		bool UpdateNode(BookmarkData* pNode);

		// Summary  : 添加书签
		// Returns	: bool
		// Parameter: BookmarkData * pNewNode新节点
		bool InserBookMarkNode(BookmarkData* pNewNode);
	};

}