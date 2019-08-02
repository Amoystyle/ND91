#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/PhoneNumber.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"

PhoneNumber::PhoneNumber(Item* pItem, 
                         PhoneNumberType_enum type, 
                         wstring customType, 
                         wstring number)
{
    _pItem      = pItem;
    _type       = type;
    _customType = customType;
    _number     = number;

    RETURN_IF((_type & PhoneNumber_Custom) != PhoneNumber_Custom);
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

PhoneNumber::~PhoneNumber()
{
}

void PhoneNumber::Update()
{
    _pItem->SetName(TelecommunicationsAddressingTypes::TEL);
    _pItem->SetParameters(PhoneTypeToString(_type, _customType));
    _pItem->SetValue(_number);
}

wstring PhoneNumber::PhoneTypeToString(PhoneNumberType_enum type, wstring customType)
{
    wstring retVal;
    if ((type & PhoneNumber_BBS) != 0)
        retVal += L"BBS" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Car) != 0)
        retVal += L"CAR" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Cellular) != 0)
        retVal += L"CELL" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Fax) != 0)
        retVal += L"FAX" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Home) != 0)
        retVal += L"HOME" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_ISDN) != 0)
        retVal += L"ISDN" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Modem) != 0)
        retVal += L"MODEM" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Msg) != 0)
        retVal += L"MSG" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Pager) != 0)
        retVal += L"PAGER" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_PCS) != 0)
        retVal += L"PCS" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Preferred) != 0)
        retVal += L"PREF" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Video) != 0)
        retVal += L"VIDEO" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Voice) != 0)
        retVal += L"VOICE" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Work) != 0)
        retVal += L"WORK" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_CallbackTel) != 0)
        retVal += L"CALLBACKTEL" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Company) != 0)
        retVal += L"COMPANY" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Main) != 0)
        retVal += L"MAIN" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Other) != 0)
        retVal += L"OTHER" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Radio) != 0)
        retVal += L"RADIO" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Telex ) != 0)
        retVal += L"TELEX" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_TtyTdd) != 0)
        retVal += L"TTYTDD" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_Assistant ) != 0)
        retVal += L"ASSISTANT" + vCard::Name_Param_Splitor;
    if ((type & PhoneNumber_MMS) != 0)
        retVal += L"MMS" + vCard::Name_Param_Splitor;

    if (!customType.empty())
        retVal += vCard::CUSTOM_TAG + customType + vCard::Name_Param_Splitor;

    if (CStrOperation::endsWithW(retVal, vCard::Name_Param_Splitor))
        retVal = retVal.substr(0, retVal.length()-1);

    return retVal;
}

PhoneNumber* PhoneNumber::Parse(Item* pItem)
{
    PhoneNumberType_enum type = PhoneNumber_NotSpecified;
    wstring temp = CStrOperation::toUpperW(pItem->GetParameters());
    if (temp.find(L"PREF") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Preferred);
    if (temp.find(L"HOME") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Home);
    if (temp.find(L"MSG") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Msg);
    if (temp.find(L"WORK") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Work);
    if (temp.find(L"VOICE") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Voice);
    if (temp.find(L"FAX") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Fax);
    if (temp.find(L"CELL") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Cellular);
    if (temp.find(L"VIDEO") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Video);
    if (temp.find(L"PAGER") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Pager);
    if (temp.find(L"BBS") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_BBS);
    if (temp.find(L"MODEM") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Modem);
    if (temp.find(L"CAR") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Car);
    if (temp.find(L"ISDN") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_ISDN);
    if (temp.find(L"PCS") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_PCS);
    if (temp.find(L"CALLBACKTEL") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_CallbackTel);
    if (temp.find(L"COMPANY") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Company);
    if (temp.find(L"MAIN") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Main);
    if (temp.find(L"OTHER") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Other);
    if (temp.find(L"RADIO") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Radio);
    if (temp.find(L"TELEX") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Telex);
    if (temp.find(L"TTYTDD") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_TtyTdd);
    if (temp.find(L"ASSISTANT") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Assistant);
    if (temp.find(L"MMS") != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_MMS);
    if (temp.find(vCard::CUSTOM_TAG) != wstring::npos)
        type = (PhoneNumberType_enum)(type | PhoneNumber_Custom);

    return new PhoneNumber(pItem, type, L"", pItem->GetValue());
}

bool PhoneNumber::IsPrimary()
{
    if ((_type & PhoneNumber_Preferred) == PhoneNumber_Preferred)
        return true;
    return false;
}

Item* PhoneNumber::GetItem() 
{ 
    return _pItem; 
}

PhoneNumberType_enum PhoneNumber::GetType()
{
    return _type;
}

void PhoneNumber::SetType(PhoneNumberType_enum type)
{
    _type = type;
    Update();
}

wstring PhoneNumber::GetCustomType()
{
    return _customType;
}

void PhoneNumber::SetCustomType(wstring customType)
{
    _customType = customType;
    if (!_customType.empty())
        _type = (PhoneNumberType_enum)(_type | PhoneNumber_Custom);
    else
        _type = (PhoneNumberType_enum)(_type & ~PhoneNumber_Custom);

    Update();
}

wstring PhoneNumber::GetNumber()
{
    return _number;
}

void PhoneNumber::SetNumber(wstring number)
{
    _number = number;
    Update();
}

//#endif