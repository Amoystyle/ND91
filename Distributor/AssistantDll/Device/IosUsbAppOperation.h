#pragma once

#include "../Monitor/ProgressSubject.h"
#include "../Data/AppData.h"
#include "IosAppOperation/IIosAppOperation.h"
#include <vector>
#include <map>

using namespace std;

namespace ND91Assistant
{
	
#define ApplicationArchivesFolder L"ApplicationArchives" //�����ļ��洢��λ��

	/************************************************************************/
	/* IOS�豸USBģʽ��Ӧ�ó���Ĳ���
	������ȡ���г����б���װ��ж�غͱ���,���ڽ�����ʱ�޷����ֹ���������ֻ����������
	WIFIģʽֻ��ͨ���ػ���װ*/
	/************************************************************************/
	class IosUsbAppOperation
	{
	public:
		static IosUsbAppOperation* GetInstance();

	private:
		//pDevPtr	: USBģʽ������/�Ͽ�ʱ�򷵻ص��豸���
		IosUsbAppOperation(void);
		~IosUsbAppOperation(void);

	public:

		ProgressSubject& GetSubject() {return *_progressSubject;}

		//��ȡָ�����͵����г������Ϣ�б�
		//pDevPtr	: ΪDeviceInfo���_ios_devptr
		//eType		: Ӧ�ó�������
		bool GetAppInfos(void* pDevPtr, eApplicationType eType, vector<AppData*> &vtInfos);

		//��װIPA����
		//pDevPtr		: ΪDeviceInfo���_ios_devptr
		//pAfcSocketApp	: ΪDeviceInfo���_ios_afcSocketApp
		//pAfcHandleApp	: ΪDeviceInfo���_ios_afcHandleApp
		//strAppFullName: Ӧ�ó���PC�ϵ��ļ�������·��������IPA�ļ�����FALSE
		bool InstallApp(void* pDevPtr, void* pAfcSocketApp, void* pAfcHandleApp, wstring strAppFullName, ProgressSubject& subject = NullProgressSubject::Instance());

		//ж��IPA����
		//pDevPtr		: ΪDeviceInfo���_ios_devptr
		//strIdentifier	: ΪAppData���_Identifier
		bool UnInstallApp(void* pDevPtr, wstring strIdentifier);

		//����IPA����
		//pDevPtr		: ΪDeviceInfo���_ios_devptr
		//pAfcHandleApp	: ΪDeviceInfo���_ios_afcHandleApp
		//strIdentifier	: ΪAppData���_Identifier
		//eMode			: ��������
		//strFileOnPc	: �����ļ������PC�ϵ�λ�ã�·��+������Ϊ��չ��ΪIPA
		bool ArchiveApp(void* pDevPtr, void* pAfcHandleApp, wstring strIdentifier, eArchiveMode eMode, wstring strFileOnPc, ProgressSubject& subject = NullProgressSubject::Instance());

	private:
		bool GetAppInfos(void* pDevPtr, eApplicationType eType, map<std::string, void*> &mapInfos);
		void PraseAppInfos(map<std::string, void*> mapInfos, vector<AppData*> &vtInfos);

		//�����򱸷����豸��
		bool ArchiveAppOnPhone(void* pDevPtr, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall = false);
		//ɾ������
		bool RemoveAppArchiveOnPhone(void* pDevPtr, wstring strIdentifier);

		//��װAPP���Ȼص�
		friend void OnPackInstallCallBack(void* pSender, void* pEvent);
		//ʹ��ITUNESĬ���ϴ�APP���Ľ��Ȼص�
		friend void OnPackDataTransferCallBack(void* pEvent);
		//ɾ��APP���ݵĽ��Ȼص�
		friend void OnRemoveIPAArchiveCallback(void* pSender, void* pEvent);
		//����APP�Ľ��Ȼص�
		friend void OnArchiveIPACallback(void* pSender, void* pEvent);

		friend void PraseProgress(void* pCFData);

	private:
		static IosUsbAppOperation* _pInstance;
		ProgressSubject* _progressSubject;
	};
}
