#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/IosBookmarkManager.h"
#include "Module/Bookmark/BookmarksSynchronizer.h"
#include "Module/Bookmark/IosBookmarkDB.h"
#include "Module/Bookmark/BookmarkFolder.h"

IosBookmarkManager::IosBookmarkManager(const DeviceData* pDevice)
:BookmarkManager(pDevice) 
{
	_version = IosPublicFunc::GetVerNum(pDevice->GetDevInfo()->_deviceProductVersion);
	_pBookmarkSync = new BookmarksSynchronizer(pDevice);	

}


// IosBookmarkManager::IosBookmarkManager(map<string,void*> mapEntities,const DeviceData* pDevice) : BookmarkData(mapEntities)
// {
//     _pDevice = (DeviceData*)pDevice;
//     _IsAndroid = _pDevice->GetDevInfo()->_deviceType == Android;
//     _version = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
// 
//     _type               = TypeBookMarkFolder;
//     _recordEntityName   = "com.apple.bookmarks.Folder";
// }

IosBookmarkManager::~IosBookmarkManager(void)
{

}

BookmarkFolder* IosBookmarkManager::LoadBookMarks()
{
	_pBookmarkSync->LoadAllData();
	_pBookmarkRoot = _pBookmarkSync->GetBookMarkFolder();
	return _pBookmarkRoot;
}

BookmarkData* IosBookmarkManager::AddBookMark( wstring tittle, BookmarkFolder* pFolder, wstring url, wstring des/*=L""*/ )
{
    BookmarkData* pNode = _pBookmarkSync->AddBookMark(tittle,url,pFolder->GetId());
	pNode->SetId(CFileOperation::GetGuid());

// 	if ( pNode->GetType() == TypeBookMarkFolder	)
// 	{
// 		BookmarkFolder* folder = dynamic_cast<BookmarkFolder*>(pNode);
// 		vector<BookmarkData*>* pVerData = folder->GetAllData();
// 		for (size_t i = 0; i < pVerData->size(); i++)
// 		{
// 			pFolder->insterData(pVerData->at(i));
// 		}
// 	}
// 	else
	pFolder->insterData(pNode);
	
    return pNode;
/*
		BookmarkData* pNode = NULL;
		if (url.empty())
			pNode = new IosBookmarkManager(_pDevice);
		else
			pNode = new BookmarkData();
		pNode->_title = tittle;
		pNode->_url = url;
		pNode->SetIosParentGuid(_ios_Guid);
		if(_version >= 320)
		{
			if(!DBOperation(EAdd,pNode))
			{
				SAFE_DELETE(pNode);
				return NULL;
			}
			pNode->SetId(GetChildOrderIndex());
		}
		_bookMarks.push_back(pNode);
		return pNode;
*/
}

bool IosBookmarkManager::ImportBookMark( BookmarkData* pBookmark, BookmarkFolder* pFolder )
{
//	DeleteBookMark(FindNode(pBookmark));
// 	pNewNode->SetParentId(folderId);
// 	if(_version >= 320)
// 	{
// 		if(!DBOperation(EAdd,pBookmark))
// 		{
// 			SAFE_DELETE(pNewNode);
// 			return NULL;
// 		}
// 		pNewNode->SetId(GetChildOrderIndex());
// 
// 		if(pNewNode->GetType() == TypeBookMarkFolder)
// 		{
// 			BookmarkFolder* folder = dynamic_cast<BookmarkFolder*>(pNewNode);
// 			for (size_t i = 0; i < folder->_pBookmarkFolder->size(); i++)
// 			{
// 				pFolder->insterData(folder->_pBookmarkFolder->at(i));
// 			}
// 		}
// 	}
// 	_bookMarks.push_back(pNewNode);

	BookmarkFolder* pf = dynamic_cast<BookmarkFolder*>(pBookmark);

	if ( pf )
	{
		BookmarkFolder* newfolder = dynamic_cast<BookmarkFolder*>( AddBookMark( pBookmark->GetTitle(), pFolder ) );
		RETURN_FALSE_IF(!newfolder);
		vector<BookmarkData*>* pVerData = pf->GetAllData(); 

		for (vector<BookmarkData*>::iterator it = pVerData->begin(); it != pVerData->end(); it++)
		{	
			ImportBookMark( (*it),	newfolder );
		}
	} 
	else
	{
		RETURN_FALSE_IF ( !AddBookMark( pBookmark->GetTitle(), pFolder, pBookmark->GetUrl() ) );
	}	

	return true;
}

