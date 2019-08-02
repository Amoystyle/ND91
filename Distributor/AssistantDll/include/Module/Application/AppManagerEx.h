#pragma once

#include "AppManager.h"

// ������������
namespace ND91Assistant
{

	class CLASSINDLL_CLASS_DECL AppManagerEx
	{
	public:
		AppManagerEx(DeviceData* pDevice);
		~AppManagerEx(void);

		// ����App�б����صĽ��������_datas
		CommandExecuteResult Load();
		// ��װApp
		// strAppFullName ������PC�ϵ�·��
		// string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		// wstring &AppId ��װ�ɹ����ذ�װ��id
		// bool bLock=true �Ƿ�����, 
		// bool bRecover=true �Ƿ񸲸�, 
		// bool bOnSDCard=true �Ƿ�װ��SdCard, Android2.2�̼�������Ч
		// AppInExeType ��׺����exe���ƶ�Ӧ�ð�װ����Ҫ˵��exe��������ĵ��������ֺ�׺����app���ݰ�
		CommandExecuteResult InstallApp(wstring strAppFullName, wstring& errorMsg, wstring &AppId, ProgressObserver* pObs=NULL, bool bLock=false, bool bRecover=true,
			bool bOnSDCard=true, bool isMove=false,App2ExeInfo* AppInExeInfo=NULL);
		// ж��App
		CommandExecuteResult UnInstallApp(wstring AppId, ProgressObserver* pObs = NULL);
		// ���ݳ���
		CommandExecuteResult ArchiveApp(wstring AppId, wstring strFileOnPC, ProgressObserver* pObs = NULL);
		// ��ȡӦ���� ֻ��ȡ��Ŀ�����ٶ��������Ż�
		int  GetAppCount(APP_OWNER appOwner);
        int  GetAppCount(bool bOnSDCard);
		// �ͷ��Ѽ��ص�����
		void ReleaseData();
		// ȡ��AppId�б�
		const vector<wstring> GetAppIds();
		//����appidȡ��appdata��Ϣ
		//��Ҫ�ⲿ�ͷŷ��ص�appdata
		//AppData* GetAppDataCopyByID( wstring AppdataID );
        bool GetData(wstring AppdataID, AppData& appData);

		// ͼ���޸�����ios�豸��
		CommandExecuteResult RepairIcon();
		// Summary  : PXL��װ��תIPA��װ��
		// Returns  : std::wstring ����ת�����IPA��������·����
		// Parameter: wstring strPxlFullName ��PXL��װ��������·����
		wstring PxlToIpa(wstring strPxlFullName);
		//��������
		//IOSδ�в鿴���̵Ľӿڣ���˴�����ֻ���ڽ����ػ�������(ж���ػ���)
		CommandExecuteResult KillDeamonProcess(wstring strProcessName);
        // ��װ�ػ����� 
        // strDaemonFullName:�ػ�zipѹ�����ļ�ȫ��(��·��)
        // daemonVersionOnService:�ػ��ڷ������ϵİ汾��
        // ��װ�����Ҫ����
        CommandExecuteResult InstallDaemon(wstring strDaemonFullName, wstring daemonVersionOnService);
		//ж���ػ�
		//ж�غ�Ҫ����������ȫж��
		CommandExecuteResult UnInstallDaemon();

		// Summary  : �ƶ�����(android)
		// Returns  : ND91Assistant::CommandExecuteResult
		// Parameter: AppData * pAppData Ҫ�ƶ���Ӧ��
		// Parameter: string & errorMsg ʧ�ܷ��ش�����Ϣ
		// Parameter: AppData * pRetAppData �ɹ������°�װ��Ӧ������
		// Parameter: ProgressObserver * pObs
		CommandExecuteResult MoveToApp(wstring AppId, wstring& errorMsg, AppData* &pRetAppData, ProgressObserver* pObs = NULL);

		//���ݰ�װ����ȡӦ�ó�����Ϣ�ӿڣ����һ��������ʾ�Ƿ�Ҫ��ȡͼ��
		static AppData* GetAppInfoByFile(wstring filepath, wstring tempdir,  bool icon=false);

		//�ж��Ƿ�װ��ĳ����� 
		bool IsInstallApp(wstring packageName);

		// �����޸�
		// ���� 0��֧����������
		// -1 �豸�̼��汾����4.0
		// -2 ���ӷ�ʽ����USB
		// -3 iTunes�汾����
		// -4 δ��װiTunes
		// -5 iTunes�汾Ϊ���װ�
		int FixFlashExit();

		// ��ȡӦ�õĳ����С���ĵ���С
		bool GetAppDiskUsage();

        // ��ȡӦ��ͼ��
        bool GetAppIcon(wstring packageName, wstring strIconPath);
        // ��ȡӦ��Ȩ���б�
        bool GetAppPermissionList( wstring packageName, vector<wstring>& permissionList );
	private:
		const AppData* GetData(wstring AppdataID);
		AppManager _AppManager;
        DeviceData*	_pDevice;
        CRITICAL_SECTION _csLock;
	};
}