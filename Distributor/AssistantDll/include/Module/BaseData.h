#pragma once

#include "AssistantDll.h"

namespace ND91Assistant
{
    // ҵ�����ݵĻ���(����)
    // ��ÿ��ҵ�����ݣ���ʵ��Ϊ���������
    class CLASSINDLL_CLASS_DECL BaseData
    {
    public:
        BaseData();
        virtual ~BaseData();

        enum DATA_STATUS {
            NO_CHANGE,      // �ޱ仯
            NEW,            // ����
            DEL,            // ɾ��
            UPDATE          // ����
        };        

        void SetStatus(DATA_STATUS s);

        DATA_STATUS GetStatus() const;
        
    private:
        // ����״̬��
        DATA_STATUS _status;
    };
}
