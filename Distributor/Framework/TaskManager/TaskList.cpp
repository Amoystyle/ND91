#include "stdafx.h"
#include "TaskList.h"
#include "TaskControl.h"
#include "Common/FileOperation.h"
//#include <afxwin.h>

UINT CTaskList::g_iIDCount = 1;
CTaskList::CTaskList(void)
{INTERFUN;
	InitializeCriticalSection(&_m_csLock);
	m_TaskList.Empty();
	_bPxlRunning=false;
}

CTaskList::~CTaskList(void)
{INTERFUN;
	DeleteCriticalSection(&_m_csLock);
	m_TaskList.Empty();
}

int CTaskList::GetTaskPercent(int iID)
{INTERFUN;
	TASK_LOCK();

	CTaskItem * item = this->_GetTaskInfoByID(iID);

	if(NULL == item)
	{
		TASK_UNLOCK();
		return 0;
	}

	int res = item->GetTaskPercent();

	TASK_UNLOCK();

	return res;
}

CTaskItem * CTaskList::GetTaskByID(int id)
{INTERFUN;
	TASK_LOCK();

	CTaskItem * item = this->_GetTaskInfoByID(id);

	TASK_UNLOCK();

	return item;
}

int CTaskList::AddItem(wstring TaskName,wstring URL,wstring m_csFileSavePath,TASK_KIND kind,
					   unsigned long size,DeviceData *pdevicedata,wstring version,
					   ProgressObserver* obs,bool sdcard,bool bUpdategrade,wstring casename,
					   void* pCpaInfo)
{INTERFUN;
	/*for(TaskList::iterator item = m_TaskList.begin();item != m_TaskList.end();item++)
	{
		CTaskItem* taskitem = *item;
		if(kind == URL_DOWNLAOD && taskitem->m_TaskFileInfo->sStrMes == CString(URL.c_str()))
		{
			if(!bUpdategrade)
			{
				return -1;
			}
			if(taskitem->m_uStatus!=PROCEDING)
			{
				int taskId=taskitem->m_uID;
				DeleteTaskByID(taskitem->m_uID);
				TaskControl::obServer->TaskDeleteNotify(taskId,L"");
				break;
			}
			taskitem->bUpadateGrade=bUpdategrade;
			return -1;
		}
		if(kind == INSTALL && taskitem->m_TaskFileInfo->savePath == CString(m_csFileSavePath.c_str())&&taskitem->m_casename==casename && taskitem->m_uStatus != FINISHED)
			return -1;
		if(kind == MOVEANDROIDAPP && taskitem->m_TaskFileInfo->sStrMes == CString(URL.c_str()) && taskitem->m_uStatus != FINISHED)
			return -1;
	}*/
	/*CTaskItem* finitem=findSameItem(URL,m_csFileSavePath,kind,bUpdategrade,casename);

	if(finitem&&kind == URL_DOWNLAOD)
	{
		if(bUpdategrade&&finitem->m_uStatus!=PROCEDING)
		{
			int taskId=finitem->m_uID;
			DeleteTaskByID(finitem->m_uID);
			TaskControl::obServer->TaskDeleteNotify(taskId,L"");
		}
		else 
		{
			if(bUpdategrade)
				finitem->bUpadateGrade=bUpdategrade;
			if(finitem->m_uStatus!=WAITING&&finitem->m_uStatus!=PROCEDING&&finitem->m_uStatus!=FINISHED)
			finitem->m_uStatus=WAITING;
			return finitem->m_uID;
		}
	}
	else if(finitem)
	{
		return finitem->m_uID;
	}*/

	CTaskItem * newTask = new CTaskItem(0,TaskName.c_str(),URL.c_str(),kind,pdevicedata,(wchar_t *)m_csFileSavePath.c_str());
	newTask->m_TaskFileInfo->TotalSize = size;
	newTask->m_appVersion =version;
	newTask->bUpadateGrade = bUpdategrade;

	newTask->_pObserver = obs;
	newTask->InstallToSdCard = sdcard;
	newTask->m_csFileDownUrl=URL;

	newTask->pCpaInfo = pCpaInfo;
	if(kind == URL_DOWNLAOD)
		newTask->m_fileguid=CFileOperation::GetGuidW();

	//设置任务ID
	newTask->m_uID = g_iIDCount;
	newTask->m_casename=casename;
	g_iIDCount++;

	TASK_LOCK();
	m_TaskList.m_list.push_back(newTask);
	TASK_UNLOCK();

	return newTask->m_uID;
}


