#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/CustomItem.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"

CustomItem::CustomItem()
{
    _pItem = NULL;
}

CustomItem::~CustomItem()
{
}

void CustomItem::Init(Item* pItem)
{
    _parameter  = pItem->GetParameters();
    _value      = pItem->GetValue();
}

void CustomItem::Update()
{
    _pItem->SetName(vCard::CUSTOM_TAG+GetName());
    _pItem->SetParameters(_parameter);
    _pItem->SetValue(_value);
}

//#endif