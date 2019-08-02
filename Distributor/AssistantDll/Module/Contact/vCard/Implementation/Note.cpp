#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/Note.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/vCard.h"

Note::Note(Item* pItem, wstring data)
{
    _pItem  = pItem;
    _data   = data;
}

Note::~Note()
{
}

void Note::Update()
{
    _pItem->SetParameters(L"ENCODING=QUOTED-PRINTABLE"+vCard::Name_Param_Splitor);
    _pItem->SetValue(_data);
}

Note* Note::Parse(Item* pItem)
{           
    return new Note(pItem, pItem->GetValue());
}


//#endif