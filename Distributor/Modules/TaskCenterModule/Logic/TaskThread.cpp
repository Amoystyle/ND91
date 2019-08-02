#include "StdAfx.h"
#include "TaskThread.h"
#include "TaskCenterController.h"

CTaskThread::CTaskThread(string threadId)
:BaseThread(threadId)
{
	this->_module = NULL;
	this->_taskId = 0;
	this->_curDeviceId = L"";
	this->pParam = NULL;
	_type = Action_UnKnow; 
}

CTaskThread::~CTaskThread(void)
{
	if (this->pParam != NULL)
	{
		delete this->pParam;
		this->pParam = NULL;
	}
	
}

unsigned long CTaskThread::Main()
{
	if (NULL != _module)
	{
		switch (this->_type)
		{
		case Action_Start_Task:
			{
				_module->StartTaskAction(this->_taskId,this->_curDeviceId);
				break;
			}
		case Action_Delete_Task:
			{
				bool isNotify =  true;
				if (this->pParam!= NULL)
				{
					isNotify = this->pParam->bParam;
				}
				bool result = _module->DeleteTaskAction(_taskId,_curDeviceId);
				if (result && isNotify)
				{
					/*TaskModuleNotifyMsg taskremoveNotifyMsg;
					taskremoveNotifyMsg._notifyType = WM_TASK_REMOVED;
					taskremoveNotifyMsg._curID = _curDeviceId;
					taskremoveNotifyMsg._param = (LPARAM)(&_taskId);
					taskremoveNotifyMsg.SendToUI();*/
					_module->Notify(WM_TASK_REMOVED,_curDeviceId, (LPARAM)(&_taskId));

				}	
				break;
			}
		case  Action_Load_TaskList:
			{
				_module->LoadIconsInternal();
				break;
			}
		}
		
	}
	return 0;
}


void CTaskThread::SetModule(TaskCenterController* module)
{
	this->_module = module;
}

void CTaskThread::SetTaskId( int taskId )
{
	this->_taskId = taskId;
}

void CTaskThread::SetDeviceId( wstring curDeviceId )
{
	this->_curDeviceId = curDeviceId;
}

void CTaskThread::SetType( TaskThreadType type )
{
	this->_type = type;
}

void CTaskThread::SetParam( TaskThreadParam* var )
{
	this->pParam = var;
}
