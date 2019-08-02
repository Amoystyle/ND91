#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/NickNameCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/NickName.h"
#include "Module/Contact/vCard/TypeDefine/IdentificationTypes.h"

NickNameCollection::NickNameCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(IdentificationTypes::NICKNAME);
    for (size_t i = 0; i < temp.size(); i++)
    {
        NickName* value = NickName::Parse(temp.at(i));
        if (value != NULL)
            _collection.push_back(value);
    }
}

NickNameCollection::~NickNameCollection()
{
    Clear();
}

void NickNameCollection::Add(wstring data)
{
    Item* pItem = _pOwner->GetItemCollection()->Add(IdentificationTypes::NICKNAME,L"",data);
    _collection.push_back(new NickName(pItem, data));
}

void NickNameCollection::Remove(NickName* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<NickName*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        NickName* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void NickNameCollection::Clear()
{
    vector<NickName*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        NickName* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif