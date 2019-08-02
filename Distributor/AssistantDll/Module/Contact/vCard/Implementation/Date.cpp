#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/Date.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"

Date::Date(Item* pItem, DateType_enum urlType, wstring customType, time_t date)
{
    _pItem      = pItem;
    _type       = urlType;
    _customType = customType;
    _date       = date;

    RETURN_IF((_type & DateType_Custom) != DateType_Custom);
    vector<wstring> pArray = CStrOperation::parseStringsW(pItem->GetParameters(), vCard::Name_Param_Splitor);
    RETURN_IF(pArray.empty());

    for (size_t i = 0; i < pArray.size(); i++)
    {
        if (!CStrOperation::startsWith(pArray.at(i), vCard::CUSTOM_TAG))
            continue;

        _customType = pArray.at(i).substr(vCard::CUSTOM_TAG.length());
        break;
    }
}

Date::~Date()
{
}

void Date::Update()
{
    _pItem->SetName(TelecommunicationsAddressingTypes::DATE);
    _pItem->SetParameters(DateTypeToString(_type, _customType));
    CDealTime dTime(_date);
    _pItem->SetValue(CCodeOperation::StringToWstring(dTime.TimeFormat()));
}

wstring Date::DateTypeToString(DateType_enum type, wstring customType)
{
    wstring retVal;
    if ((type & DateType_Other) != 0)
        retVal += L"OTHER" + vCard::Name_Param_Splitor;
    if ((type & DateType_Anniversary) != 0)
        retVal += L"ANNIVERSARY" + vCard::Name_Param_Splitor;

    if (!customType.empty())
        retVal += vCard::CUSTOM_TAG + customType + vCard::Name_Param_Splitor;

    if (CStrOperation::endsWithW(retVal, vCard::Name_Param_Splitor))
        retVal = retVal.substr(0, retVal.length()-1);

    return retVal;
}

Date* Date::Parse(Item* pItem)
{
    DateType_enum type = DateType_NotSpecified;
    wstring temp = CStrOperation::toUpperW(pItem->GetParameters());
    if (temp.find(L"OTHER") != -1)
        type = (DateType_enum)(type | DateType_Other);
    if (temp.find(L"ANNIVERSARY") != -1)
        type = (DateType_enum)(type | DateType_Anniversary);
    if (temp.find(vCard::CUSTOM_TAG) != -1)
        type = (DateType_enum)(type | DateType_Custom);

    return new Date(pItem, type, L"", CDealTime::StringToTime(pItem->GetValue()));
}

Item* Date::GetItem()
{
    return _pItem;
}

DateType_enum Date::GetType()
{
    return _type;
}

void Date::SetType(DateType_enum type)
{
    _type = type;
    Update();
}

wstring Date::GetCustomType()
{
    return _customType;
}

void Date::SetCustomType(wstring customType)
{
    _customType = customType;
    if (!_customType.empty())
        _type = (DateType_enum)(_type | DateType_Custom);
    else
        _type = (DateType_enum)(_type & ~DateType_Custom);

    Update();
}

time_t Date::GetDate()
{
    return _date;
}

void Date::SetDate(time_t date)
{
    _date = date;
    Update();
}



//#endif