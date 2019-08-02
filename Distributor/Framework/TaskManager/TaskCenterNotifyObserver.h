//#ifndef _TASK_CENTER_NOTIFY_OBSERVER_H_
//#define  _TASK_CENTER_NOTIFY_OBSERVER_H_

#pragma once
#include "TaskItem.h"
// TODO: 在此处引用程序需要的其他头文件
#ifdef _TASK_DLL
#define _DLL_EXPOR __declspec(dllexport)
#else
#define _DLL_EXPOR __declspec(dllimport)
#endif
class _DLL_EXPOR TaskCenterNotifyObserver
{
public:
	virtual void TaskDeleteNotify(int taskId, wstring deviceId=L"")=0;
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item,wstring deviceId=L"")=0;
	virtual void TaskFinishedNotify(CTaskItem* item,wstring deviceId=L"")=0;
	virtual void TaskFailNotify(CTaskItem* item,wstring deviceId=L"")=0;
};
//#endif