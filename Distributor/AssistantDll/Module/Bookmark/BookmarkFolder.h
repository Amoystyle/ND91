#pragma once
#include "Module/Bookmark/BookmarkData.h"

namespace ND91Assistant
{
	//class BookmarkData;
	class BookmarkFolder : public BookmarkData
	{
		friend class IosBookmarkDB;
		friend class IosBookmarkPlistHelper;
		friend class BookmarksSynchronizer;
	public:
		BookmarkFolder();
		BookmarkFolder(map<string,void*> mapEntities);
		virtual ~BookmarkFolder();

		vector<BookmarkData*>* GetAllData() {return _pVerBookmarks;}

		BookmarkData* FindtheData( string id );

		void DeleteFromList(string id);

		void insterData(BookmarkData* pBookmark);

		void Release();

	private:

		vector<BookmarkData*>* _pVerBookmarks;
    
	};

}