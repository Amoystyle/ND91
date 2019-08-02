#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/AnniversaryCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Extention/Anniversary.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

AnniversaryCollection::AnniversaryCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*>* pTemp = _pOwner->GetItemCollection()->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        Anniversary* date = new Anniversary();
        wstring tag_Name = CStrOperation::toUpperW(vCard::CUSTOM_TAG+date->GetName());
        if (CStrOperation::toUpperW(pTemp->at(i)->GetName()) != tag_Name)
        {
            SAFE_DELETE(date);
            continue;
        }

        date->Init(pTemp->at(i));
        _collection.push_back(date);
    }
}

AnniversaryCollection::~AnniversaryCollection()
{
    Clear();
}

void AnniversaryCollection::Add(time_t value)
{
    Add(L"", value);
}

void AnniversaryCollection::Add(wstring label, time_t value)
{
    CDealTime dTime(value);
    AddToCollecton(label, CCodeOperation::StringToWstring(dTime.TimeFormat()));
}

void AnniversaryCollection::AddToCollecton(wstring value)
{
    AddToCollecton(L"", value);
}

void AnniversaryCollection::AddToCollecton(wstring parameters, wstring value)
{
    Anniversary* relate = new Anniversary();
    Item* pItem = _pOwner->GetItemCollection()->Add(vCard::CUSTOM_TAG+relate->GetName(),
                                                    parameters,
                                                    value);
    relate->Init(pItem);
    _collection.push_back(relate);
}

void AnniversaryCollection::Remove(Anniversary* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<Anniversary*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Anniversary* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void AnniversaryCollection::Clear()
{
    vector<Anniversary*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Anniversary* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif