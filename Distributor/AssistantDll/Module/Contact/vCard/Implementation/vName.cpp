#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/vName.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Implementation/ItemCollection.h"
#include "Module/Contact/vCard/vCard.h"

vName::vName(wstring lastName, 
           wstring firstName, 
           wstring middleName, 
           wstring honorificPrefix, 
           wstring honorificSuffix)
{
    _lastName           = lastName;
    _firstName          = firstName;
    _middleName         = middleName;
    _honorificPrefix    = honorificPrefix;
    _honorificSuffix    = honorificSuffix;
}

vName::vName(wstring lastName, 
           wstring firstName, 
           wstring middleName, 
           wstring honorificPrefix, 
           wstring honorificSuffix, 
           wstring lastPhonetic, 
           wstring firstPhonetic, 
           wstring middlePhonetic)
{
    _lastName           = lastName;
    _lastPhonetic       = lastPhonetic;
    _firstName          = firstName;
    _firstPhonetic      = firstPhonetic;
    _middleName         = middleName;
    _middlePhonetic     = middlePhonetic;
    _honorificPrefix    = honorificPrefix;
    _honorificSuffix    = honorificSuffix;
}

vName::~vName()
{
}

wstring vName::ToString()
{
    wstring res;
    res += _honorificPrefix+vCard::BlankSpace;
    res += _lastName+vCard::BlankSpace;

    if (!_middleName.empty())
        res += _middleName+vCard::BlankSpace;
    if (!_firstName.empty())
        res += _firstName+vCard::BlankSpace;

    res += _honorificSuffix;
    res = CStrOperation::trimLeftW(res, L" ");
    res = CStrOperation::trimRightW(res, L" ");
    return res;
}

wstring vName::ToValueString()
{
    wstring res;
    res += CStrOperation::ReplaceW(_lastName, L";", L"\\;")+vCard::Name_Param_Splitor;
    res += CStrOperation::ReplaceW(_firstName, L";", L"\\;")+vCard::Name_Param_Splitor;
    res += CStrOperation::ReplaceW(_middleName, L";", L"\\;")+vCard::Name_Param_Splitor;
    res += CStrOperation::ReplaceW(_honorificPrefix, L";", L"\\;")+vCard::Name_Param_Splitor;
	res += CStrOperation::ReplaceW(_honorificSuffix, L";", L"\\;")+vCard::Name_Param_Splitor;
	res += CStrOperation::ReplaceW(_lastPhonetic, L";", L"\\;")+vCard::Name_Param_Splitor;
	res += CStrOperation::ReplaceW(_firstPhonetic, L";", L"\\;")+vCard::Name_Param_Splitor;
	res += CStrOperation::ReplaceW(_middlePhonetic, L";", L"\\;");
    return res;
}

vName* vName::Parse(wstring value)
{
    vector<wstring> items = vCard::SplitLine(value, L';', 8);
    vector<wstring> nameArray;
    for (size_t i = 0; i < 8; i++)
    {
        if (i < items.size())
            nameArray.push_back(CStrOperation::ReplaceW(items.at(i), L"\\;", L";"));
        else
            nameArray.push_back(L"");
    }

    return new vName(nameArray.at(0),
                    nameArray.at(1),
                    nameArray.at(2),
                    nameArray.at(3),
                    nameArray.at(4),
                    nameArray.at(5),
                    nameArray.at(6),
                    nameArray.at(7));
}


vName* vName::Instance(wstring lastName, 
                     wstring firstName, 
                     wstring middleName, 
                     wstring honorificPrefix, 
                     wstring honorificSuffix, 
                     wstring lastPhonetic, 
                     wstring firstPhonetic, 
                     wstring middlePhonetic)
{
    return new vName(lastName, 
                    firstName, 
                    middleName, 
                    honorificPrefix, 
                    honorificSuffix, 
                    lastPhonetic, 
                    firstPhonetic, 
                    middlePhonetic);
}



//#endif