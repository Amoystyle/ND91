#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/MessageReceiptData.h"

MessageReceiptData::MessageReceiptData()
{
}

MessageReceiptData::~MessageReceiptData()
{
}

MessageReceiptData* ND91Assistant::MessageReceiptData::FromDeamonNotifyBuffer( BinaryBufferReader* pReader )
{
    MessageReceiptData* pMessageReceipt = new MessageReceiptData();
    pReader->ReadInt32(pMessageReceipt->_id);
    pReader->ReadBool(pMessageReceipt->_success);
    return pMessageReceipt;
}

#endif