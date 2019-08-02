#pragma once
#include "Module/Bookmark/BookMarkManager.h"

#define  BookMarkCategory_SystemFavorite   L"My favorites"
namespace ND91Assistant
{
    class BookmarkData;
    class DeviceData;
	class BookmarkOperationCommand;
    //  ��ǩ����
    class CLASSINDLL_CLASS_DECL AndroidBookmarkManager : public BookmarkManager
    {
    public:
	    AndroidBookmarkManager(const DeviceData* pDevice);
	    virtual ~AndroidBookmarkManager(void);

		// ����������ǩ����
		virtual BookmarkFolder* LoadBookMarks();

	    // Summary  : �����ǩ
	    // Returns	: bool
	    // Parameter: wstring tittle ����
	    // Parameter: wstring url    ��ַ
	    // Parameter: wstring des	 ����
	    virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url, wstring des=L"");

	    // Summary  : ������ǩʹ��
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark ���ⲿ�ͷ�
	    virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder);

	    // Summary  : ɾ����ǩ
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark
	    virtual bool DeleteBookMark(BookmarkData* pBookmark);

	    // Summary  : ������ǩ
	    // Returns	: bool
	    // Parameter: BookmarkData * pBookmark
	    virtual bool UpdateBookMark(BookmarkData* pBookmark);

		// Summary  : ͬ�����ֻ��������ڵ����
		// Returns	: bool
		virtual bool  SyncToPhone() { return true;}

	private:

		BookmarkOperationCommand*	_pBookmarkOper;
    };

}
