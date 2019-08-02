#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/NoteCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/TypeDefine/ExplanatoryTypes.h"
#include "Module/Contact/vCard/Implementation/Note.h"

NoteCollection::NoteCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(ExplanatoryTypes::NOTE);
    for (size_t i = 0; i < temp.size(); i++)
    {
        Note* value = Note::Parse(temp.at(i));
        if (value != NULL)
            _collection.push_back(value);
    }
}

NoteCollection::~NoteCollection()
{
    Clear();
}

void NoteCollection::Add(wstring data)
{
    Item* pItem = _pOwner->GetItemCollection()->Add(ExplanatoryTypes::NOTE, L"ENCODING=QUOTED-PRINTABLE", data);
    _collection.push_back(new Note(pItem, data));
}

void NoteCollection::Remove(Note* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<Note*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Note* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void NoteCollection::Clear()
{
    vector<Note*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Note* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif