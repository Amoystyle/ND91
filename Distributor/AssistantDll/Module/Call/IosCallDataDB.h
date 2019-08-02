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

    #pragma region 通话记录的DB操作

        // 获取所有通话记录
        bool GetCalls(vector<CallData*> *callDataVec);

        // 取通话记录数量
        bool GetCallsSize(int* size);

        // 添加通话记录（假如rowid相同，则不添加）
        // 返回实际执行数据条数
        int AddCallData(const vector<const CallData*> *callDataVec, ProgressSubject * pProgressSubject);

        // 删除通话记录
        // 返回实际执行数据条数
        int DeleteCall(vector<int> rowidVec, ProgressSubject * pProgressSubject);

    #pragma endregion

    private:
        // Summary  : 获取通话类型
        // Returns  : int 
        // Parameter: const CallData * pData 通话记录
        int GetFlags(const CallData* pData);

    };
}
