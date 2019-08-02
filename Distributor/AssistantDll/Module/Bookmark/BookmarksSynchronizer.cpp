#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "Module/Bookmark/BookmarksSynchronizer.h"
#include "Module/Bookmark/BookmarkData.h"
#include "Module/Bookmark/BookmarkFolder.h"

#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

#define DATACLASSVERSION   102  // 数据类型在PC上的版本号

string BookmarksSynchronizer::BOOKMARK_ENTITY_NAME  = "com.apple.bookmarks.Bookmark";
string BookmarksSynchronizer::FOLDER_ENTITY_NAME    = "com.apple.bookmarks.Folder";

BookmarksSynchronizer::BookmarksSynchronizer(const DeviceData* pDevice) : SynchronizerBase(pDevice)
{
    _bookMarks.clear();
    _remappedIDs.clear();

    _shortDataClass     = "Bookmarks";
    _dataClass          = "com.apple.Bookmarks";
    _dataClassAnchor    = "Bookmarks-Device-Anchor";
    _dataClassVersion   = DATACLASSVERSION;
}

BookmarksSynchronizer::~BookmarksSynchronizer()
{
    //ClearCachedData();
}

void BookmarksSynchronizer::ConvertBookMarkNodesToBookMarkFolder()
{
    _folder = new BookmarkFolder();
    _folder->SetId("Root");

    // 根目录按照位置排序
    SortRootByPosition();

    vector<BookmarkData*> folders;
    folders.clear();
    // 将书签放入对应的目录下
    PutBookMarksIntoFolders(folders);

    // 子目录按照位置排序
    SortChildByPosition(folders);
}

void BookmarksSynchronizer::SortRootByPosition()
{
    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* pData = _bookMarks.at(i);
        if (pData->_parentid == "Root")
            _folder->insterData(pData);
    }

    if (_folder->_pVerBookmarks->empty())
        return ;

    for (size_t m = _folder->_pVerBookmarks->size()-1; m > 0; m--)
    {
        BookmarkData* temp = NULL;
        for (size_t n = 0; n < m; n++)
        {
            if (_folder->_pVerBookmarks->at(n)->GetPosition() > _folder->_pVerBookmarks->at(n+1)->GetPosition())
            {
                temp = _folder->_pVerBookmarks->at(n);
                _folder->_pVerBookmarks->at(n) = _folder->_pVerBookmarks->at(n+1);
                _folder->_pVerBookmarks->at(n+1) = temp;
            }
        }
    }
}

void BookmarksSynchronizer::PutBookMarksIntoFolders(vector<BookmarkData*>& folders)
{
    vector<BookmarkData*> marks;
    marks.clear();
    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node = _bookMarks.at(i);
        if (node->GetType() == TypeBookMarkFolder)
            folders.push_back(node);
        else
            marks.push_back(node);
    }

    for (size_t i = 0; i < folders.size(); i++)
    {
        BookmarkData* node = folders.at(i);
        BookmarkFolder* tempFolder = dynamic_cast<BookmarkFolder*>(node);
        for (size_t j = 0; j < marks.size(); j++)
        {
            BookmarkData* temp = marks.at(j);
            if (temp->_parentid == tempFolder->_id)
                tempFolder->insterData(temp);
        }
    }

    for (size_t i = 0; i < folders.size(); i++)
    {
        BookmarkData* node1 = folders.at(i);
        BookmarkFolder* tempFolder1 = dynamic_cast<BookmarkFolder*>(node1);
        for (size_t j = 0; j < folders.size(); j++)
        {
            BookmarkData* node2 = folders.at(j);
            BookmarkFolder* tempFolder2 = dynamic_cast<BookmarkFolder*>(node2);
            if (tempFolder1->_id == tempFolder2->_parentid)
                tempFolder1->insterData(node2);
        }
    }
}

