#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/DeliveryAddressCollection.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/DeliveryAddress.h"
#include "Module/Contact/vCard/TypeDefine/DeliveryAddressingTypes.h"

DeliveryAddressCollection::DeliveryAddressCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(DeliveryAddressingTypes::ADR);
    for (size_t i = 0; i < temp.size(); i++)
    {
        DeliveryAddress* value = DeliveryAddress::Parse(temp.at(i));
        if (value != NULL)
            _collection.push_back(value);
    }
}

DeliveryAddressCollection::~DeliveryAddressCollection()
{
    Clear();
}

void DeliveryAddressCollection::Add(DeliveryAddressType_enum type, 
                                    wstring customType, 
                                    wstring postOfficeAddress, 
                                    wstring extendedAddress, 
                                    wstring street, 
                                    wstring locality, 
                                    wstring region, 
                                    wstring postalCode, 
                                    wstring countryCode)
{
    Add(type, customType, postOfficeAddress, extendedAddress, street, locality, region, postalCode, L"", countryCode);
}    

void DeliveryAddressCollection::Add(DeliveryAddressType_enum type, 
                                    wstring postOfficeAddress, 
                                    wstring extendedAddress, 
                                    wstring street, 
                                    wstring locality, 
                                    wstring region, 
                                    wstring postalCode, 
                                    wstring country)
{
    Add(type, L"", postOfficeAddress, extendedAddress, street, locality, region, postalCode, country, L"");
}

void DeliveryAddressCollection::Add(DeliveryAddressType_enum type, 
                                    wstring customType, 
                                    wstring postOfficeAddress, 
                                    wstring extendedAddress, 
                                    wstring street, 
                                    wstring locality, 
                                    wstring region, 
                                    wstring postalCode, 
                                    wstring country, 
                                    wstring countryCode)
{
    wstring temp;
    temp+=CStrOperation::ReplaceW(postOfficeAddress, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(extendedAddress, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(street, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(locality, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(region, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(postalCode, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(country, L";", L"\\;")+L";";
    temp+=CStrOperation::ReplaceW(countryCode, L";", L"\\;");
    Item* pItem = _pOwner->GetItemCollection()->Add(DeliveryAddressingTypes::ADR,
                                                    DeliveryAddress::AddressTypeToString(type, customType),
                                                    temp);
    _collection.push_back(new DeliveryAddress(pItem, 
                                              type, 
                                              customType, 
                                              postOfficeAddress, 
                                              extendedAddress, 
                                              street, 
                                              locality, 
                                              region, 
                                              postalCode, 
                                              country, 
                                              countryCode));

}

void DeliveryAddressCollection::Remove(DeliveryAddress* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<DeliveryAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        DeliveryAddress* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void DeliveryAddressCollection::Clear()
{
    vector<DeliveryAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        DeliveryAddress* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}

//#endif