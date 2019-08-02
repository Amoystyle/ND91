#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/DeliveryAddress.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"

DeliveryAddress::DeliveryAddress(Item* pItem, 
                                 DeliveryAddressType_enum type, 
                                 wstring customType, 
                                 wstring postOfficeAddress, 
                                 wstring extendedAddress, 
                                 wstring street, 
                                 wstring locality, 
                                 wstring region, 
                                 wstring postalCode, 
                                 wstring country,
                                 wstring countryCode)
{
    _pItem              = pItem;
    _type               = type;
    _customType         = customType;
    _postOfficeAddress  = postOfficeAddress;
    _extendedAddress    = extendedAddress;
    _street             = street;
    _locality           = locality;
    _region             = region;
    _postalCode         = postalCode;
    _country            = country;
    _countryCode        = countryCode;

    RETURN_IF((_type & DeliveryAddress_Custom) != DeliveryAddress_Custom);
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

DeliveryAddress::~DeliveryAddress()
{
}

void DeliveryAddress::Update()
{
    wstring temp;
    temp+=_postOfficeAddress+L";";
    temp+=_extendedAddress+L";";
    temp+=_street+L";";
    temp+=_locality+L";";
    temp+=_region+L";";
    temp+=_postalCode+L";";
    temp+=_country+L";";
    temp+=_countryCode;

    _pItem->SetParameters(AddressTypeToString(_type, _customType));
    _pItem->SetValue(temp);
}

wstring DeliveryAddress::AddressTypeToString(DeliveryAddressType_enum type, wstring customType)
{
    wstring retVal;
    if ((type & DeliveryAddress_Domestic) != 0)
        retVal += L"DOM" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Home) != 0)
        retVal += L"HOME" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Ineternational) != 0)
        retVal += L"INTL" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Parcel) != 0)
        retVal += L"PARCEL" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Postal) != 0)
        retVal += L"POSTAL" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Preferred) != 0)
        retVal += L"Preferred" + vCard::Name_Param_Splitor;
    if ((type & DeliveryAddress_Work) != 0)
        retVal += L"WORK" + vCard::Name_Param_Splitor;
    if (type == 0)
        retVal += L"OTHER" + vCard::Name_Param_Splitor;

    if (!customType.empty())
        retVal += vCard::CUSTOM_TAG + customType + vCard::Name_Param_Splitor;

    if (CStrOperation::endsWithW(retVal, vCard::Name_Param_Splitor))
        retVal = retVal.substr(0, retVal.length()-1);

    return retVal;
}

DeliveryAddress* DeliveryAddress::Parse(Item* pItem)
{
    DeliveryAddressType_enum type = DeliveryAddress_NotSpecified;
    wstring temp = CStrOperation::toUpperW(pItem->GetParameters());
    if (temp.find(L"PREF") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Preferred);

    if (temp.find(L"DOM") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Domestic);

    if (temp.find(L"INTL") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Ineternational);

    if (temp.find(L"POSTAL") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Postal);

    if (temp.find(L"PARCEL") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Parcel);

    if (temp.find(L"HOME") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Home);

    if (temp.find(L"WORK") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Work);

    if (temp.find(L"OTHER") != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_NotSpecified);

    if (temp.find(vCard::CUSTOM_TAG) != wstring::npos)
        type = (DeliveryAddressType_enum)(type | DeliveryAddress_Custom);
    
    vector<wstring> items = vCard::SplitLine(pItem->GetValue(), L';', 8);
    vector<wstring> parameters;
    for (size_t i = 0; i < 8; i++)
    {
        if (i < items.size())
            parameters.push_back(CStrOperation::ReplaceW(items.at(i), L"\\;", L";"));
        else
            parameters.push_back(L"");
    }

    return new DeliveryAddress(pItem, type, L"", 
                               parameters.at(0),
                               parameters.at(1),
                               parameters.at(2),
                               parameters.at(3),
                               parameters.at(4),
                               parameters.at(5),
                               parameters.at(6),
                               parameters.at(7));
}

bool DeliveryAddress::IsPrimary()
{
    if ((_type & DeliveryAddress_Preferred) == DeliveryAddress_Preferred)
        return true;
    return false;
}

Item* DeliveryAddress::GetItem() 
{ 
    return _pItem; 
}

DeliveryAddressType_enum DeliveryAddress::GetType() 
{ 
    return _type; 
}

void DeliveryAddress::SetType(DeliveryAddressType_enum type) 
{ 
    _type = type; 
    Update();
}

wstring DeliveryAddress::GetCustomType() 
{ 
    return _customType; 
}

void DeliveryAddress::SetCustomType(wstring customType) 
{ 
    _customType = customType;
    if (!_customType.empty())
        _type = (DeliveryAddressType_enum)(_type | DeliveryAddress_Custom);
    else
        _type = (DeliveryAddressType_enum)(_type & ~DeliveryAddress_Custom);

    Update();
}

wstring DeliveryAddress::GetPostOfficeAddress() 
{ 
    return _postOfficeAddress; 
}

void DeliveryAddress::SetPostOfficeAddress(wstring postOfficeAddress) 
{ 
    _postOfficeAddress = postOfficeAddress; 
    Update();
}

wstring DeliveryAddress::GetExtendedAddress() 
{ 
    return _extendedAddress; 
}

void DeliveryAddress::SetExtendedAddress(wstring extendedAddress) 
{ 
    _extendedAddress = extendedAddress; 
    Update();
}

wstring DeliveryAddress::GetStreet() 
{ 
    return _street; 
}

void DeliveryAddress::SetStreet(wstring street) 
{ 
    _street = street; 
    Update(); 
}

wstring DeliveryAddress::GetLocality() 
{ 
    return _locality; 
}

void DeliveryAddress::SetLocality(wstring locality) 
{ 
    _locality = locality; 
    Update(); 
}

wstring DeliveryAddress::GetRegion() 
{ 
    return _region; 
}

void DeliveryAddress::SetRegion(wstring region) 
{ 
    _region = region; 
    Update(); 
}

wstring DeliveryAddress::GetPostalCode() 
{ 
    return _postalCode; 
}

void DeliveryAddress::SetpostalCode(wstring postalCode) 
{ 
    _postalCode = postalCode; 
    Update(); 
}

wstring DeliveryAddress::GetCountry() 
{ 
    return _country; 
}

void DeliveryAddress::SetCountry(wstring country) 
{ 
    _country = country; 
    Update(); 
}

wstring DeliveryAddress::GetCountryCode() 
{ 
    return _countryCode; 
}

void DeliveryAddress::SetCountryCode(wstring countryCode) 
{ 
    _countryCode = countryCode; 
    Update(); 
}


//#endif