void BookmarksSynchronizer::SortChildByPosition(vector<BookmarkData*>& folders)
{
    for (size_t i = 0; i < folders.size(); i++)
    {
        BookmarkFolder* tempFolder = dynamic_cast<BookmarkFolder*>(folders.at(i));
        if (tempFolder->_pVerBookmarks->empty())
            continue;

        for (size_t m = tempFolder->_pVerBookmarks->size()-1; m > 0; m--)
        {
            BookmarkData* temp = NULL;
            for (size_t n = 0; n < m; n++)
            {
                if (tempFolder->_pVerBookmarks->at(n)->GetPosition() > tempFolder->_pVerBookmarks->at(n+1)->GetPosition())
                {
                    temp = tempFolder->_pVerBookmarks->at(n);
                    tempFolder->_pVerBookmarks->at(n) = tempFolder->_pVerBookmarks->at(n+1);
                    tempFolder->_pVerBookmarks->at(n+1) = temp;
                }
            }
        }
    }
}

bool BookmarksSynchronizer::SaveChangesBookMarks()
{
    //_bookMarks = bookMarks;
    if (_bookMarks.empty())
        return true;

    return SaveChanges();
}

void BookmarksSynchronizer::DoSaveChanges()
{
    if (!PackPlistEntities(false, true))
        return ;

    if (!_syncClient->SendProcessChangesToDevice(_sendInfo))
        return ;

    if (!_syncClient->GetRemapIdentifiersFromDevice(_remappedIDs))
        return ;

    //RemapEntitiyIDs();
}

void BookmarksSynchronizer::RemapEntitiyIDs()
{
    // 无返回映射ID
}

void BookmarksSynchronizer::ClearCachedData()
{
//     for (size_t i = 0; i < _bookMarks.size(); i++)
//         SAFE_DELETE(_bookMarks.at(i));
    _bookMarks.clear();
}

void BookmarksSynchronizer::ParsePlistEntities()
{
    for (map<string, map<string,void*>>::iterator it = _mapEntities.begin(); it != _mapEntities.end(); it++)
    {
        string str = "";
        map<string, void*> temp = it->second;
        for (map<string, void*>::iterator iter = temp.begin(); iter != temp.end(); iter++)
        {
            if (iter->first != "com.apple.syncservices.RecordEntityName")
                continue;

            CCFConverter::GetIosData((void*)iter->second, str);
        }

		BookmarkData* node = NULL;
        if (str == BookmarksSynchronizer::BOOKMARK_ENTITY_NAME)
		{
            node = new BookmarkData(it->second);
			
		}
        else
		{
			BookmarkFolder* pFolder = NULL;
            pFolder = new BookmarkFolder(it->second);
// 			pFolder->_id = it->first;
// 			node = new BookmarkData(it->second);
 			node = (BookmarkData*)pFolder;
		}
		
		node->_id = it->first;
        _bookMarks.push_back(node);
    }

    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node = _bookMarks.at(i);
        if (node->_id != "Root" && node->_parentid.empty())
            node->_parentid = "Root";
    }

    ConvertBookMarkNodesToBookMarkFolder();
	//_pVerFolders->push_back(_folder);
}

