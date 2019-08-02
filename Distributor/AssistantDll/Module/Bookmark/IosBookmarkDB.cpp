#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/IosBookmarkDB.h"
#include "Module/Bookmark/BookmarkData.h"
#include "Module/Bookmark/BookmarkFolder.h"

IosBookmarkDB::IosBookmarkDB(IosDBPath iosDBPath)
:IosDBBase(iosDBPath)
{

}

IosBookmarkDB::~IosBookmarkDB(void)
{
}

const char* fields[]=
{
	"id",
	"special_id",
	"parent",
	"type",
	"title",
	"url",
	"num_children",
	"editable",
	"deletable",
	"order_index",
	"external_uuid"
};

BookmarkFolder* ND91Assistant::IosBookmarkDB::GetAllBookMark(const DeviceData* pDevice)
{
	BookmarkFolder* rootfolder	= NULL;
	CppSQLite3Buffer sql;
	sql.Format("select * from bookmarks");
	CppSQLite3Query q = ExecQuery((const char*)sql);
	map<string, BookmarkData*> nodes;

	while (!q.Eof())
	{
		string guid, parentid, External_UUID;
		wstring title, url;
		BookMarkType type = TypeBookMark;
		int childrenCount = 0;
		bool editable = true;
		bool deleteable = true;
		int indexorder = 0;

		for (int i = 0; i < 11; i++)
		{
			const char* iosData = q.GetStringField(fields[i]);
			if (!iosData)
				continue;
			switch(i)
			{
			case 0:
				guid = iosData;
				break;
			case 1:
				break;
			case 2:
				parentid= iosData;
				break;
			case 3:
				type = strcmp(iosData,"0")==0? TypeBookMark:TypeBookMarkFolder;
				break;
			case 4:
				title = CCodeOperation::UTF_8ToUnicode(iosData);
				break;
			case 5:
				url = CCodeOperation::UTF_8ToUnicode(iosData);
				break;
			case 6:
				childrenCount = atoi(iosData);
				break;
			case 7:
				editable = strcmp(iosData,"0")==0?false:true;
				break;
			case 8:
				deleteable = strcmp(iosData,"0")==0?false:true;
				break;
			case 9:
				indexorder = atoi(iosData);
				break;
			case 10:
				{
					External_UUID = iosData;
					if(CStrOperation::toLower(iosData) == "root")
						parentid = "-1";
				}
				break;
			}
		}

		BookmarkData* node			= NULL;
		if(type == TypeBookMarkFolder)
		{
			rootfolder = new BookmarkFolder();
		}
		else
		{
			node = new BookmarkData();
		}
		node->_id = guid;
		node->_parentid = parentid;
		node->_title = title;
		node->_url = url;
		node->_id = indexorder;
		node->_editable = editable;
		node->_deleteable = deleteable;
		node->_external_guid = External_UUID;
		if (CStrOperation::toLower(External_UUID) == "root")
			rootfolder = dynamic_cast<BookmarkFolder*>(node);
		nodes[node->_id] = node;
		map<string, BookmarkData*>::iterator it = nodes.find(node->_parentid);
		if(it != nodes.end())
		{
			BookmarkFolder* folder = dynamic_cast<BookmarkFolder*>(it->second);
			folder->insterData(node);
		}
		else if(rootfolder && node->_parentid != "-1")
			rootfolder->insterData(node);

		q.NextRow();
	}
	if(!rootfolder)
	{
		rootfolder = new BookmarkFolder();
		rootfolder->_external_guid = "root";
		rootfolder->_parentid = "-1";
		InserBookMarkNode(rootfolder);
		rootfolder->_external_guid = "root";
	}
	return rootfolder;
}

void ND91Assistant::IosBookmarkDB::DeleteChildren( BookmarkFolder* parent )
{
	for (size_t i = 0; i < parent->_pVerBookmarks->size(); i++)
	{
		BookmarkData* node = parent->_pVerBookmarks->at(i);
		if (node->_type == TypeBookMarkFolder)
			DeleteChildren(dynamic_cast<BookmarkFolder*>(node));

		string guid = node->_id;
		CppSQLite3Buffer sql;
		sql.Format( "Update bookmarks set num_children=num_children-1 where id=(select parent from bookmarks where id= '%s') ;\
					delete from [bookmarks] where id='%s'", 
					guid.c_str(), guid.c_str());

		ExecDML((const char*)sql);
	}
}

