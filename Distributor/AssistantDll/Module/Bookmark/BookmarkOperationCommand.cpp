#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "BookmarkOperationCommand.h"
#include "BookmarkFolder.h"
#include "Module/AndroidDefine.h"

#define  BookMarkCategory_SystemFavorite   L"My favorites"

BookmarkOperationCommand::BookmarkOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = BOOKMARK;

	_pSourceBookMark = NULL;
	_pSrcBookMarkCategory = NULL;
	_pAddBookMarkCategory = NULL;

	_pBookMarkRoot = new BookmarkFolder;
}

BookmarkOperationCommand::~BookmarkOperationCommand(void)
{
	_pSourceBookMark = NULL;
	_pSrcBookMarkCategory = NULL;
}

void ND91Assistant::BookmarkOperationCommand::Execute()
{
	_success = false;
	DefaultExecute();
}

std::string ND91Assistant::BookmarkOperationCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter .WriteInt(_nAndroidCommandID);
	switch(_nAndroidCommandID)
	{
		case Action_LoadBookMark:
			break;
		case Action_AddBookMark:
			_pSourceBookMark->AndroidDataToStream(bufwriter, GetDeviceInfo()->_platform == Platform_OMS);
			break;
		case Action_DeleteBookMark:	
			bufwriter.WriteInt( atoi(_pSourceBookMark->_id.c_str()) );
			break;
		case Action_UpdateBookMark: 
			_pSourceBookMark->AndroidDataToStream(bufwriter, GetDeviceInfo()->_platform == Platform_OMS);
			break;
		case Action_AddCategory:
			bufwriter.WriteStringAsUTF8(_pAddBookMarkCategory->_title);
			break;
		case Action_UpdateCategory:	 
			bufwriter.WriteInt( atoi(_pSrcBookMarkCategory->_id.c_str()) );
			bufwriter.WriteStringAsUTF8(_pSrcBookMarkCategory->_title);
			break;
		case Action_DeleteCategory:	
			bufwriter.WriteInt( atoi(_pSrcBookMarkCategory->_id.c_str()) );
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::BookmarkOperationCommand::SetAndroidReturnData( std::string data )
{
	int temp = 0;
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	switch(_nAndroidCommandID)
	{
		case Action_LoadBookMark:
			{
				_pBookMarkRoot->Release();
				if(GetDeviceInfo()->_platform == Platform_OMS)
				{
					int favoriteCatID = atoi(Default_BookMark_CategoryId);
					int catCount = 0;
					reader.ReadInt32(catCount);
					for (int i =0; i < catCount; i++)
					{
						BookmarkFolder* pFolder = new BookmarkFolder();
						reader.ReadInt32( temp );
						pFolder->_id = CStrOperation::Int2string(temp);
						reader.ReadStringFromUTF8(pFolder->_title);
						ParseAndroidBookMarks(reader, pFolder);
						_pBookMarkRoot->insterData( (BookmarkData*)pFolder );

						if(BookMarkCategory_SystemFavorite == pFolder->_title && atoi(pFolder->_id.c_str()) < favoriteCatID)
						{
							favoriteCatID = atoi(pFolder->_id.c_str());
						}
					}
					if(atoi(Default_BookMark_CategoryId) == favoriteCatID)
					{
						BookmarkFolder* pFolder = new BookmarkFolder();
						pFolder->_title = BookMarkCategory_SystemFavorite;
						if(AddCategory(pFolder))
						{
							_pBookMarkRoot->insterData( (BookmarkData*)pFolder );
						}
						else
							SAFE_DELETE(pFolder);
					}
				}
				else
				{
					BookmarkFolder* pFolder = new BookmarkFolder();
					pFolder->_id = Default_BookMark_CategoryId;
					pFolder->_title = L"android";
					ParseAndroidBookMarks(reader, pFolder);
					_pBookMarkRoot->insterData( (BookmarkData*)pFolder );
				}
			}
			break;
		case Action_AddBookMark:
			{
				reader.ReadBool(_success);
				if(_success)
				{
					reader.ReadInt32( temp );
					_pSourceBookMark->_id = CStrOperation::Int2string(temp);
				}
			}
			break;
		case Action_AddCategory:
			{
				reader.ReadBool(_success);
				if(_success)
				{
					reader.ReadInt32( temp );
					_pAddBookMarkCategory->_id = CStrOperation::Int2string(temp);	
				}
			}
			break;
		case Action_DeleteBookMark:	
		case Action_UpdateBookMark: 
		case Action_UpdateCategory:	 
		case Action_DeleteCategory:	
			reader.ReadBool(_success);
			break;
		default:
			break;
	}
}



void BookmarkOperationCommand::ParseAndroidBookMarks(BinaryBufferReader& reader,BookmarkFolder* pFolder)
{
	RETURN_IF(!pFolder);
	int markCount = 0;
	reader.ReadInt32(markCount);
	for (int i = 0; i < markCount; i++)
	{
		BookmarkData* pBookmark = new BookmarkData();
		pBookmark->ParseAndroidData(reader, GetDeviceInfo()->_platform == Platform_OMS);
		pFolder->insterData(pBookmark);
	}
}

BookmarkFolder* ND91Assistant::BookmarkOperationCommand::LoadBookMarks()
{
	_nAndroidCommandID = Action_LoadBookMark;
	Execute();
	return _pBookMarkRoot;
}

bool ND91Assistant::BookmarkOperationCommand::AddBookMark( BookmarkData* pBookmark )
{
	RETURN_FALSE_IF(!pBookmark);
	_nAndroidCommandID = Action_AddBookMark;
	_pSourceBookMark = pBookmark;
	Execute();
	return _success;
}

bool ND91Assistant::BookmarkOperationCommand::DeleteBookMark( BookmarkData* pBookmark )
{
	RETURN_FALSE_IF(!pBookmark);
	_nAndroidCommandID = Action_DeleteBookMark;
	_pSourceBookMark = pBookmark;
	Execute();
	return _success;
}

bool ND91Assistant::BookmarkOperationCommand::UpdateBookMark( BookmarkData* pBookmark )
{
	RETURN_FALSE_IF(!pBookmark);
	_nAndroidCommandID = Action_UpdateBookMark;
	_pSourceBookMark = pBookmark;
	Execute();
	return _success;
}

bool ND91Assistant::BookmarkOperationCommand::AddCategory( BookmarkFolder* pFolder )
{
	RETURN_FALSE_IF(!pFolder);
	_nAndroidCommandID = Action_AddCategory;
	_pAddBookMarkCategory = pFolder;
	Execute();
	return _success;
}

bool ND91Assistant::BookmarkOperationCommand::UpdateCategory(const BookmarkFolder* pFolder )
{
	RETURN_FALSE_IF(!pFolder);
	_nAndroidCommandID = Action_UpdateCategory;
	_pSrcBookMarkCategory = pFolder;
	Execute();
	return _success;
}

bool ND91Assistant::BookmarkOperationCommand::DeleteCategory(const BookmarkFolder* pFolder )
{
	RETURN_FALSE_IF(!pFolder);
	_nAndroidCommandID = Action_DeleteCategory;
	_pSrcBookMarkCategory = pFolder;
	Execute();
	return _success;
}


#endif