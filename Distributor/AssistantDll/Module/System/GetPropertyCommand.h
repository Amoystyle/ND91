#pragma once

#include "Module/BaseCommand.h"
#include "Core/DeviceInfo.h"

#define ANDROID_SHELL_GETPROPERTY       "getprop"

namespace ND91Assistant
{
    // ȡӲ����Ϣ����
    class GetPropertyCommand : public BaseCommand
    {
    public:
        GetPropertyCommand(const DeviceData* pDevice);
        virtual ~GetPropertyCommand();

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

        DeviceInfo* GetDeviceInfo(){return &_DeviceInfo;};
        bool Success(){return _success;};

    private:
        //�洢���������Ϣ��ע������캯����������DeviceInfoָ�����ֿ�
        DeviceInfo _DeviceInfo;
        bool _success;
    };
}