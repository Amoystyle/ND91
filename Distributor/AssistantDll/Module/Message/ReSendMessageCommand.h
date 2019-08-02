
#pragma  once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{

    class ReSendMessageCommand : public BaseCommand
    {

    public:
        ReSendMessageCommand(const DeviceData* pDevice,time_t startTime,time_t endTime);
        virtual ~ReSendMessageCommand();

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
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }

        // ����Ҫ�ط�����Ϣ
        void SetDataList(vector<int>* delmsgIds);

        // ������Ϣ�Ƿ�ɹ�
        bool Success();

    private:
        // ִ��Ios����
        bool IosExecute();

        // ִ��Android����
        bool AndroidExecute();

        // ��ȡRowId����
        void GetRowIdVector(vector<int> *vecRowid);

    private:
        // ��ǰ�ط�����Ϣ
        int _curMessageId;

		// Ҫ�ط��Ķ���id
		vector<int>*  _delmsgIds;

        // ���һ�η��Ϳ�ʼʱ��
        time_t _startTime;

        // ���һ�η��ͽ���ʱ��
        time_t _endTime;

        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}

