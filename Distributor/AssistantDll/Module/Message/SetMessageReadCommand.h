#pragma  once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class MessageData;

    // ���¶���Ϣ����
    class SetMessageReadCommand : public BaseCommand
    {

    public:
        SetMessageReadCommand(const DeviceData* pDevice);
        virtual ~SetMessageReadCommand();

        // ִ������
        virtual void Execute();

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

        // ����Ҫ�޸ĵ���Ϣ
        void SetDataList( const vector<const MessageData*>* DataList );

        // �����Ƿ�ɹ�
        bool Success();

    private:

        // ִ��IOS����
        bool IosExecute();

        // ���ػ�ģʽ��ִ��Ios����
        bool IosDeamonExecut();

        // ���ػ�ģʽ��ִ��Ios����
        bool IosNoDeamonExecut();

        // ִ�а�׿����
        bool AndroidExecute();

    private:

        // ��ǰ���õĶ���Ϣ
        const MessageData* _pMessageData;

        // ��Ҫ���õĶ���Ϣ
        const vector<const MessageData*>* _dataList;
        int _executeCount;// ʵ��ִ�е�����

        // ����ִ���Ƿ�ɹ�
        bool _success;

    };
}
