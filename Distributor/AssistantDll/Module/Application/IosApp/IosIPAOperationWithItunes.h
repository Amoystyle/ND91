#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"
#include "Device/IosUsbConnection.h"

using namespace std;

namespace ND91Assistant
{
    // ipa��ز�����������
    enum IPAOperationErrorType
    {
        None,                           // û�д���
        ApplicationVerificationFailed,  // Ӧ�ó���ǩ��ʧ��
        IncorrectArchitecture,          // ����ĳ���ܹ�����
        InstallProhibited,              // �豸��ֹ��װӦ��
        PackageExtractionFailed,        // ����ѹʧ��(4.x��װ���Ҳ�����װ·����ʱ��,������)
        PackageInspectionFailed,        // ������ʧ��(ipa��������)
        APIInternalError,               // ��װAPI�ڲ�����,��β��ֻ�������.(ָ����װ·��������app,6.x�̼����������)
        UnknowError
    };

    class ProgressSubject;

    #define ApplicationArchivesFolder L"ApplicationArchives" //�����ļ��洢��λ��

	//IPA���͵�APP�Ĳ����࣬ͨ��itunes�ӿ�ʵ��
	class IosIPAOperationWithItunes
	{

	public:
		IosIPAOperationWithItunes(void);
		~IosIPAOperationWithItunes(void);

	public:

		ProgressSubject& GetSubject() {return *_progressSubject;}

		//��ȡָ�����͵����г������Ϣ�б�
		//pDevPtr	: ΪDeviceInfo���_ios_devptr
		//pAfcHandle: ΪDeviceInfo���_ios_afcHandle
		//bJailbreaked: �Ƿ�Խ��
		//eType		: Ӧ�ó�������
		bool GetAppInfos(DeviceInfo* devInfo, APP_OWNER eType, vector<AppData*> &vtInfos);

		//��װIPA����
		//pDevice		: �豸����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������IPA�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		CommandExecuteResult InstallApp(const DeviceData* pDevice, wstring strAppFullName, string& errorMsg, ProgressSubject& subject = NullProgressSubject::Instance());

		//ж��IPA����
		//pDevPtr		: ΪDeviceInfo���_ios_devptr
		//strIdentifier	: ΪAppData���_PackageName
		bool UnInstallApp(const DeviceInfo* pDevInfo, wstring strIdentifier);

		//����IPA����
		//pDevPtr		: ΪDeviceInfo���_ios_devptr
		//pAfcHandleApp	: ΪDeviceInfo���_ios_afcHandleApp
		//strIdentifier	: ΪAppData���_PackageName
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪIPA
		CommandExecuteResult ArchiveApp(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode,
            wstring strFileOnPc, ProgressSubject& subject = NullProgressSubject::Instance());

		// Summary  : ��ȡӦ������
		// Returns  : int ����Ӧ������
		// Parameter: void * pDevPtr : ΪDeviceInfo���_ios_devptr
		// Parameter: APP_OWNER appOwner
		int GetAppCount(void* pDevPtr, APP_OWNER appOwner);

		//App2exe�İ�װ��ʽ
		CommandExecuteResult InstallAppEx(const DeviceData* pDevice, wstring strAppFullName, 
			string& errorMsg, ProgressSubject& subject,
			App2ExeInfo* exeInfo);

        // ��ȡӦ�õĳ����С���ĵ���С
        // int64_t _dynamicDiskUsage;      // �ĵ���С
        // int64_t _staticDiskUsage;       // �����С
        bool GetAppDiskUsage(DeviceInfo* pDevInfo, map< wstring, vector<int64_t> >& diskUsages);

        // ��ȡӦ��ICON·��
        bool GetAppIconPhonePath(DeviceInfo* pDevInfo, AppData* pApp);

        // ��ȡӦ�ù�����
        bool GetAppPurchaser(DeviceInfo* pDevInfo, AppData* pApp);

	private:

		// ��װIPA���򣨲���ֱ���ϴ�ipa�ļ���װ�ķ���
		// string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		bool OldInstallApplication(wstring strAppFullName, string& errorMsg,unsigned __int64 AppOffsetInExe=0,unsigned __int64 AppSize=0);
        int InstallAppInZipMode(wstring strAppFullName, string& errorMsg);

        // parameter: bool isGetAppDiskUsage �Ƿ��ȡ��װ����Ĵ���ʹ����Ϣ����ǰֻ��USER���͵ĲŻ᷵�ش���ʹ����Ϣ,�̼�Ҫ4.0������,�����Բ���Ч��
		bool GetAppInfos(void* pDevPtr, APP_OWNER eType, map<std::string, void*> &mapInfos, bool isGetAppDiskUsage = false);
        // ���û�ȡ��װ����Ĵ���ʹ����Ϣ�ķ�����Ϣ
        void SetAppDiskUsageSendInfo(map<string, void*>* srcDict);
		void PraseAppInfos(DeviceInfo* devInfo, map<std::string, void*> mapInfos, vector<AppData*> &vtInfos);

