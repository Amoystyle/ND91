#pragma once
#include "Module/Bookmark/BookMarkManager.h"

#define  BookMarkCategory_SystemFavorite   L"My favorites"
namespace ND91Assistant
{
    class BookmarkData;
    class DeviceData;
	class BookmarkOperationCommand;
    //  书签分类
    class CLASSINDLL_CLASS_DECL AndroidBookmarkManager : public BookmarkManager
    {
    public:
	    AndroidBookmarkManager(const DeviceData* pDevice);
	    virtual ~AndroidBookmarkManager(void);

		// 加载所有书签数据
		virtual BookmarkFolder* LoadBookMarks();

	    // Summary  : 添加书签
	    // Returns	: bool
	    // Parameter: wstring tittle 标题
	    // Parameter: wstring url    网址
	    // Parameter: wstring des	 描述
	    virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url, wstring des=L"");

	    // Summary  : 导入书签使用
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark 由外部释放
	    virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder);

	    // Summary  : 删除书签
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark
	    virtual bool DeleteBookMark(BookmarkData* pBookmark);

	    // Summary  : 更新书签
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark
	    virtual bool UpdateBookMark(BookmarkData* pBookmark);

		// Summary  : 同步到手机，仅根节点操作
		// Returns	: bool
		virtual bool  SyncToPhone() { return true;}

	private:

		BookmarkOperationCommand*	_pBookmarkOper;
    };

}
