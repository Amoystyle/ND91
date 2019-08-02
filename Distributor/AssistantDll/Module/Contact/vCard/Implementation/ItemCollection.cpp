#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/vItem.h"

ItemCollection::ItemCollection()
{
}

ItemCollection::~ItemCollection()
{
    Clear();
}

Item* ItemCollection::Add(wstring name, wstring parametes, wstring value)
{
    Item* item = new Item(name, parametes, value);
    _items.push_back(item);
    return item;
}

void ItemCollection::Remove(wstring name)
{
    vector<Item*>::iterator it = _items.begin();
    while ( it != _items.end() )
    {
        Item* pItem = *it;
        if (CStrOperation::toLowerW(pItem->GetName()) 
            == CStrOperation::toLowerW(name))
        {
            it = _items.erase(it);
            SAFE_DELETE(pItem);
        }
        else
            ++it;
    }
}

void ItemCollection::Remove(Item* item)
{
    vector<Item*>::iterator it = _items.begin();
    while ( it != _items.end() )
    {
        Item* pItem = *it;
        if (pItem == item)
        {
            it = _items.erase(it);
            SAFE_DELETE(pItem);
        }
        else
            ++it;
    }
}

void ItemCollection::Clear()
{
    vector<Item*>::iterator it = _items.begin();
    while ( it != _items.end() )
    {
        Item* pItem = *it;
        it = _items.erase(it);
        SAFE_DELETE(pItem);
    }
    _items.clear();
}

Item* ItemCollection::GetFirst(wstring name)
{
    vector<Item*>::iterator it = _items.begin();
    while ( it != _items.end() )
    {
        Item* pItem = *it;
        if (CStrOperation::toLowerW(pItem->GetName()) 
            == CStrOperation::toLowerW(name))
            return pItem;
        else
            ++it;
    }

    return NULL;
}

vector<Item*> ItemCollection::Get(wstring name)
{
    vector<Item*> res;
    vector<Item*>::iterator it = _items.begin();
    while ( it != _items.end() )
    {
        Item* pItem = *it;
        if (CStrOperation::toLowerW(pItem->GetName()) 
            == CStrOperation::toLowerW(name))
            res.push_back(pItem);

        ++it;
    }
    return res;
}

void ItemCollection::SetValue(wstring name, wstring value)
{
    SetValue(name, L"", value);
}

void ItemCollection::SetValue(wstring name, wstring parametes, wstring value)
{
    if (value.empty())
    {
        Remove(name);
        return ;
    }

    Item* item = GetFirst(name);
    if (item != NULL)
        item->SetValue(item->GetValue()+L","+value);
    else
        _items.push_back(new Item(name, parametes, value));
}

//#endif