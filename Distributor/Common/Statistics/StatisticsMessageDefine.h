#pragma once

//װ������ͳ�ƹ��ܺ궨��


//#define Statistic_APP_ID   100457		//һ��Խ��
//#define Statistic_APP_KEY  "853449c2396e68ae3667366a486cb56c874e1b3350df5031"

//#define Statistic_APP_ID   109739		//91��  ������

#define Statistic_APP_ID	100571
#define Statistic_APP_KEY	"3cdbd2244fb7d781cf4c8938a5d3eb5413e35d1b626c787b"	

//  --------------------------------����ID-------------------------------------

// һ�����ֹ��ܵ�	
#define Statistic_ID_MyDevice			"10000"		//�ҵ��豸
#define Statistic_ID_BatchTun			"20000"		//����װ��
#define Statistic_ID_LocalRes			"30000"		//������Դ
#define Statistic_ID_NetRes				"40000"		//������Դ
#define Statistic_ID_TaskCenter    		"50000"		//��������
#define Statistic_ID_Login				"60000"		//��¼
#define Statistic_ID_CheckUpdate   		"70000"		//���������½ǵļ�����
#define Statistic_ID_InstallAssist		"80000"		//��װ�ֻ�������
#define Statistic_ID_InstallForAnd		"80001"		//��װ91����(�ֻ���) for Android
#define Statistic_ID_InstallForIos  	"80002"		//��װ91����(�ֻ���) for iPhone
#define Statistic_ID_InstallForIpad		"80003"		//��װ91����(�ֻ���) for iPad
#define Statistic_ID_InstallForLegal	"80004"		//��װ91����(�ֻ���) for ����

// �����ҵ��豸	  
#define Statistic_ID_InstallPathSet		"10001"		//��װ·������
#define Statistic_ID_AddPlan     		"10002"		//����ӡ�װ������
#define Statistic_ID_WhatPlan			"10003"		//ʲô��װ��������
#define Statistic_ID_Refurbish     		"10004"		//�������豸��ˢ�¡���ť
#define Statistic_ID_InstallPlan     	"10005"		//����װ��װ������-����
#define Statistic_ID_PlanMore     		"10006"		//�����࡭��װ������
#define Statistic_ID_InstallNetPlan     "10007"		//����װ��װ������-����
#define Statistic_ID_InstallIosApp    	"10008"		//���ù��ܣ���װ����-iOS
#define Statistic_ID_InstallAndApp  	"10009"		//���ù��ܣ���װ����-Android
#define Statistic_ID_IosReboot  		"10010"		//���ù��ܣ���������-iOS
#define Statistic_ID_AndroidReboot 		"10011"		//���ù��ܣ���������-Android
#define Statistic_ID_InRecoveryMode		"10012"		//���ù��ܣ�����ָ�ģʽ
#define Statistic_ID_OutRecoveryMode	"10013"		//���ù��ܣ��˳��ָ�ģʽ
#define Statistic_ID_IosFreeMemory     	"10014"		//���ù��ܣ��ڴ��ͷ�-iOS
#define Statistic_ID_AndroidFreeMemory  "10015"		//���ù��ܣ��ڴ��ͷ�-Android
#define Statistic_ID_APP     			"11001"		//Ӧ�ó���
#define Statistic_ID_UserAppTab     	"11002"		//���û�����Tab��ǩ
#define Statistic_ID_SystemAppTab     	"11003"		//��ϵͳ����Tab��ǩ
#define Statistic_ID_InstallPcApp   	"11004"		//����װ�����������ť
#define Statistic_ID_TopUnstallBnt    	"11005"		//��ж�ء���ť������������
#define Statistic_ID_ListUnstallBnt		"11006"		//��ж�ء���ť���б�����
#define Statistic_ID_AppMoveToSDcard	"11007"		//Ӧ�ó�������SD�����б�����
#define Statistic_ID_AppMoveToPhone    	"11008"		//Ӧ�ó��������ֻ����б�����
#define Statistic_ID_LotAppMoveToSDcard	"11009"		//Ӧ�ó�������SD�����ײ�������
#define Statistic_ID_LotAppMoveToPhone 	"11010"		//Ӧ�ó��������ֻ�������������
#define Statistic_ID_BackupRestore		"12001"		//���ݻ�ԭ
#define Statistic_ID_BackupTab     		"12002"		//�����ر��ݡ�Tab��ǩ
#define Statistic_ID_RestoreTab     	"12003"		//�����ػ�ԭ��Tab��ǩ
#define Statistic_ID_DeriveTovCard		"12004"		//������ϵ��vCard�ļ�
#define Statistic_ID_DeriveToOutLook	"12005"		//������ϵ��Outlook
#define Statistic_ID_vCardImport     	"12006"		//������ϵ��vCard�ļ�
#define Statistic_ID_OutLookImport   	"12007"		//������ϵ��Outlook
#define Statistic_ID_AnyBackupFile		"12008"		//��ѡ���������ݡ���ť
#define Statistic_ID_BackupContact		"12009"		//���ݹ�ѡ����ϵ�ˡ�
#define Statistic_ID_BackupMessage		"12010"		//���ݹ�ѡ�����š�
#define Statistic_ID_BackupCallReocrd	"12011"		//���ݹ�ѡ��ͨ����¼��
#define Statistic_ID_BackupMusic     	"12012"		//���ݹ�ѡ�����֡�
#define Statistic_ID_BackupNote     	"12013"		//���ݹ�ѡ������¼��
#define Statistic_ID_BackupWallPaper	"12014"		//���ݹ�ѡ����ֽ��
#define Statistic_ID_BackupPicture		"12015"		//���ݹ�ѡ��ͼƬ��
#define Statistic_ID_BackupApp   		"12016"		//���ݹ�ѡ��Ӧ�ó���
#define Statistic_ID_BackupAppRecords	"12017"		//���ݹ�ѡ��Ӧ�ó����¼��
#define Statistic_ID_BackupAppForIPA	"12018"		//���ݹ�ѡ��Ӧ�ó��򣨽�����IPA����
#define Statistic_ID_BackupiBooks    	"12019"		//���ݹ�ѡ�������顿
#define Statistic_ID_BackupTheme     	"12020"		//���ݹ�ѡ�����⡿
#define Statistic_ID_BackupRing     	"12021"		//���ݹ�ѡ���Զ���������
#define Statistic_ID_ChangeBackupPath	"12022"		//����·�����޸ġ���ť
#define Statistic_ID_BackupBegin     	"12023"		//����ʼ���ݡ���ť
#define Statistic_ID_BackupPass     	"12024"		//����ʱ����������ť
#define Statistic_ID_BackupStop   		"12025"		//����ʱ����ֹ����ť
#define Statistic_ID_BackupEnding    	"12026"		//������ɡ��رա���ť
#define Statistic_ID_BackupFileLook		"12027"		//������ɡ������鿴����ť
#define Statistic_ID_RestoreContact		"12028"		//��ԭ��ѡ����ϵ�ˡ�
#define Statistic_ID_RestoreMessage		"12029"		//��ԭ��ѡ�����š�
#define Statistic_ID_RestoreCallReocrd	"12030"		//��ԭ��ѡ��ͨ����¼��
#define Statistic_ID_RestoreMusic     	"12031"		//��ԭ��ѡ�����֡�
#define Statistic_ID_RestoreNote     	"12032"		//��ԭ��ѡ������¼��
#define Statistic_ID_RestoreWallPaper	"12033"		//��ԭ��ѡ����ֽ��
#define Statistic_ID_RestorePicture		"12034"		//��ԭ��ѡ��ͼƬ��
#define Statistic_ID_RestoreApp   		"12035"		//��ԭ��ѡ��Ӧ�ó���
#define Statistic_ID_RestoreAppRecords	"12036"		//��ԭ��ѡ��Ӧ�ó����¼��
#define Statistic_ID_RestoreAppForIPA	"12037"		//��ԭ��ѡ��Ӧ�ó��򣨽�����IPA����
#define Statistic_ID_RestoreiBooks    	"12038"		//��ԭ��ѡ�������顿
#define Statistic_ID_RestoreTheme     	"12039"		//��ԭ��ѡ�����⡿
#define Statistic_ID_RestoreRing     	"12040"		//��ԭ��ѡ���Զ���������
#define Statistic_ID_InstallSoft		"12041"		//����ʼ��ԭ����ť

#define Statistic_ID_FileManager     	"13001"		//�ļ�����
#define Statistic_ID_CreateDir   		"13002"		//�ļ�����-���½��ļ��С�
#define Statistic_ID_UpLoadFile    		"13003"		//�ļ�����-���ϴ��ļ���
#define Statistic_ID_UpLoadDir			"13004"		//�ļ�����-���ϴ�Ŀ¼��
#define Statistic_ID_DownLoadFile		"13005"		//�ļ�����-�����ء�
#define Statistic_ID_DeleteFile			"13006"		//�ļ�����-��ɾ����
#define Statistic_ID_UpdateTree			"13007"		//�ļ�����-��ˢ�¡�
#define Statistic_ID_RightBntMenu     	"13008"		//�ļ�����- �Ҽ��˵�
#define Statistic_ID_SearchFilter     	"13009"		//�ļ�����-��������
#define Statistic_ID_FileRename			"13010"		//�ļ�����-F2������
#define Statistic_ID_MobileUdisk		"13011"		//�������-�ƶ�U��
#define Statistic_ID_MemoryCard   		"13012"		//�������-���濨
#define Statistic_ID_MyDocuments		"13013"		//�������-�ҵ��ĵ�
#define Statistic_ID_Picture			"13014"		//�������-��Ƭ
#define Statistic_ID_VoiceMemo    		"13015"		//�������-��������¼
#define Statistic_ID_UserAppList     	"13016"		//�������-�û�Ӧ�ó���
#define Statistic_ID_CydiaAppInstall    "13017"		//�������-Cydia�����װ



// ��������װ��										
#define Statistic_ID_IosSysInstall		"20001"		//�����iOSϵͳװ�����ڵ�
#define Statistic_ID_AndroidSysInstall	"20002"		//�����Androidϵͳװ�����ڵ�
#define Statistic_ID_OneKeyInstall		"20003"		//�����һ��װ������ť

// �ġ�������Դ							  
#define Statistic_ID_LocalResBnt		"30001"		//�����������Դ���ڵ�										
#define Statistic_ID_IosAppLibrary		"30002"		//�����iOS����⡿�ڵ�										
#define Statistic_ID_AndroidAppLibrary	"30003"		//�����Android����⡿�ڵ�									
#define Statistic_ID_IosLibraryUpDate	"30004"		//�����iOS����⡿-����������Ӧ�á�						
#define Statistic_ID_AnLibraryUpDate	"30005"		//�����Android����⡿-����������Ӧ�á�					
#define Statistic_ID_InstallPlanBnt		"30006"		//�����װ���������ڵ�										
#define Statistic_ID_AutoAddPcAPP		"31001"		//������Դ��������Զ���Ӵ����е��ֻ�Ӧ�á���ť			
#define Statistic_ID_StopAutoAddApp		"31002"		//������Դ��������Զ���Ӵ����е��ֻ�Ӧ�á�-��ֹͣ����ť	
#define Statistic_ID_AddIosLibraryBnt	"31003"		//������Դ���������ӡ�-��iOS����⡿��ť					
#define Statistic_ID_AddAnLibraryBnt	"31004"		//������Դ���������ӡ�-��Android����⡿��ť				
#define Statistic_ID_OpenDirBnt			"31005"		//������Դ���������Ŀ¼����ť							
#define Statistic_ID_MoveToBnt   		"31006"		//������Դ��������Ƴ�����ť								
#define Statistic_ID_DeleteBnt    		"31007"		//������Դ�������ɾ������ť								
#define Statistic_ID_IosAddBnt     		"32001"		//iOS����⣺�������ӡ���ť								
#define Statistic_ID_IosSameCheck		"32002"		//iOS����⣺��ѡ����ͬӦ�ý���ʾ���°汾��					
#define Statistic_ID_IosBatchDelete		"32003"		//iOS����⣺�����ɾ������ť��������						
#define Statistic_ID_IosBatchInstall	"32004"		//iOS����⣺�����һ��װ������ť��������					
#define Statistic_ID_IosSingleInstall	"32005"		//iOS����⣺��������������װ����ť						
#define Statistic_ID_IosSingleDelete	"32006"		//iOS����⣺������������ɾ������ť						
#define Statistic_ID_IosUpSameCheck		"32007"		//iOS�����-��������Ӧ�ã���ѡ����ͬӦ�ý���ʾ���°汾��	
#define Statistic_ID_IosUpSingleCheck	"32008"		//iOS�����-��������Ӧ�ã���������������������ť			
#define Statistic_ID_IosUpBatchCheck	"32009"		//iOS�����-��������Ӧ�ã��������������ť��������			
#define Statistic_ID_IosUpBatchOneKey	"32010"		//iOS�����-��������Ӧ�ã������һ��װ������ť��������		
#define Statistic_ID_AnAddBnt     		"33001"		//Android����⣺�������ӡ���ť							
#define Statistic_ID_AnSameCheck		"33002"		//Android����⣺��ѡ����ͬӦ�ý���ʾ���°汾��				
#define Statistic_ID_AnBatchDelete		"33003"		//Android����⣺�����ɾ������ť��������					
#define Statistic_ID_AnBatchInstall		"33004"		//Android����⣺�����һ��װ������ť��������				
#define Statistic_ID_AnSingleInstall	"33005"		//Android����⣺��������������װ����ť					
#define Statistic_ID_AnSingleDelete		"33006"		//Android����⣺������������ɾ������ť					
#define Statistic_ID_AnUpSameCheck		"33007"		//Android�����-��������Ӧ�ã���ѡ����ͬӦ�ý���ʾ���°汾��
#define Statistic_ID_AnUpSingleCheck	"33008"		//Android�����-��������Ӧ�ã���������������������ť		
#define Statistic_ID_AnUpBatchCheck		"33009"		//Android�����-��������Ӧ�ã��������������ť��������		
#define Statistic_ID_AnUpBatchOneKey	"33010"		//Android�����-��������Ӧ�ã������һ��װ������ť��������	
#define Statistic_ID_CreateNewPlan  	"34001"		//װ������������������·�������ť							
#define Statistic_ID_DeleteAllSelect  	"34002"		//װ�������������ɾ����ѡ��������ť��������				
#define Statistic_ID_APlanAddBnt  		"34003"		//װ������-ĳ���������������ӡ���ť						
#define Statistic_ID_AnAppAddBnt  		"34004"		//װ������-ĳ����������������������װ����ť				
#define Statistic_ID_AnAppMoveBnt  		"34005"		//װ������-ĳ���������������������Ƴ�����ť				
#define Statistic_ID_ABatchInstallBnt	"34006"		//װ������-ĳ�������������һ��װ������ť��������			
#define Statistic_ID_ABatchMoveBnt		"34007"		//װ������-ĳ��������������Ƴ�����ť��������				

// �塢������Դ					  
#define Statistic_ID_LatelyUpdate		"40001"		//�����������¡��ڵ�
#define Statistic_ID_TodayHot			"40002"		//������������š��ڵ�
#define Statistic_ID_InstallNeed		"40003"		//�����װ���ر����ڵ�
#define Statistic_ID_SoftTreasure		"40004"		//�����������⡿�ڵ�
#define Statistic_ID_CompleteOfGame		"40005"		//�������Ϸ��ȫ���ڵ�
#define Statistic_ID_SearchBnt			"40006"		//�������������ť
#define Statistic_ID_UpdateBnt			"40007"		//�����ˢ�¡���ť

#define Statistic_ID_DownLoadBnt		"40008"		//��������ء���ť
#define Statistic_ID_UnJailbrokenFree   "40009"		//�������Խ��Ҳ��ѡ��ڵ�
#define Statistic_ID_IphonePlatForm     "41001"		//��Խ��Ҳ��ѣ�iPhoneƽ̨
#define Statistic_ID_IpadPlatForm		"41002"		//��Խ��Ҳ��ѣ�iPadƽ̨
#define Statistic_ID_DownNumOf24Hot		"41003"		//��Խ��Ҳ��ѣ�24Сʱ����Ӧ��������
#define Statistic_ID_PageOf24Hot		"41004"		//��Խ��Ҳ��ѣ�24Сʱ����Ӧ�÷�ҳ
#define Statistic_ID_SearchLegalApp		"41005"		//��Խ��Ҳ��ѣ������������
#define Statistic_ID_DownBntOfDetail	"41006"		//��Խ��Ҳ��ѣ��������ҳ�С����ء���ť
#define Statistic_ID_DownLoadMostBnt	"41007"		//��Խ��Ҳ��ѣ���������ࡿ��ǩ
#define Statistic_ID_PriceHighBnt		"41008"		//��Խ��Ҳ��ѣ����۸���ߡ���ǩ
#define Statistic_ID_LastRelease		"41009"		//��Խ��Ҳ��ѣ������·�������ǩ
#define Statistic_ID_PullDownChange		"41010"		//��Խ��Ҳ��ѣ����������л�
#define Statistic_ID_FlashAnnealing  	"41011"		//��Խ��Ҳ��ѣ������޸�
#define Statistic_ID_DownLoadTotal 		"41012"		//��Խ��Ҳ��ѣ�������������������
#define Statistic_ID_GoToPage  			"41013"		//��Խ��Ҳ��ѣ�ҳ��ײ���ָ��ҳ����ת����
#define Statistic_ID_WebSoft_Category   "41014"		//��Խ��Ҳ��ѣ���ҳ����


// ������������	  
#define Statistic_ID_DownloadTaskBnt	"50001"		//������������񡿽ڵ�
#define Statistic_ID_MessageBoxClose	"50002"		//������ʾ�򡾹رա���ť
#define Statistic_ID_MessageBoxLook		"50003"		//������ʾ�򡾵���鿴����ť
#define Statistic_ID_DownloadBeginBnt	"51001"		//�������񣺵�������������ʼ����ť
#define Statistic_ID_DownloadStopBnt	"51002"		//�������񣺵�������������ͣ����ť
#define Statistic_ID_DownloadDeleteBnt	"51003"		//�������񣺵�����������ɾ������ť
#define Statistic_ID_DownloadBatBegin	"51004"		//�������񣺵������ʼ����ť��������
#define Statistic_ID_DownloadBatDelete	"51005"		//�������񣺵����ɾ������ť��������
#define Statistic_ID_DownloadBatAutoDel	"51006"		//�������񣺵����ɾ����-���Զ�ɾ�����������
#define Statistic_ID_InstallAnAppRetry 	"52001"		//��װж�أ����������������ԡ���ť
#define Statistic_ID_InstallAnAppDel  	"52002"		//��װж�أ�������������ɾ������ť
#define Statistic_ID_InstallBatchRetry  "52003"		//��װж�أ���������°�װ����ť��������
#define Statistic_ID_InstallBatchDel  	"52004"		//��װж�أ������ɾ������ť��������
#define Statistic_ID_InstallAutoDel  	"52005"		//��װж�أ������ɾ����-���Զ�ɾ�����������

#define Statistic_ID_NumOfTaskBegin  	"52006"		//�����ʼ����������
#define Statistic_ID_NumOfSuccess		"52007"		//������سɹ���
#define Statistic_ID_NumOfInstallBegin  "52008"		//Ӧ�ÿ�ʼ��װ������
#define Statistic_ID_NumOfInstallOK		"52009"		//Ӧ�ð�װ�ɹ���


// �ߡ���¼		  
#define Statistic_ID_NextAutoLoginBnt  	"60001"		//��ѡ���´��Զ���¼��ѡ��
#define Statistic_ID_CloseBnt  			"60002"		//������رա���ť
#define Statistic_ID_LoginInBnt  		"60003"		//δ��¼������رա���¼


//  ����ͳ�ƹ���ID
#define Statistic_ID_ConnectDeviceNum 	"900001"	//װ������ͬʱ��������豸��
#define Statistic_ID_InstallPlanNum  	"900002"	//װ������������
#define Statistic_ID_ExecuteDuration	"900003"	//����װ��������װʱ��
#define Statistic_ID_AppNumInAnPlan  	"900004"	//����װ��������Ӧ������
#define Statistic_ID_ZJStartTime  		"900005"	//װ����������ʱ��	

struct CStaticsticsMessage :
	BaseMessage<CStaticsticsMessage>
{
	CStaticsticsMessage(string id)
	{	
		_id = id;
	}
	
	string _id;
};

struct CStaticsticsMessageWithTab :
	BaseMessage<CStaticsticsMessageWithTab>
{
	string _id;
	string _tab;
	bool ret;
	CStaticsticsMessageWithTab(string id, string tab)
	{
		_id = id;
		_tab = tab;
		ret = false;
	}
};

struct CStaticsticsDevice : public BaseMessage<CStaticsticsDevice>
{
	void* pAblDeviceInfo;
};

struct PostDownloadInfoMsg : public BaseMessage<PostDownloadInfoMsg>
{
	wstring m_appid;
	wstring m_platform;
	wstring m_csFileDownUrl;
	wstring m_strName;
	wstring m_appVersion;
	wstring m_csFileSavePath;
};

struct PostInstallResultInfoMsg : public BaseMessage<PostInstallResultInfoMsg>
{
	wstring m_platform;
	int  m_fid;
	wstring m_appid;
	bool bSuccess;
};


struct PostDownloadFinishMsg : public BaseMessage<PostDownloadFinishMsg>
{
	unsigned long m_nSpeed;
	wstring m_platform;
	int  m_fid;
	int m_uStatus;
	PostDownloadFinishMsg()
	{
		m_fid = 0;
		m_uStatus = 0;
	}
};

struct PostInstallAppMsg : public BaseMessage<PostInstallAppMsg>
{
	int size;
	int duration;
	PostInstallAppMsg()
	{
		size = 0;
		duration = 0;
	}
};


struct PostInstallInfoMsg : public BaseMessage<PostInstallInfoMsg>
{
	  wstring m_appid;
	  wstring m_csFileSavePath;
	  wstring m_strName;
	  wstring m_appVersion ;
	  wstring deviceId ;
};

struct PostInstallFailInfoMsg : public BaseMessage<PostInstallFailInfoMsg>
{
	wstring filePath;
	int m_fid;
	wstring m_appid;
	wstring m_appVersion;
	unsigned long TotalSize;
	wstring m_strName;
	wstring m_installerror;
	wstring deviceId;
	
	PostInstallFailInfoMsg()
	{
		m_fid = 0;
	}
	
};

//�����Ϣ��ʱ�ȷ�����
struct RequestWindowSystemInfoMsg : public BaseMessage<RequestWindowSystemInfoMsg>
{
	// �жϲ���ϵͳ�Ƿ���64λ
	bool bIs64;

	//�Ƿ���Vistaϵͳ
	bool bIsVista;

	//�Ƿ���Windows7ϵͳ
	bool bIsWindows7;

	//�Ƿ���XPϵͳ
	bool bIsXP;

	//��ȡϵͳ��Ϣ
	wstring SystemInfo;

	//��ȡ���Ե�MAC��ַ
	std::string Mac;

	//��ȡwinϵͳ�汾
	std::string Os;

	//�Ƿ���Ҫ��װitunes
	bool bIsNeedInstalliTunes;

};

struct PostDeviceConnectNumMsg : public BaseMessage<PostDeviceConnectNumMsg>
{

};

struct PostStartTimeMsg : public BaseMessage<PostStartTimeMsg>
{

};

struct PostInstallPlanListMsg : public BaseMessage<PostInstallPlanListMsg>
{
	void * pAppInfo;
};

struct PostZJAppNumOfAnPlanMsg : public BaseMessage<PostZJAppNumOfAnPlanMsg>
{
	int num;
};

struct PostZJInstallDurationMsg : public BaseMessage<PostZJInstallDurationMsg>
{
	int num;
	int allSize;
};

struct PostInstallPlanMsg : public BaseMessage<PostInstallPlanMsg>
{
	int size;
	int num;
};

struct PostZJInstallPlanNumMsg : public BaseMessage<PostZJInstallPlanNumMsg>
{
	void* pInstallsCaseDataCollection;
};