bool ND91Assistant::IosBookmarkDB::UpdateNode( BookmarkData* pNode )
{
	string type = "0";
	string childrenCount = "0";
	if(pNode->GetType() == TypeBookMarkFolder)
	{
		BookmarkFolder* folder = dynamic_cast<BookmarkFolder*>(pNode);
		childrenCount = CStrOperation::Int2string(folder->_pVerBookmarks->size());
		type = "1";
	}
	string orderIndex= CStrOperation::Int2string(pNode->_position);
	string parentId = pNode->_parentid;
	string tittle = MyCodeOperation::UnicodeToUTF_8SQL(pNode->_title);
	string url = MyCodeOperation::UnicodeToUTF_8SQL(pNode->_url);
	string guid = pNode->_id;

	CppSQLite3Buffer sql;
	sql.Format("update [bookmarks] set   parent='%s',type='%s',title='%s',url='%s',\
				 num_children='%s',order_index='%s' where id='%s'", 
				parentId.c_str(), type.c_str(), tittle.c_str(), url.c_str(),
				childrenCount.c_str(), orderIndex.c_str(), guid.c_str());
	ExecDML((const char*)sql);
	return true;
}

bool ND91Assistant::IosBookmarkDB::InserBookMarkNode(BookmarkData* pNewNode)
{
	string parentId = pNewNode->_parentid.empty()?"0":pNewNode->_parentid;
	string tittle = MyCodeOperation::UnicodeToUTF_8SQL(pNewNode->_title);
	string url = MyCodeOperation::UnicodeToUTF_8SQL(pNewNode->_url);
	string type = pNewNode->GetType() == TypeBookMarkFolder?"1":"0";
	string delable = "1";
	string editable = "1";
	string External_UUID = pNewNode->_external_guid;
	string indexorder = "0";

	CppSQLite3Buffer sql;
	sql.Format("Insert Into [bookmarks] ([special_id],[parent],[type],[title],[url],[num_children],\
				[editable],[deletable],[order_index],[external_uuid])\
				values ('%s', '%s', '%s', '%s','%s','%s','%s','%s','%s','%s');\
				 Update bookmarks set num_children=num_children+1 where id=%s;\
				 Update bookmarks set order_index = 1 where id= %s;",
				"0", parentId.c_str(), type.c_str(), tittle.c_str(), url.c_str(),
				"0", editable.c_str(), delable.c_str(), indexorder.c_str(), External_UUID.c_str(),
				parentId.c_str(), "0");

	int result = ExecDML((const char*)sql);
	
	if(result >= 0)
	{
		sql.Clear();
		sql.Format("select * from bookmarks where url='%s'  and  type='%s'  and  title='%s'  and  external_uuid='%s'", 
					url.c_str(), type.c_str(), tittle.c_str(), External_UUID.c_str());

		CppSQLite3Query q = ExecQuery((const char*)sql);

		string guid;
		while (!q.Eof())
		{
			guid = q.GetStringField("id");
			q.NextRow();
		}
		pNewNode->_external_guid = pNewNode->_id;
		pNewNode->_id = guid;
		pNewNode->_deleteable = false;
		return true;
	}
	return false;

}

void ND91Assistant::IosBookmarkDB::DeleteNode( BookmarkData* pNode )
{
	if (pNode->_type == TypeBookMarkFolder)
		DeleteChildren(dynamic_cast<BookmarkFolder*>(pNode));

	string guid = pNode->_id;
	CppSQLite3Buffer sql;
	sql.Format( "Update bookmarks set num_children=num_children-1 where id=(select parent from bookmarks where id= '%s') ;\
				 delete from [bookmarks] where id='%s'", 
				 guid.c_str(), guid.c_str());
	ExecDML((const char*)sql);
}


#endif