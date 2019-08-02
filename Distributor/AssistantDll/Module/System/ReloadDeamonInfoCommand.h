#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // ��ȡ�ػ���Ϣ����
    class ReloadDeamonInfoCommand : public BaseCommand
    {
    public:
        ReloadDeamonInfoCommand(const DeviceData* pDevice);
        virtual ~ReloadDeamonInfoCommand();

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

        string GetMacAddress() { return _mac; }

    private:
        string  _deamonVersion;     // �ػ��汾
        string  _pwd;               // wifi��������
        bool    _isWifiOn;          // �豸�Ƿ�����wifi����
        string  _imei;              // IMEI
        string  _mac;               // mac��ַ
        string  _model;             // Ӳ���ͺ�
    };
}

