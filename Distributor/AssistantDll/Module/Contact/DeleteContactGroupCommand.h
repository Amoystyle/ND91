#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactGroupData;

    // ɾ����ϵ�˷�������
    class DeleteContactGroupCommand : public BaseCommand
    {

    public:
        DeleteContactGroupCommand(const DeviceData* pDevice, const ContactGroupData* pGroupData);
        virtual ~DeleteContactGroupCommand();

        // ִ������
        virtual void Execute();

        // ����ɾ���Ƿ�ɹ�
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


        ContactGroupData* _pGroupData; // ��ɾ������ϵ�˷�������

        bool _success;  // �Ƿ�ִ�гɹ�
    };
}

