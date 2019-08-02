#pragma once
#include "Monitor/ProgressData.h"
class TaskCenterController;

class TaskInstallObserver:public ProgressObserver
{
public:
	TaskInstallObserver( wstring deviceId, TaskCenterController*  model);
	~TaskInstallObserver(void);

	void SetTaskId(int taskId){_taskId=taskId;}

	virtual void GetPrompt(MESSAGE_CODE msgCode);

	// 接收进度数据
	virtual void GetProgress(ProgressData pData);

	// 接收进度完成消息
	virtual void Complete();

	// Summary  : 询问用户文件/目录已存在时如何处理
	// Returns  : ND91Assistant::EXIST_ACTION 用户的选择
	// Parameter: std::wstring name 文件/目录名
	virtual EXIST_ACTION GetExistAction(std::wstring name);

private:
	TaskCenterController* _pTaskCentermodel;
	int     _taskId;

	wstring _deviceId;
};
