#pragma once

#include "Module/BaseData.h"

namespace ND91Assistant
{
    // ��������CDataSet�����ݵķ��ʽӿ�
    class IVisitor
    {

    public:

        virtual void Visit(BaseData* pData) =0;

    };
}
