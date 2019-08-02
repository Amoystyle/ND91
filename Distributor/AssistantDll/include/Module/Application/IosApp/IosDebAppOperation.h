#pragma once

#include "IIosAppOperation.h"

namespace ND91Assistant
{
#define NDDEBINSTALLPATH		L"/private/var/root/Media/ndDaemon/deb/"
#define NDDEBINSTALLFULLNAME	L"/private/var/root/Media/ndDaemon/deb/ndapp.deb"

	//deb���͵�APP�Ĳ�����
	class CLASSINDLL_CLASS_DECL IosDebAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData * pDevice ���豸��Ϣ
		// Parameter: ProgressSubject & subject	�����Ȼص�SUBJECT
		IosDebAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosDebAppOperation(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//��װDEB����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������DEB�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//ж��DEB����
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//����DEB����
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪDEB
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner);

		// Summary  : ����ȫ��������Ϣ
		// Returns  : void
		// Parameter: wstring destinationOnComputer ����Ϣ��ŵ��ļ�������·������PC�ϣ�
		// Parameter: vector<AppData * > & vtInfos �����ڴ�ų�����Ϣ
		void AnalysisDebFile(wstring destinationOnComputer, vector<AppData*> &vtInfos);
		// ��ȡdeb�����ļ���Ԫ��
		// ������strPakage = ��������  strKey = Ԫ������Ӧ�ı�ʾ
		// ���أ�Ԫ���ַ���
private:
		wstring SearchDebKey(wstring strPakage, wstring strKey);
		// ���ݳ����Identifier��Essential�����ж�deb����������user����system
		APP_OWNER GetApplicationType(wstring appIdentifier, wstring appEssential); 

		// Summary  : ��������������Ϣ
		// Returns  : void
		// Parameter: wstring strXML ��������ϢXML����
		// Parameter: AppData * pAppData �����ڴ洢������Ϣ
		void SetDebAppData(wstring strXML, AppData* pAppData);
	};
}
