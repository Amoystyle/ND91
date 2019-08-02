#include "stdafx.h"
#include "AblFileOperation.h"
#include "LOGX.h"

vector<NodeData*> AblFileOperation::GetNodes( DirData* pRoot, vector<wstring> devicePathList, bool bFindLinkTarget, 
											 bool bExactMatch, BaseProgressObserver* pProgressObserver )
{
    vector<NodeData*> nodes;
    for (vector<wstring>::iterator it = devicePathList.begin(); it != devicePathList.end(); it++)
    {
		if(pProgressObserver && pProgressObserver->IsCancel())
			break;
        wstring path = *it;
        NodeData* pNode = FileHelper::GetNode( pRoot, path, bFindLinkTarget, bExactMatch );
        if (pNode)
            nodes.push_back(pNode);
    }
    return nodes;
}

TransferResult AblFileOperation::DownloadFile( DeviceData* pDevice, wstring strMobilePath, 
											  wstring strPcDir, BaseProgressObserver* pProgressObserver )
{
    vector<wstring> paths;
    paths.push_back(strMobilePath);

	RootData *pRootdata = pDevice->GetRootData();
	DirData* pRoot = pRootdata->GetDir();
    vector<NodeData*> nodes = AblFileOperation::GetNodes(pRoot, paths, true, false);
    TransferResult result = FileHelper::Download(&nodes, strPcDir, pProgressObserver);
	SAFE_DELETE(pRootdata);
	return result;
}

FileListItem* AblFileOperation::NewItem( NodeData* node, FileListItem* par_item )
{
	RETURN_NULL_IF(!node);
	FileListItem* item = new FileListItem;
	item->nType = (eNodeInfoType)node->_type;
	item->strName = &(node->_name);
	item->nSize = node->_size;
	item->modifyTime = node->_modifyTime;
	item->createTime = node->_createTime;
	item->permissions = &(node->_mode);
	item->owner = &(node->_owner);
	item->group = &(node->_group);
	item->data = (int)node;
	item->parent = par_item;
	node->userdata = (int)item;
	LOGX::Write(L"NewItem set userdata:", *item->strName);

	if(par_item)
	{
		item->hasSubFile = false;
		if(node->_type == NodeData::LINK)
		{
			LinkData* pLink = dynamic_cast<LinkData*> ( node );
			if ( pLink )
				node = pLink->GetTarget();
		}
		if(node && node->_type== NodeData::DIR)
		{
			DirData* dir = (DirData*)node;
			item->hasSubFile = dir->HasSubFile();
			LOGX::Write(L"abl_hasSubFile:", node->GetFullPath());
		}
	}
	return item;
}

FileListItem* AblFileOperation::GetItem( NodeData* node, FileListItem* par_item, bool checkAll )
{
	RETURN_NULL_IF(!node);
	FileListItem* item;
	if(node->userdata) 
		item = (FileListItem*)node->userdata;
	else
		item = NewItem(node, par_item);

	if( !item->status )
	{
		item->status = 1;
		// 注意要先判断 link ， 再判断是文件类型
		if(node->_type == NodeData::LINK)
		{
			LinkData* pLink = dynamic_cast<LinkData*> ( node );
			if ( pLink )
				node = pLink->GetTarget(); // 返回值有三种， 空， fileNode, dirNode. 不会再是link
		}
		if(node && node->_type == NodeData::DIR) 
		{
			item->status = 0;
			DirData* dir = (DirData*)node;
			deque<NodeData*> sub_nodes = dir->GetChildrens();
			item->status = 1;
			for (deque<NodeData*>::iterator it = sub_nodes.begin(); it != sub_nodes.end(); it++)
				item->subitems.push_back(checkAll ? GetItem(*it, item) : NewItem(*it, item));
		}
	}
	return item;
}

