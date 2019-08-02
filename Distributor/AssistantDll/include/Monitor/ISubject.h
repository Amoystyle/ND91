#pragma once

#pragma warning(disable : 4251)

#include "AssistantDll.h"
#include "IObserver.h"

#include <vector>

namespace ND91Assistant
{
    // 广播者基类
    class CLASSINDLL_CLASS_DECL ISubject
    {

    public:
        ISubject();
        virtual ~ISubject();

        // 添加到监听队列
        virtual void Attach(IObserver* pObserver);

        // 从监听队列中删除该Observer
        virtual void Detach(IObserver* pObserver);

        // 从监听队列中获取第一个Observer
        IObserver* GetFirstObserver();

    protected:

        std::vector<IObserver*> _observers; // 监听队列
    };
}
