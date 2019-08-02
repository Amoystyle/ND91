#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
	//Panda���͵�APP�Ĳ�����
	class IosPandaAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData* pDevice ���豸
		// Parameter: ProgressSubject & subject	�����Ȼص�SUBJECT
		IosPandaAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosPandaAppOperation(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//��װPanda����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������Panda�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo = NULL);

		//ж��Panda����
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//����Panda����
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪPanda
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner);

	private:
		// Summary  : ����ȫ��������Ϣ
		// Returns  : void
		// Parameter: wstring destinationOnComputer ����Ϣ��ŵ��ļ�������·������PC�ϣ�
		// Parameter: vector<AppData * > & vtInfos �����ڴ�ų�����Ϣ
		void AnalysisPandaFile(wstring destinationOnComputer, vector<AppData*> &vtInfos);
		// Summary  : ��������������Ϣ
		// Returns  : void
		// Parameter: wstring strXML ��������ϢXML����
		// Parameter: AppData * pAppData �����ڴ洢������Ϣ
		void SetPandaAppData(wstring strXML, AppData* pAppData);
	};
}
