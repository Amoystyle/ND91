#include "stdafx.h"
#include "DeamonMessageSubject.h"

void DeamonMessageSubject::NewMessage(const MessageData* pMsgData)
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeamonMessageObserver* pObserver = dynamic_cast<DeamonMessageObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->NewMessage(pMsgData);
    }

}

void DeamonMessageSubject::MessageReceipt( const MessageReceiptData* pMsgData )
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeamonMessageObserver* pObserver = dynamic_cast<DeamonMessageObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->MessageReceipt(pMsgData);
    }
}

void ND91Assistant::DeamonMessageSubject::PandaThemeAction( const PandaThemeActionReturn* pData )
{
    for(size_t i = 0; i < _observers.size(); ++i)
    {
        DeamonMessageObserver* pObserver = dynamic_cast<DeamonMessageObserver*>(_observers[i]);

        if ( pObserver )
            pObserver->PandaThemeAction(pData);
    }
}


void ND91Assistant::DeamonMessageSubject::DeviceLinkErrMessage( const int pData )
{
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		DeamonMessageObserver* pObserver = dynamic_cast<DeamonMessageObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->DeviceLinkErrMessage(pData);
	}
}
