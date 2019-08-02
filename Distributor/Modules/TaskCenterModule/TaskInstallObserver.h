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

	// ���ս�������
	virtual void GetProgress(ProgressData pData);

	// ���ս��������Ϣ
	virtual void Complete();

	// Summary  : ѯ���û��ļ�/Ŀ¼�Ѵ���ʱ��δ���
	// Returns  : ND91Assistant::EXIST_ACTION �û���ѡ��
	// Parameter: std::wstring name �ļ�/Ŀ¼��
	virtual EXIST_ACTION GetExistAction(std::wstring name);

private:
	TaskCenterController* _pTaskCentermodel;
	int     _taskId;

	wstring _deviceId;
};
