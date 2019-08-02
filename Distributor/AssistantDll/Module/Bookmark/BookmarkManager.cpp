#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/BookmarkManager.h"
#include "Module/Bookmark/BookmarkFolder.h"

BookmarkManager::BookmarkManager(const DeviceData* pDevice)
: IManager (pDevice)
{
	_pBookmarkRoot = new BookmarkFolder;
}

BookmarkManager::~BookmarkManager(void)
{
	SAFE_DELETE(_pBookmarkRoot);
}

BookmarkFolder* BookmarkManager::FindtheFolder(BookmarkData* pBookmark, BookmarkFolder* pRoot)
{
	if ( !pRoot )
		pRoot = _pBookmarkRoot;
	
	for (vector<BookmarkData*>::iterator it = pRoot->GetAllData()->begin(); it != pRoot->GetAllData()->end(); it++)
	{
		if ( (*it)->GetParentId() == pBookmark->GetParentId() )
		{
			return pRoot;
		}

		BookmarkFolder* pFolder = dynamic_cast<BookmarkFolder*>( (*it) );
		if ( pFolder )
		{
			BookmarkData* pdata = pFolder->FindtheData(pBookmark->GetId());
				return pFolder;
		}
	}
	return NULL;
}

void BookmarkManager::RemoveFromList( string folderId )
{
// 	for (vector<BookmarkFolder*>::iterator it = _pVerbookMarks->begin(); it != _pVerbookMarks->end(); it++)
// 	{
// 		if( (*it)->GetId() == folderId )
// 		{
// 			SAFE_DELETE((*it));
// 			_pVerbookMarks->erase(it);
// 			return;
// 		}
// 	}
}

#endif