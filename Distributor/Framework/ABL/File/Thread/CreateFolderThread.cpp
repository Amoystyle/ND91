#include "stdafx.h"
#include "CreateFolderThread.h"
#include "AblFileOperation.h"
#include "..\AblFileManager.h"
#include "LOGX.h"

CreateFolderThread::CreateFolderThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_requestCreateFolderMsg= NULL;
	_requestCheckFoldersMsg = NULL;
}

CreateFolderThread::~CreateFolderThread()
{
	SAFE_DELETE(_requestCreateFolderMsg);
	SAFE_DELETE(_requestCheckFoldersMsg);
}

bool CreateFolderThread::Action(DeviceData* pDevice)
{
	ABLFile_AutoLock;
    WAIT_DEVICE_ACTIVE_NORMAL;
	Handle_RequestCreateFolderMsg_ABL(pDevice);
	Handle_RequestCheckFoldersMsg_ABL(pDevice);
    return true;
}

void CreateFolderThread::Handle_RequestCheckFoldersMsg_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestCheckFoldersMsg);
	ResponseCheckFoldersMsg msg;
	msg.nRequestID = _nGUID;

	map<tstring, int>* paths_all = &_requestCheckFoldersMsg->paths_all;
	for (map<tstring, int>::iterator it = paths_all->begin(); it != paths_all->end(); it++ )
	{
		tstring path = it->first;
		int type = it->second;
		if(!FileHelper::ExistInDevice(path, pDevice))
		{
			if(type == 1)
			{
				FileHelper::NewFolder(pDevice, path);
				if(!FileHelper::ExistInDevice(path, pDevice))
					msg.paths_res.push_back(path);
			}
			else
				msg.paths_res.push_back(path); 
		}
	}
	msg.SendToUI();
}

void CreateFolderThread::Handle_RequestCreateFolderMsg_ABL( DeviceData* pDevice )
{
	RETURN_IF(!_requestCreateFolderMsg);
	ResponseCreateFolderMsg msg;
	msg.res=0;
	msg.nRequestID = _nGUID;
	try
	{
		DWORD tw = GetTickCount();
		tstring name = _requestCreateFolderMsg->strFolderName;
		FileListItem* par_item = (FileListItem*)_requestCreateFolderMsg->par_item;
		DirData* par_data = NULL;
		NodeData* ndata = AblFileOperation::GetNode(par_item->data);
		if(ndata && ndata->_type == NodeData::DIR)
			par_data = dynamic_cast<DirData*>(ndata);

		if(!par_data || !FileHelper::ExistInDevice(par_data->GetFullPath(), pDevice))
			msg.res = Err_NodeEmpty;

		LOGX::Write(L"创建文件夹花费时间1： ", CStrOperation::IntToWString(GetTickCount() - tw));
		if(!msg.res)
		{
			if(FileHelper::ExistInDevice(par_data->GetFullPath() + name, pDevice))
				msg.res = 1;
			else
			{
				LOGX::Write(L"创建文件夹花费时间2： ", CStrOperation::IntToWString(GetTickCount() - tw));
				bool b = par_data->MakeSubDir(name);
				LOGX::Write(L"创建文件夹花费时间3： ", CStrOperation::IntToWString(GetTickCount() - tw));
				if(b)
				{
					DirData* cur_data = dynamic_cast<DirData*>(par_data->findChildrenbyName(name));
					msg.cur_item = AblFileOperation::GetItem(cur_data, par_item);
					msg.res = msg.cur_item ? 0 : 2;
				}
				else
					msg.res = Err_Unwritable;
			}
		}
		LOGX::Write(L"创建文件夹花费时间4： ", CStrOperation::IntToWString(GetTickCount() - tw));
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
