#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class IIosFileSystem;

	//ͨ���ػ���ȡ�豸����
    class GetIosDeviceTypeCommand : public BaseCommand
	{
	public:
		GetIosDeviceTypeCommand(const DeviceData* pDevice);
        ~GetIosDeviceTypeCommand() {}

		// ִ������
		virtual void Execute();

        // ȡ���ص��豸���ͣ�����Execute֮����ã�
        std::wstring GetDeviceType();

    private:		

        // ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData() { return "";}

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data) {}

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

	private:
		std::wstring _deviceType;       // �豸����
	};
}