bool BookmarksSynchronizer::PackPlistEntities(bool isHaveRecord, bool linkAllRecordsOfPulledEntityTypeSentKey)
{
    if (_bookMarks.empty())
        return false;

    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node = _bookMarks.at(i);
        if (node->_parentid == "Root")
            node->SetParentId("");
    }

    ReleaseSendInfo();
    // 开始写数组内容
    _sendInfo.push_back(CCFConverter::StringtoCFString("SDMessageProcessChanges"));
    _sendInfo.push_back(CCFConverter::StringtoCFString(_dataClass));

    // 写实体内容
    map<string, string> dict;
    dict.clear();

    if (_bookMarks.empty())
    {
        _sendInfo.push_back(CCFConverter::DictionarytoCFDictionary(dict));
    }
    else
    {
        //map<string,map<string,void*>> temp;
        map<string, void*> temp;
        temp.clear();
        for (size_t i = 0; i < _bookMarks.size(); i++)
        {
            BookmarkData* mark = _bookMarks.at(i);
            if (mark->GetStatus() == BaseData::NO_CHANGE)
                continue;

            map<string, void*> mapInfo;
            mapInfo.clear();
            mark->ConvertToDictInfos(mapInfo);
            if (mapInfo.empty())// 删除
            {
                temp.insert(pair<string, void*>(mark->_id, CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
                //RemoveFromList(mark);
            }
            else // 添加或修改
            {
                temp.insert(pair<string, void*>(mark->_id,CCFConverter::DictionarytoCFDictionary(mapInfo)));
            }
        }
        _sendInfo.push_back(CCFConverter::DictionarytoCFDictionary(temp));
    }

    // 是否还有数据
    _sendInfo.push_back(CCFConverter::BoolToCFBool(!isHaveRecord));

    // sync actions
    map<string, vector<void*>> dict1;
    dict1.clear();

    vector<void*> linkEntityNamesKey;
    linkEntityNamesKey.clear();
    linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(BOOKMARK_ENTITY_NAME));
    linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(FOLDER_ENTITY_NAME));
    dict1.insert(pair<string,vector<void*>>("SyncDeviceLinkEntityNamesKey",linkEntityNamesKey));

    vector<void*> aaa;
    aaa.clear();
    aaa.push_back(CCFConverter::BoolToCFBool(linkAllRecordsOfPulledEntityTypeSentKey));
    dict1.insert(pair<string,vector<void*>>("SyncDeviceLinkAllRecordsOfPulledEntityTypeSentKey",aaa));

    _sendInfo.push_back(CCFConverter::ConvertStringVectorMap(dict1));

    return true;
}

void BookmarksSynchronizer::DelBookMark(BookmarkData* node)
{
	vector<BookmarkData*> siblings;
	if (node->GetType() == TypeBookMarkFolder)
	{
		BookmarkFolder* pF = dynamic_cast<BookmarkFolder*>(node);
		siblings = *(pF->GetAllData());
	}
	else
	{
		siblings = GetChildBookMarks(GetNodeByKey(node->_id));
	}

    for (size_t i = 0; i < siblings.size(); i++)
    {
        BookmarkData* sibling = siblings.at(i);
        if (sibling->GetPosition() > node->GetPosition() && sibling->GetStatus() != BaseData::DEL)
        {
            int pos = sibling->GetPosition();
            sibling->SetPosition(pos-1);
            if (sibling->GetStatus() == BaseData::NO_CHANGE)
                sibling->SetStatus(BaseData::UPDATE); 
        }
    }
    node->SetStatus(BaseData::DEL);
//     if (node->GetType() == TypeBookMarkFolder)
//     {
//         vector<BookmarkData*> children = GetChildBookMarks(node);
//         for (size_t i = 0; i < children.size(); i++)
//             DelBookMark(children.at(i));
//     }
}

BookmarkData* BookmarksSynchronizer::GetNodeByKey(string strKey)
{
    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node  = _bookMarks.at(i);
        if (node->_id == strKey)
            return node;
    }
    return NULL;
}

vector<BookmarkData*> BookmarksSynchronizer::GetChildBookMarks(BookmarkData* parent)
{
    vector<BookmarkData*> ret;
    ret.clear();
    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node = _bookMarks.at(i);
        if (node->GetStatus() == BaseData::DEL)
            continue;

        if ( (parent == NULL && node->_parentid.empty()) || (parent != NULL && parent->_parentid == node->_parentid))
            ret.push_back(node);
    }
    return ret;
}

BookmarkData* BookmarksSynchronizer::AddBookMark(wstring strName, wstring strUrl,string parentGuid)
{
    BookmarkData* nodeNew = NULL;
    if (strUrl.empty())
        nodeNew = new BookmarkFolder();
    else
    {
        nodeNew = new BookmarkData();
        nodeNew->SetUrl(strUrl);
    }

    if (parentGuid.empty())
        nodeNew->SetParentId("Root");
    else 
        nodeNew->SetParentId(parentGuid);

    nodeNew->SetTitle(strName);
    nodeNew->SetPosition(0);
    
    for (size_t i = 0; i < _bookMarks.size(); i++)
    {
        BookmarkData* node = _bookMarks.at(i);
        if (node->_parentid != nodeNew->_parentid || node->GetStatus() == BaseData::DEL)
            continue;
        if (node->GetPosition() >= nodeNew->GetPosition())
            nodeNew->SetPosition(node->GetPosition()+1);
    }
    _bookMarks.push_back(nodeNew);
    return nodeNew;
}

