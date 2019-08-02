#pragma once

enum eLoadState
{
    LoadState_Unload,
    LoadState_Loading,
    LoadState_Loaded,
};

enum DeviceConnectState
{
    Device_Connect,
    Device_Disconnect,
    Device_DFUConnect,
    Device_DFUDisconnect,
    Device_Linkin,
};

enum RunUpdateType
{
    NoRunUpdate,
    RunForNormal,
    RunForSilent,
    RunForUpdate,
};

enum ActionIndex
{
    ACTION_INFO_LOADDEVICEINFO,
    ACTION_INFO_NET,
    ACTION_INFO_POWERINFO,
    Action_File_Download,
    Action_BS_Backup,
    Action_BS_Restore,
    Action_Init,
    Action_Check,
    Action_APP_LOAD,
    Action_APP_INSTALL,
    Action_APP_UNINSTALL,
	Action_APP_UP2PARICY,
	Action_APP_UP2LEGAL,
	Action_APP_UPGRADEALL,
    Action_Load_TaskList,
	Action_Delete_Task,
	Action_Start_Task,
	Action_Install_Deamon,
    Action_Driver_Install,
    Action_LoadData,
    Action_LoadWebData,
	Action_GetCustomerInfo,
	Action_SendCustomerInfo,
	Action_FM_LoadRootData,		// ���ؽڵ�
    Action_FM_Download,         // �ļ���������
    Action_FM_UploadFile,       // �ļ������ϴ��ļ�
    Action_FM_UploadDir,        // �ļ������ϴ��ļ���
    Action_FM_Delete,           // �ļ�����ɾ��
    Action_FM_DragOutFile,      // �ļ������ϳ��ļ�
    Action_FM_Paste,            // �ļ�����ճ��
	Action_FM_Load_App,		    // �ļ���������Ӧ�ó�����
	Action_FM_Ctreat_Dir,	    // �ļ�����Ԥ����dirĿ¼
	Action_DIS_Progress,	    // ������װ���ã���ʾ����
	Action_DIS_Install,		    // ������װ���ã����ؼ���װ����
	Action_DIS_InstallOne,		// ������װ���ã��������ؼ���װ����
	Action_SearchApp,		    // �������
	Action_DoMemoryClear,       // ��������װ������
    Action_Device_Clearup,      // һ������
	Action_Device_Reboot,	    // ����
	Action_Device_FixFlashExit,	// �����޸�
	Action_BR_Export,	        // ������ϵ��
	Action_BR_Import,	        // ������ϵ��
	Action_Post_StaticData,	    // �ϱ�ͳ������(Ӧ������,������Ϣ)
    Action_SpaceInfoChanged,    // �ռ���Ϣ���
	
	Action_GetHotAppsInfo,		// ��ȡ24Сʱ����Ӧ���б�
	Action_GetSoftCategorysInfo,// ��ȡ��Ϸ������������б�
	Action_GetAppDatasInfo,		// ��ȡ��������б�,����ҳ
	Action_GetGameDatasInfo,	// ��ȡ��Ϸ�����б�
	Action_GetOneAppInfo,		// ��ȡ���������ϸ��Ϣ
	Action_SearchAppsInfo,		// �������
	Action_DownLoadApp,			// ���ص���Ӧ��
	Action_GetAppsIcon,			// ��ȡӦ�õ�����ͼ

	Action_PostDownLoadInfo,	// �ϱ�Ӧ��������Ϣ(װ���̺�̨)
	Action_PostInstallInfo,		// �ϱ�Ӧ�ð�װ��Ϣ	
	Action_SendDataToService,	// �����˷�������(��ʼδ��¼������/��װ�������¼)
	Action_PostDataToService,	// �����˷�������(֮ǰδ���ͳɹ��������������)
	Action_SendDownloadInfo,	// 91��̨�����������ͳ��
	Action_SendSuccessInfo,		// 91��̨���Ͱ�װ�ɹ�ͳ��
	Action_SendFailInfo,		// 91��̨���Ͱ�װʧ��ͳ��

	Action_InstallNetPlan,		//��װ����װ������
	Action_App_LoadIcon,        //����AppIcon
};
