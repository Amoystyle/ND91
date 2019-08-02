#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/UrlCollection.h"
#include "Module/Contact/vCard/TypeDefine/ExplanatoryTypes.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/Url.h"

UrlCollection::UrlCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(ExplanatoryTypes::URL);
    for (size_t i = 0; i < temp.size(); i++)
    {
        Url* value = Url::Parse(temp.at(i));
        if (value != NULL)
            _collection.push_back(value);
    }
}

UrlCollection::~UrlCollection()
{
    Clear();
}

void UrlCollection::Add(UrlType_enum type, wstring data)
{
    Add(type, L"", data);
}

void UrlCollection::Add(UrlType_enum type, wstring customType, wstring data)
{
    Item* pItem = _pOwner->GetItemCollection()->Add(ExplanatoryTypes::URL,
                                                    Url::UrlTypeToString(type, customType),
                                                    data);
    _collection.push_back(new Url(pItem, type, customType, data));
}

void UrlCollection::Remove(Url* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<Url*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Url* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void UrlCollection::Clear()
{
    vector<Url*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Url* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif