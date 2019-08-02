#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "BookmarkFolder.h"

BookmarkFolder::BookmarkFolder()
{
    _type				= TypeBookMarkFolder;
	_recordEntityName	= "com.apple.bookmarks.Folder";
    _pVerBookmarks		= new vector<BookmarkData*>;
}

BookmarkFolder::BookmarkFolder(map<string,void*> mapEntities)
:BookmarkData(mapEntities)
{
	_type				= TypeBookMarkFolder;
	_recordEntityName	= "com.apple.bookmarks.Folder";
	_pVerBookmarks		= new vector<BookmarkData*>;
}

BookmarkFolder::~BookmarkFolder()
{
	Release();
	SAFE_DELETE(_pVerBookmarks);
}

void BookmarkFolder::insterData( BookmarkData* pBookmark)
{
	_pVerBookmarks->push_back( pBookmark );
}

BookmarkData* BookmarkFolder::FindtheData( string id )
{
	vector<BookmarkData*>::iterator it = _pVerBookmarks->begin();
	for ( ; it!= _pVerBookmarks->end(); it++ )
	{
		if((*it)->GetType()==TypeBookMarkFolder)
		{
			BookmarkFolder* fold = dynamic_cast<BookmarkFolder*>((*it));
			BookmarkData* data= fold->FindtheData(id);
			if(data)
				return data;
		}
		else
		{
			if ( (*it)->GetId() == id )
			{
				return (*it);
			}
		}
	}
	return NULL;
}

void BookmarkFolder::DeleteFromList( string id )
{
	for ( vector<BookmarkData*>::iterator it = _pVerBookmarks->begin(); it != _pVerBookmarks->end(); it++ )
	{
		if ( (*it)->GetId() == id )
		{
			//SAFE_DELETE((*it));
			_pVerBookmarks->erase(it);
			break;
		}
	}
}


void BookmarkFolder::Release()
{
	RELEASE_VECTOR((*_pVerBookmarks));
}

#endif
