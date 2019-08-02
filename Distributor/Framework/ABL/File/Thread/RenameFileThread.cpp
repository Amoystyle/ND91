#include "stdafx.h"
#include "RenameFileThread.h"
#include "../AblFileManager.h"
#include "AblFileOperation.h"


RenameFileThread::RenameFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_requestRenameFileMsg = NULL;
}

RenameFileThread::~RenameFileThread()
{
	SAFE_DELETE(_requestRenameFileMsg);
}

bool RenameFileThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
	WAIT_DEVICE_ACTIVE_NORMAL;
	Handle_RequestRenameFileMsg(pDevice);
	return false;
}

void RenameFileThread::Handle_RequestRenameFileMsg( DeviceData* pDevice )
{
	RETURN_IF(!_requestRenameFileMsg);
	ResponseRenameFileMsg msg;
	msg.nRequestID = _nGUID;
	try
	{
		tstring name = _requestRenameFileMsg->strTargetName;
		FileListItem* cur_item = (FileListItem*)_requestRenameFileMsg->cur_item;
		NodeData* cur_data = (NodeData*)cur_item->data;
		if(cur_data->_type == NodeData::LINK)
		{
			msg.res = Err_Unwritable;
			msg.SendToUI();
			return;
		}

		if(!cur_data || !FileHelper::ExistInDevice(cur_data->GetFullPath(), pDevice))
		{
			msg.res = Err_NodeEmpty;
			msg.SendToUI();
			return;
		}

		if(FileHelper::ExistInDevice(cur_data->GetParent()->GetFullPath() + name, pDevice))
			msg.res = 1;
		else
		{
			msg.res = cur_data->Rename(name) ? 0 : Err_Unwritable;
			assert(!msg.res);
		}
	}
	catch(AssistantException e)
	{
		msg.res = 3;
	}
	catch (...)
	{
		msg.res = 4;
	}    
	msg.SendToUI();
}
