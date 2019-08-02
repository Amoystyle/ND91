#pragma  once

#include "Module/BaseCommand.h"
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // ��������������
    class UpdateContactCommand : public BaseCommand
    {
    public:
        UpdateContactCommand(const DeviceData* pDevice, ContactData* pContact, bool bVersion20, bool bOMS);
        virtual ~UpdateContactCommand();

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


        ContactData* _pContactData; // �����µ�����������

        bool _success;              // �Ƿ�ִ�гɹ�

        bool _bVersion20;           // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;                 // �Ƿ�ΪOMSϵͳ
    };
}