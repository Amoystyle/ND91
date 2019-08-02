#pragma once

#include "AssistantDll.h"

#include "IObserver.h"

namespace ND91Assistant
{
	enum DeviceLinkErrCode
	{
		ADB_CLOSE,
		DEAMON_START_ING,
		DEAMON_START_FAIL,
		ADB_START_ING,
		DEAMON_SOCK_ERR,
	};
    class MessageData;
    class MessageReceiptData;
    class PandaThemeActionReturn;
    class CLASSINDLL_CLASS_DECL DeamonMessageObserver : public IObserver
    {
    public:
        virtual ~DeamonMessageObserver() {}

        virtual void NewMessage(const MessageData* pMsgData) = 0;

        virtual void MessageReceipt(const MessageReceiptData* pMsgData) = 0;

        virtual void PandaThemeAction(const PandaThemeActionReturn* pData) = 0;

		virtual void DeviceLinkErrMessage(const int errcode) {};
    };
}