CTaskItem* CTaskList::findSameItem(wstring URL,wstring m_csFileSavePath,TASK_KIND kind,bool bUpdategrade,wstring casename)
{INTERFUN;
TASK_LOCK();
	for(TaskList::iterator item = m_TaskList.begin();item != m_TaskList.end();item++)
	{
		CTaskItem* taskitem = *item;
		if(kind == URL_DOWNLAOD && taskitem->m_TaskFileInfo->sStrMes == URL)
		{
			TASK_UNLOCK();
			return taskitem;
		}
		if(kind == INSTALL && taskitem->m_TaskFileInfo->savePath == m_csFileSavePath&&taskitem->m_casename==casename && taskitem->m_uStatus != FINISHED)
		{
			TASK_UNLOCK();
			return taskitem;
		}
		if(kind == MOVEANDROIDAPP && taskitem->m_TaskFileInfo->sStrMes == URL && taskitem->m_uStatus != FINISHED)
		{
			TASK_UNLOCK();
			return taskitem;
		}
	}
	TASK_UNLOCK();
	return NULL;
}

int CTaskList::AddItem(CTaskItem& pitem)
{INTERFUN;
	for(TaskList::iterator item = m_TaskList.begin();item != m_TaskList.end();item++)
	{
		CTaskItem* taskitem = *item;
		if(pitem.m_uTaskType == URL_DOWNLAOD && taskitem->m_TaskFileInfo->sStrMes == pitem.m_TaskFileInfo->sStrMes )
			return -1;
	}
	CTaskItem * newTask = new CTaskItem(pitem);

	//设置任务ID
	newTask->m_uID = g_iIDCount;
	g_iIDCount++;


	TASK_LOCK();
	m_TaskList.m_list.push_back(newTask);
	TASK_UNLOCK();

	return newTask->m_uID;
}

bool CTaskList::SuspendItem(int id)
{INTERFUN;
	TASK_LOCK();

	CTaskItem * item = this->_GetTaskInfoByID(id);

	if(NULL != item && item->m_uID == id && (item->m_uStatus == TASK_STATUS::WAITING || item->m_uStatus == TASK_STATUS::PROCEDING))
	{
		item->SetState(TASK_STATUS::SUSPENDED);
		TASK_UNLOCK();
		return true;
	}

	TASK_UNLOCK();

	return false;
}

bool CTaskList::ActivateItem(int id)
{INTERFUN;
	TASK_LOCK();

	CTaskItem * item = this->_GetTaskInfoByID(id);

	if(NULL != item && item->m_uID == id && (item->m_uStatus == TASK_STATUS::FAILED || item->m_uStatus == TASK_STATUS::SUSPENDED))
	{
		item->SetState(TASK_STATUS::WAITING);

		TASK_UNLOCK();
		return true;
	}

	TASK_UNLOCK();

	return false;
}

bool CTaskList::RetryItem(int id)
{INTERFUN;
	TASK_LOCK();
	CTaskItem * item = this->_GetTaskInfoByID(id);
	if(!item||(item->m_uStatus != TASK_STATUS::FAILED && item->m_uStatus != TASK_STATUS::SUSPENDED))
	{
		TASK_UNLOCK();
		return false;
	}
	int nExitLoopCount=0;
	//等待线程结束处理该任务
	while(item->hasProcing==true){
		if(nExitLoopCount==50)//5秒
			break;
		nExitLoopCount++;
		Sleep(10);
	}
	item->SetState(WAITING);
	TASK_UNLOCK();
	/*nExitLoopCount=0;
	//等待线程结束处理该任务
	while(item->hasProcing==true){
		if(nExitLoopCount==50)//5秒
			break;
		nExitLoopCount++;
		Sleep(10);
	}*/
	return true;
}

