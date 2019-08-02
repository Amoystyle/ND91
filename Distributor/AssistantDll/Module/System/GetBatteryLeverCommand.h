#pragma once
#include "Module/BaseCommand.h"
namespace ND91Assistant
{
	// ��ȡ�豸������Ϣ 
	class GetBatteryLeverCommand : public BaseCommand
	{
	public:
		GetBatteryLeverCommand(const DeviceData* pDevice);
		~GetBatteryLeverCommand();

		// ִ������
		virtual void Execute();

		// ����Android�������ݰ�
		virtual std::string GetAndroidBodyData();

		// ����Android���ؽ�����ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
		virtual std::wstring GetIosCommandFilePath() { return L""; }

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent() { return L""; }

		// ���������ļ���������ɾ���ļ�
		virtual  void SetIosReturnFile(std::wstring filename) { }

		int GetBatteryLever(){return _nBatteryLever;};

	private:
		int _nBatteryLever;
	};
}