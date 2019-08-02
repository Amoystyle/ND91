#pragma once

#include "AssistantDll.h"

namespace ND91Assistant
{
    // 业务数据的基类(抽象)
    // 对每种业务数据，都实现为该类的子类
    class CLASSINDLL_CLASS_DECL BaseData
    {
    public:
        BaseData();
        virtual ~BaseData();

        enum DATA_STATUS {
            NO_CHANGE,      // 无变化
            NEW,            // 新增
            DEL,            // 删除
            UPDATE          // 更改
        };        

        void SetStatus(DATA_STATUS s);

        DATA_STATUS GetStatus() const;
        
    private:
        // 数据状态：
        DATA_STATUS _status;
    };
}
