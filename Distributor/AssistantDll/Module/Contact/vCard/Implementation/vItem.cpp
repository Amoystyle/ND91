#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/Util.h"

Item::Item(wstring name, wstring parameters, wstring value)
{
    _name       = name;
    _parameters = parameters;
    _value      = value;
}

Item::~Item()
{
}

bool Item::IsEmpty()
{
    if (_name.empty() && _parameters.empty() && _value.empty())
        return true;
    return false;
}

wstring Item::ToString()
{
    RETURN_VALUE_IF(IsEmpty(), L"");
    if (!_parameters.empty())
        return _name+L";"+_parameters+L":"+Util::FoldLine(_value);
    else
        return _name+L":"+Util::FoldLine(_value);
}


//#endif