#pragma  once

namespace ND91Assistant
{
    class MessageData;

    // ���ͨ����¼����
    class AddMessageCommand : public BaseCommand
    {

    public:
        AddMessageCommand(const DeviceData* pDevice);
        virtual ~AddMessageCommand();

        // ִ������
        virtual void Execute();

        // װ�����ӵĶ���Ϣ
        void SetDataList(const vector<const MessageData*>* DataList);

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }
        
        // ����ִ�н�����Ƿ�ɹ�
        bool Success();

        int ExecuteCount() {return _executeCount;}
    private:
        // ִ�а�׿����
        bool AndroidExecute();

        // ִ��IOS����
        bool IosExecute();

    private:

        // ��Ҫ��ӵĶ���Ϣ����SetDataList�������ã�
        const vector<const MessageData*>* _dataList;

        // ��ִ���������Ķ���Ϣ����
        vector<const MessageData*> _executeDataList;
        int _executeCount;          // ʵ��ִ�е�����
        int _executeCountEveryTime; // ÿ��ִ�е�����

        int _maxCount; // ÿ������������ӵ��������

        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}
