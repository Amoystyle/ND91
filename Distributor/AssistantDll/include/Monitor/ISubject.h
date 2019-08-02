#pragma once

#pragma warning(disable : 4251)

#include "AssistantDll.h"
#include "IObserver.h"

#include <vector>

namespace ND91Assistant
{
    // �㲥�߻���
    class CLASSINDLL_CLASS_DECL ISubject
    {

    public:
        ISubject();
        virtual ~ISubject();

        // ��ӵ���������
        virtual void Attach(IObserver* pObserver);

        // �Ӽ���������ɾ����Observer
        virtual void Detach(IObserver* pObserver);

        // �Ӽ��������л�ȡ��һ��Observer
        IObserver* GetFirstObserver();

    protected:

        std::vector<IObserver*> _observers; // ��������
    };
}
