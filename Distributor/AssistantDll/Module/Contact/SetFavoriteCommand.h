#pragma  once

#include "Module/BaseCommand.h"
#include <vector>

using namespace std;

namespace ND91Assistant
{
    class ContactData;

    // ������ϵ���ղ���������
    class SetFavoriteCommand : public BaseCommand
    {
    public:
        SetFavoriteCommand(const DeviceData* pDevice, bool bFavorite);
        virtual ~SetFavoriteCommand();

        // ִ������
        virtual void Execute() {}

        // װ������������ݼ���
        virtual void SetDataList(const vector<ContactData*>* pDataList);

        // �����Ƿ�ִ�гɹ�
        bool Success();

        // ִ������
        int ExecuteSetFavorite();

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

        const vector<const ContactData*>* _pDataList; // �������ղ����Ե���ϵ�˼���

        int _executeCount;// ʵ��ִ�е�����

        bool _bFavorite; // �Ƿ�Ϊ�ղ�

        bool _success; // �Ƿ�ִ�гɹ�

    };
}
