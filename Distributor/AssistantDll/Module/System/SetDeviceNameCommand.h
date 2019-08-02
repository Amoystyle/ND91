#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

	class SetDeviceNameCommand: public BaseCommand
	{
	public:
		SetDeviceNameCommand(const DeviceData* pDevice, std::wstring strNewName);
		~SetDeviceNameCommand(void);

		// ִ������
		virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath(){return L"";}

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent(){return L"";}

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename){}

#pragma endregion

	private:
		std::wstring _strNewName;
	};
}
