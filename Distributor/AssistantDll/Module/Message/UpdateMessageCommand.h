#pragma  once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class DeviceData;
    class MessageData;

    // ���¶���Ϣ����
    class UpdateMessageCommand : public BaseCommand
    {

    public:
        UpdateMessageCommand(const DeviceData* pDevice);
        virtual ~UpdateMessageCommand();

        // ִ������
        virtual void Execute();

        // װ������µĶ�������
        virtual void SetDataList(const vector<const MessageData*>* DataList);

        // ����Android�����壬
        virtual std::string GetAndroidBodyData();

        // ����Android�������ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() {throw "������ʵ��";return NULL;}

        // ����ios�����ļ���������ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename){throw "������ʵ��";}

        // �����Ƿ�ɹ�
        bool Success();

    private:

        // ִ��IOS����
        bool IosExecute();

        // ִ��Android����
        bool AndroidExecute();

    private:

        // ��Ҫ���µĶ���Ϣ������SetDataList�������ã�
        const std::vector<const MessageData*>* _dataList;

        int _executeCount;// ʵ��ִ�е�����
        
        const MessageData* _pMessageData;
        
        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}
