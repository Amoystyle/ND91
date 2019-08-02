#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/WebPageCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Extention/WebPage.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"

WebPageCollection::WebPageCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*>* pTemp = _pOwner->GetItemCollection()->GetItemVec();
    for (size_t i = 0; i < pTemp->size(); i++)
    {
        WebPage* web = new WebPage();
        wstring tag_Name = CStrOperation::toUpperW(vCard::CUSTOM_TAG+web->GetName());
        if (CStrOperation::toUpperW(pTemp->at(i)->GetName()) != tag_Name)
        {
            SAFE_DELETE(web);
            continue;
        }

        web->Init(pTemp->at(i));
        _collection.push_back(web);
    }
}

WebPageCollection::~WebPageCollection()
{
    Clear();
}

void WebPageCollection::Add(wstring label, wstring url)
{
    AddToCollecton(label, url);
}

void WebPageCollection::AddToCollecton(wstring value)
{
    AddToCollecton(L"", value);
}

void WebPageCollection::AddToCollecton(wstring parameters, wstring value)
{
    WebPage* relate = new WebPage();
    Item* pItem = _pOwner->GetItemCollection()->Add(vCard::CUSTOM_TAG+relate->GetName(),
                                                    parameters,
                                                    value);
    relate->Init(pItem);
    _collection.push_back(relate);
}

void WebPageCollection::Remove(WebPage* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<WebPage*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        WebPage* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void WebPageCollection::Clear()
{
    vector<WebPage*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        WebPage* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif