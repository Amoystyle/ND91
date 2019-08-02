#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/IMAddressCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Extention/IMAddress.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

IMAddressCollection::IMAddressCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*>* pTemp = _pOwner->GetItemCollection()->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        IMAddress* im = new IMAddress();
        wstring tag_Name = CStrOperation::toUpperW(vCard::CUSTOM_TAG+im->GetName());
        if (CStrOperation::toUpperW(pTemp->at(i)->GetName()) != tag_Name)
        {
            SAFE_DELETE(im);
            continue;
        }

        im->Init(pTemp->at(i));
        _collection.push_back(im);
    }
}

IMAddressCollection::~IMAddressCollection()
{
    Clear();
}

void IMAddressCollection::Add(wstring label, wstring address)
{
    AddToCollecton(label, address);
}

void IMAddressCollection::Add(wstring label, wstring address, IMType_enum type, wstring customName)
{
    wstring typeStr;
    if (type == IMType_OTHER)
        typeStr = L"OTHER";
    else if (type == IMType_WORK)
        typeStr = L"WORK";
    else if (type == IMType_HOME)
        typeStr = L"HOME";
    else
        typeStr = L"CUSTOM";

    if (!label.empty())
        AddToCollecton(label+L";"+typeStr+L";"+customName, address);
}

void IMAddressCollection::AddToCollecton(wstring value)
{
    AddToCollecton(L"", value);
}

void IMAddressCollection::AddToCollecton(wstring parameters, wstring value)
{
    IMAddress* relate = new IMAddress();
    Item* pItem = _pOwner->GetItemCollection()->Add(vCard::CUSTOM_TAG+relate->GetName(),
                                                    parameters,
                                                    value);
    relate->Init(pItem);
    _collection.push_back(relate);
}

void IMAddressCollection::Remove(IMAddress* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<IMAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        IMAddress* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void IMAddressCollection::Clear()
{
    vector<IMAddress*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        IMAddress* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif