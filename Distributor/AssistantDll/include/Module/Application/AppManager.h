#pragma once

#include "Module/Application/AppData.h"
#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"

// ������������
namespace ND91Assistant
{
    class ProgressObserver;
	class CLASSINDLL_CLASS_DECL App2ExeInfo
	{
	public:
		wstring AppInExeType;
		unsigned __int64 OffsetAppInExe;
		unsigned __int64 len;
		wstring backdir;
		wstring deamondir;
	};
    class CLASSINDLL_CLASS_DECL AppManager
    {
    public:
        AppManager(DeviceData* pDevice);
        ~AppManager(void);

        // ����App�б����صĽ��������_datas
        CommandExecuteResult Load();
        // ��װApp
        // strAppFullName ������PC�ϵ�·��
        // string& errorMsg ��װʧ�ܷ��ش�����Ϣ
        // AppData* pRetData ��װ�ɹ����ذ�װ��Ӧ�����ݣ�ʧ�ܷ��ؿ�
        // bool bLock=true �Ƿ�����, 
        // bool bRecover=true �Ƿ񸲸�, 
        // bool bOnSDCard=true �Ƿ�װ��SdCard, Android2.2�̼�������Ч
		// AppInExeType ��׺����exe���ƶ�Ӧ�ð�װ����Ҫ˵��exe��������ĵ��������ֺ�׺����app���ݰ�
        CommandExecuteResult InstallApp(wstring strAppFullName, wstring& errorMsg, AppData* &pRetData, ProgressObserver* pObs=NULL, bool bLock=false, bool bRecover=true,
			bool bOnSDCard=true, bool isMove=false,App2ExeInfo* AppInExeInfo=NULL);
        // ж��App
        CommandExecuteResult UnInstallApp(AppData* pAppData, ProgressObserver* pObs = NULL);
        // ���ݳ���
        CommandExecuteResult ArchiveApp(AppData* pAppData, wstring strFileOnPC, ProgressObserver* pObs = NULL);
        // ��ȡӦ���� ֻ��ȡ��Ŀ�����ٶ��������Ż�
        int  GetAppCount(APP_OWNER appOwner);
        int  GetAppCount(bool bOnSDCard);
        // �ͷ��Ѽ��ص�����
        void ReleaseData();

        // ȡ��_datas
        const vector<AppData*>* GetDatas();
		
		 // ȡ��_datas
		void GetDatas(vector<AppData*>& vecData);
        void ReleaseGetDatas();
		AppData* FindAppInfoByID(wstring AppdataID);

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
        CommandExecuteResult MoveToApp(AppData* pAppData, wstring& errorMsg, AppData* &pRetAppData, ProgressObserver* pObs = NULL);

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

        // ��ǰ�豸�Ƿ��ʺ���Ȩ
        // ���� 0���ʺ���Ȩ
        // -1 �豸�̼��汾����4.0
        // -2 ���ӷ�ʽ����USB
        // -3 iTunes�汾����
        // -4 δ��װiTunes
        // -5 iTunes�汾Ϊ���װ�
        int IsValideToAuthorize();

        // ��ȡӦ�õĳ����С���ĵ���С
        bool GetAppDiskUsage();

        // Summary  : ��װ����໥������DEBӦ��
        // Returns  : bool
        // Parameter: vector<wstring> remotePaths ���deb�ļ����豸�ϵ�·��
        bool InstallSpecialDebs(vector<wstring> remotePaths);

        // Summary  : ��װAppsync
        bool InstallAppsyncService();

	private:

		//��ȡ��װ���г���ͼ��
		static wstring upzipIcon(wstring zipFilePath, wstring tempIcondir, wstring iconName,wstring appname,wstring appVersion);

        // Summary  : �ٴ��ж�Ӧ�ð�װ���
        // Returns  : ��װ���
        // Parameter: CommandExecuteResult installResult ���ð�װ�ӿڷ��صİ�װ���
        // Parameter: AppData* pPackData Ӧ�ð�����
        // Parameter: AppData* pRetData ��װ�ɹ����ذ�װ��Ӧ�����ݣ�ʧ�ܷ��ؿ�
        CommandExecuteResult JudgeInstallAppResult(CommandExecuteResult installResult, AppData* pPackData, AppData* &pRetData);

        // ж����ɺ���б����Ƴ�
		void RemoveAppFromList(AppData* pAppData);
        void RemoveAppFromList(wstring appID);

		//����ļ�������ȡ�������ļ���
		static bool GetRightFileName(wstring &oldname,wstring &filename);

    private:
        DeviceData*	_pDevice;
        vector<AppData*> _datas;
        vector<AppData*> _newDatas;     // �½�һ�ݹ�������ʹ��
        static AppManager* _pInstance; // ����ʵ��
        CRITICAL_SECTION _cs;//�����ٽ�ֵ
        bool _bLoading;
    };
}