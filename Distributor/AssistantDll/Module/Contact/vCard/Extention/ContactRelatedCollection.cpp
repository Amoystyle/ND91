#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/ContactRelatedCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Extention/ContactRelated.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

ContactRelatedCollection::ContactRelatedCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*>* pTemp = _pOwner->GetItemCollection()->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        ContactRelated* relate = new ContactRelated();
        wstring tag_Name = CStrOperation::toUpperW(vCard::CUSTOM_TAG+relate->GetName());
        if (CStrOperation::toUpperW(pTemp->at(i)->GetName()) != tag_Name)
        {
            SAFE_DELETE(relate);
            continue;
        }

        relate->Init(pTemp->at(i));
        _collection.push_back(relate);
    }
}

ContactRelatedCollection::~ContactRelatedCollection()
{
    Clear();
}

void ContactRelatedCollection::Add(wstring label, wstring address, RelatedKind type, wstring customName)
{
    wstring typeStr;
    if (type == RelatedKind_Parent)
        typeStr = ContactRelated::Related_Parent;
    else if (type == RelatedKind_Brother)
        typeStr = ContactRelated::Related_Brother;
    else if (type == RelatedKind_Sister)
        typeStr = ContactRelated::Related_Sister;
    else if (type == RelatedKind_Mother)
        typeStr = ContactRelated::Related_Mother;
    else if (type == RelatedKind_Father)
        typeStr = ContactRelated::Related_Father;
    else if (type == RelatedKind_Child)
        typeStr = ContactRelated::Related_Child;
    else if (type == RelatedKind_Friend)
        typeStr = ContactRelated::Related_Friend;
    else if (type == RelatedKind_Spouse)
        typeStr = ContactRelated::Related_Spouse;
    else if (type == RelatedKind_Partner)
        typeStr = ContactRelated::Related_Partner;
    else if (type == RelatedKind_Assistant)
        typeStr = ContactRelated::Related_Assistant;
    else if (type == RelatedKind_Manager)
        typeStr = ContactRelated::Related_Manager;
    else if (type == RelatedKind_Other)
        typeStr = ContactRelated::Related_Other;
    else
        typeStr = L"Custom";

    AddToCollecton(label+L";"+typeStr+L";"+customName, address);
}

void ContactRelatedCollection::AddToCollecton(wstring value)
{
    AddToCollecton(L"", value);
}

void ContactRelatedCollection::AddToCollecton(wstring parameters, wstring value)
{
    ContactRelated* relate = new ContactRelated();
    Item* pItem = _pOwner->GetItemCollection()->Add(vCard::CUSTOM_TAG+relate->GetName(),
                                                    parameters,
                                                    value);
    relate->Init(pItem);
    _collection.push_back(relate);
}

void ContactRelatedCollection::Remove(ContactRelated* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<ContactRelated*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        ContactRelated* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void ContactRelatedCollection::Clear()
{
    vector<ContactRelated*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        ContactRelated* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif