#include "stdafx.h"
#include "DeleteFileThread.h"
#include "AblFileOperation.h"
#include "..\AblFileManager.h"

DeleteFileThread::DeleteFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_requestDeleteFileMsg = NULL;
_requestDeleteFileByPathMsg=NULL;
_pProgressObserver = new BaseProgressObserver(_nGUID);
}

DeleteFileThread::~DeleteFileThread()
{
	SAFE_DELETE(_requestDeleteFileByPathMsg);
	SAFE_DELETE(_requestDeleteFileMsg);
}

bool DeleteFileThread::find(DirData* par_data, NodeData* sub_data)
{
	deque<NodeData*> sub_nodes = par_data->GetChildrensNoReload();
	for (deque<NodeData*>::iterator it = sub_nodes.begin(); it != sub_nodes.end(); it++)
		if(*it == sub_data)
			return true;
	return false;
}

bool DeleteFileThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
	WAIT_DEVICE_ACTIVE_NORMAL;
	try
	{
		Handle_RequestDeleteFileByPathMsg(pDevice);
		Handle_RequestDeleteFileMsg(pDevice);
	}catch(...)
	{

	}
	return true;
}

void DeleteFileThread::Handle_RequestDeleteFileByPathMsg( DeviceData* pDevice )
{
	RETURN_IF(!_requestDeleteFileByPathMsg);
	RootData *pRootdata = pDevice->GetRootData();
	DirData* pRoot = pRootdata->GetDir();

	vector<NodeData*> nodes = AblFileOperation::GetNodes(pRoot, _requestDeleteFileByPathMsg->paths, true, true, _pProgressObserver);
	
	NodeData* pFailNode=NULL;
	if(!(_pProgressObserver && _pProgressObserver->IsCancel()))
		pFailNode = FileHelper::BatchDelete(&nodes, _pProgressObserver);

	ResponseDeleteFileByPathMsg msg;
	msg.nRequestID = _nGUID;
	msg.nUserData = _requestDeleteFileByPathMsg->nUserData;
	if(pFailNode)
	{
		for (vector<tstring>::iterator it = _requestDeleteFileByPathMsg->paths.begin(); it !=  _requestDeleteFileByPathMsg->paths.end(); it++)
		{
			tstring path = *it;
			if(path != pFailNode->GetFullPath())
			{
				msg.node_del.push_back(path);
			}
			else
				break;
		}
		msg.res = 1;
	}
	else
	{
		for (vector<tstring>::iterator it = _requestDeleteFileByPathMsg->paths.begin(); it !=  _requestDeleteFileByPathMsg->paths.end(); it++)
			msg.node_del.push_back(*it);
		msg.res = 0;
	}
	msg.SendToUI();

	NotifyModuleSpaceChangedMsg spaceMsg;
	spaceMsg.deviceID = _deviceID;
	spaceMsg.nModule = eSpaceInfo_Module_File;
	spaceMsg.PostToUI();
}

void DeleteFileThread::Handle_RequestDeleteFileMsg( DeviceData* pDevice )
{
	RETURN_IF(!_requestDeleteFileMsg);
	FileListItem* par_item;
	par_item = (FileListItem*)_requestDeleteFileMsg->par_node;
	DirData* par_data = 0;

	NodeData* ndata = AblFileOperation::GetNode(par_item->data);
	if(ndata && ndata->_type == NodeData::DIR)
		par_data = dynamic_cast<DirData*>(ndata);

	ResponseDeleteFileMsg msg;
	msg.nRequestID = _nGUID;
	msg.node_fail = 0;

	if(!par_data || !FileHelper::ExistInDevice(par_data->GetFullPath(), pDevice))
	{
		msg.res = Err_NodeEmpty;
		msg.SendToUI();
		return;
	}
	 
	vector<NodeData*> nodes;
	for (vector<int>::iterator it = _requestDeleteFileMsg->nodes.begin(); it != _requestDeleteFileMsg->nodes.end(); it ++)
	{
		FileListItem* item = (FileListItem*)*it;
		nodes.push_back((NodeData*)item->data);
	}
	bool bRec = true;

	
	NodeData* pFailNode = FileHelper::BatchDelete(&nodes, _pProgressObserver);
	FileListItem* item_fail = NULL;

	if(_pProgressObserver && _pProgressObserver->IsCancel())
	{
		msg.res = Err_Cancel;
		// UI 需要处理刷新父节点
		/*for (vector<int>::iterator it = _requestDeleteFileMsg->nodes.begin(); it != _requestDeleteFileMsg->nodes.end(); it ++)
		{
			FileListItem* item = (FileListItem*)*it;
			NodeData* data = (NodeData*)(item->data);
			if(!find(par_data, data))
				msg.node_del.push_back(*it);
		}*/
	}
	else
	{
		if (pFailNode)
		{
			msg.res = 2;
			// TODO TEST
			item_fail = (FileListItem*)pFailNode->userdata;
			assert(item_fail);
			// UI 需要处理刷新父节点
			/*AblFileOperation::Refresh(item_fail);

			for (vector<int>::iterator it = _requestDeleteFileMsg->nodes.begin(); it != _requestDeleteFileMsg->nodes.end(); it ++)
			{
				FileListItem* item = (FileListItem*)*it;
				NodeData* data = (NodeData*)(item->data);
				if(!find(par_data, data))
					msg.node_del.push_back(*it);
			}*/
		}   
		else
		{
			msg.res = 0;
			for (vector<int>::iterator it = _requestDeleteFileMsg->nodes.begin(); it != _requestDeleteFileMsg->nodes.end(); it ++)
				msg.node_del.push_back(*it);
		}
	}

	msg.node_fail = (int)item_fail;
	msg.SendToUI();

	NotifyModuleSpaceChangedMsg spaceMsg;
	spaceMsg.deviceID = _deviceID;
	spaceMsg.nModule = eSpaceInfo_Module_File;
	spaceMsg.PostToUI();
}
