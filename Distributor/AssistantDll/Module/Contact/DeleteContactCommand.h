#pragma  once

#include "Module/BaseCommand.h"
#include <vector>
using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // ɾ����ϵ������
    class DeleteContactCommand : public BaseCommand
    {
    public:
        DeleteContactCommand(const DeviceData* pDevice);
        virtual ~DeleteContactCommand();

        // ִ������
        virtual void Execute();

        // װ������������ݼ���
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // �����Ƿ�ִ�гɹ�
        bool Success();

    private:

        // ִ��IOS����
        bool IosExecute();

        // ��ȡRowId����
        void GetRowIdVector(vector<int> *vecRowid);

        // ����δִ�����ݵ�״̬
        void ResetDataStatus();

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

        const vector<ContactData*>* _pDataList; // ��ɾ������ϵ�����ݼ���

        int _executeCount;// ʵ��ִ�е�����

        bool _success; // �Ƿ�ִ�гɹ�
    };
}