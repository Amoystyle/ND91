#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/DateCollection.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/Implementation/Date.h"

DateCollection::DateCollection(vCard* owner)
{
    _pOwner = owner;

    vector<Item*> temp = _pOwner->GetItemCollection()->Get(TelecommunicationsAddressingTypes::DATE);
    for (size_t i = 0; i < temp.size(); i++)
        _collection.push_back(Date::Parse(temp.at(i)));
}

DateCollection::~DateCollection()
{
    Clear();
}

void DateCollection::Add(DateType_enum type, time_t date)
{
    Add(type, L"", date);
}

void DateCollection::Add(DateType_enum type, wstring customType, time_t date)
{
    CDealTime dTime(date);
    Item* pItem = _pOwner->GetItemCollection()->Add(TelecommunicationsAddressingTypes::DATE,
                                                    Date::DateTypeToString(type, customType),
                                                    CCodeOperation::StringToWstring(dTime.TimeFormat()));
    if (!customType.empty())
        type = (DateType_enum)(type | DateType_Custom);

    _collection.push_back(new Date(pItem, type, customType, date));
}


void DateCollection::Remove(Date* pItem)
{
    _pOwner->GetItemCollection()->Remove(pItem->GetItem());

    vector<Date*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Date* pTemp = *it;
        if (pTemp == pItem)
        {
            it = _collection.erase(it);
            SAFE_DELETE(pTemp);
        }
        else
            ++it;
    }
}

void DateCollection::Clear()
{
    vector<Date*>::iterator it = _collection.begin();
    while ( it != _collection.end() )
    {
        Date* pItem = *it;
        _pOwner->GetItemCollection()->Remove(pItem->GetItem());
        it = _collection.erase(it);
        SAFE_DELETE(pItem);
    }
    _collection.clear();
}


//#endif