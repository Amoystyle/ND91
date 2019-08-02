#pragma  once

namespace ND91Assistant
{
	class ContactAccount;
    class ContactData;

    // �������������
    class AddContactDataCommand : public BaseCommand
    {
    public:
        AddContactDataCommand(const DeviceData* pDevice, ContactAccount* pAccount);
        virtual ~AddContactDataCommand();

        // ִ������
        virtual void Execute();

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // װ������������ݼ���
        void SetDataList(const vector<ContactData*>* pDataList);

        // �����Ƿ�ִ�гɹ�
        bool Success();
        int ExecuteCount() { return _executeCount;};

    private:
        // ִ�а�׿����
        bool AndroidExecute();

        // ִ��IOS����
        bool IosExecute();

        // ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData();

        void WriteAndroidGroupMemberShip(BinaryBufferWriter& writer, ContactData* pContactData);
        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath(){ return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath(){ return L""; }

        // ����ios�ļ�
        virtual std::wstring GetIosCommandFileContent(){ return L""; }

        // ����ios�豸�ķ����ļ���������ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename){}

    private:

        // ����ӵ�������
        const vector<ContactData*>* _pDataList;

        // ����ӵ���������ص��˻���Ϣ
        ContactAccount*      _pAccount;

        // ��ִ����������������
        vector<ContactData*> _executeDataList;
        int _executeCount;          // ʵ��ִ�е�����
        int _executeCountEveryTime; // ÿ��ִ�е�����

        int  _maxCount;         // ÿ������������ӵ��������
        bool _success;          // �Ƿ�ִ�гɹ�
        bool _bVersion20;       // ��׿��ϵͳ�İ汾�Ƿ�Ϊ2.0����
        bool _bOMS;             // �Ƿ�ΪOMSϵͳ
    };
}