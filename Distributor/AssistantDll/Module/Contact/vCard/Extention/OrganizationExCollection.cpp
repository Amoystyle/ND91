#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/OrganizationExCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Extention/OrganizationEx.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

OrganizationExCollection::OrganizationExCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*>* pTemp = _pOwner->GetItemCollection()->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        OrganizationEx* org = new OrganizationEx();
        wstring tag_Name = CStrOperation::toUpperW(vCard::CUSTOM_TAG+org->GetName());
        if (CStrOperation::toUpperW(pTemp->at(i)->GetName()) != tag_Name)
        {
            SAFE_DELETE(org);
            continue;
        }

        org->Init(pTemp->at(i));
        _collection.push_back(org);
    }
}

OrganizationExCollection::~OrganizationExCollection()
{
    Clear();
}

void OrganizationExCollection::Add(wstring title, wstring location)
{
    Add(L"", title, location);
}

void OrganizationExCollection::Add(wstring label, wstring title, wstring location)
{
    AddToCollecton(label, title+vCard::Name_Param_Splitor+location);
}

void OrganizationExCollection::AddToCollecton(wstring value)
{
    AddToCollecton(L"", value);
}

void OrganizationExCollection::AddToCollecton(wstring parameters, wstring value)
{
    OrganizationEx* relate = new OrganizationEx();
    Item* pItem = _pOwner->GetItemCollection()->Add(vCard::CUSTOM_TAG+relate->GetName(),
                                                    parameters,
                                                    value);
    relate->Init(pItem);
    _collection.push_back(relate);
}

void OrganizationExCollection::Remove(OrganizationEx* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<OrganizationEx*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        OrganizationEx* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void OrganizationExCollection::Clear()
{
    vector<OrganizationEx*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        OrganizationEx* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif