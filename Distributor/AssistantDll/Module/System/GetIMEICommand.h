#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ȡ�ֻ���������
    class GetIMEICommand : public BaseCommand
    {
    public:
        GetIMEICommand(const DeviceData* pDevice);
        virtual ~GetIMEICommand();

        // ִ������
        virtual void Execute();

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

		// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
		virtual std::wstring GetIosResultFilePath() { return L""; }

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

        std::wstring GetIMEI(){return _strIMEI;};

    private:
        std::wstring _strIMEI;
    };
}