#pragma once

#include "Monitor/ISubject.h"
#include "Monitor/DeamonMessageObserver.h"

namespace ND91Assistant
{
    // �ػ�������Ϣ�Ĺ㲥��
    class MessageData;
    class MessageReceiptData;
    class DeamonMessageSubject : public ISubject
    {
    public:
        virtual ~DeamonMessageSubject(){}

        // �¶���֪ͨ
        void NewMessage(const MessageData* msg);

        // ���Ż�ִ֪ͨ
        void MessageReceipt(const MessageReceiptData* pMsgData);

        // ��è�������֪ͨ
        void PandaThemeAction(const PandaThemeActionReturn* pData);
		//����ʧ��
		void DeviceLinkErrMessage( const int pData );
    };
}

