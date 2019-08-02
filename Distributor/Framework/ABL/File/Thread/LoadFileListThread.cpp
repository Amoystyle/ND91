#include "stdafx.h"
#include "LoadFileListThread.h"
#include <stack>
#include "LOGX.h"
#include "AblFileOperation.h"
#include "..\AblFileManager.h"
#include "CSLock.h"

LoadFileListThread::LoadFileListThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_requestLoadItemsByPath_ABL = NULL;
	_requestLoadItemsByItem_ABL = NULL;
	_requestJump2Path_ABL = NULL;
	_requestReloadNode_ABL = NULL;
	_requestAppDisplayNameMsg = NULL;
}

LoadFileListThread::~LoadFileListThread()
{
	SAFE_DELETE(_requestLoadItemsByPath_ABL);
	SAFE_DELETE(_requestLoadItemsByItem_ABL);
	SAFE_DELETE(_requestJump2Path_ABL);
	SAFE_DELETE(_requestReloadNode_ABL);
	SAFE_DELETE(_requestAppDisplayNameMsg);
}
bool LoadFileListThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
	WAIT_DEVICE_ACTIVE_NORMAL;
	try
	{
		Handle_RequestLoadItemsByPath_ABL(pDevice);
		Handle_RequestLoadItemsByItem_ABL(pDevice);
		Handle_RequestJump2Path_ABL(pDevice);
		Handle_RequestReloadNode_ABL(pDevice);
		Handle_RequestAppDisplayNameMsg(pDevice);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

void LoadFileListThread::Handle_RequestLoadItemsByPath_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestLoadItemsByPath_ABL)

		ResponseLoadNodes_ABL msg;
	msg.deviceID = _deviceID;
	msg.nRequestID = _nGUID;
	msg.nParam = _requestLoadItemsByPath_ABL->nParam;
	msg.nUserData = _requestLoadItemsByPath_ABL->nUserData;
	msg.sender_type = _requestLoadItemsByPath_ABL->sender_type;

	tstring path = _requestLoadItemsByPath_ABL->path;

	NodeData *cur_node;
	FileListItem *cur_item;
	DWORD dw = GetTickCount();
	cur_node = pDevice->CreateNoParentDirWithPath(path);
	DWORD dw1 = GetTickCount() - dw;
	cur_node->Refresh();
	DWORD dw2 = GetTickCount() - dw;
	cur_item = AblFileOperation::GetItem(cur_node, NULL);
	msg.items.push_back(cur_item);
	msg.item = cur_item;

	LOGX::Write(L"abl_loadpath:", path, CStrOperation::IntToWString(dw2));

	/*DirData* data = (DirData*)cur_node;
	if(data)
	{
	deque<NodeData*> sub_nodes = data->GetChildrensNoReload();
	for (deque<NodeData*>::iterator it = sub_nodes.begin(); it != sub_nodes.end(); it++)
	{
	NodeData* sub_node = *it;
	if(sub_node->_type == NodeData::DIR)
	{
	sub_node->Refresh();
	}
	}
	}*/
	DWORD dw3 = GetTickCount() - dw;


	msg.SendToUI();
}

void LoadFileListThread::Handle_RequestLoadItemsByItem_ABL( DeviceData* pDevice )
{
	// 经分析发现底层对快捷方式提供了删除， 只是删除快捷方式本身， 
	// 下载也是如此。 这样的话， 快捷方式对目标节点只有读（即使将来增加目标节点的下载）的操作。
	// 所以共用指针目前来说也是没问题的。
	// TODO 但是他的子集怎么办， 子集是可以删除的。 共用指针有问题。。。。。。。。。。。。。。。。。。。。
	RETURN_IF(!_requestLoadItemsByItem_ABL)

		ResponseLoadNodes_ABL msg;
	msg.deviceID = _deviceID;
	msg.nRequestID = _nGUID;
	msg.nParam = _requestLoadItemsByItem_ABL->nParam;
	msg.nUserData = _requestLoadItemsByItem_ABL->nUserData;
	msg.sender_type = _requestLoadItemsByItem_ABL->sender_type;

	NodeData *cur_node;
	FileListItem *cur_item;

	DWORD dw = GetTickCount();
	cur_node = (NodeData*)(_requestLoadItemsByItem_ABL->item->data);
	DWORD dw1 = GetTickCount() - dw;
	//AblFileOperation::ReloadData_ABL(cur_node);
	dw1 = GetTickCount() - dw;
	cur_item = AblFileOperation::GetItem(cur_node, NULL);
	msg.items.push_back(cur_item);
	msg.item = cur_item;

	msg.SendToUI();
}
void LoadFileListThread::Handle_RequestJump2Path_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestJump2Path_ABL)
		ResponseJump2Path_ABL msg;
	msg.deviceID = _deviceID;
	msg.nRequestID = _nGUID;
	msg.nParam = _requestJump2Path_ABL->nParam;
	msg.nUserData = _requestJump2Path_ABL->nUserData;
	msg.sender_type = _requestJump2Path_ABL->sender_type;
	msg.item = NULL;

	//NodeData *cur_node;
	FileListItem *root_item;

	tstring path = _requestJump2Path_ABL->path;
	root_item = _requestJump2Path_ABL->root_item;
	tstring root_path = _requestJump2Path_ABL->root_path;
	int index = 0;
