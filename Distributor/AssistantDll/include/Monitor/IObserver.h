#pragma once

#include "AssistantDll.h"

namespace ND91Assistant
{
    // 接收者基，与ISubject对应
    class CLASSINDLL_CLASS_DECL IObserver
    {
    public:
        virtual ~IObserver() {};
    };
}
