#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/Url.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/vCard.h"

Url::Url(Item* pItem, UrlType_enum urlType, wstring customType, wstring data)
{
    _pItem      = pItem;
    _type       = urlType;
    _customType = customType;
    _data       = data;

    RETURN_IF((_type & Url_Custom) != Url_Custom);
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

Url::~Url()
{
}

void Url::Update()
{
    _pItem->SetParameters(UrlTypeToString(_type, _customType));
    _pItem->SetValue(_data);
}

wstring Url::UrlTypeToString(UrlType_enum type, wstring customType)
{
    wstring retVal;
    if ((type & Url_Preferred) != 0)
        retVal += L"PREF" + vCard::Name_Param_Splitor;
    if ((type & Url_Work) != 0)
        retVal += L"WORK" + vCard::Name_Param_Splitor;
    if ((type & Url_Home) != 0)
        retVal += L"HOME" + vCard::Name_Param_Splitor;
    if ((type & Url_Other) != 0)
        retVal += L"OTHER" + vCard::Name_Param_Splitor;

    if (!customType.empty())
        retVal += vCard::CUSTOM_TAG + customType + vCard::Name_Param_Splitor;

    if (CStrOperation::endsWithW(retVal, vCard::Name_Param_Splitor))
        retVal = retVal.substr(0, retVal.length()-1);

    return retVal;
}

Url* Url::Parse(Item* pItem)
{
    UrlType_enum type = Url_NotSpecified;
    wstring temp = CStrOperation::toUpperW(pItem->GetParameters());
    if (temp.find(L"PREF") != wstring::npos)
        type = (UrlType_enum)(type | Url_Preferred);
    if (temp.find(L"WORK") != wstring::npos)
        type = (UrlType_enum)(type | Url_Work);
    if (temp.find(L"HOME") != wstring::npos)
        type = (UrlType_enum)(type | Url_Home);
    if (temp.find(L"OTHER") != wstring::npos)
        type = (UrlType_enum)(type | Url_Other);
    if (temp.find(vCard::CUSTOM_TAG) != wstring::npos)
        type = (UrlType_enum)(type | Url_Custom);

    return new Url(pItem, type, L"", pItem->GetValue());
}

Item* Url::GetItem()
{
    return _pItem;
}

UrlType_enum Url::GetType()
{
    return _type;
}

void Url::SetType(UrlType_enum type)
{
    _type = type;
    Update();
}

wstring Url::GetCustomType()
{
    return _customType;
}

void Url::SetCustomType(wstring customType)
{
    _customType = customType;
    if (!_customType.empty())
        _type = (UrlType_enum)(_type | Url_Custom);
    else
        _type = (UrlType_enum)(_type & ~Url_Custom);

    Update();
}

wstring Url::GetData()
{
    return _data;
}

void Url::SetData(wstring data)
{
    _data = data;
    Update();
}

//#endif