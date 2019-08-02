#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//Installer���͵�APP�Ĳ�����
	class IosInstallerAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData* pDevice ���豸
		// Parameter: ProgressSubject & subject	�����Ȼص�SUBJECT
		IosInstallerAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosInstallerAppOperation(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//��װInstaller����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������Installer�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//ж��Installer����
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//����Installer����
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪInstaller
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner);

	private:
		// Summary  : ����ȫ��������Ϣ
		// Returns  : void
		// Parameter: wstring destinationOnComputer ����Ϣ��ŵ��ļ�������·������PC�ϣ�
		// Parameter: vector<AppData * > & vtInfos �����ڴ�ų�����Ϣ
		void AnalysisInstallerFile(wstring destinationOnComputer, vector<AppData*> &vtInfos);
		// Summary  : ��������������Ϣ
		// Returns  : void
		// Parameter: wstring strXML ��������ϢXML����
		// Parameter: AppData * pAppData �����ڴ洢������Ϣ
		void SetInstallerAppData(wstring strXML, AppData* pAppData);

	};
}