bool CTaskList::DeleteTaskByID(int id)
{INTERFUN;
	CTaskItem *taskItem =  GetTaskByID(id);
	if (!taskItem)
	{		
		return true;
	}
	TASK_LOCK();
	if(taskItem&&(taskItem->m_uStatus == PROCEDING || taskItem->m_uStatus == WAITING))
	{
		taskItem->SetState(SUSPENDED);		
	}
	TASK_UNLOCK();
	int nExitLoopCount=0;
	//等待线程结束处理该任务
	while(taskItem->hasProcing==true){
		if(nExitLoopCount==500)//5秒
			break;
		nExitLoopCount++;
		Sleep(10);
	}
	TASK_LOCK();
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_uID == id)
		{		
			//delete((CTaskItem *)(*iter));
			if((*iter)&&(*iter)->m_uTaskType==URL_DOWNLAOD)
			{
				DeleteFile(((*iter)->m_TaskFileInfo->savePath+L"_"+(*iter)->m_fileguid).c_str());
			}
			iter = m_TaskList.erase(iter);
			TASK_UNLOCK();
			return true;
		}
		else
		iter++;
	}

	TASK_UNLOCK();
	return false;
}

void CTaskList::ClearList(void)
{INTERFUN;
	TASK_LOCK();
	for(TaskList::iterator iter = m_TaskList.begin();
		iter != m_TaskList.end();)
	{
		iter = m_TaskList.erase(iter);
	}
	TASK_UNLOCK();

	return ;
}

void CTaskList::ClearDone(std::list<int>* pList)
{INTERFUN;
	TASK_LOCK();

	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_uStatus == FINISHED)
		{
			pList->push_back((*iter)->m_uID);

			//遇到完成的任务时
			//			delete((CTaskItem *)(*iter));
			 m_TaskList.erase(iter);
			TRACE (_T("有清理\n"));
		}
		else
		{
			iter++;
		}
	}

	TASK_UNLOCK();

	return ;
}

bool CTaskList::MoveForward(int taskID)
{INTERFUN;
	TASK_LOCK();

	TaskList::iterator tmpIter = m_TaskList.begin();
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_uID == taskID)
		{
			//当查询到任务时
			if(iter != m_TaskList.begin())
			{
				//当需要移动的任务不为队列头时
				CTaskItem * tmpItem = (*iter);
				m_TaskList.erase(iter);

				if(tmpIter == m_TaskList.begin())
				{
					//当前一任务为队列头时，将前一任务添加到目标任务后
					m_TaskList.m_list.push_front(tmpItem);
				}
				else
				{
					tmpIter--;
					m_TaskList.m_list.insert(tmpIter, tmpItem);
				}
			}

			TASK_UNLOCK();

			return true;
		}

		tmpIter = iter;
		iter++;
	}

	TASK_UNLOCK();

	return false;
}

bool CTaskList::MoveBackwrad(int taskID)
{INTERFUN;
	TASK_LOCK();

	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_uID == taskID)
		{
			//当查询到任务时
			CTaskItem * tmpItem = (*iter);
			TaskList::iterator tmpIter = iter;

			iter++;

			if(iter != m_TaskList.end())
			{
				m_TaskList.erase(tmpIter);						//删除旧位置节点
				m_TaskList.m_list.insert(iter, tmpItem);		//将旧位置节点内容插入新位置
			}

			TASK_UNLOCK();

			return true;
		}

		iter++;
	}

	TASK_UNLOCK();

	return false;
}

bool CTaskList::GetFirstWaitingItem(CTaskItem * recTaskInfo)
{INTERFUN;
	if(NULL == recTaskInfo)
		return false;

	TASK_LOCK();

	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if(WAITING == (*iter)->m_uStatus)
		{
			//查询到等待状态任务
			(*iter)->SetState(TASK_STATUS::PROCEDING);		//设置状态为操作锁定状态
			(*iter)->hasProcing=true;
			recTaskInfo->CopyValue(*iter);					//设置接收对象值

			TASK_UNLOCK();
			return true;
		}

		iter++;
	}

	TASK_UNLOCK();

	return false;
}

