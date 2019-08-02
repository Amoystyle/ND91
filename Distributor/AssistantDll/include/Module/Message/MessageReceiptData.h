#pragma once

#include "Module/BaseData.h"
#include <common/BinaryBufferReader.h>

#include <string>   

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    // ���ŷ��ͽ����ִ������
    class CLASSINDLL_CLASS_DECL MessageReceiptData : public BaseData
    {
    friend class AndroidMonitorThread;
    public:
        MessageReceiptData();
        ~MessageReceiptData();

        int _id;                // ���� ID
        bool _success;          // �����Ƿ�ɹ�
        
        std::string _ios_number;     // ios���صĶ��ŷ�������������绰����
    
    private:
        //�������߳�֪ͨ����
        static MessageReceiptData* FromDeamonNotifyBuffer(BinaryBufferReader* pReader);
     
    };
}