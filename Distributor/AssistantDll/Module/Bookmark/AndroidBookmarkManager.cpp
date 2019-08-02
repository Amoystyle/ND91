#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/AndroidBookmarkManager.h"
#include "Module/Bookmark/BookmarkOperationCommand.h"
#include "Module/Bookmark/BookmarkFolder.h"

AndroidBookmarkManager::AndroidBookmarkManager(const DeviceData* pDevice)
:BookmarkManager(pDevice)
{
	_pBookmarkOper = new BookmarkOperationCommand(pDevice);
}

AndroidBookmarkManager::~AndroidBookmarkManager(void)
{
	SAFE_DELETE(_pBookmarkOper);
}

BookmarkFolder* AndroidBookmarkManager::LoadBookMarks()
{
	_pBookmarkRoot = _pBookmarkOper->LoadBookMarks();
	return _pBookmarkRoot;
}

BookmarkData* AndroidBookmarkManager::AddBookMark( wstring tittle, BookmarkFolder* pFolder, wstring url, wstring des/*=L""*/ )
{
	BookmarkData* pBookmark = new BookmarkData();
	pBookmark->SetTitle(tittle);
	pBookmark->SetUrl(url);
	pBookmark->_description = des;

	if(_pBookmarkOper->AddBookMark(pBookmark))
	{
		pBookmark->SetParentId(pFolder->GetId());
		pFolder->insterData(pBookmark);
		return pBookmark;
	}
	else
	{
		SAFE_DELETE(pBookmark);
		return NULL;
	}
}

bool AndroidBookmarkManager::ImportBookMark( BookmarkData* pBookmark, BookmarkFolder* pFolder )
{
// 	RETURN_NULL_IF ( !pBookmark);
// 
// 	if(_pBookmarkOper->AddBookMark(pBookmark))
// 	{
// 		pBookmark->SetParentId(pFolder->GetId());
// 		pFolder->insterData(pBookmark);
// 		_bookMarks.push_back(new BookmarkData(pBookmark));
// 		return pBookmark;
// 	}
	BookmarkFolder* pf = dynamic_cast<BookmarkFolder*>(pBookmark);
	RETURN_FALSE_IF (!pf);

	for ( vector<BookmarkData*>::iterator it = pFolder->GetAllData()->begin(); it != pFolder->GetAllData()->end(); it++ )
	{
		BookmarkFolder* newfolder = NULL;
		if ( _pDevice->GetDevInfo()->_platform == Platform_OMS )
		{
			newfolder = dynamic_cast<BookmarkFolder*>( AddBookMark( pf->GetTitle(), pFolder, pf->GetUrl() ) );
			RETURN_FALSE_IF(!newfolder);
		} 
		else
			newfolder = dynamic_cast<BookmarkFolder*>((*it));

		vector<BookmarkData*>* pVerData = pf->GetAllData(); 
		for (vector<BookmarkData*>::iterator it2 = pVerData->begin(); it2 != pVerData->end(); it2++)
		{	
			RETURN_FALSE_IF ( !AddBookMark( (*it2)->GetTitle(), newfolder, (*it2)->GetUrl() ) );
		}
	}

	return true;
}


bool AndroidBookmarkManager::DeleteBookMark( BookmarkData* pBookmark )
{
	BookmarkFolder* pFolder =  dynamic_cast<BookmarkFolder*>(pBookmark);

	if ( pFolder )
	{
		 RETURN_FALSE_IF(!_pBookmarkOper->DeleteBookMark(pFolder));
		 pFolder->DeleteFromList(pBookmark->GetId());
	}
	else
	{
		RETURN_FALSE_IF(!_pBookmarkOper->DeleteBookMark(pBookmark));

		BookmarkFolder* folder = FindtheFolder( pBookmark );
		folder->DeleteFromList(pBookmark->GetId());
	}
	return true;
}

bool AndroidBookmarkManager::UpdateBookMark( BookmarkData* pBookmark )
{
	BookmarkFolder* pFolder =  dynamic_cast<BookmarkFolder*>(pBookmark);

	if ( pFolder )
	{
		return _pBookmarkOper->UpdateBookMark(pFolder);
	}
	else
		return _pBookmarkOper->UpdateBookMark(pBookmark);
}


#endif