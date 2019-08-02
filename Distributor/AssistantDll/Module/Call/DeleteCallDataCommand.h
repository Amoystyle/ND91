#pragma once

#include "Module/Call/CallData.h"

namespace ND91Assistant
{
    // ɾ��ͨ����¼����
    class DeleteCallDataCommand : public BaseCommand
    {

    public:
        DeleteCallDataCommand(const DeviceData* pDevice);
        virtual ~DeleteCallDataCommand();

        // ִ������
        virtual void Execute();

        // װ���ɾ���ĺ��м�¼
        virtual void SetDataList(const vector<const BaseData*>* DataList);

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
 
    private:

        // ִ��IOS����
        bool IosExecute();

        // ��ȡRowId����
        void GetRowIdVector(vector<int> *vecRowid);

        // ����δִ�����ݵ�״̬
        void ResetDataStatus();

    private:
        // ��Ҫɾ����ͨ����¼������SetDataList�������ã�
        const vector<const CallData*>* _dataList;
        int _executeCount;// ʵ��ִ�е�����
        
        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}
