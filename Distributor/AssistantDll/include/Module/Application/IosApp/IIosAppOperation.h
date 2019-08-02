#pragma once

#include "Monitor/ProgressSubject.h"
#include "Module/Application/IosAppXmlHelper.h"

using namespace std;

namespace ND91Assistant
{
	typedef enum eArchiveMode
	{
		eArchiveMode_All, // ��������APP�������
		eArchiveMode_DocumentsOnly, // ֻ����APP��Ӧ��Documents(����Ϸ�浵,�������õ�)
		eArchiveMode_ApplicationOnly, // ֻ����APP,������APP��Ӧ��Documents(�����ݺ������µ�APP)
		eArchiveMode_Unknow,
	};

#define DAEMON_APP_ID_ONE L"netdragon.mobile.iphone.pandadaemon"
#define DAEMON_APP_ID_TWO L"tqdigit.iphone.pandadaemon"

#define IPA_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/ndDaemon/ipa/"
#define IPA_OPERATION_COMMAND_NAME		L"ipa_install"
#define IPA_OPERATION_COMMAND_FAIL_NAME L"ipa_install.f"

#define PXL_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/PXL/Dropoff/"
#define PXL_OPERATION_COMMAND_NAME		L"PxlPickup"

#define DEB_OPERATION_COMMAND_FOLDER	L"/private/var/root/Media/ndDaemon/deb/"
#define DEB_OPERATION_COMMAND_NAME		L"debpickup"
#define DEB_OPERATION_COMMAND_FAIL_NAME L"debpickup.f"

#define APPSYNC_OPERATION_COMMAND_FOLDER	L"/var/root/Media/ndDaemon/appsync/"

	class App2ExeInfo;
	//IOS�豸��APP�������࣬�������ʵ�ֻ�ȡ����װ��ж�غͱ���4�����ܺ���
	class CLASSINDLL_CLASS_DECL IIosAppOperation
	{
        friend class IosAppOperationFactory;
        friend class AppManager;
	public:
		IIosAppOperation(const DeviceData* pDevice, ProgressSubject& subject = NullProgressSubject::Instance());
		~IIosAppOperation(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos) = 0;

		//��װ����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·����
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL) = 0;

		//ж�س���
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier) = 0;

		//���ݳ���
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+����
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, 
			wstring strVersion, eArchiveMode eMode, wstring strFileOnPc) = 0;

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner) = 0;

	protected:		

		// ��iPad���豸�Ͻ���PXL�����װ����ͼ���޸������Ժ�
        // com.apple.mobile.installation.plist�ļ�Ȩ�޻���root.600��
        // Ŀǰ��ʱû���ҵ���ɴ������ԭ���ڴ˴�����һ��Ȩ��������
		// nTimeOut ��ʱ�ȴ�ʱ��, ����Ϊ��λ
		void NotifySB(int nTimeOut = 30);

		//ִ�а�װ���ͨ���ػ���װʱʹ��,������IPA��PXL��DEB�İ�װ
		bool InstallAppService(APP_TYPE eType);		

		// Summary  : ִ��ж������
		// Returns  : bool				���ɹ�ΪTRUE
		// Parameter: APP_TYPE eType	���������ͣ�������IPA��PXL��DEB�İ�װ
		// Parameter: wstring  strIdentifier	������Ψһ��ʶ
		bool UnInstallAppService(APP_TYPE eType, wstring strIdentifier);		

		//�޸���װ��ж�غ����ͼ��
		void RepairAppIcon();

		// Summary  : �����ļ���������ݵ�ָ��Ŀ¼
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strSrcOnPhone ���豸�ϵ��ļ�·��
		// Parameter: wstring strDesOnPc ��PC�ϵ��ļ�·��
		bool DownLoadFolder(wstring strSrcOnPhone, wstring strDesOnPc);		

	private:
		wstring GetInstallCmdFileOnPC(APP_TYPE eType);
		wstring GetCmdFileOnPhone(APP_TYPE eType);
		wstring GetBackFileOnPhone(APP_TYPE eType);
		wstring GetFailFileOnPhone(APP_TYPE eType);
		bool ExcuteAppService(wstring strCmdFileOnPC, wstring strCmdFileOnPhone, wstring strBackFile, wstring strFailFile);
		wstring GetUnInstallCmdFileOnPC(APP_TYPE eType, wstring strIdentifier);
		wstring CreateCmdFileOnPC(wstring strName, wstring strContent);		

	protected:
		const DeviceInfo* _pDevInfo; //�豸��Ϣ
		ProgressSubject* _pProgressSubject; //���ȷ�����subject
		IIosFileSystem* _pFileStream; //�豸���ļ�����ָ��
		const DeviceData* _pDevData; //�豸����
	};
}