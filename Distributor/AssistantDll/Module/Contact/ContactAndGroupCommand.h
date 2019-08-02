#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactData;
    class ContactGroupData;

    // ���������������������ϵ������
    class ContactAndGroupCommand : public BaseCommand
    {
    public:
        ContactAndGroupCommand(const DeviceData* pDevice);
        virtual ~ContactAndGroupCommand();

        // �����Ƿ�ִ�гɹ�
        bool Success();
 
        // Summary  : �ӷ������Ƴ�������
        // Returns  : int ʵ��ִ�е�����
        // Parameter: const vector<ContactData * > * pDataList  ��Ҫ�����������˼���
        // Parameter: ContactGroupData * pGroupData ��Ҫ�����ķ���
        int ExecuteRemoveContactFromGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData);

        // Summary  : ����������ӵ�����
        // Returns  : int ʵ��ִ�е�����
        // Parameter: const vector<ContactData * > * pDataList  ��Ҫ�����������˼���
        // Parameter: ContactGroupData * pGroupData ��Ҫ�����ķ���
        int ExecuteAddContactToGroup(const vector<ContactData*>* pDataList, ContactGroupData* pGroupData);

    private:
        // ִ������
        virtual void Execute() {}

    private:
        // ִ��IOS��������˵�Ⱥ������
        bool IosExecuteAddContactToGroup();

        // ִ��IOS��Ⱥ���Ƴ�����������
        bool IosExecuteRemoveContactFromGroup();

        // ��ȡRowId����
        void GetRowIdVector(vector<int> *vecRowid);

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

    private:      
      
        ContactGroupData* _pGroupData; // �������ķ�����Ϣ

        const vector<const ContactData*>* _pDataList; // �������������˼���
        int _executeCount;// ʵ��ִ�е�����

        bool _success; // �Ƿ�ִ�гɹ�

    };
}
