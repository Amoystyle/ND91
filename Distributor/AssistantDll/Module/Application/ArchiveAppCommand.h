#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class DeviceData;
    class AppData;

	class ArchiveAppCommand : public BaseCommand
	{
	public:
		// AppType			: App�����ͣ��������ֲ�ͬ�ĵ�������
		// strIdentifier	: App��Ψһ��ʶ
		// strFileOnPC		�����ݺ��ŵ��ļ�������·����
		ArchiveAppCommand(const DeviceData* pDevice, const AppData* appData, wstring strFileOnPC);
		~ArchiveAppCommand(void);

		// ִ������
		virtual void Execute();

		// ����Android�����壬
		virtual std::string GetAndroidBodyData(){return "";}

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data){}

		// ��������ķ��ذ�ͷ
		bool parseAndroidPackageHead( BinaryBufferReader* pReader ){return false;}

		// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// ���������ļ���������ɾ���ļ�
		virtual  void SetIosReturnFile(std::wstring filename) {}

	private:
		const AppData* _appData;
		wstring _strFileOnPC;	//���ݺ��ŵ��ļ�������·����
	};
}
