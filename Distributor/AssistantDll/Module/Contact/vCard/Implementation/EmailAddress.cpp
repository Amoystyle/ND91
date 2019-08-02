#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/EmailAddress.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"

wstring EmailAddress::Home          = L"HOME";
wstring EmailAddress::Work          = L"WORK";
wstring EmailAddress::Other         = L"OTHER";
wstring EmailAddress::Custom        = L"CUSTOM";
wstring EmailAddress::Pager         = L"PAGER";
wstring EmailAddress::Mobile        = L"MOBILE";
wstring EmailAddress::Anniversary   = L"ANNIVERSARY";

EmailAddress::EmailAddress(Item* pItem, 
                           EmailAddressType_enum type, 
                           wstring customType, 
                           wstring emailAddress)
{
    _pItem          = pItem;
    _type           = type;
    _customType     = customType;
    _emailAddress   = emailAddress;

    RETURN_IF((_type & EmailAddress_Custom) != EmailAddress_Custom);
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

EmailAddress::~EmailAddress()
{
}

void EmailAddress::Update()
{
    _pItem->SetParameters(EmailTypeToString(_type, _customType));
    _pItem->SetValue(_emailAddress);
}

wstring EmailAddress::EmailTypeToString(EmailAddressType_enum type, wstring customType)
{
    wstring retVal;
    if ((type & EmailAddress_Internet) != 0)
        retVal += L"INTERNET" + vCard::Name_Param_Splitor;
    if ((type & EmailAddress_Preferred) != 0)
        retVal += L"PREF" + vCard::Name_Param_Splitor;
    if ((type & EmailAddress_X400) != 0)
        retVal += L"X400" + vCard::Name_Param_Splitor;
    if (type == 0)
        retVal += L"OTHER" + vCard::Name_Param_Splitor;

    if (!customType.empty() && (type & EmailAddress_Custom) != 0)
        retVal += vCard::CUSTOM_TAG + customType + vCard::Name_Param_Splitor;

    if (CStrOperation::endsWithW(retVal, vCard::Name_Param_Splitor))
        retVal = retVal.substr(0, retVal.length()-1);

    return retVal;
}

EmailAddress* EmailAddress::Parse(Item* pItem)
{
    EmailAddressType_enum type = EmailAddress_NotSpecified;
    wstring temp = CStrOperation::toUpperW(pItem->GetParameters());
    if (temp.find(L"PREF") != wstring::npos)
        type = (EmailAddressType_enum)(type | EmailAddress_Preferred);
    if (temp.find(L"INTERNET") != wstring::npos)
        type = (EmailAddressType_enum)(type | EmailAddress_Internet);
    if (temp.find(L"X400") != wstring::npos)
        type = (EmailAddressType_enum)(type | EmailAddress_X400);
    if (temp.find(L"OTHER") != wstring::npos)
        type = (EmailAddressType_enum)(type | EmailAddress_NotSpecified);
    if (temp.find(vCard::CUSTOM_TAG) != wstring::npos)
        type = (EmailAddressType_enum)(type | EmailAddress_Custom);

    return new EmailAddress(pItem, type, L"", pItem->GetValue());
}

bool EmailAddress::IsPrimary()
{
    if ((_type & EmailAddress_Preferred) == EmailAddress_Preferred)
        return true;
    return false;
}

Item* EmailAddress::GetItem() 
{ 
    return _pItem; 
}

EmailAddressType_enum EmailAddress::GetType()
{
    return _type;
}

void EmailAddress::SetType(EmailAddressType_enum type)
{
    _type = type;
    Update();
}

wstring EmailAddress::GetCustomType()
{
    return _customType;
}

void EmailAddress::SetCustomType(wstring customType)
{
    _customType = customType;
    if (!_customType.empty())
        _type = (EmailAddressType_enum)(_type | EmailAddress_Custom);
    else
        _type = (EmailAddressType_enum)(_type & ~EmailAddress_Custom);

    Update();
}

wstring EmailAddress::GetEmailAddress()
{
    return _emailAddress;
}

void EmailAddress::SetEmailAddress(wstring emailAddress)
{
    _emailAddress = emailAddress;
    Update();
}

//#endif