BookmarkData* BookmarksSynchronizer::EditBookMark(BookmarkData* node, int iNewPos, BookmarkData* newParent)
{
    string oldParentKey = node->_parentid;
    string newParentKey = "";
    if (newParent == NULL)
        newParentKey = node->_parentid;
    else
        newParentKey = newParent->_id;

    int iOldPos = node->GetPosition();

    if (newParent == NULL)
        newParent = GetNodeByKey(node->_parentid);

    // 1.修改父节点
    if (newParent != NULL && node->_parentid != newParent->_id)
    {
        node->SetParentId(newParent->_id);
        if (node->GetStatus() == BaseData::NO_CHANGE)
            node->SetStatus(BaseData::UPDATE);
    }

    vector<BookmarkData*> oldSiblings = GetChildBookMarks(GetNodeByKey(node->_id));
    vector<BookmarkData*> newSiblings /*= GetChildBookMarks(GetNodeByKey(node->_parentid))*/;

    // 2.修改节点序号
    if (newParentKey == oldParentKey && iNewPos == -1)
        iNewPos = iOldPos;

    if (iNewPos < 0)
    {
        if ((int)newSiblings.size() == 0)
            iNewPos = 0;
        else
            iNewPos = (int)newSiblings.size() - 1;
    }

    if (iNewPos != iOldPos)
    {
        node->SetPosition(iNewPos);
        if (node->GetStatus() == BaseData::NO_CHANGE)
            node->SetStatus(BaseData::UPDATE);

		// 3.移动相邻的节点
		for (size_t i = 0; i < oldSiblings.size(); i++)
		{
			BookmarkData* sibling = oldSiblings.at(i);
			if (sibling->_id != node->_id && sibling->GetPosition() == iNewPos)
			{
				int pos = sibling->GetPosition();
				if ( iOldPos < iNewPos )
					sibling->SetPosition(pos-1);
				else
					sibling->SetPosition(pos+1);

				if (sibling->GetStatus() == BaseData::NO_CHANGE)
					sibling->SetStatus(BaseData::UPDATE);

				break;
			}
		}

		for (size_t i = 0; i < newSiblings.size(); i++)
		{
			BookmarkData* sibling = newSiblings.at(i);
			if (sibling->_id != node->_id && sibling->GetPosition() >= iNewPos)
			{
				if (sibling->GetStatus() == BaseData::NO_CHANGE)
					sibling->SetStatus(BaseData::UPDATE);

				int pos = sibling->GetPosition();
				sibling->SetPosition(pos+1);
			}
		}
    }



    // 4.改名
    if (!node->GetTitle().empty())
    {
        if (node->GetStatus() == BaseData::NO_CHANGE)
            node->SetStatus(BaseData::UPDATE);
    }

    // 5.改URL
    if (!node->GetUrl().empty())
    {
        if (node->GetStatus() == BaseData::NO_CHANGE)
            node->SetStatus(BaseData::UPDATE);
    }

    return node;
}

bool BookmarksSynchronizer::IsSupportSynchronizer()
{
    int nVerNum = IosPublicFunc::GetVerNum(_device->GetDeviceInfo()->_deviceProductVersion);
    if (nVerNum >= 500 && _device->GetDeviceInfo()->_connectType == WiFi)
    {
        return false;// NoSupportWIFIConnect
    }
    else if (_device->GetDeviceInfo()->_connectType == USB)
    {
        if (nVerNum >= 500 && IsEnableiCloud())
            return false;// FailToSyncWithDevice
        else if (IsDisableSync(true))
            return false;// FailToSyncWithDevice
    }

    return true;
}


#endif