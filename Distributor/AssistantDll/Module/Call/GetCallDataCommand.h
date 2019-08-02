#pragma once

#include "Module/Call/CallData.h"

namespace ND91Assistant
{
    // ȡͨ����¼����
    class GetCallDataCommand : public BaseCommand
    {
    public:

        GetCallDataCommand(const DeviceData* pDevice);
        virtual ~GetCallDataCommand();

        // �����Ƿ�ֻ��ȡ��������
        void SetOnlySize(bool onlySize);

        // ִ������
        virtual void Execute();

        // ��������
        const vector<CallData*>* GetDataList();

        // ������������
        int GetSize();

    private:

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

        // ִ��IOS����
        bool IosExecute();

    private:
        vector<CallData*> _dataList;
        
        bool _onlySize;// �Ƿ�ֻ��ȡ����

        int _size;     // ��������
    };
}