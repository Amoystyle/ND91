#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/BookmarkData.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

BookmarkData::BookmarkData()
{
//	_ios_Guid           = CFileOperation::GetGuidW();
//    _title              = L"";
    _position           = 0;
    SetStatus(NEW);
 //   _ios_Parent_Guid    = L"";
    _type               = TypeBookMark;
    _recordEntityName   = "com.apple.bookmarks.Bookmark";

//	_id				= 0;
	_deleteable		= true;
	_editable		= true;
//	_external_guid	= _ios_Guid;
    
	_visits			= 0;		// 访问次数
	_lastDate		= 0;		// 上次访问时间
	_createDate		= 0;		// 创建时间
	_description;				// 书签描述
	_bookmark		= 1;		// 是否显示在手机上
//	_catID			= 0;		// 所属类别的id
}

BookmarkData::~BookmarkData()
{
}

BookmarkData::BookmarkData( const BookmarkData* pMark )
{
	_id = pMark->_id;	        
	_title = pMark->_title;			
	_url = pMark->_url;			
	_visits = pMark->_visits;		
	_lastDate = pMark->_lastDate;		
	_createDate = pMark->_createDate;	
	_description = pMark->_description;	
	_bookmark = pMark->_bookmark;		
	_parentid = pMark->_parentid;			
}

BookmarkData* BookmarkData::Clone()
{
	BookmarkData* node = new BookmarkData();
	node->_title = _title;			// 书签名称
	node->_url = _url;			// 书签url
	node->_parentid = _parentid;

	node->_type = _type;
	node->_id = _id;
	return node;
}

BookmarkData::BookmarkData(map<string,void*> mapEntities)
{
    _type = TypeBookMark;
    for (map<string, void*>:: iterator it = mapEntities.begin(); it != mapEntities.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "name")
            _title = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "position")
            CCFConverter::GetIosData(it->second, _position);
        else if (it->first == "parent")
            _parentid = str;
        else if (it->first == "com.apple.syncservices.RecordEntityName")
            _recordEntityName = str;
        else if (it->first == "url")
            _url = CCodeOperation::GB2312ToUnicode(str);
    }
    SetStatus(NO_CHANGE);
}

void BookmarkData::ConvertToDictInfos(map<string, void*>& infoss)
{
    if (GetStatus() == DEL)
        return ;

    infoss.clear();
    infoss.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY,  CCFConverter::StringtoCFString(_recordEntityName)));
    infoss.insert(pair<string, void*>("name",CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_title))));
    infoss.insert(pair<string, void*>("position", CCFConverter::Int32ToCFInt32(_position) ));
    infoss.insert(pair<string, void*>("url", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_url))));

    vector<void*> parents;
    parents.clear();
    if (!_parentid.empty())
    {
        parents.push_back(CCFConverter::StringtoCFString(_parentid));
        infoss.insert(pair<string, void*>("parent", CCFConverter::ArrayToCFArray(parents)));
    }
}

void BookmarkData::ParseAndroidData( BinaryBufferReader& reader, bool IsOms)
{
	int id = 0;
	reader.ReadInt32(id);
	_id = CStrOperation::Int2string(id);
	reader.ReadStringFromUTF8(_title);
	reader.ReadStringFromUTF8(_url);
	reader.ReadInt32(_visits);
	reader.ReadInt64(_lastDate);
	reader.ReadInt64(_createDate);
	reader.ReadStringFromUTF8(_description);
	reader.ReadInt32(_bookmark);
	if(IsOms)
	{
		int parentid = 0;
		reader.ReadInt32(parentid);
		_parentid = CStrOperation::Int2string(parentid);
	}
}

void BookmarkData::AndroidDataToStream( BinaryBufferWriter& writer, bool IsOms )
{
	writer.WriteInt(atoi(_id.c_str()));
	writer.WriteStringAsUTF8(_title);
	writer.WriteStringAsUTF8(_url);
	writer.WriteInt(_visits);
	writer.WriteInt64(_lastDate);
	writer.WriteInt64(_createDate);
	writer.WriteStringAsUTF8(_description);
	writer.WriteInt(_bookmark);
	if(IsOms)
	{
		writer.WriteInt(atoi(_parentid.c_str()));
	}
}

#endif
