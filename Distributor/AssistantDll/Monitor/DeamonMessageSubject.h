#pragma once

#include "Monitor/ISubject.h"
#include "Monitor/DeamonMessageObserver.h"

namespace ND91Assistant
{
    // 守护发送信息的广播者
    class MessageData;
    class MessageReceiptData;
    class DeamonMessageSubject : public ISubject
    {
    public:
        virtual ~DeamonMessageSubject(){}

        // 新短信通知
        void NewMessage(const MessageData* msg);

        // 短信回执通知
        void MessageReceipt(const MessageReceiptData* pMsgData);

        // 熊猫桌面操作通知
        void PandaThemeAction(const PandaThemeActionReturn* pData);
		//连接失败
		void DeviceLinkErrMessage( const int pData );
    };
}