retry:
	index++;
	tstring path_real = root_path + path;
	if(
		!CStrOperation::endsWithW(path_real, L"/..") 
		&& !CStrOperation::endsWithW(path_real, L"/.") 
		&& FileHelper::ExistInDevice(path_real, pDevice))
	{
		msg.root_data = index == 1 ? _requestJump2Path_ABL->root_data : _requestJump2Path_ABL->root_data2;
		NodeData *root_node = NULL;
		msg.root_item = NULL;
		if(!root_item)
			root_node = pDevice->CreateNoParentDirWithPath(root_path);
		else
			root_node = (NodeData*)root_item->data;	

		assert(root_node);

		vector<wstring> nodeNames = CStrOperation::parseStringsW(path, L"/");
	
		DirData* pDir = dynamic_cast<DirData*>(root_node) ;
		FileListItem *par_item = NULL;
		size_t size = nodeNames.size();
		bool findFirstNewNode = false; // true: 表示不要检查第一个新节点

		if(!root_node->userdata || findFirstNewNode)
		{
			par_item = AblFileOperation::GetItem(root_node, par_item);
			msg.root_item = par_item;
			findFirstNewNode = true;
			msg.item = par_item;
		}
		else
		{
			msg.item = root_item;
			msg.root_item = root_item;
		}
		for(size_t i = 0; i < size ; ++i)
		{
			NodeData* pNode = pDir->findChildrenbyName(nodeNames[i], false, true);
			if(!pNode)
			{// 发现输入 . 用exist判断会返会真, 目前是在入口处过滤掉 . 的路径
				msg.item = NULL;
				break;
			}
			assert(pNode);
			if(!pNode->userdata || findFirstNewNode)
			{
				par_item = AblFileOperation::GetItem(pNode, par_item);
				msg.items.push_back(par_item);
				findFirstNewNode = true;
			}
			else
				par_item = (FileListItem*)pNode->userdata;

			msg.item = par_item;

			if (pNode->_type == NodeData::LINK)
				pDir = dynamic_cast<DirData*>((dynamic_cast<LinkData*>(pNode))->GetTarget());
			else
				pDir = dynamic_cast<DirData*>(pNode);
		}
	}
	else
	{
		if( !_requestJump2Path_ABL->root_path2.empty() && root_path != _requestJump2Path_ABL->root_path2)
		{
			root_item = _requestJump2Path_ABL->root_item2;
			root_path = _requestJump2Path_ABL->root_path2;
			goto retry;
		}
		msg.item = NULL;
		msg.strParam = path;
	}
	msg.SendToUI();
}

void LoadFileListThread::Handle_RequestReloadNode_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestReloadNode_ABL);
	FileListItem* cur_item = (FileListItem*)_requestReloadNode_ABL->cur_node;
	DWORD dw = GetTickCount();
	AblFileOperation::Refresh(cur_item);
	LOGX::Write(L"abl_loadpath:",  *cur_item->strName, CStrOperation::IntToWString(GetTickCount() - dw));
	ResponseReloadNode_ABL msg;
	msg.nRequestID = _nGUID;
	msg.cur_item = cur_item;
	msg.nUserData = _requestReloadNode_ABL->nUserData;
	msg.nParam = _requestReloadNode_ABL->nParam;
	msg.SendToUI();
}	

void LoadFileListThread::Handle_RequestAppDisplayNameMsg( DeviceData* pDevice )
{
	RETURN_IF(!_requestAppDisplayNameMsg);
	DWORD dw = GetTickCount();
	ResponseAppDisplayNameMsg msg;
	msg.nRequestID=_nGUID;
	for (vector<int>::iterator it = _requestAppDisplayNameMsg->nodes.begin(); it != _requestAppDisplayNameMsg->nodes.end(); it ++)
	{
		FileListItem* item = (FileListItem*)(*it);
		NodeData* data = (NodeData*)item->data;
		FileHelper::IosGetDisplayName(data, pDevice);
		msg.nodes_text[(*it)] = data->_displayName;
	}
	DWORD dw2 = GetTickCount() - dw;
	msg.SendToUI();
}
