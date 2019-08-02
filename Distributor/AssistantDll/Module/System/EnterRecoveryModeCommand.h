#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
	
	//����ָ�ģʽ
	class EnterRecoveryModeCommand : public BaseCommand
	{
	public:
		EnterRecoveryModeCommand(const DeviceData* pDevice);
		~EnterRecoveryModeCommand() {}

		// ִ������
		virtual void Execute();


	private:		

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData() { return "";}

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){};

	private:

	};
}
