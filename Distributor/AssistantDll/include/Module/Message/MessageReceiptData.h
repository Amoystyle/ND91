#pragma once

#include "Module/BaseData.h"
#include <common/BinaryBufferReader.h>

#include <string>   

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    // 短信发送结果回执数据类
    class CLASSINDLL_CLASS_DECL MessageReceiptData : public BaseData
    {
    friend class AndroidMonitorThread;
    public:
        MessageReceiptData();
        ~MessageReceiptData();

        int _id;                // 短信 ID
        bool _success;          // 发送是否成功
        
        std::string _ios_number;     // ios返回的短信发送情况，包含电话号码
    
    private:
        //从心跳线程通知来的
        static MessageReceiptData* FromDeamonNotifyBuffer(BinaryBufferReader* pReader);
     
    };
}