bool IosBookmarkManager::DeleteBookMark( BookmarkData* pBookmark )
{
	RETURN_FALSE_IF(!pBookmark);

    _pBookmarkSync->DelBookMark(pBookmark);
	
	BookmarkFolder* folder = FindtheFolder( pBookmark );
	folder->DeleteFromList(pBookmark->GetId());


    return true;
/*
		if(_version >= 320)
		{
			DBOperation(EDelete, pBookmark);
		}
		RemoveFromList(pBookmark);
		return true;
*/
}

bool IosBookmarkManager::UpdateBookMark( BookmarkData* pBookmark )
{


    _pBookmarkSync->EditBookMark(pBookmark, -1);
    return true;

// 	if(_version >= 320)
// 	{
// 		DBOperation(EUpdate, pBookmark);
// 	}
// 	return true;


}

int IosBookmarkManager::GetChildOrderIndex()
{
	int index = 0;
// 	for(size_t i = 0; i < _bookMarks.size(); i++)
// 	{
// 		if(_bookMarks[i]->GetId() > index)
// 			index = _bookMarks[i]->GetId();
// 	}
	return index+1;
}

bool IosBookmarkManager::DBOperation( DBOperateType type, BookmarkData* pBookmark )
{
	RETURN_FALSE_IF(_version < 320);
	IosBookmarkDB db(IosDBPath(_pDevice, ISBOOKMARKDB));
	RETURN_FALSE_IF(!db.OpenDB());
	bool success = false;
	switch(type)
	{
		case EDelete:
			{
				db.DeleteNode(pBookmark);
				success = true;
			}
			break;
		case EUpdate:
			success = db.UpdateNode(pBookmark);
			break;
		case EAdd:
			success = db.InserBookMarkNode(pBookmark);
			break;
	}
	db.Commit();
	db.CloseDB();
	return success;
}

bool IosBookmarkManager::SyncToPhone()
{

    return _pBookmarkSync->SaveChangesBookMarks();
/*
	IosDBPath path(_pDevice, ISBOOKMARKDB);
	DirectoryOperationCommand dircmd(_pDevice);
	wstring plistpath = path.GetDBFolder()+ ((_version < 320)?L"Bookmarks.plist":L"Bookmarks.db");
	wstring localpath = path.GetTempFolder()+((_version < 320)?L"Bookmarks.plist":L"Bookmarks.db");
	if(_version < 320)
	{
		IosBookMarkPlistHelper::SaveBookMarkToPlist(localpath, this);
	}
	
	dircmd.DeleteFileLink(plistpath);
	if(dircmd.AndroidCreateDir(path.GetDBFolder()))
	{
		UploadFileCommand cmd(_pDevice);
		if(cmd.Upload(localpath, plistpath))
		{
			dircmd.ChangeMode(path.GetDBFolder());
            DeviceData* pdata = (DeviceData*)_pDevice;
            RETURN_FALSE_IF(!pdata->GetCommands()->LogoffDev());
			return true;
		}
	}
	return false;
*/
}

bool IosBookmarkManager::MoveBookMarkNode( BookmarkData* pChildBookMark, bool Up )
{
	BookmarkData* find = NULL; 
	int index = -1;
	
	BookmarkFolder *pFolder = FindtheFolder( pChildBookMark );
	vector<BookmarkData*>* pVerData = pFolder->GetAllData();

	for (size_t i = 0; i < pVerData->size(); i++)
	{
		if ( pChildBookMark->GetId() == pVerData->at(i)->GetId() )
		{
			find = pVerData->at(i);
			index = i;
			break;
		}
	}

	RETURN_FALSE_IF(!find || (Up && index==0) || (!Up && index == pVerData->size()-1));
/*
	int changerIndex = Up?index-1:index+1;
	BookmarkData* changer = _bookMarks[changerIndex];
	if(_version > 320)
	{
		int find_Id = find->GetId();
		find->SetId(changer->GetId());
		changer->SetId(find_Id);
		DBOperation(EUpdate, find);
		DBOperation(EUpdate, changer);
	}
*/

    int changerIndex = Up?index-1:index+1;
    _pBookmarkSync->EditBookMark(pChildBookMark,changerIndex);

    BookmarkData* changer = pVerData->at(changerIndex);
	pVerData->at(index)= changer;
	pVerData->at(changerIndex) = find;

	return true;
}



#endif