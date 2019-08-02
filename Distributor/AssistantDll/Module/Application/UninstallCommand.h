#pragma once

#include "Module/BaseCommand.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
    class AppData;

    //���ֻ���ж���ļ�
    class UninstallCommand : public BaseCommand
    {
    public:
		// AppType			: App�����ͣ��������ֲ�ͬ�ĵ�������
		// strIdentifier	: App��Ψһ��ʶ
        UninstallCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~UninstallCommand();

		// ִ������
		virtual void Execute();

		// ����Android�����壬
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ��������ķ��ذ�ͷ
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );

		// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// ���������ļ���������ɾ���ļ�
		virtual  void SetIosReturnFile(std::wstring filename) {}
		
		// Summary  : ж��һ��App �ļ򵥵��÷���  ��̬
		// Returns  : bool �Ƿ���ɹ�
		// Parameter: const DeviceData * pDevice �豸
		// Parameter: wstring strPackageName ����
		// Parameter: APP_TYPE appType APP����
		// Parameter: bool bKeepData �Ƿ�����������
		static bool SimpleUninstallApp(const DeviceData* pDevice, wstring strPackageName, APP_TYPE appType, bool bKeepData=true);
	private:
		const AppData* _appData;
		vector<const AppData*> _dataList;
    };
}

