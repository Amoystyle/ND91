#pragma once

enum TaskStatistcsAction
{
	Action_PostDownLoadInfo,	// �ϱ�Ӧ��������Ϣ(װ���̺�̨)
	Action_PostInstallInfo,		// �ϱ�Ӧ�ð�װ��Ϣ	
	Action_SendDataToService,	// �����˷�������(��ʼδ��¼������/��װ�������¼)
	Action_PostDataToService,	// �����˷�������(֮ǰδ���ͳɹ��������������)
	Action_SendDownloadInfo,	// 91��̨�����������ͳ��
	Action_SendInstallInfo,		// 91��̨���Ͱ�װ�ɹ�ͳ��
	Action_SendFailInfo		    // 91��̨���Ͱ�װʧ��ͳ��
};

struct StaticsticsInnerMsg : BaseMessage<StaticsticsInnerMsg>
{
	TaskStatistcsAction _action;
	void* voidParam;

	StaticsticsInnerMsg(TaskStatistcsAction action)
	{
		_action = action;
		voidParam = NULL;
	}


};
