#pragma once

#include "Module/Application/IosApp/IIosAppOperation.h"

namespace ND91Assistant
{
#define PXL_NORMAL_PATH		L"/private/var/root/Media/PXL/Dropoff/ndapp.pxl"

	//PXL���͵�APP�Ĳ�����
	class IosPXLAppOperation : public IIosAppOperation
	{
	public:
		// Summary  : ���캯��
		// Returns  : 
		// Parameter: const DeviceData* pDevice ���豸
		// Parameter: ProgressSubject & subject	�����Ȼص�SUBJECT
		IosPXLAppOperation(const DeviceData* pDevice, ProgressSubject& subject);
		~IosPXLAppOperation(void);

		//��ȡָ�����͵����г������Ϣ�б�
		//eType		: Ӧ�ó�������
		virtual bool GetAppInfos(vector<AppData*> &vtInfos);

		//��װPXL����
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������PXL�ļ�����FALSE
        //string& errorMsg ��װʧ�ܷ��ش�����Ϣ
		virtual CommandExecuteResult InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo=NULL);

		//ж��PXL����
		//strIdentifier	: ΪAppData���_PackageName
		virtual bool UnInstallApp(wstring strIdentifier);

		//����PXL����
		//strIdentifier	: ΪAppData���_PackageName
		//strAppPhonePath ��ΪAppData���_strAppPhonePath
		//strAppName ��ΪAppData���_AppName
		//strVersion ��ΪAppData���_strVersion
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪPXL
		virtual CommandExecuteResult ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc);

		// Summary  : ������ȡӦ�õ�����
		// Returns  : int ����Ӧ���������޻�ʧ�ܾ�����0
		virtual int GetAppCount(APP_OWNER appOwner);

	private:		

		// Summary  : ��ⰲװ���Ƿ����Ҫ��
		// Returns  : bool ���԰�װ����TRUE,����ΪFALSE
		// Parameter: wstring strAppFullName ����������·����
		// Parameter: wstring & strIdentifier ��Ӧ�ó����Ψһ��ʶ
		// Parameter: wstring & strUnZipPath  ����ѹ���·��
		// Parameter: bool b7z  ���Ƿ�Ϊ7z�ļ�
		bool CheckInstallPkg(wstring strAppFullName, wstring &strIdentifier, wstring &strUnZipPath);

		// Summary  : �ϴ���ͨѹ����ʽ�ĳ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strAppFullName �������������·����
		bool UpLoadNormalApp(wstring strAppFullName,__int64 AppOffsetInExe=0,unsigned __int64 AppSize=0);		
	};
}