bool CTaskList::GetFirstWaitingItemAndSetProceding(CTaskItem * recTaskInfo)
{INTERFUN;
	if(NULL == recTaskInfo)
		return false;

	TASK_LOCK();

	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if(WAITING == (*iter)->m_uStatus)
		{
			// 已经开始处理，但是状态还没有切过去，还是waiting，则跳过
			if((*iter)->hasProcing)
			{
				iter++;
				continue;
			}
			// PXL只能单个执行，不能并发

			if ((*iter)->m_uTaskType==INSTALL)
			{
				wstring ext=CFileOperation::GetFileExt((*iter)->m_TaskFileInfo->savePath);
				if (CStrOperation::toLowerW(ext)==L"pxl")
				{
					if(_bPxlRunning)
					{
						iter++;
						continue;
					}
					else
						_bPxlRunning = true;
				}
			}

			// 找到下一个任务
			(*iter)->hasProcing=true;
			(*iter)->SetState(TASK_STATUS::PROCEDING);
			recTaskInfo->CopyValue(*iter);					//设置接收对象值

			TASK_UNLOCK();
			return true;
		}

		iter++;
	}

	TASK_UNLOCK();

	return false;
}

void CTaskList:: FindAllCaseInstallTask(wstring caseId,vector<int>& taskIds)
{INTERFUN;
	TASK_LOCK();
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		CTaskItem* item= *iter;
		if(item->m_casename==caseId&&item->m_uStatus!=FINISHED)
		{
			taskIds.push_back(item->m_uID);
		}
		iter++;
	}
	TASK_UNLOCK();
}

void CTaskList::SetTaskState(int id, TASK_STATUS state)
{INTERFUN;
	TASK_LOCK();

	CTaskItem * item = this->_GetTaskInfoByID(id);

	if(NULL != item)
	{
		item->SetState(state);
	}

	TASK_UNLOCK();
}

BOOL CTaskList::GetTaskState(int iTaskID, TASK_STATUS& stateTask)
{INTERFUN;
	RETURN_FALSE_IF(iTaskID<0);
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return FALSE;
	}
	stateTask = l_pItem->m_uStatus;

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskIDs(std::list<int>* pList)
{INTERFUN;
	// 验证参数
	ASSERT(pList);

	TASK_LOCK();

	// 遍历所有的任务项
	TaskList::iterator l_itr = m_TaskList.begin();
	while (l_itr != m_TaskList.end())
	{
		// 将该任务项的ID放进列表中
		pList->push_back((*l_itr)->m_uID);
		l_itr ++;
	}

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskAddTime(int iTaskID, CTime& time)
{INTERFUN;
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return FALSE;
	}
	time = l_pItem->m_tAdd;

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskStartTime(int iTaskID, CTime& time)
{INTERFUN;
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return FALSE;
	}
	time = l_pItem->m_tStart;

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskEndTime(int iTaskID, CTime& time)
{INTERFUN;
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return FALSE;
	}
	time = l_pItem->m_tEnd;

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskTimeTried(int iTaskID, int& iTimeTried)
{INTERFUN;
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		iTimeTried = -1;
		TASK_UNLOCK();
		return FALSE;
	}
	iTimeTried = l_pItem->m_uTimeTried;

	TASK_UNLOCK();
	return TRUE;
}

BOOL CTaskList::GetTaskType(int iTaskID, TASK_KIND& kindTask)
{INTERFUN;
	TASK_LOCK();

	//获取指定TaskItem
	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return FALSE;
	}
	kindTask = l_pItem->m_uTaskType;

	TASK_UNLOCK();
	return TRUE;
}

bool CTaskList::ModifyTask(int id, CTaskItem* pItem)
{INTERFUN;
	if (!pItem)	
	{
		return false;
	}

	TASK_LOCK();

	// 找到任务项
	CTaskItem* l_pItem = _GetTaskInfoByID(id);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return false;
	}
	// 将传入的数据拷贝给它
	l_pItem->CopyValue(pItem);

	TASK_UNLOCK();

	return true;
}

void CTaskList::SetTaskNote(int iTaskID, LPCTSTR szNote)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return ;
	}
	l_pItem->m_strNote = szNote;

	TASK_UNLOCK();
}

LPCTSTR CTaskList::GetTaskNote(int iTaskID)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return NULL;
	}

	TASK_UNLOCK();

	return l_pItem->m_strNote.c_str();
}

