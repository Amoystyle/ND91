#pragma once

#include "Module/BaseData.h"

namespace ND91Assistant
{
    // 用来访问CDataSet中数据的访问接口
    class IVisitor
    {

    public:

        virtual void Visit(BaseData* pData) =0;

    };
}
