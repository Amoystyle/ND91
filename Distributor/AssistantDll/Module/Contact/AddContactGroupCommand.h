#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class ContactGroupData;

    // �����ϵ�˷�������
    class AddContactGroupCommand : public BaseCommand
    {

    public:
        AddContactGroupCommand(const DeviceData* pDevice,
            /*ContactGroupData* pGroupData*/
            const vector<ContactGroupData*>* pDataList,
            bool bVersion20, bool bOMS);
        virtual ~AddContactGroupCommand();

        // ִ������
        virtual void Execute();

        // �����Ƿ�ִ�гɹ�
        bool Success();

    private:
        // ִ��IOS����
        bool IosExecute();

    private:
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

        //ContactGroupData* _pGroupData; // ����ӵ���ϵ�˷�������
        const vector<ContactGroupData*>* _pDataList;// ����ӵ���ϵ�˷�������

        bool _success;          // �Ƿ�ִ�гɹ�

        bool _bVersion20;       // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;             // �Ƿ�ΪOMSϵͳ
		ContactGroupData* _pData;

    };
}