#pragma once

#include "Module/Call/CallData.h"
#include "Monitor/ProgressSubject.h"
#include "Module/Common/IosDB/IosDBBase.h"

namespace ND91Assistant
{
    class IosCallDataDB : public IosDBBase
    {
    public:
        IosCallDataDB(IosDBPath iosDBPath);
        ~IosCallDataDB(void);

    #pragma region ͨ����¼��DB����

        // ��ȡ����ͨ����¼
        bool GetCalls(vector<CallData*> *callDataVec);

        // ȡͨ����¼����
        bool GetCallsSize(int* size);

        // ���ͨ����¼������rowid��ͬ������ӣ�
        // ����ʵ��ִ����������
        int AddCallData(const vector<const CallData*> *callDataVec, ProgressSubject * pProgressSubject);

        // ɾ��ͨ����¼
        // ����ʵ��ִ����������
        int DeleteCall(vector<int> rowidVec, ProgressSubject * pProgressSubject);

    #pragma endregion

    private:
        // Summary  : ��ȡͨ������
        // Returns  : int 
        // Parameter: const CallData * pData ͨ����¼
        int GetFlags(const CallData* pData);

    };
}
