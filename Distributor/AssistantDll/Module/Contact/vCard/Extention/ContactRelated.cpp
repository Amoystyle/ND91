#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/ContactRelated.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/ContactData.h"

wstring ContactRelated::Related_Parent       = L"Parent";
wstring ContactRelated::Related_Brother      = L"Brother";
wstring ContactRelated::Related_Sister       = L"Sister";
wstring ContactRelated::Related_Mother       = L"Mother";
wstring ContactRelated::Related_Father       = L"Father";
wstring ContactRelated::Related_Child        = L"Child";
wstring ContactRelated::Related_Friend       = L"Friend";
wstring ContactRelated::Related_Spouse       = L"Spouse";
wstring ContactRelated::Related_Partner      = L"Partner";
wstring ContactRelated::Related_Assistant    = L"Assistant";
wstring ContactRelated::Related_Manager      = L"Manager";
wstring ContactRelated::Related_Custom       = L"Custom";
wstring ContactRelated::Related_Other        = L"Other";

ContactRelated::ContactRelated()
{
    _type = RelatedKind_Parent;
}

ContactRelated::~ContactRelated()
{
}

void ContactRelated::Init(Item* pItem)
{
    CustomItem::Init(pItem);
    RETURN_IF(pItem == NULL || pItem->GetValue().empty());
    
    vector<wstring> sArray = vCard::SplitLine(pItem->GetParameters(), L';', 3);
    if (sArray.size() >= 1)
        _label = CStrOperation::ReplaceW(sArray[0], L"\\;", L";");
    RETURN_IF(sArray.size() < 2);

    wstring label = CStrOperation::toLowerW(CStrOperation::ReplaceW(sArray[1], L"\\;", L";"));
    if (label == L"parent")
        _type = RelatedKind_Parent;
    else if (label == L"brother")
        _type = RelatedKind_Brother;
    else if (label == L"sister")
        _type = RelatedKind_Sister;
    else if (label == L"mother")
        _type = RelatedKind_Mother;
    else if (label == L"father")
        _type = RelatedKind_Father;
    else if (label == L"child")
        _type = RelatedKind_Child;
    else if (label == L"friend")
        _type = RelatedKind_Friend;
    else if (label == L"spouse")
        _type = RelatedKind_Spouse;
    else if (label == L"partner")
        _type = RelatedKind_Partner;
    else if (label == L"assistant")
        _type = RelatedKind_Assistant;
    else if (label == L"manager")
        _type = RelatedKind_Manager;
    else if (label == L"other")
        _type = RelatedKind_Other;
    else if (label == L"custom")
        _type = RelatedKind_Custom;
}

wstring ContactRelated::RelatedLabelConvert(RelatedTypes imlabelSource)
{
    wstring RelatedLabelResult;
    switch (imlabelSource)
    {
    case RELATED_FATHER:
        RelatedLabelResult = Related_Father;
        break;
    case RELATED_MOTHER:
        RelatedLabelResult = Related_Mother;
        break;
    case RELATED_PARENT:
        RelatedLabelResult = Related_Partner;
        break;
    case RELATED_BROTHER:
        RelatedLabelResult = Related_Brother;
        break;
    case RELATED_SISTER:
        RelatedLabelResult = Related_Sister;
        break;
    case RELATED_CHILD:
        RelatedLabelResult = Related_Child;
        break;
    case RELATED_FRIEND:
        RelatedLabelResult = Related_Friend;
        break;
    case RELATED_SPOUSE:
        RelatedLabelResult = Related_Spouse;
        break;
    case RELATED_PARTNER:
        RelatedLabelResult = Related_Partner;
        break;
    case RELATED_ASSISTANT:
        RelatedLabelResult = Related_Assistant;
        break;
    case RELATED_MANAGER:
        RelatedLabelResult = Related_Manager;
        break;
    case RELATED_OTHER:
        RelatedLabelResult = Related_Other;
        break;
    case RELATED_CUSTOM:
        RelatedLabelResult = Related_Custom;
        break;
    default:
        break;
    }
    return RelatedLabelResult;
}

//#endif