DeviceData *CTaskList::GetTaskDevice(int iTaskID)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return NULL;
	}

	TASK_UNLOCK();

	return l_pItem->_pDeviceData;
}

int CTaskList::GetStateTaskCount(TASK_STATUS iState)
{INTERFUN;
	TASK_LOCK();

	// 遍历所有的任务项
	int l_iCount = 0;
	TaskList::iterator l_itr = m_TaskList.begin();
	while (l_itr != m_TaskList.end())
	{
		if ((*l_itr)->m_uStatus == iState)
		{
			l_iCount ++;
		}

		l_itr ++;
	}

	TASK_UNLOCK();

	return l_iCount;
}

void CTaskList::SetTaskName(int iTaskID, LPCTSTR szName)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return ;
	}
	l_pItem->m_strName = szName;

	TASK_UNLOCK();
}

LPCTSTR CTaskList::GetTaskName(int iTaskID)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return NULL;
	}

	wstring l_strName = l_pItem->m_strName;

	TASK_UNLOCK();

	return l_strName.c_str();
}

CTaskItem * CTaskList::_GetTaskInfoByID(int id)
{INTERFUN;
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_uID == id)
		{
			return *iter;
		}

		iter++;
	}

	return NULL;
}

int CTaskList::GetIDByUrl(wstring wsUrl)
{INTERFUN;
TASK_LOCK();
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		if((*iter)->m_csFileDownUrl == wsUrl)
		{
			TASK_UNLOCK();
			return (*iter)->m_uID;
		}

		iter++;
	}
TASK_UNLOCK();

	return -1;
}

void CTaskList::AppendTaskNote(int iTaskID, LPCTSTR szNote)
{INTERFUN;
	TASK_LOCK();

	CTaskItem* l_pItem = _GetTaskInfoByID(iTaskID);
	if (!l_pItem)
	{
		TASK_UNLOCK();
		return ;
	}
	l_pItem->m_strNote += _T("\r\n");
	l_pItem->m_strNote += szNote;
	TASK_UNLOCK();
}

int CTaskList::GetUnfinishedTaskCount()
{INTERFUN;
	TASK_LOCK();
	int unfinished=0;
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		CTaskItem* item= *iter;
		if(item->m_uStatus!=FINISHED)
		{
			unfinished++;
		}
		else if(item->m_uTaskType==URL_DOWNLAOD&&!CFileOperation::IsFileExist(item->m_TaskFileInfo->savePath.c_str()))
		{
			iter=m_TaskList.erase(iter);
			continue;
		}
		iter++;
	}
	TASK_UNLOCK();
	return unfinished;
}

int CTaskList::RestartAllUnfinishedTask()
{INTERFUN;
	TASK_LOCK();
	int unfinished=0;
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		CTaskItem* item= *iter;
		if(item->m_uTaskType!=URL_DOWNLAOD&&item->m_uStatus!=FINISHED)
		{
			if(item->m_uTaskType==MOVEANDROIDAPP&&item->_IsAndroidMoveStarted)
			{
				iter=m_TaskList.erase(iter);
				continue;
			}
			item->appData=NULL;
			item->SetState(WAITING);
			item->hasProcing=false;
			item->m_nProcess=0;
			unfinished++;
		}
		iter++;
	}
	TASK_UNLOCK();
	return unfinished;
}

void CTaskList::GetTaskByStatus( vector<int>* items, bool finished )
{INTERFUN;
	RETURN_IF(!items);
	TASK_LOCK();
	TaskList::iterator iter = m_TaskList.begin();
	while(iter != m_TaskList.end())
	{
		CTaskItem* item= *iter;
		if(item->m_uStatus!=FINISHED&&!finished)
			items->push_back(item->m_uID);
		else if(finished&&item->m_uStatus==FINISHED)
		{
			if(item->m_uTaskType==URL_DOWNLAOD&&!CFileOperation::IsFileExist(item->m_TaskFileInfo->savePath.c_str()))
			{
				iter=m_TaskList.erase(iter);
				continue;
			}
			items->push_back(item->m_uID);
		}
		iter++;
	}
	TASK_UNLOCK();
}