#pragma once

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    class AddContactToGroupCommand : public BaseCommand
    {

    public:
        AddContactToGroupCommand(const DeviceData* pDevice, ContactGroupData* pGroupData);
        virtual ~AddContactToGroupCommand();

        // ִ������
        virtual void Execute();

#pragma region

        // ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData();

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // װ������������ݼ���
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // ����ios�ļ�
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // ����ios�豸�ķ����ļ���������ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename){}

        // �����Ƿ�ִ�гɹ�
        bool Success();

#pragma endregion

    private:

        ContactGroupData* _pGroupData;

        const vector<const ContactData*>* _pDataList;

        bool _success;

    };
}