// 删除子集和自己。 不操作abi node
void AblFileOperation::ReleaseItem(FileListItem* item)
{
	ClearSubitems(item);
	delete(item);
}
void AblFileOperation::ClearSubitems(FileListItem* item)
{
	for (vector<FileListItem*>::iterator it = item->subitems.begin(); it != item->subitems.end(); it ++)
		ReleaseItem(*it);
	item->subitems.clear();
}
// 完整的删除一个 FileListItem* 
void AblFileOperation::RemoveFromChildrens(FileListItem* item, bool b_release_data_abi)
{
	RETURN_IF(!item);
	NodeData* node = (NodeData*)item->data;
	if(item->parent)
	{
		if(b_release_data_abi) node->GetParent()->RemoveFromChildrens(node); // 会自动调用析构， 然后析构删除子集
		for (vector<FileListItem*>::iterator it = item->parent->subitems.begin(); it != item->parent->subitems.end(); it ++)
		{
			if(item == *it)
			{
				item->parent->subitems.erase(it); 
				ReleaseItem(item);  
				break;
			}
		}
	}
	else
	{
		SAFE_DELETE(node);
		ReleaseItem(item);
	}
}
void AblFileOperation::ReloadData_ABL(NodeData* cur_node)
{
	RETURN_IF(!cur_node);
	if(cur_node->_type == NodeData::LINK)
	{
		LinkData* pLink = dynamic_cast<LinkData*> ( cur_node );
		if ( pLink )
		{	
			NodeData* node = pLink->GetTarget(); 
			if(node)
				node->Refresh(); 
		}
	}
	else
		cur_node->Refresh(); 
}
void AblFileOperation::Refresh(FileListItem* item)
{
	FileListItem* cur_item = item;
	NodeData* cur_data = (NodeData*)cur_item->data;
	AblFileOperation::ClearSubitems(cur_item);
	
	if(cur_data->_type == NodeData::LINK)
	{
		LinkData* pLink = dynamic_cast<LinkData*> ( cur_data );
		if ( pLink )
		{	
			cur_data = pLink->GetTarget(); 
			if(cur_data)
				cur_data->Refresh(); 
		}
	}
	else
		cur_data->Refresh();

	if(cur_data && cur_data->_type == NodeData::DIR) 
	{
		DirData* dir = (DirData*)cur_data;
		deque<NodeData*> sub_nodes = dir->GetChildrens();
		for (deque<NodeData*>::iterator it = sub_nodes.begin(); it != sub_nodes.end(); it++)
			cur_item->subitems.push_back(AblFileOperation::NewItem(*it, cur_item));
	}
}
int AblFileOperation::CheckWritable(DeviceData* pDevice, DirData* data)
{
	if(data)
	{
		if(pDevice->GetdeviceType() == Android)
		{// 目前发现有的设备ios 验证是否可写是否较长时间， 故不验证。
			int res=-1;
			DWORD tw = GetTickCount();
			tstring name = CFileOperation::GetGuidW();
			if(data->MakeSubDir(name))
			{
				NodeData* sub_data = data->findChildrenbyName(name);
				vector<NodeData*> datas;
				datas.push_back(sub_data);
				FileHelper::BatchDelete(&datas, NULL);
				res=0;
			}
			tw = GetTickCount() - tw;
			LOGX::Write(L"验证文件夹是否可写花费时间： ", CStrOperation::IntToWString(tw));
			return res;
		}
		else
			return 0;
	}
	return -1;
}
NodeData* AblFileOperation::GetNode( int data )
{
	NodeData* res = NULL;
	if(data)
	{
		res = (NodeData*)data;
		if(res && res->_type == NodeData::LINK)
		{
			LinkData* ldata = (LinkData*)res;
			res = ldata->GetTarget();
		}
	}
	return res;
}

void AblFileOperation::GetDirSizeW( const wstring& dir, int& nodeSum, __int64& sizeSum, vector<wstring>* fileExts, 
								   BaseProgressObserver* pProgressObserver )
{
	RETURN_IF( dir.empty() );
	RETURN_IF( ! CFileOperation::IsDirExist(dir.c_str()) );

	// 确保以 \ 结尾
	wstring d = dir;
	if ( *d.rbegin() != '\\' ) d.push_back('\\');

	WIN32_FIND_DATA fd;
	wstring strFind =  d + L"*";

	HANDLE hFindFile = FindFirstFile(strFind.c_str(), &fd);
	RETURN_IF (INVALID_HANDLE_VALUE == hFindFile);
	int iCountType = 0;
	OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);
	do
	{
		if(pProgressObserver && pProgressObserver->IsCancel())
			break;
		if (! wcscmp(fd.cFileName, L"." ) ||
			! wcscmp(fd.cFileName, L"..") )
			continue;

		wstring childName = d + fd.cFileName;
		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			AblFileOperation::GetDirSizeW(childName, nodeSum, sizeSum, fileExts, pProgressObserver);
			if(iCountType > 0)
				nodeSum++;
		}
		else
		{
			if ( fileExts != NULL && ! fileExts->empty() )
			{
				bool match = false;
				for (vector<wstring>::const_iterator it = fileExts->begin();
					it != fileExts->end(); ++it)
				{
					wstring ext = *it;
					int ext_length = ext.size();
					int offset = childName.size() - ext_length;
					if ( 0 ==_wcsnicmp(ext.c_str(), childName.c_str() + offset, ext_length) )
					{
						match = true;
						break;
					}
				}
				if (! match)
					continue;
			}

			nodeSum++;
			sizeSum += CFileOperation::GetFileSizeW(childName);
		}

	} while ( FindNextFile(hFindFile, &fd) );

	FindClose(hFindFile);
}




bool AblFileOperation::GetNodes( DirData* pRoot, wstring path, vector<NodeData*>& nodes, bool bFindLinkTarget, bool bExactMatch)
{
	RETURN_FALSE_IF( ! pRoot);
	RETURN_FALSE_IF( path.empty() );
	vector<wstring> nodeNames = CStrOperation::parseStringsW(path, L"/");

	if (nodeNames.empty())
		return false;

	DirData* pDir = pRoot;
	for(size_t i = 0; i < nodeNames.size() ; ++i)
	{
		NodeData* pNode = pDir->findChildrenbyName(nodeNames[i], bFindLinkTarget, bExactMatch);
		RETURN_FALSE_IF ( ! pNode || pNode->_type == NodeData::FILE );

		nodes.push_back(pNode);

		if (pNode->_type == NodeData::LINK)
			pDir = dynamic_cast<DirData*>((dynamic_cast<LinkData*>(pNode))->GetTarget());
		else
			pDir = dynamic_cast<DirData*>(pNode);
		RETURN_FALSE_IF( ! pDir );
	}
	return true;
}

 