#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/IMAddress.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/ContactData.h"

wstring IMAddress::IMAddress_QQ             = L"QQ";
wstring IMAddress::IMAddress_MSN            = L"MSN";
wstring IMAddress::IMAddress_GOOLETALK      = L"GoogleTalk";
wstring IMAddress::IMAddress_WINDOWSLIVE    = L"WindowsLive";
wstring IMAddress::IMAddress_YAHOO          = L"Yahoo";
wstring IMAddress::IMAddress_SKYPE          = L"Skype";
wstring IMAddress::IMAddress_AIM            = L"AIM";
wstring IMAddress::IMAddress_ICQ            = L"ICQ";
wstring IMAddress::IMAddress_JABBER         = L"Jabber";
wstring IMAddress::IMAddress_FETION         = L"Fetion";

IMAddress::IMAddress()
{
    _type = IMType_NotSpecified;
}

IMAddress::~IMAddress()
{
}

void IMAddress::Init(Item* pItem)
{
    CustomItem::Init(pItem);
    RETURN_IF(pItem == NULL || pItem->GetValue().empty());

    vector<wstring> sArray = vCard::SplitLine(pItem->GetParameters(), L';', 3);
    if (sArray.size() >= 1)
        _label = CStrOperation::ReplaceW(sArray[0], L"\\;", L";");
    RETURN_IF(sArray.size() < 2);

    wstring imType = CStrOperation::toUpperW(CStrOperation::ReplaceW(sArray[1], L"\\;", L";"));
    if (imType.find(L"OTHER") != -1)
        _type = (IMType_enum)(_type | IMType_OTHER);
    if (imType.find(L"WORK") != -1)
        _type = (IMType_enum)(_type | IMType_WORK);
    if (imType.find(L"HOME") != -1)
        _type = (IMType_enum)(_type | IMType_HOME);
    if (imType.find(L"CUSTOM") != -1)
    {
        _type = (IMType_enum)(_type | IMType_CUSTOM);
        _customName = sArray[2];
    }
}

wstring IMAddress::IMLabelConvert(IMTypesName imlabelSource)
{
    wstring imLabelResult;
    switch (imlabelSource)
    {
    case IM_ATM:
        imLabelResult = IMAddress_AIM;
        break;
    case IM_YAHOO:
        imLabelResult = IMAddress_YAHOO;
        break;
    case IM_MSN:
        imLabelResult = IMAddress_MSN;
        break;
    case IM_ICQ:
        imLabelResult = IMAddress_ICQ;
        break;
    case IM_JABBER:
        imLabelResult = IMAddress_JABBER;
        break;
    case IM_WINDOWSLIVE:
        imLabelResult = IMAddress_WINDOWSLIVE;
        break;
    case IM_SKYPE:
        imLabelResult = IMAddress_SKYPE;
        break;
    case IM_FETION:
        imLabelResult = IMAddress_FETION;
        break;
    case IM_GOOLETALK:
        imLabelResult = IMAddress_GOOLETALK;
        break;
    case IM_QQ:
        imLabelResult = IMAddress_QQ;
        break;
    default:
        imLabelResult = L"Custom";
        break;
    }
    return imLabelResult;
}

IMTypesName IMAddress::IMLabelConvert(wstring label)
{
    if (label == IMAddress_AIM)
        return IM_ATM;
    else if (label == IMAddress_YAHOO)
        return IM_YAHOO;
    else if (label == IMAddress_MSN)
        return IM_MSN;
    else if (label == IMAddress_ICQ)
        return IM_ICQ;
    else if (label == IMAddress_JABBER)
        return IM_JABBER;
    else if (label == IMAddress_WINDOWSLIVE)
        return IM_WINDOWSLIVE;
    else if (label == IMAddress_SKYPE)
        return IM_SKYPE;
    else if (label == IMAddress_FETION)
        return IM_FETION;
    else if (label == IMAddress_GOOLETALK)
        return IM_GOOLETALK;
    else if (label == IMAddress_QQ)
        return IM_QQ;
    else
        return IM_CUSTOMSERVICE;
}


//#endif