		//�����򱸷����豸��
		bool ArchiveAppOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall = false);
		//ɾ������
		bool RemoveAppArchiveOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier);

		wstring GetPurchaser(AFCFileSysemBase* pFileStream, wstring strMetadataFullName);

		//��װAPP���Ȼص�
		friend void OnPackInstallCallBack(void* pSender, void* pEvent);
		//ʹ��ITUNESĬ���ϴ�APP���Ľ��Ȼص�
		friend void OnPackDataTransferCallBack(void* pEvent);
		//ɾ��APP���ݵĽ��Ȼص�
		friend void OnRemoveIPAArchiveCallback(void* pSender, void* pEvent);
		//����APP�Ľ��Ȼص�
		friend void OnArchiveIPACallback(void* pSender, void* pEvent);

		friend void PraseProgress(void* pCFData);

        // ����KEY��XML�ļ���ȡVALUE
        wstring GetStringValue(map<string, void*> appInfoNormal, string key);
        vector<wstring> GetVectorStringValue(map<string, void*> appInfoNormal, string key);
        bool GetBoolValue(map<string, void*> appInfoNormal, string key);
        int64_t GetInt64Value(map<string, void*> appInfoNormal, string key);

        // ����������Ϣ�е�ͼ��·��
        void PraseIconPhonePath(DeviceInfo* devInfo, AppData* pData);

        // ����������Ϣ�еĹ�������Ϣ
        void PrasePurchaser(DeviceInfo* devInfo, AppData* pData);

        // ���ж�AFC�����Ƿ�����,�쳣�Ļ������÷���.
        // <remarks>����һЩ�����ᵼ��AFC�ҵ�,��˺�����Ҫafc�ĵط�������Ҫ����afc����.
        // ���簲װѹ������ʽ���淶 ND591up0712.ipa��ᵼ��AFC����ҵ�,��˺����İ�װ��Ҫ����AFC����</remarks>
        void RestartAFCServiceIfNeed(IosUsbConnection iosConet);

        // ��װIPA����(�ڲ��Զ�ѡ��һ�ַ�ʽ��װ����ʧ�ܻ��Զ���������������
        // string& errorMsg ��װʧ�ܷ��ش�����Ϣ
        bool InstallApplication(wstring strAppFullName, string& errorMsg);

        // ���ٰ�װIPA����ֱ�ӵ���iTUnes�ṩ�Ŀ��ٰ�װ����
        int ItunesFastInstallApplication(wstring strAppFullName);
        int InstallAppInUnzipModeWithiTunesInterface(wstring strAppFullName, string& errorMsg);

        // ���ٰ�װIPA�������ֽ�ѹ�ļ�����߳��ϴ�-Ȼ�����iTunes��װ����
        int FastInsallApplication(wstring strAppFullName);
        int InstallAppInUnzipMode(wstring strAppFullName, string& errorMsg);

        // ���Ӧ���Ƿ��ʺϰ�װ����ǰ�̼��豸
        // AppData* pApp Ӧ����Ϣ
        int PreparatoryCheckAppInstall(AppData* pApp);

        // ����豸�Ƿ��ʺ���Ȩ
        int CheckValideToAuthorize(int iValideToAuthorize);

        //  ��װ����
        int InstallAppSubProcedure(wstring strAppInstallFileOnPhone, wstring strAppFullName, string& errorMsg);

        // ͨ��Э��ʵ��,��װ�Ѿ��ϴ����豸�ϵ�APP,ʵ�ֺ�iTunes AMDeviceInstallApplicationһ������
        bool InstallAppOnDeviceWithProtocol(wstring strAppInstallFileOnPhone, void* clientOptionsDict, string& errorMsg);
        // ��װ,����.ж��,������ִ�еĲ���
        bool InstallationProxyPerformOperation(string operation, map<string, void*> msgDict, string& errorMsg);

        // ��ȡ������Ϣ�����԰�����
        // string errorMsg �����ʶ 
        // int iErrCode ������
        string GetAMDErrorByDescription(string errorMsg, int iErrCode);

        // ��info.ini�ļ���������Ĺ����ĵ�����,�Ա��ʶ�������������IPA
        // wstring identifier Ӧ��ID
        // wstring acountInfo 91IPA����˻���Ϣ
        void Upload91IPAInfo2Documents(wstring identifier, wstring acountInfo);

	private:
        int                 _progressId;    // �ص�����id���ο�IosIPAItunesManager
		ProgressSubject*    _progressSubject;
		APP_OWNER           _eType;

        const DeviceInfo*   _pDevInfoPtr;
        void*               _pAfcSocketApp;
        void*               _pAfcHandleApp;

        wstring             _identifier;    // ����װ��Ӧ��ID
		
	};
}
