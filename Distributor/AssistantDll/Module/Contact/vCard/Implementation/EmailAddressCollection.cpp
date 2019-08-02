#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/EmailAddressCollection.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/EmailAddress.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

EmailAddressCollection::EmailAddressCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(TelecommunicationsAddressingTypes::EMAIL);
    for (size_t i = 0; i < temp.size(); i++)
        _collection.push_back(EmailAddress::Parse(temp.at(i)));
}

EmailAddressCollection::~EmailAddressCollection()
{
    Clear();
}

EmailAddress* EmailAddressCollection::Add(EmailAddressType_enum type, wstring email)
{
    return Add(type, L"", email);
}

EmailAddress* EmailAddressCollection::Add(EmailAddressType_enum type, wstring customType, wstring email)
{
    Item* pItem = _pOwner->GetItemCollection()->Add(TelecommunicationsAddressingTypes::EMAIL,
                                                    EmailAddress::EmailTypeToString(type, customType),
                                                    email);

    EmailAddress* emailAddress = new EmailAddress(pItem, type, customType, email);
    _collection.push_back(emailAddress);
    return emailAddress;
}

void EmailAddressCollection::Remove(EmailAddress* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<EmailAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        EmailAddress* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void EmailAddressCollection::Clear()
{
    vector<EmailAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        EmailAddress* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}

//#endif