#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/OrganizationEx.h"
#include "Module/Contact/vCard/Implementation/vItem.h"

wstring OrganizationEx::ORG_Home    = L"Home";
wstring OrganizationEx::ORG_Work    = L"Work";
wstring OrganizationEx::ORG_Other   = L"Other";
wstring OrganizationEx::ORG_Custom  = L"Custom";

OrganizationEx::OrganizationEx()
{
}

OrganizationEx::~OrganizationEx()
{
}

void OrganizationEx::Init(Item* pItem)
{
    CustomItem::Init(pItem);
    RETURN_IF(pItem == NULL || pItem->GetValue().empty());

    vector<wstring> sArray = vCard::SplitLine(pItem->GetValue(), L';', 2);
    if (sArray.size() >= 1)
        _title = CStrOperation::ReplaceW(sArray[0], L"\\;", L";");
    if (sArray.size() >= 2)
        _location = CStrOperation::ReplaceW(sArray[0], L"\\;", L";");
}


//#endif