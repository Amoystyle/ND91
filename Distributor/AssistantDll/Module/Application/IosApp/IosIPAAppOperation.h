#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//IPA���͵�APP�Ĳ�����
	class IosIPAAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData* pDevice	���豸
		// Parameter: ProgressSubject & subject		�����ȷ�����subject
		IosIPAAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosIPAAppOperation(void);

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

        // ��ȡӦ�õĳ����С���ĵ���С
        // int64_t _dynamicDiskUsage;      // �ĵ���С
        // int64_t _staticDiskUsage;       // �����С
        bool GetAppDiskUsage(map< wstring, vector<int64_t> >& diskUsages);

        // ��ȡӦ��ICON·��
        bool GetAppIconPhonePath(AppData* pApp);

        // ��ȡӦ�ù�����
        bool GetAppPurchaser(AppData* pApp);

	private:
		// Summary  : �ж��Ƿ�װAPPSYNC
		// Returns  : bool
		// Parameter: int nVerNum	: �豸�汾�ţ���1.3.3��nVerNumΪ133
		bool IsInstallAppSync(int nVerNum);
		// Summary  : ��ȡIPA�������Ψһ��ʶ
		// Returns  : std::wstring
		// Parameter: wstring strIpaFullName	��IPA����������ƣ���·����PC�ϣ�
		wstring GetIpaAppID(wstring strIpaFullName);
	};
}
