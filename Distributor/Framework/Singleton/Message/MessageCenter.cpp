#include "stdafx.h"
#include "MessageCenter.h"
#include "Message.h"
#include "Common/Log.h"

MessageCenter::MessageCenter()
{
#ifdef _DEBUG
    _recordMessage = true;
#else
    _recordMessage = false;
#endif
}

MessageCenter::~MessageCenter()
{
    AutoLock;
    for(map<string, HANDLER_LIST*>::iterator it = _handlers.begin(); 
        it != _handlers.end(); ++it)
        SAFE_DELETE(it->second);
}

void MessageCenter::Attach(IHandler* pHandler)
{
    RETURN_IF( ! pHandler );

    string t = pHandler->GetType();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];

    if (pList)
    {
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it)
        {
            RETURN_IF( *it == pHandler); // �����ظ����
        }
        pList->push_back(pHandler);
    }
    else
    {
        pList = new HANDLER_LIST;
        pList->push_back(pHandler);
        _handlers[t] = pList;
    }
}

void MessageCenter::Detach(IHandler* pHandler)
{
    RETURN_IF( ! pHandler );

    string t = pHandler->GetType();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];

    if ( pList )
    {
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it)
        {
            if( *it == pHandler)
            {
                *it = NULL;     // �ݲ�ɾ����SendMsg()�л���õ� DetachReceiver ����SendMsg�б���
                break;
            }
        }
    }
}

// ������Ϣ�����н����ߣ�������ɺ�ŷ���
bool MessageCenter::Send(const IMessage* pMsg)
{
    RETURN_FALSE_IF( ! pMsg );

    string t = typeid(*pMsg).raw_name();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];


    if ( pList )
    {
        RecordMessage(pMsg);
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it )
        {
            IHandler* pHandler = *it;
            if(pHandler)
                if (!pMsg->SendTo(pHandler))
                    break;
        }
        return pList->size() > 0;
    }
    return false;
}

// ����windows��Ϣ����Ϣ���� lparam����Ϣ�������踺���ͷ�
void MessageCenter::SendToUI(HWND hWnd, const IMessage* pMsg)
{
    RETURN_IF( ! pMsg );
    pMsg->PostToUI(hWnd);
}


void MessageCenter::GetData( IMessage* pMsg )
{
    RETURN_IF( ! pMsg );

    string t = typeid(*pMsg).raw_name();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];
    if ( pList )
    {
        RecordMessage(pMsg);
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it )
        {
            IHandler* pHandler = *it;
            if(pHandler)
                if (!pMsg->SendAndGetReturn(pHandler))
                    break;
        }
    }
}

void MessageCenter::GroupMsgHandler(IMessage* pMsg,
                                    IHandler**& pArray,
                                    unsigned int& _arrayLength)
{
    string t = typeid(*pMsg).raw_name();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];
    if ( pList && pList->size() )
    {
        pArray = new IHandler*[pList->size()];
        int index = 0;
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end();
            ++it, ++index )
        {
            pArray[index] = *it;
        }
        _arrayLength = pList->size();
    }
}

void MessageCenter::Begin( const IMessage* pMsg )
{
    RETURN_IF( ! pMsg );

    string t = typeid(*pMsg).raw_name();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];
    if ( pList )
    {
        RecordMessage(pMsg, true);
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it )
        {
            IHandler* pHandler = *it;
            if(pHandler) pMsg->Begin(pHandler);
        }
    }
}

void MessageCenter::End( const IMessage* pMsg )
{
    RETURN_IF( ! pMsg );

    string t = typeid(*pMsg).raw_name();
    AutoLock;
    HANDLER_LIST* pList = _handlers[t];
    if ( pList )
    {
        RecordMessage(pMsg, false);
        for(HANDLER_LIST::iterator it = pList->begin(); it != pList->end(); ++it )
        {
            IHandler* pHandler = *it;
            if(pHandler) pMsg->End(pHandler);
        }
    }
}

void MessageCenter::RecordMessage( const IMessage* pMsg ) const
{
    if ( _recordMessage && pMsg->RecordMessage() )
    {
        LOG->Write(Log::LOG_DEBUG, L"��%S:%X��%s",
            typeid(*pMsg).name(), pMsg, pMsg->ToString().c_str());
    }
}

void MessageCenter::RecordMessage( const IMessage* pMsg, bool begin ) const
{
    if ( _recordMessage && pMsg->RecordMessage() )
    {
        LOG->Write(Log::LOG_DEBUG, L"��%S:%X��%s",
            typeid(*pMsg).name(), pMsg, begin? L"begin":L"end");
    }
}

void MessageCenter::RecordHandle(IHandler* p, const IMessage* pMsg ) const
{
    if ( _recordMessage && pMsg->RecordMessage() )
    {
        LOG->Write(Log::LOG_DEBUG, L"��%S:%X%d����%S:%X��%s",
            typeid(*pMsg).name(), pMsg,
            typeid(*p).name(), p, pMsg->ToString().c_str());
    }
}
