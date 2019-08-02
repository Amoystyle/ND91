#pragma once

#include "Module/BaseCommand.h"
#include "Module/Application/AppData.h"

namespace ND91Assistant
{
	// ���ݱ�ʶstrPackageName ��ȡĳ���������Ϣ 
	class GetAppInfoCommand : public BaseCommand
	{
	public:
		GetAppInfoCommand(const DeviceData* pDevice, wstring strPackageName);
		~GetAppInfoCommand();
		// ִ������
		virtual void Execute();

		const AppData* GetAppInfo(wstring strPackageName);

#pragma region

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
		virtual std::wstring GetIosCommandFilePath(){ return L""; }

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath(){ return L""; }

		// ����ios�ļ�
		virtual std::wstring GetIosCommandFileContent(){ return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){}

#pragma endregion

		// Summary  : ��ȡȡ����App��Ϣ
		// Returns  : const AppData*
		const AppData* GetAppInfo() { return &_appData; };
	private:
		wstring _strPackageName;
		AppData _appData;
	};
}