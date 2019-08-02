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

		// Summary  : �����ǩ
		// Returns	: bool
		// Parameter: wstring tittle ����
		// Parameter: wstring url    ��ַ, Ios�ɿգ���ʾ����ļ���
		// Parameter: wstring des	 ����
		virtual BookmarkData* AddBookMark(wstring tittle, BookmarkFolder* pFolder, wstring url=L"", wstring des=L"") = 0;

		// Summary  : ������ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool ImportBookMark(BookmarkData* pBookmark, BookmarkFolder* pFolder) = 0;

		// Summary  : ɾ����ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool DeleteBookMark(BookmarkData* pBookmark) = 0;

		// Summary  : ������ǩ
		// Returns	: bool
		// Parameter: BookmarkData * pBookmark
		virtual bool UpdateBookMark(BookmarkData* pBookmark) = 0;

		// Summary  : ͬ�����ֻ��������ڵ����
		// Returns	: bool
		virtual bool SyncToPhone() = 0;

	public:
		// Summary  : ���б����Ƴ�
		// Returns	: void
		// Parameter: BookmarkData * pBookmark
		void RemoveFromList(string folderId);

		// ���ݷ���ID�ҵ��÷���ָ��
		BookmarkFolder* FindtheFolder(BookmarkData* pBookmark, BookmarkFolder* pRoot = NULL);


	protected:

		//const DeviceData*		_pDevice;

		BookmarkFolder*			_pBookmarkRoot;

	};

}