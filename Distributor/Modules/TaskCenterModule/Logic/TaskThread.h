#pragma once
#include "Base/BaseThread.h"
#include <string>
using namespace std;
class TaskCenterController;
enum TaskThreadType
{
	Action_UnKnow,
	Action_Start_Task,
	Action_Delete_Task,
	Action_Load_TaskList

};

struct TaskThreadParam
{
	bool bParam;
    void* pParam;
	TaskThreadParam()
	{
		bParam = false;
		pParam = NULL;
	}

	~TaskThreadParam()
	{
		if (NULL != pParam)
		{
			delete pParam;
			pParam = NULL;
		}
	}
};

class CTaskThread: public BaseThread
{
public:
	CTaskThread(string threadId);
	~CTaskThread(void);

	virtual unsigned long Main();

	void SetModule(TaskCenterController* module);
	void SetTaskId(int taskId);
	void SetDeviceId(wstring curDeviceId);
	void SetType(TaskThreadType type);
	void SetParam(TaskThreadParam* var);
private:
	TaskCenterController* _module;
	int _taskId;
	wstring _curDeviceId;
	TaskThreadType _type;
	TaskThreadParam* pParam;

};
