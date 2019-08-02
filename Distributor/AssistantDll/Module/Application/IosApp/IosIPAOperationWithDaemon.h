#pragma once

#include "Device/IIosFileSystem.h"
#include "Monitor/ProgressSubject.h"
#include "Module/Application/IosApp/IIosAppOperation.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
	#define NDIPAINSTALLPATH		L"/private/var/mobile/Library/NetDragon/ipa/"
	#define NDIPAINSTALLFULLNAME	L"/private/var/mobile/Library/NetDragon/ipa/ndapp.ipa"

	class  App2ExeInfo;
	//IPA���͵�APP�Ĳ����࣬ͨ���ػ�ʵ��
	class IosIPAOperationWithDaemon : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData* pDevice ���豸
		// Parameter: ProgressSubject & subject	�����Ȼص�SUBJECT
		IosIPAOperationWithDaemon(const DeviceData* pDevice, ProgressSubject& subject);
		~IosIPAOperationWithDaemon(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//��װIPA����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������IPA�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//ж��IPA����
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//����IPA����
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪIPA
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);	

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner);
	};
}
