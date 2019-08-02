#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

	class KillProcessCommand: public BaseCommand
	{
	public:
		KillProcessCommand(const DeviceData* pDevice, std::wstring strProcessName);
		~KillProcessCommand(void);

		// ִ������
		virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData(){return "";};

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath();

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent();

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		std::wstring _strProcessName;
	};
}