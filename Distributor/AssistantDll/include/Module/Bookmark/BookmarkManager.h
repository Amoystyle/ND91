#pragma once
#include "Module/IManager.h"

#include "Module/Bookmark/BookmarkData.h"

namespace ND91Assistant
{
	class DeviceData;
	class BookmarkData;
	class BookmarkFolder;
	class CLASSINDLL_CLASS_DECL BookmarkManager : public IManager
	{
	public:
		BookmarkManager(const DeviceData* pDevice);
		virtual ~BookmarkManager(void);

		virtual BookmarkFolder* LoadBookMarks() = 0;

		// Summary  : 添加书签
		// Returns	: bool
		// Parameter: wstring tittle 标题
		// Parameter: wstring url    网址, Ios可空，表示添加文件夹
		// Parameter: wstring des	 描述
		virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url=L"", wstring des=L"") = 0;

		// Summary  : 导入书签
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder) = 0;

		// Summary  : 删除书签
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool DeleteBookMark(BookmarkData* pBookmark) = 0;

		// Summary  : 更新书签
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool UpdateBookMark(BookmarkData* pBookmark) = 0;

		// Summary  : 同步到手机，仅根节点操作
		// Returns	: bool
		virtual bool SyncToPhone() = 0;

	public:
		// Summary  : 从列表中移除
		// Returns	: void
		// Parameter: BookmarkData * pBookmark
		void RemoveFromList(string folderId);

		// 根据分类ID找到该分类指针
		BookmarkFolder* FindtheFolder(BookmarkData* pBookmark, BookmarkFolder* pRoot = NULL);


	protected:

		//const DeviceData*		_pDevice;

		BookmarkFolder*			_pBookmarkRoot;

	};

}