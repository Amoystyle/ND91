#pragma  once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class MessageData;

    // ɾ������Ϣ����
    class DeleteMessageCommand : public BaseCommand
    {

    public:
        DeleteMessageCommand(const DeviceData* pDevice);
        virtual ~DeleteMessageCommand();

        // ִ������
        virtual void Execute();
        
        // װ���ɾ���Ķ�������
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

        // ɾ���Ƿ�ɹ�
        bool Success();

    private:

        // ִ��IOS����
        bool IosExecute();

        // ɾ�������ļ�
        void DeleteMMSFile(const MessageData* pMessageData);

    private:
        // ��Ҫɾ������Ϣ������SetDataList�������ã�
        const vector<const MessageData*>* _dataList;

        int _executeCount;// ʵ��ִ�е�����

        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}
