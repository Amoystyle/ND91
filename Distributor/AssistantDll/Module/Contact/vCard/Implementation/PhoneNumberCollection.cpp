#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/PhoneNumberCollection.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/PhoneNumber.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

PhoneNumberCollection::PhoneNumberCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(TelecommunicationsAddressingTypes::TEL);
    for (size_t i = 0; i < temp.size(); i++)
        _collection.push_back(PhoneNumber::Parse(temp.at(i)));
}

PhoneNumberCollection::~PhoneNumberCollection()
{
    Clear();
}

void PhoneNumberCollection::Add(PhoneNumberType_enum type, wstring number)
{
    Add(type, L"", number);
}

void PhoneNumberCollection::Add(PhoneNumberType_enum type, wstring customType, wstring number)
{
    Item* pItem = _pOwner->GetItemCollection()->Add(TelecommunicationsAddressingTypes::TEL,
                                                    PhoneNumber::PhoneTypeToString(type, customType),
                                                    number);
    if (!customType.empty())
        type = (PhoneNumberType_enum)(type | PhoneNumber_Custom);

    _collection.push_back(new PhoneNumber(pItem, type, customType, number));
}

void PhoneNumberCollection::Remove(PhoneNumber* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<PhoneNumber*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        PhoneNumber* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void PhoneNumberCollection::Clear()
{
    vector<PhoneNumber*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        PhoneNumber* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif