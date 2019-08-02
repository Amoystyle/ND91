#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/NickName.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/vCard.h"

NickName::NickName(Item* pItem, wstring data)
{
    _pItem  = pItem;
    _data   = data;
}

NickName::~NickName()
{
}

void NickName::Update()
{
    _pItem->SetParameters(L"");
    _pItem->SetValue(_data);
}

NickName* NickName::Parse(Item* pItem)
{           
    return new NickName(pItem, pItem->GetValue());
}

//#endif